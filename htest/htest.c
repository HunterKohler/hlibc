#include <htest/htest.h>

/*
 * Internal instance of a resource. Associated to some `struct htest`.
 */
struct htest_resource_handle {
    struct list_node list;
    void *data;
    void (*destroy)(void *);
};

int htest_init(struct htest *test, const char *name, FILE *global_log,
               FILE *test_log)
{
    test->data = NULL;
    test->status = HTEST_PASSED;
    test->name = name;
    test->global_log = global_log;
    test->test_log = test_log;

    list_node_init(&test->resources);
    return 0;
}

void htest_destroy(struct htest *test)
{
    struct htest_resource_handle *res;
    struct htest_resource_handle *tmp;

    list_for_each_entry_safe (res, tmp, &test->resources, list) {
        res->destroy(res->data);
        list_rm(&res->list);
        free(res);
    }
}

void *htest_create_resource(struct htest *test, struct htest_resource *res,
                            void *arg)
{
    struct htest_resource_handle *handle = malloc(sizeof(*handle));

    if (!handle) {
        htest_fail(test, "Internal error: "
                         "Could not allocate memory to track resource.");
    }

    handle->destroy = res->destroy;
    handle->data = res->create(arg);
    list_node_init(&handle->list);
    list_add(&handle->list, &test->resources);

    return handle->data;
}

int htest_run_suite(struct htest_suite *suite, FILE *global_log, FILE *test_log)
{
    struct htest_stats stats = {};
    struct htest test = {};

    htest_log(HTEST_INFO, suite, "# Suite: %s", suite->name);

    for (struct htest_unit *unit = suite->units; unit->run; unit++) {
        size_t test_num = unit - suite->units + 1;
        stats.total++;

        if (htest_init(&test, unit->name, global_log, test_log)) {
            htest_log(HTEST_ERROR, suite, "Failed to initialize test case: %s",
                      unit->name);
            continue;
        }

        if (suite->skip) {
            test.status = HTEST_SKIPPED;
        } else {
            if (suite->init)
                suite->init(&test);

            if (!setjmp(test.return_jmp))
                unit->run(&test);

            if (suite->destroy)
                suite->destroy(&test);
        }

        switch (test.status) {
        case HTEST_SKIPPED:
            stats.skipped++;
            htest_log(HTEST_INFO, &test, "ok %zu - %s # SKIP%s%s", test_num,
                      test.name, test.status_msg ? " " : "",
                      test.status_msg ? test.status_msg : "");
            break;
        case HTEST_PASSED:
            stats.passed++;
            htest_log(HTEST_INFO, &test, "ok %zu - %s%s%s", test_num, test.name,
                      test.status_msg ? " # " : "",
                      test.status_msg ? test.status_msg : "");
            break;
        case HTEST_FAILED:
            stats.failed++;
            htest_log(HTEST_INFO, &test, "not ok %zu - %s%s%s", test_num,
                      test.name, test.status_msg ? " # " : "",
                      test.status_msg ? test.status_msg : "");
            break;
        }

        htest_destroy(&test);
    }

    if (!stats.total)
        htest_log(HTEST_WARNING, suite, "# Warning: Test suite empty");

    htest_log(HTEST_INFO, suite,
              "# Results: passed: %zu, failed: %zu, skipped: %zu, total: %zu",
              stats.passed, stats.failed, stats.skipped, stats.total);

    return 0;
}

void htest_set_status(struct htest *test, enum htest_status status,
                      const char *msg)
{
    test->status = status;
    if (msg) {
        size_t len = strlen(msg);
        size_t max_len = HTEST_STATUS_MSG_SIZE - 1;

        if (len > max_len) {
            len = max_len;
            htest_warn(test,
                       "Status message too long: wrote %zu of %zu characters.",
                       max_len, len);
        }

        memcpy(test->status_msg, msg, len);
        test->status_msg[len] = 0;
    }
}

noreturn void htest_return(struct htest *test)
{
    longjmp(test->return_jmp, 1);
}

noreturn void htest_pass(struct htest *test, const char *msg)
{
    htest_set_status(test, HTEST_PASSED, msg);
    htest_return(test);
}

noreturn void htest_skip(struct htest *test, const char *msg)
{
    htest_set_status(test, HTEST_SKIPPED, msg);
    htest_return(test);
}

noreturn void htest_fail(struct htest *test, const char *msg)
{
    htest_set_status(test, HTEST_FAILED, msg);
    htest_return(test);
}

void __htest_assert(const struct htest_assertion *assertion)
{
    if (unlikely(!assertion->result)) {
        htest_log(
            HTEST_INFO, assertion->test,
            "# Assertion Failed: test %s at %s:%d:%s:", assertion->test->name,
            assertion->file, assertion->line, assertion->func);
        htest_log(HTEST_INFO, assertion->test, "# %s", assertion->test,
                  assertion->message);
        htest_fail(assertion->test, NULL);
    }
}

void __htest_log(enum htest_log_level level, FILE *global_log, FILE *test_log,
                 const char *fmt, ...)
{
    va_list ap;

    if (global_log) {
        va_start(ap, fmt);
        vfprintf(global_log, fmt, ap);
        va_end(ap);
    }

    if (test_log) {
        va_start(ap, fmt);
        vfprintf(test_log, fmt, ap);
        va_end(ap);
    }
}
