#ifndef HLIBC_DEF_H_
#define HLIBC_DEF_H_

#include <stdatomic.h>
#include <stdalign.h>
#include <stdbool.h>

#ifndef thread_local
#define thread_local _Thread_local
#endif

#ifndef static_assert
#define static_assert _Static_assert
#endif

#define min(a, b)             \
    ({                        \
        __auto_type _a = (a); \
        __auto_type _b = (b); \
        _a < _b ? _a : _b     \
    })

#define max(a, b)             \
    ({                        \
        __auto_type _a = (a); \
        __auto_type _b = (b); \
        _a > _b ? _a : _b     \
    })

#define clamp(val, low, high) min((__typeof__(val))max(val, low), high)

#define swap(a, b)               \
    do {                         \
        __auto_type __tmp = (a); \
        (a) = (b);               \
        (b) = __tmp;             \
    } while (0);

#define IS_ARRAY \
    (!__builtin_types_compatible_p(__typeof__(a), __typeof__(&(a)[0])))

#define ARRAY_SIZE(a)                                                 \
    ({                                                                \
        _Static_assert(IS_ARRAY(a),                                   \
                       "Cannot call ARRAY_SIZE on non static array"); \
        sizeof(a) / sizeof((a)[0]);                                   \
    })

#define _CONCAT(a, b) a##b
#define CONCAT(a, b) _CONCAT(a, b)

#define N_ARGS(...) ARRAY_SIZE((int[])({ __VA_ARGS__ }))

#endif
