#ifndef INTERNAL_H_
#define INTERNAL_H_

#include <time.h>
#include <string.h>
#include <setjmp.h>

extern struct testctl_test_handle active_test;
extern struct testctl_group_handle active_group;
extern struct list_node global_tests;
extern struct list_node global_groups;
extern jmp_buf test_return_jmp;

#define safe_malloc(size) testctl_memcheck(malloc(size))
#define safe_zalloc(size) testctl_memcheck(calloc(1, size))
#define safe_realloc(ptr, size) testctl_memcheck(realloc(ptr, size))

noreturn void testctl_run();

char *safe_strdup(const char *src)
{
    return src ? testctl_memcheck(strdup(src)) : NULL;
}

void *safe_memdup(const void *mem, size_t size)
{
    void *tmp = safe_malloc(size);
    if (tmp)
        memcpy(tmp, mem, size);
    return tmp;
}

void *testctl_memcheck(void *mem)
{
    if (unlikely(!mem)) {
        fprintf(stderr, "Fatal Error: %s\n", strerror(errno));
        exit(1);
    }

    return mem;
}

struct testctl_location {
    char *file;
    char *func;
    size_t line;
};

void testctl_location_init(struct testctl_location *loc, const char *file,
                           size_t line, const char *func)
{
    memset(loc, 0, sizeof(*loc));
    loc->file = safe_strdup(file);
    loc->func = safe_strdup(func);
    loc->line = line;
}

void testctl_location_destroy(struct testctl_location *loc)
{
    free(loc->file);
    free(loc->func);
}

struct testctl_test {
    struct list_node list;
    struct testctl_location loc;
    void (*func)();
    char *name;
    bool grouped;
    // bool skip;
    // bool only;
};

void testctl_test_init(struct testctl_test *test, void (*func)(),
                       const char *name, const char *file, size_t line,
                       const char *fname)
{
    memset(test, 0, sizeof(*test));

    test->func = func;
    test->name = safe_strdup(name);

    list_node_init(&test->list);
    testctl_location_init(&test->loc, file, line, fname);
}

void testctl_test_destroy(struct testctl_test *test)
{
    free(test->name);
    testctl_location_destroy(&test->loc);
}

struct testctl_group {
    struct list_node list;
    struct testctl_location loc;
    struct testctl_test **tests;
    size_t size;
    char *name;
};

void testctl_group_init(struct testctl_group *group,
                        struct testctl_test **tests, size_t size,
                        const char *name, const char *file, size_t line,
                        const char *fname)
{
    memset(group, 0, sizeof(*group));

    group->size = size;
    group->name = safe_strdup(name);
    group->tests = safe_memdup(tests, size * sizeof(*tests));

    testctl_location_init(&group->loc, file, line, fname);
    list_node_init(&group->list);
}

void testctl_group_destroy(struct testctl_group *group)
{
    free(group->name);
    free(group->tests);
    testctl_location_destroy(&group->loc);
}

struct testctl_group_handle {
    struct testctl_group *group;
    clock_t start;
    clock_t end;
};

void testctl_group_handle_init(struct testctl_group_handle *handle,
                               struct testctl_group *group)
{
    memset(handle, 0, sizeof(*handle));
    handle->group = group;
}

void testctl_group_handle_destroy(struct testctl_group_handle *handle)
{
}

struct testctl_test_handle {
    struct testctl_test *test;
    struct list_node resources;
    struct list_node captures;
    clock_t start;
    clock_t end;

    bool failed;
    struct testctl_location failpos;

    jmp_buf jmp;
};

void testctl_test_handle_init(struct testctl_test_handle *handle,
                              struct testctl_test *test)
{
    memset(handle, 0, sizeof(*handle));

    handle->test = test;

    list_init(&handle->resources);
    list_init(&handle->captures);
}

void testctl_test_handle_destroy(struct testctl_test_handle *handle)
{
    struct testctl_capture *cap, *tmpcap;
    struct testctl_resource_handle *res, *tmpres;

    list_for_each_entry_safe (cap, tmpcap, handle->captures, list) {
        testctl_capture_destroy(cap);
    }

    list_for_each_entry_safe (res, tmpres, handle->resources, list) {
        testctl_resource_handle_destroy(res);
    }
}

struct testctl_capture {
    struct list_node list;
    void *value;
    size_t size;
    char *expr;
    struct testctl_location loc;
    enum __testctl_type type;
};

void testctl_capture_init(struct testctl_capture *cap, void *val, size_t size,
                          enum __testctl_type type, const char *expr,
                          const char *file, size_t line, const char *fname)
{
    memset(cap, 0, sizeof(*cap));

    cap->val = safe_memdup(val, size);
    cap->size = size;
    cap->type = type;
    cap->expr = safe_strdup(expr);

    testctl_location_init(&cap->loc, file, line, fname);
    list_node_init(&cap->list);
}

void testctl_capture_destroy(struct testctl_capture *cap)
{
    free(cap->val);
    free(cap->expr);
    testctl_location_destroy(&cap->loc);
}

struct testctl_resource_handle {
    struct list_node list;
    struct testctl_resource resource;
    struct testctl_location loc;
    void *data;
};

void *testctl_resource_handle_init(struct testctl_resource_handle *handle,
                                   const struct testctl_resource *resource,
                                   void *data, const char *file, size_t line,
                                   const char *fname)
{
    memset(handle, 0, sizeof(*handle));

    handle->data = resource->create ? resource->create(data) : data;
    handle->resource = *resource;

    testctl_location_init(&handle->loc, file, line, fname);
    list_node_init(&handle->list);

    return handle->data;
}

void testctl_resource_handle_destroy(struct testctl_resource_handle *handle)
{
    if (handle->resource.destroy)
        handle->resource.destroy(handle->data);
    testctl_location_destroy(&handle->loc);
}

#endif
