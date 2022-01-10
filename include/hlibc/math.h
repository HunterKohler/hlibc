/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIBC_MATH_H_
#define HLIBC_MATH_H_

#include <hlibc/def.h>

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
unsigned int prime_count_upper(unsigned int n);

/*
 * Lower bound for the Prime Counting Function.
 * https://mathworld.wolfram.com/PrimeCountingFunction.html
 */
unsigned int prime_count_lower(unsigned int n);

/*
 * Sigmoid Function
 * https://mathworld.wolfram.com/SigmoidFunction.html
 */
double sigmoid(double x);

/*
 * Fast floor log base 2.
 */
unsigned int floor_log2(unsigned int x);

/*
 * Fast ceiling log base 2.
 */
unsigned int ceil_log2(unsigned int x);

/*
 * Fast floor log base 10.
 */
unsigned int floor_log10(unsigned int x);

/*
 * Fast floor log base 10.
 */
unsigned int ceil_log10(unsigned int x);

/*
 * Greatest common divisor.
 */
int gcd(int a, int b);

/*
 * Least common multiple.
 */
int lcm(int a, int b);

#endif
