/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <hlibc/heap.h>

#define PARENT(i) (((i)-1) >> 1)
#define LEFT(n) (((i) << 1) + 1)
#define RIGHT(n) (((i) << 1) + 2)

static void *heap_up(size_t target, void *base, size_t size,
                     bool (*less)(void *, void *))
{
    uint8_t *begin = base;
    size_t i;

    for (i = target; i > 0; i = PARENT(i)) {
        uint8_t *pos = begin + size * i;
        uint8_t *parent = begin + size * PARENT(i);

        if (!less(pos, parent))
            break;

        memswap(pos, parent, size);
    }

    return begin + size * i;
}

static void heap_down(size_t target, void *base, size_t n, size_t size,
                      bool (*less)(void *, void *))
{
    uint8_t *begin = base;
    uint8_t *end = base + n * size;

    /*
     * Not orderly, I must say.
     */
    for (size_t i = target; i < n;) {
        size_t prev = i;
        uint8_t *pos = begin + size * i;
        uint8_t *left = begin + size * LEFT(i);
        uint8_t *right = begin + size * RIGHT(i);

        if (right < end) {
            if (less(left, right)) {
                if (less(left, pos)) {
                    memswap(left, pos, size);
                    i = LEFT(i);
                }
            } else if (less(right, pos)) {
                memswap(right, pos, size);
                i = RIGHT(i);
            }
        } else if (left < end) {
            if (less(left, pos)) {
                memswap(left, pos, size);
                i = LEFT(i);
            }
        }

        if (prev == i)
            break;
    }
}

void heapify(void *base, size_t n, size_t size, bool (*less)(void *, void *))
{
    for (int i = 0; i < n; i++) {
        heap_up(i, base, size, less);
    }
}

void *heap_add(void *elem, void *base, size_t n, size_t size,
               bool (*less)(void *, void *))
{
    uint8_t *begin = base;
    memcpy(begin + n * size, elem, size);
    return heap_up(n, begin, size, less);
}

void heap_pop(void *copy, void *base, size_t n, size_t size,
              bool (*less)(void *, void *))
{
    uint8_t *begin = base;

    if (!n)
        return;

    if (copy)
        memcpy(copy, begin, size);

    if (!--n)
        return;

    memcpy(begin, begin + n * size, size);
    heap_down(0, base, n, size, less);
}

bool is_minheap(void *base, size_t n, size_t size, bool (*less)(void *, void *))
{
    uint8_t *begin = base;
    uint8_t *end = base + n * size;

    for (int i = 0; i < n; i++) {
        uint8_t *pos = begin + size * i;
        uint8_t *left = begin + size * LEFT(i);
        uint8_t *right = begin + size * RIGHT(i);

        if ((left < end && !less(pos, left)) ||
            (right < end && !less(pos, right)))
            return false;
    }

    return true;
}
