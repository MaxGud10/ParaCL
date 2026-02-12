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

    void accept(Visitor& visitor) override {
        visitor.Visit(*this);
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

    void accept(Visitor& visitor) override {
        visitor.Visit(*this);
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

    void accept(Visitor& visitor) override {
        visitor.Visit(*this);
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

    void accept(Visitor& visitor) override {
        visitor.Visit(*this);
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

    void accept(Visitor& visitor) override {
        visitor.Visit(*this);
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

    void accept(Visitor& visitor) override {
        visitor.Visit(*this);
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

    void accept(Visitor& visitor) override {
        visitor.Visit(*this);
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

    void accept(Visitor& visitor) override {
        visitor.Visit(*this);
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

    void accept(Visitor& visitor) override {
        visitor.Visit(*this);
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

    void accept(Visitor& visitor) override {
        visitor.Visit(*this);
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

    void accept(Visitor& visitor) override {
        visitor.Visit(*this);
    }

    void accept(const Visitor& visitor) const override {
        visitor.Visit(*this);
    }
};

class VoidNode final : public ExpressionNode
{
    void accept(Visitor& visitor) override {}

    void accept(const Visitor& visitor) const override {}
};

} // namespace AST
