#pragma once

#include <cassert>
#include <cstdio>

#define ASSERT_MESSAGE(condition, ...) \
    if(!(condition)){ \
        std::printf(__VA_ARGS__); \
        std::printf("\n"); \
        assert(condition); \
    } \


