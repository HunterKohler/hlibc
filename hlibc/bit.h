/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 *
 * Bit Manipulation
 */

#ifndef HLIBC_BIT_H_
#define HLIBC_BIT_H_

#include <string.h>
#include <hlibc/def.h>

#define BITS_PER(type) (CHAR_BIT * sizeof(type))
#define BITS_TO(bits, type) (((bits) + BITS_PER(type) - 1) / BITS_PER(type))

/*
 * Macros for GCC's builtin ffs (find first signifigant) functions.
 * Reference: https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
 */
#define ffs(x) __builtin_ffs(x)
#define ffsl(x) __builtin_ffsl(x)
#define ffsll(x) __builtin_ffsll(x)

/*
 * Macros for GCC's builtin clz (count leading zero) functions.
 * Reference: https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
 */
#define clz(x) __builtin_clz(x)
#define clzl(x) __builtin_clzl(x)
#define clzll(x) __builtin_clzll(x)

/*
 * Macros for GCC's builtin ctz (count trailing zeros) functions.
 * Reference: https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
 */
#define ctz(x) __builtin_ctz(x)
#define ctzl(x) __builtin_ctzl(x)
#define ctzll(x) __builtin_ctzll(x)

/*
 * Macros for GCC's builtin clrsb (count leading redundant sign bits)
 * functions.
 * Reference: https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
 */
#define clrsb(x) __builtin_clrsb(x)
#define clrsbl(x) __builtin_clrsbl(x)
#define clrsbll(x) __builtin_clrsbll(x)

/*
 * Macros for GCC's builtin popcount functions.
 * Reference: https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
 */
#define popcount(x) __builtin_popcount(x)
#define popcountl(x) __builtin_popcountl(x)
#define popcountll(x) __builtin_popcountll(x)

/*
 * Macros for GCC's builtin parity functions.
 * Reference: https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
 */
#define parity(x) __builtin_parity(x)
#define parityl(x) __builtin_parityl(x)
#define parityll(x) __builtin_parityll(x)

/*
 * Macros for GCC's builtin bswap functions.
 * Reference: https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
 */
#define bswap16(x) __builtin_bswap16(x)
#define bswap32(x) __builtin_bswap32(x)
#define bswap64(x) __builtin_bswap64(x)
#define bswap128(x) __builtin_bswap128(x)

/*
 * TODO test for negative indices.
 */
static inline bool bit_test(const void *base, int i)
{
    return ((uint8_t *)base)[i >> 3] & (1 << (i % 8));
}

static inline void bit_set(void *base, int i)
{
    ((uint8_t *)base)[i >> 3] |= (1 << (i % 8));
}

static inline void bit_clear(void *base, int i)
{
    ((uint8_t *)base)[i >> 3] &= ~(1 << (i % 8));
}

static inline void bit_flip(void *base, int i)
{
    ((uint8_t *)base)[i >> 3] ^= (1 << (i % 8));
}

static inline bool bit_test_and_set(void *base, int i)
{
    bool old = bit_test(base, i);
    bit_set(base, i);
    return old;
}

static inline bool bit_test_and_clear(void *base, int i)
{
    bool old = bit_test(base, i);
    bit_clear(base, i);
    return old;
}

static inline bool bit_test_and_flip(void *base, int i)
{
    bool old = bit_test(base, i);
    bit_clear(base, i);
    return old;
}

/*
 * Take hamming distance on arbitrary `n` bytes.
 */
size_t hamming_distance(const void *a, const void *b, size_t n);

static inline uint8_t rotl8(uint8_t x, size_t n)
{
    return (x << n) | (x >> (8 - n));
}

static inline uint16_t rotl16(uint16_t x, size_t n)
{
    return (x << n) | (x >> (16 - n));
}

static inline uint32_t rotl32(uint32_t x, size_t n)
{
    return (x << n) | (x >> (32 - n));
}

static inline uint64_t rotl64(uint64_t x, size_t n)
{
    return (x << n) | (x >> (64 - n));
}

static inline uint128_t rotl128(uint128_t x, size_t n)
{
    return (x << n) | (x >> (128 - n));
}

static inline uint8_t rotr8(uint8_t x, size_t n)
{
    return (x >> n) | (x << (8 - n));
}

static inline uint16_t rotr16(uint16_t x, size_t n)
{
    return (x >> n) | (x << (16 - n));
}

static inline uint32_t rotr32(uint32_t x, size_t n)
{
    return (x >> n) | (x << (32 - n));
}

static inline uint64_t rotr64(uint64_t x, size_t n)
{
    return (x >> n) | (x << (64 - n));
}

static inline uint128_t rotr128(uint128_t x, size_t n)
{
    return (x >> n) | (x << (128 - n));
}

#ifdef LITTLE_ENDIAN

#define cpu_to_le128(x) ((uint128_t)(x))
#define cpu_to_le64(x) ((uint64_t)(x))
#define cpu_to_le32(x) ((uint32_t)(x))
#define cpu_to_le16(x) ((uint16_t)(x))

#define cpu_to_be128(x) (bswap128((x)))
#define cpu_to_be64(x) (bswap64((x)))
#define cpu_to_be32(x) (bswap32((x)))
#define cpu_to_be16(x) (bswap16((x)))

#define le128_to_cpu(x) ((uint128_t)(x))
#define le64_to_cpu(x) ((uint64_t)(x))
#define le32_to_cpu(x) ((uint32_t)(x))
#define le16_to_cpu(x) ((uint16_t)(x))

#define be128_to_cpu(x) (bswap128((x)))
#define be64_to_cpu(x) (bswap64((x)))
#define be32_to_cpu(x) (bswap32((x)))
#define be16_to_cpu(x) (bswap16((x)))

#elif BIG_ENDIAN

#define cpu_to_le128(x) (bswap128((x)))
#define cpu_to_le64(x) (bswap64((x)))
#define cpu_to_le32(x) (bswap32((x)))
#define cpu_to_le16(x) (bswap16((x)))

#define cpu_to_be128(x) ((uint128_t)(x))
#define cpu_to_be64(x) ((uint64_t)(x))
#define cpu_to_be32(x) ((uint32_t)(x))
#define cpu_to_be16(x) ((uint16_t)(x))

#define le128_to_cpu(x) (bswap128((x)))
#define le64_to_cpu(x) (bswap64((x)))
#define le32_to_cpu(x) (bswap32((x)))
#define le16_to_cpu(x) (bswap16((x)))

#define be128_to_cpu(x) ((uint128_t)(x))
#define be64_to_cpu(x) ((uint64_t)(x))
#define be32_to_cpu(x) ((uint32_t)(x))
#define be16_to_cpu(x) ((uint16_t)(x))

#endif

#endif
