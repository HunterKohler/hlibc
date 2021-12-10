#include <hlibc/string.h>

#include <assert.h>
#include <stdio.h>
#include <unistd.h>

struct test_case_encoding {
    size_t plain_size;
    const char *plain;

    size_t hex_size;
    const char *hex;

    size_t b64_size;
    const char *b64;
};

struct test_case_encoding test_vector[] = {
    { 0, "", 0, "", 0, "" },
    { 1, "f", 2, "66", 4, "Zg==" },
    { 2, "fo", 4, "666F", 4, "Zm8=" },
    { 3, "foo", 6, "666F6F", 4, "Zm9v" },
    { 4, "foob", 8, "666F6F62", 8, "Zm9vYg==" },
    { 5, "fooba", 10, "666F6F6261", 8, "Zm9vYmE=" },
    { 6, "foobar", 12, "666F6F626172", 8, "Zm9vYmFy" },
};

void test_b64_encode(struct test_case_encoding *tc)
{
    char out[1024] = { 0 };

    b64_encode(tc->plain, tc->plain_size, out);
    assert(!memcmp(out, tc->b64, tc->b64_size));
}

void test_b64_decode(struct test_case_encoding *tc)
{
    char buf[1024] = { 0 };

    assert(!b64_decode(tc->b64, buf));
    assert(!memcmp(buf, tc->plain, tc->plain_size));
}

void test_hex_encode(struct test_case_encoding *tc)
{
    char out[1024] = { 0 };

    hex_encode(tc->plain, tc->plain_size, out);
    assert(!memcmp(out, tc->hex, tc->hex_size));
}

void test_hex_decode(struct test_case_encoding *tc)
{
    char out[1024] = { 0 };

    assert(!hex_decode(tc->hex, out));
    assert(!memcmp(out, tc->plain, tc->plain_size));
}

// void test_hex_decode_error()
// {
// }

// void test_b64_decode_error()
// {
// }

int main()
{
    for (int i = 0; i < ARRAY_SIZE(test_vector); i++) {
        test_hex_encode(test_vector + i);
        test_hex_decode(test_vector + i);
        test_b64_encode(test_vector + i);
        test_b64_decode(test_vector + i);
    }

    // test_hex_decode_error();
    // test_b64_decode_error();

    printf("Success\n");
}
