#pragma once

#include <iostream>

// #define ENABLE_LOGGING

#if defined(__has_include)
#  if __has_include(<format>)
#    include <format>
#    define PARACL_HAS_FORMAT 1
#  else
#    define PARACL_HAS_FORMAT 0
#  endif
#else
#  define PARACL_HAS_FORMAT 0
#endif

#ifdef ENABLE_LOGGING

#include "llvm/Support/raw_ostream.h"

#define MSG(msg)                                                               \
    do                                                                         \
    {                                                                          \
        std::clog << __FUNCTION__ << ": " << msg;                              \
        std::clog << msg;                                                      \
    }                                                                          \
    while (false)

#if PARACL_HAS_FORMAT
#define LOG(msg, ...)                                                          \
    do                                                                         \
    {                                                                          \
        std::clog << __FUNCTION__ << ": ";                                     \
        std::clog << std::format(msg, __VA_ARGS__);                            \
    }                                                                          \
    while (false)
#else

#define LOG(msg, ...)                                                          \
    do                                                                         \
    {                                                                          \
        std::clog << __FUNCTION__ << ": " << msg;                              \
        std::clog << msg;                                                      \
    }                                                                          \
    while (false)
#endif

#define LLVM_MSG(msg)                                                          \
    do                                                                         \
    {                                                                          \
        llvm::errs() << msg;                                                   \
    }                                                                          \
    while (false)

#define LLVM_MSGLN(msg)                                                        \
    do                                                                         \
    {                                                                          \
        llvm::errs() << msg << "\n";                                           \
    }                                                                          \
    while (false)

#define LLVM_PRINT(value)                                                      \
    // do                                                                         \
    // {                                                                          \
    //     (value)->print(llvm::errs());                                          \
    // }                                                                          \
    // while (false)

#else

#define MSG(msg)                                                               \
    do                                                                         \
    {                                                                          \
    }                                                                          \
    while (false)

#define LOG(msg, ...)                                                          \
    do                                                                         \
    {                                                                          \
    }                                                                          \
    while (false)

#define LLVM_MSG(msg)                                                          \
    do                                                                         \
    {                                                                          \
    }                                                                          \
    while (false)

#define LLVM_MSGLN(msg)                                                        \
    do                                                                         \
    {                                                                          \
    }                                                                          \
    while (false)

#define LLVM_PRINT(value)                                                      \
    do                                                                         \
    {                                                                          \
    }                                                                          \
    while (false)

#endif
