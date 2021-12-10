/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <hlibc/crypto/des.h>

#include <stdio.h>
#include <assert.h>

static uint64_t schedule_test_key = 0x93C0EEB98CEF5C;
static uint64_t schedule_test_expected[] = {
    0x2781DD7319DEB9, 0x4F03BAE633BD72, 0x3C0EEB98CEF5C9, 0xF03BAE433BD726,
    0xC0EEB93CEF5C98, 0x03BAE4F3BD7263, 0x0EEB93CEF5C98C, 0x3BAE4F0BD72633,
    0x775C9E07AE4C67, 0xDD72781EB9319D, 0x75C9E07AE4C677, 0xD72781DB9319DE,
    0x5C9E077E4C677A, 0x72781DD9319DEB, 0xC9E07754C677AE, 0x93C0EEB98CEF5C,
};

void test_des_schedule()
{
    uint64_t schedule[16];
    des_schedule(schedule_test_key, schedule);

    for (int i = 0; i < 16; i++) {
        assert(schedule[i] == schedule_test_expected[i]);
    }
}

void test_des_left_shift()
{
    uint64_t k = schedule_test_key;

    for (int i = 0; i < 16; i++) {
        k = des_left_shift(k, i);
        assert(k == schedule_test_expected[i]);
    }
}

void test_des_PC_1()
{
    uint64_t k = 0xE7A1926FFE75BCA3;
    uint64_t expected = 0x00D739FB79D79584;

    k = des_PC_1(k);

    assert(k == expected);
}

void test_des_PC_2()
{
    uint64_t k = 0xD4A5D5F058FFD2;
    uint64_t expected = 0x00F941ADDC5CBC;

    k = des_PC_2(k);

    assert(k == expected);
}

void test_des_P()
{
    uint64_t x = 0x71BEBA57;
    uint64_t expected = 0x3772DBE6;

    x = des_P(x);

    assert(x == expected);
}

void test_des_E()
{
    uint64_t x = 0x0001AC31B3E;
    uint64_t expected = 0xF56068F69FC;

    x = des_E(x);

    assert(x == expected);
}

void test_des_IP()
{
    uint64_t x = 0x5E9D0E69E9093B5D;
    uint64_t expected = 0x99C387FA1258FF45;

    x = des_IP(x);

    assert(x == expected);
}

void test_des_IP_inv()
{
    uint64_t x = 0x5E9D0E69E9093B5D;
    uint64_t expected = 0xBB4C56FF5A89C390;

    x = des_IP_inv(x);

    assert(x == expected);
}

void test_des_single()
{
}

void test_des_S()
{
}

void test_des_f()
{
}

void test_des()
{
}

int main()
{
    // uint64_t input = 0x95F8A5E5DD31D900;
    // uint64_t expected = 0x8000000000000000;

    // printf("Block:    %.16llX\n"
    //        "Key:      %.16X\n"
    //        "Expected: %.16llX\n"
    //        "Recieved: %.16llX\n",
    //        input, 0, expected, des_single(input, 0));

    uint64_t input = 0x12C4FACC29FF33B9;
    uint64_t expect = 0xAA6C31A76EAE3537;

    printf("Block:   %llX\n"
           "Expect:  %llX\n"
           "Recieve: %llX\n",
           input, expect, des_IP(input));

    // test_des_IP();
    // test_des_IP_inv();
    // test_des_E();
    // test_des_P();
    // test_des_S();
    // test_des_left_shift();
    // test_des_PC_1();
    // test_des_PC_2();
    // test_des_f();
    // test_des_single();
    // test_des_schedule();
    // test_des();
}
