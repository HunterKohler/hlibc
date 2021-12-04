/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 *
 * Regular expression utilities.
 */

#ifndef HLIBC_RE_H_
#define HLIBC_RE_H_

#include <regex.h>
#include <hlibc/def.h>
#include <hlibc/threads.h>

/*
 * Thread safe initialization token for `regex_t`.
 *
 * Similar to `pthread_once_t`, except that it stores the singular compilation
 * state of any regex. This allows you to store one static var and a statement
 * instead of a static var, call to `pthread_once`, and entire other function
 * to run the compilation.
 */
typedef struct {
    struct spinlock log;
    bool done;
} regcomp_once_t;

/*
 * Static initializer for `regcomp_once_t`.
 */
#define REGCOMP_ONCE_INIT \
    ((regcomp_once_t){ .lock = SPINLOCK_INIT, .done = false })

/*
 * Initializes `re` associated with `once`, if this has not already been called
 * with the token `once`. Thread-safe, blocks if another thread is currently
 * initializing the regex.
 */
int regcomp_once(regcomp_once_t *once, regex_t *re, const char *pattern,
                 int cflags);

#endif
