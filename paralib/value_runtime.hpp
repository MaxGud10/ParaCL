#pragma once

#include <cstddef>
#include <cstdint>

extern "C"
{

enum ParaTag : int32_t
{
    PARA_INT = 0,
    PARA_CLOSURE = 1
};

struct ParaClosure;

struct ParaValue
{
    int32_t tag;
    union
    {
        int32_t int_value;
        ParaClosure* closure_value;
    };
};

using ParaCodePtr = ParaValue (*)(ParaClosure* self, ParaValue* args, int32_t argc);

struct ParaClosure
{
    ParaCodePtr code;
    void* env;
};

ParaValue paracl_make_int(int32_t x);
int32_t paracl_value_as_int(ParaValue v);
int32_t paracl_value_is_int(ParaValue v);

ParaClosure* paracl_make_closure(ParaCodePtr code, void* env);
ParaValue paracl_make_closure_value(ParaClosure* closure);
ParaClosure* paracl_value_as_closure(ParaValue v);

void* paracl_alloc_env(std::size_t size);

ParaValue paracl_call_closure(ParaClosure* closure, ParaValue* args, int32_t argc);
void* paracl_closure_get_env(ParaClosure* closure);
}