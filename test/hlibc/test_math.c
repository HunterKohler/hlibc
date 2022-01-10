/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <testlib/testlib.h>
#include <hlibc/math.h>

TEST(test_ceil_log2)
{
    ASSERT_EQ(ceil_log2(1), 0);
    ASSERT_EQ(ceil_log2(10), 4);
    ASSERT_EQ(ceil_log2(16), 4);
    ASSERT_EQ(ceil_log2(1 << 30), 30);
    ASSERT_EQ(ceil_log2((1 << 30) - 1), 30);
}

TEST(test_floor_log2)
{
    ASSERT_EQ(floor_log2(1), 0);
    ASSERT_EQ(floor_log2(10), 3);
    ASSERT_EQ(floor_log2(16), 4);
    ASSERT_EQ(floor_log2(1 << 30), 30);
    ASSERT_EQ(floor_log2((1 << 30) - 1), 29);
}

TEST(test_gcd)
{
    ASSERT_EQ(gcd(961596251, 761316822), 55433);
    ASSERT_EQ(gcd(761316822, 961596251), 55433);
    ASSERT_EQ(gcd(-961596251, 761316822), 55433);
    ASSERT_EQ(gcd(961596251, -761316822), 55433);
    ASSERT_EQ(gcd(-961596251, -761316822), 55433);
    ASSERT_EQ(gcd(12345, 0), 12345);
    ASSERT_EQ(gcd(0, 12345), 12345);
    ASSERT_EQ(gcd(0, 0), 0);
}

TEST(test_lcm)
{
    ASSERT_EQ(lcm(95951, 7622), 731338522);
    ASSERT_EQ(lcm(7622, 95951), 731338522);
    ASSERT_EQ(lcm(-95951, 7622), 731338522);
    ASSERT_EQ(lcm(95951, -7622), 731338522);
    ASSERT_EQ(lcm(-95951, -7622), 731338522);
    ASSERT_EQ(lcm(0, 7622), 0);
    ASSERT_EQ(lcm(7622, 0), 0);
    ASSERT_EQ(lcm(0, 0), 0);
}
