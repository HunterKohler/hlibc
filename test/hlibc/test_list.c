/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <testctl/testctl.h>
#include <hlibc/list.h>

struct container {
    char buf[128];
    struct list_node list;
};

void container_list_init(struct container *conts, size_t count)
{
    for (int i = 0; i < count; i++)
        list_node_init(&conts[i].list);
    for (int i = 1; i < count; i++)
        list_add_tail(&conts[i].list, &conts[0].list);
}

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
    list_swap(&first, &first);

    ASSERT_PTR_EQ(x.next, &second);
    ASSERT_PTR_EQ(x.prev, &second);
    ASSERT_PTR_EQ(second.next, &x);
    ASSERT_PTR_EQ(second.prev, &x);
    ASSERT_PTR_EQ(y.next, &first);
    ASSERT_PTR_EQ(y.prev, &first);
    ASSERT_PTR_EQ(first.next, &y);
    ASSERT_PTR_EQ(first.prev, &y);
}

TEST(test_list_entry)
{
    struct container *res;
    struct container conts[1];
    container_list_init(conts, array_size(conts));

    res = list_entry(&conts[0].list, struct container, list);
    ASSERT_PTR_EQ(res, &conts[0]);
}

TEST(test_list_next_entry)
{
    struct container conts[2];
    container_list_init(conts, array_size(conts));
    ASSERT_PTR_EQ(list_next_entry(conts, list), &conts[1]);
    ASSERT_PTR_EQ(list_next_entry(conts + 1, list), &conts[0]);
}

TEST(test_list_prev_entry)
{
    struct container conts[2];
    container_list_init(conts, array_size(conts));
    ASSERT_PTR_EQ(list_prev_entry(conts, list), &conts[1]);
    ASSERT_PTR_EQ(list_prev_entry(conts + 1, list), &conts[0]);
}

TEST(test_list_first_entry)
{
    struct container conts[2];
    container_list_init(conts, array_size(conts));
    ASSERT_PTR_EQ(list_first_entry(&conts[0].list, struct container, list),
                  &conts[1]);
}

TEST(test_list_last_entry)
{
    struct container conts[2];
    container_list_init(conts, array_size(conts));
    ASSERT_PTR_EQ(list_last_entry(&conts[0].list, struct container, list),
                  &conts[1]);
}

TEST(test_list_for_each)
{
    struct list_node *it;
    struct container conts[15];
    container_list_init(conts, array_size(conts));

    int i = 1;
    list_for_each (it, &conts[0].list)
        ASSERT_PTR_EQ(it, &conts[i++].list);

    ASSERT_PTR_EQ(it, &conts[0].list);
}

TEST(test_list_for_each_entry)
{
    struct container *it;
    struct container conts[15];
    container_list_init(conts, array_size(conts));

    int i = 1;
    list_for_each_entry (it, &conts[0].list, list)
        ASSERT_PTR_EQ(it, &conts[i++]);

    ASSERT_PTR_EQ(it, &conts[0]);
}
