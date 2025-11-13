#include <string>

#include "ast.hpp"
#include "driver.hpp"
#include "log.h"

int main(int argc, char **argv)
{
    MSG("MACROSES:\n");
    LOG("YYDEBUG: {}\n", YYDEBUG);
    LOG("YY_FLEX_DEBUG: {}\n", YY_FLEX_DEBUG);

    int status = 0;

    Driver drv;

    for (int i = 1; i < argc; ++i)
        status = drv.parse(argv[i]);

    LOG("global statements amount: {}\n", drv.ast.globalScope->nstms());
    if (status == 0)
    {
        drv.ast.eval();
    }

    // drv.ast.eval();

    return status;
}
