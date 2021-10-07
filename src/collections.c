/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <hlibc/def.h>
#include <hlibc/collections.h>

int basic_cmp(const void *a, const void *b)
{
    return a < b ? -1 : a > b ? 1 : 0;
}

struct stack *stack_new()
{
    struct stack *st = calloc(1, sizeof *st);
    return st;
}

int stack_push(struct stack *st, void *value)
{
    struct stack_node *node = malloc(sizeof *node);
    if (node) {
        node->next = st->head;
        node->value = value;
        st->head = node;
        st->size++;
        return 0;
    } else {
        return 1;
    }
}

void *stack_pop(struct stack *st)
{
    struct stack_node *node = st->head;
    void *value = st->head->value;
    st->head = node->next;
    st->size--;
    free(node);
    return value;
}

void *stack_peek(struct stack *st)
{
    return st->head ? st->head->value : NULL;
}

void stack_destroy(struct stack *st, destroy_t destroy)
{
    struct stack_node *node = st->head;
    struct stack_node *next;
    while (node) {
        if (destroy) {
            destroy(node->value);
        }
        next = node->next;
        free(node);
        node = next;
    }
    free(st);
}

struct rb_tree *rb_tree_new(cmp_t cmp)
{
    struct rb_tree *tree = calloc(1, sizeof *tree);
    tree->cmp = cmp ? cmp : basic_cmp;
    tree->dynamic = true;
    return tree;
}

void rb_tree_destroy(struct rb_tree *tree, destroy_t destroy)
{
    struct rb_node *node = tree->root;
    while (node) {
        if (node->left) {
            node = node->left;
        } else if (node->right) {
            node = node->right;
        } else {
            struct rb_node *parent = node->parent;

            if (destroy) {
                destroy(node->value);
            }

            free(node);
            node = parent;
        }
    }
    if (tree->dynamic) {
        free(tree);
    }
}

// Primitive recursive implementation.
int _rb_tree_count(struct rb_node *node, void *value, cmp_t cmp)
{
    if (!node) {
        return 0;
    }

    int dir = cmp(node->value, value);
    if (dir < 0) {
        return _rb_tree_count(node->left, value, cmp);
    } else if (dir > 0) {
        return _rb_tree_count(node->right, value, cmp);
    } else {
        return 1 + _rb_tree_count(node->left, value, cmp) +
               _rb_tree_count(node->right, value, cmp);
    }
}

int rb_tree_count(struct rb_tree *tree, void *value)
{
    return _rb_tree_count(tree->root, value, tree->cmp);
}

struct h_vector *h_vector_new(size_t elem_size)
{
    struct h_vector *vec = calloc(1, sizeof *vec);
    if (vec) {
        vec->elem_size = elem_size;
        vec->dynamic = true;
    }
    return vec;
}

void h_vector_destroy(struct h_vector *vec, destroy_t destroy)
{
    if (destroy) {
        for (size_t i = 0; i < vec->size; i++) {
            destroy(vec->data[i]);
        }
    }

    free(vec->data);
    if (vec->dynamic) {
        free(vec);
    }
}

int h_vector_reserve(struct h_vector *vec, size_t n)
{
    if (vec->capacity < n) {
        void *data = vec->capacity ? realloc(vec->data, n * vec->elem_size)
                                   : malloc(n * vec->elem_size);
        if (data) {
            vec->data = data;
        } else {
            return -1;
        }
    }
    return 0;
}

int h_vector_resize(struct h_vector *vec, size_t size)
{
    int reserve_result = h_vector_reserve(vec, size);
    if (reserve_result) {
        return reserve_result;
    } else if (vec->size < size) {
        memset(vec->data + vec->size, 0, size - vec->size);
    }
    vec->size = size;
    return 0;
}

int h_vector_shrink_to_fit(struct h_vector *vec)
{
    void *ptr = realloc(vec->data, vec->capacity);
    if (ptr) {
        vec->capacity = vec->size;
        return 0;
    } else {
        return -1;
    }
}

int h_vector_push_back(struct h_vector *vec, void *value)
{
    if (vec->size == vec->capacity) {
        int err = h_vector_reserve(vec, vec->capacity * 3 / 2);
        if (err) {
            return err;
        }
    }

    *(vec->data + (vec->elem_size * vec->size++)) = value;
    return 0;
}

void *h_vector_pop_back(struct h_vector *vec)
{
    return vec->data[--vec->size];
}

void h_vector_clear(struct h_vector *vec)
{
    vec->capacity = 0;
    vec->size = 0;
}

void *h_vector_front(struct h_vector *vec)
{
    return vec->data[0];
}

void *h_vector_back(struct h_vector *vec)
{
    return vec->data[vec->size - 1];
}

void *h_vector_at(struct h_vector *vec, size_t pos)
{
    return vec->data[pos];
}

void h_vector_sort(struct h_vector *vec, cmp_t cmp)
{
    qsort(vec->data, vec->size, vec->elem_size, cmp);
}
