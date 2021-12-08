/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */
#ifndef HLIBC_MATH_H_
#define HLIBC_MATH_H_

#include <hlibc/def.h>
#include <math.h>

#define max(a, b)              \
    ({                         \
        typeof(a) __a = (a);   \
        typeof(b) __b = (b);   \
        __a > __b ? __a : __b; \
    })

#define min(a, b)              \
    ({                         \
        typeof(a) __a = (a);   \
        typeof(b) __b = (b);   \
        __a < __b ? __a : __b; \
    })

#define clamp(x, a, b) min(max((x), (a)), (b))

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

/*
 * Sigmoid Function
 * https://mathworld.wolfram.com/SigmoidFunction.html
 */
double sigmoid(double x);

#endif
