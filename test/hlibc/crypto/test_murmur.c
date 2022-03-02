/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <hlibc/bit.h>
#include <hlibc/crypto/murmur.h>
#include <htest/htest.h>

struct test_case {
    uint8_t input[50];
    size_t size;
    uint32_t seed;
    uint32_t hash;
};

struct test_case test_vector[] = {
    {
        .input = {},
        .size = 0,
        .seed = 0,
        .hash = 0,
    },
    {
        .input = {},
        .size = 0,
        .seed = 1,
        .hash = 0x514E28B7,
    },
    {
        .input = {},
        .size = 0,
        .seed = 0xFFFFFFFF,
        .hash = 0x81F16F39,
    },
    {
        .input = { 0xFF, 0xFF, 0xFF, 0xFF },
        .size = 4,
        .seed = 0,
        .hash = 0x76293B50,
    },
    {
        .input = { 0x21, 0x43, 0x65, 0x87 },
        .size = 4,
        .seed = 0,
        .hash = 0xF55B516B,
    },
    {
        .input = { 0x21, 0x43, 0x65, 0x87 },
        .size = 4,
        .seed = 0x5082EDEE,
        .hash = 0x2362F9DE,
    },
    {
        .input = { 0x21, 0x43, 0x65 },
        .size = 3,
        .seed = 0,
        .hash = 0x7E4A8634,
    },
    {
        .input = { 0x21, 0x43 },
        .size = 2,
        .seed = 0,
        .hash = 0xA0F7B07A,
    },
    {
        .input = { 0x21 },
        .size = 1,
        .seed = 0,
        .hash = 0x72661CF4,
    },
    {
        .input = { 0x00, 0x00, 0x00, 0x00 },
        .size = 4,
        .seed = 0,
        .hash = 0x2362F9DE,
    },
    {
        .input = { 0x00, 0x00, 0x00 },
        .size = 3,
        .seed = 0,
        .hash = 0x85F0B427,
    },
    {
        .input = { 0x00, 0x00 },
        .size = 2,
        .seed = 0,
        .hash = 0x30F4C306,
    },
    {
        .input = { 0x00 },
        .size = 1,
        .seed = 0,
        .hash = 0x514E28B7,
    },
    {
        .input = {
            0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20, 0x77, 0x6F, 0x72,  0x6C,
            0x64, 0x21,
        },
        .size = 13,
        .seed = 1234,
        .hash = 0xFAF6CDB3,
    },
    {
        .input = {
            0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20, 0x77, 0x6F, 0x72, 0x6C,
            0x64, 0x21,
        },
        .size = 13,
        .seed = 4321,
        .hash = 0xBF505788,
    },
};

void test_murmurhash3_x86_32(struct htest *test)
{
    uint32_t out;

    for (int i = 0; i < array_size(test_vector); i++) {
        murmurhash3_x86_32(test_vector[i].input, test_vector[i].size,
                           test_vector[i].seed, &out);

        HTEST_ASSERT_EQ(test, le32_to_cpu(out), test_vector[i].hash);
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
