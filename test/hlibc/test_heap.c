/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <testlib/testlib.h>
#include <hlibc/heap.h>

bool less_int(const void *a, const void *b)
{
    return *(int *)a < *(int *)b;
}

TEST(test_heapify)
{
    int x[][5] = {
        { 1, 2, 3, 4, 5 },
        { 5, 4, 3, 2, 1 },
        { -12, 4, 42, 299, -51 },
    };

    for (int i = 0; i < array_size(x); i++) {
        heapify(x + i, array_size(x), sizeof(int), less_int);
        ASSERT(is_minheap(x + i, array_size(x), sizeof(int), less_int));
    }
}

TEST(test_heap_add)
{
    int n = 7;
    int x[10];

    for (int i = 0; i < n; i++)
        x[i] = i + 1;

    int ret[] = {
        *(int *)heap_add(&(int){ -52 }, x, n++, sizeof(int), less_int),
        *(int *)heap_add(&(int){ 128 }, x, n++, sizeof(int), less_int),
        *(int *)heap_add(&(int){ 42 }, x, n++, sizeof(int), less_int),
    };

    ASSERT_EQ(ret[0], -52);
    ASSERT_EQ(ret[1], 128);
    ASSERT_EQ(ret[2], 42);
    ASSERT(is_minheap(x, n, sizeof(int), less_int));
}

TEST(test_heap_pop)
{
    {
        int x[] = { -52, 2, 1, 4, 5, 3, 128, 42 };
        int order[] = { -52, 1, 2, 3, 4, 5, 42, 128 };
        int n = array_size(x);

        for (int i = 0; n > 0; i++) {
            int copy;
            heap_pop(&copy, x, n--, sizeof(int), less_int);

            ASSERT_EQ(copy, order[i]);
            ASSERT(is_minheap(x, n, sizeof(int), less_int));
        }
    }

    {
        int x[] = { 1, 2, 3 };
        heap_pop(NULL, x, 3, sizeof(int), less_int);
        ASSERT_EQ(x[0], 2);
        ASSERT_EQ(x[1], 3);
    }
}
