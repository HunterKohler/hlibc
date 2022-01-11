/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#ifndef HLIBC_HTABLE_H_
#define HLIBC_HTABLE_H_

#include <hlibc/def.h>

struct hlist_node {
    struct hlist_node *next;
    struct hlist_node **pprev;
};

struct hlist_head {
    struct hlist_node *first;
};

#define HLIST_HEAD_INIT ((struct hlist_head){ .first = NULL })

#define HLIST_NODE_INIT ((struct hlist_node){ .pprev = NULL, .next = NULL })

static inline void hlist_head_init(struct hlist_head *head)
{
    head->first = NULL;
}

static inline void hlist_node_init(struct hlist_node *node)
{
    node->next = NULL;
    node->pprev = NULL;
}

static inline void hlist_add_head(struct hlist_node *item,
                                  struct hlist_head *head)
{
    if (head->first)
        head->first->pprev = &item->next;

    item->next = head->first;
    item->pprev = &head->first;
    head->first = item;
}

static inline void hlist_add_prev(struct hlist_node *item,
                                  struct hlist_node *node)
{
    item->pprev = node->pprev;
    item->next = node->next;
    node->pprev = &item->next;
    *item->pprev = item;
}

static inline void hlist_add_next(struct hlist_node *item,
                                  struct hlist_node *node)
{
    item->next = node->next;
    item->pprev = &node->next;

    if (node->next)
        node->next->pprev = &item->next;

    node->next = item;
}

static inline void hlist_del(struct hlist_node *node)
{
    *node->pprev = node->next;
    if (node->next)
        node->next->pprev = node->pprev;
}

static inline void hlist_rehead(struct hlist_head *old, struct hlist_head *head)
{
    head->first = old->first;
    if (head->first)
        head->first->pprev = &head->first;
    old->first = NULL;
}

#define __assert_hlist_node(x) \
    ({ static_assert(types_compatible((x), struct hlist_node)); })

#define hlist_entry(node, type, member)           \
    ({                                            \
        __assert_hlist_node(*(node));             \
        __assert_hlist_node(((type *)0)->member); \
        container_of(node, type, member);         \
    })

#define hlist_first_entry(head, type, member) \
    hlist_entry((head)->next, type, member)

#define hlist_last_entry(head, type, member) \
    hlist_entry((head)->prev, type, member)

#define hlist_next_entry(entry, member) \
    hlist_entry((entry)->member.next, typeof(*(entry)), member)

#define hlist_prev_entry(entry, member) \
    hlist_entry((entry)->member.prev, typeof(*(entry)), member)

#define hlist_for_each(pos, head)                                   \
    for (__assert_hlist_node(*(head)), __assert_hlist_node(*(pos)), \
         pos = (head)->next;                                        \
         pos != (head); pos = pos->next)

#define hlist_for_each_entry(pos, head, member)                 \
    for (pos = hlist_first_entry(head, typeof(*(pos)), member); \
         !hlist_is_head(&pos->member); pos = hlist_next_entry(pos))

#endif
