/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */
#ifndef HLIBC_MATH_H_
#define HLIBC_MATH_H_

#include <hlibc/def.h>

#define max(a, b)                                                          \
    ({                                                                     \
        typeof(a) __a = (a);                                               \
        typeof(b) __b = (b);                                               \
        static_assert(types_compatible(__a, __b),                          \
                      "max(" #a "," #b ") called on incompatible types."); \
        __a > __b ? __a : __b;                                             \
    })

#define min(a, b)                                                          \
    ({                                                                     \
        typeof(a) __a = (a);                                               \
        typeof(b) __b = (b);                                               \
        static_assert(types_compatible(__a, __b),                          \
                      "min(" #a "," #b ") called on incompatible types."); \
        __a < __b ? __a : __b;                                             \
    })

#define clamp(x, a, b)                                                    \
    ({                                                                    \
        typeof(x) __x = (x);                                              \
        typeof(a) __a = (a);                                              \
        typeof(b) __b = (b);                                              \
                                                                          \
        static_assert(                                                    \
            types_compatible(__a, __b) && types_compatible(__b, __c),     \
            "clamp(" #x "," #a "," #b ") called on incompatible types."); \
                                                                          \
        __a > __x ? __a : __b < __x ? __b : __x;                          \
    })

/*
 * Upper bound for the Prime Counting Function.
 * https://mathworld.wolfram.com/PrimeCountingFunction.html
 */
size_t prime_count_upper(size_t n);

/*
 * Lower bound for the Prime Counting Function.
 * https://mathworld.wolfram.com/PrimeCountingFunction.html
 */
size_t prime_count_lower(size_t n);

#endif
