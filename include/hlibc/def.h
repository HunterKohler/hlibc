/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 *
 * Project wide definition file. Defines many compiler specific macros,
 * and polyfills for certain headers.
 */
#ifndef HLIBC_DEF_H_
#define HLIBC_DEF_H_

#include <stdalign.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <complex.h>
#include <errno.h>
#include <limits.h>

/*
 * Thread support.
 *
 * Remarks:
 * OSX does not support C11 threads (as of 11.6, though likely, then, never).
 */
#ifndef thread_local
#define thread_local _Thread_local
#endif

#ifndef noreturn
#define noreturn _Noreturn
#endif

#ifndef complex
#define complex _Complex
#endif

#ifndef alignas
#define alignas _Alignas
#endif

#ifndef alignof
#define alignof _Alignof
#endif

#ifndef bool
#define bool _Bool
#endif

#ifndef static_assert
#define static_assert _Static_assert
#endif

/* clang-format off */

/*
 * Should actually be `1` until C23, but this has slightly better error
 * checking most places. Either way, let built in headers define when possible.
 *
 * Reference:
 * https://en.cppreference.com/w/c/types
 */
#ifndef true
#define true ((_Bool)+1u)
#endif

/*
 * Should actually be `0` until C23, but this has slightly better error
 * checking most places. Either way, let built in headers define when possible.
 *
 * Reference:
 * https://en.cppreference.com/w/c/types
 */
#ifndef false
#define false ((_Bool)+0u)
#endif

#define typeof __typeof

#define types_compatible(a, b) \
    (__builtin_types_compatible_p(typeof(a), typeof(b)))

#define is_array(a) (!types_compatible((a), &(a)[0]))

/* clang-format on */

#define container_of(ptr, type, member)                                \
    ({                                                                 \
        void *__ptr = (ptr);                                           \
        static_assert(types_compatible(*(ptr), ((type *)0)->member) || \
                      types_compatible(*(ptr), void));                 \
        (type *)(__ptr - offsetof(type, member));                      \
    })

/*
 * Get compile-time type-checked array length.
 */
#define array_size(a)               \
    ({                              \
        static_assert(is_array(a)); \
        sizeof(a) / sizeof((a)[0]); \
    })

#undef LITTLE_ENDIAN
#undef BIG_ENDIAN

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define LITTLE_ENDIAN
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define BIG_ENDIAN
#else
#error There may be a byte order issue. Try fixing macros. Do not try to use \
       a middle endian machine because I will rain hellfire upon you.
#endif

/*
 * Reference:
 * https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
 */
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

/*
 * Reference:
 * https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
 */
#define choose_expr __builtin_choose_expr

#ifndef DEBUG
#define DEBUG 0
#endif

#define debug_printf(fmt, ...)                              \
    do {                                                    \
        if (DEBUG) {                                        \
            flockfile(stderr);                              \
            fprintf(stderr, "%s:%d: ", __FILE__, __LINE__); \
            fprintf(stderr, fmt, ##__VA_ARGS__);            \
            fprintf(stderr, "\n");                          \
            fflush(stderr);                                 \
            funlockfile(stderr);                            \
        }                                                   \
    } while (0)

#define __PASTE(a, b) a##b

/*
 * Paste identifiers and preserve macro expansion.
 */
#define PASTE(a, b) __PASTE(a, b)

/*
 * Unique identifier with GCC's '__COUNTER__' macro.
 *
 * Reference:
 * https://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html
 */
#define UNIQUE_ID(prefix) PASTE(PASTE(__UNIQUE_ID_, prefix), __COUNTER__)

/*
 * Packed type attribute.
 *
 * Reference:
 * https://gcc.gnu.org/onlinedocs/gcc/Common-Type-Attributes.html
 */
#define __packed __attribute__((__packed__))

/*
 * Get the result of unscaled pointer arithmetic while retaining type.
 */
#define movptr(ptr, diff)                      \
    ({                                         \
        void *__ptr = (ptr);                   \
        ptrdiff_t __diff = (diff);             \
        (typeof(ptr))((char *)__ptr + __diff); \
    })

/*
 * Get literal distance of pointer addresses regardless of type.
 */
#define ptrdiff(a, b)              \
    ({                             \
        void *__a = (a);           \
        void *__b = (b);           \
        (char *)__a - (char *)__b; \
    })

#endif
