#pragma once

#include <string>
#include <sstream>

namespace AST
{

struct SourceLocation
{
    std::string file;
    int         line = 0;
    int         column = 0;

    bool valid() const { return line > 0 && column > 0; }

    std::string to_string() const
    {
        std::ostringstream oss;
        if (!file.empty())
            oss << file << ":";
        oss << line << ":" << column;
        
        return oss.str();
    }
};

} // namespace AST