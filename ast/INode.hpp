#pragma once

#include <algorithm>
#include <memory>
#include <cstddef>
#include <string>

namespace AST 
{
 
class INode;

using INodePtr = std::unique_ptr<INode>;

enum class BinaryOp
{
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    GR,
    LS,
    EQ,
    GR_EQ,
    LS_EQ,
    NOT_EQ,
    AND,
    OR,
};

enum class UnaryOp
{
    NEG,
    NOT,
};



class INode
{
protected:
    size_t childCount_;

    INode(size_t childCount = 0) : childCount_(childCount) {} 

public:
    virtual const INodePtr& get_child(size_t i) const = 0;

    virtual int eval() const = 0;

    size_t get_сhild_сount() const
    {
        return childCount_;
    }

    virtual ~INode() = default;
};

class IScope : public INode
{
protected:
    IScope(size_t childCount) : INode(childCount) {}

public:
    virtual const std::unique_ptr<IScope>& reset_scope()               = 0;
    virtual const std::string& get_variable (const std::string& name)  = 0;
    virtual void insert_variable (std::string name, int initialValue)  = 0;
    virtual void insert_node (const INodePtr& node)                    = 0;
};

} // namespace AST


