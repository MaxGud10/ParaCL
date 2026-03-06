#include <iostream>
#include <stdexcept>


extern "C" void paracl_print(int value)
{
    std::cout << value << '\n';
}