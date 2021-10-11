/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIBC_IO_H_
#define HLIBC_IO_H_

#include <stdio.h>
#include <sys/stat.h>

#include <hlibc/def.h>
#include <hlibc/term.h>

/*
 * Get optimal block size to read in file i/o.
 */
blksize_t io_blksize(struct stat *st);

/*
 * Get size of file pointed to by `stream`. Returns `-1` and sets `errno` on
 * failure.
 */
off_t fgetsize(FILE *stream);

/*
 * Rewind file pointer 'stream' to beginning. Returns -1 on failure and sets
 * errno. Replacement for `rewind()` for better error-handling.
 */
int frewind(FILE *stream);

/*
 * Reads file at path. Returns NULL and sets errno on failure, otherwise
 * returns a new buffer.
 */
char *readfile(const char *path);

/*
 * Safely read remaining contents of file pointer 'stream' to a new buffer.
 * Returns NULL and sets errno on filure. File position is reset to that which
 * was passed.
 */
char *freadfile(FILE *stream);

/*
 * Write decmial to stream cast as `long long`. Returns non-negative number on
 * success and EOF on failure, `errno` and the stream error indicator will be
 * set accordingly.
 */
int fputd(long long d, FILE *stream);

/*
 * Write float to stream cast as `long double`. Returns non-negative number on
 * success and EOF on failure, `errno` and the stream error indicator will be
 * set accordingly.
 */
int fputf(long double f, FILE *stream);

/*
 * Alias for `fputd` using stdout.
 */
int putd(long long d);

/*
 * Alias for `fputf` using stdout.
 */
int putf(long double f);

#endif
