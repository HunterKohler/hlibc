/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 *
 * Defines helpers for manipulating a minimum heap.
 */

#ifndef HLIBC_HEAP_H_
#define HLIBC_HEAP_H_

#include <hlibc/def.h>
#include <hlibc/string.h>

/*
 * Establish the minimum heap invariant on array `base` of size `n`, where
 * each element is `size` bytes.
 */
void heapify(void *base, size_t n, size_t size,
             bool (*less)(const void *, const void *));

/*
 * Add `elem` to the heap.
 *
 * Returns a pointer to the position of `elem`'s copy within the heap.
 */
void *heap_add(void *elem, void *base, size_t n, size_t size,
               bool (*less)(const void *, const void *));

/*
 * Remove top element of heap. Optionally pass `copy` as a non-null pointer
 * to copy the top elements value to.
 */
void heap_pop(void *copy, void *base, size_t n, size_t size,
              bool (*less)(const void *, const void *));

/*
 * Test if `base` is a well formed min-heap.
 */
bool is_minheap(void *base, size_t n, size_t size,
                bool (*less)(const void *, const void *));

#endif
