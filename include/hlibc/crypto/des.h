/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#ifndef HLIBC_CRYPTO_DES_H_
#define HLIBC_CRYPTO_DES_H_

#include <hlibc/def.h>

#define DES_BLOCK_SIZE 8
#define DES_KEY_SIZE 8

struct des_context {
    uint64_t ks[16];
};

void des_set_parity(void *key);

int des_check_parity(void *key);

int des_init(struct des_context *ctx, const void *key);

void des_encrypt(const struct des_context *ctx, const void *src, void *dest);

void des_decrypt(const struct des_context *ctx, const void *src, void *dest);

size_t des_ecb_encrypt(const struct des_context *ctx, const void *src,
                       size_t size, void *dest);

size_t des_ecb_decrypt(const struct des_context *ctx, const void *src,
                       size_t size, void *dest);

size_t des_cbc_encrypt(const struct des_context *ctx, const void *iv,
                       const void *src, size_t size, void *dest);

size_t des_cbc_decrypt(const struct des_context *ctx, const void *iv,
                       const void *src, size_t size, void *dest);

#endif
