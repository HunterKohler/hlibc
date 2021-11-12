/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */
#ifndef HLIBC_DSTRING_H_
#define HLIBC_DSTRING_H_

#include <string.h>

char *ds_new(size_t n);

size_t ds_len(const char *ds);

size_t ds_cap(const char *ds);

void ds_free(char *ds);

char *ds_dup(const char *ds);

int ds_reserve(char **ds_ptr, size_t n);

int ds_resize(char **ds_ptr, size_t n, char c);

char *ds_substr(const char *str, size_t a, size_t b);

int ds_cmp(const char *ds_1, const char *ds_2);

#endif
