#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/event.h>

// struct kevent {
// 	uintptr_t       ident;  /* identifier for this event */
// 	int16_t         filter; /* filter for event */
// 	uint16_t        flags;  /* general flags */
// 	uint32_t        fflags; /* filter-specific flags */
// 	intptr_t        data;   /* filter-specific data */
// 	void            *udata; /* opaque user data identifier */
// };

//  EV_ADD         Adds the event to the kqueue.  Re-adding an existing event
//                     will modify the parameters of the original event, and not
//                     result in a duplicate entry.  Adding an event automati-cally automatically
//                     cally enables it, unless overridden by the EV_DISABLE
//                     flag.

// EV_ENABLE      Permit kevent() to return the event if it is triggered.

// EV_DISABLE     Disable the event so kevent() will not return it.  The
//             filter itself is not disabled.

// EV_DELETE      Removes the event from the kqueue.  Events which are
//             attached to file descriptors are automatically deleted on
//             the last close of the descriptor.

// EV_RECEIPT     This flag is useful for making bulk changes to a kqueue
//             without draining any pending events. When passed as input,
//             it forces EV_ERROR to always be returned.  When a filter
//             is successfully added. The data field will be zero.

// EV_ONESHOT     Causes the event to return only the first occurrence of
//             the filter being triggered.  After the user retrieves the
//             event from the kqueue, it is deleted.

// EV_CLEAR       After the event is retrieved by the user, its state is
//             reset.  This is useful for filters which report state
//             transitions instead of the current state.  Note that some
//             filters may automatically set this flag internally.

struct kqueue_table {
    int kq;
    pid_t pid;
    size_t nchanges;
    size_t nevents;
    size_t changelist_cap;
    size_t eventlist_cap;
    struct kevent *changelist;
    struct kevent *eventlist;
};

void kevent_set(struct kevent *event, uintptr_t ident, int16_t filter,
                uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
    EV_SET(event, ident, filter, flags, fflags, data, udata);
}

int kqueue_table_init(struct kqueue_table *kq, pid_t pid)
{
    kq->pid = pid <= 0 ? getpid() : pid;
    kq->eventlist = malloc(kq->eventlist_cap * sizeof(kq->eventlist));

    if (!kq->eventlist)
        return -1;

    kq->changelist = malloc(kq->changelist_cap * sizeof(kq->changelist));
    if (!(kq->changelist)) {
        free(kq->eventlist);
        return -1;
    }

    if ((kq->k = kqueue()) < 0) {
        free(kq->eventlist);
        free(kq->changelist);
        return -1;
    }

    return 0;
}

void kqueue_table_destroy(struct kqueue_table *kq)
{
    free(kq->eventlist);
    free(kq->changelist);
}

int kqueue_table_change(struct kqueue_table *kq, struct kevent *event,
                        uintptr_t ident, int16_t filter, uint16_t flags,
                        uint32_t fflags, intptr_t data, void *udata)
{
    if (kq->changelist_cap == kq->nchanges) {
        size_t cap = kq->changelist_cap < 2 ? 2 : kq->changelist_cap * 3 / 2;
        void *mem = realloc(cap * sizeof(*kq->changelist));

        if (!mem)
            return ENOMEM;

        kq->changelist_cap = cap;
        kq->changelist = mem;
    }

    kevent_set(kq->changelist[kq->nchanges++], ident, filter, flags, fflags,
               data, udata);
    return 0;
}

int kqueue_table_add(struct kqueue_table *kq, uintptr_t ident, int16_t filter,
                     uint32_t fflags, intptr_t data, void *udata)
{
    return kqueue_table_push(kq, ident, filter, EV_ADD, fflags, data, udata);
}

int kqueue_table_delete(struct kqueue_table *kq, uintptr_t ident,
                        int16_t filter)
{
    return kqueue_table_push(kq, ident, filter, EV_DELETE, 0, 0, NULL);
}

int kqueue_table_event_enable(struct kqueue_table *kq, uintptr_t ident,
                              int16_t filter)
{
    return kqueue_table_push(kq, ident, filter, EV_ENABLE, 0, 0, NULL);
}

int kqueue_table_disable(struct kqueue_table *kq, uintptr_t ident,
                         int16_t filter)
{
    return kqueue_table_push(kq, ident, filter, EV_DISABLE, 0, 0, NULL);
}

int kqueue_table_once(struct kqueue_table *kq, uintptr_t ident, int16_t filter,
                      uint32_t fflags, intptr_t data, void *udata)
{
    return kqueue_table_push(kq, ident, filter, EV_ONESHOT, fflags, data,
                             udata);
}

int main()
{
}
