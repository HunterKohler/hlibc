/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <stdlib.h>
#include <hlibc/io.h>

long fgetsize(FILE *stream)
{
    long start, end;

    if ((start = ftell(stream)) < 0 || fseek(stream, 0, SEEK_END) ||
        (end = ftell(stream)) < 0 || fseek(stream, start, SEEK_SET))
        return -1;

    return end;
}
