/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 *
 * Allocator abstraction.
 */
#ifndef HLIBC_ALLOCATOR_H_
#define HLIBC_ALLOCATOR_H_

#include <hlibc/def.h>

struct allocator_ops;

struct allocator {
    const struct allocator_ops *ops;
};

struct allocator_ops {
    void *(*alloc)(struct allocator *, size_t);
    void (*free)(struct allocator *, void *);
};

extern const struct allocator *default_allocator;

static inline void *allocator_alloc(struct allocator *a, size_t size)
{
    return a->ops->allocate(a, size);
}

static inline void allocator_free(struct allocator *a, void *ptr)
{
    a->ops->deallocate(a, ptr);
}

#endif
