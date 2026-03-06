#pragma once

#include <memory>
#include <string_view>
#include <variant>
#include <vector>
#include <unordered_map>

namespace AST
{
class ScopeNode;

namespace detail
{

using Name        = std::string_view;

struct Frame;
using FramePtr    = std::shared_ptr<Frame>;

struct Function;
using FunctionPtr = std::shared_ptr<Function>;
using Value       = std::variant<int, FunctionPtr>;

struct Frame
{
    std::unordered_map<Name, Value> vars;
    FramePtr                        parent{};
};

struct Function
{
    std::vector<Name>     params;
    AST::ScopeNode       *body = nullptr;
    std::weak_ptr<Frame>  env{};
    Name                  internalName{};

    bool hasInternalName = false;
};

} // namespace detail
} // namespace AST