/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <stdlib.h>
#include <string.h>
#include <hlibc/mem.h>

void *memdup(const void *src, size_t n)
{
    void *ret = malloc(n);
    if (ret) {
        memcpy(ret, src, n);
    } else if (!errno) {
        errno = ENOMEM;
    }
    return ret;
}

// TODO benchmark against method of doubling area copied and linear copy.
void *memrep(const void *src, size_t n, size_t m)
{
    void *ret = malloc(n * m);
    if (ret) {
        for (int i = 0; i < m; i++) {
            memcpy(ret + i * n, src, n);
        }
    } else if (!errno) {
        errno = ENOMEM;
    }
    return ret;
}
