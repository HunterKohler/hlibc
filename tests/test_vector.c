/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <hlibc/vector.h>
#include <hlibc/mem.h>

static int test_vector_new()
{
    struct h_vector *vec = h_vector_new(sizeof(int));

    assert(vec);
    assert(!vec->size);
    assert(!vec->capacity);
    assert(vec->elem_size == sizeof(int));

    free(vec->data);
    free(vec);

    return 0;
}

static int test_vector_destroy()
{
    struct h_vector *vec = h_vector_new(sizeof(int));
    h_vector_destroy(vec);
    return 0;
}

static int test_vector_reserve()
{
    struct h_vector *vec = h_vector_new(sizeof(int));
    size_t n = 10;

    assert(!vec->capacity);
    assert(!vec->size);
    assert(!h_vector_reserve(vec, n));
    assert(vec->capacity == n * sizeof(int));
    assert(!vec->size);
    assert(h_vector_reserve(vec, SIZE_MAX));
    assert(vec->capacity == n * sizeof(int));
    assert(!vec->size);

    h_vector_destroy(vec);
    return 0;
}

static int test_vector_resize()
{
    struct h_vector *vec = h_vector_new(sizeof(int));
    size_t n = 10;

    assert(!h_vector_resize(vec, n));

    assert(vec->size == n);
    assert(vec->capacity >= n);

    for (int i = 0; i < n; i++) {
        assert(*(int *)(vec->data + i * sizeof(int)) == 0);
    }

    assert(h_vector_resize(vec, SIZE_MAX));
    assert(vec->size == n);
    assert(vec->capacity >= n);

    size_t n_small = 2;

    assert(!h_vector_resize(vec, n_small));
    assert(vec->size == n_small);
    assert(vec->capacity >= n);

    h_vector_destroy(vec);
    return 0;
}

static int test_vector_shrink_to_fit()
{
    struct h_vector *vec = h_vector_new(sizeof(int));
    size_t size = 50;
    size_t cap = 100;

    assert(!h_vector_resize(vec, size));
    assert(!h_vector_reserve(vec, cap));

    assert(vec->size == size);
    assert(vec->capacity == cap);

    h_vector_shrink_to_fit(vec);

    assert(vec->size == size);
    assert(vec->capacity == cap);

    h_vector_destroy(vec);
    return 0;
}

static int test_vector_push_back()
{
    struct h_vector *vec = h_vector_new(sizeof(int));
    int elem_ints[] = { 4846, 74, 83, 42, 62, 36, 73, 7, 722905 };

    assert(!vec->size);
    assert(!vec->capacity);

    assert(!h_vector_push_back(vec, elem_ints + 0));
    assert(!h_vector_push_back(vec, elem_ints + 1));

    assert(vec->size == 2);
    assert(vec->capacity == 2);

    for (int i = 2; i < ARRAY_SIZE(elem_ints); i++) {
        assert(!h_vector_push_back(vec, elem_ints + i));
    }

    assert(vec->size == ARRAY_SIZE(elem_ints));
    assert(vec->capacity >= 15);
    assert(memcmp(elem_ints, vec->data, sizeof elem_ints));

    h_vector_destroy(vec);

    vec = h_vector_new(SIZE_MAX);
    assert(h_vector_push_back(vec, NULL));
    h_vector_destroy(vec);

    return 0;
}

static int test_vector_pop_back()
{
    struct h_vector *vec = h_vector_new(sizeof(int));
    int n = 10;

    assert(!h_vector_resize(vec, n));
    h_vector_pop_back(vec);
    assert(vec->size == n - 1);
    assert(vec->capacity == n);

    return 0;
}

static int test_vector_clear()
{
    struct h_vector *vec = h_vector_new(sizeof(int));

    assert(!h_vector_resize(vec, 10));
    h_vector_clear(vec);
    assert(vec->capacity == 10);
    assert(vec->size == 0);

    return 0;
}

static int test_vector_front()
{
    struct h_vector *vec = h_vector_new(sizeof(int));

    assert(!h_vector_resize(vec, 10));
    assert(h_vector_front(vec) == vec->data);

    return 0;
}

static int test_vector_back()
{
    struct h_vector *vec = h_vector_new(sizeof(int));

    assert(!h_vector_resize(vec, 10));
    assert(!h_vector_reserve(vec, 15));
    assert(h_vector_back(vec) == vec->data + 10 * sizeof(int));

    return 0;
}

static int test_vector_at()
{
    struct h_vector *vec = h_vector_new(sizeof(int));
    size_t n = 10;

    h_vector_resize(vec, n);
    for (int i = 0; i < n; i++) {
        assert(h_vector_at(vec, i) == vec->data + i * sizeof(int));
    }

    h_vector_destroy(vec);
    return 0;
}

static int int_cmp(const void *a, const void *b)
{
    const int *_a = a;
    const int *_b = b;
    return *_a < *_b ? -1 : *_a > *_b ? 1 : 0;
}

static int test_vector_qsort()
{
    struct h_vector *vec = h_vector_new(sizeof(int));
    int elem_ints[] = { 4846, 74, 83, 42, 62, 36, 73, 7, 722905 };
    int elem_ints_sorted[] = { 7, 36, 42, 62, 73, 74, 83, 4846, 722905 };

    for (int i = 0; i < ARRAY_SIZE(elem_ints); i++) {
        assert(!h_vector_push_back(vec, elem_ints + i));
    }

    assert(!memcmp(elem_ints, vec->data, sizeof elem_ints));
    h_vector_qsort(vec, int_cmp);
    assert(!memcmp(elem_ints_sorted, vec->data, sizeof elem_ints_sorted));

    h_vector_destroy(vec);
    return 0;
}

static int test_vector_insert()
{
    struct h_vector *vec = h_vector_new(sizeof(int));
    int elem_ints[] = { 4846, 74, 83, 61, 63 };
    int front_ins[] = { 80, 1, 2 };
    int back_ins[] = { 0, 0, 5 };
    int middle_ins[] = { 10, 1 };
    int middle_pos = 5;
    int final[] = { 80, 1, 2, 4846, 74, 10, 1, 83, 61, 63, 0, 0, 5 };

    assert(!h_vector_insert(vec, 0, ARRAY_SIZE(elem_ints), elem_ints));
    assert(
        !h_vector_insert(vec, vec->size - 1, ARRAY_SIZE(back_ins), back_ins));
    assert(
        !h_vector_insert(vec, middle_pos, ARRAY_SIZE(middle_ins), middle_ins));
    assert(!h_vector_insert(vec, 0, ARRAY_SIZE(front_ins), front_ins));
    assert(!h_vector_insert(vec, 0, 0, NULL));
    assert(!h_vector_insert(vec, vec->size - 1, 0, NULL));
    assert(!memcmp(vec, final, sizeof final));
    assert(vec->size == ARRAY_SIZE(final));
    assert(vec->capacity >= vec->size);
    assert(!h_vector_insert(vec, 100, ARRAY_SIZE(final), final));

    for (int i = ARRAY_SIZE(final); i < 100; i++) {
        assert(!((int *)vec->data)[i]);
    }

    assert(!memcmp(vec->data, final, sizeof(final)));
    assert(!memcmp(vec->data + 100, final, sizeof(final)));

    h_vector_destroy(vec);
    return 0;
}

static int test_vector_erase()
{
    struct h_vector *vec = h_vector_new(sizeof(int));
    int elems[] = { 80, 1, 2, 4846, 74, 10, 1, 83, 61, 63, 0, 0, 5 };
    int final[] = { 74, 10, 83 };

    for (int i = 0; i < ARRAY_SIZE(elems); i++) {
        h_vector_push_back(vec, elems + i);
    }

    h_vector_erase(vec, 0, 4);
    h_vector_erase(vec, 2, 1);
    assert(vec->size == ARRAY_SIZE(elems) - 5);
    h_vector_erase(vec, 3, 100);
    assert(vec->size == 3);
    assert(vec->capacity >= ARRAY_SIZE(elems));
    assert(!memcmp(vec->data, final, sizeof final));

    return 0;
}

int test_vector()
{
    assert(!test_vector_new());
    assert(!test_vector_destroy());
    assert(!test_vector_reserve());
    assert(!test_vector_resize());
    assert(!test_vector_shrink_to_fit());
    assert(!test_vector_push_back());
    assert(!test_vector_pop_back());
    assert(!test_vector_clear());
    assert(!test_vector_front());
    assert(!test_vector_back());
    assert(!test_vector_at());
    assert(!test_vector_qsort());
    assert(!test_vector_insert());
    assert(!test_vector_erase());
    return 0;
}
