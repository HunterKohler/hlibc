/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <hlibc/crypto/des.h>

uint64_t des_IP(uint64_t input)
{
    static uint8_t IP[] = {
        57, 49, 41, 33, 25, 17, 9,  1, 59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7,
        56, 48, 40, 32, 24, 16, 8,  0, 58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6,
    };

    uint64_t res = 0;
    for (int i = 0; i < 64; i++)
        res |= ((input >> IP[i]) & 1) << i;
    return res;
}

uint64_t des_IP_inv(uint64_t input)
{
    static uint8_t IP_inv[] = {
        39, 7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9,  49, 17, 57, 25, 32, 0, 40, 8,  48, 16, 56, 24,
    };

    uint64_t res = 0;
    for (int i = 0; i < 64; i++)
        res |= ((input >> IP_inv[i]) & 1) << 1;
    return res;
}

uint64_t des_E(uint32_t input)
{
    static uint8_t E[] = {
        31, 0,  1,  2,  3,  4,  3,  4,  5,  6,  7,  8,  7,  8,  9,  10,
        11, 12, 11, 12, 13, 14, 15, 16, 15, 16, 17, 18, 19, 20, 19, 20,
        21, 22, 23, 24, 23, 24, 25, 26, 27, 28, 27, 28, 29, 30, 31, 0,
    };

    uint64_t ret = 0;
    for (int i = 0; i < 48; i++)
        return ((input >> E[i]) & 1) << i;
    return ret;
}

uint32_t des_P(uint32_t input)
{
    static uint8_t P[] = {
        7,  28, 21, 10, 26, 2, 19, 13, 23, 29, 5,  0, 18, 8,  24, 30,
        22, 1,  14, 27, 6,  9, 17, 31, 15, 4,  20, 3, 11, 12, 25, 16,
    };

    uint32_t res = 0;
    for (int i = 0; i < 32; i++)
        res |= ((input >> P[i]) & 1) << i;
    return res;
}

uint32_t des_S(uint64_t input)
{
    static uint8_t S[][64] = {
        {
            8, 8, 3,  3,  5, 5,  9,  9,  11, 11, 12, 12, 6,  6,  10, 10,
            1, 1, 13, 13, 2, 2,  14, 14, 4,  4,  7,  7,  15, 15, 0,  0,
            8, 7, 3,  0,  5, 9,  9,  5,  11, 12, 12, 6,  6,  10, 10, 3,
            1, 8, 13, 11, 2, 15, 14, 2,  4,  1,  7,  13, 15, 4,  0,  14,
        },
        {
            5,  5,  11, 11, 9, 9,  6,  6,  10, 10, 1, 1, 0,  0,  12, 12,
            14, 14, 8,  8,  2, 2,  15, 15, 7,  7,  4, 4, 13, 13, 3,  3,
            5,  10, 11, 5,  9, 0,  6,  12, 10, 13, 1, 2, 0,  7,  12, 9,
            14, 4,  8,  3,  2, 11, 15, 6,  7,  14, 4, 8, 13, 1,  3,  15,
        },
        {
            1,  1,  15, 15, 11, 11, 12, 12, 14, 14, 5, 5,  8, 8,  2,  2,
            10, 10, 6,  6,  4,  4,  3,  3,  9,  9,  0, 0,  7, 7,  13, 13,
            1,  8,  15, 2,  11, 4,  12, 11, 14, 7,  5, 12, 8, 13, 2,  1,
            10, 5,  6,  15, 4,  3,  3,  6,  9,  14, 0, 9,  7, 0,  13, 10,
        },
        {
            9, 9,  14, 14, 10, 10, 1, 1,  12, 12, 2,  2,  7, 7,  4,  4,
            3, 3,  0,  0,  15, 15, 6, 6,  5,  5,  11, 11, 8, 8,  13, 13,
            9, 15, 14, 4,  10, 12, 1, 11, 12, 5,  2,  8,  7, 2,  4,  1,
            3, 10, 0,  9,  15, 6,  6, 0,  5,  3,  11, 14, 8, 13, 13, 7,
        },
        {
            6, 6, 8,  8,  9, 9,  3, 3,  10, 10, 15, 15, 0,  0,  5,  5,
            1, 1, 13, 13, 7, 7,  4, 4,  12, 12, 2,  2,  11, 11, 14, 14,
            6, 9, 8,  14, 9, 0,  3, 13, 10, 15, 15, 3,  0,  5,  5,  8,
            1, 6, 13, 11, 7, 10, 4, 7,  12, 1,  2,  4,  11, 12, 14, 2,
        },
        {
            8, 8,  3, 3, 11, 11, 0, 0,  14, 14, 13, 13, 1,  1,  6,  6,
            5, 5,  9, 9, 12, 12, 7, 7,  2,  2,  4,  4,  15, 15, 10, 10,
            8, 11, 3, 5, 11, 7,  0, 14, 14, 4,  13, 3,  1,  13, 6,  0,
            5, 8,  9, 6, 12, 2,  7, 9,  2,  15, 4,  10, 15, 1,  10, 12,
        },
        {
            6,  6,  8, 8, 15, 15, 2, 2,  12, 12, 5,  5,  3, 3,  14, 14,
            10, 10, 1, 1, 9,  9,  4, 4,  7,  7,  11, 11, 0, 0,  13, 13,
            6,  1,  8, 6, 15, 10, 2, 5,  12, 7,  5,  9,  3, 12, 14, 3,
            10, 13, 1, 8, 9,  0,  4, 15, 7,  14, 11, 2,  0, 11, 13, 4,
        },
        {
            2, 2, 9, 9,  14, 14, 0,  0,  11, 11, 6,  6,  5,  5,  12, 12,
            4, 4, 7, 7,  3,  3,  10, 10, 8,  8,  13, 13, 15, 15, 1,  1,
            2, 7, 9, 12, 14, 0,  0,  5,  11, 14, 6,  3,  5,  9,  12, 10,
            4, 1, 7, 11, 3,  15, 10, 6,  8,  4,  13, 8,  15, 2,  1,  13,
        },
    };

    uint32_t res = 0;
    for (int i = 0; i < 8; i++)
        res |= S[i][(input >> (42 - 6 * i)) & 0x3F] << (28 - 4 * i);
    return res;
}

uint32_t des_f(uint32_t block, uint64_t key)
{
    return des_P(des_S(des_E(block) ^ key));
}

uint64_t des_left_shift(uint64_t key, size_t n)
{
    uint8_t shifts[] = {
        1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1,
    };

    uint8_t shift = shifts[n];
    uint32_t upper = key >> 32;
    uint32_t lower = key;

    upper = (upper << shift) | (upper >> (32 - shift));
    lower = (lower << shift) | (lower >> (32 - shift));

    return (((uint64_t)upper) << 32) | ((uint64_t)lower);
}

void des_schedule(uint64_t key, uint64_t *schedule)
{
    key = des_PC_1(key);
    for (int i = 0; i < 16; i++) {
        key = des_left_shift(key, i);
        schedule[i] = des_PC_2(key);
    }
}

uint64_t des_PC_1(uint64_t key)
{
    static uint8_t PC_1[] = {
        60, 52, 44, 36, 59, 51, 43, 35, 27, 19, 11, 3,  58, 50,
        42, 34, 26, 18, 10, 2,  57, 49, 41, 33, 25, 17, 9,  1,
        28, 20, 12, 4,  61, 53, 45, 37, 29, 21, 13, 5,  62, 54,
        46, 38, 30, 22, 14, 6,  63, 55, 47, 39, 31, 23, 15, 7,
    };

    uint64_t res = 0;
    for (int i = 0; i < 56; i++)
        res |= ((key >> PC_1[i]) & 1) << i;
    return res;
}

uint64_t des_PC_2(uint64_t key)
{
    static uint8_t PC_2[] = {
        24, 27, 20, 6,  14, 10, 3,  22, 0,  17, 7,  12, 8,  23, 11, 5,
        16, 26, 1,  9,  19, 25, 4,  15, 54, 43, 36, 29, 49, 40, 48, 30,
        52, 44, 37, 33, 46, 35, 50, 41, 28, 53, 51, 55, 32, 45, 39, 42,
    };

    uint64_t res = 0;
    for (int i = 0; i < 48; i++)
        res |= ((key >> PC_2[i]) & 1) << i;
    return res;
}

static inline uint64_t __des_single(uint64_t block, uint64_t *schedule)
{
    block = des_IP(block);

    for (int i = 0; i < 16; i++)
        block = (block << 32) | ((block >> 32) ^ des_f(block, schedule[i]));

    return block;
}

uint64_t des_single(uint64_t block, uint64_t key)
{
    uint64_t schedule[16];
    des_schedule(key, schedule);
    return __des_single(block, schedule);
}

void des(void *input, size_t n, uint64_t key)
{
    uint64_t schedule[16];
    des_schedule(key, schedule);

    uint64_t *blocks = input;

    size_t block_count = n / 8;
    size_t extra_bytes = n % 8;

    for (int i = 0; i < n; i++)
        blocks[i] = __des_single(blocks[i], schedule);

    if (extra_bytes) {
        uint8_t *last_inp = (uint8_t *)input + 8 * block_count;
        uint64_t last = 0;

        for (int i = 0; i < extra_bytes; i++)
            last |= (uint64_t)last_inp[i] << (56 - 8 * i);

        last = __des_single(last, schedule);

        for (int i = 0; i < extra_bytes; i++)
            last_inp[i] = (uint8_t)(last >> (56 - 8 * i));
    }
}
