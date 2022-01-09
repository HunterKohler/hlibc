/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 *
 * General array for homogeneous elements.
 */

#ifndef HLIBC_ARRAY_H_
#define HLIBC_ARRAY_H_

#include <stddef.h>

struct array {
    size_t size;
    size_t capacity;
    size_t elem_size;
    void *data;
};

/*
 * Static initializer for elements of size `elem_size`.
 */
#define ARRAY_INIT(elem_size)   \
    ((struct array){            \
        .size = 0,              \
        .capacity = 0,          \
        .elem_size = elem_size, \
        .data = NULL,           \
    })

/*
 * Internal reallocation method. Sets the new buffer and capacity.
 */
static inline int __array_realloc(struct array *arr, size_t capacity)
{
    void *buf = realloc(arr->data, capacity * arr->elem_size);
    if (!buf)
        return ENOMEM;
    arr->data = buf;
    arr->capacity = capacity;
    return 0;
}

/*
 * Return pointer to `index` in the array buffer.
 */
static inline void *array_at(const struct array *arr, size_t index)
{
    return arr->data + arr->elem_size * index;
}

/*
 * Copies `n` elements from `vals` into the array `arr` at index `pos`. This
 * moves other elements up and reallocates if necessary.
 */
static inline int array_insert(struct array *arr, size_t pos, const void *vals,
                               size_t n)
{
    int err = __array_realloc(arr, arr->size + n);
    if (err)
        return err;

    memmove(array_at(arr, pos + n), array_at(arr, pos),
            arr->elem_size * (arr->size - pos));
    memmove(array_at(arr, pos), vals, n * arr->elem_size);

    return 0;
}

/*
 * Dynamic array initializer.
 */
static inline int array_init(struct array *arr, size_t elem_size)
{
    memset(arr, 0, sizeof(*arr));
    arr->elem_size = elem_size;
    return 0;
}

/*
 * Destroy associated resources. The array is not re-initialized.
 */
static inline void array_destroy(struct array *arr)
{
    free(arr->data);
}

/*
 * Clear elements in the array. Capacity is not reduced, see `array_fit()`.
 */
static inline void array_clear(struct array *arr)
{
    arr->size = 0;
}

/*
 * Reserve enough memory to fit `n` elements.
 */
static inline int array_reserve(struct array *arr, size_t n)
{
    return n > arr->capacity ? __array_realloc(arr, n) : 0;
}

/*
 * Copies element pointed to by `elem` into the `n` positions of array `arr`
 * at and after index `pos`. If `elem` is `NULL`, the positions are
 * zero-initialized. The behavior is undefined if `arr`'s size is less than
 * `pos + n`.
 */
static inline void array_fill(struct array *arr, size_t pos, size_t n,
                              void *elem)
{
    if (elem) {
        for (int i = 0; i < n; i++)
            memcpy(array_at(arr, pos + i), elem, arr->elem_size);
    } else {
        memset(array_at(arr, pos), 0, n * arr->elem_size);
    }
}

/*
 * Resizes array to size `n`. If `n` is greater than the current size,
 * `array_resize()` may allocate. Any additional elements will be a copy of
 * the element pointed to by `elem`, or zero-initialized if `elem` is `NULL`.
 */
static inline int array_resize(struct array *arr, size_t n, void *elem)
{
    if (n > arr->size) {
        int err = array_reserve(arr, n);
        if (err)
            return err;
        array_fill(arr, arr->size, n, elem);
    }

    arr->size = n;
}

/*
 * Removes `n` elements from array `arr` at index `pos`. Calling
 * `array_erase()` on an array with size less than `pos + n` is undefined.
 */
static inline void array_erase(struct array *arr, size_t pos, size_t n)
{
    memmove(array_at(arr, pos), array_at(arr, pos + n), arr->size - (pos + n));
    arr->size -= n;
}

/*
 * Returns pointer to first element of array `arr`. The result of calling
 * `array_first()` on an empty array is undefined.
 */
static inline void *array_first(const struct array *arr)
{
    return array_at(arr, 0);
}

/*
 * Returns pointer to last element of array `arr`. The result of calling
 * `array_last()` on an empty array is undefined.
 */
static inline void *array_last(const struct array *arr)
{
    return array_at(arr, arr->size - 1);
}

/*
 * Copies contents of array `src` to array `dest`. Any current contents of
 * `dest` are not destroyed, see `array_destroy()`.
 */
static inline int array_copy(struct array *restrict dest,
                             const struct array *restrict src)
{
    size_t size = src->capacity * src->elem_size;
    void *buf = malloc(size);

    if (!buf)
        return ENOMEM;

    memcpy(buf, src->data, size);
    memcpy(dest, src, sizeof(*src));

    dest->data = buf;
    return 0;
}

/*
 * Reduces array `arr`'s capacity to fit the current number of elements.
 */
static inline int array_fit(struct array *arr)
{
    return __array_realloc(arr, arr->size);
}

/*
 * Copies element pointed to by `elem` to the end of array `arr`, allocating if
 * neccessaary.
 */
static inline int array_push(struct array *arr, void *elem)
{
    return array_insert(arr, 0, elem, 1);
}

/*
 * Removes the last element of the array `arr`. Calling `array_pop()` on an
 * empty array is undefined.
 */
static inline void array_pop(struct array *arr)
{
    arr->size--;
}

#endif
