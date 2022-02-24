/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#ifndef HLIBC_SYS_H_
#define HLIBC_SYS_H_

#include <stddef.h>

/*
 * Get system's core count.
 */
size_t core_count();

/*
 * Get system's page size.
 */
size_t page_size();

/*
 * Get the starting address of the page containing `addr`.
 */
void *pageof(const void *addr);

#endif
