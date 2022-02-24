/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#ifndef HLIBC_MEMORY_H_
#define HLIBC_MEMORY_H_

#include <hlibc/def.h>

static inline void *align(void *ptr, size_t alignment)
{
    return ((uintptr_t)ptr + alignment - 1) & ~(alignment - 1);
}

#endif
