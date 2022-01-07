/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 *
 * Wrappers around BSD native `kqueue` interface.
 */

#ifndef HLIBC_KQUEUE_H_
#define HLIBC_KQUEUE_H_

#include <sys/event.h>
#include <hlibc/def.h>

/*
 * kqueue (2) documentation:
 *
 * FreeBSD: https://www.freebsd.org/cgi/man.cgi?query=kevent&apropos=0&sektion=
 * 0&manpath=FreeBSD+9.0-RELEASE&arch=default&format=html
 *
 * MacOS: https://developer.apple.com/library/archive/documentation/System/Conc
 * eptual/ManPages_iPhoneOS/man2/kqueue.2.html
 */

/*
 * `EV_SET` wrapper for clarity and type safety.
 */
void kevent_set(struct kevent *event, uintptr_t ident, int16_t filter,
                uint16_t flags, uint32_t fflags, intptr_t data, void *udata);
/*
 * Tracks and describes events and changes for `kqueue`, including arbitrary
 * 'list' allocations.
 */
struct kqueue_table {
    /*
     * `kqueue` psuedo file descriptor.
     */
    int kq;

    /*
     * Process ID associated with the `kqueue`. This will be the one that
     * spawned the descriptor. Use this field when using shared mem mapping.
     */
    pid_t pid;

    int nchanges;
    int nevents;
    struct kevent *changelist;
    struct kevent *eventlist;

    /*
     * Fields `changelist_cap` and `eventlist_cap` are struct internals for
     * tracking array allocations.
     */
    size_t changelist_cap;
    size_t eventlist_cap;
};

int kqueue_table_init(struct kqueue_table *kt);

/*
 * Adds to change list. Returns negative on failure. May reallocate changelist.
 */
int kqueue_table_change(struct kqueue_table *kt, uintptr_t ident,
                        int16_t filter, uint16_t flags, uint32_t fflags,
                        intptr_t data, void *udata);

/*
 * Passes data to `kevent()` system call. Specify `nevents` for the max number
 * of events to return at once. The list will be alloated inside of the
 * `kqueue_table` struct. Specify timeout, or `NULL` for infinite wait. Put
 * zero'd `timespec` to exact a poll.
 */
int kqueue_table_commit(struct kqueue_table *kt, int nevents,
                        struct timespec *timeout);

/*
 * TODO: Look into de-registering events on `kqueue_table` destruction.
 */
void kqueue_table_destroy(struct kqueue_table *kt);

#endif
