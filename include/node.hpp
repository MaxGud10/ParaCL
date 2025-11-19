#pragma once

#include "detail/context.hpp"
#include "detail/inode.hpp"
#include "inode.hpp"
#include "log.h"
#include "nodeDump.hpp"

#include <cmath>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>

namespace AST
{

class StatementNode  : public INode {};

class ExpressionNode : public StatementNode {};

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

		return 0;
    }

    void dump(std::ostream& os) const override
    {
        for (const auto &child : children_) {
            MSG("dumping child\n");
            child->dump(os);
            MSG("dumped child\n");
        }
    }

    void push_child(StmtPtr&& stmt)
    {
        children_.push_back(std::move(stmt));
    }

	size_t nstms() const { return children_.size(); }
    std::vector<StmtPtr> get_children() const { return std::move(const_cast<std::vector<StmtPtr>&>(children_)); ; }
};

using ScopePtr = std::unique_ptr<ScopeNode>;

class ConstantNode final : public ExpressionNode
{
private:
    const int val_;

public:
    ConstantNode(int val) : val_(val) {}

    int eval([[maybe_unused]]detail::Context& ctx) const override
    {
		LOG("Evaluating constant: {}\n", val_);
        return val_;
    }

    int get_val() const
    {
        return val_;
    }

    friend std::ostream& operator<<(std::ostream& os, const ConstantNode& n) {
        os << SET_NODE << &n
        << SET_MRECORD_SHAPE
        << SET_LABEL << n.val_ << SET_ADR << &n << END_LABEL
        << SET_FILLED << SET_COLOR << std::hex << CONSTANT_NODE_COLOR << std::dec
        << END_NODE;

        return os;
    }

    void dump(std::ostream& os) const override {
        MSG("dumping constant node\n");
        os << *this;
        MSG("dumped constant node\n");
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

    int eval(detail::Context& ctx) const override
    {
		LOG("Evaluating variable: {}\n", name_);

        for (int32_t scopeId = ctx.curScope_; scopeId >= 0; --scopeId)
        {
            auto it = ctx.varTables_[static_cast<std::size_t>(scopeId)].find(name_);

            if (it != ctx.varTables_[static_cast<std::size_t>(scopeId)].end())
            {
				LOG("It's {}\n", it->second);
                return it->second;
            }
        }

		throw std::runtime_error("Undeclared variable: " + name_ + "\n");
    }

    friend std::ostream& operator<<(std::ostream& os, const VariableNode& n) {
        os << SET_NODE << &n
        << SET_MRECORD_SHAPE
        << SET_LABEL << n.name_ << SET_ADR << &n << END_LABEL
        << SET_FILLED << SET_COLOR << std::hex << VARIABLE_NODE_COLOR << std::dec
        << END_NODE;

        return os;
    }

    void dump(std::ostream& os) const override {
        MSG("dumping variable node\n");
        os << *this;
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

    int eval(detail::Context& ctx) const override
    {
		MSG("Evaluating Binary Operation\n");

        int leftVal = left_->eval(ctx);
        int rightVal = right_->eval(ctx);

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

            case BinaryOp::BIT_AND:
                result = leftVal & rightVal;
                break;

            case BinaryOp::BIT_OR:
                result = leftVal | rightVal;
                break;

            default:
                throw std::runtime_error("Unknown binary operation");
        }

		LOG("It's {}\n", result);

		return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const BinaryOpNode& n)
    {
        os << SET_NODE << &n
        << SET_MRECORD_SHAPE
        << SET_LABEL << "binary: " << BinaryOpNames[static_cast<std::size_t>(n.op_)] << SET_ADR << &n << END_LABEL
        << SET_FILLED << SET_COLOR << std::hex << BINARYOP_NODE_COLOR << std::dec
        << END_NODE;

        os << SET_NODE << &n << SET_LINK << SET_NODE << n.left_.get() << std::endl;
        os << SET_NODE << &n << SET_LINK << SET_NODE << n.right_.get() << std::endl;

        n.left_->dump(os);
        n.right_->dump(os);

        return os;
    }

    void dump(std::ostream& os) const override {
        MSG("dumping binary node\n");
        os << *this;
    }
};

class UnaryOpNode final : public ExpressionNode
{
private:
    ExprPtr operand_;
    UnaryOp op_;

public:
    UnaryOpNode(ExprPtr&& operand, UnaryOp op) : operand_(std::move(operand)), op_(op) {}

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

    friend std::ostream& operator<<(std::ostream& os, const UnaryOpNode& n)
    {
        os << SET_NODE << &n
        << SET_MRECORD_SHAPE
        << SET_LABEL << "unary: " << UnaryOpNames[static_cast<std::size_t>(n.op_)] << SET_ADR << &n << END_LABEL
        << SET_FILLED << SET_COLOR << std::hex << UNARYOP_NODE_COLOR << std::dec
        << END_NODE;

        os << SET_NODE << &n << SET_LINK << SET_NODE << n.operand_.get() << std::endl;

        n.operand_->dump(os);

        return os;
    }

    void dump(std::ostream& os) const override {
        MSG("dumping unary node\n");
        os << *this;
    }
};

class AssignNode final : public StatementNode
{
private:
    std::unique_ptr<VariableNode> dest_;
    ExprPtr                       expr_;

public:
    AssignNode(std::unique_ptr<VariableNode>&& dest, ExprPtr&& expr) : dest_(std::move(dest)), expr_(std::move(expr)) {}

    int eval(detail::Context& ctx) const override
    {
		MSG("Evaluating assignment\n");

        std::string destName = dest_->get_name();

		MSG("Getting assigned value\n");
        int value = expr_->eval(ctx);
		LOG("Assigned value is {}\n", value);

        int32_t scopeId = 0;

        while (scopeId < ctx.curScope_)
		{
            if (ctx.varTables_[static_cast<std::size_t>(scopeId)].contains(destName))
                break;

			scopeId++;
		}

        ctx.varTables_[static_cast<std::size_t>(scopeId)][destName] = value;

        return value;
    }

    friend std::ostream& operator<<(std::ostream& os, const AssignNode& n)
    {
        os << SET_NODE << &n
        << SET_MRECORD_SHAPE
        << SET_LABEL << "ASSIGN '='" << SET_ADR << &n << END_LABEL
        << SET_FILLED << SET_COLOR << std::hex << ASSIGN_NODE_COLOR << std::dec
        << END_NODE;

        os << SET_NODE << &n << SET_LINK << SET_NODE << n.expr_.get() << std::endl;

        n.expr_->dump(os);

        return os;
    }

    void dump(std::ostream& os) const override {
        MSG("dumping assign node\n");
        os << *this;
    }

};

class WhileNode final : public ConditionalStatementNode
{
private:
    ExprPtr cond_;
    StmtPtr scope_;

public:
    WhileNode(ExprPtr&& cond, StmtPtr&& scope) : cond_(std::move(cond)), scope_(std::move(scope)) {}

    int eval(detail::Context& ctx) const override
    {
        int result = 0;

        while (cond_->eval(ctx))
        {
            result = scope_->eval(ctx);
        }

        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const WhileNode& n)
    {
        os << SET_NODE << &n
        << SET_MRECORD_SHAPE
        << SET_LABEL << "WHILE" << SET_ADR << &n << END_LABEL
        << SET_FILLED << SET_COLOR << std::hex << WHILE_NODE_COLOR << std::dec
        << END_NODE;

        os << SET_NODE << &n << SET_LINK << SET_NODE << n.cond_.get() << std::endl;
        os << SET_NODE << &n << SET_LINK << SET_NODE << n.scope_.get() << std::endl;

        n.cond_->dump(os);
        n.scope_->dump(os);

        return os;
    }

    void dump(std::ostream& os) const override {
        MSG("dumping while node\n");
        os << *this;
    }

};

using AssignPtr = std::unique_ptr<AssignNode>;

class ForNode final : public ConditionalStatementNode
{
private:
    AssignPtr                   init_;
    ExprPtr                     cond_;
    AssignPtr                   iter_;
    StmtPtr                     body_;

public:
    ForNode(std::unique_ptr<AssignNode>&& init, ExprPtr&& cond,
            std::unique_ptr<AssignNode>&& iter, StmtPtr&& body)
        : init_(std::move(init)),
          cond_(std::move(cond)),
          iter_(std::move(iter)),
          body_(std::move(body)) {}

    int eval(detail::Context& ctx) const override
    {
        int result = 0;

        if (init_)
            init_->eval(ctx);

        while (cond_->eval(ctx))
        {
            result = body_->eval(ctx);

            if (iter_)
                iter_->eval(ctx);
        }

        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const ForNode& n)
    {
        os << SET_NODE << &n
        << SET_MRECORD_SHAPE
        << SET_LABEL << "FOR" << SET_ADR << &n << END_LABEL
        << SET_FILLED << SET_COLOR << std::hex << WHILE_NODE_COLOR << std::dec
        << END_NODE;

        os << SET_NODE << &n << SET_LINK << SET_NODE << n.init_.get() << std::endl;
        os << SET_NODE << &n << SET_LINK << SET_NODE << n.cond_.get() << std::endl;
        os << SET_NODE << &n << SET_LINK << SET_NODE << n.iter_.get() << std::endl;
        os << SET_NODE << &n << SET_LINK << SET_NODE << n.body_.get() << std::endl;

        n.init_->dump(os);
        n.cond_->dump(os);
        n.iter_->dump(os);
        n.body_->dump(os);

        return os;
    }

    void dump(std::ostream& os) const override {
        MSG("dumping for node\n");
        os << *this;
    }
};

class IfNode final : public ConditionalStatementNode
{
private:
    ExprPtr cond_;
    StmtPtr action_;
    StmtPtr else_action_;

public:
    IfNode(ExprPtr&& cond, StmtPtr&& action, StmtPtr&& else_action = nullptr)
        : cond_       (std::move(cond       )),
          action_     (std::move(action     )),
          else_action_(std::move(else_action)) {}


    int eval(detail::Context& ctx) const override
    {
        if (cond_->eval(ctx))
            return action_->eval(ctx);

        if (else_action_)
            return else_action_->eval(ctx);

        return 0;
    }

    friend std::ostream& operator<<(std::ostream& os, const IfNode& n)
    {
        os << SET_NODE << &n
        << SET_MRECORD_SHAPE
        << SET_LABEL << "IF" << SET_ADR << &n << END_LABEL
        << SET_FILLED << SET_COLOR << std::hex << IF_NODE_COLOR << std::dec
        << END_NODE;

        os << SET_NODE << &n << SET_LINK << SET_NODE << n.cond_.get() << std::endl;
        os << SET_NODE << &n << SET_LINK << SET_NODE << n.action_.get() << std::endl;
        os << SET_NODE << &n << SET_LINK << SET_NODE << n.else_action_.get() << std::endl;

        n.cond_->dump(os);
        n.action_->dump(os);
        if (n.else_action_) n.else_action_->dump(os);

        return os;
    }

    void dump(std::ostream& os) const override {
        MSG("dumping if node\n");
        os << *this;
        MSG("dumped if node\n");
    }
};

class PrintNode final : public StatementNode
{
private:
    ExprPtr expr_;

public:
    PrintNode(ExprPtr&& expr) : expr_(std::move(expr)) {}

    int eval(detail::Context& ctx) const override
    {
		MSG("Evaluation print\n");

        int value = expr_->eval(ctx);

        ctx.out << value << std::endl;

        return value;
    }

    friend std::ostream& operator<<(std::ostream& os, const PrintNode& n)
    {
        os << SET_NODE << &n
        << SET_MRECORD_SHAPE
        << SET_LABEL << "PRINT" << SET_ADR << &n << END_LABEL
        << SET_FILLED << SET_COLOR << std::hex << PRINT_NODE_COLOR << std::dec
        << END_NODE;

        os << SET_NODE << &n << SET_LINK << SET_NODE << n.expr_.get() << std::endl;

        n.expr_->dump(os);

        return os;
    }

    void dump(std::ostream& os) const override {
        MSG("dumping print node\n");
        os << *this;
        MSG("dumped print node\n");
    }
};

class InNode final : public ExpressionNode
{
public:
    int eval([[maybe_unused]] detail::Context& ctx) const override
    {
        int value = 0;

        std::cin >> value;

        if (std::cin.fail())
        {
            throw std::runtime_error("Incorrect input");
        }

        return value;
    }

    friend std::ostream& operator<<(std::ostream& os, const InNode& n)
    {
        os << SET_NODE << &n
        << SET_MRECORD_SHAPE
        << SET_LABEL << "IN" << SET_ADR << &n << END_LABEL
        << SET_FILLED << SET_COLOR << std::hex << PRINT_NODE_COLOR << std::dec
        << END_NODE;

        return os;
    }

    void dump(std::ostream& os) const override {
        MSG("dumping in node\n");
        os << *this;
    }
};

class VoidNode final : public ExpressionNode
{
	int eval([[maybe_unused]] detail::Context& ctx) const override { return 0; }
    void dump(std::ostream& os) const override {}
};

} // namespace AST
