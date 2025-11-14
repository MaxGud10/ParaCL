#pragma once

#include <memory>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <array>

#include <context.hpp>

namespace AST
{

class INode;

using INodePtr = std::unique_ptr<INode>;
using VarIterator = std::unordered_map<std::string, int>::iterator;

template<class T>
using ObserverPtr = T*;

const size_t numBinaryOp = 13;
enum class BinaryOp
{
    ADD = 0,
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

static const std::array<std::string, numBinaryOp> BinaryOpNames = {{
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "MOD",
    "GR",
    "LS",
    "EQ",
    "GR_EQ",
    "LS_EQ",
    "NOT_EQ",
    "AND",
    "OR"
}};

const size_t numUnaryOp = 2;
enum class UnaryOp
{
    NEG = 0,
    NOT,
};

static const std::array<std::string, numUnaryOp> UnaryOpNames = {{
    "NEG",
    "NOT"
}};
//
// const std::string enumToStringBinary(enum BinaryOp op_) {
//     return BinaryOpNames[static_cast<std::size_t>(op_)];
// }
//
// const std::string enumToStringUnary(enum UnaryOp op_) {
//     return UnaryOpNames[static_cast<std::size_t>(op_)];
// }

class INode
{
public:
    virtual int eval(detail::Context& ctx) const = 0;

    virtual ~INode() = default;
};

} // namespace AST

