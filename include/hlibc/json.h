/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 *
 * JSON parsing and construction.
 */

#ifndef HLIBC_JSON_H_
#define HLIBC_JSON_H_

#include <hlibc/def.h>

/*
 * JSON token types.
 */
enum json_token_type {
    JSON_TOKEN_BEGIN_ARRAY,
    JSON_TOKEN_BEGIN_OBJECT,
    JSON_TOKEN_END_ARRAY,
    JSON_TOKEN_END_OBJECT,
    JSON_TOKEN_NAME_SEP,
    JSON_TOKEN_VALUE_SEP,
    JSON_TOKEN_FALSE,
    JSON_TOKEN_TRUE,
    JSON_TOKEN_NULL,
    JSON_TOKEN_MINUS,
    JSON_TOKEN_INT,
    JSON_TOKEN_FRAC,
    JSON_TOKEN_EXP,
    JSON_TOKEN_STRING,
};

/*
 * Description of JSON token.
 */
struct json_token {
    enum json_token_type type;
    char *begin;
    char *end;
};

/*
 * Error codes used in JSON library functions.
 */
enum json_error {
    JSON_ENOMEM,
    JSON_EINVAL,
    JSON_EEOF,
};

/*
 * Returns a pointer to a string describing a value of `enum json_error`.
 *
 * The return value must not be modified by the application, nor any other
 * library function, but may be modified by subsequent calls to `json_strerror`.
 * Returns `NULL` if `err` is not a valid `enum json_error`.
 */
char *json_strerror(int err);

/*
 * Store the next input token from `str` in `token`.
 *
 * On success, `json_next_token` returns `0`, otherwise, it returns some
 * `enum json_error` as an integer.
 */
int json_next_token(char *str, struct json_token *token);

// enum json_type {
//     JSON_ARRAY,
//     JSON_OBJECT,
//     JSON_STRING,
//     JSON_INT,
//     JSON_FLOAT,
//     JSON_BOOL,
//     JSON_NULL,
// };

#endif
