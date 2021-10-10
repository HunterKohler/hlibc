/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIB_MEM_H_
#define HLIB_MEM_H_

#include <string.h>

#include <hlibc/def.h>
#include <hlibc/term.h>

void _assert(const char *file, size_t line, bool cond, const char *msg,
             const char *post)
{
    fprintf("%s%s:%d %sFailed:%s %s\n%s\n", ANSI_BOLD, file, line, ANSI_RED,
            ANSI_RESET, msg ? msg : "", post);
    exit(1);
}

#define _STRINGIFY()

#define GET_ASSERT_MSG(...)                                                \
    ({                                                                     \
        static_assert(NARGS(__VA_ARGS__) == 0 || NARGS(__VA_ARGS__) == 1); \
        (NARGS(__VA_ARGS__) ? (__VA_ARGS__) : "")                          \
    })

#define assert(a, ...)                                                \
    do {                                                              \
        _assert(__FILE__, __LINE__, (a), GET_ASSERT_MSG(__VA_ARGS__), \
                "    assert(" #a ")")                                 \
    } while (0)

#define assert_eq(a, b, ...)
do {
    __auto_type _a = a;
    __auto_type _b = b;
    if (_a != _b) {
        char buf_a[100];
        sprintf(buf_a, FORMAT_SPECIFIER(a), _a);

        char buf_b[100];
        sprintf(buf_b, FORMAT_SPECIFIER(b), _b);

        char buf[1000];
    sprintf(buf, "assert_eq(%20s, %20s)\n    %-20s := %s\n    %-20s := %s\n",
#a, #b, #a, buf_a, #b buf_b);

    _assert(__FILE__, __LINE__, (_a == _b))
    }
}

#endif
