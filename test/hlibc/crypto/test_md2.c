/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <hlibc/string.h>
#include <hlibc/crypto/md2.h>
#include <htest/htest.h>

struct md2_test_case {
    const char *plain;
    const char *hash;
};

struct md2_test_case md2_test_vector[] = {
    {
        .plain = "",
        .hash = "8350E5A3E24C153DF2275C9F80692773",
    },
    {
        .plain = "a",
        .hash = "32EC01EC4A6DAC72C0AB96FB34C0B5D1",
    },
    {
        .plain = "abc",
        .hash = "DA853B0D3F88D99B30283A69E6DED6BB",
    },
    {
        .plain = "message digest",
        .hash = "AB4F496BFB2A530B219FF33031FE06B0",
    },
    {
        .plain = "abcdefghijklmnopqrstuvwxyz",
        .hash = "4E8DDFF3650292AB5A4108C3AA47940B",
    },
    {
        .plain =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
        .hash = "DA33DEF2A42DF13975352846C30338CD",
    },
    {
        .plain =
            "12345678901234567890123456789012345678901234567890123456789012345"
            "678901234567890",
        .hash = "D5976F79D83D3A0DC9806C3C66F3EFD8",
    },
};

void test_md2(struct htest *test)
{
    struct md2_context ctx;
    uint8_t result[MD2_DIGEST_LEN];
    char result_hex[MD2_DIGEST_LEN * 2 + 1];

    for (int i = 0; i < array_size(md2_test_vector); i++) {
        const char *plain = md2_test_vector[i].plain;
        const char *hash = md2_test_vector[i].hash;

        md2_init(&ctx);
        md2_update(&ctx, plain, strlen(plain));
        md2_finalize(&ctx, result);

        hex_encode(result, sizeof(result), result_hex);
        HTEST_ASSERT_STR_EQ(test, result_hex, hash);
    }
}

struct htest_unit md2_unit_tests[] = {
    HTEST_UNIT(test_md2),
    {},
};

struct htest_suite md2_test_suite = {
    .name = "md2 test suite",
    .units = md2_unit_tests,
};

HTEST_DECLARE_SUITES(&md2_test_suite);
