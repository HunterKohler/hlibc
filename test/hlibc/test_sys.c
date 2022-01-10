/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <testlib/testlib.h>
#include <hlibc/sys.h>
#include <hlibc/bit.h>

TEST(test_core_count)
{
    ASSERT_GTE(core_count(), 1);
    ASSERT_LTE(core_count(), 32);
}

TEST(test_page_size)
{
    ASSERT_GTE(page_size(), 1024);
    ASSERT_LTE(page_size(), 16384);
    ASSERT_EQ(popcount(page_size()), 1);
}
