#ifndef HLIBC_MACRO_H_
#define HLIBC_MACRO_H_

#include <stdatomic.h>
#include <stdalign.h>
#include <stdbool.h>

#ifndef thread_local
#define thread_local _Thread_local
#endif

#ifndef static_assert
#define static_assert _Static_assert
#endif

#define TYPE_COMPATABLE(x, y) __builtin_types_compatible_p(typeof(x), typeof(y))
#define CHOOSE_EXPR(b, x, y) __builtin_choose_expr((b), (x), (y))

#define IS_ARRAY(a) (!TYPE_COMPATABLE((a), &(a)[0]))

#define ARRAY_LENGTH(a)                          \
    ({                                           \
        static_assert(IS_ARRAY(a), "non-array"); \
        sizeof(a) / sizeof((a)[0]);              \
    })

#define NARGS(...) ARRAY_LENGTH((int[])({ __VA_ARGS__ }))

#endif