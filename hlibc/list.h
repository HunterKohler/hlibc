/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 *
 * Circular-doubly linked list. Follows struct-embedded pattern, with
 * homogeneous handles. Methods are all inlined due to size and theoretical
 * frequency of use.
 */

#ifndef HLIBC_LIST_H_
#define HLIBC_LIST_H_

#include <hlibc/def.h>

struct list_handle {
    struct list_handle *next;
    struct list_handle *prev;
};

/*
 * Static initializer for `struct list handle`.
 */
#define LIST_HANDLE_INIT(name) ({ &(name), &(name) })

static inline void __list_add(struct list_handle *item,
                              struct list_handle *prev,
                              struct list_handle *next)
{
    prev->next = item;
    next->prev = item;
    item->next = next;
    item->prev = prev;
}

static inline void __list_del(struct list_handle *prev,
                              struct list_handle *next)
{
    prev->next = next;
    next->prev = prev;
}

/*
 * Dynamic initializer for `struct list handle`.
 */
static inline void list_handle_init(struct list_handle *handle)
{
    handle->next = handle;
    handle->prev = handle;
}

static inline void list_add(struct list_handle *item,
                            struct list_handle *handle)
{
    __list_add(item, handle, handle->next);
}

static inline void list_add_tail(struct list_handle *item,
                                 struct list_handle *handle)
{
    __list_add(item, handle->prev, handle);
}

static inline void list_del(struct list_handle *handle)
{
    __list_del(handle->prev, handle->next);

    if (DEBUG) {
        handle->next = DEBUG_NULL;
        handle->prev = DEBUG_NULL;
    }
}

static inline void list_move(struct list_handle *item,
                             struct list_handle *handle)
{
    list_del(item);
    list_add(item, handle);
}

static inline void list_move_tail(struct list_handle *item,
                                  struct list_handle *handle)
{
    list_del(item);
    list_add_tail(handle);
}

static inline void list_replace(struct list_handle *old,
                                struct list_handle *item)
{
    item->next = old->next;
    item->prev = old->prev;
    item->prev->next = item;
    item->next->prev = item;

    if (DEBUG) {
        old->next = DEBUG_NULL;
        old->prev = DEBUG_NULL;
    }
}

static inline void list_swap(struct list_handle *a, struct list_handle *b)
{
    struct list_handle *tmp = a->prev;

    if (a != b) {
        list_del(a);
        list_replace(b, a);
        list_add(tmp, b);
    }
}

static inline bool list_is_head(struct list_handle *item,
                                struct list_handle *handle)
{
    return handle->prev == item;
}

static inline bool list_is_tail(struct list_handle *item,
                                struct list_handle *handle)
{
    return handle->next == item;
}

static inline bool list_empty(struct list_handle *handle)
{
    return handle->next == handle;
}

// clang-format off
#define list_entry(handle, type, member) container_of(handle, type, member);

#define list_first_entry(handle, type, member) \
    list_entry((handle)->next, type, member)

#define list_last_entry(handle, type, member) \
    list_entry((handle)->prev, type, member)

#define list_next_entry(entry, member) \
    list_entry((entry)->member->next, typeof(*(entry)), member)

#define list_prev_entry(entry, member) \
    list_entry((entry)->member->prev, typeof(*(entry)), member)

#define list_for_each_entry(it, handle, member)              \
    for (it = list_first_entry(handle, typeof(*it), member); \
         it->member != (handle);                             \
         it = list_next_entry(it, member))

#define list_for_each_entry_safe(it, tmp, head, member)      \
    for (it = list_first_entry(handle, typeof(*it), member), \
         tmp = list_next_entry(it, member);                  \
         it->member != (handle);                             \
         it = tmp, tmp = list_next_entry(tmp))
// clang-format on

#endif
