/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <hlibc/crypto/murmur.h>
#include <hlibc/bit.h>

/* MurmurHash3 32-bit finalization mix. */
static inline uint32_t murmur_fmix32(uint32_t h)
{
    h ^= h >> 16;
    h *= 0x85EBCA6B;
    h ^= h >> 13;
    h *= 0xC2B2AE35;
    h ^= h >> 16;

    return h;
}

/* MurmurHash3 64-bit finalization mix. */
static inline uint64_t murmur_fmix64(uint64_t h)
{
    h ^= h >> 33;
    h *= 0xFF51AFD7ED558CCDULL;
    h ^= h >> 33;
    h *= 0xC4CEB9FE1A85EC53ULL;
    h ^= h >> 33;

    return h;
}

void murmurhash3_x86_32(const void *input, size_t len, uint32_t seed, void *out)
{
    const size_t blocks = len / 4;
    const uint8_t *tail = (const uint8_t *)input + 4 * blocks;
    const uint32_t c0 = 0xCC9E2D51;
    const uint32_t c1 = 0x1B873593;

    uint32_t h = seed;
    uint32_t k = 0;

    for (int i = 0; i < blocks; i++) {
        k = le32_to_cpu(((uint32_t *)input)[i]);

        k *= c0;
        k = rotl32(k, 15);
        k *= c1;

        h ^= k;
        h = rotl32(h, 13);
        h = h * 5 + 0xE6546B64;
    }

    k = 0;

    switch (len % 4) {
    case 3:
        k |= (uint32_t)tail[2] << 16;
        [[fallthrough]];
    case 2:
        k |= (uint32_t)tail[1] << 8;
        [[fallthrough]];
    case 1:
        k |= (uint32_t)tail[0];

        k *= c0;
        k = rotl32(k, 15);
        k *= c1;
        h ^= k;
    }

    h ^= len;
    h = murmur_fmix32(h);

    ((uint32_t *)out)[0] = cpu_to_le32(h);
}

void murmurhash3_x86_128(const void *input, size_t len, uint32_t seed,
                         void *out)
{
    const size_t blocks = len / 16;
    const uint8_t *tail = (const uint8_t *)input + 16 * blocks;
    const uint32_t c0 = 0x239b961b;
    const uint32_t c1 = 0xab0e9789;
    const uint32_t c2 = 0x38b34ae5;
    const uint32_t c3 = 0xa1e38b93;

    uint32_t h0 = seed;
    uint32_t h1 = seed;
    uint32_t h2 = seed;
    uint32_t h3 = seed;

    uint32_t k0 = 0;
    uint32_t k1 = 0;
    uint32_t k2 = 0;
    uint32_t k3 = 0;

    for (int i = 0; i < blocks; i += 4) {
        k0 = le32_to_cpu(((uint32_t *)input)[i]);
        k1 = le32_to_cpu(((uint32_t *)input)[i + 1]);
        k2 = le32_to_cpu(((uint32_t *)input)[i + 2]);
        k3 = le32_to_cpu(((uint32_t *)input)[i + 3]);

        k0 *= c0;
        k0 = rotl32(k0, 15);
        k0 *= c1;

        h0 ^= k0;
        h0 = rotl32(h0, 19);
        h0 += h1;
        h0 = h0 * 5 + 0x561CCD1B;

        k1 *= c1;
        k1 = rotl32(k1, 16);
        k1 *= c2;

        h1 ^= k1;
        h1 = rotl32(h1, 17);
        h1 += h2;
        h1 = h1 * 5 + 0x0BCAA747;

        k2 *= c2;
        k2 = rotl32(k2, 17);
        k2 *= c3;

        h2 ^= k2;
        h2 = rotl32(h2, 15);
        h2 += h3;
        h2 = h2 * 5 + 0x96CD1C35;

        k3 *= c3;
        k3 = rotl32(k3, 18);
        k3 *= c0;

        h3 ^= k3;
        h3 = rotl32(h3, 13);
        h3 += h0;
        h3 = h3 * 5 + 0x32AC3B17;
    }

    k0 = 0;
    k1 = 0;
    k2 = 0;
    k3 = 0;

    switch (len % 16) {
    case 15:
        k3 |= (uint32_t)tail[14] << 16;
        [[fallthrough]];
    case 14:
        k3 |= (uint32_t)tail[13] << 8;
        [[fallthrough]];
    case 13:
        k3 |= (uint32_t)tail[12];

        k3 *= c3;
        k3 = rotl32(k3, 18);
        k3 *= c0;
        h3 ^= k3;

        [[fallthrough]];
    case 12:
        k2 |= (uint32_t)tail[11] << 24;
        [[fallthrough]];
    case 11:
        k2 |= (uint32_t)tail[10] << 16;
        [[fallthrough]];
    case 10:
        k2 |= (uint32_t)tail[9] << 8;
        [[fallthrough]];
    case 9:
        k2 |= (uint32_t)tail[8];

        k2 *= c2;
        k2 = rotl32(k2, 17);
        k2 *= c3;
        h2 ^= k2;

        [[fallthrough]];
    case 8:
        k1 |= (uint32_t)tail[7] << 24;
        [[fallthrough]];
    case 7:
        k1 |= (uint32_t)tail[6] << 16;
        [[fallthrough]];
    case 6:
        k1 |= (uint32_t)tail[5] << 8;
        [[fallthrough]];
    case 5:
        k1 |= (uint32_t)tail[4];

        k1 *= c1;
        k1 = rotl32(k1, 16);
        k1 *= c2;
        h1 ^= k1;

        [[fallthrough]];
    case 4:
        k0 |= (uint32_t)tail[3] << 24;
        [[fallthrough]];
    case 3:
        k0 |= (uint32_t)tail[2] << 16;
        [[fallthrough]];
    case 2:
        k0 |= (uint32_t)tail[1] << 8;
        [[fallthrough]];
    case 1:
        k0 |= (uint32_t)tail[0];

        k0 *= c0;
        k0 = rotl32(k0, 15);
        k0 *= c1;
        h0 ^= k0;
    }

    h0 ^= len;
    h1 ^= len;
    h2 ^= len;
    h3 ^= len;

    h0 += h1 + h2 + h3;
    h1 += h0;
    h2 += h0;
    h3 += h0;

    h0 = murmur_fmix32(h0);
    h1 = murmur_fmix32(h1);
    h2 = murmur_fmix32(h2);
    h3 = murmur_fmix32(h3);

    h0 += h1 + h2 + h3;
    h1 += h0;
    h2 += h0;
    h3 += h0;

    ((uint32_t *)out)[0] = cpu_to_le32(h0);
    ((uint32_t *)out)[1] = cpu_to_le32(h1);
    ((uint32_t *)out)[2] = cpu_to_le32(h2);
    ((uint32_t *)out)[3] = cpu_to_le32(h3);
}

void murmurhash3_x64_128(const void *input, size_t len, uint64_t seed,
                         void *out)
{
    const size_t blocks = len / 16;
    const uint8_t *tail = (const uint8_t *)input + 16 * blocks;
    const uint64_t c0 = 0x87C37B91114253D5;
    const uint64_t c1 = 0x4CF5AD432745937F;

    uint64_t h0 = seed;
    uint64_t h1 = seed;

    uint64_t k0 = 0;
    uint64_t k1 = 0;

    for (int i = 0; i < blocks; i += 2) {
        k0 = le64_to_cpu(((uint64_t *)input)[i]);
        k1 = le64_to_cpu(((uint64_t *)input)[i + 1]);

        k0 *= c0;
        k0 = rotl64(k0, 31);
        k0 *= c1;

        h0 ^= k0;
        h0 = rotl64(h0, 27);
        h0 += h1;
        h0 = h0 * 5 + 0x52DCE729;

        k1 *= c1;
        k1 = rotl64(k1, 33);
        k1 *= c0;

        h1 ^= k1;
        h1 = rotl64(h1, 31);
        h1 += h0;
        h1 = h1 * 5 + 0x38495AB5;
    }

    k0 = 0;
    k1 = 0;

    switch (len % 16) {
    case 15:
        k1 |= (uint64_t)tail[14] << 48;
        [[fallthrough]];
    case 14:
        k1 |= (uint64_t)tail[13] << 40;
        [[fallthrough]];
    case 13:
        k1 |= (uint64_t)tail[12] << 32;
        [[fallthrough]];
    case 12:
        k1 |= (uint64_t)tail[11] << 24;
        [[fallthrough]];
    case 11:
        k1 |= (uint64_t)tail[10] << 16;
        [[fallthrough]];
    case 10:
        k1 |= (uint64_t)tail[9] << 8;
        [[fallthrough]];
    case 9:
        k1 |= (uint64_t)tail[8];

        k1 *= c1;
        k1 = rotl64(k1, 33);
        k1 *= c0;
        h1 ^= k1;

        [[fallthrough]];
    case 8:
        k0 |= (uint64_t)tail[7] << 56;
        [[fallthrough]];
    case 7:
        k0 |= (uint64_t)tail[6] << 48;
        [[fallthrough]];
    case 6:
        k0 |= (uint64_t)tail[5] << 40;
        [[fallthrough]];
    case 5:
        k0 |= (uint64_t)tail[4] << 32;
        [[fallthrough]];
    case 4:
        k0 |= (uint64_t)tail[3] << 24;
        [[fallthrough]];
    case 3:
        k0 |= (uint64_t)tail[2] << 16;
        [[fallthrough]];
    case 2:
        k0 |= (uint64_t)tail[1] << 8;
        [[fallthrough]];
    case 1:
        k0 |= (uint64_t)tail[0];

        k0 *= c0;
        k0 = rotl64(k0, 31);
        k0 *= c1;
        h0 ^= k0;
    }

    h0 ^= len;
    h1 ^= len;

    h0 += h1;
    h1 += h0;

    h0 = murmur_fmix64(h0);
    h1 = murmur_fmix64(h1);

    h0 += h1;
    h1 += h0;

    ((uint64_t *)out)[0] = cpu_to_le64(h0);
    ((uint64_t *)out)[1] = cpu_to_le64(h1);
}
