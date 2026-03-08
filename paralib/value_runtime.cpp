#include "value_runtime.hpp"

#include <cstdlib>
#include <new>
#include <stdexcept>

extern "C"
{

ParaValue paracl_make_int(int32_t x)
{
    ParaValue v{};
    v.tag = PARA_INT;
    v.int_value = x;
    return v;
}

int32_t paracl_value_as_int(ParaValue v)
{
    if (v.tag != PARA_INT)
        throw std::runtime_error("Type error: expected int value");

    return v.int_value;
}

int32_t paracl_value_is_int(ParaValue v)
{
    return v.tag == PARA_INT ? 1 : 0;
}

ParaClosure* paracl_make_closure(ParaCodePtr code, void* env)
{
    if (!code)
        throw std::runtime_error("Cannot create closure with null code pointer");

    ParaClosure* closure = new ParaClosure{};
    closure->code = code;
    closure->env = env;
    return closure;
}

ParaValue paracl_make_closure_value(ParaClosure* closure)
{
    if (!closure)
        throw std::runtime_error("Cannot wrap null closure");

    ParaValue v{};
    v.tag = PARA_CLOSURE;
    v.closure_value = closure;
    return v;
}

ParaClosure* paracl_value_as_closure(ParaValue v)
{
    if (v.tag != PARA_CLOSURE)
        throw std::runtime_error("Type error: expected function/closure value");

    return v.closure_value;
}

void* paracl_alloc_env(std::size_t size)
{
    void* mem = std::calloc(1, size);
    if (!mem)
        throw std::bad_alloc();

    return mem;
}

ParaValue paracl_call_closure(ParaClosure* closure, ParaValue* args, int32_t argc)
{
    if (!closure)
        throw std::runtime_error("Cannot call null closure");

    if (!closure->code)
        throw std::runtime_error("Closure has null code pointer");

    return closure->code(closure, args, argc);
}

void* paracl_closure_get_env(ParaClosure* closure)
{
    if (!closure)
        throw std::runtime_error("Null closure in paracl_closure_get_env");

    return closure->env;
}

}