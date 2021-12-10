/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <stdlib.h>
#include <ctype.h>
#include <tgmath.h>
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

int hex_val(char c)
{
    static uint8_t hex_val_table[] = {
        [0 ... 255] = -1, ['0'] = 0,  ['1'] = 1,  ['2'] = 2,  ['3'] = 3,
        ['4'] = 4,        ['5'] = 5,  ['6'] = 6,  ['7'] = 7,  ['8'] = 8,
        ['9'] = 9,        ['a'] = 10, ['b'] = 11, ['c'] = 12, ['d'] = 13,
        ['e'] = 14,       ['f'] = 15, ['A'] = 10, ['B'] = 11, ['C'] = 12,
        ['D'] = 13,       ['E'] = 14, ['F'] = 15,
    };

    return hex_val_table[(unsigned char)c];
}

int strcmp_safe(const char *a, const char *b)
{
    return a == b ? 0 : a ? b ? strcmp(a, b) : -1 : 1;
}

int strcasecmp_safe(const char *a, const char *b)
{
    return (a == b) ? 0 : a ? b ? strcasecmp(a, b) : -1 : 1;
}

char *strcat_n(const char **src, size_t n)
{
    size_t *lens = malloc(n * sizeof(*lens));
    size_t len = 0;

    if (!lens)
        return NULL;

    for (int i = 0; i < n; i++) {
        if (src[i]) {
            lens[i] = strlen(src[i]);
            len += lens[i];
        }
    }

    char *buf = stralloc(len);
    if (!buf) {
        free(lens);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        if (src[i]) {
            memcpy(buf, src[i], lens[i]);
            buf += lens[i];
        }
    }

    free(lens);
    return buf - len;
}

size_t strtrim(char *str)
{
    size_t begin = strspn(str, " \t\n\r\f\v");
    size_t end = strlen(str);

    while (end && isspace(str[end - 1]))
        end--;

    size_t size = end - begin;

    memmove(str, str + begin, size);
    str[size] = 0;

    return size;
}

char *__to_string_c(char c, char *dest)
{
    if (dest || (dest = malloc(2))) {
        dest[0] = c;
        dest[1] = 0;
    }

    return dest;
}

char *__to_string_b(bool b, char *dest)
{
    int len = b ? 5 : 6;

    if (dest || (dest = malloc(len)))
        memcpy(dest, b ? "true" : "false", len);

    return dest;
}

char *__to_string_ld(long double d, char *dest)
{
    char tmp[350];
    size_t size = sprintf(tmp, "%Lf", d) + 1;

    if (dest || (dest = malloc(size)))
        memcpy(dest, tmp, size);

    return dest;
}

char *__to_string_cld(complex long double d, char *dest)
{
    char tmp[700];
    size_t size = sprintf(tmp, "%Lf + %Lfi", creal(d), cimag(d)) + 1;

    if (dest || (dest = malloc(size)))
        memcpy(dest, tmp, size);

    return dest;
}

char *__to_string_ll(long long n, char *dest, unsigned int base)
{
    if (!base || base > 36)
        return NULL;

    char tmp[CHAR_BIT * sizeof(n) + 2];
    size_t start = ARRAY_SIZE(tmp) - 1;

    tmp[start--] = 0;
    for (long long i = n; i; i /= base)
        tmp[start--] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[i % base];

    if (n < 0)
        tmp[start--] = '-';
    else if (n == 0)
        tmp[start--] = '0';

    start += 1;

    size_t size = ARRAY_SIZE(tmp) - start;
    if (dest || (dest = malloc(size)))
        memcpy(dest, tmp + start + 1, size);

    return dest;
}

char *__to_string_ull(unsigned long long n, char *dest, unsigned int base)
{
    if (!base || base > 36)
        return NULL;

    char tmp[CHAR_BIT * sizeof(n) + 2];
    size_t start = ARRAY_SIZE(tmp) - 1;

    tmp[start--] = 0;
    for (long long i = n; i; i /= base)
        tmp[start--] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[i % base];

    if (n == 0)
        tmp[start--] = '0';

    start += 1;

    size_t size = ARRAY_SIZE(tmp) - start;
    if (dest || (dest = malloc(size)))
        memcpy(dest, tmp + start + 1, size);

    return dest;
}

size_t hex_encode_size(size_t n)
{
    return 2 * n;
}

size_t hex_decode_size(size_t n)
{
    return (n + 1) / 2;
}

void hex_encode(const void *restrict src, size_t n, char *restrict dest)
{
    static const char hex_charset[] = "0123456789ABCDEF";

    const char *in = src;
    for (int i = 0; i < n; i++) {
        *dest++ = hex_charset[in[i] >> 4];
        *dest++ = hex_charset[in[i] & 15];
    }

    *dest = 0;
}

int hex_decode(const char *restrict src, void *restrict dest)
{
    char *out = dest;

    size_t len = strlen(src);
    if (len & 1) {
        if ((*out++ = hex_val(*src++)) < 0)
            return -1;
    }

    while (*src) {
        char a = hex_val(*src++);
        char b = hex_val(*src++);

        if (a < 0 || b < 0)
            return -1;

        *out++ = 16 * a + b;
    }

    return 0;
}

size_t b64_encode_size(size_t n)
{
    return ((n + 2) / 3) << 2;
}

size_t b64_decode_size(const char *src, size_t n)
{
    return (n >> 2) * 3 - (src ? (src[n - 1] == '=') + (src[n - 2] == '=') : 0);
}

void b64_encode(const void *restrict src, size_t n, char *restrict dest)
{
    static const char b64_charset[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    const uint8_t *in = src;
    for (const uint8_t *end = in + n - 3; in <= end; in += 3) {
        *dest++ = b64_charset[in[0] >> 2];
        *dest++ = b64_charset[(in[0] & 0x03) << 4 | in[1] >> 4];
        *dest++ = b64_charset[(in[1] & 0x0F) << 2 | in[2] >> 6];
        *dest++ = b64_charset[in[2] & 0x3F];
    }

    switch (n % 3) {
    case 1:
        *dest++ = b64_charset[in[0] >> 2];
        *dest++ = b64_charset[(in[0] & 0x03) << 4];
        *dest++ = '=';
        *dest++ = '=';
        break;
    case 2:
        *dest++ = b64_charset[in[0] >> 2];
        *dest++ = b64_charset[(in[0] & 0x03) << 4 | in[1] >> 4];
        *dest++ = b64_charset[(in[1] & 0x0F) << 2];
        *dest++ = '=';
        break;
    }
}

int b64_val(char c)
{
    static const uint8_t b64_table[] = {
        [0 ... 255] = 64, ['A'] = 0,  ['B'] = 1,  ['C'] = 2,  ['D'] = 3,
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

int b64_decode(const char *restrict src, size_t n, void *restrict dest)
{
    int i = 0;
    int end = n - 4 * (src[n - 1] == '=');

    uint8_t *out = dest;

    while (i < end) {
        uint8_t a, b, c, d;

        if ((a = b64_val(src[i++])) > 63 || (b = b64_val(src[i++])) > 63 ||
            (c = b64_val(src[i++])) > 63 || (d = b64_val(src[i++])) > 63)
            return -1;

        *out++ = a << 2 | b >> 4;
        *out++ = b << 4 | c >> 2;
        *out++ = c << 6 | d;
    }

    if (src[n - 2] == '=') {
        uint8_t a, b;

        if ((a = b64_val(src[i++])) > 63 || (b = b64_val(src[i++])) > 63)
            return -1;

        *out++ = a << 2 | b >> 4;
    } else if (src[n - 1] == '=') {
        uint8_t a, b, c;

        if ((a = b64_val(src[i++])) > 63 || (b = b64_val(src[i++])) > 63 ||
            (c = b64_val(src[i++])) > 63)
            return -1;

        *out++ = a << 2 | b >> 4;
        *out++ = b << 4 | c >> 2;
    }

    return 0;
}
