/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#ifndef HLIBC_THREADS_H_
#define HLIBC_THREADS_H_

#include <stdatomic.h>
#include <time.h>
#include <sched.h>
#include <pthread.h>
#include <hlibc/def.h>

/*
 * Requires C11 atomics.
 *
 * "I repeat: do not use spinlocks in user space, unless you actually know what
 * you're doing. And be aware that the likelihood that you know what you are
 * doing is basically nil."
 *     - Linus Torvalds
 *
 * Anti-Reference:
 * https://www.realworldtech.com/forum/?threadid=189711&curpostid=189723
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
