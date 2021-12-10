/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <stdlib.h>
#include <ctype.h>
#include <regex.h>
#include <pthread.h>
#include <hlibc/def.h>
#include <hlibc/string.h>
#include <hlibc/regex.h>
#include <hlibc/http.h>

regex_t re_uri;
regex_t re_ip4;
regex_t re_hostname;

static void http_module_init_routine()
{
    regcomp(&re_uri,
            "^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?$",
            REG_EXTENDED);
    regcomp(
        &re_ip4,
        "^((25[0-5]|2[0-4]\\d|1?\\d{1,2})\\.){3}(25[0-5]|2[0-4]\\d|1?\\d{1,2})$",
        REG_EXTENDED);
}

static void http_module_init()
{
    static pthread_once_t once = PTHREAD_ONCE_INIT;
    pthread_once(&once, http_module_init_routine);
}

static bool is_unreserved(char c)
{
    return isalnum(c) || c == '-' || c == '.' || c == '_' || c == '~';
}

static int is_percent_encoded(const char *str)
{
    return str[0] == '%' && isxdigit(str[1]) && isxdigit(str[2]);
}

bool uri_equal(const struct URI *a, const struct URI *b)
{
    return a == b ||
           (a && b && !strcmp_safe(a->scheme, b->scheme) &&
            !strcmp_safe(a->host, b->host) &&
            !strcmp_safe(a->userinfo, b->userinfo) &&
            !strcmp_safe(a->path, b->path) &&
            !strcmp_safe(a->query, b->query) &&
            !strcmp_safe(a->fragment, b->fragment) && a->port == b->port);
}

char *uri_repr(const struct URI *uri)
{
    if (!uri)
        return strdup("NULL");

    char *ret;
    asprintf(&ret,
             "(struct URI) {\n"
             "    scheme: %s\n"
             "    userinfo: %s\n"
             "    host: %s\n"
             "    port: %u\n"
             "    path: %s\n"
             "    query: %s\n"
             "    fragment: %s\n"
             "}",
             uri->scheme, uri->userinfo, uri->host, uri->port, uri->path,
             uri->query, uri->fragment);
    return ret;
}

void destroy_uri(struct URI *uri)
{
    free(uri->scheme);
    free(uri->host);
    free(uri->path);
    free(uri->query);
    free(uri->fragment);
    free(uri->userinfo);
    memset(uri, 0, sizeof(*uri));
}

int normalize_uri(struct URI *uri)
{
    return 0;
}

int normalize_scheme(char *scheme)
{
    if (!isalpha(*scheme))
        return EINVAL;

    for (char *it = scheme; *it; it++) {
        if (!isalnum(*it) && !strchr("+-.", *it))
            return EINVAL;
        *it = tolower(*it);
    }

    return 0;
}

int normalize_hostname(char *hostname)
{
    if (!isalnum(*hostname))
        return EINVAL;

    char *it = hostname + 1;
    size_t len = 1;
    size_t seglen = 1;

    while (*it) {
        *it = tolower(*it);
        if (++seglen > 63 || ++len > 253 ||
            (!isalnum(*it) &&
             !(it[0] == '-' && isalnum(it[-1]) && isalnum(it[1]))))
            return EINVAL;
    }

    return 0;
}

int parse_port(const char *str, uint16_t *dest, size_t *len)
{
    if (!str || !isdigit(str[0]))
        return EINVAL;

    uint32_t val = 0;
    size_t pos = 0;

    while (isdigit(str[pos]) && pos < 5)
        val += str[pos++] - '0';

    if (isdigit(str[pos]) || val >= 1 << 16)
        return EOVERFLOW;

    if (dest)
        *dest = val;
    if (len)
        *len = pos;

    return 0;
}

int parse_uri(struct URI *restrict uri, const char *str,
              const struct URI *restrict base)
{
    http_module_init();

    regmatch_t matches[10];
    char *scheme = NULL;
    char *authority = NULL;
    char *path = NULL;
    char *query = NULL;
    char *fragment = NULL;

    int err;

    if ((err = regexec(&re_uri, str, ARRAY_SIZE(matches), matches, 0)) ||
        (err = regmatch_text(str, &matches[2], &scheme)) ||
        (err = regmatch_text(str, &matches[4], &authority)) ||
        (err = regmatch_text(str, &matches[5], &path)) ||
        (err = regmatch_text(str, &matches[7], &query)) ||
        (err = regmatch_text(str, &matches[9], &fragment))) {
        goto error;
    }

    // if (scheme) {
    //     if (!valid_scheme(scheme))
    //         goto error_invalid;
    // } else {
    //     if (!base) {
    //         goto error_invalid;
    //     } else if (!(scheme = strdup(base->scheme))) {
    //         goto error_memory;
    //     } else if (!authority) {
    //         if (!(authority = strdup(base->authority)))
    //             goto error_memory;

    //         if (!path[0]) {
    //             free(path);
    //             if (!(path = strdup(base->path)) ||
    //                 (!query && !(query = strdup(base->query)))) {
    //                 goto error_memory;
    //             }
    //         } else if (path[0] != '/') {
    //             size_t base_len = strlen(base->path);
    //             size_t path_len = strlen(path);

    //             if (base_len) {
    //                 while (base_len && base->path[base_len - 1] != '/')
    //                     base_len--;

    //                 char *mem = malloc(base_len + path_len + 1);
    //                 memcpy(mem, base->path, base_len);
    //                 memcpy(mem + base_len, path_len);
    //                 mem[base_len + path_len] = 0;
    //             } else {
    //                 char *mem = malloc(path_len + 2);
    //                 if (!mem)
    //                     goto error_memory;
    //                 mem[0] = '/';
    //                 memcpy(mem + 1, path, path_len + 1);
    //                 free(path);
    //             }
    //         }
    //     }
    // }

error_invalid:
    err = EINVAL;
    goto error;
error_memory:
    err = ENOMEM;
    goto error;
error:
    destroy_uri(uri);
    return err ? err : -1;
}

// memset(uri, 0, sizeof(*uri));

// if (regmatch_len(match_scheme)) {
//     size_t start = match_scheme->rm_so;
//     size_t len = match_scheme->rm_eo - start;
//     char *scheme = stralloc(len);
//     uri->scheme = scheme;

//     if (!scheme)
//         goto error_memory;

//     if (!isalpha(str[start]))
//         goto error_invalid;

//     for (int i = 0; i < len; i++) {
//         char c = str[start + i];
//         if (isalnum(c) || strchr("+-.", c))
//             scheme[i] = tolower(c);
//         else
//             goto error_invalid;
//     }
// }

// if (regmatch_len(match_authority)) {
//     size_t start = match_authority->rm_so;
//     size_t len = 0;

//     while (isalnum(str[start + len]) ||
//            strchr("-._~:!$&'()*+,;=", str[start + len]) ||
//            is_percent_encoded(str + start + len))
//         len++;

//     if (str[start + len] == '@') {
//         if (len > 0) {
//             if (!(uri->userinfo = memdup(str + start, len + 1)))
//                 goto error_memory;

//             uri->userinfo[len] = 0;
//         }

//         start += len + 1;
//     }

//     len = 0;
//     if (str[start] == '[') {
//         while (isxdigit(str[start + len]) || strchr(":.]", str[start + len]))
//             len++;

//         if (str[start + len - 1] != ']')
//             goto error_invalid;

//         uri->host = memdup(str + start, len + 1);
//         if (!uri->host)
//             goto error_memory;

//         uri->host[len] = 0;
//         uri->host_type = URI_HOST_IP6;
//         start += len;
//         len = 0;
//     } else {
//         if ((len = strcspn(str + start, ":/?#")) > 253)
//             goto error_invalid;

//         char *host = memdup(str + start, len + 1);
//         if (!host)
//             goto error_memory;

//         host[len] = 0;
//         uri->host = host;

//         if (!regexec(&re_ip4, host, 0, NULL, 0)) {
//             uri->host_type = URI_HOST_IP4;
//         } else {
//             uri->host_type = URI_HOST_NAMED;

//             if (!isalnum(host[0]))
//                 goto error_invalid;

//             for (int i = 0; i < len; i++) {
//                 if (host[i] == '.' ?
//                         !isalnum(host[i - 1]) || !isalnum(host[i + 1]) :
//                         !isalnum(host[i]) && host[i] != '-')
//                     goto error_invalid;

//                 host[i] = tolower(host[i]);
//             }
//         }

//         start += len;
//         len = 0;
//     }

//     if (str[start] == ':') {
//         start++;

//         int n = 0;
//         int max_len = len + 5;

//         while (len < max_len && isdigit(str[start + len]))
//             n = 10 * n + str[start + len++] - '0';

//         if (n >= 1 << 16)
//             goto error_invalid;

//         uri->port = n;
//         start += len;
//     }

//     if (start != match_authority->rm_eo)
//         goto error_invalid;
// }

// if (regmatch_len(match_path)) {
//     size_t start = match_path->rm_so;
//     size_t len = match_path->rm_eo - start;
//     char *path = memdup(str + start, len + 1);
//     uri->path = path;

//     if (!path)
//         goto error_memory;

//     path[len] = 0;
// }

// if (regmatch_len(match_query)) {
//     size_t start = match_query->rm_so;
//     size_t len = match_query->rm_eo - start;
//     char *query = memdup(str + start, len + 1);
//     uri->query = query;

//     if (!query)
//         goto error_memory;

//     query[len] = 0;
// }

// if (regmatch_len(match_fragment)) {
//     size_t start = match_fragment->rm_so;
//     size_t len = match_fragment->rm_eo - start;
//     char *fragment = memdup(str + start, len + 1);
//     uri->fragment = fragment;

//     if (!fragment)
//         goto error_memory;

//     fragment[len] = 0;
// }

// return 0;

// }

char *normalize_path(const char *path)
{
    if (!path)
        return NULL;

    /* Temporary copy, and over-allocated buf for ease of implementation. */
    size_t buf_len = 3 * strlen(path);
    char *input = NULL;
    char *buf = NULL;

    if (!(input = strdup(path)) || !(buf = malloc(buf_len)))
        goto error;

    char *in = input;
    char *out = buf;

    while (*in) {
        if (!memcmp(in, "//", 2)) {
            in++;
        } else if (!memcmp(in, "../", 3)) {
            in += 3;
        } else if (!memcmp(in, "./", 2)) {
            in += 2;
        } else if (!memcmp(in, "/./", 3)) {
            in += 2;
        } else if (!memcmp(in, "/.", 3)) {
            *(++in) = '/';
        } else if (!memcmp(in, "/../", 4)) {
            in += 3;
            while (out > buf && *out != '/')
                out--;
        } else if (!memcmp(in, "/..", 4)) {
            *(in += 2) = '/';
            while (out > buf && *out != '/')
                out--;
        } else if (!memcmp(in, "..", 3) || !memcmp(in, ".", 2)) {
            break;
        } else if (is_percent_encoded(in)) {
            char c = hex_val(in[1]) * 16 + hex_val(in[2]);

            if (is_unreserved(c)) {
                *out++ = c;
            } else {
                *out++ = '%';
                *out++ = toupper(in[1]);
                *out++ = toupper(in[2]);
            }

            in += 3;
        } else if (is_unreserved(*in) || *in == '/' || *in == '.') {
            *out++ = *in++;
        } else {
            unsigned char c = *in++;
            *out++ = '%';
            *out++ = "0123456789ABCDEF"[c >> 4];
            *out++ = "0123456789ABCDEF"[c & 0b1111];
        }
    }

    size_t ret_len = out - buf;
    char *ret = realloc(buf, ret_len + 1);
    if (!ret)
        goto error;

    ret[ret_len] = 0;
    free(input);
    return ret;

error:
    free(buf);
    free(input);
    return NULL;
}

bool http_status_informational(int code)
{
    return code >= 100 && code < 200;
}

bool http_status_successful(int code)
{
    return code >= 200 && code < 300;
}

bool http_status_redirect(int code)
{
    return code >= 300 && code < 400;
}

bool http_status_client_error(int code)
{
    return code >= 400 && code < 500;
}

bool http_status_server_error(int code)
{
    return code >= 500 && code < 600;
}

const char *http_status_reason(int code)
{
    /* clang-format off */
    switch (code) {
    case 100: return "Continue";
    case 101: return "Switching Protocols";
    case 200: return "OK";
    case 201: return "Created";
    case 202: return "Accepted";
    case 203: return "Non-Authoritative Information";
    case 204: return "No Content";
    case 205: return "Reset Content";
    case 206: return "Partial Content";
    case 300: return "Multiple Choices";
    case 301: return "Moved Permanently";
    case 302: return "Found";
    case 303: return "See Other";
    case 304: return "Not Modified";
    case 305: return "Use Proxy";
    case 307: return "Temporary Redirect";
    case 400: return "Bad Request";
    case 401: return "Unauthorized";
    case 402: return "Payment Required";
    case 403: return "Forbidden";
    case 404: return "Not Found";
    case 405: return "Method Not Allowed";
    case 406: return "Not Acceptable";
    case 407: return "Proxy Authentication Required";
    case 408: return "Request Timeout";
    case 409: return "Conflict";
    case 410: return "Gone";
    case 411: return "Length Required";
    case 412: return "Precondition Failed";
    case 413: return "Payload Too Large";
    case 414: return "URI Too Long";
    case 415: return "Unsupported Media Type";
    case 416: return "Range Not Satisfiable";
    case 417: return "Expectation Failed";
    case 418: return "I'm a teapot";
    case 426: return "Upgrade Required";
    case 500: return "Internal Server Error";
    case 501: return "Not Implemented";
    case 502: return "Bad Gateway";
    case 503: return "Service Unavailable";
    case 504: return "Gateway Timeout";
    case 505: return "HTTP Version Not Supported";
    default: return NULL;
    }
    /* clang-format on */
}
