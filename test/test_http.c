#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <hlibc/def.h>
#include <hlibc/string.h>
#include <hlibc/http.h>
#include <hlibc/term.h>

const char *invalid[] = {
    "https://google.com:100000",
    "https://[google.com]",
    "+srv://mongodb.com",
    "http://%%\\@example.com",
    "http://aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.com",
    "http://[]/",
};

void log_uri(const struct URI *uri)
{
    char *ptr = uri_repr(uri);
    printf(ptr);
    free(ptr);
}

void test_parse_uri()
{
    struct test_case {
        const char *str;
        const struct URI uri;
    };

    struct test_case test_cases[] = {
        {
            "https://example.com",
            { .scheme = "https",
              .host = "example.com",
              .host_type = URI_HOST_NAMED },
        },
        {
            "https://example.com:1200/path/item",
            { .scheme = "https",
              .host = "example.com",
              .port = 1200,
              .path = "/path/item",
              .host_type = URI_HOST_NAMED },
        },
        {
            "https://@example.com",
            { .scheme = "https",
              .host = "example.com",
              .host_type = URI_HOST_NAMED },
        },
        {
            "https://example.com/",
            { .scheme = "https",
              .host = "example.com",
              .path = "/",
              .host_type = URI_HOST_NAMED },
        },
        {
            "https://example.com?",
            { .scheme = "https",
              .host = "example.com",
              .host_type = URI_HOST_NAMED },
        },
        {
            "ht+-.tp0123456789s://example.com#",
            { .scheme = "ht+-.tp0123456789s",
              .host = "example.com",
              .host_type = URI_HOST_NAMED },
        },
        {
            "//example.com:80/path",
            { .host = "example.com",
              .port = 80,
              .path = "/path",
              .host_type = URI_HOST_NAMED },
        },
        {
            "file:///other-path?type=json",
            { .scheme = "file",
              .path = "/other-path",
              .query = "type=json",
              .host_type = URI_HOST_NONE },
        },
        {
            "mongodb+srv://user:p%40ss@mongodb.com:27017/test?auth=admin",
            { .scheme = "mongodb+srv",
              .userinfo = "user:p%40ss",
              .host = "mongodb.com",
              .port = 27017,
              .path = "/test",
              .query = "auth=admin",
              .host_type = URI_HOST_NAMED },
        },
        {
            "http://www.ics.uci.edu/pub/ietf/uri/#Related",
            { .scheme = "http",
              .host = "www.ics.uci.edu",
              .path = "/pub/ietf/uri/",
              .fragment = "Related",
              .host_type = URI_HOST_NAMED },
        },
        {
            "SCHEME://USER:PASS_%ab@EXAMPLE.COM:1000/PAGE_%AB?QUERY=VAL_%AB#FRAGMENT",
            { .scheme = "scheme",
              .userinfo = "USER:PASS_%AB",
              .host = "example.com",
              .path = "/PAGE_%AB",
              .query = "?QUERY=VAL_%AB",
              .fragment = "RELATED",
              .host_type = URI_HOST_NAMED },
        },
        {
            "http://127.0.0.1:8000/folder",
            { .scheme = "http",
              .host = "127.0.0.1",
              .port = 8000,
              .path = "/folder",
              .host_type = URI_HOST_IP4 },
        }
    };

    /* Valid cases */
    for (int i = 0; i < ARRAY_SIZE(test_cases); i++) {
        struct URI uri;

        int err = parse_uri(&uri, test_cases[i].str, NULL);
        if (err) {
            char *exp = uri_repr(&test_cases[i].uri);
            printf("Returned error while parsing URI: %d\n"
                   "Input: %s\n"
                   "Expected: %s\n",
                   err, test_cases[i].str, exp);
            free(exp);
            abort();
        } else if (!uri_equal(&uri, &test_cases[i].uri)) {
            char *out = uri_repr(&uri);
            char *exp = uri_repr(&test_cases[i].uri);
            printf("Test failed:\n"
                   "Input: %s\n"
                   "Expected: %s\n"
                   "Recieved: %s\n",
                   test_cases[i].str, exp, out);
            free(out);
            free(exp);
            abort();
        }

        destroy_uri(&uri);
    }

    /* Invalid cases */
    for (int i = 0; i < ARRAY_SIZE(invalid); i++) {
        struct URI uri;

        int err = parse_uri(&uri, invalid[i], NULL);
        if (!err) {
            char *repr = uri_repr(&uri);
            printf("Expected EINVAL, but URI was parsed:\n"
                   "Result: %s\n",
                   repr);
            abort();
        } else if (err != EINVAL) {
            printf("Expected EINVAL, but got error code %d\n", err);
            abort();
        }

        destroy_uri(&uri);
    }
}

void test_normalize_path()
{
    const char *test_cases[][2] = {
        { "../a", "a" },
        { "./a", "a" },
        { ".././a", "a" },
        { "/a/./b", "/a/b" },
        { "/a/./", "/a/" },
        { "/.", "/" },
        { "/a/.", "/a/" },
        { "/a/../b", "/b" },
        { "/a/../", "/" },
        { "/..", "/" },
        { "/a/..", "/" },
        { "/a/b/..", "/a/" },
        { ".", "" },
        { "..", "" },
        { "//a//////", "/a/" },
        { "/a/./b/.././a//a", "/a/a/a" },
        { "/%Fa//./%1e", "/%FA/%1E" },
        { "/ab"
          "\x63"
          "de/xx"
          "\xA5"
          "xx",
          "/abcde/xx%A5xx" },
    };

    for (int i = 0; i < ARRAY_SIZE(test_cases); i++) {
        const char *input = test_cases[i][0];
        const char *expected = test_cases[i][1];
        char *path = normalize_path(input);

        if (strcmp(path, expected)) {
            printf("Test %d of 'test_normalize_path()' failed:\n"
                   "    Input: %s\n"
                   "    Expected: %s\n"
                   "    Recieved Path: %s\n",
                   i + 1, input, expected, path);
            abort();
        }

        free(path);
    }
}

int main()
{
    test_normalize_path();
    // test_parse_uri();
    printf("Success\n");
}
