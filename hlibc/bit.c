/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <hlibc/bit.h>

int bit_at(const void *target, long long i)
{
    return (((const char *)target)[i >> 3] >> (7 - (i & 7))) & 1;
}

uint8_t rotl_8(uint8_t x, size_t n)
{
    return (x << n) | (x >> (8 - n));
}

uint8_t rotr8(uint8_t x, size_t n)
{
    return (x >> n) | (x << (8 - n));
}

uint16_t rotl_16(uint16_t x, size_t n)
{
    return (x << n) | (x >> (16 - n));
}

uint16_t rotr16(uint16_t x, size_t n)
{
    return (x >> n) | (x << (16 - n));
}

uint32_t rotl_32(uint32_t x, size_t n)
{
    return (x << n) | (x >> (32 - n));
}

uint32_t rotr32(uint32_t x, size_t n)
{
    return (x >> n) | (x << (32 - n));
}

uint64_t rotl_64(uint64_t x, size_t n)
{
    return (x << n) | (x >> (64 - n));
}

uint64_t rotr64(uint64_t x, size_t n)
{
    return (x >> n) | (x << (64 - n));
}

uint128_t rotl_128(uint128_t x, size_t n)
{
    return (x << n) | (x >> (128 - n));
}

uint128_t rotr128(uint128_t x, size_t n)
{
    return (x >> n) | (x << (128 - n));
}

size_t hamming_distance(const void *a, const void *b, size_t n)
{
    size_t dist = 0;
    size_t i = 0;

    for (; i + sizeof(unsigned long long) < n; i += sizeof(unsigned long long))
        dist += popcountll(*((const unsigned long long *)(a + i)) ^
                           *((const unsigned long long *)(b + i)));

    for (; i < n; i++)
        dist += popcount(*((const unsigned char *)(a + i)) ^
                         *((const unsigned char *)(b + i)));

    return dist;
}
