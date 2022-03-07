#include <hlibc/string.h>
#include <htest/htest.h>

/*
 * Internal instance of a resource. Associated to some `struct htest`.
 */
struct htest_resource_handle {
    struct list_node list;

    void *data;
    void (*destroy)(void *);
};

int htest_init(struct htest *test, FILE *log, const char *desc, void *param)
{
    test->data = NULL;
    test->desc = desc;
    test->log = log;
    test->param = param;
    test->status = HTEST_PASSED;

    list_node_init(&test->resources);

    int error = pthread_mutex_init(&test->lock, NULL);
    if (error)
        return error;

    return 0;
}

void htest_destroy(struct htest *test)
{
    struct htest_resource_handle *res;
    struct htest_resource_handle *tmp;

    list_for_each_entry_safe (res, tmp, &test->resources, list) {
        if (res->destroy) {
            res->destroy(res->data);
        }

        list_rm(&res->list);
        free(res);
    }
}

void htest_lock(struct htest *test)
{
    int error = pthread_mutex_lock(&test->lock);
    if (error) {
        htest_fail(test, "Internal Error: Failed to lock test mutex: %s",
                   strerror(error));
    }
}

void htest_unlock(struct htest *test)
{
    int error = pthread_mutex_unlock(&test->lock);
    if (error) {
        htest_fail(test, "Internal Error: Failed to unlock test mutex: %s",
                   strerror(error));
    }
}

void *htest_create_resource(struct htest *test, struct htest_resource *res,
                            void *arg)
{
    htest_lock(test);

    struct htest_resource_handle *handle = malloc(sizeof(*handle));
    if (!handle) {
        htest_fail(test, "Internal Error: Failed to allocate memory");
    }

    handle->destroy = res->destroy;
    handle->data = res->create(arg);
    list_node_init(&handle->list);
    list_add(&handle->list, &test->resources);

    htest_unlock(test);
    return handle->data;
}

int htest_run_suite(struct htest_suite *suite, struct htest_log *log)
{
    struct htest test;
    struct htest_stats stats = {};

    fprintf(log->info, "Suite: %s\n", suite->name);

    for (struct htest_unit *unit = suite->units; unit->run_test; unit++) {
        if (unit->gen_params) {
            fprintf("Test: %s\n", unit->name);

            char desc[256];
            void *param = NULL;
            int index = 0;

            desc[0] = 0;

            while ((param = unit->gen_params(param, desc))) {
                if (!desc[0]) {
                    snprintf(desc, array_size(desc), "%d", index);
                }

                htest_init(&test, log, desc, param);

                if (!setjmp(test.return_jmp))
                    unit->run_test(&test);

                switch (test.status) {
                case HTEST_PASSED:
                    break;
                case HTEST_SKIPPED:
                    break;
                case HTEST_FAILED:
                    break;
                }

                htest_destroy(&test);

                desc[0] = 0;
                index++;
            }
        } else {
            htest_init(&test, log, unit->name, NULL);
            htest_run(unit->run_test, &test);

            switch (test.status) {
            case HTEST_PASSED:
                break;
            case HTEST_SKIPPED:
                break;
            case HTEST_FAILED:
                break;
            }

            htest_destroy(&test);
        }
    }
}

static noreturn void htest_return(struct htest *test, enum htest_status status)
{
    test->status = status;
    longjmp(test->return_jmp, 1);
}

noreturn void htest_pass(struct htest *test, const char *fmt, ...)
{
    if (test->log.info && fmt) {
        va_list ap;
        va_start(ap, fmt);
        vfprintf(test->log.info, fmt, ap);
        va_end(ap);
    }

    htest_return(test, HTEST_PASSED);
}

noreturn void htest_skip(struct htest *test, const char *fmt, ...)
{
    if (test->log.info && fmt) {
        va_list ap;
        va_start(ap, fmt);
        vfprintf(test->log.info, fmt, ap);
        va_end(ap);
    }

    htest_return(test, HTEST_SKIPPED);
}

noreturn void htest_fail(struct htest *test, const char *fmt, ...)
{
    if (test->log.info && fmt) {
        va_list ap;
        va_start(ap, fmt);
        vfprintf(test->log.info, fmt, ap);
        va_end(ap);
    }

    htest_return(test, HTEST_FAILED);
}

void __htest_assert(const struct htest_assertion *assertion)
{
    if (unlikely(!assertion->passed)) {
        htest_info(assertion->test, "# Assertion Failed: test %s at %s:%d:%s:",
                   assertion->test->name, assertion->file, assertion->line,
                   assertion->func);

        htest_info(assertion->test, "# %s", assertion->test,
                   assertion->message);

        htest_fail(assertion->test, NULL);
    }
}

void htest_info(struct htest *test, const char *fmt, ...)
{
    va_list ap;

    if (test->log.info) {
        va_start(ap, fmt);
        vfprintf(test->log.info, fmt, ap);
        va_end(ap);
    }
}

void htest_warn(struct htest *test, const char *fmt, ...)
{
    va_list ap;

    if (test->log.warn) {
        va_start(ap, fmt);
        vfprintf(test->log.warn, fmt, ap);
        va_end(ap);
    }
}

void htest_error(struct htest *test, const char *fmt, ...)
{
    va_list ap;

    if (test->log.error) {
        va_start(ap, fmt);
        vfprintf(test->log.error, fmt, ap);
        va_end(ap);
    }
}

struct list_node __htest_global_suite_list =
    LIST_NODE_INIT(__htest_global_suite_list);

void __htest_declare_suites(struct htest_suite **suites, size_t n)
{
    for (int i = 0; i < n; i++) {
        list_add(&suites[i]->list, &__htest_global_suite_list);
    }
}

void htest_run_global_suites()
{
    struct htest_suite *suite;

    list_for_each_entry (suite, &__htest_global_suite_list, list) {
        htest_run_suite(suite, stdout);
    }
}

void *htest_memory_resource_create(void *arg)
{
    return malloc(*(size_t *)arg);
}

void htest_memory_resource_destroy(void *arg)
{
    free(arg);
}

struct htest_resource htest_memory_resource = {
    .create = htest_memory_resource_create,
    .destroy = htest_memory_resource_destroy,
};

void *htest_malloc(struct htest *test, size_t size)
{
    return htest_create_resource(test, &htest_memory_resource,
                                 &(size_t){ size });
}

struct htest_fstream_resource_params {
    const char *restrict pathname;
    const char *restrict mode;
};

void *htest_fstream_resource_create(void *arg)
{
    struct htest_fstream_resource_params *params = arg;
    return fopen(params->pathname, params->mode);
}

void htest_fstream_resource_destroy(void *arg)
{
    fclose((FILE *)arg);
}

struct htest_resource htest_fstream_resource = {
    .create = htest_fstream_resource_create,
    .destroy = htest_fstream_resource_destroy,
};

FILE *htest_fopen(struct htest *test, const char *restrict pathname,
                  const char *restrict mode)
{
    return htest_create_resource(
        test, &htest_fstream_resource,
        &(struct htest_fstream_resource_params){ pathname, mode });
}

void *htest_string_resource_create(void *arg)
{
    return stralloc(*(size_t *)arg);
}

void htest_string_resource_destroy(void *arg)
{
    free(arg);
}

struct htest_resource htest_string_resource = {
    .create = htest_string_resource_create,
    .destroy = htest_string_resource_destroy,
};

void *htest_stralloc(struct htest *test, size_t size)
{
    return htest_create_resource(test, &htest_string_resource,
                                 &(size_t){ size });
}
