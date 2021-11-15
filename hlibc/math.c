/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <math.h>
#include <sys/types.h>
#include <hlibc/math.h>

size_t prime_count_upper(size_t n)
{
    return (size_t)(1.25506 * n / log(n)) + 1;
}

size_t prime_count_lower(size_t n)
{
    return (size_t)(n / log(n));
}
