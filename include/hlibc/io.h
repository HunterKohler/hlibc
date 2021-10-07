/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIBC_IO_H_
#define HLIBC_IO_H_

#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Get optimal block size to read in file i/o.
 */
blksize_t io_blksize(struct stat *st);

/*
 * Get size of file pointer 'stream', pass to pointer 'size'. Return 'true' and
 * sets 'errno' on error, 'false' otherwise.
 */
bool fgetsize(FILE *stream, off_t *size);

/*
 * Rewind file pointer 'stream' to begining. Return 'true' and sets errno on
 * error, reutrns 'false' otherwise. Used to replace
 * 'void rewind(FILE *stream);', because it doesn't report errors.
 */
int frewind(FILE *stream);

/*
 * Reads file from 'path'. Returns NULL and sets errno on failure, new buffer
 * on success of file contents.
 */
char *readfile(const char *path);

/*
 * Safely read remaining contents of file pointer 'stream' to a new buffer.
 * Returns NULL and sets errno on faulure. File position is reset to that which
 * was passed.
 */
char *freadfile(FILE *stream);

#ifdef __cplusplus
}
#endif

#endif
