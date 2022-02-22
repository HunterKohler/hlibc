/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 *
 * Thoughts about a test driver. Not great lol.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdnoreturn.h>
#include <dlfcn.h>
#include <unistd.h>
#include <unistdio.h>
#include <setjmp.h>
#include <stdarg.h>
#include <errno.h>

int main(int argc, char **argv)
{
    const char *path = "build/test/test.so";
    const char *symbol = "htest_run_global_suites";

    int fdout[2];
    int fderr[2];

    pipe(fdout);
    pipe(fderr);

    pid_t child = fork();

    if (child) {
        close(fdout[1]);
        close(fderr[1]);

        size_t bufsize_out = 5000;
        size_t bufsize_err = 5000;
        char *buf_out = malloc(bufsize_out);
        char *buf_err = malloc(bufsize_err);
        ssize_t readlen_out = read(fdout[0], buf_out, bufsize_out);
        ssize_t readlen_err = read(fderr[0], buf_err, bufsize_err);

        perror("");

        int code;
        wait(&code);

        printf("out: %s\n", buf_out);
        printf("err: %s\n", buf_out);
        printf("%d\n", code);
    } else {
        close(fdout[0]);
        close(fderr[0]);

        dup2(fdout[1], STDOUT_FILENO);
        dup2(fderr[1], STDERR_FILENO);

        void *handle = dlopen(path, RTLD_NOW);
        void (*run)() = dlsym(handle, symbol);

        run();
    }
}
