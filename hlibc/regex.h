/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 *
 * Regex utilities. Mainly POSIX.
 */

#ifndef HLIBC_REGEX_H_
#define HLIBC_REGEX_H_

#include <regex.h>
#include <hlibc/def.h>

size_t regmatch_len(const regmatch_t *match);

/*
 * Allocated a new buffer at `dest`, and copies the regex match `match` from
 * string. `dest` is set to `NULL` for non-matches.
 */
int regmatch_text(const char *restrict str, const regmatch_t *match,
                  char *restrict *dest);

#endif
