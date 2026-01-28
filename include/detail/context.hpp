#pragma once

#include <cstdint>
#include <unordered_map>
#include <set>
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
    using Name     = std::string_view;
    using VarTable = std::unordered_map<Name, int>;  

    std::vector<VarTable> varTables_;
    int32_t               curScope_ = -1;
    std::ostream&         out;

private:
    std::set<std::string> namePool_; 

public:
    Context(std::ostream& _out = std::cout) : out(_out) {}

    Name intern(std::string_view s) 
    {
        auto [it, inserted] = namePool_.emplace(s);
        return *it; 
    }
};

} // namespace detail

} // namespace AST

