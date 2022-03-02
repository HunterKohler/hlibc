/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <hlibc/bit.h>
#include <hlibc/string.h>
#include <hlibc/crypto/md5.h>

#define MD5_F(X, Y, Z) (((X) & (Y)) | (~(X) & (Z)))
#define MD5_G(X, Y, Z) (((X) & (Z)) | ((Y) & ~(Z)))
#define MD5_H(X, Y, Z) ((X) ^ (Y) ^ (Z))
#define MD5_I(X, Y, Z) ((Y) ^ ((X) | ~(Z)))

#define MD5_ROUND(fn, a, b, c, d, x, t, s) \
    (a) = (b) + rotl32(((a) + fn((b), (c), (d)) + (x) + (t)), (s));

#define MD5_GET(ptr, idx) get_unaligned_le32((uint32_t *)(ptr) + (idx))

static inline void md5_process_chunk(struct md5_context *restrict ctx,
                                     const void *restrict data)
{
    uint32_t a = ctx->a;
    uint32_t b = ctx->b;
    uint32_t c = ctx->c;
    uint32_t d = ctx->d;

    MD5_ROUND(MD5_F, a, b, c, d, MD5_GET(data, 0), 0xD76AA478, 7);
    MD5_ROUND(MD5_F, d, a, b, c, MD5_GET(data, 1), 0xE8C7B756, 12);
    MD5_ROUND(MD5_F, c, d, a, b, MD5_GET(data, 2), 0x242070DB, 17);
    MD5_ROUND(MD5_F, b, c, d, a, MD5_GET(data, 3), 0xC1BDCEEE, 22);
    MD5_ROUND(MD5_F, a, b, c, d, MD5_GET(data, 4), 0xF57C0FAF, 7);
    MD5_ROUND(MD5_F, d, a, b, c, MD5_GET(data, 5), 0x4787C62A, 12);
    MD5_ROUND(MD5_F, c, d, a, b, MD5_GET(data, 6), 0xA8304613, 17);
    MD5_ROUND(MD5_F, b, c, d, a, MD5_GET(data, 7), 0xFD469501, 22);
    MD5_ROUND(MD5_F, a, b, c, d, MD5_GET(data, 8), 0x698098D8, 7);
    MD5_ROUND(MD5_F, d, a, b, c, MD5_GET(data, 9), 0x8B44F7AF, 12);
    MD5_ROUND(MD5_F, c, d, a, b, MD5_GET(data, 10), 0xFFFF5BB1, 17);
    MD5_ROUND(MD5_F, b, c, d, a, MD5_GET(data, 11), 0x895CD7BE, 22);
    MD5_ROUND(MD5_F, a, b, c, d, MD5_GET(data, 12), 0x6B901122, 7);
    MD5_ROUND(MD5_F, d, a, b, c, MD5_GET(data, 13), 0xFD987193, 12);
    MD5_ROUND(MD5_F, c, d, a, b, MD5_GET(data, 14), 0xA679438E, 17);
    MD5_ROUND(MD5_F, b, c, d, a, MD5_GET(data, 15), 0x49B40821, 22);

    MD5_ROUND(MD5_G, a, b, c, d, MD5_GET(data, 1), 0xF61E2562, 5);
    MD5_ROUND(MD5_G, d, a, b, c, MD5_GET(data, 6), 0xC040B340, 9);
    MD5_ROUND(MD5_G, c, d, a, b, MD5_GET(data, 11), 0x265E5A51, 14);
    MD5_ROUND(MD5_G, b, c, d, a, MD5_GET(data, 0), 0xE9B6C7AA, 20);
    MD5_ROUND(MD5_G, a, b, c, d, MD5_GET(data, 5), 0xD62F105D, 5);
    MD5_ROUND(MD5_G, d, a, b, c, MD5_GET(data, 10), 0x2441453, 9);
    MD5_ROUND(MD5_G, c, d, a, b, MD5_GET(data, 15), 0xD8A1E681, 14);
    MD5_ROUND(MD5_G, b, c, d, a, MD5_GET(data, 4), 0xE7D3FBC8, 20);
    MD5_ROUND(MD5_G, a, b, c, d, MD5_GET(data, 9), 0x21E1CDE6, 5);
    MD5_ROUND(MD5_G, d, a, b, c, MD5_GET(data, 14), 0xC33707D6, 9);
    MD5_ROUND(MD5_G, c, d, a, b, MD5_GET(data, 3), 0xF4D50D87, 14);
    MD5_ROUND(MD5_G, b, c, d, a, MD5_GET(data, 8), 0x455A14ED, 20);
    MD5_ROUND(MD5_G, a, b, c, d, MD5_GET(data, 13), 0xA9E3E905, 5);
    MD5_ROUND(MD5_G, d, a, b, c, MD5_GET(data, 2), 0xFCEFA3F8, 9);
    MD5_ROUND(MD5_G, c, d, a, b, MD5_GET(data, 7), 0x676F02D9, 14);
    MD5_ROUND(MD5_G, b, c, d, a, MD5_GET(data, 12), 0x8D2A4C8A, 20);

    MD5_ROUND(MD5_H, a, b, c, d, MD5_GET(data, 5), 0xFFFA3942, 4);
    MD5_ROUND(MD5_H, d, a, b, c, MD5_GET(data, 8), 0x8771F681, 11);
    MD5_ROUND(MD5_H, c, d, a, b, MD5_GET(data, 11), 0x6D9D6122, 16);
    MD5_ROUND(MD5_H, b, c, d, a, MD5_GET(data, 14), 0xFDE5380C, 23);
    MD5_ROUND(MD5_H, a, b, c, d, MD5_GET(data, 1), 0xA4BEEA44, 4);
    MD5_ROUND(MD5_H, d, a, b, c, MD5_GET(data, 4), 0x4BDECFA9, 11);
    MD5_ROUND(MD5_H, c, d, a, b, MD5_GET(data, 7), 0xF6BB4B60, 16);
    MD5_ROUND(MD5_H, b, c, d, a, MD5_GET(data, 10), 0xBEBFBC70, 23);
    MD5_ROUND(MD5_H, a, b, c, d, MD5_GET(data, 13), 0x289B7EC6, 4);
    MD5_ROUND(MD5_H, d, a, b, c, MD5_GET(data, 0), 0xEAA127FA, 11);
    MD5_ROUND(MD5_H, c, d, a, b, MD5_GET(data, 3), 0xD4EF3085, 16);
    MD5_ROUND(MD5_H, b, c, d, a, MD5_GET(data, 6), 0x4881D05, 23);
    MD5_ROUND(MD5_H, a, b, c, d, MD5_GET(data, 9), 0xD9D4D039, 4);
    MD5_ROUND(MD5_H, d, a, b, c, MD5_GET(data, 12), 0xE6DB99E5, 11);
    MD5_ROUND(MD5_H, c, d, a, b, MD5_GET(data, 15), 0x1FA27CF8, 16);
    MD5_ROUND(MD5_H, b, c, d, a, MD5_GET(data, 2), 0xC4AC5665, 23);

    MD5_ROUND(MD5_I, a, b, c, d, MD5_GET(data, 0), 0xF4292244, 6);
    MD5_ROUND(MD5_I, d, a, b, c, MD5_GET(data, 7), 0x432AFF97, 10);
    MD5_ROUND(MD5_I, c, d, a, b, MD5_GET(data, 14), 0xAB9423A7, 15);
    MD5_ROUND(MD5_I, b, c, d, a, MD5_GET(data, 5), 0xFC93A039, 21);
    MD5_ROUND(MD5_I, a, b, c, d, MD5_GET(data, 12), 0x655B59C3, 6);
    MD5_ROUND(MD5_I, d, a, b, c, MD5_GET(data, 3), 0x8F0CCC92, 10);
    MD5_ROUND(MD5_I, c, d, a, b, MD5_GET(data, 10), 0xFFEFF47D, 15);
    MD5_ROUND(MD5_I, b, c, d, a, MD5_GET(data, 1), 0x85845DD1, 21);
    MD5_ROUND(MD5_I, a, b, c, d, MD5_GET(data, 8), 0x6FA87E4F, 6);
    MD5_ROUND(MD5_I, d, a, b, c, MD5_GET(data, 15), 0xFE2CE6E0, 10);
    MD5_ROUND(MD5_I, c, d, a, b, MD5_GET(data, 6), 0xA3014314, 15);
    MD5_ROUND(MD5_I, b, c, d, a, MD5_GET(data, 13), 0x4E0811A1, 21);
    MD5_ROUND(MD5_I, a, b, c, d, MD5_GET(data, 4), 0xF7537E82, 6);
    MD5_ROUND(MD5_I, d, a, b, c, MD5_GET(data, 11), 0xBD3AF235, 10);
    MD5_ROUND(MD5_I, c, d, a, b, MD5_GET(data, 2), 0x2AD7D2BB, 15);
    MD5_ROUND(MD5_I, b, c, d, a, MD5_GET(data, 9), 0xEB86D391, 21);

    ctx->a += a;
    ctx->b += b;
    ctx->c += c;
    ctx->d += d;
}

void md5_init(struct md5_context *ctx)
{
    ctx->a = 0x67452301;
    ctx->b = 0xEFCDAB89;
    ctx->c = 0x98BADCFE;
    ctx->d = 0x10325476;
    ctx->size = 0;
}

void md5_update(struct md5_context *restrict ctx, const void *restrict src,
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
        md5_process_chunk(ctx, data);
    }

    memcpy(ctx->tail, data, end - data);
}

void md5_finalize(struct md5_context *restrict ctx, void *restrict dest)
{
    int mod = ctx->size & 63;
    ctx->tail[mod] = 128;

    if (mod >= 56) {
        memzero(ctx->tail + mod + 1, 63 - mod);
        md5_process_chunk(ctx, ctx->tail);
        mod = -1;
    }

    memzero(ctx->tail + mod + 1, 55 - mod);
    put_unaligned_le64(ctx->size << 3, ctx->tail + 56);
    md5_process_chunk(ctx, ctx->tail);

    put_unaligned_le32(ctx->a, (uint32_t *)dest + 0);
    put_unaligned_le32(ctx->b, (uint32_t *)dest + 1);
    put_unaligned_le32(ctx->c, (uint32_t *)dest + 2);
    put_unaligned_le32(ctx->d, (uint32_t *)dest + 3);
}
