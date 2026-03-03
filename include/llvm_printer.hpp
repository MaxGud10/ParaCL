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

    void Visit(const AST::ConstantNode& n) const override {
        auto&& currentValue = llvm::ConstantInt::get(
            context,
            llvm::APInt(32, n.get_val(), true);
        );
    }

    void Visit(const AST::VariableNode& n) const override {
        llvm::AllocaInst* allocated_val = nullptr;

        for (auto&& it = symbolScopes.rbegin(), end = symbolScopes.rend(); it != end; ++it) {
            auto&& found = it->find(n.get_name());
            if (found != it->end()) {
                allocated_val = found->second;
                break;
            }

        }

        if (!allocated_value) {
            throw std::runtime_error("Unknown value:" + n.get_name() + ":(");
        }

        curVal = builder.CreateLoad( // <-----------------------------------------------------------]
            allocated_val->getAllocatedType(),                                              //      |
            allocated_val,                                                                  //      |
            n.get_name()                                                                    //      |
        );                                                                                  //      |
    }                                                                                       //      |
                                                                                            //      |
    void Visit(const AST::AssignNode& n) const override {                                   //      |
        n.get_expr()->accept(*this);                                                        //      |
        auto&& expr = curVal;                                                               //      |
                                                                                            //      |
        n.get_dest()->accept(*this); // <- here curVal gets a pointer to our value created here  ->|
        curVal = expr;
    }

    void Visit(const AST::ExpressionNode& n) const override {
        n.accept(*this);
    }

    void Visit(const AST::StatementNode& n) const override {
        n.accept(*this);
    }

    void Visit(const AST::ConditionalStatementNode& n) const override {
        n.accept(*this);
    }

    void Visit(const AST::IfNode& n) const override {
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


    void Visit(const AST::BinaryOpNode& n) const override {
        n.get_left()->accept(*this);
        auto&& left = curVal;

        n.get_right()->accept(*this);
        auto&& right = curVal;

        switch (n.get_op()) {
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
                curVal = builder.CreateAnd(left, right, "bit_and")
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
                throw std::runtime_error("Undefined binary op" + n.get_op());

        }
    }

};
