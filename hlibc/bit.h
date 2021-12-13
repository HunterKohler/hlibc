/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 *
 * Bit Manipulation
 */

#ifndef HLIBC_BIT_H_
#define HLIBC_BIT_H_

#include <string.h>
#include <hlibc/def.h>

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
 * Get the value of the bit at an offset `i` from `target`.
 */
int bit_at(const void *target, long long i);

/*
 * Take hamming distance on arbitrary `n` bytes.
 */
size_t hamming_distance(const void *a, const void *b, size_t n);

uint8_t rotl8(uint8_t x, size_t n);
uint8_t rotr8(uint8_t x, size_t n);
uint16_t rotl16(uint16_t x, size_t n);
uint16_t rotr16(uint16_t x, size_t n);
uint32_t rotl32(uint32_t x, size_t n);
uint32_t rotr32(uint32_t x, size_t n);
uint64_t rotl64(uint64_t x, size_t n);
uint64_t rotr64(uint64_t x, size_t n);
uint128_t rotl128(uint128_t x, size_t n);
uint128_t rotr128(uint128_t x, size_t n);

#endif
