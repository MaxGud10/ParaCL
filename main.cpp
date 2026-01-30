#include <string>
#include <chrono>
#include <cstring>
#include <fstream>

#include "ast.hpp"
#include "driver.hpp"
#include "log.h"
#include "inode.hpp"

constexpr std::string_view DUMP_DIR = "./dumps/";
constexpr std::string_view DOT_DIR  = "dot/";

std::string generateFileName(std::string_view prefix = "dot", std::string_view extension = "dot") 
{
    auto now      = std::chrono::system_clock::now();
    auto time_now = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    // ss << std::put_time(std::localtime(&time_now), "%Y-%m-%d_%H-%M-%S");

    std::string res;
    res.reserve(DUMP_DIR.size() + DOT_DIR.size() + prefix.size() + extension.size() + 32);

    res += DUMP_DIR;
    res += DOT_DIR;
    res.append(prefix);
    res += "_";
    res += ss.str();
    res += ".";
    res.append(extension);

    return res;
}

int main(int argc, char **argv)
{
    MSG("MACROSES:\n");
    LOG("YYDEBUG: {}\n", YYDEBUG);
    LOG("YY_FLEX_DEBUG: {}\n", YY_FLEX_DEBUG);

    int status = 0;

    Driver drv;

    if (argc == 1)
        status = drv.parse("-");
    else
    {
        for (int i = 1; i < argc; ++i)
        {
            if (strcmp("--dump", argv[i]))
            {
                status = drv.parse(argv[i]);
                if (status != 0)
                    break;
            }
        }
    }

    if (status != 0)
    {
        std::cerr << "Parsing failed" << std::endl;
        return status;
    }

    if (!drv.ast.globalScope || drv.ast.globalScope->nstms() == 0)
    {
        return 0;
    }

    LOG("global statements amount: {}\n", drv.ast.globalScope->nstms());

    try
    {
        drv.ast.eval();
    }
    catch (const std::runtime_error& e)
    {
        auto pos = drv.location.begin;

        std::cerr << drv.file    << ":" << pos.line << "." << pos.column
                  << ": error: " << e.what()        << "\n";

        int lineNum = 0;
        lineNum = pos.line - 1 < drv.source_lines.size() ? 1 : 0;
        lineNum = pos.line - 2 < drv.source_lines.size() ? 2 : 0;
        if (lineNum != 0)
        {
            const std::string& src = drv.source_lines[pos.line - lineNum];
            std::cerr << "  " << pos.line << " | " << src << "\n";
            std::cerr << "    | ";
            size_t caret_col = pos.column > 1 ? pos.column - 2 : 0;

            std::cerr << std::string(caret_col, ' ') << "^\n";
        }

        return 1;
    }



    // if (status == 0 && drv.ast.globalScope != nullptr)
    // {
    //     LOG("global statements amount: {}\n", drv.ast.globalScope->nstms());

    //     if (drv.ast.globalScope->nstms() > 0)
    //       drv.ast.eval();
    //     else
    //         std::cout << "No statements to execute" << std::endl;
    // }
    // else
    //     std::cerr << "Parsing failed or AST not created" << std::endl;

    // handling dump flag
    for (int i = 0; i < argc; ++i)
    {
        if (!strcmp(argv[i], "--dump"))
        {
            std::string fileName = generateFileName();
            std::cout << "generatedfileName" << fileName << std::endl;
            std::ofstream outFile(fileName);
            if (outFile.is_open())
            {
                drv.ast.dump(outFile);
                std::cout << "dumped" << std::endl;
                outFile.close();
                return status;
            }
        }
    }

    return status;
}
