/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#ifndef HLIBC_CRYPTO_DES_H_
#define HLIBC_CRYPTO_DES_H_

#include <hlibc/def.h>

struct des_context {
    uint64_t ks[16];
};

int des_init(struct des_context *ctx, uint64_t key);

uint64_t des_encrypt(const struct des_context *ctx, uint64_t block);

uint64_t des_decrypt(const struct des_context *ctx, uint64_t block);

void des_ecb_encrypt(const struct des_context *restrict ctx, const void *src,
                     size_t size, void *dest);

void des_ecb_decrypt(const struct des_context *restrict ctx, const void *src,
                     size_t size, void *dest);

#endif
