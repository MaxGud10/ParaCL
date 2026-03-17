#pragma once

#include <stdexcept>
#include <string>
#include "source_location.hpp"

class SemanticError : public std::runtime_error
{
public:
    SemanticError(const AST::SourceLocation& loc, const std::string& message)
        : std::runtime_error(format(loc, message)) {}

private:
    static std::string format(const AST::SourceLocation& loc, const std::string& message)
    {
        if (loc.valid())
            return loc.to_string() + ": " + message;
        return "semantic error: " + message;
    }
};