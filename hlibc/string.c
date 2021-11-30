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
        memcpy(dest + len1, src2, len2);
    }

    return dest;
}
