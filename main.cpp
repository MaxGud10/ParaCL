#include "driver.hpp"
#include "log.h"

#include <iostream>

int main(int argc, char **argv)
{
    int res = 0;

	MSG("MACROSES:\n");
	LOG("{}\n", YYDEBUG);
	LOG("{}\n", YY_FLEX_DEBUG);

    Driver drv;

    for (int i = 1; i < argc; ++i)
        if (!drv.parse(argv[i]))
            std::cout << drv.result << '\n';
            
        else
            res = 1;

    return res;
}