/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIBC_DEF_H_
#define HLIBC_DEF_H_

#include <stdalign.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdnoreturn.h>
#include <limits.h>
#include <errno.h>

#ifndef thread_local
#define thread_local _Thread_local
#endif

#ifndef static_assert
#define static_assert _Static_assert
#endif

#define swap(a, b)               \
    do {                         \
        __auto_type __tmp = (a); \
        (a) = (b);               \
        (b) = __tmp;             \
    } while (0)

#define IS_ARRAY(a) \
    (!__builtin_types_compatible_p(__typeof__(a), __typeof__(&(a)[0])))

#define ARRAY_SIZE(a)                                                \
    ({                                                               \
        static_assert(IS_ARRAY(a),                                   \
                      "Cannot call ARRAY_SIZE on non static array"); \
        sizeof(a) / sizeof((a)[0]);                                  \
    })

#define debug_printf(fmt, ...)                                           \
    do {                                                                 \
        fprintf(stderr, "%s:%d: " fmt, __FILE__, __LINE__, __VA_ARGS__); \
    } while (0)

#endif
