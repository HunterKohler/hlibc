/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <stdlib.h>
#include <hlibc/string.h>

char *stralloc(size_t n)
{
    char *buf = malloc(n + 1);
    if (buf)
        buf[n] = '\0';
    return buf;
}

char *astrcat(const char *src1, const char *src2)
{
    size_t len1 = strlen(src1);
    size_t len2 = strlen(src2);
    char *dest = malloc(len1 + len2 + 1);

    if (dest) {
        memcpy(dest, src1, len1);
        memcpy(dest + len1, src2, len2 + 1);
    }

    return dest;
}

char *substr(const char *src, size_t begin, size_t end)
{
    size_t n = begin < end ? end - begin : 0;
    char *dest = malloc(n + 1);

    if (dest) {
        memcpy(dest, src + begin, n);
        dest[n] = 0;
    }

    return dest;
}

void *memdup(const void *src, size_t n)
{
    void *ret = malloc(n);
    if (ret)
        memcpy(ret, src, n);
    return ret;
}

// TODO benchmark against method of doubling area copied and linear copy.
void *memrep(const void *src, size_t n, size_t m)
{
    void *ret = malloc(n * m);
    if (ret) {
        for (int i = 0; i < m; i++)
            memcpy(ret + i * n, src, n);
    }
    return ret;
}

int hex_val_table[] = {
    [0 ... 255] = -1, ['0'] = 0,  ['1'] = 1,  ['2'] = 2,  ['3'] = 3,
    ['4'] = 4,        ['5'] = 5,  ['6'] = 6,  ['7'] = 7,  ['8'] = 8,
    ['9'] = 9,        ['a'] = 10, ['b'] = 11, ['c'] = 12, ['d'] = 13,
    ['e'] = 14,       ['f'] = 15, ['A'] = 10, ['B'] = 11, ['C'] = 12,
    ['D'] = 13,       ['E'] = 14, ['F'] = 15,
};

int hex_val(char c)
{
    return hex_val_table[(unsigned char)c];
}
