/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include <hlibc/json.h>

int hexdigit(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

/*

struct json_value *json_load(const char *src)
{
    struct json_value *json = malloc(sizeof(*json));

    if (json) {
        _json_get_value(src, json);
    } else {
        return; // error;
    }

    return json;
}

int _json_get_value(const char *pos, struct json_value *json)
{
    while (*pos) {
        switch (*pos) {
        case '{':
            _json_get_obj(pos, src);
            break;
        case '[':
            _json_get_arr(pos, src);
            break;
        case '\"':
            _json_get_str(pos, src);
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case '-':
            _json_get_num(pos, src);
            break;
        case 't':
            if (!memcmp(pos, "true", 4)) {
                json->type = JSON_BOOL;
                json->buf = malloc(sizeof(bool));
                if (!json->buf) {
                    return; //errors
                }
                *json->buf = true;
            } else {
                return; // error
            }
            break;
        case 'f':
            if (!memcmp(pos, "false", 5)) {
                json->type = JSON_BOOL;
                json->buf = malloc(sizeof(bool));
                if (!json->buf) {
                    return; //errors
                }
                *json->buf = true;
            } else {
                return; // error
            }
            break;
        case 'n':
            if (!memcmp(pos, "null", 4)) {
                json->type = JSON_NULL;
                json->buf = NULL;
                pos += 4;
            } else {
                return; // error
            }
            break;
        case ' ':
        case '\t':
        case '\n':
        case '\v':
        case '\f':
        case '\r':
            break;
        default:
            return; // error
        }
    }
}

int _json_get_obj(const char *pos, struct json_value *json)
{
}


int _json_get_str(const char *src, struct json_value *json)
{
    size_t size = 0;
    size_t buf_size = 10;
    char *buf = malloc(buf_size);

    int i = 0;
    while (src[++i] != '"') {
        if (size == buf_size) {
            void *tmp = realloc(buf, buf_size = buf_size * 3 / 2);
            if (!tmp) {
                return; // error
            }
            buf = tmp;
        }

        if (iscntrl(src[i])) {
            return; // invalid ctrl char err
        } else if (src[i] == '\\') {
            switch (src[++i]) {
            case '\\':
                buf[size++] = '\\';
                break;
            case '/':
                buf[size++] = '/';
                break;
            case '"':
                buf[size++] = '"';
                break;
            case 'b':
                buf[size++] = '\b';
                break;
            case 'f':
                buf[size++] = '\f';
                break;
            case 'n':
                buf[size++] = '\n';
                break;
            case 'r':
                buf[size++] = '\r';
                break;
            case 't':
                buf[size++] = '\t';
                break;
            case 'u':
                // assuming valid codepoint for now
                buf[size++] = hexdigit(src[i++]) * 16 + hexdigit(src[i++]);
                buf[size++] = hexdigit(src[i++]) * 16 + hexdigit(src[i++]);
                break;
            default:
                return; // invalid escape err
            }
        } else {
            buf[size++] = src[i];
        }
    }
    buf[size++] = '\0';

    char *ptr = realloc(buf, size);
    if (!ptr) {
        return; // error memory
    }

    json->buf = ptr;
    json->type = JSON_STR;

    return i;
}

*/

int _json_get_num(const char *src, struct json_value *json)
{
    bool negative = *src == '-';
    int i = negative;

    double num = 0;
    while (isdigit(src[i])) {
        num = 10 * num + src[i++] - '0';
    }

    if (src[i] == '.') {
        i++;
        int place = 1;
        while (isdigit(src[i])) {
            num += src[i++] / (place * 10);
        }
    }

    int exp = 0;
    if (src[i] == 'e' || src[i] == 'E') {
        bool exp_negative = src[++i] == '-';
        i += exp_negative;

        while (isdigit(src[i])) {
            exp = exp * 10 + src[i++];
        }

        if (exp_negative) {
            exp = -exp;
        }

        num *= pow(10, exp);
    }

    json->buf = malloc(sizeof(double));
    json->type = JSON_NUM;

    return i;
}

/* Unimplemented

int _json_get_arr(const char *str, struct json_value *json)
{
    size_t arr_size = 0;
    size_t arr_cap = 10;

    struct json_value **arr = malloc(arr_cap * sizeof(*arr));

    int i = 0;
    while (str[i] != ']') {
        if (arr_size == arr_cap) {
            arr_cap = arr_cap * 3 / 2;
            struct json_value **ptr = realloc(arr, arr_cap * sizeof(*ptr));
            if (!ptr) {
                free(arr);
                return; // error
            }
        }

        int di = _json_get_value();
    }

    return i;
}

*/
