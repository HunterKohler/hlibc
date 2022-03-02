/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <hlibc/ctype.h>
#include <hlibc/uri.h>
#include <hlibc/inet.h>

const char scheme_cset[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const char userinfo_cset[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const char name_cset[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const char query_cset[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const char fragment_cset[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const char ipvf_cset[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const char path_cset[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static inline bool is_pct_enc(const char *pos)
{
    return pos[0] == '%' && isxdigit(pos[1]) && isxdigit(pos[2]);
}

static inline bool is_scheme_cset(const char *pos)
{
    return scheme_cset[(unsigned)*pos];
}

static inline bool is_userinfo_cset(const char *pos)
{
    return userinfo_cset[(unsigned)*pos] || is_pct_enc(pos);
}

static inline bool is_query_cset(const char *pos)
{
    return query_cset[(unsigned)*pos] || is_pct_enc(pos);
}

static inline bool is_fragment_cset(const char *pos)
{
    return fragment_cset[(unsigned)*pos] || is_pct_enc(pos);
}

static inline bool is_name_cset(const char *pos)
{
    return name_cset[(unsigned)*pos] || is_pct_enc(pos);
}

static inline bool is_ipvf_cset(const char *pos)
{
    return ipvf_cset[(unsigned)*pos];
}

static inline bool is_path_cset(const char *pos)
{
    return path_cset[(unsigned)*pos] || is_pct_enc(pos);
}

static int parse_scheme(const char **src_pos)
{
    const char *pos = *src_pos;

    if (!hlib_isalpha(*pos))
        return EINVAL;

    while (is_scheme_cset(pos))
        pos++;

    if (*pos++ != ':')
        return EINVAL;

    *src_pos = pos;
    return 0;
}

static int parse_userinfo(const char **src_pos)
{
    const char *pos = *src_pos;

    while (is_userinfo_cset(pos))
        pos++;

    if (*pos++ != '@')
        return EINVAL;

    *src_pos = pos;
    return 0;
}

static int parse_query(const char **src_pos)
{
    const char *pos = *src_pos;

    while (is_query_cset(pos))
        pos++;

    *src_pos = pos;
    return 0;
}

static int parse_fragment(const char **src_pos)
{
    const char *pos = *src_pos;

    while (is_fragment_cset(pos))
        pos++;

    *src_pos = pos;
    return 0;
}

static int parse_ipvf(const char **src_pos)
{
    const char *pos = *src_pos;

    if (*pos++ != '[' || *pos++ != 'v' || !isxdigit(*pos++))
        return EINVAL;

    while (isxdigit(*pos))
        pos++;

    if (*pos++ != '.' || !is_ipvf_cset(pos++))
        return EINVAL;

    while (is_ipvf_cset(pos))
        pos++;

    if (*pos++ != ']')
        return EINVAL;

    *src_pos = pos;
    return 0;
}

static int parse_host(const char **src_pos, enum uri_host_type *type)
{
    const char *pos = *src_pos;

    if (!parse_ipvf(&pos)) {
        *type = URI_HOST_IPVF;
    } else if (*pos == '[') {
        char host[40];
        uint8_t bin[16];

        pos++;
        *type = URI_HOST_IPV6;
        for (int i = 0; i < array_size(host); i++) {
            if (!*pos) {
                return EINVAL;
            } else if (*pos == ']') {
                pos++;
                host[i] = 0;
                break;
            }

            host[i] = *pos++;
        }

        if (ipv6_to_bin(host, bin))
            return EINVAL;
    } else {
        char host[16];
        uint8_t bin[4];

        for (int i = 0; i < array_size(host); i++) {
            if (isdigit(pos[i]) || pos[i] == '.') {
                host[i] = pos[i];
            } else {
                host[i] = 0;
                if (!ipv4_to_bin(host, bin)) {
                    *type = URI_HOST_IPV4;
                    *src_pos = pos + i;
                    return 0;
                }
                break;
            }
        }

        *type = URI_HOST_NAME;
        while (is_name_cset(pos))
            pos++;
    }

    return 0;
}

static int parse_port(const char **src_pos)
{
    const char *pos = *src_pos;

    while (isdigit(*pos))
        pos++;

    *src_pos = pos;
    return 0;
}

static int parse_path(const char **src_pos, bool scheme, bool authority)
{
    const char *pos = *src_pos;

    if (authority) {
        if (*pos != '/' && is_path_cset(pos))
            return EINVAL;
    } else if (!scheme) {
        while (is_path_cset(pos) && *pos != '/') {
            if (*pos == ':')
                return EINVAL;
            pos++;
        }
    }

    while (is_path_cset(pos))
        pos++;

    *src_pos = pos;
    return 0;
}

/*
 * For an invalid URI, the contents of `dest` are undefined.
 */
int uri_parse_segments(const char *src, struct uri_segments *dest)
{
    const char *pos = src;
    const char *last = src;

    memzero(dest, sizeof(*dest));

    if (!parse_scheme(&pos)) {
        dest->scheme.str = last;
        dest->scheme.size = pos - last - 1;
        last = pos;
    }

    pos = last;
    if (*pos++ == '/' && *pos++ == '/') {
        last = pos;

        if (!parse_userinfo(&pos)) {
            dest->userinfo.str = last;
            dest->userinfo.size = pos - last - 1;
            last = pos;
        }

        if (parse_host(&pos, &dest->host_type))
            return EINVAL;

        dest->host.str = last;
        dest->host.size = pos - last;
        last = pos;

        if (*pos == ':') {
            last = ++pos;
            if (parse_port(&pos))
                return EINVAL;

            dest->port.str = last;
            dest->port.size = pos - last;
            last = pos;
        }
    }

    pos = last;
    if (parse_path(&pos, dest->scheme.str, dest->host.str)) {
        return EINVAL;
    } else {
        dest->path.str = last;
        dest->path.size = pos - last;
    }

    if (*pos == '?') {
        last = ++pos;
        if (parse_query(&pos)) {
            return EINVAL;
        } else {
            dest->query.str = last;
            dest->query.size = pos - last;
        }
    }

    if (*pos == '#') {
        last = ++pos;
        if (parse_fragment(&pos)) {
            return EINVAL;
        } else {
            dest->fragment.str = last;
            dest->fragment.size = pos - last;
        }
    }

    return *pos ? EINVAL : 0;
}

static int normalize_with_cset(const char *src, char *dest,
                               const char cset[256])
{
    while (*src) {
        if (*src == '%') {
            int a = hex_val(src[1]);
            if (a < 0)
                return EINVAL;

            int b = hex_val(src[2]);
            if (b < 0)
                return EINVAL;

            int v = 16 * a + b;
            if (cset[v]) {
                *dest++ = v;
            } else {
                *dest++ = '%';
                *dest++ = toupper(src[1]);
                *dest++ = toupper(src[2]);
            }

            src += 3;
        } else if (cset[(unsigned)*src]) {
            *dest++ = *src++;
        } else {
            return EINVAL;
        }
    }

    *dest = 0;
    return 0;
}

int uri_normalize_scheme(const char *src, char *dest)
{
    if (!hlib_isalpha(*src))
        return EINVAL;

    *dest++ = tolower(*src++);
    while (*src) {
        if (scheme_cset[(unsigned)*src]) {
            *dest++ = tolower(*src++);
        } else {
            return EINVAL;
        }
    }

    return 0;
}

int uri_normalize_userinfo(const char *src, char *dest)
{
    return normalize_with_cset(src, dest, userinfo_cset);
}

int uri_normalize_host(const char *src, char *dest)
{
    enum uri_host_type type;

    if (parse_host(&src, &type))
        return EINVAL;

    switch (type) {
    case URI_HOST_IPV6:
        char host[40];

        if (*src++ != '[')
            return EINVAL;

        *dest++ = '[';
        for (int i = 0; i < array_size(host) && src[i]; i++) {
            if (src[i] == ']') {
                if (src[i + 1])
                    return EINVAL;

                host[i] = 0;
                if (ipv6_norm(host, dest))
                    return EINVAL;

                strcat(dest, "]");
                return 0;
            }

            host[i] = src[i];
        }

        return EINVAL;
    case URI_HOST_IPVF:
        if (src[0] != '[' || src[1] != 'v' || !isxdigit(src[2]))
            return EINVAL;

        *dest++ = *src++;
        *dest++ = *src++;
        *dest++ = tolower(*src++);

        while (isxdigit(*src))
            *dest++ = tolower(*src++);

        if (*src != '.')
            return EINVAL;

        return normalize_with_cset(src, dest, ipvf_cset);
    case URI_HOST_NAME:
        return normalize_with_cset(src, dest, name_cset);
    default:
        return 0;
    }
}

int uri_normalize_path(const char *src, char *dest)
{
    return normalize_with_cset(src, dest, path_cset);
}

int uri_normalize_query(const char *src, char *dest)
{
    return normalize_with_cset(src, dest, query_cset);
}

int uri_normalize_fragment(const char *src, char *dest)
{
    return normalize_with_cset(src, dest, fragment_cset);
}
