#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <hlibc/uri.h>

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

static bool is_pct_enc(const char *pos)
{
    return pos[0] == '%' && isxdigit(pos[1]) && isxdigit(pos[2]);
}

static bool is_scheme_cset(const char *pos)
{
    return scheme_cset[(unsigned)*pos];
}

static bool is_userinfo_cset(const char *pos)
{
    return userinfo_cset[(unsigned)*pos] || is_pct_enc(pos);
}

static bool is_query_cset(const char *pos)
{
    return query_cset[(unsigned)*pos] || is_pct_enc(pos);
}

static bool is_fragment_cset(const char *pos)
{
    return fragment_cset[(unsigned)*pos] || is_pct_enc(pos);
}

static bool is_name_cset(const char *pos)
{
    return name_cset[(unsigned)*pos] || is_pct_enc(pos);
}

static bool is_ipvf_cset(const char *pos)
{
    return ipvf_cset[(unsigned)*pos];
}

static bool is_path_cset(const char *pos)
{
    return path_cset[(unsigned)*pos] || is_pct_enc(pos);
}

int parse_scheme(const char **src_pos)
{
    const char *pos = *src_pos;

    if (isalpha(*pos)) {
        while (is_scheme_cset(pos))
            pos++;

        if (*pos++ == ':') {
            *src_pos = pos;
            return 0;
        }
    }

    return EINVAL;
}

int parse_userinfo(const char **src_pos)
{
    const char *pos = *src_pos;

    while (is_userinfo_cset(pos))
        pos++;

    if (*pos++ == '@') {
        *src_pos = pos;
        return 0;
    }

    return EINVAL;
}

int parse_query(const char **src_pos)
{
    const char *pos = *src_pos;

    while (is_query_cset(pos))
        pos++;

    *src_pos = pos;
    return 0;
}

int parse_fragment(const char **src_pos)
{
    const char *pos = *src_pos;

    while (is_fragment_cset(pos))
        pos++;

    *src_pos = pos;
    return 0;
}

int parse_name(const char **src_pos)
{
    const char *pos = *src_pos;

    while (is_name_cset(pos))
        pos++;

    *src_pos = pos;
    return 0;
}

int parse_ipv4(const char **src_pos)
{
    const char *pos = *src_pos;

    for (int i = 0; i < 4; i++) {
        int val = 0;

        if (isdigit(*pos)) {
            if (*pos == '0') {
                pos++;
            } else {
                val = 10 * val + *pos++ - '0';
                if (isdigit(*pos)) {
                    val = 10 * val + *pos++ - '0';
                    if (isdigit(*pos)) {
                        val = 10 * val + *pos++ - '0';
                        if (val > 255)
                            return EINVAL;
                    }
                }
            }
        } else {
            return EINVAL;
        }

        if (i < 3 && *pos++ != '.')
            return EINVAL;
    }

    *src_pos = pos;
    return 0;
}

int parse_ipv6(const char **src_pos)
{
    const char *pos = *src_pos;
    const char *last = pos;
    bool comp = false;
    int count = 0;

    if (*pos++ != '[')
        return EINVAL;

    while (comp + count < 8) {
        if (*pos == ':') {
            pos++;
            if (*pos == ':') {
                if (comp)
                    return EINVAL;

                pos++;
                comp = true;
            }
        } else if (*pos == '.') {
            pos = last;
            count += 2;
            if (parse_ipv4(&pos))
                return EINVAL;
            break;
        }

        if (*pos == ']')
            break;

        last = pos;
        if (hlib_isxdigit(*pos)) {
            pos++;
            if (hlib_isxdigit(*pos)) {
                pos++;
                if (hlib_isxdigit(*pos)) {
                    pos++;
                    if (hlib_isxdigit(*pos))
                        pos++;
                }
            }

            count++;
        } else {
            return EINVAL;
        }
    }

    if (*pos++ != ']' || (comp ? count >= 8 : count < 8))
        return EINVAL;

    *src_pos = pos;
    return 0;
}

int parse_ipvf(const char **src_pos)
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

int parse_port(const char **src_pos)
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
int uri_parse_segments_switch(const char *src, struct uri_segments *dest)
{
    const char *pos = src;
    const char *last = src;

    memset(dest, 0, sizeof(*dest));

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

        if (!parse_ipvf(&pos))
            dest->host_type = URI_HOST_IPVF;
        else if (!parse_ipv6(&pos))
            dest->host_type = URI_HOST_IPV6;
        else if (!parse_ipv4(&pos))
            dest->host_type = URI_HOST_IPV4;
        else if (!parse_name(&pos))
            dest->host_type = URI_HOST_NAME;
        else
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
