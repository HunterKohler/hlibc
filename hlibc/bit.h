/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 *
 * Bit Manipulation
 */

#ifndef HLIBC_BIT_H_
#define HLIBC_BIT_H_

#include <stddef.h>
#include <stdint.h>

/*
 * Get the value of the bit at an offset `i` from `target`.
 */
int bit_at(const void *target, size_t i);

/*
 * Rotate the 32 bit integer `x` by `n` bits.
 */
uint32_t rotl_32(uint32_t x, size_t n);

#endif
