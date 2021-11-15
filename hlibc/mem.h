/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIB_MEM_H_
#define HLIB_MEM_H_

#include <hlibc/def.h>

/*
 * Copies `n` bytes to new buffer starting at `src`. Returns `NULL` and sets
 * `errno` on failure if not already set.
 */
void *memdup(const void *src, size_t n);

/*
 * Returns new buffer consisting of the first `n` bytes from `src` repeated
 * `m` times. Returns `NULL` and sets `errno` on failure if not already set.
 */
void *memrep(const void *src, size_t n, size_t m);

#endif
