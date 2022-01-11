/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <hlibc/sys.h>
#include <hlibc/reljmp.h>

int reljmp_init(struct reljmp *jmp, int32_t len)
{
    jmp->opcode = 0xE9;
    jmp->len = len;
    return 0;
}

int replace_func(void *func, void *repl)
{
    struct reljmp op;
    void *page = pageof(func);
    int len = func - page + sizeof(op);
    int jmp_len = repl - (func + sizeof(op));

    reljmp_init(&op, jmp_len);

    if (mprotect(page, len, PROT_READ | PROT_EXEC | PROT_WRITE))
        return errno;

    memcpy(func, &op, sizeof(op));

    if (mprotect(page, len, PROT_READ | PROT_EXEC))
        return errno;
    return 0;
}
