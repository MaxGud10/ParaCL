#pragma once

#include <array>
#include <cstddef>
#include <memory>
#include <string_view>
#include <unordered_map>

#include <context.hpp>
#include <ivisitor.hpp>

namespace AST
{

class INode;

using INodePtr    = std::unique_ptr<INode>;
using VarIterator = std::unordered_map<std::string, int>::iterator;

template<class T>
using ObserverPtr = T*;

enum class BinaryOp : std::uint8_t
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
    BIT_AND,
    BIT_OR,
    COUNT
};

inline constexpr std::size_t numBinaryOp =
    static_cast<std::size_t>(BinaryOp::COUNT);

inline constexpr  std::array<std::string_view, numBinaryOp> BinaryOpNames =
{{
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
    "OR",
    "BIT_AND",
    "BIT_OR"
}};

enum class UnaryOp: std::uint8_t
{
    NEG = 0,
    NOT,
    COUNT
};

inline constexpr std::size_t numUnaryOp =
    static_cast<std::size_t>(UnaryOp::COUNT);

inline constexpr std::array<std::string_view, numUnaryOp> UnaryOpNames =
{{
    "NEG",
    "NOT"
}};


class INode
{
public:
    virtual void accept(Visitor& visitor) = 0;
    virtual void accept(const Visitor& visitor) const = 0;
    virtual ~INode() = default;

};

} // namespace AST
