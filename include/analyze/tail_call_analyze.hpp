// analyze/tail_call_analyze.hpp
#pragma once

#include "node.hpp"
#include "detail/ivisitor.hpp"

struct TailCallSignal
{
    std::vector<AST::detail::Value> args;
};

class TailCallAnalyzer : public Visitor {
public:
    explicit TailCallAnalyzer(const AST::detail::Name funcName)
        : funcName_(funcName), inTailPosition_(true) {}

    void analyze(AST::INode* node) {
        MSG("STARTED ANALYZING FOR TAIL RECURSION\n");
        node->accept(*this);
    }

private:
    std::string funcName_;
    bool inTailPosition_;


    void Visit(AST::ScopeNode& scopeNode) override {
        const auto& children = scopeNode.get_children();

        for (size_t i = 0; i < children.size(); ++i) {
            bool wasTail = inTailPosition_;
            inTailPosition_ = (i == children.size() - 1) && wasTail;

            children[i]->accept(*this);

            inTailPosition_ = wasTail;
        }
    }

    void Visit(AST::ReturnNode& returnNode) override {
        bool wasTail = inTailPosition_;
        inTailPosition_ = true;

        if (auto* expr = returnNode.get_expr()) {
            expr->accept(*this);
        }

        inTailPosition_ = wasTail;
    }

        static std::optional<std::string_view> get_callee_name(const AST::ExprPtr& expr) {
            MSG("Trying to get callee name...\n");
            if (!expr) return std::nullopt;
            if (auto* func = dynamic_cast<AST::FunctionNode*>(expr)) {

                MSG("Successful dyn_cast to function node!\n");
                LOG("Function has internal_name {}\n", func->get_internal_name());

                return func->get_internal_name();
            }

            if (auto* var = dynamic_cast<AST::VariableNode*>(expr)) {

                MSG("Successful dyn_cast to variable!\n");
                LOG("Function has internal_name {}\n", var->get_name());

                return var->get_name();
            }

        return std::nullopt;
    }

    void Visit(AST::CallNode& callNode) override {
        bool wasTail = inTailPosition_;

        for (auto* arg : callNode.get_args()) {
            inTailPosition_ = false;
            arg->accept(*this);
        }

        inTailPosition_ = wasTail;

        if (inTailPosition_) {
            auto calleeName = get_callee_name(callNode.get_callee());
            LOG("Got calleeName = {}, funcName = {}\n", *calleeName, funcName_);
            if (calleeName && *calleeName == funcName_) {
                MSG("setting tail call to true... \n");
                callNode.set_tail_call(true);
                LOG("TCO: marked '{}' as tail call\n", *calleeName);
            }
        }
    }


    void Visit(AST::IfNode& ifNode) override {
        bool wasTail = inTailPosition_;
        inTailPosition_ = false;
        ifNode.get_cond()->accept(*this);

        inTailPosition_ = wasTail;
        ifNode.get_action()->accept(*this);

        if (auto* elseAction = ifNode.get_else_action()) {
            inTailPosition_ = wasTail;
            elseAction->accept(*this);
        }
    }


    void Visit(AST::BinaryOpNode& binaryOpNode) override {
        bool wasTail = inTailPosition_;
        inTailPosition_ = false;
        binaryOpNode.get_left()->accept(*this);
        binaryOpNode.get_right()->accept(*this);
        inTailPosition_ = wasTail;
    }

    void Visit(AST::FunctionNode& ) override {}

    void Visit(AST::ConstantNode&) override {}
    void Visit(AST::VariableNode&) override {}
    void Visit(AST::AssignNode& assignNode) override {
        bool wasTail = inTailPosition_;
        inTailPosition_ = false;
        assignNode.get_expr()->accept(*this);
        inTailPosition_ = wasTail;
    }
    void Visit(AST::PrintNode& printNode) override {
        bool wasTail = inTailPosition_;
        inTailPosition_ = false;
        printNode.get_expr()->accept(*this);
        inTailPosition_ = wasTail;
    }
    void Visit(AST::InNode&) override {}
    void Visit(AST::WhileNode& whileNode) override {
        bool wasTail = inTailPosition_;
        inTailPosition_ = false;
        whileNode.get_cond()->accept(*this);
        whileNode.get_scope()->accept(*this);
        inTailPosition_ = wasTail;
    }
    void Visit(AST::ForNode& forNode) override {
        bool wasTail = inTailPosition_;
        inTailPosition_ = false;
        if (forNode.get_init()) forNode.get_init()->accept(*this);
        forNode.get_cond()->accept(*this);
        forNode.get_body()->accept(*this);
        if (forNode.get_iter()) forNode.get_iter()->accept(*this);
        inTailPosition_ = wasTail;
    }
    void Visit(AST::UnaryOpNode& unaryOpNode) override {
        bool wasTail = inTailPosition_;
        inTailPosition_ = false;
        unaryOpNode.get_operand()->accept(*this);
        inTailPosition_ = wasTail;
    }

    void Visit(AST::ConditionalStatementNode&) override {}
    void Visit(AST::StatementNode&) override {}
    void Visit(AST::ExpressionNode&) override {}

    void Visit(const AST::StatementNode&) const override {}
    void Visit(const AST::ExpressionNode&) const override {}
    void Visit(const AST::ConditionalStatementNode&) const override {}
    void Visit(const AST::ScopeNode&) const override {}
    void Visit(const AST::ForNode&) const override {}
    void Visit(const AST::ConstantNode&) const override {}
    void Visit(const AST::AssignNode&) const override {}
    void Visit(const AST::WhileNode&) const override {}
    void Visit(const AST::IfNode&) const override {}
    void Visit(const AST::InNode&) const override {}
    void Visit(const AST::VariableNode&) const override {}
    void Visit(const AST::BinaryOpNode&) const override {}
    void Visit(const AST::UnaryOpNode&) const override {}
    void Visit(const AST::PrintNode&) const override {}
    void Visit(const AST::FunctionNode&) const override {}
    void Visit(const AST::CallNode&) const override {}
    void Visit(const AST::ReturnNode&) const override {}
};
