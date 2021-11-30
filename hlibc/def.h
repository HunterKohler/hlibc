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

#ifndef __static_assert
#define __static_assert _Static_assert
#endif

#define __types_compatible(a, b) \
    __builtin_types_compatible_p(__typeof(a), __typeof(b))

#define __is_array(a) (!__types_compatible((a), &(a)[0]))

#define __static_assert_array(a) \
    __static_assert(__is_array(a), "Non-array type: " #a)

#define container_of(ptr, type, member)                     \
    ({                                                      \
        const __typeof(((type *)0)->member) *__ptr = (ptr); \
        (type *)((char *)__ptr - offsetof(type, member));   \
    })

#define ARRAY_SIZE(a)               \
    ({                              \
        __static_assert_array(a);   \
        sizeof(a) / sizeof((a)[0]); \
    })

#endif
