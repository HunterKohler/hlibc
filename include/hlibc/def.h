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
 * Should actually be `0` until C23, but this has slightly bettser error
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

#define is_char(a) \
    (types_compatible((a), char) || \
     types_compatible((a), signed char) || \
     types_compatible((a), unsigned char))

#define is_signed(a)                       \
    (types_compatible((a), signed char) || \
     types_compatible((a), short) ||       \
     types_compatible((a), int) ||         \
     types_compatible((a), long) ||        \
     types_compatible((a), long long) ||   \
     types_compatible((a), int128_t))

#define is_unsigned(a)                            \
    (types_compatible((a), unsigned char) ||      \
     types_compatible((a), unsigned short) ||     \
     types_compatible((a), unsigned int) ||       \
     types_compatible((a), unsigned long) ||      \
     types_compatible((a), unsigned long long) || \
     types_compatible((a), bool) ||               \
     types_compatible((a), uint128_t))

#define is_integral(a)                            \
    (is_char(a) || is_signed(a) || is_unsigned(a))

#define is_complex(a)                            \
    (types_compatible((a), complex float) ||     \
     types_compatible((a), complex double) ||    \
     types_compatible((a), complex long double))

#define is_floating_point(a)               \
    (types_compatible((a), float) ||       \
     types_compatible((a), double) ||      \
     types_compatible((a), long double) || \
     is_complex(a))

#define is_void_ptr(a) types_compatible((a), void *)
#define is_array(a) (!types_compatible((a), &(a)[0]))

/* clang-format on */

#define container_of(ptr, type, member)                                \
    ({                                                                 \
        void *__ptr = (ptr);                                           \
        static_assert(types_compatible(*(ptr), ((type *)0)->member) || \
                      types_compatible(*(ptr), void));                 \
        (type *)(__ptr - offsetof(type, member));                      \
    })

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
            flockfile(stdout);                              \
            fprintf(stdout, "%s:%d: ", __FILE__, __LINE__); \
            fprintf(stdout, fmt, __VA_ARGS__);              \
            fprintf(stdout, "\n");                          \
            fflush(stdout);                                 \
            funlockfile(stdout);                            \
        }                                                   \
    } while (0)

/*
 * Reference:
 * https://en.cppreference.com/w/c/language/type
 * https://gcc.gnu.org/onlinedocs/gcc/_005f_005fint128.html
 */
typedef __int128 int128_t;
typedef unsigned __int128 uint128_t;

#define UINT128_MAX ((uint128_t)-1)
#define INT128_MAX (((int128_t)0x7fffffffffffffff << 64) | 0xffffffffffffffff)
#define INT128_MIN (-INT128_MAX - 1)

#define swap(a, b)                                 \
    do {                                           \
        static_assert(types_compatible((a), (b))); \
        typeof(*a) *__a = (a);                     \
        typeof(*b) *__b = (b);                     \
        typeof(*a) __tmp = *__a;                   \
        *__a = *__b;                               \
        *__b = __tmp;                              \
    } while (0)

/*
 * Use `it` to iterate through each
 */
#define for_each(it, arr)                                                     \
    for (int __for_each_##it = 0;                                             \
         ((it = (arr) + __for_each_##it), __for_each_##it < array_size(arr)); \
         __for_each_##it++)

#endif
