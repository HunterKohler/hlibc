/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIBC_MATH_H_
#define HLIBC_MATH_H_

#define min(a, b)             \
    ({                        \
        __auto_type _a = (a); \
        __auto_type _b = (b); \
        _a < _b ? _a : _b;    \
    })

#define max(a, b)             \
    ({                        \
        __auto_type _a = (a); \
        __auto_type _b = (b); \
        _a > _b ? _a : _b;    \
    })

#define clamp(val, low, high) min((__typeof__(val))max(val, low), high)

#endif
