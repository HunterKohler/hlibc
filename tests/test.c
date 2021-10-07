#include <errno.h>
#include <hlibc/io.h>
#include <hlibc/test.h>

void test_frewind()
{
    FILE *f = fopen("tests/fixtures/", "r");

    fseek(f, 25, SEEK_CUR);

    assert(ftell(f) == 25);

    int a = ftell(f);
    int b = frewind(f);
    int c = ftell(f);

    // fclose(f);

    int d = frewind(f);

    assert(a == 25);
    assert(b == 0);
    assert(c == 0);
    assert(d != 0);
}

int main()
{
    struct unit_test *test =
        unit_test_new(test_frewind, "frewind()", NULL, NULL);

    struct unit_test_result *result = unit_test_run(test);

    printf("%s\n", unit_test_result_string(result));
}
