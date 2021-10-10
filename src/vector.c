/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <stdlib.h>
#include <string.h>
#include <hlibc/vector.h>

struct h_vector *h_vector_new(size_t elem_size)
{
    struct h_vector *vec = calloc(1, sizeof *vec);
    if (vec) {
        vec->elem_size = elem_size;
    }
    return vec;
}

void h_vector_destroy(struct h_vector *vec)
{
    free(vec->data);
    free(vec);
}

int h_vector_reserve(struct h_vector *vec, size_t n)
{
    if (vec->capacity < n) {
        void *data = realloc(vec->data, n * vec->elem_size);
        if (data) {
            vec->capacity = n;
            vec->data = data;
        } else {
            return -1;
        }
    }
    return 0;
}

int h_vector_resize(struct h_vector *vec, size_t size)
{
    int err = h_vector_reserve(vec, size);
    if (err) {
        return err;
    } else if (vec->size < size) {
        memset(h_vector_at(vec, vec->size), 0,
               (size - vec->size) * vec->elem_size);
    }
    vec->size = size;
    return 0;
}

int h_vector_shrink_to_fit(struct h_vector *vec)
{
    void *data = realloc(vec->data, vec->size * vec->elem_size);
    if (data) {
        vec->capacity = vec->size;
        vec->data = data;
        return 0;
    } else {
        return -1;
    }
}

int h_vector_push_back(struct h_vector *vec, const void *src)
{
    if (vec->size == vec->capacity) {
        // 8/5 approx golden ratio
        int err = h_vector_reserve(
            vec, max(vec->capacity + 1, vec->capacity * 8 / 5));
        if (err) {
            return err;
        }
    }

    memcpy(h_vector_at(vec, vec->size++), src, vec->elem_size);
    return 0;
}

int h_vector_insert(struct h_vector *vec, size_t pos, size_t n, const void *src)
{
    int err = h_vector_resize(vec, max(vec->size, pos) + n);
    if (err) return err;

    memmove(h_vector_at(vec, pos + n), h_vector_at(vec, pos),
            (vec->size - pos - n) * vec->elem_size);
    memcpy(h_vector_at(vec, pos), src, n * vec->elem_size);

    return 0;
}

void h_vector_erase(struct h_vector *vec, size_t pos, size_t n)
{
    pos = min(pos, vec->size);
    n = min(vec->size - pos, n);
    memmove(h_vector_at(vec, pos), h_vector_at(vec, pos + n),
            (vec->size - pos - n) * vec->elem_size);
    vec->size -= n;
}

void *h_vector_at(struct h_vector *vec, size_t pos)
{
    return vec->data + pos * vec->elem_size;
}

void *h_vector_front(struct h_vector *vec)
{
    return vec->data;
}

void *h_vector_back(struct h_vector *vec)
{
    return h_vector_at(vec, vec->size - 1);
}

void h_vector_pop_back(struct h_vector *vec)
{
    vec->size--;
}

void h_vector_clear(struct h_vector *vec)
{
    vec->size = 0;
}

void h_vector_qsort(struct h_vector *vec, cmp_t cmp)
{
    qsort(vec->data, vec->size, vec->elem_size, cmp);
}
