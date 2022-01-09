/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIBC_URI_H_
#define HLIBC_URI_H_

#include <ctype.h>
#include <string.h>
#include <hlibc/string.h>
#include <hlibc/def.h>

enum uri_host_type {
    URI_HOST_IP4,
    URI_HOST_IP6,
    URI_HOST_IP_FUTURE,
    URI_HOST_DNS,
    URI_HOST_UNKNOWN,
};

struct uri {
    char *scheme;
    char *userinfo;
    char *host;
    char *path;
    char *query;
    char *fragment;
    uint16_t port;
    enum uri_host_type host_type;
};

int uri_init(struct uri *target);
void uri_destroy(struct uri *target);

/*
 * Sets `dest`'s scheme to a normalized copy of `scheme`.
 *
 * Per RFC 3986:
 * scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
 */
int uri_set_scheme(struct uri *dest, const char *src);

int uri_set_host(struct uri *dest, const char *src);
int uri_set_userinfo(struct uri *dest, const char *src);
int uri_set_port(struct uri *dest, uint16_t port);
int uri_parse_segments(struct uri *dest, const char *ptr);

bool is_dns(const char *str);
bool is_ipv6(const char *str);
bool is_ipv4(const char *str);
bool is_ipfuture(const char *str);


#endif
