/*
 * Copyright (C) 2021-2022 Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef SHARED_H_
#define SHARED_H_

#include <ctype.h>

static const double en_letter_freq[256] = {
    [0 ... 255] = 0, ['A'] = 0.00431, ['B'] = 0.00210, ['C'] = 0.00352,
    ['D'] = 0.00199, ['E'] = 0.00212, ['F'] = 0.00155, ['G'] = 0.00143,
    ['H'] = 0.00190, ['I'] = 0.00342, ['J'] = 0.00121, ['K'] = 0.00071,
    ['L'] = 0.00164, ['M'] = 0.00398, ['N'] = 0.00315, ['O'] = 0.00162,
    ['P'] = 0.00221, ['Q'] = 0.00018, ['R'] = 0.00225, ['S'] = 0.00468,
    ['T'] = 0.00499, ['U'] = 0.00088, ['V'] = 0.00048, ['W'] = 0.00164,
    ['Y'] = 0.00012, ['X'] = 0.00145, ['Z'] = 0.00009, ['a'] = 0.08073,
    ['b'] = 0.01328, ['c'] = 0.03007, ['d'] = 0.03634, ['e'] = 0.11873,
    ['f'] = 0.01989, ['g'] = 0.01851, ['h'] = 0.04533, ['i'] = 0.06943,
    ['j'] = 0.00101, ['k'] = 0.00707, ['l'] = 0.03916, ['m'] = 0.02250,
    ['n'] = 0.06956, ['o'] = 0.07253, ['p'] = 0.01926, ['q'] = 0.00083,
    ['r'] = 0.06346, ['s'] = 0.06420, ['t'] = 0.08447, ['u'] = 0.02474,
    ['v'] = 0.01002, ['w'] = 0.01558, ['x'] = 0.00190, ['y'] = 0.01629,
    ['x'] = 0.00102,
};


static inline double single_xor_error(const void *buf, size_t size, uint8_t key)
{
    const uint8_t *input = buf;

    int count[256] = { 0 };
    double error = 0;

    for (int i = 0; i < size; i++) {
        uint8_t res = input[i] ^ key;
        count[res]++;
    }

    for (int i = 0; i < 256; i++) {
        error += fabs(en_letter_freq[i] - (double)count[i] / size);
    }

    return error;
}

static inline size_t hamming_dist(const void *a, const void *b, size_t n)
{
    const uint8_t *x = a;
    const uint8_t *y = b;

    size_t dist = 0;
    for(int i = 0; i < n; i++)
        dist += __builtin_popcount(x[i] ^ y[i]);
    return dist;
}

#endif
