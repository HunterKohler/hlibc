/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <string.h>
#include <hlibc/list.h>
#include <htest/htest.h>

struct container {
    char buf[128];
    struct list_node list;
};

void container_list_init(struct container *conts, size_t count)
{
    for (int i = 0; i < count; i++) {
        list_node_init(&conts[i].list);
    }

    for (int i = 1; i < count; i++) {
        list_add_tail(&conts[i].list, &conts[0].list);
    }
}

void test_list_node_init(struct htest *test)
{
    struct list_node node = LIST_NODE_INIT(node);

    HTEST_ASSERT_PTR_EQ(test, &node, node.next);
    HTEST_ASSERT_PTR_EQ(test, &node, node.prev);

    memset(&node, 0, sizeof(node));

    list_node_init(&node);

    HTEST_ASSERT_PTR_EQ(test, &node, node.next);
    HTEST_ASSERT_PTR_EQ(test, &node, node.prev);
}

void test_list_add(struct htest *test)
{
    struct list_node node = LIST_NODE_INIT(node);
    struct list_node first, second;

    list_add(&first, &node);
    list_add(&second, &node);

    HTEST_ASSERT_PTR_EQ(test, node.next, &second);
    HTEST_ASSERT_PTR_EQ(test, node.prev, &first);
    HTEST_ASSERT_PTR_EQ(test, first.next, &node);
    HTEST_ASSERT_PTR_EQ(test, first.prev, &second);
    HTEST_ASSERT_PTR_EQ(test, second.next, &first);
    HTEST_ASSERT_PTR_EQ(test, second.prev, &node);
}

void test_list_add_tail(struct htest *test)
{
    struct list_node node = LIST_NODE_INIT(node);
    struct list_node first, second;

    list_add_tail(&first, &node);
    list_add_tail(&second, &node);

    HTEST_ASSERT_PTR_EQ(test, node.next, &first);
    HTEST_ASSERT_PTR_EQ(test, node.prev, &second);
    HTEST_ASSERT_PTR_EQ(test, first.next, &second);
    HTEST_ASSERT_PTR_EQ(test, first.prev, &node);
    HTEST_ASSERT_PTR_EQ(test, second.next, &node);
    HTEST_ASSERT_PTR_EQ(test, second.prev, &first);
}

void test_list_rm(struct htest *test)
{
    struct list_node node = LIST_NODE_INIT(node);
    struct list_node first, second;

    list_add(&first, &node);
    list_add(&second, &node);
    list_rm(&first);

    HTEST_ASSERT_PTR_EQ(test, second.next, &node);
    HTEST_ASSERT_PTR_EQ(test, second.prev, &node);

    list_rm(&second);

    HTEST_ASSERT_PTR_EQ(test, node.next, &node);
    HTEST_ASSERT_PTR_EQ(test, node.prev, &node);
}

void test_list_move(struct htest *test)
{
    struct list_node src = LIST_NODE_INIT(src);
    struct list_node dest = LIST_NODE_INIT(dest);
    struct list_node first, second;

    list_add(&first, &src);
    list_add(&second, &dest);
    list_move(&first, &dest);

    HTEST_ASSERT_PTR_EQ(test, src.next, &src);
    HTEST_ASSERT_PTR_EQ(test, src.prev, &src);
    HTEST_ASSERT_PTR_EQ(test, dest.next, &first);
    HTEST_ASSERT_PTR_EQ(test, dest.prev, &second);
    HTEST_ASSERT_PTR_EQ(test, first.next, &second);
    HTEST_ASSERT_PTR_EQ(test, first.prev, &dest);
    HTEST_ASSERT_PTR_EQ(test, second.next, &dest);
    HTEST_ASSERT_PTR_EQ(test, second.prev, &first);
}

void test_list_move_tail(struct htest *test)
{
    struct list_node src = LIST_NODE_INIT(src);
    struct list_node dest = LIST_NODE_INIT(dest);
    struct list_node first, second;

    list_add(&first, &src);
    list_add(&second, &dest);
    list_move_tail(&first, &dest);

    HTEST_ASSERT_PTR_EQ(test, src.next, &src);
    HTEST_ASSERT_PTR_EQ(test, src.prev, &src);
    HTEST_ASSERT_PTR_EQ(test, dest.next, &second);
    HTEST_ASSERT_PTR_EQ(test, dest.prev, &first);
    HTEST_ASSERT_PTR_EQ(test, first.next, &dest);
    HTEST_ASSERT_PTR_EQ(test, first.prev, &second);
    HTEST_ASSERT_PTR_EQ(test, second.next, &first);
    HTEST_ASSERT_PTR_EQ(test, second.prev, &dest);
}

void test_list_replace(struct htest *test)
{
    struct list_node node = LIST_NODE_INIT(node);
    struct list_node first, second;

    list_add(&first, &node);
    list_replace(&first, &second);

    HTEST_ASSERT_PTR_EQ(test, node.next, &second);
    HTEST_ASSERT_PTR_EQ(test, node.prev, &second);
    HTEST_ASSERT_PTR_EQ(test, second.next, &node);
    HTEST_ASSERT_PTR_EQ(test, second.prev, &node);
}

void test_list_swap(struct htest *test)
{
    struct list_node x = LIST_NODE_INIT(x);
    struct list_node y = LIST_NODE_INIT(y);
    struct list_node first, second;

    list_add(&first, &x);
    list_add(&second, &y);
    list_swap(&first, &second);
    list_swap(&first, &first);

    HTEST_ASSERT_PTR_EQ(test, x.next, &second);
    HTEST_ASSERT_PTR_EQ(test, x.prev, &second);
    HTEST_ASSERT_PTR_EQ(test, second.next, &x);
    HTEST_ASSERT_PTR_EQ(test, second.prev, &x);
    HTEST_ASSERT_PTR_EQ(test, y.next, &first);
    HTEST_ASSERT_PTR_EQ(test, y.prev, &first);
    HTEST_ASSERT_PTR_EQ(test, first.next, &y);
    HTEST_ASSERT_PTR_EQ(test, first.prev, &y);
}

void test_list_entry(struct htest *test)
{
    struct container *res;
    struct container conts[1];
    container_list_init(conts, array_size(conts));

    res = list_entry(&conts[0].list, struct container, list);
    HTEST_ASSERT_PTR_EQ(test, res, &conts[0]);
}

void test_list_next_entry(struct htest *test)
{
    struct container conts[2];

    container_list_init(conts, array_size(conts));

    HTEST_ASSERT_PTR_EQ(test, list_next_entry(conts, list), &conts[1]);
    HTEST_ASSERT_PTR_EQ(test, list_next_entry(conts + 1, list), &conts[0]);
}

void test_list_prev_entry(struct htest *test)
{
    struct container conts[2];
    container_list_init(conts, array_size(conts));
    HTEST_ASSERT_PTR_EQ(test, list_prev_entry(conts, list), &conts[1]);
    HTEST_ASSERT_PTR_EQ(test, list_prev_entry(conts + 1, list), &conts[0]);
}

void test_list_first_entry(struct htest *test)
{
    struct container conts[2];
    container_list_init(conts, array_size(conts));
    HTEST_ASSERT_PTR_EQ(
        test, list_first_entry(&conts[0].list, struct container, list),
        &conts[1]);
}

void test_list_last_entry(struct htest *test)
{
    struct container conts[2];
    container_list_init(conts, array_size(conts));
    HTEST_ASSERT_PTR_EQ(test,
                        list_last_entry(&conts[0].list, struct container, list),
                        &conts[1]);
}

void test_list_for_each(struct htest *test)
{
    struct list_node *it;
    struct container conts[15];
    container_list_init(conts, array_size(conts));

    int i = 1;
    list_for_each (it, &conts[0].list) {
        HTEST_ASSERT_PTR_EQ(test, it, &conts[i++].list);
    }

    HTEST_ASSERT_PTR_EQ(test, it, &conts[0].list);
}

void test_list_for_each_entry(struct htest *test)
{
    struct container *it;
    struct container conts[15];
    container_list_init(conts, array_size(conts));

    int i = 1;
    list_for_each_entry (it, &conts[0].list, list) {
        HTEST_ASSERT_PTR_EQ(test, it, &conts[i++]);
    }

    HTEST_ASSERT_PTR_EQ(test, it, &conts[0]);
}

struct htest_unit list_test_units[] = {
    HTEST_UNIT(test_list_node_init),      HTEST_UNIT(test_list_add),
    HTEST_UNIT(test_list_add_tail),       HTEST_UNIT(test_list_rm),
    HTEST_UNIT(test_list_move),           HTEST_UNIT(test_list_move_tail),
    HTEST_UNIT(test_list_replace),        HTEST_UNIT(test_list_swap),
    HTEST_UNIT(test_list_entry),          HTEST_UNIT(test_list_next_entry),
    HTEST_UNIT(test_list_prev_entry),     HTEST_UNIT(test_list_first_entry),
    HTEST_UNIT(test_list_last_entry),     HTEST_UNIT(test_list_for_each),
    HTEST_UNIT(test_list_for_each_entry), {},
};

struct htest_suite list_test_suite = {
    .name = "list test suite",
    .units = list_test_units,
};

HTEST_DECLARE_SUITES(&list_test_suite);
