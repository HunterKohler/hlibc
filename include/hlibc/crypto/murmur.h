/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 *
 * Murmur Hash 3
 */
#ifndef HLIBC_CRYPTO_MURMUR_H_
#define HLIBC_CRYPTO_MURMUR_H_

#include <hlibc/def.h>

/*
 * Murmur Hash 3 x86-32bit
 *
 * Reference:
 * https://github.com/aappleby/smhasher
 */
void murmurhash3_x86_32(const void *input, size_t size, uint32_t seed,
                        void *out);

#endif
