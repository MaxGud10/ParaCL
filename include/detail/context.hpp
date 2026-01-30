#pragma once

#include <cstdint>
#include <unordered_map>
#include <string_view>
#include <stdexcept>
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

    Context(std::ostream& _out = std::cout) : out(_out) {}

    VarTable& table_at(int32_t scopeId)
    {
        return varTables_[static_cast<std::size_t>(scopeId)];
    }

    const VarTable& table_at(int32_t scopeId) const
    {
        return varTables_[static_cast<std::size_t>(scopeId)];
    }

    VarTable* find_table_with(Name name)
    {
        for (int32_t scopeId = curScope_; scopeId >= 0; --scopeId)
        {
            auto& t = table_at(scopeId);
            if (t.contains(name))
                return &t;
        }
        return nullptr;
    }

    const VarTable* find_table_with(Name name) const
    {
        for (int32_t scopeId = curScope_; scopeId >= 0; --scopeId)
        {
            auto& t = table_at(scopeId);
            if (t.contains(name))
                return &t;
        }
        return nullptr;
    }

    int assign(Name name, int value)
    {
        if (auto* t = find_table_with(name))
        {
            (*t)[name] = value;
            return value;
        }

        current_table()[name] = value;
        return value;
    }

    VarTable& current_table()
    {
        return varTables_[static_cast<std::size_t>(curScope_)];
    }

    const VarTable &current_table() const
    {
        return varTables_[static_cast<std::size_t>(curScope_)];
    }

    bool try_get(Name name, int &out_value) const
    {
        if (auto* t = find_table_with(name))
        {
            out_value = t->at(name);
            return true;
        }
        
        return false;
    }

    int get_or_throw(Name name) const
    {
        int v = 0;
        if (try_get(name, v))
            return v;

        throw std::runtime_error(std::string("Undeclared variable: ") + std::string(name) + "\n");
    }
 
};

} // namespace detail

} // namespace AST

