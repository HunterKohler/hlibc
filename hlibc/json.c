/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <ctype.h>
#include <hlibc/json.h>

int json_next_token(char *str, struct json_token *token)
{
    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r')
        str++;

    switch (*str) {
    case '[':
        token->type = JSON_TOKEN_BEGIN_ARRAY;
        token->begin = str;
        token->end = str + 1;
        return 0;

    case '{':
        token->type = JSON_TOKEN_BEGIN_OBJECT;
        token->begin = str;
        token->end = str + 1;
        return 0;

    case ']':
        token->type = JSON_TOKEN_END_ARRAY;
        token->begin = str;
        token->end = str + 1;
        return 0;

    case '}':
        token->type = JSON_TOKEN_END_OBJECT;
        token->begin = str;
        token->end = str + 1;
        return 0;

    case ':':
        token->type = JSON_TOKEN_NAME_SEP;
        token->begin = str;
        token->end = str + 1;
        return 0;

    case ',':
        token->type = JSON_TOKEN_VALUE_SEP;
        token->begin = str;
        token->end = str + 1;
        return 0;

    case 'f':
        if (!str[1] || !str[2] || !str[3] || !str[4])
            return JSON_EEOF;
        else if (str[1] != 'a' || str[2] != 'l' || str[3] != 's' ||
                 str[4] != 'e')
            return JSON_EINVAL;

        token->type = JSON_TOKEN_FALSE;
        token->begin = str;
        token->end = str + 5;
        return 0;

    case 't':
        if (!str[1] || !str[2] || !str[3])
            return JSON_EEOF;
        else if (str[1] != 'r' || str[2] != 'u' || str[3] != 'e')
            return JSON_EINVAL;

        token->type = JSON_TOKEN_TRUE;
        token->begin = str;
        token->end = str + 4;
        return 0;

    case 'n':
        if (!str[1] || !str[2] || !str[3])
            return JSON_EEOF;
        else if (str[1] != 'u' || str[2] != 'l' || str[3] != 'l')
            return JSON_EINVAL;

        token->type = JSON_TOKEN_NULL;
        token->begin = str;
        token->end = str + 4;
        return 0;

    case '-':
        token->type = JSON_TOKEN_MINUS;
        token->begin = str;
        token->end = str + 1;
        return 0;

    case '0':
        if (isdigit(str[1]))
            return JSON_EINVAL;
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        token->type = JSON_TOKEN_INT;
        token->begin = str++;

        while (isdigit(*str))
            str++;

        token->end = str;
        return 0;

    case 'e':
    case 'E':
        token->type = JSON_TOKEN_EXP;
        token->begin = str++;

        if (*str == '+' || *str == '-')
            str++;

        if (!isdigit(*str++))
            return JSON_EINVAL;

        while (isdigit(*str))
            str++;

        token->end = str;
        return 0;

    case '"':
        token->type = JSON_TOKEN_STRING;
        token->begin = str++;

        while (*str) {
            if (*str == '"') {
                token->end = str++;
                return 0;
            } else if (*str == '\\') {
                switch (str[1]) {
                case '"':
                case '\\':
                case '/':
                case 'b':
                case 'f':
                case 'n':
                case 'r':
                case 't':
                    str += 2;
                    break;

                case 'u':
                    if (isxdigit(str[2]) && isxdigit(str[3]) &&
                        isxdigit(str[4]) && isxdigit(str[5])) {
                        str += 6;
                        break;
                    }

                default:
                    return JSON_EINVAL;
                }
            } else if (0x20 <= *str) {
                str++;
            } else {
                return JSON_EINVAL;
            }
        }
        return JSON_EEOF;

    case '\0':
        return JSON_EEOF;

    default:
        return JSON_EINVAL;
    }
}

char *json_strerror(int err)
{
    switch (err) {
    case JSON_ENOMEM:
        return "Failed to allocate memory";
    case JSON_EINVAL:
        return "Invalid JSON";
    case JSON_EEOF:
        return "Unexpected end of file";
    default:
        return NULL;
    }
}
