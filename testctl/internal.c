#include <testctl/api.h>
#include <testctl/internal.h>

struct testctl_test_handle active_test;
struct testctl_group_handle active_group;
struct list_node global_tests = LIST_NODE_INIT(global_tests);
struct list_node global_groups = LIST_NODE_INIT(global_groups);
struct testctl_group root_group;
jmp_buf test_return_jmp;

noreturn void testctl_main()
{
    struct testctl_group *group;
    struct testctl_test *test;

    int group_count = 0;
    int grouped = 0;
    list_for_each_entry (group, global_groups, list) {
        group_count++;

        for (int i = 0; i < group->size; i++) {
            grouped += !group->tests[i]->grouped;
            group->tests[i]->grouped = true;
        }
    }

    int test_count = 0;
    list_for_each_entry (test, global_tests, list) {
        test_count++;
    }

    int root_size = 0;
    struct testctl_test **root_tests =
        safe_malloc(sizeof(*root_tests) * (test_count - grouped));

    list_for_each_entry (test, global_tests, list) {
        if (!test->grouped) {
            root_tests[root_size++] = test;
        }
    }

    testctl_group_init(&root_group, root_tests, root_size, NULL, NULL, 0, NULL);
    list_add(&root_group.list, &global_groups)
    free(root_tests);

    list_for_each_entry (group, global_groups, list) {
        testctl_run_group(group);
    }
}

void testctl_run_group(struct testctl_group *group)
{
    struct testctl_test *test;
    for(int i = 0; i < group->size; i++) {
        test = group->tests + i;
        testctl_run_test(test);
    }
}

void testctl_run_test(struct testctl_test *test)
{
    struct testctl_init
}

// static size_t testctl_type_stringify(enum testctl_type_id type_id, void *value,
//                                      size_t value_size, char *dest, size_t size)
// {
//     switch (type_id) {
//     case TESTCTL_TYPE_ID_SHORT:
//         return snprintf(dest, size, "%hd", *(short *)value);
//     case TESTCTL_TYPE_ID_INT:
//         return snprintf(dest, size, "%d", *(int *)value);
//     case TESTCTL_TYPE_ID_LONG:
//         return snprintf(dest, size, "%ld", *(long *)value);
//     case TESTCTL_TYPE_ID_LLONG:
//         return snprintf(dest, size, "%lld", *(long long *)value);
//     case TESTCTL_TYPE_ID_USHORT:
//         return snprintf(dest, size, "%hu", *(unsigned short *)value);
//     case TESTCTL_TYPE_ID_UINT:
//         return snprintf(dest, size, "%u", *(unsigned int *)value);
//     case TESTCTL_TYPE_ID_ULONG:
//         return snprintf(dest, size, "%lu", *(unsigned long *)value);
//     case TESTCTL_TYPE_ID_ULLONG:
//         return snprintf(dest, size, "%llu", *(unsigned long long *)value);
//     case TESTCTL_TYPE_ID_CHAR:
//         return snprintf(dest, size, "%c", *(char *)value);
//     case TESTCTL_TYPE_ID_UCHAR:
//         return snprintf(dest, size, "%c", *(unsigned char *)value);
//     case TESTCTL_TYPE_ID_SCHAR:
//         return snprintf(dest, size, "%c", *(signed char *)value);
//     case TESTCTL_TYPE_ID_FLOAT:
//         return snprintf(dest, size, "%f", *(float *)value);
//     case TESTCTL_TYPE_ID_DOUBLE:
//         return snprintf(dest, size, "%lf", *(double *)value);
//     case TESTCTL_TYPE_ID_LDOUBLE:
//         return snprintf(dest, size, "%Lf", *(long double *)value);
//     case TESTCTL_TYPE_ID_BOOL:
//         return snprintf(dest, size, "%s", *(bool *)value ? "true" : "false");
//     case TESTCTL_TYPE_ID_PTR:
//         return snprintf(dest, size, "%p", *(void **)value);
//     case TESTCTL_TYPE_ID_STR:
//         return snprintf(dest, size, "%s", (char *)value);
//     case TESTCTL_TYPE_ID_MEM:
//         size_t ret = min(value_size * 2, size - 1) & ~1;
//         hex_encode(value, ret / 2, dest);
//         return strlen(dest);
//     default:
//         return snprintf(dest, size, "(cannot serialize type)");
//     }
// }

