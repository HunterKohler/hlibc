/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <hlibc/string.h>
#include <hlibc/regex.h>

static inline bool regmatch_valid(const regmatch_t *match)
{
    return match->rm_so >= 0 && match->rm_so <= match->rm_eo;
}

size_t regmatch_len(const regmatch_t *match)
{
    return regmatch_valid(match) ? 0 : match->rm_eo - match->rm_so;
}

int regmatch_text(const char *restrict str, const regmatch_t *match,
                  char *restrict *dest)
{
    if (regmatch_valid(match)) {
        size_t len = regmatch_len(match);
        char *buf = memdup(str + match->rm_so, len + 1);

        if (!buf)
            return ENOMEM;

        buf[len] = 0;
        *dest = buf;
        return 0;
    } else {
        *dest = NULL;
        return 0;
    }
}
