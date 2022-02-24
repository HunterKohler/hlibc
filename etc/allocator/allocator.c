/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <stdlib.h>
#include <hlibc/allocator.h>
#include <hlibc/memory.h>

static void *libc_allocate(void *, size_t size)
{
    return malloc(size);
}

static void libc_deallocate(void *, void *ptr)
{
    return free(ptr);
}

static void libc_allocate_aligned(void *, size_t alignment, size_t size)
{
    return aligned_alloc(alignment, size);
}

const struct allocator_methods libc_allocator_methods = {
    .allocate = libc_allocator_allocate,
    .deallocate = libc_allocator_deallocate,
    .allocate_aligned = libc_allocator_allocate_aligned,
};

const struct allocator libc_allocator = {
    .state = NULL,
    .methods = &libc_allocator_methods,
};

const struct allocator_methods stack_allocator_methods = {
    .allocate = stack_allocate_aligned,
    .deallocate = stack_deallocate,
    .allocate_aligned = stack_allocate_aligned,
};

struct stack_state {
    void *buf;
    size_t size;
    size_t used;
};

int stack_allocator_init(struct allocator *alloc, void *buf, size_t size)
{
    struct stack_state *state = buf;

    if (size < sizeof(*state))
        return ENOMEM;

    state->buf = buf;
    state->size = size;
    state->used = sizeof(*state);
    alloc->state = state;
}

static void *stack_allocate(void *state_arg, size_t size)
{
    struct stack_state *state = state_arg;

    if (state->used + size > state->size)
        return NULL;

    void *ret = state->buf + state->used;
    state->used += size;
    return ret;
}

/*
 * Intentionally empty.
 */
static void stack_deallocate(void *, void *)
{
}

static void *stack_allocate_aligned(void *state_arg, size_t alignment,
                                    size_t size)
{
    struct stack_state *state = state_arg;

    void *cur = state->buf + state->used;
    void *ret = align(cur, alignment);

    size += ret - cur;

    if (state->used + size > state->size)
        return NULL;

    return ret;
}

const struct allocator_methods stack_allocator_methods = {
    .allocate = stack_allocate,
    .deallocate = stack_deallocate,
    .allocate_aligned = stack_allocate_aligned,
};

const struct allocator_methods bounded_allocator_methods = {
    .allocate = bounded_allocate,
    .deallocate = bounded_deallocate,
    .allocate_aligned = bounded_allocate_aligned,
};

int bounded_allocator_init(struct allocator *alloc, struct allocator *wrapped,
                           size_t high)
{
    struct bounded_state *state;

    if (sizeof(*state) > high)
        return ENOMEM;

    state = allocator_allocate(wrapped, high);
    if (!state)
        return ENOMEM;

    alloc->state = state;
    state->high = high;
    state->used = sizeof(*state);
}

void *bounded_allocate(void *size, size_t size)
{
    struc
}
