/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <testctl/testctl.h>
#include <hlibc/reljmp.h>
#include <dlfcn.h>

int original_add(int a, int b)
{
    return a + b;
}

int replaced_add(int a, int b)
{
    return a + b + 1;
}

TEST(test_replace_func)
{
    FILE *tmp = testctl_mkstemp();

    ASSERT_EQ(original_add(1, 2), 3);
    ASSERT_EQ(replaced_add(1, 2), 4);

    int ret = replace_func(original_add, replaced_add);
    perror(NULL);
    ASSERT_EQ(ret, 0);

    ASSERT_EQ(original_add(1, 2), 4);
    ASSERT_EQ(replaced_add(1, 2), 4);

    testctl_mkstemp()
}
