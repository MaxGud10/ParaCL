#pragma once

#if defined __has_include
#if __has_include(<format>)

#include <format>

#define ENABLE_LOGGING
#ifdef ENABLE_LOGGING

#define MSG(msg)                                                               \
    do                                                                         \
    {                                                                          \
        std::clog << __FUNCTION__ << ": ";                                     \
        std::clog << msg;                                                      \
    }                                                                          \
    while (false)

#define LOG(msg, ...)                                                          \
    do                                                                         \
    {                                                                          \
        std::clog << __FUNCTION__ << ": ";                                     \
        std::clog << std::format(msg, __VA_ARGS__);                            \
    }                                                                          \
    while (false)

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

#endif // ENABLE_LOGGING

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

#endif // __has_include(<format>)
#endif // defined __has_include
