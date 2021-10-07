#ifndef HLIBC_TEST_H_
#define HLIBC_TEST_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <hlibc/def.h>

#define UNIT_TEST_COPY_STR false

struct unit_test {
    void (*fn)();

    const char *name;
    const char *directive;
    const char *directive_desc;
};

struct unit_test *unit_test_new(const char *name,
                                const char *directive,
                                const char *directive_desc)
{
    struct unit_test *ret = malloc(sizeof(*ret));

    if (!ret) {
        return NULL;
    } else if (!(ret->name = strdup(name)) ||
               !(ret->directive = strdup(directive)) ||
               !(ret->directive_desc = strdup(directive_desc))) {
        unit_test_free(ret);
        return NULL;
    }

    return ret;
}

void unit_test_free(struct unit_test *test)
{
    free(test->name);
    free(test->directive);
    free(test->directive_desc);
    free(test);
}

enum unit_test_result_status { ACTIVE, PASSED, FAILED };

struct unit_test_result {
    size_t id;
    clock_t duration;

    enum unit_test_result_status status;
};

#endif