#include <string>
#include <chrono>
#include <cstring>
#include <fstream>

#include "ast.hpp"
#include "driver.hpp"
#include "log.h"
#include "inode.hpp"

const std::string DUMP_DIR = "./dumps/";
const std::string DOT_DIR  = "dot/";

std::string generateFileName(const std::string& prefix="dot", const std::string& extension="dot") {
    auto now = std::chrono::system_clock::now();
    auto time_now = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    // ss << std::put_time(std::localtime(&time_now), "%Y-%m-%d_%H-%M-%S");
    return DUMP_DIR + DOT_DIR + prefix + "_" + ss.str() + "." + extension;
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

    LOG("global statements amount: {}\n", drv.ast.globalScope->nstms());
    if (status == 0)
    {
        LOG("global statements amount: {}\n", drv.ast.globalScope->nstms());
        drv.ast.eval();
    }

    // handling dump flag
    for (int i = 0; i < argc; ++i) {
        if (!strcmp(argv[i], "--dump")) {
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
