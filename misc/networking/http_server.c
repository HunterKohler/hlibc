/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>

#include "common.h"

#define PORT 8000
#define BACKLOG 16

int main(int argc, char **argv)
{
    if (argc == 1) {
        fprintf(stderr, "Usage: %s messages...\n", argv[0]);
        exit(1);
    }

    int fd_sock;
    struct sockaddr_in addr;

    if ((fd_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        die("socket");
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(fd_sock, (struct sockaddr *)&addr, sizeof(addr))) {
        die("socket");
    }

    if (listen(fd_sock, BACKLOG)) {
        die("listen");
    }

    for (int i = 1; i < argc; i++) {
        int n;
        int fd_conn;
        char recvbuf[4096];
        char sendbuf[4096];

        if ((fd_conn = accept(fd_sock, NULL, NULL)) < 0) {
            die("accept");
        }

        while ((n = recv(fd_conn, recvbuf, 4096, 0)) > 0) {
            printf("%.*s", n, recvbuf);

            if (memcmp(recvbuf + n - 4, "\r\n\r\n", 4)) {
                break;
            }
        }

        if (n < 0) {
            die("recv");
        }

        sprintf(sendbuf,
                "HTTP/1.1 200 OK\r\n"
                "Content-Length: %zu\r\n"
                "Content-Type: text/plain; charset=utf-8\r\n"
                "\r\n"
                "%s",
                strlen(argv[i]), argv[i]);

        if (send(fd_conn, sendbuf, strlen(sendbuf), 0) < 0) {
            die("send");
        }

        if (close(fd_conn)) {
            die("close");
        }
    }

    if (close(fd_sock)) {
        die("close");
    }
}
