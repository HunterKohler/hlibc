/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
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
 * Static initializer for `struct list node`.
 */
#define LIST_INIT(name) ({ .next = &(name), .prev = &(name) })

static inline void __list_add(struct list_node *item, struct list_node *prev,
                              struct list_node *next)
{
    prev->next = item;
    next->prev = item;
    item->next = next;
    item->prev = prev;
}

static inline void __list_del(struct list_node *prev, struct list_node *next)
{
    prev->next = next;
    next->prev = prev;
}

/*
 * Dynamic initializer for `struct list node`.
 */
static inline void list_node_init(struct list_node *node)
{
    node->next = node;
    node->prev = node;
}

static inline void list_add(struct list_node *item, struct list_node *node)
{
    __list_add(item, node, node->next);
}

static inline void list_add_tail(struct list_node *item, struct list_node *node)
{
    __list_add(item, node->prev, node);
}

static inline void list_del(struct list_node *node)
{
    __list_del(node->prev, node->next);
}

static inline void list_move(struct list_node *item, struct list_node *node)
{
    list_del(item);
    list_add(item, node);
}

static inline void list_move_tail(struct list_node *item,
                                  struct list_node *node)
{
    list_del(item);
    list_add_tail(node);
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
        list_del(a);
        list_replace(b, a);
        list_add(tmp, b);
    }
}

static inline bool list_is_head(struct list_node *item, struct list_node *node)
{
    return node->prev == item;
}

static inline bool list_is_tail(struct list_node *item, struct list_node *node)
{
    return node->next == item;
}

static inline bool list_empty(struct list_node *node)
{
    return node->next == node;
}

#define list_entry(node, type, member) container_of(node, type, member);

#define list_first_entry(node, type, member) \
    list_entry((node)->next, type, member)

#define list_last_entry(node, type, member) \
    list_entry((node)->prev, type, member)

#define list_next_entry(entry, member) \
    list_entry((entry)->member->next, typeof(*(entry)), member)

#define list_prev_entry(entry, member) \
    list_entry((entry)->member->prev, typeof(*(entry)), member)

#define list_for_each_entry(it, node, member)              \
    for (it = list_first_entry(node, typeof(*it), member); \
         it->member != (node); it = list_next_entry(it, member))

#define list_for_each_entry_safe(it, tmp, head, member)    \
    for (it = list_first_entry(node, typeof(*it), member), \
        tmp = list_next_entry(it, member);                 \
         it->member != (node); it = tmp, tmp = list_next_entry(tmp))

#endif
