#pragma once

#include <iostream>
#include <stack>
#include "detail/ivisitor.hpp"
#include "node.hpp"
#include "detail/context.hpp"

class TreeTraverse : public Visitor {
    private:
        AST::detail::Context& ctx_;
        std::stack<int> eval_stack;

    public:
    TreeTraverse(AST::detail::Context& ctx) : ctx_(ctx) {}
    ~TreeTraverse() = default;

    int get_result() const {
        if (eval_stack.empty()) return 0;
        return eval_stack.top();
    }

    void Visit(AST::ScopeNode& n) override {
        ++ctx_.curScope_;

        ctx_.varTables_.push_back(AST::detail::Context::VarTable());

		LOG("ctx.curScope_ = {}\n", ctx_.curScope_);
		LOG("ctx.varTables_ size = {}\n", ctx_.varTables_.size());

		MSG("Scopes children:\n");
        auto children_ = n.get_children();
		for ([[maybe_unused]]const auto& child : children_)
        {
			LOG("{}\n", static_cast<const void*>(child));
        }

        int last_result = 0;
        for (const auto& child : children_)
        {
			LOG("Evaluating {}\n", static_cast<const void*>(child));
            child->accept(*this);
            if (!eval_stack.empty()) {
                last_result = eval_stack.top();
                eval_stack.pop();
            }
        }

        ctx_.varTables_.pop_back();

        --ctx_.curScope_;
         eval_stack.push(last_result);

		LOG("ctx.curScope_ = {}\n", ctx_.curScope_);
		LOG("ctx.varTables_ size = {}\n", ctx_.varTables_.size());

		return;
    }

    void Visit(AST::ConstantNode& n) override {
        LOG("Evaluating constant: {}\n", n.get_val());
        eval_stack.push(n.get_val());
    }

    void Visit(AST::AssignNode& n) override {
        MSG("Evaluating assignment\n");
        n.get_expr()->accept(*this);

        int value = eval_stack.top(); eval_stack.pop();
        ctx_.assign(n.get_dest()->get_name(), value);
        eval_stack.push(value);
    }

    void Visit(AST::WhileNode& n) override {
        int result = 0;

        n.get_cond()->accept(*this);
        int cond = eval_stack.top(); eval_stack.pop();

        while (cond) {
            n.get_scope()->accept(*this);

            if (!eval_stack.empty()) {
                result = eval_stack.top(); eval_stack.pop();
            }

            n.get_cond()->accept(*this);
            cond = eval_stack.top(); eval_stack.pop();
        }

        eval_stack.push(result);
    }

    void Visit(AST::IfNode& n) override {
        n.get_cond()->accept(*this);
        int cond_res = eval_stack.top(); eval_stack.pop();

        if (cond_res) {
            n.get_action()->accept(*this);
            return;
        } else if (n.get_else_action()) {
            n.get_else_action()->accept(*this);
        } else {
            eval_stack.push(0);
            return;
        }

        if (eval_stack.empty())
            eval_stack.push(0);

    }

    void Visit(AST::PrintNode& n) override {
		MSG("Evaluation print\n");

        n.get_expr()->accept(*this);
        int value = eval_stack.top(); eval_stack.pop();

        ctx_.out << value << std::endl;

        eval_stack.push(value);
    }

    void Visit(AST::InNode& n) override {
        int value = 0;

        std::cin >> value;

        if (std::cin.fail())
        {
            throw std::runtime_error("Incorrect input");
        }

        eval_stack.push(value);
    }

    void Visit(AST::VariableNode& n) override {
        int value = ctx_.get_value(n.get_name());
        eval_stack.push(value);
    }

    void Visit(AST::BinaryOpNode& n) override {
    MSG("Evaluating Binary Operation\n");

    switch (n.get_op()) {
        case AST::BinaryOp::AND: {
            n.get_left()->accept(*this);
            int leftVal = eval_stack.top(); eval_stack.pop();

            if (!leftVal) {
                eval_stack.push(0);
                return;
            }

            n.get_right()->accept(*this);
            int rightVal = eval_stack.top(); eval_stack.pop();
            eval_stack.push(rightVal ? 1 : 0);
            return;
        }

        case AST::BinaryOp::OR: {
            n.get_left()->accept(*this);
            int leftVal = eval_stack.top(); eval_stack.pop();

            if (leftVal) {
                eval_stack.push(1);
                return;
            }

            n.get_right()->accept(*this);
            int rightVal = eval_stack.top(); eval_stack.pop();
            eval_stack.push(rightVal ? 1 : 0);
            return;
        }

        case AST::BinaryOp::DIV: {
            n.get_left()->accept(*this);
            int leftVal = eval_stack.top(); eval_stack.pop();

            n.get_right()->accept(*this);
            int rightVal = eval_stack.top(); eval_stack.pop();

            if (rightVal == 0)
                throw std::runtime_error("Divide by zero");

            eval_stack.push(leftVal / rightVal);
            return;
        }

        default: {
            n.get_left()->accept(*this);
            int leftVal = eval_stack.top(); eval_stack.pop();

            n.get_right()->accept(*this);
            int rightVal = eval_stack.top(); eval_stack.pop();

            int result;
            switch (n.get_op()) {
                case AST::BinaryOp::ADD:    result = leftVal + rightVal; break;
                case AST::BinaryOp::SUB:    result = leftVal - rightVal; break;
                case AST::BinaryOp::MUL:    result = leftVal * rightVal; break;
                case AST::BinaryOp::MOD:    result = leftVal % rightVal; break;

                case AST::BinaryOp::LS:     result = leftVal <  rightVal; break;
                case AST::BinaryOp::GR:     result = leftVal >  rightVal; break;
                case AST::BinaryOp::LS_EQ:  result = leftVal <= rightVal; break;
                case AST::BinaryOp::GR_EQ:  result = leftVal >= rightVal; break;
                case AST::BinaryOp::EQ:     result = leftVal == rightVal; break;
                case AST::BinaryOp::NOT_EQ: result = leftVal != rightVal; break;

                case AST::BinaryOp::BIT_AND: result = leftVal & rightVal; break;
                case AST::BinaryOp::BIT_OR:  result = leftVal | rightVal; break;

                default:
                    throw std::runtime_error("Unknown binary operation");
            }

            LOG("It's {}\n", result);
            eval_stack.push(result);
            return;
        }
    }
}
    void Visit(AST::UnaryOpNode& n) override {

        n.get_operand()->accept(*this);
        int operandVal = eval_stack.top(); eval_stack.pop();

        switch (n.get_op())
        {
            case AST::UnaryOp::NEG:
                eval_stack.push(-operandVal);
                return;

            case AST::UnaryOp::NOT:
                eval_stack.push(!operandVal);
                return;

            default:
                throw std::runtime_error("Unknown unary operation");
        }
    }

    void Visit(AST::ForNode& n) override {
        int result = 0;

        if (n.get_init()) {
            n.get_init()->accept(*this);
        }

        n.get_cond()->accept(*this);
        int cond = eval_stack.top(); eval_stack.pop();
        for ( ; cond; )
        {
            n.get_body()->accept(*this);
            if (!eval_stack.empty()) {
                result = eval_stack.top(); eval_stack.pop();
            }

            if (n.get_iter()) {
                n.get_iter()->accept(*this);
            }
            if (!eval_stack.empty()) eval_stack.pop();

            n.get_cond()->accept(*this);
            cond = eval_stack.top(); eval_stack.pop();
        }

        eval_stack.push(result);
        return;
    }

    void Visit(const AST::ScopeNode& node) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
    void Visit(const AST::ForNode& node) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
    void Visit(const AST::ConstantNode& node) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
    void Visit(const AST::AssignNode& node) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
    void Visit(const AST::WhileNode& node) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
    void Visit(const AST::IfNode& node) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
    void Visit(const AST::InNode& node) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
    void Visit(const AST::VariableNode& node) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
    void Visit(const AST::BinaryOpNode& node) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
    void Visit(const AST::UnaryOpNode& node) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
    void Visit(const AST::PrintNode& node) const override { throw std::runtime_error("TreeTraverse::Visit const not implemented"); }
};

namespace TestUtils {
    inline int evaluate(AST::INode& node, AST::detail::Context& ctx) {
        TreeTraverse visitor(ctx);
        node.accept(visitor);
        return visitor.get_result();
    }
}


