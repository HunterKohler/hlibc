/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#ifndef HTEST_TEST_H_
#define HTEST_TEST_H_

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include <setjmp.h>
#include <hlibc/def.h>
#include <hlibc/list.h>

// #define HTEST_ARRAY_PARAM(name, arr, gen_desc)                            \
//     static void *name(void *__prev, char *__desc)                         \
//     {                                                                     \
//         typeof(*(arr)) *__it = __prev ? (typeof(__it))__prev + 1 : (arr); \
//         void (*__gen_desc)(typeof(__it), char *) = (gen_desc);            \
//                                                                           \
//         if (__it == array_end((arr)))                                     \
//             return NULL;                                                  \
//                                                                           \
//         __gen_desc(__it, __desc);                                         \
//         return __it;                                                      \
//     }

#define HTEST_UNIT(run_test_fn)    \
    ((struct htest_unit){          \
        .name = #run_test_fn,      \
        .run_test = (run_test_fn), \
    })

#define HTEST_UNIT_PARAM(run_test_fn, gen_param_fn) \
    ((struct htest_unit){                           \
        .name = #run_test_fn,                       \
        .run_test = (run_test_fn),                  \
        .gen_param = (gen_param_fn),                \
    })

enum htest_status {
    HTEST_PASSED,
    HTEST_SKIPPED,
    HTEST_FAILED,
};

struct htest {
    const char *desc;

    void *data;
    void *param;

    enum htest_status status;
    struct list_node resources;

    FILE *log;
    jmp_buf return_jmp;
    pthread_mutex_t lock;
};

struct htest_suite {
    const char *name;

    struct htest_unit *units;

    // hooks
    // int (*before_each)(struct htest *);
    // int (*after_each)(struct htest *s;
    // int (*before_all)(struct htest_suite *);
    // int (*after_all)(struct htest_suite *);

    struct list_node list; // member node

    // bool skip;
};

struct htest_unit {
    const char *name;
    void (*run_test)(struct htest *);
    void *(*gen_params)(void *data, char *desc);
};

struct htest_stats {
    size_t total;
    size_t passed;
    size_t failed;
    size_t skipped;
};

struct htest_assertion {
    struct htest *test;
    bool passed;
    int line;
    const char *message;
    const char *file;
    const char *func;
};

struct htest_resource {
    void *(*create)(void *);
    void (*destroy)(void *);
};

/*
 * Creates, returns, and attaches resource to test. This will be cleaned up
 * on test termination.
 */
void *htest_create_resource(struct htest *test, struct htest_resource *res,
                            void *arg);

/*
 * Allocates memory, and adds to the test's resource list.
 */
void *htest_malloc(struct htest *test, size_t size);

/*
 * Allocates memory for a string, and adds to the test's resource list.
 */
char *htest_stralloc(struct htest *test, size_t size);

/*
 * Duplicate a null-terminated string, and add it to the test's resource list.
 */
char *htest_strdup(struct htest *test, const char *src);

/*
 * Opens a file, and adds an associated resource to the test's resource list.
 */
FILE *htest_fopen(struct htest *test, const char *restrict pathname,
                  const char *restrict mode);

/*
 * Runs all tests in the suite, logging appropriatly.
 */
int htest_run_suite(struct htest_suite *suite, FILE *log);

/*
 * Pass a test. Optionally pass a message. Semantically equivalent to
 * returning without a failure from the unit's `run` function.
 */
noreturn void htest_pass(struct htest *test, const char *fmt, ...);

/*
 * Skip a test. Optionally pass a message.
 */
noreturn void htest_skip(struct htest *test, const char *fmt, ...);

/*
 * Fail a test. Optionally pass a message. Semantically equivalent to
 * a failed assertion.
 */
noreturn void htest_fail(struct htest *test, const char *fmt, ...);

void htest_error(struct htest *test, const char *fmt, ...) __printf(2, 3);
void htest_warn(struct htest *test, const char *fmt, ...) __printf(2, 3);
void htest_info(struct htest *test, const char *fmt, ...) __printf(2, 3);

void __htest_assert(struct htest_assertion *);

#define __HTEST_ASSERT(_test, _pass, _message) \
    __htest_assert(&(struct htest_assertion){  \
        .test = (_test),                       \
        .passed = (_pass),                     \
        .message = (_message),                 \
        .file = __FILE__,                      \
        .line = __LINE__,                      \
        .func = __func__,                      \
    })

#define HTEST_ASSERT(test, cond)                                               \
    do {                                                                       \
        struct htest *__test = (test);                                         \
        bool __cond = !!(cond);                                                \
        __HTEST_ASSERT(__test, __cond, "Expected condition to evaluate true"); \
    } while (0)

#define HTEST_ASSERT_NOT(test, cond)                            \
    do {                                                        \
        struct htest *__test = (test);                          \
        bool __cond = !(cond);                                  \
        __HTEST_ASSERT(__test, __cond,                          \
                       "Expected condition to evaluate false"); \
    } while (0)

#define HTEST_ASSERT_EQ(test, a, b)                                          \
    do {                                                                     \
        struct htest *__test = (test);                                       \
        typeof(a) __a = (a);                                                 \
        typeof(b) __b = (b);                                                 \
        __HTEST_ASSERT(__test, __a == __b, "Expected operands to be equal"); \
    } while (0)

#define HTEST_ASSERT_NE(test, a, b)                          \
    do {                                                     \
        struct htest *__test = (test);                       \
        typeof(a) __a = (a);                                 \
        typeof(b) __b = (b);                                 \
        __HTEST_ASSERT(__test, __a != __b,                   \
                       "Expected operands to not be equal"); \
    } while (0)

#define HTEST_ASSERT_LT(test, a, b)                                        \
    do {                                                                   \
        struct htest *__test = (test);                                     \
        typeof(a) __a = (a);                                               \
        typeof(b) __b = (b);                                               \
        __HTEST_ASSERT(__test, __a < __b,                                  \
                       "Expected left operand to be less than the right"); \
    } while (0)

#define HTEST_ASSERT_GT(test, a, b)                                           \
    do {                                                                      \
        struct htest *__test = (test);                                        \
        typeof(a) __a = (a);                                                  \
        typeof(b) __b = (b);                                                  \
        __HTEST_ASSERT(__test, __a > __b,                                     \
                       "Expected left operand to be greater than the right"); \
    } while (0)

#define HTEST_ASSERT_LTE(test, a, b)                                        \
    do {                                                                    \
        struct htest *__test = (test);                                      \
        typeof(a) __a = (a);                                                \
        typeof(b) __b = (b);                                                \
        __HTEST_ASSERT(                                                     \
            __test, __a <= __b,                                             \
            "Expected left operand to be less than or equal to the right"); \
    } while (0)

#define HTEST_ASSERT_GTE(test, a, b)                                           \
    do {                                                                       \
        struct htest *__test = (test);                                         \
        typeof(a) __a = (a);                                                   \
        typeof(b) __b = (b);                                                   \
        __HTEST_ASSERT(                                                        \
            __test, __a >= __b,                                                \
            "Expected left operand to be greater than or equal to the right"); \
    } while (0)

/*
 * Temporarily aliased. Will change later, to allow for pointer
 * serialization.
 */
#define HTEST_ASSERT_PTR_EQ HTEST_ASSERT_EQ
#define HTEST_ASSERT_PTR_NE HTEST_ASSERT_NE

#define HTEST_ASSERT_STR_EQ(test, a, b)                 \
    do {                                                \
        struct htest *__test = (test);                  \
        const char *__a = (a);                          \
        const char *__b = (b);                          \
        __HTEST_ASSERT(__test, !strcmp(__a, __b),       \
                       "Expected strings to be equal"); \
    } while (0)

#define HTEST_ASSERT_STR_NE(test, a, b)                     \
    do {                                                    \
        struct htest *__test = (test);                      \
        const char *__a = (a);                              \
        const char *__b = (b);                              \
        __HTEST_ASSERT(__test, strcmp(__a, __b),            \
                       "Expected strings to not be equal"); \
    } while (0)

#define HTEST_ASSERT_MEM_EQ(test, a, b, size)             \
    do {                                                  \
        struct htest *__test = (test);                    \
        const void *__a = (a);                            \
        const void *__b = (b);                            \
        size_t __size = (size);                           \
        __HTEST_ASSERT(__test, !memcmp(__a, __b, __size), \
                       "Expected memory to be equal");    \
    } while (0)

#define HTEST_ASSERT_MEM_NE(test, a, b, size)                \
    do {                                                     \
        struct htest *__test = (test);                       \
        const void *__a = (a);                               \
        const void *__b = (b);                               \
        size_t __size = (size);                              \
        __HTEST_ASSERT(__test, memcmp(__a, __b, __sizesize), \
                       "Expected memory to not be equal");   \
    } while (0)

extern struct list_node __htest_global_suite_list;

void __htest_declare_suites(struct htest_suite **suites, size_t n);

#define __HTEST_DECLARE_SUITES(unique_array, unique_constructor, ...)   \
    static struct htest_suite *unique_array[] = { __VA_ARGS__ };        \
    static __attribute__((constructor)) void unique_constructor()       \
    {                                                                   \
        __htest_declare_suites(unique_array, array_size(unique_array)); \
    }

#define HTEST_DECLARE_SUITES(...)                                  \
    __HTEST_DECLARE_SUITES(UNIQUE_ID(__htest_declare_array),       \
                           UNIQUE_ID(__htest_declare_constructor), \
                           __VA_ARGS__)

void htest_run_global_suites();

#endif
