/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 *
 * SipHash
 */
#ifndef HLIBC_CRYPTO_SIPHASH_H_
#define HLIBC_CRYPTO_SIPHASH_H_

#include <hlibc/def.h>

#define SIPHASH_BLOCK_SIZE 8
#define SIPHASH_KEY_SIZE 16

/*
 * SipHash 2-4
 *
 * Utility function to run on full input without streaming.
 *
 * Reference:
 * https://www.aumasson.jp/siphash/siphash.pdf
 */
void siphash(const void *input, size_t size, const void *key, void *out);

#endif
