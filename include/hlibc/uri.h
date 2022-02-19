#ifndef HLIBC_URI_H_
#define HLIBC_URI_H_

#include <hlibc/ctype.h>
#include <hlibc/string.h>

struct string_view {
    const char *str;
    size_t size;
};

enum uri_host_type {
    URI_HOST_IPV4,
    URI_HOST_IPV6,
    URI_HOST_IPVF,
    URI_HOST_NAME,
};

struct uri_segments {
    struct string_view scheme;
    struct string_view userinfo;
    struct string_view host;
    struct string_view port;
    struct string_view path;
    struct string_view query;
    struct string_view fragment;
    enum uri_host_type host_type;
};

int uri_parse_segments(const char *src, struct uri_segments *dest);

#endif
