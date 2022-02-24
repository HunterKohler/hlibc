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
    return (CHAR_BIT * sizeof(x) - 1) - __builtin_clz(x);
}

unsigned int ceil_log2(unsigned int x)
{
    return floor_log2(x) + !!(x & (x - 1));
}

int gcd(int a, int b)
{
    a = abs(a);
    b = abs(b);

    if (a < b) {
        int c = a;
        a = b;
        b = c;
    }

    while (b) {
        int c = a % b;
        a = b;
        b = c;
    }

    return a;
}

int lcm(int a, int b)
{
    int c = gcd(a, b);
    return abs(a * b) / (c + !c);
}
