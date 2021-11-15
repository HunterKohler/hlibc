/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include "hlibc/dstring.c"

void test_ds_new()
{
    // Test:
    for (int i = 0; i < 2; i++) {
        int n = ((int[]){ 1, 100 })[i];
        char *ds;

        ds = ds_new(n);

        assert(((size_t *)ds)[-2] == 0);
        assert(((size_t *)ds)[-1] == n);
        assert(ds);
        assert(!ds[0]);

        ds_free(ds);
    }
}

void test_ds_len()
{
    // Test:
    size_t arr[] = { 10, 5 };
    assert(ds_len((char *)(arr + 2)) == 10);
}

void test_ds_cap()
{
    // Test:
    size_t arr[] = { 10, 5 };
    assert(ds_cap((char *)(arr + 2)) == 5);
}

void test_ds_dup()
{
    // Test:
    size_t arr[4] = { sizeof(size_t), 2 * sizeof(size_t), 0, 0 };
    memcpy(arr + 2, "abcdefghijklmnopqrstuvwxyz", sizeof(size_t));
    size_t *result = (size_t *)ds_dup((char *)(arr + 2)) - 2;
    arr[1] = sizeof(size_t);

    assert(arr != result);
    assert(!memcmp(arr, result, 3 * sizeof(size_t)));
}

void test_ds_reserve()
{
    // Test:
    size_t n = 100000;
    size_t arr[3] = { 0, sizeof(size_t) - 1, 0 };

    ds_reserve((char **)&arr, n);

    memset(arr, 0, n); // You'll know it doen't work if this crashes; maybe?
    assert(arr[0] == 0);
    assert(arr[1] == n);

    // Test:
    assert(!errno);
    assert(ds_reserve((char **)&arr, -1));
    assert(errno);

    errno = 0;
}

void test_ds_resize()
{
    // Test:
    size_t n = 10000;
    char c = '*';
    size_t arr[3] = { sizeof(size_t), sizeof(size_t) - 1, 0 };

    ds_resize((char **)&arr, n, c);

    assert(arr[0] == n);
    assert(arr[1] == n);
    for (int i = 0; i < n; i++) {
        assert(((char *)arr)[i] == '*');
    }
}

int main()
{
    test_ds_new();
    test_ds_len();
    test_ds_cap();
    test_ds_dup();
    test_ds_reserve();

    printf("Success\n");
}
