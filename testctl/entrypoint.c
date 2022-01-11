/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <testctl/internal.h>

#if __has_attribute(weak)

__attribute__((weak))
int main()
{
    testctl_main();
}

#endif

