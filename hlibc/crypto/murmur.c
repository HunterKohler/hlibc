/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
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

void murmurhash3_x86_32(const void *input, size_t size, uint32_t seed,
                        void *out)
{
    size_t tail_size = size & 3;

    const uint8_t *pos = input;
    const uint8_t *tail = input + size - tail_size;

    uint32_t c0 = 0xCC9E2D51;
    uint32_t c1 = 0x1B873593;
    uint32_t h = seed;
    uint32_t k = 0;

    while (pos < tail) {
        k = get_unaligned_le32(pos);
        pos += 4;

        k *= c0;
        k = rotl32(k, 15);
        k *= c1;

        h ^= k;
        h = rotl32(h, 13);
        h = h * 5 + 0xE6546B64;
    }

    k = 0;

    switch (tail_size) {
    case 3:
        k |= (uint32_t)tail[2] << 16;
    case 2:
        k |= (uint32_t)tail[1] << 8;
    case 1:
        k |= (uint32_t)tail[0];

        k *= c0;
        k = rotl32(k, 15);
        k *= c1;
        h ^= k;
    }

    h ^= size;
    h = murmur_fmix32(h);

    put_unaligned_le32(h, out);
}
