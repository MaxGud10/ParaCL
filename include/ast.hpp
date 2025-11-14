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
private:
    using VarTable = std::unordered_map<std::string, int>;

public:
    std::unique_ptr<ScopeNode> globalScope;

private:
    std::vector<VarTable>      VarTables_;

    detail::Context ctx;


public:
	AST(std::ostream& out = std::cout) : ctx(out) {}

    void eval()
    {
		MSG("Evaluating global scope\n");
        globalScope->eval(ctx);
    }

    void RecursiveDump()
    {

    }
};

} // namespace AST
