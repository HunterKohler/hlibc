/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <hlibc/bit.h>

int bit_at(const void *target, size_t i)
{
    return (((const char *)target)[i >> 3] >> (7 - (i & 7))) & 1;
}

uint32_t rotl_32(uint32_t x, size_t n)
{
    return (x << n) | (x >> (32 - n));
}
