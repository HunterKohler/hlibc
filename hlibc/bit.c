/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <hlibc/bit.h>

int bit_at(const void *target, long long i)
{
    return (((const char *)target)[i >> 3] >> (7 - (i & 7))) & 1;
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
