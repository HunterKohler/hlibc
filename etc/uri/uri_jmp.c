#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

#include "uri.h"

// clang-format off
#define ALPHA_UPPER \
         'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': \
    case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': \
    case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': \
    case 'V': case 'W': case 'X': case 'Y': case 'Z'

#define ALPHA_LOWER \
         'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': \
    case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': \
    case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': \
    case 'v': case 'w': case 'x': case 'y': case 'z'

#define ALPHA ALPHA_UPPER: case ALPHA_LOWER

#define DIGIT \
         '0': case '1': case '2': case '3': case '4': case '5': case '6': \
    case '7': case '8': case '9'

#define HEXDIGIT \
         '0': case '1': case '2': case '3': case '4': case '5': case '6': \
    case '7': case '8': case '9': case 'a': case 'b': case 'c': case 'd': \
    case 'e': case 'f': case 'A': case 'B': case 'C': case 'D': case 'E': \
    case 'F'

#define SUBDELIMS \
         '!': case '$': case '&': case '\'': case '(': case ')': case '*': \
    case '+': case ',': case ';': case '='

#define UNRESERVED ALPHA: case DIGIT: case '-': case '.': case '_': case '~'

#define PCHAR SUBDELIMS: case UNRESERVED: case ':': case '@'
// clang-format on

static bool pct_enc_move(const char **pos)
{
    switch (*(*pos)++) {
    case HEXDIGIT:
        break;
    default:
        return false;
    }

    switch (*(*pos)++) {
    case HEXDIGIT:
        return true;
    default:
        return false;
    }
}

static bool parse_ipv4_move(const char **src_pos)
{
    const char *pos = *src_pos;

    for (int i = 0; i < 4; i++) {
        int val = 0;

        if (hlib_isdigit(*pos)) {
            if (*pos == '0') {
                pos++;
            } else {
                val = 10 * val + *pos++ - '0';
                if (hlib_isdigit(*pos)) {
                    val = 10 + val + *pos++ - '0';
                    if (hlib_isdigit(*pos)) {
                        val = 10 + val + *pos++ - '0';
                        if (val > 255)
                            return false;
                    }
                }
            }
        } else {
            return false;
        }

        if (i < 3) {
            if (*pos++ != '.')
                return false;
        }
    }

    *src_pos = pos;
    return true;
}

static bool parse_ipv6_move(const char **src_pos)
{
    const char *pos = *src_pos;
    const char *last = pos;
    int count = 0;
    bool seen = false;

    if (*pos++ != '[')
        return EINVAL;

    if (*pos++ == ':') {
        seen = true;
        if (*pos++ != ':')
            return false;
    }

segment:
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
    } else {
        return false;
    }

    switch (*pos++) {
    case ':':
        if (*pos == ':') {
            if (seen)
                return false;
            seen = true;
            pos++;
        } else {
            count++;
        }

        if (count > 8)
            return false;

        goto segment;
    case '.':
        pos = last;
        if (!parse_ipv4_move(&pos) || *pos++ != ']')
            return false;

        count += 2;
        goto end;

    case ']':
        count++;
        goto end;

    default:
        return false;
    }

end:
    if (seen ? count >= 8 : count != 8)
        return false;

    *src_pos = pos;
    return true;
}

/*
 * For an invalid URI, the contents of `dest` are undefined.
 */
int uri_parse_segments_jmp(const char *src, struct uri_segments *dest)
{
    memset(dest, 0, sizeof(*dest));

    const char *pos = src;
    const char *last = src;

scheme:
    switch (*pos++) {
    case ALPHA:
        goto scheme_loop;
    default:
        pos = last;
        goto authority;
    }

scheme_loop:
    switch (*pos++) {
    case ALPHA:
    case DIGIT:
    case '+':
    case '-':
    case '.':
        goto scheme_loop;
    case ':':
        dest->scheme.str = last;
        dest->scheme.size = pos - last - 1;
        last = pos;
        goto authority;
    default:
        pos = last;
        goto authority;
    }

authority:
    if (*pos++ == '/' && *pos++ == '/') {
        last = pos;
        goto userinfo;
    } else {
        pos = last;
        goto path;
    }

userinfo:
    switch (*pos++) {
    case UNRESERVED:
    case SUBDELIMS:
    case ':':
        goto userinfo;
    case '%':
        if (pct_enc_move(&pos))
            goto userinfo;
        else
            goto error;
    case '@':
        dest->userinfo.str = last;
        dest->userinfo.size = pos - last - 1;
        last = pos;
        goto host;
    default:
        pos = last;
        goto host;
    }

host:
    switch (*pos++) {
    case '[':
        goto host_ipvf;
    default:
        goto host_ipv4;
    }

host_ipvf:
    switch (*pos++) {
    case 'v':
        goto host_ipvf_hex;
    default:
        pos = last;
        goto host_ipv6;
    }

host_ipvf_hex:
    switch (*pos++) {
    case HEXDIGIT:
        goto host_ipvf_hex_loop;
    default:
        goto error;
    }

host_ipvf_hex_loop:
    switch (*pos++) {
    case HEXDIGIT:
        goto host_ipvf_hex_loop;
    case '.':
        goto host_ipvf_tail;
    default:
        goto error;
    }

host_ipvf_tail:
    switch (*pos++) {
    case UNRESERVED:
    case SUBDELIMS:
    case ':':
        goto host_ipvf_tail_loop;
    default:
        goto error;
    }

host_ipvf_tail_loop:
    switch (*pos++) {
    case UNRESERVED:
    case SUBDELIMS:
    case ':':
        goto host_ipvf_tail_loop;
    case ']':
        dest->host_type = URI_HOST_IPVF;
        goto host_end;
    default:
        goto error;
    }

host_ipv6:
    if (parse_ipv6_move(&pos)) {
        dest->host_type = URI_HOST_IPV6;
        goto host_end;
    } else {
        goto error;
    }

host_ipv4:
    if (!parse_ipv4_move(&pos))
        goto host_name;

    switch (*pos) {
    case '/':
    case ':':
    case '?':
    case '#':
    case '\0':
        dest->host_type = URI_HOST_IPV4;
        goto host_end;
    default:
        goto host_name;
    }

host_name:
    switch (*pos++) {
    case UNRESERVED:
    case SUBDELIMS:
        goto host_name;
    case '%':
        if (pct_enc_move(&pos))
            goto host_name;
        else
            goto error;
    default:
        pos--;
        dest->host_type = URI_HOST_NAME;
        goto host_end;
    }

host_end:
    dest->host.str = last;
    dest->host.size = pos - last;

    switch (*pos++) {
    case ':':
        last = pos;
        goto port;
    default:
        last = --pos;
        goto path;
    }

port:
    switch (*pos++) {
    case DIGIT:
        goto port;
    default:
        dest->port.str = last;
        dest->port.size = pos - last - 1;
        last = --pos;
        goto path;
    }

path:
    if (dest->host.str) {
        if (*pos == '/')
            goto path_segments;
        else
            goto path_empty;
    } else if (pos[0] == '/' && pos[1] == '/') {
        goto error;
    } else if (dest->scheme.str) {
        goto path_segments;
    } else {
        goto path_noscheme;
    }

path_noscheme:
    switch (*pos++) {
    case UNRESERVED:
    case SUBDELIMS:
    case '@':
        goto path_segments;
    case '%':
        if (pct_enc_move(&pos))
            goto path_segments;
        else
            goto error;
    case '/':
        goto path_segments;
    default:
        pos--;
        goto path_empty;
    }

path_segments:
    switch (*pos++) {
    case PCHAR:
    case '/':
        goto path_segments;
    case '%':
        if (pct_enc_move(&pos))
            goto path_segments;
        else
            goto error;
    default:
        pos--;
        goto path_empty;
    }

path_empty:
    dest->path.str = last;
    dest->path.size = pos - last;
    last = pos + 1;

    switch (*pos++) {
    case '#':
        goto fragment;
    case '?':
        goto query;
    case '\0':
        goto end;
    default:
        goto error;
    }

query:
    switch (*pos++) {
    case PCHAR:
    case '/':
    case '?':
        goto query;
    case '%':
        if (pct_enc_move(&pos))
            goto query;
        else
            goto error;
    default:
        pos--;
        goto query_end;
    }

query_end:
    dest->query.str = last;
    dest->query.size = pos - last;
    last = pos + 1;

    switch (*pos++) {
    case '#':
        goto fragment;
    case '\0':
        goto end;
    default:
        goto error;
    }

fragment:
    switch (*pos++) {
    case PCHAR:
    case '/':
    case '?':
        goto fragment;
    case '%':
        if (pct_enc_move(&pos))
            goto fragment;
        else
            goto error;
    case '\0':
        dest->fragment.str = last;
        dest->fragment.size = last - pos;
        goto end;
    default:
        goto error;
    }

error:
    return EINVAL;
end:
    return 0;
}

static int parse_port(const char *src)
{
    int port = 0;

    for (int i = 0; i < 5 && hlib_isdigit(*src); i++)
        port = 10 * port + *src++ - '0';

    if (*src || !port || port > 65535)
        return -1;

    return port;
}

// int uri_normalize_scheme(const char *src, char *dest)
// {
//     while (*src)
//         *dest++ = hlib_toupper(*src++);
//     *dest = 0;
//     return false;
// }

// static int pct_enc_load(const char **src)
// {
//     const char *pos = *src;

//     if (*pos++ != '%')
//         return -1;

//     int a = hex_val(*pos++);
//     if (a < 0)
//         return -1;

//     int b = hex_val(*pos++);
//     if (b < 0)
//         return -1;

//     *src = pos;
//     return 16 * a + b;
// }

// int uri_normalize_userinfo(const char *src, char *dest)
// {
//     while (*src) {
//         switch (*src) {
//         case UNRESERVED:
//         case SUBDELIMS:
//         case ':':
//             *dest++ = *src++;
//             break;

//         case '%':
//             int val = pct_enc_load(&src);
//             if (val < 0)
//                 return EINVAL;

//             switch (val) {
//             case UNRESERVED:
//             case SUBDELIMS:
//             case ':':
//                 *dest++ = val;
//                 src += 3;
//                 break;

//             default:
//                 *dest++ = *src++;
//                 *dest++ = hlib_toupper(*src++);
//                 *dest++ = hlib_toupper(*src++);
//                 break;
//             }

//         default:
//             return EINVAL;
//         }
//     }

//     *dest++;
//     return 0;
// }

// int uri_normalize_path(const char *src, char *dest)
// {
//     while (*src) {
//         switch (*src) {
//         case PCHAR:
//         case '/':
//             *dest++ = *src++;
//             break;

//         case '%':
//             int val = pct_enc_load(&src);
//             if (val < 0)
//                 return EINVAL;

//             switch (val) {
//             case PCHAR:
//             case '/':
//                 *dest++ = val;
//                 src += 3;
//                 break;

//             default:
//                 *dest++ = *src++;
//                 *dest++ = hlib_toupper(*src++);
//                 *dest++ = hlib_toupper(*src++);
//                 break;
//             }

//         default:
//             return EINVAL;
//         }
//     }

//     *dest++;
//     return 0;
// }

// int uri_normalize_query(const char *src, char *dest)
// {
//     while (*src) {
//         switch (*src) {
//         case PCHAR:
//         case '/':
//         case '?':
//             *dest++ = *src++;
//             break;

//         case '%':
//             int val = pct_enc_load(&src);
//             if (val < 0)
//                 return EINVAL;

//             switch (val) {
//             case PCHAR:
//             case '/':
//             case '?':
//                 *dest++ = val;
//                 src += 3;
//                 break;

//             default:
//                 *dest++ = *src++;
//                 *dest++ = hlib_toupper(*src++);
//                 *dest++ = hlib_toupper(*src++);
//                 break;
//             }

//         default:
//             return EINVAL;
//         }
//     }

//     *dest++ = 0;
//     return 0;
// }

// int uri_normalize_fragment(const char *src, char *dest)
// {
//     return uri_normalize_query(src, dest);
// }
