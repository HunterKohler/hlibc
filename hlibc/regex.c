/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <hlibc/string.h>
#include <hlibc/regex.h>

bool regmatch_valid(const regmatch_t *match)
{
    return match && match->rm_so >= 0 && match->rm_so <= match->rm_eo;
}

size_t regmatch_len(const regmatch_t *match)
{
    return regmatch_valid(match) ? 0 : match->rm_eo - match->rm_so;
}
