#pragma once

#include "INode.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <exception>
#include <unordered_map>
#include <vector>

namespace AST
{

class Scope final : public IScope
{
private:
    std::vector<INodePtr> nodes_;

    const std::unique_ptr<IScope>& parent_;

    std::unordered_map<std::string, int> variableTable_;

public:
    using VarIterator = std::unordered_map<std::string, int>::iterator;

    Scope(const std::unique_ptr<IScope>& parent) : IScope(0), parent_(parent) {}

    const std::unique_ptr<IScope>& resetScope() override
    {
        return parent_;
    }

    void insert_node(const INodePtr& node) override
    {
        nodes_.push_back(node);

        childCount_ = nodes_.size();
    }

    const INodePtr& get_child(size_t i) const override
    {
        return nodes_.at(i);
    }

    void insert_variable(std::string name, int initialValue = 0) override 
    {
        variableTable_.emplace(name, initialValue);
    }

    VarIterator get_variable_iterator(const std::string& name) 
    {
        auto it = variableTable_.find(name);

        if (it != variableTable_.end()) 
        {
            return it;
        }

        return variableTable_.end(); 
        // throw std::runtime_error("Variable not found: " + name);
    }
};

class ConstantNode final : public INode
{
private:
    const int val_;

public:
    ConstantNode(int val) : val_(val) {}

    int eval() const override
    {
        return val_;
    }
};

class VariableNode final : public INode
{
private:
    Scope::VarIterator varIt_;

public:
    VariableNode(Scope::VarIterator varIt) : varIt_(varIt) {}

    void setValue(int value)
    {
        varIt_->second = value;
    }

    int eval() const override
    {
        return varIt_->second;
    }
};

class BinaryOpNode final : public INode
{
private:
    INodePtr left_;
    INodePtr right_;
    BinaryOp op_;

public:
    BinaryOpNode(const INodePtr& left, BinaryOp op, const INodePtr& right) : INode(2), left_(std::move(left)), right_(std::move(right)), op_(op) {}

    INode* get_child(size_t i) const override
    {
        if (i > childCount_)
        {
            // TODO: error handling
        }

        return i == 0 ? left_.get() : right_.get();
    }

    int eval() const override
    {
        int leftVal  = left_ ->eval();
        int rightVal = right_->eval();

        switch (op_) 
        {
            case BinaryOp::ADD:
                return leftVal + rightVal;

            case BinaryOp::SUB:
                return leftVal - rightVal;

            case BinaryOp::MUL:
                return leftVal * rightVal;

            case BinaryOp::DIV:
                if (rightVal == 0) // TODO: error handle
                return leftVal / rightVal;

            case BinaryOp::MOD:
                return leftVal % rightVal;

            case BinaryOp::LS:
                return leftVal < rightVal;

            case BinaryOp::GR:
                return leftVal > rightVal;

            case BinaryOp::LS_EQ:
                return leftVal <= rightVal;

            case BinaryOp::GR_EQ:
                return leftVal >= rightVal;

            case BinaryOp::EQ:
                return leftVal == rightVal;

            case BinaryOp::NOT_EQ:
                return leftVal != rightVal;

            case BinaryOp::AND:
                return leftVal && rightVal;

            case BinaryOp::OR:
                return leftVal || rightVal;
                
            default:
                // TODO: handle error
            
        }

    }
};

class UnaryOpNode final : public INode
{
private:
    INodePtr operand_;
    UnaryOp op_;

public:
    UnaryOpNode(const INodePtr& operand, UnaryOp op) : INode(1), operand_(std::move(operand)), op_(op) {}

    INode* get_child(size_t i) const override
    {
        // TODO: do we need to handle the input?
        return operand_.get();
    }

    int eval() const override
    {
        int operandVal = operand_->eval();

        switch (op_)
        {
            case UnaryOp::NEG:
                return - operandVal;
            
            case UnaryOp::NOT:
                return ! operandVal;

            default:
                // TODO: error handle
        }
    }
};

class AssignNode final : public INode
{
private:
    std::unique_ptr<VariableNode> dest_;
    INodePtr expr_;

public:
    AssignNode(const std::unique_ptr<VariableNode>& dest_, const INodePtr& expr_) : dest_(std::move(dest)), expr_(std::move(expr)) {}
    
    INode* get_child(size_t i) const override
    {
        if (i > childCount_)
        {
            // TODO: error handling
        }

        return i == 0 ? dest_.get() : expr_.get();
    }

    int eval() const override
    {
        int value = expr_->eval();

        dest_->setValue(value);

        return value;
    }
};

class WhileNode final : public INode
{
private:
    INodePtr cond_;
    INodePtr scope_;

public:
    WhileNode(const INodePtr& cond, const INodePtr& scope) : cond_(std::move(cond)), scope_(std::move(scope)) {}

    INode* get_child(size_t i) const override
    {
        if (i > childCount_)
        {
        }
    }

    int eval() const override
    {
        int result = 0;
        
        while (cond_->eval())
        {
            result = scope_->eval();
        }
        
        return result;
    }
};

class IfNode final : public INode
{
private:
    INodePtr cond_;
    INodePtr ifScope_;
    INodePtr elseScope_;

public:
    IfNode(const INodePtr& cond, const INodePtr& ifScope, const INodePtr& elseScope = nullptr) : cond_(std::move(cond)), ifScope_(std::move(ifScope)), elseScope_(std::move(elseScope)) {}

    INode* get_child(size_t i) const override;

    int eval() const override
    {
        if (cond_->eval())
        {
            return ifScope_->eval();
        }
        else if (elseScope_)
        {
            return elseScope_->eval();
        }

        return 0;
    }
};

class PrintNode final : public INode
{
private:
    INodePtr expr_;

public:
    PrintNode(const INodePtr& expr) : expr_(std::move(expr)) {}

    INode* get_child(size_t i) const override;

    int eval() const override
    {
        int value = expr->eval();

        std::cout << value << std::endl;

        return value;
    }
};


class InNode final : public INode
{
public:
    InNode() = default;

    INode* get_child(size_t i) const override;

    int eval() const override
    {
        int value = 0;

        std::cin >> value;

        if (! std::cin.good())
        {
            // TODO: error handle
        }

        return value;
    }

    ~InNode() = default;
};

} // namespace AST
