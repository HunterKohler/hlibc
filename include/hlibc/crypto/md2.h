/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 *
 * MD2 implementation.
 *
 * Reference:
 * https://datatracker.ietf.org/doc/html/rfc1319
 */
#ifndef HLIBC_CRYPTO_MD2_H_
#define HLIBC_CRYPTO_MD2_H_

#include <hlibc/def.h>

struct md2_context {
    size_t size;
    uint8_t state[16];
    uint8_t checksum[16];
    uint8_t tail[16];
};

void md2_init(struct md2_context *ctx);
void md2_update(struct md2_context *restrict ctx, const void *restrict src,
                size_t size);
void md2_finalize(struct md2_context *restrict ctx, void *restrict dest);

#endif
