/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#ifndef HLIBC_INET_H_
#define HLIBC_INET_H_

#include <hlibc/def.h>

/*
 * Get the string representation of an IPV4 address.
 */
void ipv4_to_str(const void *restrict src, char *restrict dest);

/*
 * Get the binary representation of an IPV4 address string. Returns `EINVAL` on
 * error.
 */
int ipv4_to_bin(const char *restrict src, void *restrict dest);

/*
 * Get the string representation of an IPV6 address.
 */
void ipv6_to_str(const void *restrict src, char *restrict dest);

/*
 * Get the binary representation of an IPV6 address string. Returns `EINVAL` on
 * error.
 */
int ipv6_to_bin(const char *restrict src, void *restrict dest);

/*
 * Normalize and IPV6 address string. Return `EINVAL` on error.
 */
int ipv6_norm(const char *restrict src, char *restrict dest);

#endif
