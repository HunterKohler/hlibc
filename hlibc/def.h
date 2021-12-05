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

#ifndef noreturn
#define noreturn _Noreturn
#endif

#ifndef static_assert
#define static_assert _Static_assert
#endif

#ifndef typeof
#define typeof __typeof
#endif

#define types_compatible(a, b) \
    __builtin_types_compatible_p(typeof(a), typeof(b))

/*
 * Compile time type checking macro for array types.
 */
#define is_array(a) (!types_compatible((a), &(a)[0]))

/*
 * Asserts the argument in an array type.
 */
#define static_assert_array(a) static_assert(is_array(a), "Non-array type: " #a)

/*
 * TODO: Reasearch practical usage of `likely`, `unlikely`, and `barrier`
 */
// #define likely(x) __builtin_expect(!!(x), 1)
// #define unlikely(x) __builtin_expect(!!(x), 0)
// #define barrier() __asm__ __volatile__("": : :"memory")

#define container_of(ptr, type, member)                     \
    ({                                                      \
        const __typeof(((type *)0)->member) *__ptr = (ptr); \
        (type *)((char *)__ptr - offsetof(type, member));   \
    })

#define ARRAY_SIZE(a)               \
    ({                              \
        static_assert_array(a);     \
        sizeof(a) / sizeof((a)[0]); \
    })

#endif
