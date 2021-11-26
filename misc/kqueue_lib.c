#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
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

typedef void *array_t;

struct kevent_table {
    size_t nchanges;
    size_t nevents;
    size_t eventlist_size;
    size_t changelist_size;
    struct kevent *changelist;
    struct kevent *eventlist;
    struct timespec timeout;
};

// int kq, const struct kevent *changelist, int nchanges, struct kevent *eventlist,
//     int nevents,
//     const struct timespec *timeout

int kevent_table(int kq, int nevents, struct kevent_table *kt)
{
    return kevent(kt->kq, kt->changelist, kt->nchanges, kt->eventlist,
                  kt->nevents, kt->timeout);
}

void kevent_set(struct kevent *event, uintptr_t ident, int16_t filter,
                uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
    EV_SET(event, ident, filter, flags, fflags, data, udata);
}

int kevent_table_init(struct kevent_table *kt)
{
    memset(table, 0, sizeof(*table));
    return 0;
}

void kevent_table_destroy(struct kevent_table *kt)
{
    free(kt->eventlist);
    free(kt->changelist);
}

int kevent_table_change(struct kevent_table *kt, uintptr_t ident,
                        int16_t filter, uint16_t flags, uint32_t fflags,
                        intptr_t data, void *udata)
{
    if (kt->changelist_size == kt->nchanges) {
        size_t size = kt->changelist_size < 2 ? 2 : kt->changelist_size * 3 / 2;
        void *mem = realloc(kt->changelist, size * sizeof(*kt->changelist));

        if (!mem)
            return ENOMEM;

        kt->changelist = mem;
        kt->changelist_size = size;
    }

    kevent_set(kt->changelist[kt->nchanges++], ident, filter, flags, fflags,
               data, udata);
    return 0;
}

int main()
{
}
