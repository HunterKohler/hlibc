/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <limits.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "common.h"

#define PORT 8000
#define BACKLOG 32

int main(int argc, char **argv)
{
    int fd_sock;
    long maxres;
    struct sockaddr_in addr;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s maxres\n", argv[0]);
        exit(1);
    } else if ((maxres = strtol(argv[1], NULL, 10)) <= 0 || errno) {
        die("Invalid maxres");
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((fd_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        die("socket");
    }

    if ((bind(fd_sock, (struct sockaddr *)&addr, sizeof(addr)))) {
        die("bind");
    };

    if ((listen(fd_sock, BACKLOG))) {
        die("listen");
    }

    while (maxres-- > 0) {
        int fd_conn;
        char addrstr[INET_ADDRSTRLEN];
        struct sockaddr_in client_addr;
        socklen_t client_addrlen = sizeof(client_addr);

        if ((fd_conn = accept(fd_sock, (struct sockaddr *)&client_addr,
                              &client_addrlen)) < 0) {
            die("accept");
        }

        if (!inet_ntop(AF_INET, &client_addr.sin_addr, addrstr,
                       INET_ADDRSTRLEN)) {
            die("inet_ntop");
        }

        printf("Connection from: %s\n", addrstr);
        close(fd_conn);
    }

    close(fd_sock);
}
