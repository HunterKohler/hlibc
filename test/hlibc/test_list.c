/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <testlib/testlib.h>

TEST(test_list_node_init)
{
    struct list_node node = LIST_NODE_INIT(node);

    ASSERT_PTR_EQ(&node, node.next);
    ASSERT_PTR_EQ(&node, node.prev);

    memset(&node, 0, sizeof(node));

    list_node_init(&node);

    ASSERT_PTR_EQ(&node, node.next);
    ASSERT_PTR_EQ(&node, node.prev);
}

TEST(test_list_add)
{
    struct list_node node = LIST_NODE_INIT(node);
    struct list_node first, second;

    list_add(&first, &node);
    list_add(&second, &node);

    ASSERT_PTR_EQ(node.next, &second);
    ASSERT_PTR_EQ(node.prev, &first);
    ASSERT_PTR_EQ(first.next, &node);
    ASSERT_PTR_EQ(first.prev, &second);
    ASSERT_PTR_EQ(second.next, &first);
    ASSERT_PTR_EQ(second.prev, &node);
}

TEST(test_list_add_tail)
{
    struct list_node node = LIST_NODE_INIT(node);
    struct list_node first, second;

    list_add_tail(&first, &node);
    list_add_tail(&second, &node);

    ASSERT_PTR_EQ(node.next, &first);
    ASSERT_PTR_EQ(node.prev, &second);
    ASSERT_PTR_EQ(first.next, &second);
    ASSERT_PTR_EQ(first.prev, &node);
    ASSERT_PTR_EQ(second.next, &node);
    ASSERT_PTR_EQ(second.prev, &first);
}

TEST(test_list_rm)
{
    struct list_node node = LIST_NODE_INIT(node);
    struct list_node first, second;

    list_add(&first, &node);
    list_add(&second, &node);
    list_rm(&first);

    ASSERT_PTR_EQ(second.next, &node);
    ASSERT_PTR_EQ(second.prev, &node);

    list_rm(&second);

    ASSERT_PTR_EQ(node.next, &node);
    ASSERT_PTR_EQ(node.prev, &node);
}

TEST(test_list_move)
{
    struct list_node src = LIST_NODE_INIT(src);
    struct list_node dest = LIST_NODE_INIT(dest);
    struct list_node first, second;

    list_add(&first, &src);
    list_add(&second, &dest);
    list_move(&first, &dest);

    ASSERT_PTR_EQ(src.next, &src);
    ASSERT_PTR_EQ(src.prev, &src);
    ASSERT_PTR_EQ(dest.next, &first);
    ASSERT_PTR_EQ(dest.prev, &second);
    ASSERT_PTR_EQ(first.next, &second);
    ASSERT_PTR_EQ(first.prev, &dest);
    ASSERT_PTR_EQ(second.next, &dest);
    ASSERT_PTR_EQ(second.prev, &first);
}

TEST(test_list_move_tail)
{
    struct list_node src = LIST_NODE_INIT(src);
    struct list_node dest = LIST_NODE_INIT(dest);
    struct list_node first, second;

    list_add(&first, &src);
    list_add(&second, &dest);
    list_move_tail(&first, &dest);

    ASSERT_PTR_EQ(src.next, &src);
    ASSERT_PTR_EQ(src.prev, &src);
    ASSERT_PTR_EQ(dest.next, &second);
    ASSERT_PTR_EQ(dest.prev, &first);
    ASSERT_PTR_EQ(first.next, &dest);
    ASSERT_PTR_EQ(first.prev, &second);
    ASSERT_PTR_EQ(second.next, &first);
    ASSERT_PTR_EQ(second.prev, &dest);
}

TEST(test_list_replace)
{
    struct list_node node = LIST_NODE_INIT(node);
    struct list_node first, second;

    list_add(&first, &node);
    list_replace(&first, &second);

    ASSERT_PTR_EQ(node.next, &second);
    ASSERT_PTR_EQ(node.prev, &second);
    ASSERT_PTR_EQ(second.next, &node);
    ASSERT_PTR_EQ(second.prev, &node);
}

TEST(test_list_swap)
{
    struct list_node x = LIST_NODE_INIT(x);
    struct list_node y = LIST_NODE_INIT(y);
    struct list_node first, second;

    list_add(&first, &x);
    list_add(&second, &y);
    list_swap(&first, &second);

    ASSERT_PTR_EQ(x.next, &second);
    ASSERT_PTR_EQ(x.prev, &second);
    ASSERT_PTR_EQ(second.next, &x);
    ASSERT_PTR_EQ(second.prev, &x);
    ASSERT_PTR_EQ(y.next, &first);
    ASSERT_PTR_EQ(y.prev, &first);
    ASSERT_PTR_EQ(first.next, &y);
    ASSERT_PTR_EQ(first.prev, &y);
}
