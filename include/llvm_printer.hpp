#pragma once

#include <iostream>
#include <unordered_map>
#include "detail/ivisitor.hpp"
#include "node.hpp"

// LLVM headers
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/BasicBlock.h"

#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

// JIT FOR FUTURE
#include "llvm/ExecutionEngine/Orc/LLJIT.h"

class LLVMPrinter : public Visitor {
private:
        llvm::LLVMContext& context;
        llvm::IRBuilder<> builder;
        std::unique_ptr<llvm::Module> module;
        std::unordered_map<std::string, llvm::Value*> namedValues;
        std::vector<std::unordered_map<std::string, llvm::AllocaInst*>> symbolScopes;

        std::unordered_map<std::string, llvm::Function*> functions;

        llvm::Value* curVal = nullptr;

public: // constuctor/destructor

    LLVMPrinter(llvm::LLVMContext& ctx,
                const std::string& moduleName = "paracl_module"
                /*const std::string& targetTriple */)
    : context(ctx),
      builder(ctx),
      module(std::make_unique<llvm::Module>(moduleName, ctx))
    {
        //TODO
        // module->setTargetTriple(targetTriple);
        // module->setDataLayout(llvm::DataLayout::getDefaultDataLayout(targetTriple));

        pushScope(); // creating global scope
    }

    ~LLVMPrinter() {
        popScope(); // deleting global scope
    }
    void Visit(AST::ConstantNode& n) override {
        curVal = llvm::ConstantInt::get(
            context,
            llvm::APInt(32, n.get_val(), true)
        );
    }

public: // visitors

    void Visit(AST::VariableNode& n) override {
        llvm::AllocaInst* allocated_val = nullptr;

        for (auto&& it = symbolScopes.rbegin(), end = symbolScopes.rend(); it != end; ++it) {
            auto&& found = it->find(std::string(n.get_name()));
            if (found != it->end()) {
                allocated_val = found->second;
                break;
            }

        }

        if (!allocated_val) {
            throw std::runtime_error("Unknown value: " + std::string(n.get_name()) + ":(");
        }

        curVal = builder.CreateLoad( // <-----------------------------------------------------------]
            allocated_val->getAllocatedType(),                                              //      |
            allocated_val,                                                                  //      |
            n.get_name() //NOTE - issue with ssa-form?                                      //      |
        );                                                                                  //      |
    }                                                                                       //      |
                                                                                            //      |
    void Visit(AST::AssignNode& n) override {                                               //      |
        n.get_expr()->accept(*this);                                                        //      |
        auto&& expr = curVal;                                                               //      |
                                                                                            //      |
        n.get_dest()->accept(*this); // <- here curVal gets a pointer to our value created here   ->|
        curVal = expr;
    }

    void Visit(AST::IfNode& n) override {
        llvm::Function* curFunc = builder.GetInsertBlock()->getParent();

        // creating if diamond //
        llvm::BasicBlock* thenBB = llvm::BasicBlock::Create(context, "if.then", curFunc);
        llvm::BasicBlock* elseBB = llvm::BasicBlock::Create(context, "if.else", curFunc);
        llvm::BasicBlock* mergeBB = llvm::BasicBlock::Create(context, "if.merge", curFunc);

        // creating condition
        n.get_cond()->accept(*this);
        llvm::Value* condValue = curVal;
        builder.CreateCondBr(condValue, thenBB, elseBB);

        // generating code in thenBB section (don't forget to set insert point)
        builder.SetInsertPoint(thenBB);
        n.get_action()->accept(*this);
        builder.CreateBr(mergeBB); // unconditional jump to merge bb.

        // generating `br` for else even if there is no `else` condition
        // the block will be considered as dead code in llvm ir optimizations and deleted
        // that's why i decided to create else basic block in any case
        //NOTE - what can be done with multiple `else if` conditions ???

        builder.SetInsertPoint(elseBB);
        if (n.get_else_action()) {
            // generating code in elseBB
            n.get_else_action()->accept(*this);
        }
        builder.CreateBr(mergeBB); // unconditional jump to merge bb.
        builder.SetInsertPoint(mergeBB); // <- setting insert point (important!)
    }


    void Visit(AST::BinaryOpNode& n) override {
        n.get_left()->accept(*this);
        auto&& left = curVal;

        n.get_right()->accept(*this);
        auto&& right = curVal;

        curVal = createLLVMBinaryOp(n.get_op(), left, right);
    }


    void Visit(AST::ForNode& n) override {
        llvm::Function* curFunc = builder.GetInsertBlock()->getParent();

        // creating for diamond // i manually copied it from llvm ir c for translation (all credits to godbolt) ;)
        llvm::BasicBlock* initBB = llvm::BasicBlock::Create(context, "for.init", curFunc);
        llvm::BasicBlock* condBB = llvm::BasicBlock::Create(context, "for.cond", curFunc);
        llvm::BasicBlock* bodyBB = llvm::BasicBlock::Create(context, "for.body", curFunc);
        llvm::BasicBlock* incBB = llvm::BasicBlock::Create(context, "for.inc", curFunc);
        llvm::BasicBlock* endBB = llvm::BasicBlock::Create(context, "for.end", curFunc);

        n.get_init()->accept(*this);
        builder.CreateBr(condBB);

        builder.SetInsertPoint(condBB);
        n.get_cond()->accept(*this);
        llvm::Value* condValue = curVal;
        builder.CreateCondBr(condValue, bodyBB, endBB);

        builder.SetInsertPoint(bodyBB);
        n.get_body()->accept(*this);
        builder.CreateBr(incBB);

        builder.SetInsertPoint(incBB);
        n.get_iter()->accept(*this);
        builder.CreateBr(condBB);

        builder.SetInsertPoint(endBB);
    }

    void Visit(AST::CallNode& n) override {
        n.get_callee()->accept(*this);
        llvm::Value* calleeValue = curVal;

        std::vector<llvm::Value*> values;
        for (auto&& arg : n.get_args()) {
            arg->accept(*this);
            values.push_back(curVal);
        }

        //NOTE - dyn_cast - debatable
        llvm::Function* func = llvm::dyn_cast<llvm::Function>(calleeValue);
        if (!func) {
            //NOTE - is it the best path to throw an exception ?
            throw std::runtime_error("Can't dyn_cast calleeValue to llvm::Function");
            return;
        }

        llvm::CallInst* call = builder.CreateCall(func, values);
        curVal = call;
    }

    void Visit(AST::ScopeNode& n) override {

        pushScope();

        for (auto&& stmt : n.get_children()) {
            stmt->accept(*this);
        }

        popScope();

    }

    void Visit(AST::InNode& n) override {

        llvm::FunctionType* inType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(context),
        {},
        false // no varagrs
        );

        auto* func = getOrCreateParaLibFunc("paracl_in", inType);
        auto* val = builder.CreateCall(func, {});
        curVal = val;

    }

    void Visit(AST::PrintNode& n) override {

        n.get_expr()->accept(*this);


        llvm::FunctionType* printType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(context),
        {llvm::Type::getInt32Ty(context)},
        false // no varagrs
        );

        auto* func = getOrCreateParaLibFunc("paracl_print", printType);
        auto* val = builder.CreateCall(func, curVal);

    }

    void Visit(AST::ExpressionNode& n)              override {}
    void Visit(AST::StatementNode& n)               override {}
    void Visit(AST::ConditionalStatementNode& n)    override {}

public: // helper methods
    void pushScope() {
        symbolScopes.emplace_back();
    }

    void popScope() {
        if (!symbolScopes.empty()) {
            symbolScopes.pop_back();
        }
    }

    llvm::Function* getOrCreateParaLibFunc(const std::string& name, llvm::FunctionType* type) {
        if (auto* func = module->getFunction(name)) {
            return func;
        }
        return llvm::Function::Create(
            type,
            llvm::Function::ExternalLinkage,  // <-- Важно!
            name,
            module.get()
        );
    }


    //NOTE - breaking srp maybe (sorry)
    llvm::Value* createLLVMBinaryOp(const AST::BinaryOp& op, llvm::Value* left, llvm::Value* right) {
        llvm::Value* curVal = nullptr;
        switch (op) {
            case AST::BinaryOp::ADD:
                curVal = builder.CreateAdd(left, right, "add");
                break;

            case AST::BinaryOp::SUB:
                curVal = builder.CreateSub(left, right, "sub");
                break;

            case AST::BinaryOp::AND:
                // NOTE think about branch prediction here ?
                curVal = builder.CreateAnd(left, right, "and");
                break;

            case AST::BinaryOp::BIT_AND:
                curVal = builder.CreateAnd(left, right, "bit_and");
                break;

            case AST::BinaryOp::OR:
                // NOTE branch prediction ;)
                curVal = builder.CreateOr(left, right, "or");
                break;

            case AST::BinaryOp::BIT_OR:
                curVal = builder.CreateOr(left, right, "bit_or");
                break;

            case AST::BinaryOp::DIV:
                curVal = builder.CreateSDiv(left, right, "sdiv");
                break;

            case AST::BinaryOp::EQ:
                curVal = builder.CreateICmpEQ(left, right, "eq");
                break;

            case AST::BinaryOp::GR:
                curVal = builder.CreateICmpSGT(left, right, "sgt");
                break;

            case AST::BinaryOp::GR_EQ:
                curVal = builder.CreateICmpSGE(left, right, "sge");
                break;

            case AST::BinaryOp::LS:
                curVal = builder.CreateICmpSLT(left, right, "slt");
                break;

            case AST::BinaryOp::LS_EQ:
                curVal = builder.CreateICmpSLE(left, right, "sle");
                break;

            case AST::BinaryOp::MOD:
                curVal = builder.CreateSRem(left, right, "srem");
                break;

            case AST::BinaryOp::MUL:
                curVal = builder.CreateMul(left, right, "mul");
                break;

            case AST::BinaryOp::NOT_EQ:
                curVal = builder.CreateICmpNE(left, right, "ne");
                break;

            default:
                throw std::runtime_error("Undefined binary op :(");
        }
        return curVal;
    }
};
