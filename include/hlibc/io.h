/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#ifndef HLIBC_IO_H_
#define HLIBC_IO_H_

#include <stdio.h>
#include <hlibc/def.h>

/*
 * Get size of file stream
 *
 * Return -1 on error.
 */
long fgetsize(FILE *stream);

#endif
