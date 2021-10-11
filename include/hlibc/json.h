/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIBC_JSON_H_
#define HLIBC_JSON_H_

#include <hlibc/def.h>

int hexdigit(char c);

enum json_type {
    JSON_NULL,
    JSON_STR,
    JSON_NUM,
    JSON_BOOL,
    JSON_OBJ,
    JSON_ARR,
    JSON_ERR
};

struct json_object {
};

struct json_value {
    enum json_type type;
    union {
        void *null;
        double num;
        char *str;
        struct json_object *obj;
        struct json_value **arr;
    };
};

struct json_value *json_load(const char *src);

int _json_get_value(const char *pos, struct json_value *json);
int _json_get_obj(const char *pos, struct json_value *json);
int _json_get_str(const char *src, struct json_value *json);
int _json_get_num(const char *src, struct json_value *json);
int _json_get_arr(const char *str, struct json_value *json);

#endif
