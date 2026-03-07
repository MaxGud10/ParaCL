#include <iostream>
#include <stdexcept>


extern "C" void paracl_print(int value)
{
    std::cout << value << '\n';
}

extern "C" int paracl_in() {
    int value = 0;
    std::cin >> value;
    return value;
}
