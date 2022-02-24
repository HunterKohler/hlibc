/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <hlibc/string.h>
#include <hlibc/crypto/md5.h>
#include <htest/htest.h>

struct md5_test_case {
    const char *plain;
    const char *hash;
};

struct md5_test_case md5_test_vector[] = {
    {
        .plain = "",
        .hash = "D41D8CD98F00B204E9800998ECF8427E",
    },
    {
        .plain = "a",
        .hash = "0CC175B9C0F1B6A831C399E269772661",
    },
    {
        .plain = "abc",
        .hash = "900150983CD24FB0D6963F7D28E17F72",
    },
    {
        .plain = "message digest",
        .hash = "F96B697D7CB7938D525A2F31AAF161D0",
    },
    {
        .plain = "abcdefghijklmnopqrstuvwxyz",
        .hash = "C3FCD3D76192E4007DFB496CCA67E13B",
    },
    {
        .plain =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
        .hash = "D174AB98D277D9F5A5611C2C9F419D9F",
    },
    {
        .plain =
            "12345678901234567890123456789012345678901234567890123456789012345"
            "678901234567890",
        .hash = "57EDF4A22BE3C955AC49DA2E2107B67A",
    },
};

void test_md5(struct htest *test)
{
    struct md5_context ctx;
    uint8_t result[MD5_DIGEST_LEN];
    char result_hex[MD5_DIGEST_LEN * 2 + 1];

    for (int i = 0; i < array_size(md5_test_vector); i++) {
        const char *plain = md5_test_vector[i].plain;
        const char *hash = md5_test_vector[i].hash;

        md5_init(&ctx);
        md5_update(&ctx, plain, strlen(plain));
        md5_finalize(&ctx, result);

        hex_encode(result, sizeof(result), result_hex);
        HTEST_ASSERT_STR_EQ(test, result_hex, hash);
    }
}

struct htest_unit md5_unit_tests[] = {
    HTEST_UNIT(test_md5),
    {},
};

struct htest_suite md5_test_suite = {
    .name = "MD5 test suite",
    .units = md5_unit_tests,
};

HTEST_DECLARE_SUITES(&md5_test_suite);
