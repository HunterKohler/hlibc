#ifndef HLIBC_BENCHMARK_H_
#define HLIBC_BENCHMARK_H_

#include <time.h>

#define BENCHMARK(expr) ({ \
    clock_t t0 = clock(); \
    { expr } \
    (double)(clock() - t0) / CLOCKS_PER_SEC; \
})

#define BENCHMARK_P(label, expr) ({ \
    double dt = BENCHMARK(expr); \
    printf(label ": %f seconds\n", dt); \
    dt; \
})

#endif