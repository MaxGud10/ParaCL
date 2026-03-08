#include <iostream>
#include <stdexcept>

#include "value_runtime.hpp"

extern "C" void paracl_print(int value)
{
    std::cout << value << '\n';
}

extern "C" int paracl_in()
{
    int value = 0;
    std::cin >> value;
    return value;
}

extern "C" ParaValue paracl_in_value()
{
    int value = 0;
    std::cin >> value;

    if (std::cin.fail())
        throw std::runtime_error("Incorrect input");

    return paracl_make_int(value);
}

extern "C" void paracl_print_value(ParaValue v)
{
    if (v.tag != PARA_INT)
        throw std::runtime_error("Cannot print non-int value yet");

    std::cout << v.int_value << '\n';
}