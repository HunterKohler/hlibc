/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIBC_VECTOR_H_
#define HLIBC_VECTOR_H_

#include <hlibc/def.h>
#include <hlibc/cmp.h>
#include <hlibc/math.h>

/*
 * Dynamic array.
 */
struct h_vector {
    /*
     * Number of contained elements.
     */
    size_t size;

    /*
     * Maximum number of elements which can fit in buffer.
     */
    size_t capacity;

    /*
     * Size of element in bytes.
     */
    size_t elem_size;

    /*
     * Underlying buffer.
     */
    void *data;
};

/*
 * Allocates new vector. Returns `NULL` if failed to allocate memory.
 */
struct h_vector *h_vector_new(size_t elem_size);

/*
 * Destroys vector.
 */
void h_vector_destroy(struct h_vector *vec);

/*
 * Reserves at least enough space to fit `n` elements. Returns non-zero int on
 * failure.
 */
int h_vector_reserve(struct h_vector *vec, size_t n);

/*
 * Resizes vector to `size`. Any new element positions will be zero-initialized.
 * Returns non-zero int on failure.
 */
int h_vector_resize(struct h_vector *vec, size_t size);

/*
 * Attempts to free excess capacity. Returns non-zero int on failure.
 */
int h_vector_shrink_to_fit(struct h_vector *vec);

/*
 * Pushes element to back of vector. May allocate. Returns non-zero int on
 * failure.
 */
int h_vector_push_back(struct h_vector *vec, const void *src);

/*
 * Removes last element.
 */
void h_vector_pop_back(struct h_vector *vec);

/*
 * Clears elements. Does not deallocate memory.
 */
void h_vector_clear(struct h_vector *vec);

/*
 * Returns pointer to first element.
 */
void *h_vector_front(struct h_vector *vec);

/*
 * Returns pointer to last element
 */
void *h_vector_back(struct h_vector *vec);

/*
 * Returns pointer to element at `pos`.
 */
void *h_vector_at(struct h_vector *vec, size_t pos);

/*
 * Sort vector according to given comparator.
 */
void h_vector_qsort(struct h_vector *vec, cmp_t cmp);

/*
 * Inserts `n` elements before a position in the array. Returns non-zero on
 * failure.
 */
int h_vector_insert(struct h_vector *vec, size_t pos, size_t n,
                    const void *src);

/*
 * Erases `n` elements starting at a position in the array. Will not
 * de-allocate.
 */
void h_vector_erase(struct h_vector *vec, size_t pos, size_t n);

#endif
