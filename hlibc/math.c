/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <math.h>
#include <limits.h>
#include <hlibc/math.h>
#include <hlibc/bit.h>

unsigned int prime_count_upper(unsigned int n)
{
    return (unsigned int)(1.25506 * n / log(n)) + 1;
}

unsigned int prime_count_lower(unsigned int n)
{
    return (unsigned int)(n / log(n));
}

double sigmoid(double x)
{
    return 1 / (1 + exp(-x));
}

unsigned int floor_log2(unsigned int x)
{
    return CHAR_BIT * sizeof(x) - __builtin_clz(x) - 1;
}

unsigned int floor_log10(unsigned int x)
{
    /* Using unsigned char to save space. */
    static unsigned char pow2_width[] = {
        1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5,  5,
        5, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10,
    };

    static unsigned int pow10[] = {
        0,      10,      100,      1000,      10000,
        100000, 1000000, 10000000, 100000000, 1000000000,
    };

    int digits = pow2_width[floor_log2(x) + 1];
    return digits - (x < pow10[digits - 1]);
}
