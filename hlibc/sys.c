/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <unistd.h>
#include <hlibc/sys.h>

size_t page_size()
{
    static size_t ret;
    return ret ? ret : (ret = sysconf(_SC_PAGESIZE));
}

size_t core_count()
{
    static size_t ret;
    return ret ? ret : (ret = sysconf(_SC_NPROCESSORS_ONLN));
}

void *pageof(const void *addr)
{
    return (void *)((uintptr_t)addr & ~(page_size() - 1));
}
