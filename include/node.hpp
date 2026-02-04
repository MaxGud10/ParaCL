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
#include "detail/visitor.hpp"
#include "inode.hpp"
#include "log.h"
#include "nodeDump.hpp"


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

    void dump(std::ostream& os) const override
    {
        for (const auto &child : children_) {
            MSG("dumping child\n");
            child->dump(os);
            MSG("dumped child\n");
        }
    }

    void accept(Visitor& visitor) const override {
        visitor.VisitScopeNode(*this);
    }

    void push_child(StmtPtr&& stmt)
    {
        children_.push_back(std::move(stmt));
    }

	size_t nstms() const { return children_.size(); }


    public: // getters
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

    void dump(std::ostream& os) const override {
        MSG("dumping constant node\n");
        os << *this;
        MSG("dumped constant node\n");
    }

    void accept(Visitor& visitor) const override {
        visitor.VisitConstantNode(*this);
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

    void dump(std::ostream& os) const override {
        MSG("dumping variable node\n");
        os << *this;
    }

    void accept(Visitor& visitor) const override {
        visitor.VisitVariableNode(*this);
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

    void dump(std::ostream& os) const override {
        MSG("dumping binary node\n");
        os << *this;
    }

    void accept(Visitor& visitor) const override {
        visitor.VisitBinaryOpNode(*this);
    }

    public: // getters
        const ExprPtr get_left()    { return left_; }
        const ExprtPtr get_right()  { return right_; }
        const BinaryOp get_op()     { return op_; }
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

    void dump(std::ostream& os) const override {
        MSG("dumping unary node\n");
        os << *this;
    }

    void accept(Visitor& visitor) const override {
        visitor.VisitUnaryOpNode(*this);
    }

    public: // getters
        const ExprPtr get_operand() { return operand_; }
        const ExprPtr get_op() { return op_; }
};

class AssignNode final : public StatementNode
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

    void dump(std::ostream& os) const override {
        MSG("dumping assign node\n");
        os << *this;
    }

    void accept(Visitor& visitor) const override {
        visitor.VisitAssignNode(*this);
    }

    public: // getters
        const VariableNode get_dest() { return dest_; }
        const ExprPtr get_expr() { return expr_; }

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

    void dump(std::ostream& os) const override {
        MSG("dumping while node\n");
        os << *this;
    }

    void accept(Visitor& visitor) const override {
        visitor.VisitWhileNode(*this);
    }

    public: // getters
        const ExprPtr get_cond() { return cond_; }
        cons StmtPtr get_scope() {return scope_; }
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


    void dump(std::ostream& os) const override {
        MSG("dumping for node\n");
        os << *this;
    }

    void accept(Visitor& visitor) const override {
        visitor->VisitForNode(*this);
    }

    public: // getters
        const AssingNode get_init() { return init_; }
        const ExprPtr get_cond() {return cond_;}
        const AssignPtr get_iter() { return iter_; }
        const StmtPtr get_body() { return body_; }
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

    void dump(std::ostream& os) const override {
        MSG("dumping if node\n");
        os << *this;
        MSG("dumped if node\n");
    }

    void accept(Visitor& visitor) const override {
        visitor.VisitIfNode(*this);
    }

    public: // getters
        const ExprPtr get_cond() { return cond_; }
        const StmtPtr get_action() { return action_; }
        const StmtPtr get_else_action() { return else_action_; }
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

    void dump(std::ostream& os) const override {
        MSG("dumping print node\n");
        os << *this;
        MSG("dumped print node\n");
    }

    void accept(Visitor& visitor) const override {
        visitor.VisitPrintNode(*this);
    }

    public: // getters
        const ExprPtr get_expr() { return expr_; }
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

    void dump(std::ostream& os) const override {
        MSG("dumping in node\n");
        os << *this;
    }

    void accept(Visitor& visitor) const override {
        visitor.VisitInNode(*this);
    }
};

class VoidNode final : public ExpressionNode
{
	int  eval(detail::Context&) const override { return 0; }
    void dump(std::ostream&)    const override {}
    void accept(Visitor& visitor) const override {}
};

//     std::ostream& operator<<(std::ostream& os, const ForNode& n)
//     {
//         os << SET_NODE << &n
//         << SET_MRECORD_SHAPE
//         << SET_LABEL  << "FOR"     << SET_ADR  << &n << END_LABEL
//         << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::WHILE_NODE_COLOR << std::dec
//         << END_NODE;
//
//         os << SET_NODE << &n << SET_LINK << SET_NODE << n.init_ << std::endl;
//         os << SET_NODE << &n << SET_LINK << SET_NODE << n.cond_ << std::endl;
//         os << SET_NODE << &n << SET_LINK << SET_NODE << n.iter_ << std::endl;
//         os << SET_NODE << &n << SET_LINK << SET_NODE << n.body_ << std::endl;
//
//         n.init_->dump(os);
//         n.cond_->dump(os);
//         n.iter_->dump(os);
//         n.body_->dump(os);
//
//         return os;
//     }
//
//
//     std::ostream& operator<<(std::ostream& os, const InNode& n)
//     {
//         os << SET_NODE << &n
//         << SET_MRECORD_SHAPE
//         << SET_LABEL  << "IN"      << SET_ADR  << &n << END_LABEL
//         << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::PRINT_NODE_COLOR << std::dec
//         << END_NODE;
//
//         return os;
//     }
//
//     std::ostream& operator<<(std::ostream& os, const ConstantNode& n) {
//         os << SET_NODE << &n
//         << SET_MRECORD_SHAPE
//         << SET_LABEL  << n.val_    << SET_ADR  << &n << END_LABEL
//         << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::CONSTANT_NODE_COLOR << std::dec
//         << END_NODE;
//
//         return os;
//     }
//
//     std::ostream& operator<<(std::ostream& os, const VariableNode& n) {
//         os << SET_NODE << &n
//         << SET_MRECORD_SHAPE
//         << SET_LABEL  << n.name_   << SET_ADR  << &n << END_LABEL
//         << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::VARIABLE_NODE_COLOR << std::dec
//         << END_NODE;
//
//         return os;
//     }
//
//
//     std::ostream& operator<<(std::ostream& os, const BinaryOpNode& n)
//     {
//         os << SET_NODE << &n
//         << SET_MRECORD_SHAPE
//         << SET_LABEL  << "binary: " << BinaryOpNames[static_cast<std::size_t>(n.op_)]    << SET_ADR << &n << END_LABEL
//         << SET_FILLED << SET_COLOR  << std::hex << AST::dump_style::BINARYOP_NODE_COLOR << std::dec
//         << END_NODE;
//
//         os << SET_NODE << &n << SET_LINK << SET_NODE << n.left_  << std::endl;
//         os << SET_NODE << &n << SET_LINK << SET_NODE << n.right_ << std::endl;
//
//         n.left_ ->dump(os);
//         n.right_->dump(os);
//
//         return os;
//     }
//
//
//     std::ostream& operator<<(std::ostream& os, const UnaryOpNode& n)
//     {
//         os << SET_NODE << &n
//         << SET_MRECORD_SHAPE
//         << SET_LABEL  << "unary: " << UnaryOpNames[static_cast<std::size_t>(n.op_)]    << SET_ADR << &n << END_LABEL
//         << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::UNARYOP_NODE_COLOR  << std::dec
//         << END_NODE;
//
//         os << SET_NODE << &n << SET_LINK << SET_NODE << n.operand_ << std::endl;
//
//         n.operand_->dump(os);
//
//         return os;
//     }
//
//
//     std::ostream& operator<<(std::ostream& os, const AssignNode& n)
//     {
//         os << SET_NODE << &n
//         << SET_MRECORD_SHAPE
//         << SET_LABEL  << "ASSIGN '='" << SET_ADR  << &n << END_LABEL
//         << SET_FILLED << SET_COLOR    << std::hex << AST::dump_style::ASSIGN_NODE_COLOR << std::dec
//         << END_NODE;
//
//         os << SET_NODE << &n << SET_LINK << SET_NODE << n.expr_ << std::endl;
//
//         n.expr_->dump(os);
//
//         return os;
//     }
//
//
//     std::ostream& operator<<(std::ostream& os, const WhileNode& n)
//     {
//         os << SET_NODE << &n
//         << SET_MRECORD_SHAPE
//         << SET_LABEL  << "WHILE"   << SET_ADR  << &n << END_LABEL
//         << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::WHILE_NODE_COLOR << std::dec
//         << END_NODE;
//
//         os << SET_NODE << &n << SET_LINK << SET_NODE << n.cond_  << std::endl;
//         os << SET_NODE << &n << SET_LINK << SET_NODE << n.scope_ << std::endl;
//
//         n.cond_ ->dump(os);
//         n.scope_->dump(os);
//
//         return os;
//     }
//
//
//     std::ostream& operator<<(std::ostream& os, const IfNode& n)
//     {
//         os << SET_NODE << &n
//         << SET_MRECORD_SHAPE
//         << SET_LABEL  << "IF"      << SET_ADR  << &n << END_LABEL
//         << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::IF_NODE_COLOR << std::dec
//         << END_NODE;
//
//         os << SET_NODE << &n << SET_LINK << SET_NODE << n.cond_        << std::endl;
//         os << SET_NODE << &n << SET_LINK << SET_NODE << n.action_      << std::endl;
//         os << SET_NODE << &n << SET_LINK << SET_NODE << n.else_action_ << std::endl;
//
//         n.cond_  ->dump(os);
//         n.action_->dump(os);
//         if (n.else_action_) n.else_action_->dump(os);
//
//         return os;
//     }
//
//
//     std::ostream& operator<<(std::ostream& os, const PrintNode& n)
//     {
//         os << SET_NODE << &n
//         << SET_MRECORD_SHAPE
//         << SET_LABEL  << "PRINT"   << SET_ADR  << &n << END_LABEL
//         << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::PRINT_NODE_COLOR << std::dec
//         << END_NODE;
//
//         os << SET_NODE << &n << SET_LINK << SET_NODE << n.expr_ << std::endl;
//
//         n.expr_->dump(os);
//
//         return os;
//     }


} // namespace AST
