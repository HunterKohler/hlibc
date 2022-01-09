/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIBC_SYS_H_
#define HLIBC_SYS_H_

#include <stddef.h>

size_t core_count();
size_t page_size();
void *pageof(const void *addr);


#endif
