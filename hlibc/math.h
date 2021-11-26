/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */
#ifndef HLIBC_MATH_H_
#define HLIBC_MATH_H_

/*
 * Type generic non-recalculating minimum macro.
 */
#define min(a, b)              \
    ({                         \
        __auto_type __a = (a); \
        __auto_type __b = (b); \
        __a < __b ? __a : __b; \
    })

/*
 * Type generic non-recalculating maximum macro.
 */
#define max(a, b)              \
    ({                         \
        __auto_type __a = (a); \
        __auto_type __b = (b); \
        __a > __b ? __a : __b; \
    })

/*
 * Type generic non-recalculating clamp macro.
 */
#define clamp(val, low, high) min((__typeof(val))max(val, low), high)

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
