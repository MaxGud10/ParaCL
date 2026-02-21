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

    void accept(Visitor& visitor)
    {
        if (!globalScope)
            throw std::runtime_error("AST is empty");
        globalScope->accept(visitor);
    }

    void accept(const Visitor& visitor) const
    {
        if (!globalScope)
            throw std::runtime_error("AST is empty");
        globalScope->accept(visitor);
    }

          detail::Context& getCtx()       { return ctx; }
    const detail::Context& getCtx() const { return ctx; }
};

} // namespace AST
