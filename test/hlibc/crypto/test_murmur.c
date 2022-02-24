/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <hlibc/bit.h>
#include <hlibc/crypto/murmur.h>
#include <htest/htest.h>

void test_murmurhash3_x86_32(struct htest *test)
{
    struct test_case {
        const char *input;
        size_t len;
        uint32_t seed;
        uint32_t expected;
    };

    struct test_case test_vector[] = {
        { "", 0, 0, 0 },
        { "", 0, 1, 0x514E28B7 },
        { "", 0, 0xFFFFFFFF, 0x81F16F39 },
        { "\xff\xff\xff\xff", 4, 0, 0x76293B50 },
        { "\x21\x43\x65\x87", 4, 0, 0xF55B516B },
        { "\x21\x43\x65\x87", 4, 0x5082EDEE, 0x2362F9DE },
        { "\x21\x43\x65", 3, 0, 0x7E4A8634 },
        { "\x21\x43", 2, 0, 0xA0F7B07A },
        { "\x21", 1, 0, 0x72661CF4 },
        { "\x00\x00\x00\x00", 4, 0, 0x2362F9DE },
        { "\x00\x00\x00", 3, 0, 0x85F0B427 },
        { "\x00\x00", 2, 0, 0x30F4C306 },
        { "\x00", 1, 0, 0x514E28B7 },
        { "Hello, world!", 13, 1234, 0xFAF6CDB3 },
        { "Hello, world!", 13, 4321, 0xBF505788 },
    };

    uint32_t out;

    for (int i = 0; i < array_size(test_vector); i++) {
        murmurhash3_x86_32(test_vector[i].input, test_vector[i].len,
                           test_vector[i].seed, &out);

        HTEST_ASSERT_EQ(test, le32_to_cpu(out), test_vector[i].expected);
    }
}

struct htest_unit murmur_test_units[] = {
    HTEST_UNIT(test_murmurhash3_x86_32),
    {},
};

struct htest_suite murmur_test_suite = {
    .name = "murmurhash test suite",
    .units = murmur_test_units,
};

HTEST_DECLARE_SUITES(&murmur_test_suite);
