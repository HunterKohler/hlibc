/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <htest/htest.h>
#include <hlibc/string.h>

struct encoding_test_case {
    const char *plain;
    const char *hex;
    const char *b64;
    const char *b64_nopad;
};

const struct encoding_test_case encoding_test_cases[] = {
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

void test_b64_encode(struct htest *test)
{
    for (int i = 0; i < array_size(encoding_test_cases); i++) {
        const struct encoding_test_case *tc = encoding_test_cases + i;
        char out[256] = { 0 };

        b64_encode(tc->plain, strlen(tc->plain), out);
        HTEST_ASSERT_STR_EQ(test, out, tc->b64);
    }
}

void test_b64_decode(struct htest *test)
{
    /* With padding */
    for (int i = 0; i < array_size(encoding_test_cases); i++) {
        const struct encoding_test_case *tc = encoding_test_cases + i;
        char out[256] = { 0 };

        int ret = b64_decode(tc->b64, out, array_size(out));

        HTEST_ASSERT_NOT(test, ret);
        HTEST_ASSERT_STR_EQ(test, tc->plain, out);
    }

    /* Without padding */
    for (int i = 0; i < array_size(encoding_test_cases); i++) {
        const struct encoding_test_case *tc = encoding_test_cases + i;
        char out[256] = { 0 };

        int ret = b64_decode(tc->b64_nopad, out, array_size(out));

        HTEST_ASSERT_NOT(test, ret);
        HTEST_ASSERT_STR_EQ(test, tc->plain, out);
    }

    { /* Invalid input */
        char out[256] = { 0 };
        char *invalid[] = { "a", "a=", "abc#", "a==",  "a===",
                            "=", "(",  "@",    "ab==0" };

        for (int i = 0; i < array_size(invalid); i++) {
            int ret = b64_decode(invalid[i], out, array_size(out));
            HTEST_ASSERT_EQ(test, ret, EINVAL);
        }
    }

    /* Short output buffer */
    for (int i = 0; i < array_size(encoding_test_cases); i++) {
        const struct encoding_test_case *tc = encoding_test_cases + i;
        char out[256] = { 0 };
        int len = strlen(tc->plain);
        int ret;

        for (int i = 0; i < len; i++) {
            ret = b64_decode(tc->b64, out, i);
            HTEST_ASSERT_EQ(test, ret, ENOMEM);

            ret = b64_decode(tc->b64_nopad, out, i);
            HTEST_ASSERT_EQ(test, ret, ENOMEM);
        }

        ret = b64_decode(tc->b64, out, len);
        HTEST_ASSERT_NOT(test, ret);

        ret = b64_decode(tc->b64_nopad, out, len);
        HTEST_ASSERT_NOT(test, ret);
    }
}

void test_hex_encode(struct htest *test)
{
    for (int i = 0; i < array_size(encoding_test_cases); i++) {
        const struct encoding_test_case *tc = encoding_test_cases + i;
        char out[256] = { 0 };

        hex_encode(tc->plain, strlen(tc->plain), out);
        HTEST_ASSERT_STR_EQ(test, out, tc->hex);
    }
}

void test_hex_decode(struct htest *test)
{
    for (int i = 0; i < array_size(encoding_test_cases); i++) {
        const struct encoding_test_case *tc = encoding_test_cases + i;
        char out[256] = { 0 };
        int ret = hex_decode(tc->hex, out, array_size(out));

        HTEST_ASSERT_NOT(test, ret);
        HTEST_ASSERT_STR_EQ(test, out, tc->plain);
    }

    { /* testing invalids */
        char out[256] = { 0 };
        char *invalid[] = { "a", "x", "#", "abcde", "abcdefg" };

        for (int i = 0; i < array_size(invalid); i++) {
            int ret = hex_decode(invalid[i], out, array_size(out));
            HTEST_ASSERT_EQ(test, ret, EINVAL);
        }
    }

    /* testing small output buffer */
    for (int i = 0; i < array_size(encoding_test_cases); i++) {
        const struct encoding_test_case *tc = encoding_test_cases + i;
        char out[256] = { 0 };
        int len = strlen(tc->plain);
        int ret;

        for (int i = 0; i < len; i++) {
            ret = hex_decode(tc->hex, out, i);
            HTEST_ASSERT_EQ(test, ret, ENOMEM);
        }

        ret = hex_decode(tc->hex, out, len);
        HTEST_ASSERT_NOT(test, ret);
    }
}

struct htest_unit string_test_units[] = {
    HTEST_UNIT(test_b64_encode),
    HTEST_UNIT(test_b64_decode),
    HTEST_UNIT(test_hex_encode),
    HTEST_UNIT(test_hex_decode),
    {},
};

struct htest_suite string_test_suite = {
    .name = "string test suite",
    .units = string_test_units,
};

HTEST_DECLARE_SUITES(&string_test_suite);
