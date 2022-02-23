
#include <hlibc/bit.h>
#include <hlibc/string.h>
#include <hlibc/crypto/sha1.h>

#define SHA1_K0 0x5A827999
#define SHA1_K1 0x6ED9EBA1
#define SHA1_K2 0x8F1BBCDC
#define SHA1_K3 0xCA62C1D6

#define SHA1_H0 0x67452301
#define SHA1_H1 0xEFCDAB89
#define SHA1_H2 0x98BADCFE
#define SHA1_H3 0x10325476
#define SHA1_H4 0xC3D2E1F0

#define SHA1_F0(B, C, D) (((B) & (C)) | (~(B) & (D)))
#define SHA1_F1(B, C, D) ((B) ^ (C) ^ (D))
#define SHA1_F2(B, C, D) (((B) & (C)) | ((B) & (D)) | ((C) & (D)))
#define SHA1_F3(B, C, D) ((B) ^ (C) ^ (D))

static inline void sha1_process_block(struct sha1_context *restrict ctx,
                                      const void *restrict src)
{
    const uint32_t *data = src;
    uint32_t *H = ctx->state;
    uint32_t A = H[0];
    uint32_t B = H[1];
    uint32_t C = H[2];
    uint32_t D = H[3];
    uint32_t E = H[4];
    uint32_t TEMP;
    uint32_t W[80];

    for (int i = 0; i < 16; i++) {
        W[i] = data[i];
    }

    for (int i = 16; i < 80; i++) {
        W[i] = W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16];
    }

    for (int i = 0; i < 20; i++) {
        TEMP = rotl32(A, 5) + SHA1_F0(B, C, D) + E + W[i] + SHA1_K0;
        E = D;
        D = C;
        C = rotl32(B, 30);
        B = A;
        A = TEMP;
    }

    for (int i = 20; i < 40; i++) {
        TEMP = rotl32(A, 5) + SHA1_F1(B, C, D) + E + W[i] + SHA1_K1;
        E = D;
        D = C;
        C = rotl32(B, 30);
        B = A;
        A = TEMP;
    }

    for (int i = 40; i < 60; i++) {
        TEMP = rotl32(A, 5) + SHA1_F2(B, C, D) + E + W[i] + SHA1_K2;
        E = D;
        D = C;
        C = rotl32(B, 30);
        B = A;
        A = TEMP;
    }

    for (int i = 60; i < 80; i++) {
        TEMP = rotl32(A, 5) + SHA1_F3(B, C, D) + E + W[i] + SHA1_K3;
        E = D;
        D = C;
        C = rotl32(B, 30);
        B = A;
        A = TEMP;
    }

    H[0] += A;
    H[1] += B;
    H[2] += C;
    H[3] += D;
    H[4] += E;
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
    int used = ctx->size & 63;
    ctx->size += size;

    if (used) {
        int comp = 64 - used;
        uint8_t *back = ctx->tail + used;

        if (size + comp < 64) {
            memcpy(back, data, size);
            return;
        }

        memcpy(back, data, size);
        data += size;

        sha1_process_block(ctx, ctx->tail);
    }

    for (; data + 64 <= end; data += 64) {
        sha1_process_block(ctx, data);
    }

    memcpy(ctx->tail, data, end - data);
}

void sha1_finalize(struct sha1_context *restrict ctx, void *restrict dest)
{
    int used = ctx->size & 64;
    ctx->tail[used] = 128;

    if (used >= 56) {
        memzero(ctx->tail + used + 1, 63 - used);
        sha1_process_block(ctx, ctx->tail);
        used = -1;
    }

    /*
     * SHA-family uses big endian.
     */
    memzero(ctx->tail + used + 1, 55 - used);
    ((uint64_t *)ctx->tail)[7] = cpu_to_be64(ctx->size << 3);

    sha1_process_block(ctx, ctx->tail);

    ((uint32_t *)dest)[0] = cpu_to_be32(ctx->state[0]);
    ((uint32_t *)dest)[1] = cpu_to_be32(ctx->state[1]);
    ((uint32_t *)dest)[2] = cpu_to_be32(ctx->state[2]);
    ((uint32_t *)dest)[3] = cpu_to_be32(ctx->state[3]);
    ((uint32_t *)dest)[4] = cpu_to_be32(ctx->state[4]);
}
