/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#ifndef TESTLIB_H_
#define TESTLIB_H_

#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <hlibc/def.h>
#include <hlibc/list.h>
#include <hlibc/string.h>

#ifndef __has_attribute
#define __has_attribute(x) 0
#endif

#if __has_attribute(constructor)
#define __testlib_constructor __attribute__((constructor)) static
#else
#define __testlib_constructor static
#endif

#if __has_attribute(format)
#define __testlib_format(archetype, fmt, first) \
    __attribute__((format(archetype, fmt, first)))
#else
#define __testlib_format
#endif

#define TESTLIB_STACKTRACE_SIZE 5
#define DECLARE_TEST(NAME)           \
    extern struct testlib_test NAME; \
    static void __test_fn_##NAME();

#define DECLARE_TEST_GROUP(NAME) extern struct testlib_group NAME;

#define TEST(NAME)                                              \
    DECLARE_TEST(NAME);                                         \
                                                                \
    struct testlib_test NAME = {                                \
        .name = #NAME,                                          \
        .fn = __test_fn_##NAME,                                 \
    };                                                          \
                                                                \
    __testlib_constructor void __testlib_register_test_##NAME() \
    {                                                           \
        testlib_register_test(&NAME);                           \
    }                                                           \
                                                                \
    static void __test_fn_##NAME()

#define TEST_GROUP(NAME, TESTS)                                   \
    DECLARE_TEST_GROUP(NAME);                                     \
                                                                  \
    __testlib_constructor void __testlib_register_group_##NAME()  \
    {                                                             \
        testlib_register_group(&NAME);                            \
    }                                                             \
                                                                  \
    struct testlib_group NAME = {                                 \
        .name = #NAME,                                            \
        .test_count = array_size((struct testlib_test *[])TESTS), \
        .tests = (struct testlib_test *[])TESTS,                  \
    };

#define TESTLIB_FAIL_TEST() testlib_fail_test(__FILE__, __LINE__, __func__);

#define ASSERT(cond)                  \
    do {                              \
        bool __cond = !!(cond);       \
        if (unlikely(!__cond)) {      \
            __CAPTURE(#cond, __cond); \
            TESTLIB_FAIL_TEST()       \
        }                             \
    } while (0)

#define ASSERT_NOT(cond)              \
    do {                              \
        bool __cond = !!(cond);       \
        if (unlikely(__cond)) {       \
            __CAPTURE(#cond, __cond); \
            TESTLIB_FAIL_TEST()       \
        }                             \
    } while (0)

#define ASSERT_EQ(a, b)                                        \
    do {                                                       \
        static_assert(sizeof((a)) == sizeof((b)),              \
                      "Comparing values of difference sizes"); \
        typeof(a) __a = (a);                                   \
        typeof(b) __b = (b);                                   \
        if (unlikely(memcmp(&__a, &__b, sizeof(__a)))) {       \
            __CAPTURE(#a, __a);                                \
            __CAPTURE(#b, __b);                                \
            TESTLIB_FAIL_TEST()                                \
        }                                                      \
    } while (0)

#define ASSERT_PTR_EQ(a, b)                                    \
    do {                                                       \
        static_assert(sizeof((a)) == sizeof((b)),              \
                      "Comparing values of difference sizes"); \
        typeof(a) __a = (a);                                   \
        typeof(b) __b = (b);                                   \
        if (unlikely(memcmp(&__a, &__b, sizeof(__a)))) {       \
            __CAPTURE_PTR(#a, __a);                            \
            __CAPTURE_PTR(#b, __b);                            \
            TESTLIB_FAIL_TEST()                                \
        }                                                      \
    } while (0)

#define ASSERT_STR_EQ(a, b)                                    \
    do {                                                       \
        const char *__a = (a);                                 \
        const char *__b = (b);                                 \
        if (unlikely(strcmp(__a, __b))) {                      \
            __CAPTURE_STR(#a, __a);                            \
            __CAPTURE_STR(#b, __b);                            \
            TESTLIB_FAIL_TEST()                                \
        }                                                      \
    } while (0)

#define ASSERT_MEM_EQ(a, b, size)                              \
    do {                                                       \
        const void *__a = (a);                                 \
        const void *__b = (b);                                 \
        size_t __size = (size);                                \
        if (unlikely(memcmp(__a, __b, __size))) {              \
            __CAPTURE_MEM(#a, __a, __size);                    \
            __CAPTURE_MEM(#b, __b, __size);                    \
            TESTLIB_FAIL_TEST()                                \
        }                                                      \
    } while (0)

#define CAPTURE(val) __CAPTURE(#val, val)
#define CAPTURE_PTR(val) __CAPTURE_PTR(#val, val)
#define CAPTURE_STR(val) __CAPTURE_STR(#val, val)
#define CAPTURE_MEM(val, size) __CAPTURE_MEM(#val, val, size)

#define __CAPTURE(expr, value)                                          \
    do {                                                                \
        typeof(value) __value = (value);                                \
        testlib_add_capture(&__value, sizeof(__value),                  \
                            TESTLIB_TYPE_ID(__value), (expr), __FILE__, \
                            __LINE__, __func__);                        \
    } while (0)

#define __CAPTURE_PTR(expr, value)                                          \
    do {                                                                    \
        typeof(value) __value = (value);                                    \
        testlib_add_capture(&__value, sizeof(__value), TESTLIB_TYPE_ID_PTR, \
                            (expr), __FILE__, __LINE__, __func__);          \
    } while (0)

#define __CAPTURE_STR(expr, value)                                           \
    do {                                                                     \
        const char *__value = (value);                                       \
        testlib_add_capture(__value, strlen(__value) + 1,                    \
                            TESTLIB_TYPE_ID_STR, (expr), __FILE__, __LINE__, \
                            __func__);                                       \
    } while (0)

#define __CAPTURE_MEM(expr, value, size)                                     \
    do {                                                                     \
        const void *__value = (value);                                       \
        testlib_add_capture(__value, (size),                                 \
                            TESTLIB_TYPE_ID_MEM, (expr), __FILE__, __LINE__, \
                            __func__);                                       \
    } while (0)


struct testlib_location {
    char *file;
    char *func;
    size_t line;
    void **trace;
    char **trace_syms;
    size_t trace_size;
};

struct testlib_options {
};

struct testlib_group {
    struct list_node list;
    char *name;
    size_t test_count;
    struct testlib_test **tests;
};

struct testlib_test {
    struct list_node list;
    bool grouped;
    bool skip;
    bool only;
    void (*fn)();
    size_t line;
    char *file;
    char *name;
};

struct testlib_group_instance {
    struct list_node list;
    struct list_node tests;
    struct testlib_group *group;
};

struct testlib_test_instance {
    struct list_node list;
    struct list_node captures;
    struct list_node resources;
    struct testlib_test *test;
    struct testlib_fail *fail;
    clock_t begin;
    clock_t end;
};

enum testlib_type_id {
    TESTLIB_TYPE_ID_SHORT,
    TESTLIB_TYPE_ID_INT,
    TESTLIB_TYPE_ID_LONG,
    TESTLIB_TYPE_ID_LLONG,

    TESTLIB_TYPE_ID_USHORT,
    TESTLIB_TYPE_ID_UINT,
    TESTLIB_TYPE_ID_ULONG,
    TESTLIB_TYPE_ID_ULLONG,

    TESTLIB_TYPE_ID_CHAR,
    TESTLIB_TYPE_ID_UCHAR,
    TESTLIB_TYPE_ID_SCHAR,

    TESTLIB_TYPE_ID_FLOAT,
    TESTLIB_TYPE_ID_DOUBLE,
    TESTLIB_TYPE_ID_LDOUBLE,

    TESTLIB_TYPE_ID_BOOL,
    TESTLIB_TYPE_ID_PTR,
    TESTLIB_TYPE_ID_STR,
    TESTLIB_TYPE_ID_MEM,
};

/* clang-format off */

#define TESTLIB_TYPE_ID(x) _Generic((x),        \
    short: TESTLIB_TYPE_ID_SHORT,               \
    int: TESTLIB_TYPE_ID_INT,                   \
    long: TESTLIB_TYPE_ID_LONG,                 \
    long long: TESTLIB_TYPE_ID_LLONG,           \
                                                \
    unsigned short: TESTLIB_TYPE_ID_USHORT,     \
    unsigned int: TESTLIB_TYPE_ID_UINT,         \
    unsigned long: TESTLIB_TYPE_ID_ULONG,       \
    unsigned long long: TESTLIB_TYPE_ID_ULLONG, \
                                                \
    char: TESTLIB_TYPE_ID_CHAR,                 \
    signed char: TESTLIB_TYPE_ID_SCHAR,         \
    unsigned char: TESTLIB_TYPE_ID_UCHAR,       \
                                                \
    float: TESTLIB_TYPE_ID_FLOAT,               \
    double: TESTLIB_TYPE_ID_DOUBLE,             \
    long double: TESTLIB_TYPE_ID_LDOUBLE,       \
                                                \
    bool: TESTLIB_TYPE_ID_BOOL                  \
)

/* clang-format on */

struct testlib_capture {
    struct list_node list;
    size_t size;
    void *value;
    char *expr;
    struct testlib_location location;
    enum testlib_type_id type_id;
};

struct testlib_fail {
    struct testlib_location location;
};

int testlib_main(int argc, char **argv);
void *testlib_memcheck(void *mem);

#define testlib_malloc(size) testlib_memcheck(malloc(size))
#define testlib_zalloc(size) testlib_memcheck(calloc(1, size))
#define testlib_realloc(ptr, size) testlib_memcheck(realloc(ptr, size))
#define testlib_strdup(str) testlib_memcheck(strdup(str))
#define testlib_memdup(ptr, size) testlib_memcheck(memdup(ptr, size))

int testlib_asprintf(char **ptr, const char *fmt, ...)
    __testlib_format(printf, 2, 3);

void testlib_location_init(struct testlib_location *location, const char *file,
                           size_t line, const char *func);

void testlib_location_destroy(struct testlib_location *location);

void testlib_add_capture(const void *value, size_t size, enum testlib_type_id type_id,
                         const char *expr, const char *file, size_t line,
                         const char *func);

void testlib_capture_destroy(struct testlib_capture *capture);

void testlib_register_group(struct testlib_group *group);
void testlib_register_test(struct testlib_test *test);

void testlib_fail_test(const char *file, size_t line, const char *func);

noreturn void testlib_run(struct testlib_options *opts,
                          struct testlib_test **tests, size_t test_count,
                          struct testlib_group **groups, size_t group_count);

#endif
