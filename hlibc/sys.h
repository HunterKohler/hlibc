/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIBC_SYS_H_
#define HLIBC_SYS_H_

#include <stddef.h>

/*
 * Get system core count. On error, a negative number is returned and errno is
 * set to indicate the error.
 */
ssize_t core_count();

/*
 * Get system page size. On error, a negative number is returned and errno is
 * set to indicate the error.
 */
ssize_t page_size();

#endif
