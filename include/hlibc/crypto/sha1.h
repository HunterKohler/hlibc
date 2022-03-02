/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 *
 * SHA1 implementation.
 *
 * Reference:
 * https://datatracker.ietf.org/doc/html/rfc3174
 */
#ifndef HLIBC_CRYPTO_SHA1_H_
#define HLIBC_CRYPTO_SHA1_H_

#include <hlibc/def.h>

#define SHA1_DIGEST_LEN 20

struct sha1_context {
    uint64_t size;
    uint32_t state[5];
    uint8_t tail[64];
};

void sha1_init(struct sha1_context *ctx);

void sha1_update(struct sha1_context *restrict ctx, const void *restrict src,
                 size_t size);

void sha1_finalize(struct sha1_context *restrict ctx, void *restrict dest);

#endif
