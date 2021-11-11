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

    struct addrinfo hints;
    struct addrinfo *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_CANONNAME;

    int err;
    if ((err = getaddrinfo(argv[1], "80", &hints, &res))) {
        fprintf(stderr, "getaddrinfo() failed: %s\n", gai_strerror(err));
        exit(1);
    }

    for (struct addrinfo *it = res; it; it = it->ai_next) {
        char addr_str[INET6_ADDRSTRLEN];
        void *ptr;

        if (it->ai_family == AF_INET) {
            ptr = &((struct sockaddr_in *)it->ai_addr)->sin_addr;
        } else {
            ptr = &((struct sockaddr_in6 *)it->ai_addr)->sin6_addr;
        }

        if (!(inet_ntop(it->ai_family, ptr, addr_str, INET6_ADDRSTRLEN))) {
            fprintf(stderr, "inet_ntop() failed: %s\n", strerror(errno));
            exit(1);
        }

        printf("Hostname: %s\nIPv%c: %s\n%s", it->ai_canonname,
               it->ai_family == AF_INET ? '4' : '6', addr_str,
               it->ai_next ? "\n" : "");
    }
}
