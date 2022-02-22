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

#define HTEST_UNIT(func) ((struct htest_unit){ .name = #func, .run = (func) })

/*
 * Max length of test status message buffer.
 */
#define HTEST_STATUS_MSG_SIZE 256

enum htest_log_level {
    HTEST_INFO,
    HTEST_WARNING,
    HTEST_ERROR,
};

enum htest_status {
    HTEST_PASSED,
    HTEST_SKIPPED,
    HTEST_FAILED,
};

/*
 * Represents an instance of a `htest_unit`.
 */
struct htest {
    /*
     * Generic field to pass data into tests.
     */
    void *data;

    /*
     * Inherited name from test unit.
     */
    const char *name;

    FILE *log;

    char status_msg[HTEST_STATUS_MSG_SIZE];
    enum htest_status status;

    struct list_node resources;
    jmp_buf return_jmp;
};

struct htest_suite {
    struct htest_unit *units;
    char *name;

    /*
     * Skip all tests in this suite.
     */
    bool skip;

    /*
     * Runs before each test case. Return non-zero to indicate error in the
     * hook; errors will cancel the test case.
     */
    int (*init)(struct htest *);

    /*
     * Runs after each test case.
     */
    void (*destroy)(struct htest *);

    FILE *log;

    /*
     * Member node of global or other suite list.
     */
    struct list_node list;
};

struct htest_unit {
    const char *name;
    void (*run)(struct htest *);
};

struct htest_stats {
    size_t total;
    size_t passed;
    size_t failed;
    size_t skipped;
};

struct htest_assertion {
    struct htest *test;
    bool result;
    int line;
    const char *message;
    const char *file;
    const char *func;
};

struct htest_resource {
    void (*destroy)(void *);
    void *(*create)(void *);
};

/*
 * Initialize `struct htest` instance.
 */
int htest_init(struct htest *test, const char *name, FILE *log);

/*
 * Destroy `struct htest` and associated resources.
 */
void htest_destroy(struct htest *test);

/*
 * Creates, returns, and attaches resource to test. This will be cleaned up
 * on test termination.
 */
void *htest_create_resource(struct htest *test, struct htest_resource *res,
                            void *arg);

/*
 * Runs all tests in the suite, logging appropriatly.
 */
int htest_run_suite(struct htest_suite *suite, FILE *log);

/*
 * Pass a test. Optionally pass a message. Semantically equivalent to
 * returning without a failure from the unit's `run` function.
 */
noreturn void htest_pass(struct htest *test, const char *msg);

/*
 * Skip a test. Optionally pass a message.
 */
noreturn void htest_skip(struct htest *test, const char *msg);

/*
 * Fail a test. Optionally pass a message. Semantically equivalent to
 * a failed assertion.
 */
noreturn void htest_fail(struct htest *test, const char *msg);

void __htest_assert(const struct htest_assertion *assertion);
void __htest_log(enum htest_log_level level, FILE *log, const char *fmt, ...);

#define htest_log(level, target, fmt, ...)                            \
    do {                                                              \
        typeof(target) __target = (target);                           \
        __htest_log((level), __target->log, fmt "\n", ##__VA_ARGS__); \
    } while (0)

#define htest_error(test, fmt, ...)                                    \
    do {                                                               \
        struct htest *__test = (test);                                 \
        htest_log(HTEST_ERROR, __test, "    # %s: " fmt, __test->name, \
                  ##__VA_ARGS__);                                      \
    } while (0)

#define htest_warn(test, fmt, ...)                                       \
    do {                                                                 \
        struct htest *__test = (test);                                   \
        htest_log(HTEST_WARNING, __test, "    # %s: " fmt, __test->name, \
                  ##__VA_ARGS__);                                        \
    } while (0)

#define htest_info(test, fmt, ...)                                    \
    do {                                                              \
        struct htest *__test = (test);                                \
        htest_log(HTEST_INFO, __test, "    # %s: " fmt, __test->name, \
                  ##__VA_ARGS__);                                     \
    } while (0)

#define __HTEST_ASSERT(_test, _result, _message) \
    __htest_assert(&(struct htest_assertion){    \
        .test = (_test),                         \
        .result = (_result),                     \
        .message = (_message),                   \
        .file = __FILE__,                        \
        .line = __LINE__,                        \
        .func = __func__,                        \
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
