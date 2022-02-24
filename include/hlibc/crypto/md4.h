/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 *
 * MD4 implementation.
 *
 * Reference:
 * https://datatracker.ietf.org/doc/html/rfc1320
 */
#ifndef HLIBC_CRYPTO_MD4_H_
#define HLIBC_CRYPTO_MD4_H_

#include <hlibc/def.h>

#define MD4_DIGEST_LEN 16

struct md4_context {
    uint32_t a, b, c, d;
    uint64_t size;
    uint8_t tail[64];
};

void md4_init(struct md4_context *ctx);
void md4_update(struct md4_context *restrict ctx, const void *restrict src,
                size_t size);
void md4_finalize(struct md4_context *restrict ctx, void *restrict dest);

#endif
