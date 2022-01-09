/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <ctype.h>
#include <hlibc/string.h>
#include <hlibc/http.h>

/*
 * tchar = "!" / "#" / "$" / "%" / "&" / "'" / "*" / "+" / "-" / "." /
 *         "^" / "_" / "`" / "|" / "~" / DIGIT / ALPHA
 */
static bool is_tchar(char c)
{
    return isalnum(c) || strchr("!#$%&'*+-.^_`|~", c);
}

/*
 * reason-phrase  = *( HTAB / SP / VCHAR / obs-text )
 */
static bool is_reason_char(char c)
{
    unsigned char v = c;
    return v == 0x9 || (v >= 0x20 && v <= 0x7E) || (v >= 0x80);
}

char *find_eol(const char *ptr)
{
    return strstr(ptr, "\r\n");
}

enum http_msg_type {
    HTTP_REQUEST,
    HTTP_RESPONSE,
};

struct http_msg {
    char *method;
    char *reason;

    int status;
    int major;
    int minor;

    enum http_msg_type type;
};

// int http_parse_version(struct http_msg *msg, const char *src)
// {
//     if (src[0] != 'H' || src[1] != 'T' || src[2] != 'T' || src[3] != 'P' ||
//         src[4] != '/' || !isdigit(src[5]) || src[6] != '.' || !isdigit(src[7]))
//         return EINVAL;
//     msg->major == src[5] - '0';
//     msg->minor == src[7] - '0';
//     return 0;
// }

// int http_parse_status(struct http_msg *msg, const char *src)
// {
//     if(!isdigit(src[0]) || !isdigit(src[1]) || !isdigit(src[2]))
//         return EINVAL;
//     msg->status = 100 * (src[0] - '0') + 10 * (src[1] - '0') + (src[2] - '0');
//     return 0;
// }

// int http_parse_startline(struct http_msg *msg, const char *src, size_t *dist)
// {
//     const char *pos = src;

//     if (http_parse_version(msg, src)) {
//         const char *sp = pos + 8

//         msg->type = HTTP_RESPONSE;
//         pos += 8;

//         if(pos[0] != ' ')
//             goto error_inval;

//         pos += 1;

//         if(http_parse_status(msg))

//         if(*pos[0] != ' ')

//         if(*pos[0] != ' ' || http_parse_status(msg, pos + 1) || pos[4] != ' ')
//             goto error_inval;

//         pos += 5;
//         msg->status =
//             100 * (pos[4] - '0') + 10 * (pos[5] - '0') + (pos[6] - '0');

//         pos += 8;
//         msg->reason = pos;

//         while (is_reason_char(pos[0]))
//             pos++;

//         if (memcmp(pos, "\r\n", 2))
//             goto error_inval;

//         msg->reason = strndup(msg->reason, pos - msg->reason);
//         if (!msg->reason)
//             goto error_nomem;

//     } else {
//         msg->type = HTTP_REQUEST;
//         msg->method = pos;

//         while (is_tchar(pos[0]))
//             pos++;

//         if (*pos != ' ' || pos == msg->method)
//             goto error_inval;

//         msg->method = strndup(msg->reason, pos - msg->method);
//         if (!msg->method)
//             goto error_nomem;

//         // actually parse uri here
//         // ----------------
//         pos = strpbrk(pos + 1, ' ');
//         // ----------------

//         if (pos[0] != ' ')
//             goto error_inval;
//     }
// }

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
