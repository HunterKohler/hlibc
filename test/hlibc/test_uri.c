/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <assert.h>
#include <stdio.h>
#include <hlibc/uri.h>

struct is_ipv4_test_case {
    char *input;
    bool valid;
};

const struct is_ipv4_test_case is_ipv4_test_cases[] = {
    { "185.216.27.142", true },
    { "13.89.120.251", true },
    { "40.76.112.230", true },
    { "159.69.198.101", true },
    { "134.209.146.16", true },
    { "139.59.48.222", true },
    { "149.154.153.153", true },
    { "255.255.255.255", true },
    { "0.0.0.0", true },
    { "0.0.0.", false },
    { "0.0.0", false },
    { "0.0.", false },
    { "0.0", false },
    { "0.", false },
    { "0", false },
    { "", false },
    { "0.0.256.0", false },
    { "255.255", false },
    { "255..255", false },
};

void test_is_ipv4()
{
    for (int i = 0; i < array_size(is_ipv4_test_cases); i++) {
        const struct is_ipv4_test_case *tc = is_ipv4_test_cases + i;
        bool res = is_ipv4(tc->input);

        if (res != tc->valid) {
            printf("Failed 'test_is_ipv4' case:\n"
                   "\tinput := '%s'\n"
                   "\texpected := %s\n",
                   tc->input, tc->valid ? "true" : "false");
            abort();
        }
    }
}

struct is_dns_test_case {
    char *input;
    bool valid;
};

struct is_dns_test_case is_dns_test_cases[] = {
    { "www.google.com", true },
    { "www.google.co--m", true },
    { "www.goo-------gle.com", true },
    { "www.goog_le.com", false },
    { "wwwgooglecom", false },
    { ".www.google.com", false },
    { "www.google.com.", false },
    { "www.-gle.com", false },
    { "-gle.com", false },
    { "gle-.com", false },
    { "gle.-com", false },
    { "gle.com-", false },
    { "gle.com", true },
    { "gle.a", false },
    { "000000000000000000000000000000.ga", true },
    { "0.0.0.0.com", true },
    { "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx.com",
      false },
    { "com.xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
      false },
    { "abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc."
      "abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc."
      "abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc."
      "abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc",
      true },
    { "abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc."
      "abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc."
      "abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc."
      "abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc.abc",
      false },
};

void test_is_dns()
{
    for (int i = 0; i < array_size(is_dns_test_cases); i++) {
        const struct is_dns_test_case *tc = is_dns_test_cases + i;
        bool res = is_dns(tc->input);

        if (res != tc->valid) {
            printf("Failed 'test_is_dns' case:\n"
                   "\tinput := '%s'\n"
                   "\texpected := %s\n",
                   tc->input, tc->valid ? "true" : "false");
            abort();
        }
    }
}

// int main()
// {
//     test_is_ipv4();
//     test_is_dns();
// }
