#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>

namespace AST
{

namespace detail
{

class Context final
{
public:
    using VarTable = std::unordered_map<std::string, int>;

public:
    std::vector<VarTable> varTables_;
    int32_t               curScope_ = -1;
	std::ostream&         out;

public:
	Context(std::ostream& _out = std::cout) : out(_out) {}
};

} // namespace detail

} // namespace AST

