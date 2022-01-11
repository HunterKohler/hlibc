/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 *
 * Circular-doubly linked list. Follows struct-embedded pattern, with
 * homogeneous nodes. Methods are all inlined due to size and theoretical
 * frequency of use.
 */

#ifndef HLIBC_LIST_H_
#define HLIBC_LIST_H_

#include <hlibc/def.h>

struct list_node {
    struct list_node *next;
    struct list_node *prev;
};

/*
 * Static initializer for `struct list_node`.
 */
#define LIST_NODE_INIT(name) \
    ((struct list_node){ .next = &(name), .prev = &(name) })

/*
 * Dynamic initializer for `struct list node`.
 */
static inline void list_node_init(struct list_node *node)
{
    node->next = node;
    node->prev = node;
}

static inline void __list_add(struct list_node *item, struct list_node *prev,
                              struct list_node *next)
{
    prev->next = item;
    next->prev = item;
    item->next = next;
    item->prev = prev;
}

static inline void __list_rm(struct list_node *prev, struct list_node *next)
{
    prev->next = next;
    next->prev = prev;
}

static inline void list_add(struct list_node *item, struct list_node *node)
{
    __list_add(item, node, node->next);
}

static inline void list_add_tail(struct list_node *item, struct list_node *node)
{
    __list_add(item, node->prev, node);
}

static inline void list_rm(struct list_node *node)
{
    __list_rm(node->prev, node->next);
}

static inline void list_move(struct list_node *item, struct list_node *node)
{
    list_rm(item);
    list_add(item, node);
}

static inline void list_move_tail(struct list_node *item,
                                  struct list_node *node)
{
    list_rm(item);
    list_add_tail(item, node);
}

static inline void list_replace(struct list_node *old, struct list_node *item)
{
    item->next = old->next;
    item->prev = old->prev;
    item->prev->next = item;
    item->next->prev = item;
}

static inline void list_swap(struct list_node *a, struct list_node *b)
{
    struct list_node *tmp = a->prev;

    if (a != b) {
        list_rm(a);
        list_replace(b, a);
        list_add(b, tmp);
    }
}

static inline bool list_is_first(struct list_node *item, struct list_node *node)
{
    return node->prev == item;
}

static inline bool list_is_last(struct list_node *item, struct list_node *node)
{
    return node->next == item;
}

static inline bool list_empty(struct list_node *node)
{
    return node->next == node;
}

#define __assert_list_node(x) \
    ({ static_assert(types_compatible((x), struct list_node)); })

#define list_entry(node, type, member)           \
    ({                                           \
        __assert_list_node(*(node));             \
        __assert_list_node(((type *)0)->member); \
        container_of(node, type, member);        \
    })

#define list_first_entry(head, type, member) \
    list_entry((head)->next, type, member)

#define list_last_entry(head, type, member) \
    list_entry((head)->prev, type, member)

#define list_next_entry(entry, member) \
    list_entry((entry)->member.next, typeof(*(entry)), member)

#define list_prev_entry(entry, member) \
    list_entry((entry)->member.prev, typeof(*(entry)), member)

#define list_for_each(pos, head)                                  \
    for (__assert_list_node(*(head)), __assert_list_node(*(pos)), \
         pos = (head)->next;                                      \
         pos != (head); pos = pos->next)

#define list_for_each_entry(pos, head, member)                 \
    for (pos = list_first_entry(head, typeof(*(pos)), member); \
         &pos->member != head; pos = list_next_entry(pos, member))

#define list_for_each_entry_safe(pos, tmp, head, member)       \
    for (pos = list_first_entry(head, typeof(*(pos)), member), \
        tmp = list_next_entry(pos, member);                    \
         &tmp->member != head; pos = tmp, tmp = list_next_entry(tmp, member))

#endif
