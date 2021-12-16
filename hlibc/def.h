/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
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

#ifndef static_assert
#define static_assert _Static_assert
#endif

#ifndef noreturn
#define noreturn _Noreturn
#endif

#ifndef typeof
#define typeof __typeof
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

#define container_of(ptr, type, member)                   \
    ({                                                    \
        const typeof(((type *)0)->member) *__ptr = (ptr); \
        (type *)((char *)__ptr - offsetof(type, member)); \
    })

#define array_size(a)               \
    ({                              \
        static_assert(is_array(a)); \
        sizeof(a) / sizeof((a)[0]); \
    })

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
 * https://en.cppreference.com/w/c/language/type
 * https://gcc.gnu.org/onlinedocs/gcc/_005f_005fint128.html
 */
typedef unsigned __int128 __uint128;
typedef __int128_t int128_t;
typedef __uint128_t uint128_t;

#define swap(a, b)                                  \
    do {                                            \
        _Static_assert(types_compatible((a), (b))); \
        typeof(*a) *__a = (a);                      \
        typeof(*b) *__b = (b);                      \
        typeof(*a) __tmp = *__a;                    \
        *__a = *__b;                                \
        *__b = __tmp;                               \
    } while (0)

/*
 * Reference:
 * https://gcc.gnu.org/onlinedocs/gcc/Statement-Attributes.html#Statement-Attributes
 */
#define __fallthrough __attribute__((fallthrough))

#endif
