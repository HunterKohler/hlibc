
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <hlibc/heap.h>

bool less_int(const void *a, const void *b)
{
    const int *x = a;
    const int *y = b;
    return *x < *y;
}

void test_heapify()
{
    int x[][5] = {
        { 1, 2, 3, 4, 5 },
        { 5, 4, 3, 2, 1 },
        { -12, 4, 42, 299, -51 },
    };

    size_t n = array_size(*x);
    size_t size = sizeof(**x);

    for (int i = 0; i < array_size(x); i++) {
        heapify(x + i, n, size, less_int);
        assert(is_minheap(x, n, size, less_int));
    }
}

void test_heap_add()
{
    {
        int *ret;
        int x[10] = { 1, 2, 3, 4, 5 };
        int n = 5;
        int size = sizeof(int);

        ret = heap_add(&(int){ -52 }, x, n++, size, less_int);
        assert(*ret == -52);

        ret = heap_add(&(int){ 128 }, x, n++, size, less_int);
        assert(*ret == 128);

        ret = heap_add(&(int){ 42 }, x, n++, size, less_int);
        assert(*ret == 42);

        assert(is_minheap(x, n, size, less_int));
    }
}

void test_heap_pop()
{
    {
        int x[] = { -52, 2, 1, 4, 5, 3, 128, 42};
        int order[] = { -52, 1, 2, 3, 4, 5, 42, 128};
        int size = sizeof(int);
        int n = array_size(x);
        int copy;

        for(int i = 0; n > 0; i++) {
            heap_pop(&copy, x, n--, size, less_int);

            assert(copy == order[i]);
            assert(is_minheap(x, n, size, less_int));
        }
    }

    {
        int x[] = { 1, 2, 3 };
        heap_pop(NULL, x, 3, sizeof(int), less_int);
        assert(!memcmp(x, &(int[]){ 2, 3 }, 2 * sizeof(int)));
    }
}

// int main()
// {
//     test_heapify();
//     test_heap_add();
//     test_heap_pop();
// }
