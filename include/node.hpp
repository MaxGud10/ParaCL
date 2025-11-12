#pragma once 

#include "detail/context.hpp"
#include "detail/inode.hpp"
#include "inode.hpp"
#include "log.h"

#include <cmath>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>

namespace AST
{

class StatementNode
{
public:
	virtual void eval(detail::Context& ctx) const = 0;

	virtual ~StatementNode() = default;
};

class ExpressionNode : public StatementNode
{
public:
	void eval(detail::Context& ctx) const override
	{
		eval_value(ctx);
	}

	virtual int eval_value(detail::Context& ctx) const = 0;
};

class ConditionalStatementNode : public StatementNode {};

using ExprPtr     = std::unique_ptr<ExpressionNode>;
using StmtPtr     = std::unique_ptr<StatementNode>;
using CondStmtPtr = std::unique_ptr<ConditionalStatementNode>;

class ScopeNode final : public StatementNode
{
private:
    std::vector<StmtPtr> children_;

public:
	ScopeNode(std::vector<StmtPtr>&& stms)
	{
		for (auto& stm : stms)
		{
			children_.push_back(std::move(stm));
		}
	}

    void eval(detail::Context& ctx) const override
    {
        if (children_.empty()) 
            return;

		MSG("Evaluating scope\n");

        ++ctx.curScope_;

        ctx.varTables_.push_back(detail::Context::VarTable());

		LOG("ctx.curScope_ = {}\n", ctx.curScope_);
		LOG("ctx.varTables_ size = {}\n", ctx.varTables_.size());

		MSG("Scopes children:\n");
		for ([[maybe_unused]]const auto& child : children_)
        {
			LOG("{}\n", static_cast<const void*>(child.get()));
        }

        for (const auto& child : children_)
        {
			LOG("Evaluating {}\n", static_cast<const void*>(child.get()));
            child->eval(ctx);
        }

        ctx.varTables_.pop_back();

        --ctx.curScope_;

		LOG("ctx.curScope_ = {}\n", ctx.curScope_);
		LOG("ctx.varTables_ size = {}\n", ctx.varTables_.size());

    }

    void push_child(StmtPtr&& stmt)
    {
        children_.push_back(std::move(stmt));
    }

	size_t nstms() const { return children_.size(); }
};

using ScopePtr = std::unique_ptr<ScopeNode>;

class ConstantNode final : public ExpressionNode
{
private:
    const int val_;

public:
    ConstantNode(int val) : val_(val) {}

    int eval_value([[maybe_unused]]detail::Context& ctx) const override
    {
		LOG("Evaluating constant: {}\n", val_);
        return val_;
    }

    int get_val() const
    {
        return val_;
    }
};

class VariableNode final : public ExpressionNode
{
private:
    std::string name_;

public:
	VariableNode(const std::string& name): name_(name) {}

    const std::string& get_name() const
    {
        return name_;
    }

    int eval_value(detail::Context& ctx) const override
    {
		LOG("Evaluating variable: {}\n", name_);

        for (int32_t scopeId = ctx.curScope_; scopeId >= 0; --scopeId)
        {
            auto it = ctx.varTables_[scopeId].find(name_);

            if (it != ctx.varTables_[scopeId].end())
            {
				LOG("It's {}\n", it->second);
                return it->second;
            }
        }

		throw std::runtime_error("Undeclared variable: " + name_ + "\n");
    }
};

class BinaryOpNode final : public ExpressionNode
{
private:
    ExprPtr  left_;
    ExprPtr  right_;
    BinaryOp op_;

public:
    BinaryOpNode(ExprPtr&& left, BinaryOp op, ExprPtr&& right) : left_(std::move(left)), right_(std::move(right)), op_(op) {}

    int eval_value(detail::Context& ctx) const override
    {
		MSG("Evaluating Binary Operation\n");

        int leftVal  = left_->eval_value(ctx);
        int rightVal = right_->eval_value(ctx);

		int result = 0;

        switch (op_)
        {
            case BinaryOp::ADD:
                result = leftVal + rightVal;
				break;

            case BinaryOp::SUB:
                result = leftVal - rightVal;
				break;

            case BinaryOp::MUL:
                result = leftVal * rightVal;
				break;

            case BinaryOp::DIV:
                if (rightVal == 0) { throw std::runtime_error("Divide by zero"); }
                result = leftVal / rightVal;
				break;

            case BinaryOp::MOD:
                result = leftVal % rightVal;
				break;

            case BinaryOp::LS:
                result = leftVal < rightVal;
				break;

            case BinaryOp::GR:
                result = leftVal > rightVal;
				break;

            case BinaryOp::LS_EQ:
                result = leftVal <= rightVal;
				break;

            case BinaryOp::GR_EQ:
                result = leftVal >= rightVal;
				break;

            case BinaryOp::EQ:
                result = leftVal == rightVal;
				break;
 
            case BinaryOp::NOT_EQ:
                result = leftVal != rightVal;
				break;

            case BinaryOp::AND:
                result = leftVal && rightVal;
				break;

            case BinaryOp::OR:
                result = leftVal || rightVal;
				break;

            default:
                throw std::runtime_error("Unknown binary operation"); 
        }

		LOG("It's {}\n", result);

		return result;
    }
};

class UnaryOpNode final : public ExpressionNode
{
private:
    ExprPtr operand_;
    UnaryOp op_;

public:
    UnaryOpNode(ExprPtr&& operand, UnaryOp op) : operand_(std::move(operand)), op_(op) {}

    int eval_value(detail::Context& ctx) const override
    {
        int operandVal = operand_->eval_value(ctx);

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
};

class AssignNode final : public ExpressionNode
{
private:
    std::unique_ptr<VariableNode> dest_;
    ExprPtr                       expr_;

public:
    AssignNode(std::unique_ptr<VariableNode>&& dest, ExprPtr&& expr) : dest_(std::move(dest)), expr_(std::move(expr)) {}

    int eval_value(detail::Context& ctx) const override
    {
		MSG("Evaluating assignment\n");

        std::string destName = dest_->get_name();

		MSG("Getting assigned value\n");
        int value = expr_->eval_value(ctx);
		LOG("Assigned value is {}\n", value);

        int32_t scopeId = 0;

        while (scopeId < ctx.curScope_)
		{
            if (ctx.varTables_[scopeId].contains(destName)) break;
			scopeId++;
		}

        ctx.varTables_[scopeId][destName] = value;

        return value;
    }
};

class WhileNode final : public ConditionalStatementNode
{
private:
    ExprPtr cond_;
    StmtPtr scope_;

public: // [x]
    WhileNode(ExprPtr&& cond, StmtPtr&& scope) : cond_(std::move(cond)), scope_(std::move(scope)) {}

    void eval(detail::Context& ctx) const override
    {
        while (cond_->eval_value(ctx))
        {
            scope_->eval(ctx);
        }
    }
};

class ElseLikeNode : public StatementNode {};

using ElseLikePtr = std::unique_ptr<ElseLikeNode>;

class ElseIfNode final : public ElseLikeNode
{
private:
    ExprPtr     cond_;
    StmtPtr     action_;
	ElseLikePtr alt_action_;

public:
    ElseIfNode(ExprPtr&& cond, StmtPtr&& action) : cond_(std::move(cond)), action_(std::move(action)) {}

	ElseIfNode(ExprPtr&& cond, StmtPtr&& action, ElseLikePtr&& alt_action) : cond_(std::move(cond)), action_(std::move(action)),	alt_action_(std::move(alt_action)) {}

    void eval(detail::Context& ctx) const override
    {
        if (cond_->eval_value(ctx))
            action_->eval(ctx);

		else
		{
			if (alt_action_) 
                alt_action_->eval(ctx);
		}
    }
};

class ElseNode final : public ElseLikeNode
{
private:
	StmtPtr action_;

public:
	ElseNode(StmtPtr&& action) : action_(std::move(action)) {}

	void eval(detail::Context& ctx) const override
    {
        action_->eval(ctx);
    }
};

class IfNode final : public ConditionalStatementNode
{
private:
    ExprPtr     cond_;
    StmtPtr     action_;
    ElseLikePtr alt_action_;

public:
    IfNode(ExprPtr&& cond, StmtPtr&& action) : cond_(std::move(cond)), action_(std::move(action)) {}

    	IfNode(ExprPtr&& cond, StmtPtr&& action, ElseLikePtr&& alt_action) 
        : cond_      (std::move(cond      )), 
          action_    (std::move(action    )), 
          alt_action_(std::move(alt_action)) {}

    void eval(detail::Context& ctx) const override
    {
        if (cond_->eval_value(ctx))
        {
            action_->eval(ctx);
        }
		else
		{
			if (alt_action_) 
                alt_action_->eval(ctx);
		}
    }
};

class PrintNode final : public StatementNode
{
private:
    ExprPtr expr_;

public:
    PrintNode(ExprPtr&& expr) : expr_(std::move(expr)) {}

    void eval(detail::Context& ctx) const override
    {
		MSG("Evaluation print\n");

        int value = expr_->eval_value(ctx);

        ctx.out << value;
    }
};

class InNode final : public ExpressionNode
{
public:
    int eval_value([[maybe_unused]] detail::Context& ctx) const override
    {
        int value = 0;

        std::cin >> value;

        if (std::cin.fail())
        {
            throw std::runtime_error("Incorrect input");
        }

        return value;
    }
};

// class VoidNode final : public ExpressionNode
// {
// 	int eval([[maybe_unused]] detail::Context& ctx) const override { return 0; }
// };

} // namespace AST

