/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <hlibc/sys.h>
#include <hlibc/reljump.h>

/*
 * `rwx` permissions are neccesary when functions are on the same page as
 * `func` at the time of protection changes. Than may include `replace_func`
 * itself.
 *
 * Note:
 * MACOS requires compiling with special max permissions on the __TEXT segment
 * `-segprot __TEXT rwx rx`, or linking dynamically.
 */
int replace_func(void *func, void *repl)
{
    struct reljump op = { 0xE9, repl - (func + sizeof(op)) };

    void *page = pageof(func);
    if (mprotect(page, sizeof(op), PROT_READ | PROT_EXEC | PROT_WRITE))
        return errno;

    memcpy(func, &op, sizeof(op));

    if (mprotect(page, sizeof(op), PROT_READ | PROT_EXEC))
        return errno;
    return 0;
}
