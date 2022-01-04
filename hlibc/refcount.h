/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 *
 * Reference counting utility. Intended to be abstract for continued extension,
 * namely further guarentees of thread-safety.
 */

#ifndef HLIBC_REFCOUNT_H_
#define HLIBC_REFCOUNT_H_

#include <hlibc/def.h>

/*
 * Represents an opaque, thread-safe reference counter.
 *
 * Static initialization can occur with `REFCOUNT_INIT`, and dynamic
 * initialization may occur simply with `refcount_set`.
 */
typedef struct {
    atomic_size_t count;
} refcount_t;

/*
 * Static initializer for `refcount_t`. Begins refrences at `n`.
 */
#define REFCOUNT_INIT(n) ((refcount_t){ .count = (n) })

/*
 * Atomically subtract `n` from the value of `rc`. Returns `true` if there are
 * no references after the operation.
 */
static inline bool refcount_sub(refcount_t *rc, size_t n)
{
    return atomic_fetch_sub(&rc->count, n) == n;
}

/*
 * Atomically decrement `rc`. Returns `true` when `rc` has no references after
 * the decrement.
 */
static inline bool refcount_dec(refcount_t *rc)
{
    return refcount_sub(rc, 1);
}

/*
 * Atomically add `n` references to `rc`.
 */
static inline void refcount_add(refcount_t *rc, size_t n)
{
    atomic_fetch_add(&rc->count, n);
}

/*
 * Atomically increment `rc`.
 */
static inline void refcount_inc(refcount_t *rc)
{
    refcount_add(rc, 1);
}

/*
 * Atomically set `rc` to `n`.
 */
static inline void refcount_set(refcount_t *rc, size_t n)
{
    atomic_store(&rc->count, n);
}

/*
 * Atomically read and return the reference count on `rc`.
 */
static inline size_t refcount_read(refcount_t *rc)
{
    return atomic_load(rc);
}

#endif
