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
 * Checks if a regex match, is actually a match.
 */
bool regmatch_valid(const regmatch_t *match);

#endif
