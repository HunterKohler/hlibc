#include <execinfo.h>
#include <setjmp.h>
#include <hlibc/list.h>
#include <hlibc/math.h>
#include <testlib/testlib.h>

struct list_node testlib_groups = LIST_NODE_INIT(testlib_groups);
struct list_node testlib_tests = LIST_NODE_INIT(testlib_tests);
struct testlib_test_instance *testlib_active_test;
struct testlib_group_instance *testlib_active_group;
struct testlib_group testlib_root;

jmp_buf testlib_fail_jmp;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
int testlib_main(int argc, char **argv)
{
    struct testlib_options opts;
    testlib_run(&opts, NULL, 0, NULL, 0);
}
#pragma GCC diagnostic pop

void *testlib_memcheck(void *mem)
{
    if (!mem)
        exit(1);
    return mem;
}

void testlib_location_init(struct testlib_location *location, const char *file,
                           size_t line, const char *func)
{
    location->file = testlib_strdup(file);
    location->func = testlib_strdup(func);
    location->line = line;
    location->trace_size = TESTLIB_STACKTRACE_SIZE;
    location->trace =
        testlib_malloc(location->trace_size * sizeof(*location->trace_syms));
    location->trace_size = backtrace(location->trace, location->trace_size);
    location->trace_syms = testlib_memcheck(
        backtrace_symbols(location->trace, location->trace_size));
}

void testlib_location_destroy(struct testlib_location *location)
{
    free(location->file);
    free(location->func);
    free(location->trace);
    free(location->trace_syms);
}

void testlib_add_capture(const void *value, size_t size, enum testlib_type_id type_id,
                         const char *expr, const char *file, size_t line,
                         const char *func)
{
    struct testlib_capture *capture = testlib_malloc(sizeof(*capture));

    capture->size = size;
    capture->type_id = type_id;
    capture->value = testlib_memdup(value, size);
    capture->expr = testlib_strdup(expr);

    testlib_location_init(&capture->location, file, line, func);
    list_add(&capture->list, &testlib_active_test->captures);
}

void testlib_capture_destroy(struct testlib_capture *capture)
{
    free(capture->value);
    free(capture->expr);
    testlib_location_destroy(&capture->location);
    list_rm(&capture->list);
}

void testlib_register_group(struct testlib_group *group)
{
    list_add(&group->list, &testlib_groups);

    for (int i = 0; i < group->test_count; i++)
        group->tests[i]->grouped = true;
}

void testlib_register_test(struct testlib_test *test)
{
    list_add(&test->list, &testlib_tests);
}

void testlib_fail_test(const char *file, size_t line, const char *func)
{
    struct testlib_fail *fail = testlib_malloc(sizeof(*fail));
    testlib_location_init(&fail->location, file, line, func);
    testlib_active_test->fail = fail;
    longjmp(testlib_fail_jmp, 1);
}

static void testlib_init_root()
{
    size_t capacity = 50;
    struct testlib_test *test;
    struct testlib_group *root = &testlib_root;

    root->test_count = 0;
    root->tests = testlib_malloc(capacity * sizeof(*root->tests));

    list_for_each_entry (test, &testlib_tests, list) {
        if (!test->grouped) {
            if (root->test_count == capacity) {
                root->tests = testlib_realloc(
                    root->tests, (capacity *= 2) * sizeof(*root->tests));
            }

            root->tests[root->test_count++] = test;
        }
    }

    root->name = "root";
    root->tests =
        testlib_realloc(root->tests, root->test_count * sizeof(*root->tests));

    testlib_register_group(root);
}

static size_t testlib_type_stringify(enum testlib_type_id type_id, void *value,
                                     char *dest, size_t size)
{
    switch (type_id) {
    case TESTLIB_TYPE_ID_SHORT:
        return snprintf(dest, size, "%hd", *(short *)value);
    case TESTLIB_TYPE_ID_INT:
        return snprintf(dest, size, "%d", *(int *)value);
    case TESTLIB_TYPE_ID_LONG:
        return snprintf(dest, size, "%ld", *(long *)value);
    case TESTLIB_TYPE_ID_LLONG:
        return snprintf(dest, size, "%lld", *(long long *)value);
    case TESTLIB_TYPE_ID_USHORT:
        return snprintf(dest, size, "%hu", *(unsigned short *)value);
    case TESTLIB_TYPE_ID_UINT:
        return snprintf(dest, size, "%u", *(unsigned int *)value);
    case TESTLIB_TYPE_ID_ULONG:
        return snprintf(dest, size, "%lu", *(unsigned long *)value);
    case TESTLIB_TYPE_ID_ULLONG:
        return snprintf(dest, size, "%llu", *(unsigned long long *)value);
    case TESTLIB_TYPE_ID_CHAR:
        return snprintf(dest, size, "%c", *(char *)value);
    case TESTLIB_TYPE_ID_UCHAR:
        return snprintf(dest, size, "%c", *(unsigned char *)value);
    case TESTLIB_TYPE_ID_SCHAR:
        return snprintf(dest, size, "%c", *(signed char *)value);
    case TESTLIB_TYPE_ID_FLOAT:
        return snprintf(dest, size, "%f", *(float *)value);
    case TESTLIB_TYPE_ID_DOUBLE:
        return snprintf(dest, size, "%lf", *(double *)value);
    case TESTLIB_TYPE_ID_LDOUBLE:
        return snprintf(dest, size, "%Lf", *(long double *)value);
    case TESTLIB_TYPE_ID_BOOL:
        return snprintf(dest, size, "%s", *(bool *)value ? "true" : "false");
    case TESTLIB_TYPE_ID_PTR:
        return snprintf(dest, size, "%p", *(void **)value);
    case TESTLIB_TYPE_ID_STR:
        return snprintf(dest, size, "%s", (char *)value);
    default:
        return snprintf(dest, size, "(cannot serialize type)");
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
noreturn void testlib_run(struct testlib_options *opts,
                          struct testlib_test **tests, size_t test_count,
                          struct testlib_group **groups, size_t group_count)
{
    bool fails = false;
    struct testlib_group *group;
    struct list_node *group_instances =
        testlib_malloc(sizeof(*group_instances));
    list_node_init(group_instances);

    for (int i = 0; i < test_count; i++)
        testlib_register_test(tests[i]);

    for (int i = 0; i < group_count; i++)
        testlib_register_group(groups[i]);

    testlib_init_root();

    list_for_each_entry (group, &testlib_groups, list) {
        testlib_active_group = testlib_malloc(sizeof(*testlib_active_group));
        testlib_active_group->group = group;

        list_node_init(&testlib_active_group->tests);
        list_add(&testlib_active_group->list, group_instances);

        for (int i = 0; i < group->test_count; i++) {
            testlib_active_test = testlib_malloc(sizeof(*testlib_active_test));
            testlib_active_test->test = group->tests[i];

            list_node_init(&testlib_active_test->captures);
            list_node_init(&testlib_active_test->resources);
            list_add(&testlib_active_test->list, &testlib_active_group->tests);

            if (setjmp(testlib_fail_jmp)) {
                struct testlib_fail *fail = testlib_active_test->fail;
                fails = true;

                printf("%s failed\n", testlib_active_test->test->name);
                printf("%s:%zu:\n", fail->location.file, fail->location.line);

                struct testlib_capture *capture;
                list_for_each_entry (capture, &testlib_active_test->captures,
                                     list) {
                    char tmp[1024];
                    testlib_type_stringify(capture->type_id, capture->value,
                                           tmp, 1024);
                    printf("%s := %s\n", capture->expr, tmp);
                }

            } else {
                testlib_active_test->test->fn();
                printf("%s succeeded\n", testlib_active_test->test->name);
            }
        }
    }

    exit(fails);
}
#pragma GCC diagnostic pop

int testlib_asprintf(char **ptr, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int ret = vasprintf(ptr, fmt, ap);
    va_end(ap);

    if (ret < 0)
        testlib_memcheck(NULL);
    return ret;
}
