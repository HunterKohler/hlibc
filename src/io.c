/*
 * Copyright (C) 2021 John Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <stdlib.h>
#include <sys/stat.h>
#include <hlibc/def.h>
#include <hlibc/io.h>
#include <hlibc/string.h>
#include <hlibc/math.h>

blksize_t io_blksize(struct stat *st)
{
    return max(BUFSIZ, st->st_blksize);
}

off_t fgetsize(FILE *stream)
{
    int fd;
    struct stat st;
    if ((fd = fileno(stream)) == -1 || fstat(fd, &st)) {
        return -1;
    }
    return st.st_size;
}

int frewind(FILE *stream)
{
    return fseek(stream, 0, SEEK_SET);
}

char *readfile(const char *path)
{
    FILE *stream = fopen(path, "r");
    if (!stream) {
        return NULL;
    }

    char *buf = freadfile(stream);
    if (fclose(stream)) {
        free(buf);
        return NULL;
    }

    return buf;
}

char *freadfile(FILE *stream)
{
    off_t pos_i;
    struct stat st;

    if ((pos_i = ftello(stream)) == -1 || fstat(fileno(stream), &st)) {
        return NULL;
    }

    size_t buf_size = st.st_size - pos_i;
    char *buf = stralloc(buf_size);

    if (!buf) {
        return NULL;
    }

    blksize_t block_size = io_blksize(&st);
    blksize_t read_size = block_size;

    for (int i = 0; i < buf_size && read_size == block_size; i += block_size) {
        read_size = fread(buf + i, sizeof *buf, block_size, stream);
    }

    if (feof(stream) && !fseeko(stream, pos_i, SEEK_SET)) {
        return buf;
    } else {
        free(buf);
        if (!errno) {
            errno = EIO;
        }
        return NULL;
    }
}
