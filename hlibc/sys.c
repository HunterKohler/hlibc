/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <unistd.h>
#include <hlibc/sys.h>

size_t core_count()
{
    return sysconf(_SC_NPROCESSORS_ONLN);
}
