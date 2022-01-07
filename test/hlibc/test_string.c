/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <hlibc/string.h>
#include <testlib/testlib.h>

struct encoding_test_case {
    const char *plain;
    const char *hex;
    const char *b64;
    const char *b64_nopad;
};

struct encoding_test_case encoding_test_cases[] = {
    {
        .plain = "",
        .hex = "",
        .b64 = "",
        .b64_nopad = "",
    },
    {
        .plain = "f",
        .hex = "66",
        .b64 = "Zg==",
        .b64_nopad = "Zg",
    },
    {
        .plain = "fo",
        .hex = "666F",
        .b64 = "Zm8=",
        .b64_nopad = "Zm8",
    },
    {
        .plain = "foo",
        .hex = "666F6F",
        .b64 = "Zm9v",
        .b64_nopad = "Zm9v",
    },
    {
        .plain = "foob",
        .hex = "666F6F62",
        .b64 = "Zm9vYg==",
        .b64_nopad = "Zm9vYg",
    },
    {
        .plain = "fooba",
        .hex = "666F6F6261",
        .b64 = "Zm9vYmE=",
        .b64_nopad = "Zm9vYmE",
    },
    {
        .plain = "foobar",
        .hex = "666F6F626172",
        .b64 = "Zm9vYmFy",
        .b64_nopad = "Zm9vYmFy",
    },
};

TEST(test_b64_encode)
{
    struct encoding_test_case *tc;
    for_each (tc, encoding_test_cases) {
        char out[256] = { 0 };
        b64_encode(tc->plain, strlen(tc->plain), out);
        ASSERT_STR_EQ(out, tc->b64);
    }
}

TEST(test_b64_decode)
{
    struct encoding_test_case *tc;


    /* With padding */
    for_each (tc, encoding_test_cases) {
        char out[256] = { 0 };
        int ret = b64_decode(tc->b64, out, array_size(out));

        ASSERT_NOT(ret);
        ASSERT_STR_EQ(tc->plain, out);
    }

    /* Without padding */
    for_each (tc, encoding_test_cases) {
        char out[256] = { 0 };
        int ret = b64_decode(tc->b64_nopad, out, array_size(out));

        ASSERT_NOT(ret);
        ASSERT_STR_EQ(tc->plain, out);
    }

    { /* Invalid input */
        char out[256] = { 0 };
        char *invalid[] = { "a", "a=", "a==", "a===", "=", "(", "@", "ab==0" };
        char **input;

        for_each(input, invalid) {
            int ret = b64_decode(*input, out, array_size(out));
            ASSERT_EQ(ret, EINVAL);
        }
    }

    /* Short output buffer */
    for_each (tc, encoding_test_cases) {
        char out[256] = { 0 };
        int len = strlen(tc->plain);
        int ret;

        for (int i = 0; i < len; i++) {
            ret = b64_decode(tc->b64, out, i);
            ASSERT_EQ(ret, ENOMEM);

            ret = b64_decode(tc->b64_nopad, out, i);
            ASSERT_EQ(ret, ENOMEM);
        }

        ret = b64_decode(tc->b64, out, len);
        ASSERT_NOT(ret);

        ret = b64_decode(tc->b64_nopad, out, len);
        ASSERT_NOT(ret);
    }
}


TEST(test_hex_encode)
{
    struct encoding_test_case *tc;

    for_each (tc, encoding_test_cases) {
        char out[256] = { 0 };

        hex_encode(tc->plain, strlen(tc->plain), out);
        ASSERT_STR_EQ(out, tc->hex);
    }
}

TEST(test_hex_decode)
{
    struct encoding_test_case *tc;

    for_each (tc, encoding_test_cases) {
        char out[256] = { 0 };
        int ret = hex_decode(tc->hex, out, array_size(out));

        ASSERT_NOT(ret);
        ASSERT_STR_EQ(out, tc->plain);
    }

    { /* testing invalids */
        char out[256] = { 0 };
        char *invalid[] = { "a", "x", "#", "abcde", "abcdefg" };
        char **input;

        for_each(input, invalid) {
            int ret = hex_decode(*input, out, array_size(out));
            ASSERT_EQ(ret, EINVAL);
        }
    }

    /* testing small output buffer */
    for_each (tc, encoding_test_cases) {
        char out[256] = { 0 };
        int len = strlen(tc->plain);
        int ret;

        for (int i = 0; i < len; i++) {
            ret = hex_decode(tc->hex, out, i);
            ASSERT_EQ(ret, ENOMEM);
        }

        ret = hex_decode(tc->hex, out, len);
        ASSERT_NOT(ret);
    }
}
