#ifndef TESTCTL_API_H_
#define TESTCTL_API_H_

#include <hlibc/def.h>
#include <hlibc/list.h>

#ifndef __has_attribute
#define __has_attribute(x) 0
#endif

#if __has_attribute(constructor)
#define __testctl_constructor __attribute__((constructor)) static
#else
#define __testctl_constructor static
#endif

#define DECLARE_TEST(NAME)            \
    extern struct testctl_test *NAME; \
    void __test_##NAME();

#define DECLARE_TEST_GROUP(NAME) extern struct testctl_group *NAME;

#define TEST(NAME)                                                        \
    DECLARE_TEST(NAME);                                                   \
                                                                          \
    struct testctl_test *NAME;                                            \
                                                                          \
    __testctl_constructor void __testctl_register_test_##NAME()           \
    {                                                                     \
        __testctl_register_test(&NAME, __test_fn_##NAME, #NAME, __FILE__, \
                                __LINE__, __func__);                      \
    }                                                                     \
                                                                          \
    static void __test_fn_##NAME()

#define TEST_GROUP(NAME, TESTS)                                       \
    DECLARE_TEST_GROUP(NAME);                                         \
                                                                      \
    struct testctl_group *NAME;                                       \
                                                                      \
    __testctl_constructor void __testctl_register_group_##NAME()      \
    {                                                                 \
        struct testctl_test *tests[] = TESTS;                         \
        __testctl_register_group(&NAME, tests, size, #NAME, __FILE__, \
                                 __LINE__, __func__);                 \
    }

#define ASSERT(cond)                  \
    do {                              \
        bool __cond = !!(cond);       \
        if (unlikely(!__cond)) {      \
            __CAPTURE(#cond, __cond); \
            testctl_fail();           \
        }                             \
    } while (0)

#define ASSERT_NOT(cond)              \
    do {                              \
        bool __cond = !!(cond);       \
        if (unlikely(__cond)) {       \
            __CAPTURE(#cond, __cond); \
            testctl_fail();           \
        }                             \
    } while (0)

#define ASSERT_EQ(a, b)             \
    do {                            \
        typeof(a) __a = (a);        \
        typeof(b) __b = (b);        \
        if (unlikely(__a != __b)) { \
            __CAPTURE(#a, __a);     \
            __CAPTURE(#b, __b);     \
            testctl_fail();         \
        }                           \
    } while (0)

#define ASSERT_NE(a, b)             \
    do {                            \
        typeof(a) __a = (a);        \
        typeof(b) __b = (b);        \
        if (unlikely(__a == __b)) { \
            __CAPTURE(#a, __a);     \
            __CAPTURE(#b, __b);     \
            testctl_fail();         \
        }                           \
    } while (0)

#define ASSERT_GT(a, b)             \
    do {                            \
        typeof(a) __a = (a);        \
        typeof(b) __b = (b);        \
        if (unlikely(__a <= __b)) { \
            __CAPTURE(#a, __a);     \
            __CAPTURE(#b, __b);     \
            testctl_fail();         \
        }                           \
    } while (0)

#define ASSERT_LT(a, b)             \
    do {                            \
        typeof(a) __a = (a);        \
        typeof(b) __b = (b);        \
        if (unlikely(__a >= __b)) { \
            __CAPTURE(#a, __a);     \
            __CAPTURE(#b, __b);     \
            testctl_fail();         \
        }                           \
    } while (0)

#define ASSERT_GTE(a, b)           \
    do {                           \
        typeof(a) __a = (a);       \
        typeof(b) __b = (b);       \
        if (unlikely(__a < __b)) { \
            __CAPTURE(#a, __a);    \
            __CAPTURE(#b, __b);    \
            testctl_fail();        \
        }                          \
    } while (0)

#define ASSERT_LTE(a, b)           \
    do {                           \
        typeof(a) __a = (a);       \
        typeof(b) __b = (b);       \
        if (unlikely(__a > __b)) { \
            __CAPTURE(#a, __a);    \
            __CAPTURE(#b, __b);    \
            testctl_fail();        \
        }                          \
    } while (0)

#define ASSERT_PTR_EQ(a, b)         \
    do {                            \
        typeof(a) __a = (a);        \
        typeof(b) __b = (b);        \
        if (unlikely(__a != __b)) { \
            __CAPTURE_PTR(#a, __a); \
            __CAPTURE_PTR(#b, __b); \
            testctl_fail();         \
        }                           \
    } while (0)

#define ASSERT_PTR_NE(a, b)         \
    do {                            \
        typeof(a) __a = (a);        \
        typeof(b) __b = (b);        \
        if (unlikely(__a == __b)) { \
            __CAPTURE_PTR(#a, __a); \
            __CAPTURE_PTR(#b, __b); \
            testctl_fail();         \
        }                           \
    } while (0)

#define ASSERT_STR_EQ(a, b)               \
    do {                                  \
        const char *__a = (a);            \
        const char *__b = (b);            \
        if (unlikely(strcmp(__a, __b))) { \
            __CAPTURE_STR(#a, __a);       \
            __CAPTURE_STR(#b, __b);       \
            testctl_fail();               \
        }                                 \
    } while (0)

#define ASSERT_STR_NE(a, b)                \
    do {                                   \
        const char *__a = (a);             \
        const char *__b = (b);             \
        if (unlikely(!strcmp(__a, __b))) { \
            __CAPTURE_STR(#a, __a);        \
            __CAPTURE_STR(#b, __b);        \
            testctl_fail();                \
        }                                  \
    } while (0)

#define ASSERT_MEM_EQ(a, b, size)               \
    do {                                        \
        const void *__a = (a);                  \
        const void *__b = (b);                  \
        size_t __size = (size);                 \
        if (unlikely(memcmp(__a, __b, size))) { \
            __CAPTURE_MEM(#a, __a, __size);     \
            __CAPTURE_MEM(#b, __b, __size);     \
            testctl_fail();                     \
        }                                       \
    } while (0)

#define ASSERT_MEM_NE(a, b, size)                \
    do {                                         \
        const void *__a = (a);                   \
        const void *__b = (b);                   \
        size_t __size = (size);                  \
        if (unlikely(!memcmp(__a, __b, size))) { \
            __CAPTURE_MEM(#a, __a, __size);      \
            __CAPTURE_MEM(#b, __b, __size);      \
            testctl_fail();                      \
        }                                        \
    } while (0)

#define CAPTURE(val) __CAPTURE(#val, val)
#define CAPTURE_PTR(val) __CAPTURE_PTR(#val, val)
#define CAPTURE_STR(val) __CAPTURE_STR(#val, val)
#define CAPTURE_MEM(val, size) __CAPTURE_MEM(#val, val, size)

#define __CAPTURE(expr, val)                                              \
    do {                                                                  \
        typeof(val) __val = (val);                                        \
        __testctl_capture(&__val, sizeof(__val), __testctl_typeof(__val), \
                          (expr), __FILE__, __LINE__, __func__);          \
    } while (0)

#define __CAPTURE_PTR(expr, val)                                            \
    do {                                                                    \
        void *__val = (val);                                                \
        __testctl_capture(__val, sizeof(__val), __TESTCTL_TYPE_PTR, (expr), \
                          __FILE__, __LINE__, __func__);                    \
    } while (0)

#define __CAPTURE_STR(expr, val)                                        \
    do {                                                                \
        const char *__val = (val);                                      \
        __testctl_capture(__val, strlen(__val) + 1, __TESTCTL_TYPE_STR, \
                          (expr), __FILE__, __LINE__, __func__);        \
    } while (0)

#define __CAPTURE_MEM(expr, val, size)                                         \
    do {                                                                       \
        const void *__val = (val);                                             \
        __testctl_capture(__val, (size), __TESTCTL_TYPE_STR, (expr), __FILE__, \
                          __LINE__, __func__);                                 \
    } while (0)

enum __testctl_type {
    __TESTCTL_TYPE_SHORT,
    __TESTCTL_TYPE_INT,
    __TESTCTL_TYPE_LONG,
    __TESTCTL_TYPE_LLONG,
    __TESTCTL_TYPE_USHORT,
    __TESTCTL_TYPE_UINT,
    __TESTCTL_TYPE_ULONG,
    __TESTCTL_TYPE_ULLONG,
    __TESTCTL_TYPE_CHAR,
    __TESTCTL_TYPE_UCHAR,
    __TESTCTL_TYPE_SCHAR,
    __TESTCTL_TYPE_FLOAT,
    __TESTCTL_TYPE_DOUBLE,
    __TESTCTL_TYPE_LDOUBLE,
    __TESTCTL_TYPE_BOOL,
    __TESTCTL_TYPE_PTR,
    __TESTCTL_TYPE_STR,
    __TESTCTL_TYPE_MEM,
};

/* clang-format off */
#define __testctl_typeof(x) _Generic((x),      \
    short: __TESTCTL_TYPE_SHORT,               \
    int: __TESTCTL_TYPE_INT,                   \
    long: __TESTCTL_TYPE_LONG,                 \
    long long: __TESTCTL_TYPE_LLONG,           \
                                               \
    unsigned short: __TESTCTL_TYPE_USHORT,     \
    unsigned int: __TESTCTL_TYPE_UINT,         \
    unsigned long: __TESTCTL_TYPE_ULONG,       \
    unsigned long long: __TESTCTL_TYPE_ULLONG, \
                                               \
    char: __TESTCTL_TYPE_CHAR,                 \
    signed char: __TESTCTL_TYPE_SCHAR,         \
    unsigned char: __TESTCTL_TYPE_UCHAR,       \
                                               \
    float: __TESTCTL_TYPE_FLOAT,               \
    double: __TESTCTL_TYPE_DOUBLE,             \
    long double: __TESTCTL_TYPE_LDOUBLE,       \
                                               \
    bool: __TESTCTL_TYPE_BOOL                  \
)
/* clang-format on */

struct testctl_test;
struct testctl_group;
struct testctl_resource {
    char *name;
    void *(*create)(void *);
    void (*destroy)(void *);
};

#define testctl_fail() __testctl_fail(__FILE__, __LINE__, __func__)
#define testctl_create_resource(res, arg) \
    __testctl_create_resource((res), (arg), __FILE__, __LINE__, __func__)

void __testctl_create_resource(const struct testctl_resource *res, void *arg,
                               const char *file, size_t line, const char *func);

void __testctl_register_test(struct testctl_test **tp, void (*func)(),
                             const char *name, const char *file, size_t line);

void __testctl_register_group(struct testctl_group **gp,
                              struct testctl_test **tests, size_t size,
                              const char *name, const char *file, size_t line);

void __testctl_capture(void *val, size_t size, enum __testctl_type type,
                       const char *expr, const char *file, size_t line,
                       const char *func);

void __testctl_fail(const char *file, size_t line, const char *func);

#endif
