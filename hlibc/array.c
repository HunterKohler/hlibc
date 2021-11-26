#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "array.h"
#include "math.h"

static inline int _hlib_array_realloc(struct hlib_array *arr, size_t capacity)
{
    void *buf = realloc(arr->buf, capacity * arr->elem_size);
    if (!buf)
        return ENOMEM;
    arr->buf = buf;
    arr->capacity = capacity;
    return 0;
}

static inline int _hlib_array_grow_size(size_t size)
{
    return size < 2 ? 2 : size * 3 / 2;
}

static inline int _hlib_array_grow(struct hlib_array *arr)
{
    return _hlib_array_realloc(arr, _hlib_array_grow_size(arr->capacity));
}

static inline void *_hlib_array_at(struct hlib_array *arr, size_t index)
{
    return arr->buf + arr->elem_size * index;
}

static inline int _hlib_array_insert(struct hlib_array *restrict arr,
                                     size_t pos, void *restrict vals, size_t n)
{
    int err = _hlib_array_realloc(
        max(_hlib_array_grow_size(arr->size + nvals), arr->capacity + vals));

    if (err)
        return err;

    memmove(_hlib_array_at(arr, n + pos), _hlib_array_at(arr, pos),
            arr->elem_size * (arr->size - pos));
    memcpy(_hlib_array_at(arr, pos), vals, n * arr->elem_size);
}

int hlib_array_init(struct hlib_array *arr, size_t elem_size)
{
    arr->size = 0;
    arr->capacity = 0;
    arr->elem_size = elem_size;
    arr->buf = NULL;
}

void hlib_array_destroy(struct hlib_array *arr)
{
    free(arr->buf);
}

int hlib_array_reserve(struct hlib_array *arr, size_t size)
{
    if (size > arr->capacity) {
        void *buf = realloc(arr->buf, size * arr->elem_size);
        if (!buf)
            return ENOMEM;
        arr->buf = buf;
        arr->capacity = size;
    }
    return 0;
}

int hlib_array_push_back(struct hlib_array *arr, void *val)
{
    return _hlib_array_insert(arr, arr->size - 1, val, 1);
}

int hlib_array_push_front(struct hlib_array *arr, void *val)
{
    return _hlib_array_insert(arr, 0, val, 1);
}

void *hlib_array_pop_front(struct hlib_array *arr)
{
    void *elem = malloc(arr->elem_size);
    if (!elem) {
        errno = ENOMEM;
        return NULL;
    }

    memcpy(elem, _hlib_array_at(arr, 0), arr->elem_size);
    memmove(_hlib_array_at(arr, 0), _hlib_array_at(arr, 1),
            arr->elem_size * --arr->size);
    return elem;
}

void *hlib_array_pop_back(struct hlib_array *arr)
{
    void *elem = malloc(arr->elem_size);
    if (!elem) {
        errno = ENOMEM;
        return NULL;
    }
    memcpy(elem, _hlib_array_at(arr, --arr->size), arr->elem_size);
    return elem;
}

void *hlib_array_front(const struct hlib_array *arr)
{
    return _hlib_array_at(arr, 0);
}

void *hlib_array_back(const struct hlib_array *arr)
{
    return _hlib_array_at(arr, arr->size - 1);
}

int hlib_array_copy(const struct hlib_array *restrict src,
                    struct hlib_array *restrict dest)
{
    size_t buf_size = src->elem_size * src->size;
    void *buf = malloc(buf_size);

    if (!buf)
        return ENOMEM;

    memcpy(buf, src->buf, buf_size);
    dest->capacity = src->size;
    dest->size = src->size;
    dest->elem_size = src->elem_size;
    dest->buf = buf;
    return 0;
}

int hlib_array_shrink(struct hlib_array *arr)
{
    return arr->size < arr->capacity ? _hlib_array_realloc(arr, arr->size) : 0;
}

void *hlib_array_at(const struct hlib_array *arr, size_t index)
{
    return _hlib_array_at(arr, index);
}

int hlib_array_insert(struct hlib_array *restrict arr, size_t pos,
                      void *restrict vals, size_t n)
{
    return _hlib_array_insert(arr, pos, vals, n);
}
