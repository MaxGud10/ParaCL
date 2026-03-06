#pragma once

#include <iostream>
#include <stack>
#include <vector>
#include <stdexcept>

#include "detail/ivisitor.hpp"
#include "node.hpp"
#include "detail/context.hpp"
#include "detail/value.hpp"
#include "log.h"

struct ReturnSignal
{
    AST::detail::Value value;
};

class TreeTraverse : public Visitor
{
private:
    using Value = AST::detail::Value;

    AST::detail::Context& ctx_;
    std::stack<Value>     eval_stack_;

    struct CallDepthGuard
    {
        AST::detail::Context& context;
        bool                  isActive = false;

        explicit CallDepthGuard(AST::detail::Context& ctx) : context(ctx)
        {
            context.enter_call();
            isActive = true;
        }

        ~CallDepthGuard()
        {
            if (isActive)
                context.exit_call();
        }
    };

    int scope_depth() const
    {
        int depth = -1;
        for (auto framePointer = ctx_.current_; framePointer; framePointer = framePointer->parent)
            ++depth;
        return depth;
    }

    static int as_int(const Value& value)
    {
        if (!std::holds_alternative<int>(value))
            throw std::runtime_error("Type error: expected int, got func");
        return std::get<int>(value);
    }

    static AST::detail::FunctionPtr as_function(const Value& value)
    {
        if (!std::holds_alternative<AST::detail::FunctionPtr>(value))
            throw std::runtime_error("Type error: expected func, got int");
        return std::get<AST::detail::FunctionPtr>(value);
    }

    static Value pop_value(std::stack<Value>& valueStack)
    {
        if (valueStack.empty())
            return Value{0};

        Value topValue = valueStack.top();
        valueStack.pop();

        return topValue;
    }

    static const char* value_kind(const Value& value)
    {
        if (std::holds_alternative<int>(value))
            return "int";
        return "func";
    }

    void push_value(const Value& value)
    {
        LOG("EVAL: push {} (stack size before: {})\n", value_kind(value), eval_stack_.size());
        eval_stack_.push(value);
    }

    Value pop_value()
    {
        Value value = pop_value(eval_stack_);
        LOG("EVAL: pop  {} (stack size after: {})\n", value_kind(value), eval_stack_.size());
        return value;
    }

public:
    explicit TreeTraverse(AST::detail::Context& context)
        : ctx_(context){}

    ~TreeTraverse() override = default;

    int get_result() const
    {
        if (eval_stack_.empty())
            return 0;

        return as_int(eval_stack_.top());
    }

    void Visit(AST::ScopeNode& scopeNode) override
    {
        LOG("SCOPE: enter depth={} children={}\n", scope_depth() + 1, scopeNode.nstms());
        ctx_.push_scope();

        Value lastExpressionValue = Value{0};

        const auto& scopeChildren = scopeNode.get_children();
        for (auto* childStatement : scopeChildren)
        {
            LOG("SCOPE: eval child ptr={}\n", static_cast<const void*>(childStatement));
            childStatement->accept(*this);
            lastExpressionValue = pop_value();
        }

        ctx_.pop_scope();
        LOG("SCOPE: exit  depth={}\n", scope_depth());

        push_value(lastExpressionValue);
    }

    void Visit(AST::ConstantNode& constantNode) override
    {
        LOG("CONST: {}\n", constantNode.get_val());
        push_value(Value{constantNode.get_val()});
    }

    void Visit(AST::VariableNode& variableNode) override
    {
        Value loadedValue = ctx_.get_or_throw(variableNode.get_name());
        LOG("VAR: name='{}' kind={}\n", variableNode.get_name(), value_kind(loadedValue));
        push_value(loadedValue);
    }

    void Visit(AST::AssignNode& assignNode) override
    {
        MSG("ASSIGN: begin\n");

        assignNode.get_expr()->accept(*this);
        Value assignedValue = pop_value();

        std::string_view destinationName = assignNode.get_dest()->get_name();
        ctx_.assign(destinationName, assignedValue);

        LOG("ASSIGN: name='{}' kind={}\n", destinationName, value_kind(assignedValue));
        push_value(assignedValue);
    }

    void Visit(AST::PrintNode& printNode) override
    {
        MSG("PRINT: begin\n");

        printNode.get_expr()->accept(*this);
        Value printedValue = pop_value();

        int printedInteger = as_int(printedValue);
        ctx_.out << printedInteger << std::endl;

        LOG("PRINT: value={}\n", printedInteger);
        push_value(printedValue);
    }

    void Visit(AST::InNode& /*inputNode*/) override
    {
        int inputValue = 0;
        std::cin >> inputValue;

        if (std::cin.fail())
            throw std::runtime_error("Incorrect input");

        LOG("INPUT: {}\n", inputValue);
        push_value(Value{inputValue});
    }

    void Visit(AST::IfNode& ifNode) override
    {
        MSG("IF: begin\n");

        ifNode.get_cond()->accept(*this);
        int conditionValue = as_int(pop_value());

        LOG("IF: cond={}\n", conditionValue);

        if (conditionValue)
        {
            ifNode.get_action()->accept(*this);
            return;
        }

        if (ifNode.get_else_action())
        {
            ifNode.get_else_action()->accept(*this);
            return;
        }

        push_value(Value{0});
    }

    void Visit(AST::WhileNode& whileNode) override
    {
        MSG("WHILE: begin\n");

        Value lastIterationValue = Value{0};

        whileNode.get_cond()->accept(*this);
        int conditionValue = as_int(pop_value());

        while (conditionValue)
        {
            whileNode.get_scope()->accept(*this);
            lastIterationValue = pop_value();

            whileNode.get_cond()->accept(*this);
            conditionValue      = as_int(pop_value());
        }

        push_value(lastIterationValue);
    }

    void Visit(AST::ForNode& forNode) override
    {
        MSG("FOR: begin\n");

        Value lastIterationValue = Value{0};

        if (forNode.get_init())
        {
            forNode.get_init()->accept(*this);
            (void)pop_value();
        }

        forNode.get_cond()->accept(*this);
        int conditionValue = as_int(pop_value());

        while (conditionValue)
        {
            forNode.get_body()->accept(*this);
            lastIterationValue = pop_value();

            if (forNode.get_iter())
            {
                forNode.get_iter()->accept(*this);
                (void)pop_value();
            }

            forNode.get_cond()->accept(*this);
            conditionValue = as_int(pop_value());
        }

        push_value(lastIterationValue);
    }

    void Visit(AST::UnaryOpNode& unaryOpNode) override
    {
        MSG("UNARY: begin\n");

        unaryOpNode.get_operand()->accept(*this);
        int operandValue = as_int(pop_value());

        int resultValue = 0;
        switch (unaryOpNode.get_op())
        {
            case AST::UnaryOp::NEG: resultValue = -operandValue; break;
            case AST::UnaryOp::NOT: resultValue = !operandValue; break;

            default:
                throw std::runtime_error("Unknown unary operation");
        }

        LOG("UNARY: operand={} result={}\n", operandValue, resultValue);
        push_value(Value{resultValue});
    }

    void Visit(AST::BinaryOpNode& binaryOpNode) override
    {
        MSG("BINARY: begin\n");

        if (binaryOpNode.get_op() == AST::BinaryOp::AND)
        {
            binaryOpNode.get_left()->accept(*this);
            int leftValue = as_int(pop_value());

            if (!leftValue)
            {
                push_value(Value{0});
                return;
            }

            binaryOpNode.get_right()->accept(*this);
            int rightValue = as_int(pop_value());
            push_value(Value{rightValue ? 1 : 0});

            return;
        }

        if (binaryOpNode.get_op() == AST::BinaryOp::OR)
        {
            binaryOpNode.get_left()->accept(*this);
            int leftValue = as_int(pop_value());

            if (leftValue)
            {
                push_value(Value{1});
                return;
            }

            binaryOpNode.get_right()->accept(*this);
            int rightValue = as_int(pop_value());
            push_value(Value{rightValue ? 1 : 0});

            return;
        }

        binaryOpNode.get_left()->accept(*this);
        int leftValue = as_int(pop_value());

        binaryOpNode.get_right()->accept(*this);
        int rightValue = as_int(pop_value());

        int resultValue = 0;

        switch (binaryOpNode.get_op())
        {
            case AST::BinaryOp::ADD:     resultValue = leftValue + rightValue; break;
            case AST::BinaryOp::SUB:     resultValue = leftValue - rightValue; break;
            case AST::BinaryOp::MUL:     resultValue = leftValue * rightValue; break;

            case AST::BinaryOp::DIV:
                if (rightValue == 0)
                    throw std::runtime_error("Divide by zero");
                resultValue = leftValue / rightValue;
                break;

            case AST::BinaryOp::MOD:     resultValue = leftValue % rightValue; break;

            case AST::BinaryOp::LS:      resultValue = leftValue <  rightValue; break;
            case AST::BinaryOp::GR:      resultValue = leftValue >  rightValue; break;
            case AST::BinaryOp::LS_EQ:   resultValue = leftValue <= rightValue; break;
            case AST::BinaryOp::GR_EQ:   resultValue = leftValue >= rightValue; break;
            case AST::BinaryOp::EQ:      resultValue = leftValue == rightValue; break;
            case AST::BinaryOp::NOT_EQ:  resultValue = leftValue != rightValue; break;

            case AST::BinaryOp::BIT_AND: resultValue = leftValue &  rightValue; break;
            case AST::BinaryOp::BIT_OR:  resultValue = leftValue |  rightValue; break;

            default:
                throw std::runtime_error("Unknown binary operation");
        }

        LOG("BINARY: left={} right={} result={}\n", leftValue, rightValue, resultValue);
        push_value(Value{resultValue});
    }

    void Visit(AST::FunctionNode& functionNode) override
    {
        MSG("FUNC: literal begin\n");

        auto functionObject    = std::make_shared<AST::detail::Function>();
        functionObject->params = functionNode.get_params();
        functionObject->body   = functionNode.get_body();
        functionObject->env    = ctx_.current_;

        if (functionNode.has_internal_name())
        {
            functionObject->internalName    = functionNode.get_internal_name();
            functionObject->hasInternalName = true;

            LOG("FUNC: internalName='{}'\n", functionObject->internalName);
        }

        push_value(Value{functionObject});
    }

    void Visit(AST::CallNode& callNode) override
    {
        CallDepthGuard callDepthGuard(ctx_);

        MSG("CALL: begin\n");

        callNode.get_callee()->accept(*this);
        Value calleeValue = pop_value();

        AST::detail::FunctionPtr functionObject = as_function(calleeValue);

        // args
        std::vector<Value> evaluatedArguments;
        evaluatedArguments.reserve(callNode.get_args().size());

        for (auto* argumentExpression : callNode.get_args())
        {
            argumentExpression->accept(*this);
            evaluatedArguments.push_back(pop_value());
        }

        if (evaluatedArguments.size() != functionObject->params.size())
            throw std::runtime_error("Arity mismatch in function call");

        auto savedFrame   = ctx_.current_;
        auto callFrame    = std::make_shared<AST::detail::Frame>();
        callFrame->parent = functionObject->env;

        for (size_t index = 0; index < evaluatedArguments.size(); ++index)
        {
            std::string_view paramName = functionObject->params[index];
            callFrame->vars[paramName] = evaluatedArguments[index];
            LOG("CALL: bind param '{}' kind={}\n", paramName, value_kind(evaluatedArguments[index]));
        }

        if (functionObject->hasInternalName)
        {
            callFrame->vars[functionObject->internalName] = functionObject;
            LOG("CALL: bind internalName '{}' -> self\n", functionObject->internalName);
        }

        ctx_.current_ = callFrame;

        try
        {
            functionObject->body->accept(*this);
            Value functionResult = pop_value();
            ctx_.current_ = savedFrame;

            LOG("CALL: normal return kind={}\n", value_kind(functionResult));
            push_value(functionResult);
        }
        catch (const ReturnSignal& returnSignal)
        {
            ctx_.current_ = savedFrame;
            LOG("CALL: explicit return kind={}\n", value_kind(returnSignal.value));
            push_value(returnSignal.value);
        }
    }

    void Visit(AST::ReturnNode& returnNode) override
    {
        MSG("RETURN: begin\n");

        if (auto* returnedExpression = returnNode.get_expr())
        {
            returnedExpression->accept(*this);
            Value returnedValue = pop_value();
            LOG("RETURN: kind={}\n", value_kind(returnedValue));
            throw ReturnSignal{returnedValue};
        }

        throw ReturnSignal{Value{0}};
    }

    void Visit(const AST::ScopeNode&   ) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
    void Visit(const AST::ForNode&     ) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
    void Visit(const AST::ConstantNode&) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
    void Visit(const AST::AssignNode&  ) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
    void Visit(const AST::WhileNode&   ) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
    void Visit(const AST::IfNode&      ) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
    void Visit(const AST::InNode&      ) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
    void Visit(const AST::VariableNode&) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
    void Visit(const AST::BinaryOpNode&) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
    void Visit(const AST::UnaryOpNode& ) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
    void Visit(const AST::PrintNode&   ) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
    void Visit(const AST::FunctionNode&) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
    void Visit(const AST::CallNode&    ) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
    void Visit(const AST::ReturnNode&  ) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
};

namespace TestUtils
{
    inline int evaluate(AST::INode& node, AST::detail::Context& context)
    {
        TreeTraverse visitor(context);
        node.accept(visitor);
        return visitor.get_result();
    }
}
