#pragma once 

#include <memory>
#include <cstddef>
#include <string>
#include <unordered_map>

#include <context.hpp>

namespace AST
{

class INode;

using INodePtr = std::unique_ptr<INode>;
using VarIterator = std::unordered_map<std::string, int>::iterator;

template<class T>
using ObserverPtr = T*;

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
public:
    virtual int eval(detail::Context& ctx) const = 0;

    virtual ~INode() = default;
};

} // namespace AST

