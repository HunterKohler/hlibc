#include <hlibc/kqueue.h>

void kevent_set(struct kevent *event, uintptr_t ident, int16_t filter,
                uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
    EV_SET(event, ident, filter, flags, fflags, data, udata);
}

int kqueue_table_init(struct kqueue_table *kt)
{
    int kq = kqueue();
    if (kq < 0)
        return kq;

    kt->kq = kq;
    kt->pid = getpid();
    kt->nchanges = 0;
    kt->nevents = 0;
    kt->changelist = NULL;
    kt->eventlist = NULL;
    kt->changelist_cap = 0;
    kt->changelist_cap = 0;

    return 0;
}

int kqueue_table_change(struct kqueue_table *kt, uintptr_t ident,
                        int16_t filter, uint16_t flags, uint32_t fflags,
                        intptr_t data, void *udata)
{
    if (kt->changelist_cap == kt->nchanges) {
        kt->changelist_cap = max(kt->changelist_cap * 3 / 2, 2);
        kt->changelist = realloc(kt->changelist,
                                 kt->changelist_cap * sizeof(*kt->changelist));
        if (!kt->changelist)
            return ENOMEM;
    }

    kevent_set(kt->changelist + kt->nchanges++, ident, filter, flags, fflags,
               data, udata);
    return 0;
}

int kqueue_table_commit(struct kqueue_table *kt, int nevents,
                        struct timespec *timeout)
{
    kt->nevents = kevent(kt->kq, kt->changelist, kt->nchanges, kt->eventlist,
                         nevents, timeout);
    return kt->nevents;
}

void kqueue_table_destroy(struct kqueue_table *kt)
{
    free(kt->changelist);
    free(kt->eventlist);
}
