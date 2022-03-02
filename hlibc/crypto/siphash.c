/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <hlibc/crypto/siphash.h>
#include <hlibc/bit.h>

#define SIPROUND             \
    do {                     \
        v0 += v1;            \
        v2 += v3;            \
        v1 = rotl64(v1, 13); \
        v3 = rotl64(v3, 16); \
        v1 ^= v0;            \
        v3 ^= v2;            \
        v0 = rotl64(v0, 32); \
        v2 += v1;            \
        v0 += v3;            \
        v1 = rotl64(v1, 17); \
        v3 = rotl64(v3, 21); \
        v1 ^= v2;            \
        v3 ^= v0;            \
        v2 = rotl64(v2, 32); \
    } while (0)

void siphash(const void *input, size_t size, const void *key, void *out)
{
    size_t tail_size = size & 7;
    const uint8_t *pos = input;
    const uint8_t *tail = pos + size - tail_size;

    const uint64_t k0 = get_unaligned_le64(key);
    const uint64_t k1 = get_unaligned_le64((const uint8_t *)key + 8);

    uint64_t v0 = k0 ^ 0x736F6D6570736575ULL;
    uint64_t v1 = k1 ^ 0x646F72616E646F6DULL;
    uint64_t v2 = k0 ^ 0x6C7967656E657261ULL;
    uint64_t v3 = k1 ^ 0x7465646279746573ULL;

    uint64_t last = (uint64_t)size << 56;

    while (pos < tail) {
        uint64_t block = get_unaligned_le64(pos);
        pos += 8;

        v3 ^= block;
        SIPROUND;
        SIPROUND;
        v0 ^= block;
    }

    switch (tail_size) {
    case 7:
        last |= (uint64_t)tail[6] << 48;
    case 6:
        last |= (uint64_t)tail[5] << 40;
    case 5:
        last |= (uint64_t)tail[4] << 32;
    case 4:
        last |= (uint64_t)tail[3] << 24;
    case 3:
        last |= (uint64_t)tail[2] << 16;
    case 2:
        last |= (uint64_t)tail[1] << 8;
    case 1:
        last |= (uint64_t)tail[0];
        break;
    }

    v3 ^= last;
    SIPROUND;
    SIPROUND;
    v0 ^= last;

    v2 ^= 0xFF;

    SIPROUND;
    SIPROUND;
    SIPROUND;
    SIPROUND;

    uint64_t hash = v0 ^ v1 ^ v2 ^ v3;
    put_unaligned_le64(hash, out);
}
