/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#ifndef RELJMP_H_
#define RELJMP_H_

#include <stdint.h>

struct reljmp {
    uint8_t opcode;
    int32_t len;
} __attribute__((packed));

/*
 * Initialize `struct reljmp`.
 */
int reljmp_init(struct reljmp *jmp, int32_t len);

/*
 * `rwx` permissions are neccesary when functions are on the same page as
 * `func` at the time of protection changes. Than may include `replace_func`
 * itself.
 *
 * Errors:
 * Ought to inherit from `mprotect` on the system. Will set and return `errno`
 * on failure.
 *
 * Note:
 * MACOS requires compiling with special max permissions on the __TEXT segment
 * `-segprot __TEXT rwx rx`, or linking dynamically.
 */
int replace_func(void *func, void *repl);

#endif
