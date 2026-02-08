#pragma once

#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <string_view>
#include <ranges>
#include <algorithm>

#include "detail/context.hpp"
#include "detail/inode.hpp"
#include "detail/ivisitor.hpp"
#include "inode.hpp"
#include "log.h"


namespace AST
{


class StatementNode  : public INode {};

class ExpressionNode : public StatementNode {};

class ConditionalStatementNode : public StatementNode {};

using ExprPtr     = ExpressionNode*;
using StmtPtr     = StatementNode*;
using CondStmtPtr = ConditionalStatementNode*;

class ScopeNode final : public StatementNode
{
private:
    std::vector<StmtPtr> children_;

public:

    explicit ScopeNode(std::vector<StmtPtr> stms)
        : children_(std::move(stms)) {}

    int eval(detail::Context& ctx) const override
    {
		MSG("Evaluating scope\n");

        ++ctx.curScope_;

        ctx.varTables_.push_back(detail::Context::VarTable());

		LOG("ctx.curScope_ = {}\n", ctx.curScope_);
		LOG("ctx.varTables_ size = {}\n", ctx.varTables_.size());

		MSG("Scopes children:\n");
		for ([[maybe_unused]]const auto& child : children_)
        {
			LOG("{}\n", static_cast<const void*>(child));
        }

        for (const auto& child : children_)
        {
			LOG("Evaluating {}\n", static_cast<const void*>(child));
            child->eval(ctx);
        }

        ctx.varTables_.pop_back();

        --ctx.curScope_;

		LOG("ctx.curScope_ = {}\n", ctx.curScope_);
		LOG("ctx.varTables_ size = {}\n", ctx.varTables_.size());

		return 0;
    }



    void accept(const Visitor& visitor) const override {
        visitor.Visit(*this);
    }

    void push_child(StmtPtr&& stmt)
    {
        children_.push_back(std::move(stmt));
    }

	size_t nstms() const { return children_.size(); }


public:
    const std::vector<StmtPtr> &get_children() const { return children_; }
};

class ConstantNode final : public ExpressionNode
{
private:
    int val_;

public:

    explicit ConstantNode(int val) : val_(val) {}

    int eval(detail::Context&) const override
    {
		LOG("Evaluating constant: {}\n", val_);
        return val_;
    }


    void accept(const Visitor& visitor) const override {
        visitor.Visit(*this);
    }

public: // getters
    int get_val() const {
        return val_;
    }



};

class VariableNode final : public ExpressionNode
{
private:
    std::string_view name_;

public:
	explicit VariableNode(std::string_view name): name_(name) {}

    int eval(detail::Context& ctx) const override
    {
		LOG("Evaluating variable: {}\n", name_);

        auto tableIt = std::find_if(ctx.varTables_.rbegin(), ctx.varTables_.rend(),
            [&](const auto& table) { return table.contains(name_); });

        if (tableIt != ctx.varTables_.rend())
            return tableIt->find(name_)->second;

		throw std::runtime_error("Undeclared variable: " + std::string(name_) + "\n");
    }



    void accept(const Visitor& visitor) const override {
        visitor.Visit(*this);
    }

public: // getters
    std::string_view get_name() const { return name_; }
};

class BinaryOpNode final : public ExpressionNode
{
private:
    ExprPtr  left_;
    ExprPtr  right_;
    BinaryOp op_;

public:
    BinaryOpNode(ExpressionNode* left, BinaryOp op, ExpressionNode* right)
        : left_(left), right_(right), op_(op) {}

    int eval(detail::Context& ctx) const override
    {
        MSG("Evaluating Binary Operation\n");

        int result = 0;

        switch (op_)
        {
            case BinaryOp::AND:
            {
                const int leftVal = left_->eval(ctx);
                if (!leftVal)
                    return 0;

                const int rightVal = right_->eval(ctx);
                return rightVal ? 1 : 0;
            }

            case BinaryOp::OR:
            {
                const int leftVal = left_->eval(ctx);
                if (leftVal)
                    return 1;

                const int rightVal = right_->eval(ctx);
                return rightVal ? 1 : 0;
            }

            case BinaryOp::DIV:
            {
                const int leftVal  = left_ ->eval(ctx);
                const int rightVal = right_->eval(ctx);

                if (rightVal == 0)
                    throw std::runtime_error("Divide by zero");

                result = leftVal / rightVal;
                break;
            }

            default:
            {
                const int leftVal  = left_ ->eval(ctx);
                const int rightVal = right_->eval(ctx);

                switch (op_)
                {
                    case BinaryOp::ADD:     result = leftVal + rightVal; break;
                    case BinaryOp::SUB:     result = leftVal - rightVal; break;
                    case BinaryOp::MUL:     result = leftVal * rightVal; break;
                    case BinaryOp::MOD:     result = leftVal % rightVal; break;

                    case BinaryOp::LS:      result = leftVal <  rightVal; break;
                    case BinaryOp::GR:      result = leftVal >  rightVal; break;
                    case BinaryOp::LS_EQ:   result = leftVal <= rightVal; break;
                    case BinaryOp::GR_EQ:   result = leftVal >= rightVal; break;
                    case BinaryOp::EQ:      result = leftVal == rightVal; break;
                    case BinaryOp::NOT_EQ:  result = leftVal != rightVal; break;

                    case BinaryOp::BIT_AND: result = leftVal  & rightVal; break;
                    case BinaryOp::BIT_OR:  result = leftVal  | rightVal; break;

                    default:
                        throw std::runtime_error("Unknown binary operation");
                }
            }
        }

        LOG("It's {}\n", result);
        return result;
    }



    void accept(const Visitor& visitor) const override {
        visitor.Visit(*this);
    }

public: // getters
    const ExprPtr  get_left()  const { return left_;  }
    const ExprPtr  get_right() const { return right_; }
    const BinaryOp get_op()    const { return op_;    }
};

class UnaryOpNode final : public ExpressionNode
{
private:
    ExprPtr operand_;
    UnaryOp op_;

public:
    UnaryOpNode(ExprPtr operand, UnaryOp op)
        : operand_(operand), op_(op) {}

    int eval(detail::Context& ctx) const override
    {
        int operandVal = operand_->eval(ctx);

        switch (op_)
        {
            case UnaryOp::NEG:
                return - operandVal;

            case UnaryOp::NOT:
                return ! operandVal;

            default:
                throw std::runtime_error("Unknown unary operation");
        }
    }



    void accept(const Visitor& visitor) const override {
        visitor.Visit(*this);
    }

public: // getters
    const ExprPtr get_operand() const { return operand_; }
    const UnaryOp get_op()      const { return op_;      }
};

class AssignNode final : public ExpressionNode
{
private:
    VariableNode* dest_ = nullptr;
    ExprPtr       expr_ = nullptr;

public:
    AssignNode(VariableNode* dest, ExprPtr expr)
        : dest_(dest), expr_(expr) {}

    int eval(detail::Context& ctx) const override
    {
        MSG("Evaluating assignment\n");
        return ctx.assign(dest_->get_name(), expr_->eval(ctx));
    }



    void accept(const Visitor& visitor) const override {
        visitor.Visit(*this);
    }

public: // getters
    const VariableNode* get_dest() const { return dest_; }
    const ExprPtr       get_expr() const { return expr_; }

};

class WhileNode final : public ConditionalStatementNode
{
private:
    ExprPtr cond_;
    StmtPtr scope_;

public:
    WhileNode(ExprPtr cond, StmtPtr scope)
        : cond_(cond), scope_(scope) {}

    int eval(detail::Context& ctx) const override
    {
        int result = 0;

        while (cond_->eval(ctx))
        {
            result = scope_->eval(ctx);
        }

        return result;
    }



    void accept(const Visitor& visitor) const override {
        visitor.Visit(*this);
    }

public: // getters
    const ExprPtr get_cond()  const { return cond_; }
    const StmtPtr get_scope() const {return scope_; }
};

using AssignPtr = AssignNode*;

class ForNode final : public ConditionalStatementNode
{
private:
    AssignPtr init_ = nullptr;
    ExprPtr   cond_ = nullptr;
    AssignPtr iter_ = nullptr;
    StmtPtr   body_ = nullptr;

public:
    ForNode(AssignPtr init, ExprPtr cond, AssignPtr iter, StmtPtr body)
        : init_(init),
          cond_(cond),
          iter_(iter),
          body_(body) {}


    int eval(detail::Context &ctx) const override
    {
        int result = 0;

        if (init_)
            init_->eval(ctx);

        for ( ; cond_->eval(ctx); )
        {
            result = body_->eval(ctx);

            if (iter_)
                iter_->eval(ctx);
        }

        return result;
    }




    void accept(const Visitor& visitor) const override {
        visitor.Visit(*this);
    }

public: // getters
    const AssignPtr get_init() const { return init_; }
    const ExprPtr   get_cond() const { return cond_; }
    const AssignPtr get_iter() const { return iter_; }
    const StmtPtr   get_body() const { return body_; }
};

class IfNode final : public ConditionalStatementNode
{
private:
    ExprPtr cond_;
    StmtPtr action_;
    StmtPtr else_action_;

public:
    IfNode(ExprPtr cond, StmtPtr action, StmtPtr else_action = nullptr)
        : cond_       (cond),
          action_     (action),
          else_action_(else_action) {}


    int eval(detail::Context& ctx) const override
    {
        if (cond_->eval(ctx))
            return action_->eval(ctx);

        if (else_action_)
            return else_action_->eval(ctx);

        return 0;
    }



    void accept(const Visitor& visitor) const override {
        visitor.Visit(*this);
    }

public: // getters
    const ExprPtr get_cond()        const { return cond_;        }
    const StmtPtr get_action()      const { return action_;      }
    const StmtPtr get_else_action() const { return else_action_; }
};

class PrintNode final : public StatementNode
{
private:
    ExprPtr expr_;

public:
    explicit PrintNode(ExprPtr expr) : expr_(expr) {}

    int eval(detail::Context& ctx) const override
    {
		MSG("Evaluation print\n");

        int value = expr_->eval(ctx);

        ctx.out << value << std::endl;

        return value;
    }



    void accept(const Visitor& visitor) const override {
        visitor.Visit(*this);
    }

public: // getters
    const ExprPtr get_expr() const { return expr_; }
};

class InNode final : public ExpressionNode
{
public:
    int eval(detail::Context&) const override
    {
        int value = 0;

        std::cin >> value;

        if (std::cin.fail())
        {
            throw std::runtime_error("Incorrect input");
        }

        return value;
    }



    void accept(const Visitor& visitor) const override {
        visitor.Visit(*this);
    }
};

class VoidNode final : public ExpressionNode
{
	int  eval(detail::Context&) const override { return 0; }
    void accept(const Visitor& visitor) const override {}
};

} // namespace AST
