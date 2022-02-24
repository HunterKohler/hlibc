/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <hlibc/string.h>
#include <hlibc/crypto/md4.h>
#include <htest/htest.h>

struct md4_test_case {
    const char *plain;
    const char *hash;
};

struct md4_test_case md4_test_vector[] = {
    {
        .plain = "",
        .hash = "31D6CFE0D16AE931B73C59D7E0C089C0",
    },
    {
        .plain = "a",
        .hash = "BDE52CB31DE33E46245E05FBDBD6FB24",
    },
    {
        .plain = "abc",
        .hash = "A448017AAF21D8525FC10AE87AA6729D",
    },
    {
        .plain = "message digest",
        .hash = "D9130A8164549FE818874806E1C7014B",
    },
    {
        .plain = "abcdefghijklmnopqrstuvwxyz",
        .hash = "D79E1C308AA5BBCDEEA8ED63DF412DA9",
    },
    {
        .plain =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
        .hash = "043F8582F241DB351CE627E153E7F0E4",
    },
    {
        .plain =
            "12345678901234567890123456789012345678901234567890123456789012345"
            "678901234567890",
        .hash = "E33B4DDC9C38F2199C3E7B164FCC0536",
    },
};

void test_md4(struct htest *test)
{
    struct md4_context ctx;
    uint8_t result[MD4_DIGEST_LEN];
    char result_hex[MD4_DIGEST_LEN * 2 + 1];

    for (int i = 0; i < array_size(md4_test_vector); i++) {
        const char *plain = md4_test_vector[i].plain;
        const char *hash = md4_test_vector[i].hash;

        md4_init(&ctx);
        md4_update(&ctx, plain, strlen(plain));
        md4_finalize(&ctx, result);

        hex_encode(result, sizeof(result), result_hex);
        HTEST_ASSERT_STR_EQ(test, result_hex, hash);
    }
}

struct htest_unit md4_unit_tests[] = {
    HTEST_UNIT(test_md4),
    {},
};

struct htest_suite md4_test_suite = {
    .name = "MD4 test suite",
    .units = md4_unit_tests,
};

HTEST_DECLARE_SUITES(&md4_test_suite);
