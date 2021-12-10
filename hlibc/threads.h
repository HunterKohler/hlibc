/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIBC_THREADS_H_
#define HLIBC_THREADS_H_

#include <stdatomic.h>
#include <time.h>
#include <sched.h>
#include <pthread.h>
#include <hlibc/def.h>

/*
 * Sleeps the full amount of time specified as a summed duration, while
 * ignoring any signal interupts.
 */
int nanosleep_full(long nsec, time_t sec);

/*
 * Standard spinlock. Requires c11 atomics.
 */
struct spinlock {
    atomic_flag flag;
};

/*
 * Static `struct spinlock` initializer.
 */
#define SPINLOCK_INIT ((struct spinlock){ .flag = ATOMIC_FLAG_INIT })

int spinlock_lock(struct spinlock *lock);
int spinlock_trylock(struct spinlock *lock);
int spinlock_unlock(struct spinlock *lock);

/*
 * Dynamic `struct spinlock` initializer.
 */
int spinlock_init(struct spinlock *lock);

/*
 * Dynamic `struct spinlock` destructor.
 */
int spinlock_destroy(struct spinlock *lock);

#endif
