#pragma once

#include "node.hpp"
#include "log.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace AST
{

class AST final
{
public:
    ScopeNode* globalScope = nullptr;

private:
    detail::Context ctx;

public:
	AST(std::ostream& out = std::cout) : ctx(out) {}

    void accept(Visitor& visitor) {
        visitor.Visit(*globalScope);
    }

    detail::Context& getCtx() { return ctx; }
};

} // namespace AST
