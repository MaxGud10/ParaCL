#pragma once

#include <iostream>
#include <unordered_map>
#include "detail/ivisitor.hpp"
#include "node.hpp"
#include "paraio.hpp"

#include "log.h"

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

class LLVMPrinter : public Visitor
{
private:
        llvm::LLVMContext&            context;
        llvm::IRBuilder<>             builder;
        std::unique_ptr<llvm::Module> module;

        // std::unordered_map<std::string, llvm::Value*>                   namedValues;
        std::vector<std::unordered_map<std::string, llvm::AllocaInst*>> symbolScopes;

        std::unordered_map<std::string, llvm::Function*>                       functions;
        std::unordered_map<const AST::FunctionNode*, std::vector<std::string>> functionCaptures_;
        std::vector<std::unordered_map<std::string, llvm::Value*>>             captureScopes_;

        llvm::Function*   currentFunction          = nullptr;
        llvm::Function*   paraclPrintFn            = nullptr;
        llvm::Function*   paraclInFn               = nullptr;
        llvm::Function*   paraclMakeIntFn          = nullptr;
        llvm::Function*   paraclValueAsIntFn       = nullptr;
        llvm::Function*   paraclInValueFn          = nullptr;
        llvm::Function*   paraclPrintValueFn       = nullptr;
        llvm::Function*   paraclMakeClosureFn      = nullptr;
        llvm::Function*   paraclMakeClosureValueFn = nullptr;
        llvm::Function*   paraclValueAsClosureFn   = nullptr;
        llvm::Function*   paraclCallClosureFn      = nullptr;
        llvm::Function*   paraclAllocEnvFn         = nullptr;
        llvm::Function*   paraclClosureGetEnvFn    = nullptr;
        llvm::StructType* paraValueTy             = nullptr;

        llvm::Value* curVal = nullptr;

        size_t currentFunctionScopeBase_ = 0;

private:
    void dbg(const std::string& msg) const
    {
        LLVM_MSGLN("[LLVMPrinter] " << msg);
    }

    void dbgValue(const char* label, llvm::Value* value) const
    {
        LLVM_MSG("[LLVMPrinter] " << label << ": ");
        if (!value)
        {
            LLVM_MSGLN("<null>");
            return;
        }

        LLVM_PRINT(value);
        LLVM_MSG(" | type=");
        LLVM_PRINT(value->getType());
        LLVM_MSG("\n");
    }

    void dbgBlock(const char* label) const
    {
        LLVM_MSG("[LLVMPrinter] " << label << ": ");
        if (!builder.GetInsertBlock())
        {
            LLVM_MSGLN("<no insert block>");
            return;
        }

        LLVM_MSGLN(builder.GetInsertBlock()->getName());
    }

    void dbgFunctionIR(llvm::Function* fn) const
    {
        LLVM_MSGLN("");
        LLVM_MSG("[LLVMPrinter] function dump: " << fn->getName() << "\n");
        LLVM_PRINT(fn);
        LLVM_MSGLN("");
        LLVM_MSGLN("");
    }

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

    ~LLVMPrinter()
    {
        popScope(); // deleting global scope
    }

public:
    void generate(AST::AST& ast)
    {
        if (!ast.globalScope)
            throw std::runtime_error("LLVMPrinter::generate(): AST is empty");

        declareRuntimeFunctions();

        currentFunction = createMainFunction();

        llvm::BasicBlock* entryBB =
            llvm::BasicBlock::Create(context, "entry", currentFunction);
        builder.SetInsertPoint(entryBB);

        pushScope();

        ast.globalScope->accept(*this);

        popScope();

        if (!builder.GetInsertBlock()->getTerminator())
        {
            builder.CreateRet(llvm::ConstantInt::get(
                llvm::Type::getInt32Ty(context), 0, true));
        }

        LLVM_MSGLN("");
        LLVM_MSGLN("[LLVMPrinter] FULL MODULE BEFORE VERIFY");
        LLVM_PRINT(module.get());
        LLVM_MSGLN("");
        LLVM_MSGLN("");

        std::string funcError;
        llvm::raw_string_ostream funcErrorStream(funcError);
        if (llvm::verifyFunction(*currentFunction, &funcErrorStream))
        {
            throw std::runtime_error(
                "verifyFunction(main) failed:\n" + funcErrorStream.str());
        }

        std::string moduleError;
        llvm::raw_string_ostream moduleErrorStream(moduleError);
        if (llvm::verifyModule(*module, &moduleErrorStream))
        {
            throw std::runtime_error(
                "verifyModule failed:\n" + moduleErrorStream.str());
        }
    }

    void dump(llvm::raw_ostream& out) const
    {
        module->print(out, nullptr);
    }

    llvm::Module& getModule()
    {
        return *module;
    }

    const llvm::Module& getModule() const
    {
        return *module;
    }

private:
    void declareRuntimeFunctions()
    {
        llvm::StructType* paraValueTy = getParaValueType();

        llvm::FunctionType* makeIntType = llvm::FunctionType::get(
            paraValueTy,
            { llvm::Type::getInt32Ty(context) },
            false
        );

        llvm::FunctionType* valueAsIntType = llvm::FunctionType::get(
            llvm::Type::getInt32Ty(context),
            { paraValueTy },
            false
        );

        llvm::FunctionType* inValueType = llvm::FunctionType::get(
            paraValueTy,
            {},
            false
        );

        llvm::FunctionType* printValueType = llvm::FunctionType::get(
            llvm::Type::getVoidTy(context),
            { paraValueTy },
            false
        );

        paraclMakeIntFn = llvm::Function::Create(
            makeIntType,
            llvm::Function::ExternalLinkage,
            "paracl_int",
            module.get()
        );

        paraclValueAsIntFn = llvm::Function::Create(
            valueAsIntType,
            llvm::Function::ExternalLinkage,
            "paracl_as_int",
            module.get()
        );

        paraclInValueFn = llvm::Function::Create(
            inValueType,
            llvm::Function::ExternalLinkage,
            "paracl_in",
            module.get()
        );

        paraclPrintValueFn = llvm::Function::Create(
            printValueType,
            llvm::Function::ExternalLinkage,
            "paracl_print",
            module.get()
        );

        llvm::FunctionType* makeClosureType = llvm::FunctionType::get(
            getClosurePtrType(),
            { getVoidPtrType(), getVoidPtrType() }, // code ptr, env ptr
            false
        );

        llvm::FunctionType* makeClosureValueType = llvm::FunctionType::get(
            paraValueTy,
            { getClosurePtrType() },
            false
        );

        llvm::FunctionType* valueAsClosureType = llvm::FunctionType::get(
            getClosurePtrType(),
            { paraValueTy },
            false
        );

        llvm::FunctionType* callClosureType = llvm::FunctionType::get(
            paraValueTy,
            { getClosurePtrType(), llvm::PointerType::getUnqual(paraValueTy), llvm::Type::getInt32Ty(context) },
            false
        );

        llvm::FunctionType* allocEnvType = llvm::FunctionType::get(
            getVoidPtrType(),
            { llvm::Type::getInt64Ty(context) },
            false
        );

        paraclMakeClosureFn = llvm::Function::Create(
            makeClosureType,
            llvm::Function::ExternalLinkage,
            "paracl_closure",
            module.get()
        );

        paraclMakeClosureValueFn = llvm::Function::Create(
            makeClosureValueType,
            llvm::Function::ExternalLinkage,
            "paracl_closure_value",
            module.get()
        );

        paraclValueAsClosureFn = llvm::Function::Create(
            valueAsClosureType,
            llvm::Function::ExternalLinkage,
            "paracl_as_closure",
            module.get()
        );

        paraclCallClosureFn = llvm::Function::Create(
            callClosureType,
            llvm::Function::ExternalLinkage,
            "paracl_call",
            module.get()
        );

        paraclAllocEnvFn = llvm::Function::Create(
            allocEnvType,
            llvm::Function::ExternalLinkage,
            "paracl_env_alloc",
            module.get()
        );

        llvm::FunctionType* closureGetEnvType = llvm::FunctionType::get(
            getVoidPtrType(),
            { getClosurePtrType() },
            false
        );

        paraclClosureGetEnvFn = llvm::Function::Create(
            closureGetEnvType,
            llvm::Function::ExternalLinkage,
            "paracl_env",
            module.get()
        );
    }

    llvm::Function* createMainFunction()
    {
        llvm::FunctionType* mainType = llvm::FunctionType::get(
            llvm::Type::getInt32Ty(context),
            {},
            false
        );

        return llvm::Function::Create(
            mainType,
            llvm::Function::ExternalLinkage,
            "main",
            module.get()
        );
    }

    llvm::StructType* getParaValueType()
    {
        if (!paraValueTy)
        {
            paraValueTy = llvm::StructType::create(context, "struct.ParaValue");
            paraValueTy->setBody({
                llvm::Type::getInt32Ty(context),
                llvm::Type::getInt64Ty(context)
            });
        }
        return paraValueTy;
    }

    llvm::Type* getVoidPtrType()
    {
        return llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context));
    }

    llvm::Type* getClosurePtrType() // TODO: сделать по нормальному
    {
        return getVoidPtrType();
    }

    llvm::Type* getParaValuePtrType()
    {
        return llvm::PointerType::getUnqual(getParaValueType());
    }

    llvm::Function* createFunction(const std::string& name, size_t /*argCount*/)
    {
        llvm::StructType* paraValueTy = getParaValueType();

        llvm::FunctionType* type = llvm::FunctionType::get(
            paraValueTy,
            {
                getClosurePtrType(),                           // self
                llvm::PointerType::getUnqual(paraValueTy),     // args
                llvm::Type::getInt32Ty(context)                // argc
            },
            false
        );

        return llvm::Function::Create(
            type,
            llvm::Function::ExternalLinkage,
            name,
            module.get()
        );
    }

    static bool containsName(const std::vector<std::string_view>& names, const std::string& target)
    {
        for (auto n : names)
        {
            if (n == target)
                return true;
        }
        return false;
    }

    void pushScope()
    {
        symbolScopes.emplace_back();
    }

    void popScope()
    {
        if (symbolScopes.empty())
            throw std::runtime_error("LLVMPrinter::popScope(): no scope to pop");

        symbolScopes.pop_back();
    }

    llvm::AllocaInst* createEntryBlockAlloca(std::string_view name)
    {
        if (!currentFunction)
            throw std::runtime_error("createEntryBlockAlloca(): currentFunction is null");

        llvm::IRBuilder<> tmpBuilder(
            &currentFunction->getEntryBlock(),
            currentFunction->getEntryBlock().begin()
        );

        return tmpBuilder.CreateAlloca(
            getParaValueType(),
            nullptr,
            llvm::Twine(name)
        );
    }

    llvm::AllocaInst* lookupVariable(std::string_view name)
    {
        const std::string key(name);

        if (symbolScopes.empty())
            return nullptr;

        for (size_t i = symbolScopes.size(); i-- > currentFunctionScopeBase_;)
        {
            auto found = symbolScopes[i].find(key);
            if (found != symbolScopes[i].end())
                return found->second;
        }

        return nullptr;
    }

    llvm::Value* lookupCapture(std::string_view name)
    {
        const std::string key(name);

        for (auto it = captureScopes_.rbegin(); it != captureScopes_.rend(); ++it)
        {
            auto found = it->find(key);
            if (found != it->end())
                return found->second;
        }

        return nullptr;
    }

    llvm::AllocaInst* lookupOrCreateVariable(std::string_view name)
    {
        if (llvm::AllocaInst* existing = lookupVariable(name))
            return existing;

        if (symbolScopes.empty())
            throw std::runtime_error("lookupOrCreateVariable(): no active scope");

        llvm::AllocaInst* newAlloca = createEntryBlockAlloca(name);
        symbolScopes.back().emplace(std::string(name), newAlloca);
        return newAlloca;
    }

    std::vector<std::string> collectOuterVisibleNames(const AST::FunctionNode& n)
    {
        std::vector<std::string> result;

        if (symbolScopes.size() < 2)
            return result;

        for (size_t scopeIndex = 0; scopeIndex + 1 < symbolScopes.size(); ++scopeIndex)
        {
            for (const auto& [name, _] : symbolScopes[scopeIndex])
            {
                if (containsName(n.get_params(), name))
                    continue;

                if (n.has_internal_name() && name == std::string(n.get_internal_name()))
                    continue;

                if (std::find(result.begin(), result.end(), name) == result.end())
                    result.push_back(name);
            }
        }

        return result;
    }

    llvm::Value* toBool(llvm::Value* value)
    {
        if (!value)
            throw std::runtime_error("toBool(): null value");

        if (value->getType()->isIntegerTy(1))
            return value;

        if (value->getType()->isIntegerTy(32))
        {
            llvm::Value* zero = llvm::ConstantInt::get(
                llvm::Type::getInt32Ty(context), 0, true);
            return builder.CreateICmpNE(value, zero, "tobool");
        }

        throw std::runtime_error("toBool(): unsupported value type");
    }

    llvm::Value* toInt32Bool(llvm::Value* value)
    {
        if (!value)
            throw std::runtime_error("toInt32Bool(): null value");

        if (value->getType()->isIntegerTy(32))
            return value;

        if (value->getType()->isIntegerTy(1))
        {
            return builder.CreateZExt(
                value,
                llvm::Type::getInt32Ty(context),
                "booltoint"
            );
        }

        throw std::runtime_error("toInt32Bool(): unsupported value type");
    }

    llvm::Value* ensureInt32(llvm::Value* value)
    {
        if (!value)
            throw std::runtime_error("ensureInt32(): null value");

        if (value->getType()->isIntegerTy(32))
            return value;

        if (value->getType()->isIntegerTy(1))
            return toInt32Bool(value);

        throw std::runtime_error("ensureInt32(): unsupported value type");
    }

    llvm::Value* emitMakeIntValue(llvm::Value* intVal)
    {
        if (!intVal || !intVal->getType()->isIntegerTy(32))
            throw std::runtime_error("emitMakeIntValue(): expected i32");

        return builder.CreateCall(paraclMakeIntFn, { intVal }, "makeint");
    }

    llvm::Value* emitExtractInt(llvm::Value* paraValue)
    {
        if (!paraValue)
            throw std::runtime_error("emitExtractInt(): null value");

        return builder.CreateCall(paraclValueAsIntFn, { paraValue }, "asint");
    }

    llvm::Value* createLLVMBinaryOp(AST::BinaryOp op, llvm::Value* left, llvm::Value* right)
    {
        llvm::Value* leftInt  = emitExtractInt(left);
        llvm::Value* rightInt = emitExtractInt(right);

        llvm::Value* resultInt = nullptr;

        switch (op)
        {
            case AST::BinaryOp::ADD:
                resultInt = builder.CreateAdd(leftInt, rightInt, "add");
                break;

            case AST::BinaryOp::SUB:
                resultInt = builder.CreateSub(leftInt, rightInt, "sub");
                break;

            case AST::BinaryOp::MUL:
                resultInt = builder.CreateMul(leftInt, rightInt, "mul");
                break;

            case AST::BinaryOp::DIV:
                resultInt = builder.CreateSDiv(leftInt, rightInt, "sdiv");
                break;

            case AST::BinaryOp::MOD:
                resultInt = builder.CreateSRem(leftInt, rightInt, "srem");
                break;

            case AST::BinaryOp::EQ:
                resultInt = toInt32Bool(builder.CreateICmpEQ(leftInt, rightInt, "eq"));
                break;

            case AST::BinaryOp::NOT_EQ:
                resultInt = toInt32Bool(builder.CreateICmpNE(leftInt, rightInt, "ne"));
                break;

            case AST::BinaryOp::GR:
                resultInt = toInt32Bool(builder.CreateICmpSGT(leftInt, rightInt, "sgt"));
                break;

            case AST::BinaryOp::GR_EQ:
                resultInt = toInt32Bool(builder.CreateICmpSGE(leftInt, rightInt, "sge"));
                break;

            case AST::BinaryOp::LS:
                resultInt = toInt32Bool(builder.CreateICmpSLT(leftInt, rightInt, "slt"));
                break;

            case AST::BinaryOp::LS_EQ:
                resultInt = toInt32Bool(builder.CreateICmpSLE(leftInt, rightInt, "sle"));
                break;

            case AST::BinaryOp::BIT_AND:
                resultInt = builder.CreateAnd(leftInt, rightInt, "bitand");
                break;

            case AST::BinaryOp::BIT_OR:
                resultInt = builder.CreateOr(leftInt, rightInt, "bitor");
                break;

            default:
                throw std::runtime_error("createLLVMBinaryOp(): unsupported binary op");
        }

        return emitMakeIntValue(resultInt);
    }

    void generateLogicalOr(AST::BinaryOpNode& n)
    {
        llvm::Function* func = builder.GetInsertBlock()->getParent();

        n.get_left()->accept(*this);
        llvm::Value* leftInt  = emitExtractInt(curVal);
        llvm::Value* leftBool = toBool(leftInt);

        llvm::BasicBlock* trueBB  = llvm::BasicBlock::Create(context, "or.true", func);
        llvm::BasicBlock* rhsBB   = llvm::BasicBlock::Create(context, "or.rhs", func);
        llvm::BasicBlock* mergeBB = llvm::BasicBlock::Create(context, "or.merge", func);

        builder.CreateCondBr(leftBool, trueBB, rhsBB);

        builder.SetInsertPoint(trueBB);
        llvm::Value* trueResultInt = llvm::ConstantInt::get(
            llvm::Type::getInt32Ty(context), 1, true);
        llvm::Value* trueResult = emitMakeIntValue(trueResultInt);
        builder.CreateBr(mergeBB);
        llvm::BasicBlock* trueEndBB = builder.GetInsertBlock();

        builder.SetInsertPoint(rhsBB);
        n.get_right()->accept(*this);
        llvm::Value* rightInt    = emitExtractInt(curVal);
        llvm::Value* rightBool   = toBool(rightInt);
        llvm::Value* rightAsInt  = toInt32Bool(rightBool);
        llvm::Value* rightResult = emitMakeIntValue(rightAsInt);
        builder.CreateBr(mergeBB);
        llvm::BasicBlock* rhsEndBB = builder.GetInsertBlock();

        builder.SetInsertPoint(mergeBB);
        llvm::PHINode* phi = builder.CreatePHI(getParaValueType(), 2, "ortmp");
        phi->addIncoming(trueResult, trueEndBB);
        phi->addIncoming(rightResult, rhsEndBB);

        curVal = phi;
    }

    void generateLogicalAnd(AST::BinaryOpNode& n)
    {
        llvm::Function* func = builder.GetInsertBlock()->getParent();

        n.get_left()->accept(*this);
        llvm::Value* leftInt  = emitExtractInt(curVal);
        llvm::Value* leftBool = toBool(leftInt);

        llvm::BasicBlock* rhsBB   = llvm::BasicBlock::Create(context, "and.rhs", func);
        llvm::BasicBlock* falseBB = llvm::BasicBlock::Create(context, "and.false", func);
        llvm::BasicBlock* mergeBB = llvm::BasicBlock::Create(context, "and.merge", func);

        builder.CreateCondBr(leftBool, rhsBB, falseBB);

        builder.SetInsertPoint(falseBB);
        llvm::Value* falseResultInt = llvm::ConstantInt::get(
            llvm::Type::getInt32Ty(context), 0, true);
        llvm::Value* falseResult = emitMakeIntValue(falseResultInt);
        builder.CreateBr(mergeBB);
        llvm::BasicBlock* falseEndBB = builder.GetInsertBlock();

        builder.SetInsertPoint(rhsBB);
        n.get_right()->accept(*this);
        llvm::Value* rightInt    = emitExtractInt(curVal);
        llvm::Value* rightBool   = toBool(rightInt);
        llvm::Value* rightAsInt  = toInt32Bool(rightBool);
        llvm::Value* rightResult = emitMakeIntValue(rightAsInt);
        builder.CreateBr(mergeBB);
        llvm::BasicBlock* rhsEndBB = builder.GetInsertBlock();

        builder.SetInsertPoint(mergeBB);
        llvm::PHINode* phi = builder.CreatePHI(getParaValueType(), 2, "andtmp");
        phi->addIncoming(falseResult, falseEndBB);
        phi->addIncoming(rightResult, rhsEndBB);

        curVal = phi;
    }

public:
    void Visit(AST::ConstantNode& n) override
    {
        llvm::Value* rawInt = llvm::ConstantInt::get(
            llvm::Type::getInt32Ty(context),
            n.get_val(),
            true
        );

        curVal = emitMakeIntValue(rawInt);
    }

    void Visit(AST::VariableNode& n) override
    {
        if (llvm::AllocaInst* storage = lookupVariable(n.get_name()))
        {
            curVal = builder.CreateLoad(
                getParaValueType(),
                storage,
                llvm::Twine(n.get_name()));
            return;
        }

        if (llvm::Value* capturedPtr = lookupCapture(n.get_name()))
        {
            curVal = builder.CreateLoad(
                getParaValueType(),
                capturedPtr,
                llvm::Twine(n.get_name()) + ".captured");
            return;
        }

        throw std::runtime_error(
            "Unknown variable in codegen: " + std::string(n.get_name()));
    }

    void Visit(AST::AssignNode& n) override
    {
        dbg("Visit AssignNode -> " + std::string(n.get_dest()->get_name()));

        n.get_expr()->accept(*this);
        llvm::Value* rhs = curVal;

        dbgValue("Assign rhs before handling", rhs);

        if (!rhs)
            throw std::runtime_error("AssignNode: rhs is null");

        llvm::AllocaInst* storage = lookupOrCreateVariable(n.get_dest()->get_name());
        builder.CreateStore(rhs, storage);

        curVal = rhs;

        dbgValue("Assign final rhs", rhs);
    }

    void Visit(AST::UnaryOpNode& n) override
    {
        n.get_operand()->accept(*this);
        llvm::Value* operand = curVal;

        llvm::Value* operandInt = emitExtractInt(operand);
        llvm::Value* resultInt = nullptr;

        switch (n.get_op())
        {
            case AST::UnaryOp::NEG:
                resultInt = builder.CreateNeg(operandInt, "neg");
                break;

            case AST::UnaryOp::NOT:
            {
                llvm::Value* boolVal = toBool(operandInt);
                llvm::Value* notVal  = builder.CreateNot(boolVal, "not");
                resultInt = toInt32Bool(notVal);
                break;
            }

            default:
                throw std::runtime_error("Unsupported unary op in codegen");
        }

        curVal = emitMakeIntValue(resultInt);
    }

    void Visit(AST::BinaryOpNode& n) override
    {
        if (n.get_op() == AST::BinaryOp::AND)
        {
            generateLogicalAnd(n);
            return;
        }

        if (n.get_op() == AST::BinaryOp::OR)
        {
            generateLogicalOr(n);
            return;
        }

        n.get_left()->accept(*this);
        llvm::Value* left = curVal;

        n.get_right()->accept(*this);
        llvm::Value* right = curVal;

        dbgValue("Binary left", left);
        dbgValue("Binary right", right);

        curVal = createLLVMBinaryOp(n.get_op(), left, right);
        dbgValue("Binary result", curVal);
    }

    void Visit(AST::IfNode& n) override
    {
        dbg("Visit IfNode");
        dbgBlock("IfNode current block before cond");

        llvm::Value* zeroValue = emitMakeIntValue(
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0, true)
        );

        n.get_cond()->accept(*this);
        dbgValue("IfNode raw condition", curVal);

        llvm::Value* condInt  = emitExtractInt(curVal);
        llvm::Value* condBool = toBool(condInt);
        dbgValue("IfNode bool condition", condBool);

        llvm::Function* func = builder.GetInsertBlock()->getParent();

        llvm::BasicBlock* thenBB  = llvm::BasicBlock::Create(context, "if.then", func);
        llvm::BasicBlock* elseBB  = llvm::BasicBlock::Create(context, "if.else", func);
        llvm::BasicBlock* mergeBB = llvm::BasicBlock::Create(context, "if.merge", func);

        builder.CreateCondBr(condBool, thenBB, elseBB);

        builder.SetInsertPoint(thenBB);
        n.get_action()->accept(*this);
        llvm::Value* thenValue = curVal ? curVal : zeroValue;
        llvm::BasicBlock* thenEndBB = builder.GetInsertBlock();
        bool thenTerminated = (thenEndBB->getTerminator() != nullptr);
        if (!thenTerminated)
            builder.CreateBr(mergeBB);

        builder.SetInsertPoint(elseBB);
        llvm::Value* elseValue = zeroValue;
        if (n.get_else_action())
        {
            n.get_else_action()->accept(*this);
            elseValue = curVal ? curVal : zeroValue;
        }
        llvm::BasicBlock* elseEndBB = builder.GetInsertBlock();
        bool elseTerminated = (elseEndBB->getTerminator() != nullptr);
        if (!elseTerminated)
            builder.CreateBr(mergeBB);

        if (thenTerminated && elseTerminated)
        {
            curVal = zeroValue;
            return;
        }

        builder.SetInsertPoint(mergeBB);
        llvm::PHINode* phi = builder.CreatePHI(getParaValueType(), 2, "iftmp");

        if (!thenTerminated)
            phi->addIncoming(thenValue, thenEndBB);
        if (!elseTerminated)
            phi->addIncoming(elseValue, elseEndBB);

        curVal = phi;
        dbgValue("IfNode phi", phi);
    }


    void Visit(AST::WhileNode& n) override
    {
        llvm::Function* func = builder.GetInsertBlock()->getParent();

        llvm::BasicBlock* condBB = llvm::BasicBlock::Create(context, "while.cond", func);
        llvm::BasicBlock* bodyBB = llvm::BasicBlock::Create(context, "while.body", func);
        llvm::BasicBlock* endBB  = llvm::BasicBlock::Create(context, "while.end", func);

        if (!builder.GetInsertBlock()->getTerminator())
            builder.CreateBr(condBB);

        builder.SetInsertPoint(condBB);
        n.get_cond()->accept(*this);
        llvm::Value* condInt  = emitExtractInt(curVal);
        llvm::Value* condBool = toBool(condInt);
        builder.CreateCondBr(condBool, bodyBB, endBB);

        builder.SetInsertPoint(bodyBB);
        n.get_scope()->accept(*this);
        if (!builder.GetInsertBlock()->getTerminator())
            builder.CreateBr(condBB);

        builder.SetInsertPoint(endBB);
        curVal = emitMakeIntValue(
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0, true)
        );
    }

    void Visit(AST::ForNode& n) override
    {
        llvm::Function* curFunc = builder.GetInsertBlock()->getParent();

        llvm::BasicBlock* condBB = llvm::BasicBlock::Create(context, "for.cond", curFunc);
        llvm::BasicBlock* bodyBB = llvm::BasicBlock::Create(context, "for.body", curFunc);
        llvm::BasicBlock* incBB  = llvm::BasicBlock::Create(context, "for.inc", curFunc);
        llvm::BasicBlock* endBB  = llvm::BasicBlock::Create(context, "for.end", curFunc);

        n.get_init()->accept(*this);
        builder.CreateBr(condBB);

        builder.SetInsertPoint(condBB);
        n.get_cond()->accept(*this);
        llvm::Value* condInt  = emitExtractInt(curVal);
        llvm::Value* condBool = toBool(condInt);
        builder.CreateCondBr(condBool, bodyBB, endBB);

        builder.SetInsertPoint(bodyBB);
        n.get_body()->accept(*this);
        builder.CreateBr(incBB);

        builder.SetInsertPoint(incBB);
        n.get_iter()->accept(*this);
        builder.CreateBr(condBB);

        builder.SetInsertPoint(endBB);
        curVal = emitMakeIntValue(
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0, true)
        );
    }

    void Visit(AST::FunctionNode& n) override
    {
        dbg("Visit FunctionNode");

        std::string fname;

        if (n.has_internal_name())
            fname = std::string(n.get_internal_name());
        else
            fname = "lambda_" + std::to_string(functions.size());

        dbg("Function name = " + fname);

        llvm::Function* func = module->getFunction(fname);
        if (!func)
            func = createFunction(fname, n.get_params().size());

        functions[fname] = func;

        dbg("Registered function " + fname);

        std::vector<std::string> captures = collectOuterVisibleNames(n);
        functionCaptures_[&n] = captures;

        if (!func->empty())
        {
            llvm::Value* codePtr = builder.CreateBitCast(
                func,
                getVoidPtrType(),
                "codeptr"
            );

            llvm::Value* envPtr = llvm::ConstantPointerNull::get(
                llvm::cast<llvm::PointerType>(getVoidPtrType())
            );

            llvm::Value* closurePtr = builder.CreateCall(
                paraclMakeClosureFn,
                { codePtr, envPtr },
                "closure"
            );

            curVal = builder.CreateCall(
                paraclMakeClosureValueFn,
                { closurePtr },
                "closure_value"
            );
            return;
        }

        llvm::Function*   oldFunc        = currentFunction;
        llvm::BasicBlock* oldInsertBlock = builder.GetInsertBlock();
        size_t oldFunctionScopeBase = currentFunctionScopeBase_;

        currentFunction = func;

        llvm::BasicBlock* entry =
            llvm::BasicBlock::Create(context, "entry", func);

        builder.SetInsertPoint(entry);

        dbgBlock("Function entry block");

        currentFunctionScopeBase_ = symbolScopes.size();
        pushScope();

        auto argIt = func->arg_begin();

        llvm::Value* selfArg   = &*argIt++;
        llvm::Value* argsArray = &*argIt++;
        llvm::Value* argcValue = &*argIt++;

        (void)argcValue;

        if (n.has_internal_name())
        {
            llvm::Value* selfValue = builder.CreateCall(
                paraclMakeClosureValueFn,
                { selfArg },
                "self.closure.value"
            );

            std::string selfName = std::string(n.get_internal_name());

            llvm::AllocaInst* selfAlloca = createEntryBlockAlloca(selfName);
            builder.CreateStore(selfValue, selfAlloca);
            symbolScopes.back()[selfName] = selfAlloca;
        }

        for (size_t i = 0; i < n.get_params().size(); ++i)
        {
            std::string pname = std::string(n.get_params()[i]);

            llvm::Value* idx = llvm::ConstantInt::get(
                llvm::Type::getInt32Ty(context),
                static_cast<uint64_t>(i)
            );

            llvm::Value* argPtr = builder.CreateGEP(
                getParaValueType(),
                argsArray,
                idx,
                pname + ".arg.ptr"
            );

            llvm::Value* argVal = builder.CreateLoad(
                getParaValueType(),
                argPtr,
                pname + ".arg"
            );

            llvm::AllocaInst* alloca = createEntryBlockAlloca(pname);
            builder.CreateStore(argVal, alloca);
            symbolScopes.back()[pname] = alloca;
        }

        captureScopes_.emplace_back();

        auto captureIt = functionCaptures_.find(&n);
        if (captureIt != functionCaptures_.end() && !captureIt->second.empty())
        {
            llvm::Value* envRawPtr = builder.CreateCall(
                paraclClosureGetEnvFn,
                { selfArg },
                "closure.env.raw"
            );

            llvm::Value* envTypedPtr = builder.CreateBitCast(
                envRawPtr,
                llvm::PointerType::getUnqual(getParaValueType()),
                "closure.env.typed"
            );

            for (size_t i = 0; i < captureIt->second.size(); ++i)
            {
                const std::string& name = captureIt->second[i];

                llvm::Value* idx = llvm::ConstantInt::get(
                    llvm::Type::getInt32Ty(context),
                    static_cast<uint64_t>(i)
                );

                llvm::Value* slotPtr = builder.CreateGEP(
                    getParaValueType(),
                    envTypedPtr,
                    idx,
                    name + ".capture.ptr"
                );

                captureScopes_.back()[name] = slotPtr;
            }
        }

        n.get_body()->accept(*this);

        dbgValue("Function body curVal after body", curVal);
        dbgBlock("After function body");

        if (!builder.GetInsertBlock()->getTerminator())
        {
            llvm::Value* retVal = curVal;

            if (!retVal)
            {
                retVal = emitMakeIntValue(
                    llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0, true)
                );
            }

            dbgValue("Function implicit return value", retVal);
            builder.CreateRet(retVal);
        }

        captureScopes_.pop_back();
        popScope();

        currentFunctionScopeBase_ = oldFunctionScopeBase;
        currentFunction = oldFunc;
        if (oldInsertBlock)
            builder.SetInsertPoint(oldInsertBlock);

        llvm::Value* envPtr = llvm::ConstantPointerNull::get(
            llvm::cast<llvm::PointerType>(getVoidPtrType())
        );

        if (!captures.empty())
        {
            uint64_t envSize = captures.size() * 8;
            llvm::Value* envSizeVal = llvm::ConstantInt::get(
                llvm::Type::getInt64Ty(context),
                envSize
            );

            envPtr = builder.CreateCall(
                paraclAllocEnvFn,
                { envSizeVal },
                "env"
            );

            llvm::Value* envTypedPtr = builder.CreateBitCast(
                envPtr,
                llvm::PointerType::getUnqual(getParaValueType()),
                "env.typed"
            );

            for (size_t i = 0; i < captures.size(); ++i)
            {
                llvm::AllocaInst* outerVar = lookupVariable(captures[i]);
                if (!outerVar)
                    throw std::runtime_error("Capture source not found: " + captures[i]);

                llvm::Value* capturedValue = builder.CreateLoad(
                    getParaValueType(),
                    outerVar,
                    captures[i] + ".captured.load"
                );

                llvm::Value* idx = llvm::ConstantInt::get(
                    llvm::Type::getInt32Ty(context),
                    static_cast<uint64_t>(i)
                );

                llvm::Value* slotPtr = builder.CreateGEP(
                    getParaValueType(),
                    envTypedPtr,
                    idx,
                    captures[i] + ".env.slot"
                );

                builder.CreateStore(capturedValue, slotPtr);
            }
        }

        llvm::Value* codePtr = builder.CreateBitCast(
            func,
            getVoidPtrType(),
            "codeptr"
        );

        llvm::Value* closurePtr = builder.CreateCall(
            paraclMakeClosureFn,
            { codePtr, envPtr },
            "closure"
        );

        curVal = builder.CreateCall(
            paraclMakeClosureValueFn,
            { closurePtr },
            "closure_value"
        );

        dbgFunctionIR(func);
    }


    void Visit(AST::ReturnNode& n) override
    {
        dbg("Visit ReturnNode");
        dbgBlock("Return current block");

        if (!currentFunction)
            throw std::runtime_error("return outside function");

        if (n.get_expr())
        {
            n.get_expr()->accept(*this);
            dbgValue("Return expr raw curVal", curVal);

            if (!curVal)
                throw std::runtime_error("ReturnNode: null return value");

            builder.CreateRet(curVal);
        }
        else
        {
            llvm::Value* zero = emitMakeIntValue(
                llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0, true)
            );
            builder.CreateRet(zero);
        }
    }

    void Visit(AST::CallNode& n) override
    {
        dbg("Visit CallNode");
        n.get_callee()->accept(*this);

        dbgValue("Call callee raw", curVal);

        if (!curVal)
            throw std::runtime_error("call of null value");

        llvm::Value* closurePtr = builder.CreateCall(
            paraclValueAsClosureFn,
            { curVal },
            "asclosure"
        );

        std::vector<llvm::Value*> argValues;
        argValues.reserve(n.get_args().size());

        for (auto* a : n.get_args())
        {
            a->accept(*this);
            if (!curVal)
                throw std::runtime_error("CallNode: null argument value");

            argValues.push_back(curVal);
        }

        llvm::ArrayType* argsArrayTy =
            llvm::ArrayType::get(getParaValueType(), argValues.size());

        llvm::AllocaInst* argsAlloca =
            builder.CreateAlloca(argsArrayTy, nullptr, "call.args");

        for (size_t i = 0; i < argValues.size(); ++i)
        {
            llvm::Value* zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
            llvm::Value* idx  = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), i);

            llvm::Value* elemPtr = builder.CreateGEP(
                argsArrayTy,
                argsAlloca,
                { zero, idx },
                "arg.ptr"
            );

            builder.CreateStore(argValues[i], elemPtr);
        }

        llvm::Value* argsPtr = builder.CreateGEP(
            argsArrayTy,
            argsAlloca,
            {
                llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0),
                llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0)
            },
            "args.ptr"
        );

        llvm::Value* argcVal = llvm::ConstantInt::get(
            llvm::Type::getInt32Ty(context),
            static_cast<uint64_t>(argValues.size())
        );

        curVal = builder.CreateCall(
            paraclCallClosureFn,
            { closurePtr, argsPtr, argcVal },
            "calltmp"
        );

        dbgValue("Call result", curVal);
    }

    void Visit(AST::ScopeNode& n) override
    {
        dbg("Visit ScopeNode enter");
        pushScope();

        llvm::Value* lastValue = nullptr;

        size_t idx = 0;
        for (auto&& stmt : n.get_children())
        {
            if (builder.GetInsertBlock() && builder.GetInsertBlock()->getTerminator())
            {
                dbg("ScopeNode: block already terminated, break");
                break;
            }

            dbg("ScopeNode: visiting child #" + std::to_string(idx));

            curVal = nullptr;
            stmt->accept(*this);

            dbgValue("ScopeNode curVal after child", curVal);

            if (curVal)
                lastValue = curVal;

            ++idx;
        }

        popScope();

        if (!lastValue)
        {
            lastValue = emitMakeIntValue(
                llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0, true)
            );
        }

        curVal = lastValue;
        dbgValue("ScopeNode exit curVal", curVal);
    }

    void Visit(AST::InNode& /*n*/) override
    {
        curVal = builder.CreateCall(paraclInValueFn, {}, "input_value");
    }

    void Visit(AST::PrintNode& n) override
    {
        n.get_expr()->accept(*this);
        llvm::Value* value = curVal;

        if (!value)
            throw std::runtime_error("PrintNode: null value");

        builder.CreateCall(paraclPrintValueFn, { value });

        curVal = value;
    }

    void Visit(AST::ExpressionNode&)              override {}
    void Visit(AST::StatementNode&)               override {}
    void Visit(AST::ConditionalStatementNode&)    override {}

    llvm::Function* getOrCreateParaLibFunc(const std::string& name, llvm::FunctionType* type)
    {
        if (auto* func = module->getFunction(name))
            return func;

        return llvm::Function::Create(
            type,
            llvm::Function::ExternalLinkage,
            name,
            module.get()
        );
    }

    void Visit(const AST::ConditionalStatementNode&) const override { throw std::runtime_error("const Visit not implemented"); }
    void Visit(const AST::StatementNode&)            const override { throw std::runtime_error("const Visit not implemented"); }
    void Visit(const AST::ExpressionNode&)           const override { throw std::runtime_error("const Visit not implemented"); }
    void Visit(const AST::ForNode&)                  const override { throw std::runtime_error("const Visit not implemented"); }
    void Visit(const AST::ConstantNode&)             const override { throw std::runtime_error("const Visit not implemented"); }
    void Visit(const AST::AssignNode&)               const override { throw std::runtime_error("const Visit not implemented"); }
    void Visit(const AST::WhileNode&)                const override { throw std::runtime_error("const Visit not implemented"); }
    void Visit(const AST::IfNode&)                   const override { throw std::runtime_error("const Visit not implemented"); }
    void Visit(const AST::InNode&)                   const override { throw std::runtime_error("const Visit not implemented"); }
    void Visit(const AST::VariableNode&)             const override { throw std::runtime_error("const Visit not implemented"); }
    void Visit(const AST::BinaryOpNode&)             const override { throw std::runtime_error("const Visit not implemented"); }
    void Visit(const AST::UnaryOpNode&)              const override { throw std::runtime_error("const Visit not implemented"); }
    void Visit(const AST::PrintNode&)                const override { throw std::runtime_error("const Visit not implemented"); }
    void Visit(const AST::ScopeNode&)                const override { throw std::runtime_error("const Visit not implemented"); }
    void Visit(const AST::FunctionNode&)             const override { throw std::runtime_error("const Visit not implemented"); }
    void Visit(const AST::CallNode&)                 const override { throw std::runtime_error("const Visit not implemented"); }
    void Visit(const AST::ReturnNode&)               const override { throw std::runtime_error("const Visit not implemented"); }
};