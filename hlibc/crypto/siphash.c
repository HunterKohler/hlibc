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

void siphash(const void *input, size_t len, const void *k, void *out)
{
    const size_t blocks = len / 8;
    const uint8_t *tail = (const uint8_t *)input + 8 * blocks;
    const uint64_t k0 = le64_to_cpu(((uint64_t *)k)[0]);
    const uint64_t k1 = le64_to_cpu(((uint64_t *)k)[1]);

    uint64_t v0 = k0 ^ 0x736F6D6570736575ULL;
    uint64_t v1 = k1 ^ 0x646F72616E646F6DULL;
    uint64_t v2 = k0 ^ 0x6C7967656E657261ULL;
    uint64_t v3 = k1 ^ 0x7465646279746573ULL;

    uint64_t last = (uint64_t)len << 56;

    switch (len % 8) {
    case 7:
        last |= (uint64_t)tail[6] << 48;
        [[fallthrough]];
    case 6:
        last |= (uint64_t)tail[5] << 40;
        [[fallthrough]];
    case 5:
        last |= (uint64_t)tail[4] << 32;
        [[fallthrough]];
    case 4:
        last |= (uint64_t)tail[3] << 24;
        [[fallthrough]];
    case 3:
        last |= (uint64_t)tail[2] << 16;
        [[fallthrough]];
    case 2:
        last |= (uint64_t)tail[1] << 8;
        [[fallthrough]];
    case 1:
        last |= (uint64_t)tail[0];
        break;
    }

    for (int i = 0; i < blocks; i++) {
        uint64_t block = le64_to_cpu(((uint64_t *)input)[i]);

        v3 ^= block;
        SIPROUND;
        SIPROUND;
        v0 ^= block;
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
    ((uint64_t *)out)[0] = cpu_to_le64(hash);
}
