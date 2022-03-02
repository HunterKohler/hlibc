#include <hlibc/inet.h>
#include <htest/htest.h>

struct ipv4_test_case {
    char str[16];
    uint8_t bin[4];
};

const struct ipv4_test_case ipv4_test_vec[] = {
    { .str = "0.0.0.0", .bin = { 0, 0, 0, 0 } },
    { .str = "1.1.1.1", .bin = { 1, 1, 1, 1 } },
    { .str = "11.11.11.11", .bin = { 11, 11, 11, 11 } },
    { .str = "1.2.3.4", .bin = { 1, 2, 3, 4 } },
    { .str = "57.112.99.212", .bin = { 57, 112, 99, 212 } },
    { .str = "255.255.255.255", .bin = { 255, 255, 255, 255 } },
};

const char *ipv4_test_vec_inval[] = {
    "0",
    "0.",
    "0.0",
    "0.0.",
    "0.0.0",
    "00.0.0.0",
    "0.00.0.0",
    "0.0.00.0",
    "0.0.0.00",
    ".0.0.0",
    "0..0.0",
    "0.0..0",
    "0.0.0.",
    "256.255.255.255",
    "255.256.255.255",
    "255.255.256.255",
    "255.255.255.256",
};

void test_ipv4_to_str(struct htest *test)
{
    char dest[16];
    for (int i = 0; i < array_size(ipv4_test_vec); i++) {
        ipv4_to_str(ipv4_test_vec[i].bin, dest);
        HTEST_ASSERT_STR_EQ(test, dest, ipv4_test_vec[i].str);
    }
}

void test_ipv4_to_bin(struct htest *test)
{
    uint8_t dest[16];

    for (int i = 0; i < array_size(ipv4_test_vec); i++) {
        int ret = ipv4_to_bin(ipv4_test_vec[i].str, dest);
        HTEST_ASSERT_EQ(test, ret, 0);
        HTEST_ASSERT_MEM_EQ(test, dest, ipv4_test_vec[i].bin, 4);
    }

    for (int i = 0; i < array_size(ipv4_test_vec_inval); i++) {
        int ret = ipv4_to_bin(ipv4_test_vec_inval[i], dest);
        HTEST_ASSERT_EQ(test, ret, EINVAL);
    }
}

struct htest_unit inet_test_units[] = {
    HTEST_UNIT(test_ipv4_to_str),
    HTEST_UNIT(test_ipv4_to_bin),
    {},
};

struct htest_suite inet_test_suite = {
    .name = "inet test suite",
    .units = inet_test_units,
};

HTEST_DECLARE_SUITES(&inet_test_suite);
