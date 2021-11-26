#ifndef HLIBC_ARRAY_H_
#define HLIBC_ARRAY_H_

#include <stddef.h>

struct hlib_array {
    size_t size;
    size_t capacity;
    size_t elem_size;
    void *data;
};

int hlib_array_init(struct hlib_array *arr, size_t elem_size);
void hlib_array_destroy(struct hlib_array *arr);
int hlib_array_reserve(struct hlib_array *arr, size_t size);
int hlib_array_push_back(struct hlib_array *arr, void *val);
int hlib_array_push_front(struct hlib_array *arr, void *val);
void *hlib_array_pop_front(struct hlib_array *arr);
void *hlib_array_pop_back(struct hlib_array *arr);
void *hlib_array_front(const struct hlib_array *arr);
void *hlib_array_back(const struct hlib_array *arr);
int hlib_array_copy(const struct hlib_array *restrict src,
                    struct hlib_array *restrict dest);
int hlib_array_shrink(struct hlib_array *arr);
void *hlib_array_at(const struct hlib_array *arr, size_t index);
int hlib_array_insert(struct hlib_array *restrict arr, size_t pos,
                      void *restrict vals, size_t n);
#endif
