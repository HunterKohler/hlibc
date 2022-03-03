/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <hlibc/bit.h>
#include <hlibc/string.h>
#include <hlibc/crypto/sha1.h>

#define SHA1_H0 0x67452301
#define SHA1_H1 0xEFCDAB89
#define SHA1_H2 0x98BADCFE
#define SHA1_H3 0x10325476
#define SHA1_H4 0xC3D2E1F0

#define SHA1_K0 0x5A827999
#define SHA1_K1 0x6ED9EBA1
#define SHA1_K2 0x8F1BBCDC
#define SHA1_K3 0xCA62C1D6

#define SHA1_f0(B, C, D) (((B) & (C)) | (~(B) & (D)))
#define SHA1_f1(B, C, D) ((B) ^ (C) ^ (D))
#define SHA1_f2(B, C, D) (((B) & (C)) | ((B) & (D)) | ((C) & (D)))
#define SHA1_f3(B, C, D) ((B) ^ (C) ^ (D))

#define SHA1_ROUND(f, A, B, C, D, E, W_i, K, TEMP)                  \
    do {                                                            \
        (TEMP) = rotl32((A), 5) + f((B), (C), (D)) + (E) + W_i + K; \
        (E) = (D);                                                  \
        (D) = (C);                                                  \
        (C) = rotl32((B), 30);                                      \
        (B) = (A);                                                  \
        (A) = (TEMP);                                               \
    } while (0)

static void sha1_process_block(struct sha1_context *restrict ctx,
                               const void *src)
{
    uint32_t A = ctx->state[0];
    uint32_t B = ctx->state[1];
    uint32_t C = ctx->state[2];
    uint32_t D = ctx->state[3];
    uint32_t E = ctx->state[4];
    uint32_t W[80];
    uint32_t TEMP;
    const uint32_t *blocks = src;

#pragma GCC unroll 16
    for (int i = 0; i < 16; i++) {
        W[i] = rotl32(get_unaligned_be32(blocks + i), 1);
    }

#pragma GCC unroll 64
    for (int i = 16; i < 80; i++) {
        W[i] = rotl32(W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16], 1);
    }

#pragma GCC unroll 20
    for (int i = 0; i < 20; i++) {
        SHA1_ROUND(SHA1_f0, A, B, C, D, E, W[i], SHA1_K0, TEMP);
    }

#pragma GCC unroll 20
    for (int i = 20; i < 40; i++) {
        SHA1_ROUND(SHA1_f1, A, B, C, D, E, W[i], SHA1_K1, TEMP);
    }

#pragma GCC unroll 20
    for (int i = 40; i < 60; i++) {
        SHA1_ROUND(SHA1_f2, A, B, C, D, E, W[i], SHA1_K2, TEMP);
    }

#pragma GCC unroll 20
    for (int i = 60; i < 80; i++) {
        SHA1_ROUND(SHA1_f3, A, B, C, D, E, W[i], SHA1_K3, TEMP);
    }

    ctx->state[0] += A;
    ctx->state[1] += B;
    ctx->state[2] += C;
    ctx->state[3] += D;
    ctx->state[4] += E;
}

void sha1_init(struct sha1_context *ctx)
{
    ctx->size = 0;
    ctx->state[0] = SHA1_H0;
    ctx->state[1] = SHA1_H1;
    ctx->state[2] = SHA1_H2;
    ctx->state[3] = SHA1_H3;
    ctx->state[4] = SHA1_H4;
}

void sha1_update(struct sha1_context *restrict ctx, const void *restrict src,
                 size_t size)
{
    const uint8_t *data = src;
    const uint8_t *end = data + size;
    const uint8_t *last = end - 8;
    size_t used = ctx->size & 63;

    ctx->size += size;

    if (used) {
        uint8_t *tail = ctx->tail + used;

        if (size + used < 64) {
            memcpy(tail, data, size);
            return;
        }

        memcpy(tail, data, 64 - used);
        sha1_process_block(ctx, ctx->tail);
        data += size;
    }

    for (; data <= last; data += 8) {
        sha1_process_block(ctx, data);
        data += 8;
    }

    memcpy(ctx->tail, data, end - data);
}

void sha1_finalize(struct sha1_context *restrict ctx, void *restrict dest)
{
    uint32_t *out = dest;
    size_t used = ctx->size & 63;
    ctx->tail[used++] = 128;

    if (used > 56) {
        memzero(ctx->tail + used, 64 - used);
        used = 0;
    }

    memzero(ctx->tail + used, 56 - used);
    put_unaligned_be64(ctx->size, ctx->tail + 56);
    sha1_process_block(ctx, ctx->tail);

#pragma GCC unroll 5
    for (int i = 0; i < 5; i++)
        put_unaligned_be32(ctx->state[i], out + i);
}
