/*
 * Copyright (C) 2021-2022 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <ctype.h>
#include <string.h>
#include <hlibc/string.h>
#include <hlibc/uri.h>

int uri_init(struct uri *target)
{
    memset(target, 0, sizeof(*target));
    return 0;
}

void uri_destroy(struct uri *target)
{
    free(target->scheme);
    free(target->userinfo);
    free(target->host);
    free(target->query);
    free(target->fragment);
}

int uri_set_scheme(struct uri *dest, const char *src)
{
    if (!src) {
        free(dest->scheme);
        dest->scheme = NULL;
        return 0;
    }

    if (!isalpha(src[0]))
        return EINVAL;

    char *scheme = strdup(src);
    if (!scheme)
        return ENOMEM;

    while (isalnum(*scheme) || *scheme == '+' || *scheme == '-' ||
           *scheme == '.')
        *scheme = tolower(*scheme);

    if (*scheme) {
        free(dest->scheme);
        dest->scheme = scheme;
        return 0;
    } else {
        free(scheme);
        return EINVAL;
    }
}

int uri_set_host(struct uri *dest, const char *src)
{
    if (!src) {
        free(dest->host);
        dest->host = NULL;
        return 0;
    }

    char *host;
    enum uri_host_type type = is_ipv4(src)     ? URI_HOST_IP4 :
                              is_ipv6(src)     ? URI_HOST_IP6 :
                              is_ipfuture(src) ? URI_HOST_IP_FUTURE :
                              is_dns(src)      ? URI_HOST_DNS :
                                                 URI_HOST_UNKNOWN;

    if (type == URI_HOST_UNKNOWN) {
        size_t len = strlen(src);
        host = malloc(3 * len);
        if (!host)
            return ENOMEM;

        len = 0;
        while (*src) {
            if (src[0] == '%' && isxdigit(src[1]) && isxdigit(src[2])) {
                int val = 16 * hex_val(src[1]) + hex_val(src[2]);

                if (isalnum(val) || strchr("!$&'()*+,;=-._~", val)) {
                    host[len++] = val;
                    src += 3;
                } else {
                    host[len++] = *src++;
                    host[len++] = toupper(*src++);
                    host[len++] = toupper(*src++);
                }
            } else if (isalnum(*src) || strchr("!$&'()*+,;=-._~", *src)) {
                host[len++] = *src++;
            } else {
                host[len++] = '%';
                host[len++] = *src / 16;
                host[len++] = *src % 16;
                src += 1;
            }
        }

        char *buf = realloc(host, len + 1);
        if (!buf) {
            free(host);
            return ENOMEM;
        }

        buf[len] = 0;
        host = buf;
    } else {
        host = strdup(src);
        if (!host)
            return ENOMEM;
    }

    free(dest->host);
    dest->host = host;
    dest->host_type = type;
    return 0;
}

int uri_set_userinfo(struct uri *dest, const char *src)
{
    if (!src) {
        free(dest->userinfo);
        dest->userinfo = NULL;
        return 0;
    }

    int len = strlen(src);
    char *userinfo = malloc(3 * len + 1);
    if (!userinfo)
        return ENOMEM;

    len = 0;
    while (*src) {
        if (src[0] == '%' && isxdigit(src[1]) && isxdigit(src[2])) {
            char val = 16 * hex_val(src[1]) + hex_val(src[2]);

            if (isalnum(val) || strchr("!$&'()*+,;=-._~:", val)) {
                userinfo[len++] = *src++;
                userinfo[len++] = *src++;
                userinfo[len++] = *src++;
            } else {
                userinfo[len++] = val;
                src += 3;
            }
        } else if (isalnum(src[0]) || strchr("!$&'()*+,;=-._~:", src[0])) {
            userinfo[len++] = '%';
            userinfo[len++] = src[0] / 16;
            userinfo[len++] = src[0] % 16;
            src += 1;
        } else {
            userinfo[len++] = *src++;
        }
    }

    char *buf = realloc(userinfo, len + 1);
    if (!buf) {
        free(userinfo);
        return ENOMEM;
    }

    buf[len] = 0;
    dest->userinfo = buf;
    return 0;
}

int uri_set_port(struct uri *dest, uint16_t port)
{
    dest->port = port;
    return 0;
}

int uri_parse_segments(struct uri *dest, const char *ptr)
{
    size_t len;
    memset(dest, 0, sizeof(*dest));

    if (!ptr)
        return EINVAL;

    len = strcspn(ptr, ":/?#");
    len *= ptr[len] == ':';

    if (len) {
        dest->scheme = strndup(ptr, len);
        if (!dest->scheme)
            goto error_nomem;

        ptr += len + 1;
    }

    if (!memcmp("//", ptr, 2)) {
        ptr += 2;
        len = strcspn(ptr, "@/?#");

        if (ptr[len] == '@') {
            dest->userinfo = strndup(ptr, len);
            if (!dest->userinfo)
                goto error_nomem;

            ptr += len + 1;
        }

        if (ptr[0] == '[') {
            len = strcspn(ptr, "]/?#");
            if (ptr[len++] != ']')
                goto error_inval;
        } else {
            len = strcspn(ptr, ":/?#");
        }

        dest->host = strndup(ptr, len);
        if (!dest->host)
            goto error_nomem;

        ptr += len;
        if (ptr[0] == ':') {
            ptr += 1;
            len = strcspn(ptr, "/?#");
            if (!len || len > 5 || ptr[0] == '0')
                goto error_inval;

            int val = 0;
            for (int i = 0; i < len; i++) {
                if (!isdigit(ptr[i]))
                    goto error_inval;
                val = 10 * val + ptr[i] - '0';
            }

            if (val > (1 << 16))
                goto error_inval;

            dest->port = val;
            ptr += len;
        }
    }

    len = strcspn(ptr, "?#");
    dest->path = strndup(ptr, len);
    if (!dest->path)
        goto error_nomem;

    ptr += len;

    if (ptr[0] == '?') {
        ptr += 1;
        len = strcspn(ptr, "#");
        dest->query = strndup(ptr, len);
        if (!dest->path)
            goto error_nomem;

        ptr += len;
    }

    if (ptr[0] == '#') {
        ptr += 1;
        dest->query = strdup(ptr);
        if (!dest->query)
            goto error_nomem;
    }

    return 0;
error_nomem:
    uri_destroy(dest);
    return ENOMEM;
error_inval:
    uri_destroy(dest);
    return EINVAL;
}

bool is_dns(const char *str)
{
    if (!str || !isalnum(*str))
        return false;

    int labels = 1;
    int label_len = 1;
    int i = 1;

    for (; i < 254 && str[i]; i++) {
        if (str[i] == '.') {
            if (!isalnum(str[i - 1]) || !isalnum(str[i + 1]) || label_len > 63)
                return false;

            labels += 1;
            label_len = 0;
        } else if (isalnum(str[i]) || str[i] == '-') {
            label_len += 1;
        } else {
            return false;
        }
    }

    return !str[i] && label_len >= 2 && label_len <= 63 && labels >= 2 &&
           isalnum(str[i - 1]);
}

bool is_ipv6(const char *str)
{
    return false;
}

bool is_ipfuture(const char *str)
{
    return false;
}

bool is_ipv4(const char *str)
{
    for (int i = 0; i < 4; i++) {
        if (isdigit(str[0])) {
            if (isdigit(str[1])) {
                if (str[0] == '0')
                    return false;
                if (isdigit(str[2])) {
                    if (255 < 100 * (str[0] - '0') + 10 * (str[1] - '0') +
                                  (str[2] - '0'))
                        return false;
                    str += 3;
                } else {
                    str += 2;
                }
            } else {
                str += 1;
            }
        } else {
            return false;
        }

        if (i == 3 ? *str : *str++ != '.')
            return false;
    }

    return true;
}
