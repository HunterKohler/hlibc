/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef OPTS_H_
#define OPTS_H_

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * General comparator function-pointer type. Negative return, positive return,
 * and zero return values respectively imply the first argument to be less than,
 * greater than, or equal to the second argument.
 */
typedef int (*cmp_t)(const void *a, const void *b);

/*
 * General function-pointer type which should destroy the passed resource.
 * Example: `free()`
 */
typedef void (*destroy_t)(void *resource);

/*
 * Simplest `cmp_t` comparator.
 */
int basic_cmp(const void *a, const void *b);

/*
 * Stack node
 */
struct stack_node {
    /*
     * Child node in stack.
     */
    struct stack_node *next;

    /*
     * Contained value.
     */
    void *value;
};

/*
 * Stack
 */
struct stack {
    size_t size;
    struct stack_node *head;
};

/*
 * Allocates and initializes a stack.
 */
struct stack *stack_new();

/*
 * Adds item to a stack.
 */
int stack_push(struct stack *st, void *value);

/*
 * Removes and returns top item of stack. If the stack is empty, behavior is
 * undefined.
 */
void *stack_pop(struct stack *st);

/*
 * Returns value of the top item of stack without removal. If the stack is
 * empty, behavior is undefined.
 */
void *stack_peek(struct stack *st);

/*
 * Destroys stack. Optionally, pass a `destroy_t` to call on each element. By
 * default, elements are not freed.
 */
void stack_destroy(struct stack *st, destroy_t destroy);

/*
 * Red-black tree node.
 */
struct rb_node {
    /*
     * `true` if red, assumed black otherwise.
     */
    bool red;

    /*
     * Contained element.
     */
    void *value;

    /*
     * Parent node.
     */
    struct rb_node *parent;

    /*
     * Left child node.
     */
    struct rb_node *left;

    /*
     * Right child node.
     */
    struct rb_node *right;
};

/*
 * Red-black tree.
 */
struct rb_tree {
    /*
     * Comparator; determines element order.
     */
    cmp_t cmp;

    /*
     * Number of contained node.
     */
    size_t size;

    /*
     * Root node of tree.
     */
    struct rb_node *root;

    /*
     * `true` when allocated by internals, `false` otherwise.
     */
    bool dynamic;
};

/*
 * Allocated and initializes red-black tree.
 */
struct rb_tree *rb_tree_new(cmp_t cmp);

/*
 * Destroys red-black tree. Optionally, pass a `destroy_t` to call on each
 * element. By default, elements are not freed.
 */
void rb_tree_destroy(struct rb_tree *tree, destroy_t destroy);

/*
 * Counts all occurrence of `value` in the tree, according to the tree's
 * comparator.
 */
int rb_tree_count(struct rb_tree *tree, void *value);

// int rb_tree_insert(struct rb_tree *tree, void *value);

/*
 * Dynamic array.
 */
struct h_vector {
    /*
     * Number of contained elements.
     */
    size_t size;

    /*
     * Maximum number of elements which can fit in buffer.
     */
    size_t capacity;

    /*
     * Size of element in bytes.
     */
    size_t elem_size;

    /*
     * Underlying buffer.
     */
    void **data;

    /*
     * `true` when allocated by internals, false otherwise.
     */
    bool dynamic;
};

/*
 * Allocates new vector. Returns `NULL` if failed to allocate memory.
 */
struct h_vector *h_vector_new(size_t elem_size);

/*
 * Destroys vector. Optionally, pass a `destroy_t` to call on each element. By
 * default, elements are not freed.
 */
void h_vector_destroy(struct h_vector *vec, destroy_t destroy);

/*
 * Reserves at least enough space to fit `n` elements. Returns non-zero int on
 * failure.
 */
int h_vector_reserve(struct h_vector *vec, size_t n);

/*
 * Resizes vector to `size`. Any new element positions will be zero-initialized.
 * Returns non-zero int on failure.
 */
int h_vector_resize(struct h_vector *vec, size_t size);

/*
 * Attempts to free excess capacity. Returns non-zero int on failure.
 */
int h_vector_shrink_to_fit(struct h_vector *vec);

/*
 * Pushes element to back of vector. May allocate. Returns non-zero int on
 * failure.
 *
 * Grows at factor 1.5x.
 */
int h_vector_push_back(struct h_vector *vec, void *value);

/*
 * Remove and return last element of vector.
 */
void *h_vector_pop_back(struct h_vector *vec);

/*
 * Clears elements. Does not deallocate memory.
 */
void h_vector_clear(struct h_vector *vec);

/*
 * Returns first element.
 */
void *h_vector_front(struct h_vector *vec);

/*
 * Returns last element.
 */
void *h_vector_back(struct h_vector *vec);

/*
 * Access element at `pos`.
 */
void *h_vector_at(struct h_vector *vec, size_t pos);

/*
 * Sort vector according to given `cmp`.
 */
void h_vector_sort(struct h_vector *vec, cmp_t cmp);

#ifdef __cplusplus
}
#endif

#endif
