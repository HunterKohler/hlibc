/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <hlibc/dstring.h>
#include <hlibc/math.h>
#include <stdlib.h>

char *_ds_new(size_t len, size_t cap)
{
    char *data = malloc(cap + 1 + 2 * sizeof(size_t));

    if (data) {
        ((size_t *)data)[0] = len;
        ((size_t *)data)[1] = cap;
        data += 2 * sizeof(size_t);
        data[len] = 0;
    }

    return data;
}

char *ds_new(size_t n)
{
    char *ds = _ds_new(0, n);
    if (ds) ds[0] = 0;
    return ds;
}

size_t ds_len(const char *ds)
{
    return ((size_t *)ds)[-2];
}

size_t ds_cap(const char *ds)
{
    return ((size_t *)ds)[-1];
}

void ds_free(char *ds)
{
    free((size_t *)ds - 2);
}

char *ds_dup(const char *ds)
{
    size_t len = ds_len(ds);
    char *dup = _ds_new(len, len);

    if (dup) {
        memcpy(dup, ds, len + 1);
    }

    return dup;
}

int ds_reserve(char **ds_ptr, size_t n)
{
    if (n > ds_cap(*ds_ptr)) {
        char *res = realloc((size_t *)ds_ptr - 2, n);
        if (!res) {
            return -1;
        }

        ((size_t *)res)[1] = n;
        *ds_ptr = res + 2 * sizeof(size_t);
    }

    return 0;
}

int ds_resize(char **ds_ptr, size_t n, char c)
{
    size_t len = ds_len(*ds_ptr);

    if (ds_reserve(ds_ptr, n)) {
        return -1;
    }

    ((size_t *)*ds_ptr)[-2] = n;

    if (n > len) {
        memset(*ds_ptr + len, c, n - len);
    }

    (*ds_ptr)[n] = 0;
    return 0;
}

char *ds_substr(const char *str, size_t a, size_t b)
{
    size_t len = b - a;
    char *sub = _ds_new(len, len);

    if (sub) {
        memcpy(sub, str, len);
    }

    return sub;
}

int ds_cmp(const char *ds_1, const char *ds_2)
{
    size_t len_1 = ds_len(ds_1);
    size_t len_2 = ds_len(ds_2);
    int res = memcmp(ds_1, ds_2, min(len_1, len_2));
    return res ? res : len_2 - len_1;
}

// int ds_append(char **dest, const char *src)
// {
//     ds_reserve(dest, ds_len(*dest) + ds_len(src));
// }
