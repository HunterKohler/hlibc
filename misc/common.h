/*
 * Copyright (C) 2021 John Hunter Kohler <jhunterkohler@gmail.com>
 *
 * Stores shared headers and static helpers between some misc/ files.
 */
#ifndef MISC_COMMON_H_
#define MISC_COMMON_H_
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <stdnoreturn.h>
#include <errno.h>

noreturn static void die(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, ": %s\n", strerror(errno));
    exit(1);
}

#endif
