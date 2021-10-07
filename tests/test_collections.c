/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./collections.h"
#include "./term.h"

void *mock_free_calls[100];
static void mock_free(void *ptr)
{
    static size_t i = 0;
    assert(i < 100);
    mock_free_calls[i++] = ptr;
}

void test_stack()
{
    char *data[] = { strdup("0"), strdup("1"), strdup("2") };
    struct stack *st = stack_new();

    assert(st->size == 0);
    assert(st->head == NULL);

    assert(stack_push(st, data[0]) == 0);
    assert(stack_push(st, data[1]) == 0);
    assert(stack_push(st, data[2]) == 0);

    assert(st->size == 3);
    assert(strcmp(stack_peek(st), data[2]) == 0);
    assert(st->size == 3);

    assert(strcmp(stack_pop(st), data[2]) == 0);
    assert(strcmp(stack_pop(st), data[1]) == 0);
    assert(strcmp(stack_pop(st), data[0]) == 0);

    assert(st->size == 0);

    assert(stack_push(st, data[0]) == 0);
    assert(stack_push(st, data[1]) == 0);
    assert(stack_push(st, data[2]) == 0);

    stack_destroy(st, mock_free);

    assert(strcmp(data[0], mock_free_calls[2]) == 0);
    assert(strcmp(data[1], mock_free_calls[1]) == 0);
    assert(strcmp(data[2], mock_free_calls[0]) == 0);

    free(data[0]);
    free(data[1]);
    free(data[2]);
}

void test_rb_tree()
{
    struct rb_tree *tree = rb_tree_new(NULL);

    assert(tree->cmp == basic_cmp);
    assert(tree->root == NULL);
    assert(tree->size == 0);
    assert(tree->dynamic == true);
}

void test_h_vector()
{
}

void success(const char *label)
{
    printf("%s%s%s  success  %s %s%s%s\n",
           ANSI_FG_GREEN,
           ANSI_REVERSE,
           ANSI_BOLD,
           ANSI_RESET,
           ANSI_BOLD,
           label,
           ANSI_RESET);
}

#define run_test(fn) run_test(#fn, fn)
void(run_test)(const char *label, void (*fn)())
{
    fn();
    success(label);
}

int main()
{
    run_test(test_stack);
    run_test(test_rb_tree);
    run_test(test_h_vector);
}
