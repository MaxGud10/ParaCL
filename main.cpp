#include "driver.hpp"
#include "log.h"

#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
	MSG("MACROSES:\n");
	LOG("YYDEBUG: {}\n", YYDEBUG);
	LOG("YY_FLEX_DEBUG: {}\n", YY_FLEX_DEBUG);

    int res = 0;

	std::ofstream out("output.txt");

	if (!out.is_open()) return -1;

    Driver drv(out);

    for (int i = 1; i < argc; ++i)
    	res = drv.parse(argv[i]);

    return res;
}