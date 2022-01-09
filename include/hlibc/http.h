/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIBC_HTTP_H_
#define HLIBC_HTTP_H_

#include <stdint.h>
#include <hlibc/def.h>
#include <hlibc/array.h>

/*
 * Returns the (technically 'a') reason message for some known HTTP status
 * `code`. If not known, returns NULL.
 */
const char *http_status_reason(int code);

bool http_status_informational(int code);
bool http_status_successful(int code);
bool http_status_redirect(int code);
bool http_status_client_error(int code);
bool http_status_server_error(int code);

#endif
