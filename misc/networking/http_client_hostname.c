/*
 * Copyright (C) 2021 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdnoreturn.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s hostname\n", argv[0]);
        exit(1);
    }

    char *hostname = argv[1];
    struct addrinfo *result;
    struct addrinfo hint;

    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;

    int err;
    if ((err = getaddrinfo(hostname, "80", &hint, &result))) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
        exit(1);
    }

    int fd_sock =
        socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (fd_sock < 0) {
        fprintf(stderr, "socket: %s\n", strerror(errno));
        exit(1);
    }

    if (connect(fd_sock, result->ai_addr, result->ai_addrlen)) {
        fprintf(stderr, "connect: %s\n", strerror(errno));
        exit(1);
    }

    freeaddrinfo(result);

    char sendbuf[4096];
    char recvbuf[4096];

    strcpy(sendbuf, "GET / HTTP/1.1\r\n"
                    "Connection: close\r\n"
                    "\r\n");

    if (send(fd_sock, sendbuf, strlen(sendbuf), 0) < 0) {
        fprintf(stderr, "send: %s\n", strerror(errno));
        exit(1);
    }

    int n;
    while ((n = recv(fd_sock, recvbuf, 4096, 0)) > 0) {
        printf("%.*s", n, recvbuf);
    }

    if (n < 0) {
        fprintf(stderr, "recv: %s\n", strerror(errno));
        exit(1);
    }
}
