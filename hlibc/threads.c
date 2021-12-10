/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <hlibc/threads.h>

int nanosleep_full(long nsec, time_t sec)
{
    int ret = 0;
    struct timespec spec = { .tv_nsec = nsec, .tv_sec = sec };
    do {
        ret = nanosleep(&spec, &spec);
    } while (ret && errno == EINTR);
    return ret;
}

int spinlock_lock(struct spinlock *lock)
{
    while (true) {
        for (int i = 0; i < 100000; i++) {
            if (atomic_flag_test_and_set(&lock->flag)) {
                return 0;
            }
        }
        sched_yield();
    }
}

int spinlock_trylock(struct spinlock *lock)
{
    if (atomic_flag_test_and_set(&lock->flag))
        return 0;
    else
        return EBUSY;
}

int spinlock_unlock(struct spinlock *lock)
{
    atomic_flag_clear(&lock->flag);
    return 0;
}

int spinlock_init(struct spinlock *lock)
{
    atomic_flag_clear(&lock->flag);
    return 0;
}

int spinlock_destroy(struct spinlock *lock)
{
    atomic_flag_clear(&lock->flag);
    return 0;
}
