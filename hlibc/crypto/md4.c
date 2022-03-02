/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <hlibc/bit.h>
#include <hlibc/string.h>
#include <hlibc/crypto/md4.h>

#define MD4_F(X, Y, Z) (((X) & (Y)) | (~(X) & (Z)))
#define MD4_G(X, Y, Z) (((X) & (Y)) | ((X) & (Z)) | ((Y) & (Z)))
#define MD4_H(X, Y, Z) ((X) ^ (Y) ^ (Z))

#define MD4_ROUND(f, a, b, c, d, x, v, s) \
    (a) = rotl32((a) + f(b, c, d) + (x) + (v), (s))

#define MD4_GET(ptr, idx) get_unaligned_le32((uint32_t *)(ptr) + (idx))

static inline void md4_process_chunk(struct md4_context *restrict ctx,
                                     const void *restrict data)
{
    uint32_t a = ctx->a;
    uint32_t b = ctx->b;
    uint32_t c = ctx->c;
    uint32_t d = ctx->d;

    MD4_ROUND(MD4_F, a, b, c, d, MD4_GET(data, 0), 0, 3);
    MD4_ROUND(MD4_F, d, a, b, c, MD4_GET(data, 1), 0, 7);
    MD4_ROUND(MD4_F, c, d, a, b, MD4_GET(data, 2), 0, 11);
    MD4_ROUND(MD4_F, b, c, d, a, MD4_GET(data, 3), 0, 19);
    MD4_ROUND(MD4_F, a, b, c, d, MD4_GET(data, 4), 0, 3);
    MD4_ROUND(MD4_F, d, a, b, c, MD4_GET(data, 5), 0, 7);
    MD4_ROUND(MD4_F, c, d, a, b, MD4_GET(data, 6), 0, 11);
    MD4_ROUND(MD4_F, b, c, d, a, MD4_GET(data, 7), 0, 19);
    MD4_ROUND(MD4_F, a, b, c, d, MD4_GET(data, 8), 0, 3);
    MD4_ROUND(MD4_F, d, a, b, c, MD4_GET(data, 9), 0, 7);
    MD4_ROUND(MD4_F, c, d, a, b, MD4_GET(data, 10), 0, 11);
    MD4_ROUND(MD4_F, b, c, d, a, MD4_GET(data, 11), 0, 19);
    MD4_ROUND(MD4_F, a, b, c, d, MD4_GET(data, 12), 0, 3);
    MD4_ROUND(MD4_F, d, a, b, c, MD4_GET(data, 13), 0, 7);
    MD4_ROUND(MD4_F, c, d, a, b, MD4_GET(data, 14), 0, 11);
    MD4_ROUND(MD4_F, b, c, d, a, MD4_GET(data, 15), 0, 19);

    MD4_ROUND(MD4_G, a, b, c, d, MD4_GET(data, 0), 0x5A827999, 3);
    MD4_ROUND(MD4_G, d, a, b, c, MD4_GET(data, 4), 0x5A827999, 5);
    MD4_ROUND(MD4_G, c, d, a, b, MD4_GET(data, 8), 0x5A827999, 9);
    MD4_ROUND(MD4_G, b, c, d, a, MD4_GET(data, 12), 0x5A827999, 13);
    MD4_ROUND(MD4_G, a, b, c, d, MD4_GET(data, 1), 0x5A827999, 3);
    MD4_ROUND(MD4_G, d, a, b, c, MD4_GET(data, 5), 0x5A827999, 5);
    MD4_ROUND(MD4_G, c, d, a, b, MD4_GET(data, 9), 0x5A827999, 9);
    MD4_ROUND(MD4_G, b, c, d, a, MD4_GET(data, 13), 0x5A827999, 13);
    MD4_ROUND(MD4_G, a, b, c, d, MD4_GET(data, 2), 0x5A827999, 3);
    MD4_ROUND(MD4_G, d, a, b, c, MD4_GET(data, 6), 0x5A827999, 5);
    MD4_ROUND(MD4_G, c, d, a, b, MD4_GET(data, 10), 0x5A827999, 9);
    MD4_ROUND(MD4_G, b, c, d, a, MD4_GET(data, 14), 0x5A827999, 13);
    MD4_ROUND(MD4_G, a, b, c, d, MD4_GET(data, 3), 0x5A827999, 3);
    MD4_ROUND(MD4_G, d, a, b, c, MD4_GET(data, 7), 0x5A827999, 5);
    MD4_ROUND(MD4_G, c, d, a, b, MD4_GET(data, 11), 0x5A827999, 9);
    MD4_ROUND(MD4_G, b, c, d, a, MD4_GET(data, 15), 0x5A827999, 13);

    MD4_ROUND(MD4_H, a, b, c, d, MD4_GET(data, 0), 0x6ED9EBA1, 3);
    MD4_ROUND(MD4_H, d, a, b, c, MD4_GET(data, 8), 0x6ED9EBA1, 9);
    MD4_ROUND(MD4_H, c, d, a, b, MD4_GET(data, 4), 0x6ED9EBA1, 11);
    MD4_ROUND(MD4_H, b, c, d, a, MD4_GET(data, 12), 0x6ED9EBA1, 15);
    MD4_ROUND(MD4_H, a, b, c, d, MD4_GET(data, 2), 0x6ED9EBA1, 3);
    MD4_ROUND(MD4_H, d, a, b, c, MD4_GET(data, 10), 0x6ED9EBA1, 9);
    MD4_ROUND(MD4_H, c, d, a, b, MD4_GET(data, 6), 0x6ED9EBA1, 11);
    MD4_ROUND(MD4_H, b, c, d, a, MD4_GET(data, 14), 0x6ED9EBA1, 15);
    MD4_ROUND(MD4_H, a, b, c, d, MD4_GET(data, 1), 0x6ED9EBA1, 3);
    MD4_ROUND(MD4_H, d, a, b, c, MD4_GET(data, 9), 0x6ED9EBA1, 9);
    MD4_ROUND(MD4_H, c, d, a, b, MD4_GET(data, 5), 0x6ED9EBA1, 11);
    MD4_ROUND(MD4_H, b, c, d, a, MD4_GET(data, 13), 0x6ED9EBA1, 15);
    MD4_ROUND(MD4_H, a, b, c, d, MD4_GET(data, 3), 0x6ED9EBA1, 3);
    MD4_ROUND(MD4_H, d, a, b, c, MD4_GET(data, 11), 0x6ED9EBA1, 9);
    MD4_ROUND(MD4_H, c, d, a, b, MD4_GET(data, 7), 0x6ED9EBA1, 11);
    MD4_ROUND(MD4_H, b, c, d, a, MD4_GET(data, 15), 0x6ED9EBA1, 15);

    ctx->a += a;
    ctx->b += b;
    ctx->c += c;
    ctx->d += d;
}

void md4_init(struct md4_context *ctx)
{
    ctx->a = 0x67452301;
    ctx->b = 0xEFCDAB89;
    ctx->c = 0x98BADCFE;
    ctx->d = 0x10325476;
    ctx->size = 0;
}

void md4_update(struct md4_context *restrict ctx, const void *restrict src,
                size_t size)
{
    const uint8_t *data = src;
    const uint8_t *end = data + size;
    int used = ctx->size & 0x3F;
    ctx->size += size;

    if (used) {
        int comp = 64 - used;

        if (size < comp) {
            memcpy(ctx->tail + used, data, size);
            return;
        }

        memcpy(ctx->tail + used, data, comp);
        data += comp;
    }

    for (; data + 64 <= end; data += 64) {
        md4_process_chunk(ctx, data);
    }

    memcpy(ctx->tail, data, end - data);
}

void md4_finalize(struct md4_context *restrict ctx, void *restrict dest)
{
    int mod = ctx->size & 63;
    ctx->tail[mod] = 128;

    if (mod >= 56) {
        memzero(ctx->tail + mod + 1, 63 - mod);
        md4_process_chunk(ctx, ctx->tail);
        mod = -1;
    }

    memzero(ctx->tail + mod + 1, 55 - mod);
    put_unaligned_le64(ctx->size << 3, ctx->tail + 56);
    md4_process_chunk(ctx, ctx->tail);

    put_unaligned_le32(ctx->a, (uint32_t *)dest + 0);
    put_unaligned_le32(ctx->b, (uint32_t *)dest + 1);
    put_unaligned_le32(ctx->c, (uint32_t *)dest + 2);
    put_unaligned_le32(ctx->d, (uint32_t *)dest + 3);
}
