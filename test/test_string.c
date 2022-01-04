/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <hlibc/string.h>

#include <assert.h>
#include <stdio.h>
#include <unistd.h>

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

void test_b64_encode()
{
    for_each (tc, encoding_test_cases) {
        char out[256] = { 0 };

        b64_encode(tc->plain, strlen(tc->plain), out);

        assert(!strcmp(out, tc->b64));
    }
}

void test_b64_decode()
{
    /* testing with with padding */
    for_each (tc, encoding_test_cases) {
        char out[256] = { 0 };

        int ret = b64_decode(tc->b64, out, array_size(out));

        assert(!ret);
        assert(!strcmp(tc->plain, out));
    }

    /* testing with no padding */
    for_each (tc, encoding_test_cases) {
        char out[256] = { 0 };

        int ret = b64_decode(tc->b64_nopad, out, array_size(out));

        assert(!ret);
        assert(!strcmp(tc->plain, out));
    }

    for_each (tc, encoding_test_cases) {
        char out[256] = { 0 };

        int ret = b64_decode(tc->b64_nopad, out, array_size(out));

        assert(!ret);
        assert(!strcmp(tc->plain, out));
    }

    { /* testing invalids */
        char out[256] = { 0 };
        char *invalid[] = { "a", "a=", "a==", "a===", "=", "(", "@", "ab==0" };

        for_each (input, invalid) {
            int ret = b64_decode("a", out, array_size(out));
            assert(ret == EINVAL);
        }
    }

    /* testing small output buffer */
    for_each (tc, encoding_test_cases) {
        char out[256] = { 0 };
        int len = strlen(tc->plain);
        int ret;

        for (int i = 0; i < len; i++) {
            ret = b64_decode(tc->b64, out, i);
            assert(ret == ENOMEM);

            ret = b64_decode(tc->b64_nopad, out, i);
            assert(ret == ENOMEM);
        }

        ret = b64_decode(tc->b64, out, len);
        assert(!ret);

        ret = b64_decode(tc->b64_nopad, out, len);
        assert(!ret);
    }
}

void test_hex_encode()
{
    for_each (tc, encoding_test_cases) {
        char out[256] = { 0 };

        hex_encode(tc->plain, strlen(tc->plain), out);

        assert(!strcmp(out, tc->hex));
    }
}

void test_hex_decode()
{
    for_each (tc, encoding_test_cases) {
        char out[256] = { 0 };

        int ret = hex_decode(tc->hex, out, array_size(out));

        assert(!ret);
        assert(!strcmp(out, tc->plain));
    }

    { /* testing invalids */
        char out[256] = { 0 };
        char *invalid[] = { "a", "x", "#", "abcde", "abcdefg" };

        for_each (input, invalid) {
            int ret = b64_decode("a", out, array_size(out));
            assert(ret == EINVAL);
        }
    }

    /* testing small output buffer */
    for_each (tc, encoding_test_cases) {
        char out[256] = { 0 };
        int len = strlen(tc->plain);
        int ret;

        for (int i = 0; i < len; i++) {
            ret = hex_decode(tc->hex, out, i);
            assert(ret == ENOMEM);
        }

        ret = hex_decode(tc->hex, out, len);
        assert(!ret);
    }
}

int main()
{
    test_hex_encode();
    test_hex_decode();
    test_b64_encode();
    test_b64_decode();
}
