/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIBC_IO_H_
#define HLIBC_IO_H_

#include <stdio.h>
#include <sys/stat.h>

#include <hlibc/def.h>

/*
 * Returns filesize associated with `stream`, or `-1` on error.
 */
off_t filesize(FILE *stream);

/*
 * Read the contents of the file at `path` into a new buffer at `dest`. The
 * size of the file is returned (thus the size of the buffer `*dest`), or `-1`
 * if an error occurred.
 */
off_t readfile(const char *path, void **dest);

/*
 * Read the remaining contents of `stream` into a new buffer at `dest`. The
 * size of the file is returned (thus the size of the buffer `*dest`), or `-1`
 * if an error occurred.
 */
off_t freadfile(FILE *stream, void **dest);

#endif
