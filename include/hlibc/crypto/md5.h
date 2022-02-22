/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 *
 * MD5 implementation.
 *
 * Reference:
 * https://datatracker.ietf.org/doc/html/rfc1321
 */
#ifndef HLIBC_CRYPTO_MD5_H_
#define HLIBC_CRYPTO_MD5_H_

#include <hlibc/def.h>

struct md5_context {
    uint32_t a, b, c, d;
    uint64_t size;
    uint8_t tail[64];
};

void md5_init(struct md5_context *ctx);
void md5_update(struct md5_context *restrict ctx, const void *restrict src,
                size_t size);
void md5_finalize(struct md5_context *restrict ctx, void *restrict dest);

#endif
