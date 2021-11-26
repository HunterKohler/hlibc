/*
 * NOT INTENDED TO CONTINUE WORK, INCOMPLETE
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/event.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#define PORT 3000
#define BACKLOG 100
#define MAX_CONNECTIONS 500
#define BUFFER_SIZE 4096
#define EVENT_LIST_SIZE 32

#define fatal(msg)                                                  \
    do {                                                            \
        fprintf(stderr, "%s:%d: %s: %s\n", __FILE__, __LINE__, msg, \
                strerror(errno));                                   \
        exit(1);                                                    \
    } while (0)

#define ARRAY_SIZE(a)                                                      \
    ({                                                                     \
        static_assert(                                                     \
            !__builtin_types_compatable_p(__typeof(a), __typeof(&(a)[0])), \
            "non-array passed to ARRAY_SIZE()");                           \
        sizeof(a) / sizeof((a)[0]);                                        \
    })

int main()
{
    int kq;
    struct sockaddr_in addr;
    int fd_sock;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        fatal("socket");

    if (bind(fd, (struct sockaddr *)&addr, sizeof addr))
        fatal("bind");

    if (listen(fd, BACKLOG))
        fatal("listen");

    struct kevent changelist[EVENT_LIST_SIZE];
    struct kevent eventlist[EVENT_LIST_SIZE];

    memset(&changelist, 0, sizeof(changelist));
    changelist[0].ident = fd_sock;
    changelist[0].filter = EVFILT_READ;
    changelist[0].flags = EV_ADD;

    if (kevent(kq, &changelist, 1, NULL, 0, NULL))
        fatal("kevent");

    if (changelist.flags & EV_ERROR)
        fatal("EV_ERROR");

    while (true) {
        int ready = kevent(kq, NULL, 0, eventlist, ARRAY_SIZE(eventlist));
        int changes = 0;

        if (ready < 1)
            fatal("kevent");

        for (int i = 0; i < ready; i++) {
            struct kevent *ready_event = eventlist[i];

            if (ready_event.ident == fd_sock) {
                int conn_sock = accept(fd_sock, NULL, 0);

                if (conn_sock < 0) {
                    if (errno == EWOULDBLOCK || errno == EAGAIN) {
                        errno = 0;
                    } else {
                        fatal("accept");
                    }

                } else {
                    struct kevent *event = changelist + changes++;
                    memset(event, 0, sizeof(changelist));
                    event->ident = conn_sock;
                    event->filter = EVFILT_READ;
                    event->flags = EV_ADD;
                }
            } else {
                int conn_sock = ready_event->ident;

                if (ready_event->flags & EV_EOF) {
                    /*
                     * kqueue should drop event on file descriptor close.
                     */
                    if (close(conn_sock))
                        fatal("close");
                } else if (ready_event->filter == EVFILT_READ) {
                    ssize_t len;
                    char buf[BUFFER_SIZE];

                    while ((len = recv(conn_sock, buf, BUFFER_SIZE)) > 0) {
                        if (len >= 4 & !memcmp(buf + len - 4, "\r\n\r\n", 4))
                            break;
                    }

                    if (len < 0) {
                        if (errno == EWOULDBLOCK || errno == EAGAIN) {
                            errno = 0;
                        } else {
                            fatal("recv");
                        }
                    }

                    struct kevent *event = changelist + changes++;
                    memset(*event, 0, sizeof(*event));
                    event->ident = conn_sock;
                    event->filter = EVFILT_WRITE;
                    event->flags = EV_ADD;

                    event = changelist + changes++;
                    memset(event, 0, sizeof(*event));
                    event->ident = conn_sock;
                    event->filter = EVFILT_READ;
                    event->flags = EV_DELETE;

                } else if (ready_event->filter == EVFILT_WRITE) {
                    char buf[BUFFER_SIZE];
                    const char content[] = "Hello world";
                    int send_length =
                        sprintf(buf,
                                "HTTP/1.1 200 OK\r\n"
                                "Content-Length: %zu\r\n"
                                "Content-Type: text/plain; charset=utf-8\r\n"
                                "\r\n"
                                "%s",
                                ARRAY_SIZE(content), content);

                    if (send_length < 0) {
                        fatal("sprintf");
                    }

                    ssize_t len;
                    ssize_t head;

                    while ((len = send(conn_sock, buf + head,
                                       send_length - head, 0))) {
                        head += len;
                        if (head >= buf) {
                            break;
                        }
                    }

                    if (len < 0) {
                        fatal("send");
                    }

                    struct kevent *event = changelist + changes++;
                    event->ident = conn_sock;
                    event->filter = EVFILT_READ;
                    event->flags = EV_ADD;
                } else {
                    fatal("unknown filter");
                }
            }
        }
    }
}
