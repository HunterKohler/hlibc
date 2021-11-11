/*
 * Copyright (C) 2021 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdnoreturn.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define LINESIZE 4096

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s url\n", argv[0]);
        exit(1);
    }

    char *ip = argv[1];

    int sock_fd;
    struct sockaddr_in servaddr;

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(80);

    switch (inet_pton(AF_INET, ip, &servaddr.sin_addr)) {
    case 0:
        fprintf(stderr, "invalid network address: '%s'\n", ip);
        exit(1);
    case -1:
        fprintf(stderr, "inet_pton() failed: %s\n", strerror(errno));
        exit(1);
    }

    printf("address interpreted\n");

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "socket() failed: %s\n", strerror(errno));
        exit(1);
    }

    if (connect(sock_fd, (struct sockaddr *)&servaddr, sizeof(servaddr))) {
        fprintf(stderr, "connect() failed: %s\n", strerror(errno));
        exit(1);
    }

    char sendbuf[LINESIZE];
    strcpy(sendbuf, "GET / HTTP/1.1\r\n"
                    "Connection: close\r\n"
                    "\r\n");
    if (send(sock_fd, sendbuf, strlen(sendbuf), 0) != strlen(sendbuf)) {
        fprintf(stderr, "send() failed: %s\n", strerror(errno));
        exit(1);
    }

    int n;
    char recvbuf[LINESIZE];
    while ((n = read(sock_fd, recvbuf, LINESIZE)) > 0) {
        printf("%.*s", n, recvbuf);
    }

    if (n < 0) {
        fprintf(stderr, "recv() failed: %s\n", strerror(errno));
        exit(1);
    }

    close(sock_fd);
}
