/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <stdlib.h>
#include <hlibc/def.h>
#include <hlibc/io.h>

off_t filesize(FILE *stream)
{
    off_t pos;
    off_t size;

    if ((pos = ftello(stream)) < 0 || fseek(stream, 0, SEEK_END) ||
        (size = ftello(stream)) < 0 || fseek(stream, pos, SEEK_SET)) {
        return -1;
    }

    return size;
}

off_t readfile(const char *path, void **dest)
{
    FILE *stream = NULL;
    off_t size = -1;

    if ((stream = fopen(path, "r"))) {
        size = freadfile(stream, dest);
        fclose(stream);
    }

    return size;
}

off_t freadfile(FILE *stream, void **dest)
{
    off_t size;

    if ((size = filesize(stream)) < 0 || !(*dest = malloc(size)) ||
        fread(*dest, 1, size, stream) != size) {
        free(*dest);
        return -1;
    }

    return size;
}
