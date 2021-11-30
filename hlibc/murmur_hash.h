/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIBC_MURMUR_HASH_H_
#define HLIBC_MURMUR_HASH_H_

#include <stddef.h>
#include <stdint.h>
#include <hlibc/def.h>

static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__);
static_assert(__CHAR_BIT__ == 8);

/*
 * Small implementation of the x86, 32-bit version of MurmurHash3. Current
 * implementation only considers little-endian, 8-bit-char machines.
 *
 * Reference:
 * https://github.com/aappleby/smhasher
 */
uint32_t murmur_hash_x86_32(const void *key, size_t len, uint32_t seed);

#endif
