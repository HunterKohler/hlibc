/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 *
 * Allocator abstraction.
 */
#ifndef HLIBC_ALLOCATOR_H_
#define HLIBC_ALLOCATOR_H_

#include <hlibc/def.h>

struct allocator_methods {
    void *(*allocate)(void *, size_t);
    void (*deallocate)(void *, void *);
    void *(*allocate_aligned)(void *, size_t, size_t);
};

struct allocator {
    const struct allocator_methods *methods;
    void *state;
};

extern const struct allocator libc_allocator;

#ifndef DEFAULT_ALLOCATOR
#define DEFAULT_ALLOCATOR (&libc_allocator)
#endif

static inline void *allocator_allocate(struct allocator *alloc, size_t size)
{
    return alloc->methods->allocate(alloc->state, size);
}

static inline void allocator_deallocate(struct allocator *alloc, void *ptr)
{
    alloc->methods->deallocate(alloc->state, ptr);
}

static inline void *allocator_allocate_aligned(struct allocator *alloc,
                                               size_t alignment, size_t size)
{
    return alloc->methods->allocate_aligned(alloc->state, alignment, size);
}

int stack_allocator_init(struct allocator *alloc, void *buf, size_t size);
void *stack_allocator_destroy(struct allocator *alloc);

#endif
