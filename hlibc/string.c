/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <stdlib.h>
#include <ctype.h>
#include <tgmath.h>
#include <hlibc/bit.h>
#include <hlibc/string.h>

void *zalloc(size_t n)
{
    return calloc(1, n);
}

char *stralloc(size_t n)
{
    char *buf = malloc(n + 1);
    if (buf)
        buf[n] = '\0';
    return buf;
}

void *memdup(const void *src, size_t n)
{
    void *ret = malloc(n);
    if (ret)
        memcpy(ret, src, n);
    return ret;
}

void memswap(void *restrict a, void *restrict b, size_t n)
{
    uint8_t *x = a;
    uint8_t *y = b;

    for (int i = 0; i < n; i++) {
        uint8_t tmp = x[i];
        x[i] = y[i];
        y[i] = tmp;
    }
}

void memzero(void *ptr, size_t n)
{
    memset(ptr, 0, n);
}

static const int16_t hex_val_table[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0,  1,  2,  3,  4,  5,  6,  7,  8,
    9,  -1, -1, -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

int hex_val(char c)
{
    return hex_val_table[(unsigned char)c];
}

const char hex_cset[] = "0123456789ABCDEF";

void hex_encode(const void *restrict src, size_t n, char *restrict dest)
{
    const uint8_t *pos = src;
    const uint8_t *end = pos + n;

    while (pos < end) {
        hex_encode_byte(*pos++, dest);
        dest += 2;
    }

    *dest = 0;
}

void hex_encode_byte(uint8_t src, char *dest)
{
    dest[0] = hex_cset[src >> 4];
    dest[1] = hex_cset[src & 15];
}

int hex_decode(const char *restrict src, void *restrict dest, size_t size)
{
    int a, b;
    int i = 0;
    uint8_t *out = dest;

    while (*src) {
        if ((a = hex_val(*src++)) < 0 || (b = hex_val(*src++)) < 0)
            return EINVAL;

        if (i == size)
            return ENOMEM;

        out[i++] = (a << 4) + b;
    }

    return 0;
}

int b64_val(char c)
{
    static const int b64_table[] = {
        [0 ... 255] = -1, ['A'] = 0,  ['B'] = 1,  ['C'] = 2,  ['D'] = 3,
        ['E'] = 4,        ['F'] = 5,  ['G'] = 6,  ['H'] = 7,  ['I'] = 8,
        ['J'] = 9,        ['K'] = 10, ['L'] = 11, ['M'] = 12, ['N'] = 13,
        ['O'] = 14,       ['P'] = 15, ['Q'] = 16, ['R'] = 17, ['S'] = 18,
        ['T'] = 19,       ['U'] = 20, ['V'] = 21, ['W'] = 22, ['X'] = 23,
        ['Y'] = 24,       ['Z'] = 25, ['a'] = 26, ['b'] = 27, ['c'] = 28,
        ['d'] = 29,       ['e'] = 30, ['f'] = 31, ['g'] = 32, ['h'] = 33,
        ['i'] = 34,       ['j'] = 35, ['k'] = 36, ['l'] = 37, ['m'] = 38,
        ['n'] = 39,       ['o'] = 40, ['p'] = 41, ['q'] = 42, ['r'] = 43,
        ['s'] = 44,       ['t'] = 45, ['u'] = 46, ['v'] = 47, ['w'] = 48,
        ['x'] = 49,       ['y'] = 50, ['z'] = 51, ['0'] = 52, ['1'] = 53,
        ['2'] = 54,       ['3'] = 55, ['4'] = 56, ['5'] = 57, ['6'] = 58,
        ['7'] = 59,       ['8'] = 60, ['9'] = 61, ['+'] = 62, ['/'] = 63,
    };

    return b64_table[(unsigned char)c];
}

void b64_encode(const void *restrict src, size_t n, char *restrict dest)
{
    static const char b64_charset[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    const uint8_t *in = src;
    const uint8_t *in_last = src + n - 3;

    while (in <= in_last) {
        *dest++ = b64_charset[in[0] >> 2];
        *dest++ = b64_charset[(in[0] & 0x3) << 4 | in[1] >> 4];
        *dest++ = b64_charset[(in[1] & 0xF) << 2 | in[2] >> 6];
        *dest++ = b64_charset[in[2] & 0x3F];

        in += 3;
    }

    switch (n % 3) {
    case 1:
        *dest++ = b64_charset[in[0] >> 2];
        *dest++ = b64_charset[(in[0] & 0x3) << 4];
        *dest++ = '=';
        *dest++ = '=';
        break;
    case 2:
        *dest++ = b64_charset[in[0] >> 2];
        *dest++ = b64_charset[(in[0] & 0x3) << 4 | in[1] >> 4];
        *dest++ = b64_charset[(in[1] & 0xF) << 2];
        *dest++ = '=';
        break;
    }

    *dest++ = 0;
}

int b64_decode(const char *restrict src, void *restrict dest, size_t size)
{
    int a, b, c, d;
    int i = 0;
    uint8_t *out = dest;
    uint8_t *out_end = out + size;

    while ((a = b64_val(src[i++])) >= 0 && (b = b64_val(src[i++])) >= 0 &&
           (c = b64_val(src[i++])) >= 0 && (d = b64_val(src[i++])) >= 0) {
        if (out + 2 >= out_end)
            return ENOMEM;

        *out++ = a << 2 | b >> 4;
        *out++ = b << 4 | c >> 2;
        *out++ = c << 6 | d >> 0;
    }

    src += --i & ~3;

    switch (i % 4) {
    case 0:
        return !!src[0] * EINVAL;

    case 1:
        return EINVAL;

    case 2:
        if (src[2] && (src[2] != '=' || src[3] != '=' || src[4]))
            return EINVAL;

        if (out >= out_end)
            return ENOMEM;

        *out++ = a << 2 | b >> 4;
        break;

    case 3:
        if (src[3] && (src[3] != '=' || src[4]))
            return EINVAL;

        if (out + 1 >= out_end)
            return ENOMEM;

        *out++ = a << 2 | b >> 4;
        *out++ = b << 4 | c >> 2;
        break;
    }

    return 0;
}

int int128_to_string(int128_t val, char *buf, size_t buf_size)
{
    char tmp[40];
    int scale = 1 - 2 * (val < 0);
    int i = 0;

    if (!val)
        tmp[i++] = '0';

    while (val) {
        tmp[i++] = scale * (val % 10) + '0';
        val /= 10;
    }

    if (scale == -1)
        tmp[i++] = '-';

    if (i + 1 > buf_size)
        return ENOMEM;

    for (int j = 0; j < i; j++)
        buf[j] = tmp[i - 1 - j];

    buf[i] = 0;
    return 0;
}

int uint128_to_string(uint128_t val, char *buf, size_t buf_size)
{
    char tmp[40];
    int i = 0;

    if (!val)
        tmp[i++] = '0';

    while (val) {
        tmp[i++] = (val % 10) + '0';
        val /= 10;
    }

    if (i + 1 > buf_size)
        return ENOMEM;

    for (int j = 0; j < i; j++)
        buf[j] = tmp[i - 1 - j];

    buf[i] = 0;
    return 0;
}
