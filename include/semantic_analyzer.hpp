#pragma once

#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>
#include <stdexcept>

#include "ast.hpp"
#include "detail/ivisitor.hpp"
#include "semantic_error.hpp"
#include "source_location.hpp"

class SemanticAnalyzer final : public Visitor
{
private:
    using Scope = std::unordered_set<std::string_view>;

    const std::unordered_map<const AST::INode*, AST::SourceLocation>& locations_;

    std::vector<Scope> scopes_;
    int                functionDepth_ = 0;

public:
    explicit SemanticAnalyzer(const std::unordered_map<const AST::INode*, AST::SourceLocation>& locations)
        : locations_(locations)
    {
        push_scope(); // global scope
    }

    void analyze(AST::AST& ast)
    {
        if (!ast.globalScope)
            throw std::runtime_error("SemanticAnalyzer: AST is empty");

        ast.globalScope->accept(*this);
    }

private:
    void push_scope()
    {
        scopes_.emplace_back();
    }

    void pop_scope()
    {
        if (scopes_.empty())
            throw std::runtime_error("SemanticAnalyzer: pop_scope on empty stack");
        scopes_.pop_back();
    }

    bool is_declared(std::string_view name) const
    {
        for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it)
        {
            if (it->find(name) != it->end())
                return true;
        }
        return false;
    }

    bool exists_in_current_scope(std::string_view name) const
    {
        if (scopes_.empty())
            return false;
        return scopes_.back().find(name) != scopes_.back().end();
    }

    void declare_in_current_scope(std::string_view name)
    {
        if (scopes_.empty())
            throw std::runtime_error("SemanticAnalyzer: no active scope");
        scopes_.back().insert(name);
    }

    const AST::SourceLocation& loc_or_dummy(const AST::INode& node) const
    {
        auto it = locations_.find(&node);
        static const AST::SourceLocation dummy{};
        if (it == locations_.end())
            return dummy;
        return it->second;
    }

    [[noreturn]] void error(const AST::INode& node, const std::string& message) const
    {
        throw SemanticError(loc_or_dummy(node), message);
    }

    void assign_name(std::string_view name)
    {
        for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it)
        {
            if (it->find(name) != it->end())
                return;
        }

        declare_in_current_scope(name);
    }

public:
    void Visit(AST::ScopeNode& n) override
    {
        push_scope();

        for (auto* stmt : n.get_children())
            stmt->accept(*this);

        pop_scope();
    }

    void Visit(AST::ConstantNode&) override {}

    void Visit(AST::InNode&) override {}

    void Visit(AST::VariableNode& n) override
    {
        if (!is_declared(n.get_name()))
            error(n, "undeclared variable '" + std::string(n.get_name()) + "'");
    }

    void Visit(AST::AssignNode& n) override
    {
        n.get_expr()->accept(*this);
        assign_name(n.get_dest()->get_name());
    }

    void Visit(AST::PrintNode& n) override
    {
        n.get_expr()->accept(*this);
    }

    void Visit(AST::UnaryOpNode& n) override
    {
        n.get_operand()->accept(*this);
    }

    void Visit(AST::BinaryOpNode& n) override
    {
        n.get_left() ->accept(*this);
        n.get_right()->accept(*this);
    }

    void Visit(AST::IfNode& n) override
    {
        n.get_cond()  ->accept(*this);
        n.get_action()->accept(*this);

        if (n.get_else_action())
            n.get_else_action()->accept(*this);
    }

    void Visit(AST::WhileNode& n) override
    {
        n.get_cond() ->accept(*this);
        n.get_scope()->accept(*this);
    }

    void Visit(AST::ForNode& n) override
    {
        push_scope();

        if (n.get_init())
            n.get_init()->accept(*this);

        if (n.get_cond())
            n.get_cond()->accept(*this);

        if (n.get_iter())
            n.get_iter()->accept(*this);

        if (n.get_body())
            n.get_body()->accept(*this);

        pop_scope();
    }

    void Visit(AST::FunctionNode& n) override
    {
        push_scope();
        ++functionDepth_;

        if (n.has_internal_name())
            declare_in_current_scope(n.get_internal_name());

        for (auto param : n.get_params())
            declare_in_current_scope(param);

        n.get_body()->accept(*this);

        --functionDepth_;
        pop_scope();
    }

    void Visit(AST::CallNode& n) override
    {
        n.get_callee()->accept(*this);

        for (auto* arg : n.get_args())
            arg->accept(*this);
    }

    void Visit(AST::ReturnNode& n) override
    {
        if (functionDepth_ <= 0)
            error(n, "return outside function");

        if (n.get_expr())
            n.get_expr()->accept(*this);
    }

    void Visit(AST::ExpressionNode&)           override {}
    void Visit(AST::StatementNode&)            override {}
    void Visit(AST::ConditionalStatementNode&) override {}

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