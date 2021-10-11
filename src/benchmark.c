/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <stdlib.h>
#include <hlibc/benchmark.h>

struct benchmark_results *benchmark(const char *label, void (*callback)(void *),
                                    void *data)
{
    struct benchmark_results *results = malloc(sizeof(*results));

    return results;
}
