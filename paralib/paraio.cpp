#include <iostream>
#include <new>
#include <stdexcept>

#include "paraio.hpp"

extern "C" ParaValue paracl_in()
{
    int value = 0;
    std::cin >> value;

    if (std::cin.fail())
        throw std::runtime_error("Incorrect input");

    return paracl_int(value);
}

extern "C" void paracl_print(ParaValue v)
{
    if (v.tag != PARA_INT)
        throw std::runtime_error("Cannot print non-int value yet");

    std::cout << v.int_value << '\n';
}

extern "C" ParaValue paracl_int(int32_t x)
{
    ParaValue v{};
    v.tag       = PARA_INT;
    v.int_value = x;
    return v;
}

extern "C" int32_t paracl_as_int(ParaValue v)
{
    if (v.tag != PARA_INT)
        throw std::runtime_error("Type error: expected int value");

    return v.int_value;
}

extern "C" ParaClosure* paracl_closure(ParaCodePtr code, void* env)
{
    if (!code)
        throw std::runtime_error("Cannot create closure with null code pointer");

    ParaClosure* closure = new ParaClosure{};
    closure->code = code;
    closure->env  = env;

    return closure;
}

extern "C" ParaValue paracl_closure_value(ParaClosure* closure)
{
    if (!closure)
        throw std::runtime_error("Cannot wrap null closure");

    ParaValue v{};
    v.tag           = PARA_CLOSURE;
    v.closure_value = closure;

    return v;
}

extern "C" ParaClosure* paracl_as_closure(ParaValue v)
{
    if (v.tag != PARA_CLOSURE)
        throw std::runtime_error("Type error: expected function/closure value");

    return v.closure_value;
}

extern "C" void* paracl_env_alloc(std::size_t size)
{
    void* mem = std::calloc(1, size);
    if (!mem)
        throw std::bad_alloc();

    return mem;
}

extern "C" ParaValue paracl_call(ParaClosure* closure, ParaValue* args, int32_t argc)
{
    if (!closure)
        throw std::runtime_error("Cannot call null closure");

    if (!closure->code)
        throw std::runtime_error("Closure has null code pointer");

    return closure->code(closure, args, argc);
}

extern "C" void* paracl_env(ParaClosure* closure)
{
    if (!closure)
        throw std::runtime_error("Null closure in paracl_env");

    return closure->env;
}