/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#ifndef HLIBC_CTYPE_H_
#define HLIBC_CTYPE_H_

#include <hlibc/def.h>

static inline bool hlib_isalpha(char c)
{
    extern const bool isalpha_table[256];
    return isalpha_table[(unsigned char)c];
}

static inline bool hlib_isalnum(char c)
{
    extern const bool isalnum_table[256];
    return isalnum_table[(unsigned char)c];
}

static inline bool hlib_isdigit(char c)
{
    extern const bool isdigit_table[256];
    return isdigit_table[(unsigned char)c];
}

static inline bool hlib_isxdigit(char c)
{
    extern const bool isxdigit_table[256];
    return isxdigit_table[(unsigned char)c];
}

static inline bool hlib_toupper(char c)
{
    extern const char toupper_table[256];
    return toupper_table[(unsigned char)c];
}

static inline bool hlib_tolower(char c)
{
    extern const char tolower_table[256];
    return tolower_table[(unsigned char)c];
}

#endif
