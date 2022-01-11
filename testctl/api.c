#include <setjmp.h>
#include <testctl/api.h>
#include <testctl/internal.h>

void __testctl_create_resource(const struct testctl_resource *res, void *arg,
                               const char *file, size_t line, const char *func)
{
    struct testctl_resource_handle *handle = safe_malloc(sizeof(*handle));
    testctl_resource_handle_init(handle, res, arg, file, line, func);
    list_add_tail(&handle->list, &active_test.resources);
}

void __testctl_register_test(struct testctl_test **tp, void (*func)(),
                             const char *name, const char *file, size_t line,
                             const char *fname)
{
    struct testctl_test *test = safe_malloc(sizeof(*test));
    testctl_test_init(test, func, name, file, line, fname);
    list_add_tail(&test->list, &global_tests);
    *tp = test;
}

void __testctl_register_group(struct testctl_group **gp,
                              struct testctl_test **tests, size_t size,
                              const char *name, const char *file, size_t line,
                              const char *fname)
{
    struct testctl_test *group = safe_malloc(sizeof(*group));
    testctl_group_init(group, tests, size, name, file, line, fname);
    list_add_tail(&group->list, &global_groups);
    *gp = group;
}

void __testctl_capture(void *val, size_t size, enum __testctl_type type,
                       const char *expr, const char *file, size_t line,
                       const char *func)
{
    struct testctl_capture *cap = safe_malloc(sizeof(*cap));
    testctl_capture_init(cap, val, size, type, expr, file, line, fname);
    list_add_tail(cap->list, &active_test.captures);
}

void __testctl_fail(const char *file, size_t line, const char *func)
{
    active_test.failed = true;
    testctl_location_init(&active_test.failpos, file, line, func);
    longjmp(&test_return_jmp, 1);
}
