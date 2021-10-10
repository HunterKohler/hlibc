/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIBC_STRING_H_
#define HLIBC_STRING_H_

#include <string.h>
#include <hlibc/def.h>

/*
 * Allocates `n + 1` bytes and null terminates buffer. Returns `NULL` and sets
 * `errno` on failure if not already set.
 */
char *stralloc(size_t n);

/*
 * Allocates enough memory to store concatenated strings. Returns `NULL` and
 * sets `errno` on failure if not already set.
 */
char *astrcat(const char *src1, const char *src2);

#endif
