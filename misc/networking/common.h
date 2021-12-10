/*
 * Copyright (C) 2021 John Hunter Kohler <jhunterkohler@gmail.com>
 *
 * Stores shared headers and static helpers between some misc/networking/ files.
 */
#ifndef MISC_NETWORKING_COMMON_H_
#define MISC_NETWORKING_COMMON_H_
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <stdnoreturn.h>
#include <errno.h>
#include <math.h>

#ifndef thread_local
#define thread_local _Thread_local
#endif

static noreturn void die(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, ": %s\n", strerror(errno));
    exit(1);
}

static size_t core_count()
{
    return sysconf(_SC_NPROCESSORS_ONLN);
}

// Prime Counting Function Bounds:
// See https://mathworld.wolfram.com/PrimeCountingFunction.html
static size_t prime_count_upper(size_t n)
{
    return (size_t)(1.25506 * n / log(n)) + 1;
}

static size_t prime_count_lower(size_t n)
{
    return (size_t)(n / log(n));
}

#endif
