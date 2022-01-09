/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 *
 * Embedded Red-Black Tree (working)
 */

#ifndef HLIBC_RBTREE_H_
#define HLIBC_RBTREE_H_

#include <hlibc/def.h>

typedef bool rb_color_t;

#define RB_RED ((rb_color_t)0)
#define RB_BLACK ((rb_color_t)1)

struct rb_tree {
    struct rb_node *root;
};

struct rb_node {
    struct rb_node *left;
    struct rb_node *right;
    struct rb_node *parent;
    rb_color_t color;
};

#define RBTREE_INIT ((struct rbtree){ .root = NULL })

struct rb_node *rb_first(struct rb_tree *tree);
struct rb_node *rb_last(struct rb_tree *tree);
struct rb_node *rb_next(struct rb_node *node);
struct rb_node *rb_prev(struct rb_node *node);
struct rb_node *rb_sibling(struct rb_node *node);
void rb_rotate(struct rb_node *root, struct rb_node *pivot);
void rb_insert(struct rb_tree *tree, struct rb_node *node,
               bool (*less)(struct rb_node *, struct rb_node *));
void rb_remove(struct rb_tree *tree, struct rb_node *node);

#endif
