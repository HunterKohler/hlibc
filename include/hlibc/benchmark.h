/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIBC_BENCHMARK_H_
#define HLIBC_BENCHMARK_H_

#include <hlibc/def.h>
#include <time.h>

struct benchmark_results {
    time_t start_s;
    time_t end_s;
    long start_ns;
    long end_ns;
};

struct benchmark_results *benchmark(const char *label, void (*callback)(void *),
                                    void *data);

#endif
