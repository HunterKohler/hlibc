/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <testctl/testctl.h>
#include <hlibc/bit.h>
#include <hlibc/crypto/murmur.h>

TEST(test_murmurhash3_x86_32)
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
    struct test_case *tc;

    for_each (tc, test_vector) {
        murmurhash3_x86_32(tc->input, tc->len, tc->seed, &out);
        ASSERT_EQ(le32_to_cpu(out), tc->expected);
    }
}
