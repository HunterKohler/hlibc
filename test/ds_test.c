/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <assert.h>
#include <stdio.h>
#include "../src/dstring.c"

void test_ds_new()
{
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
    size_t arr[] = { 10, 5 };
    assert(ds_len((char *)(arr + 2)) == 10);
}

void test_ds_cap()
{
    size_t arr[] = { 10, 5 };
    assert(ds_cap((char *)(arr + 2)) == 5);
}

void ds_dup()
{
    char *ds = ds_new(10);
}

int main()
{
    test_ds_new();
    printf("Success\n");
}
