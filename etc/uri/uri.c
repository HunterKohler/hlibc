#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <hlibc/math.h>
#include "uri.h"

const char *host_type_str(enum uri_host_type type)
{
    switch (type) {
    case URI_HOST_IPV4:
        return "ipv4";
    case URI_HOST_IPV6:
        return "ipv6";
    case URI_HOST_IPVF:
        return "ipvf";
    case URI_HOST_NAME:
        return "name";
    default:
        return "";
    }
}

int print_uri_segments(FILE *stream, const struct uri_segments *seg)
{
    const char *host_type = seg->host.str ? host_type_str(seg->host_type) : "";

    fprintf(stream,
            "{\n"
            "    scheme    := \"%.*s\"\n"
            "    userinfo  := \"%.*s\"\n"
            "    host      := \"%.*s\"\n"
            "    host_type := \"%.*s\"\n"
            "    port      := \"%.*s\"\n"
            "    path      := \"%.*s\"\n"
            "    query     := \"%.*s\"\n"
            "    fragment  := \"%.*s\"\n"
            "}\n",
            seg->scheme.size, seg->scheme.str, //
            seg->userinfo.size, seg->userinfo.str, //
            seg->host.size, seg->host.str, //
            strlen(host_type), host_type, //
            seg->port.size, seg->port.str, //
            seg->path.size, seg->path.str, //
            seg->query.size, seg->query.str, //
            seg->fragment.size, seg->fragment.str);
}

__attribute__((format(printf, 1, 2))) noreturn void fatal(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    exit(1);
}

int benchmark(char *const *input, size_t size)
{
    struct uri_segments *dest = malloc(sizeof(*dest));

    for (int i = 0; i < size; i++) {
        uri_parse_segments_re(input[i], dest);
    }

    int reps = 1000;
    clock_t start = clock();

    for (int j = 0; j < reps; j++) {
        for (int i = 0; i < size; i++) {
            uri_parse_segments_re(input[i], dest);
        }
    }

    clock_t end = clock();

    printf("jmp: %lf sec\n", (double)(end - start) / CLOCKS_PER_SEC);
}

int main(size_t argc, size_t argv)
{
    FILE *src = fopen("./uri.txt", "r");
    size_t buf_size = 8000;
    char *buf = malloc(buf_size);
    size_t line_count = 0;

    while (fgets(buf, buf_size, src)) {
        line_count++;
    }

    fseek(src, 0, SEEK_SET);
    char **lines = malloc(line_count * sizeof(*lines));

    for (int i = 0; i < line_count; i++) {
        fgets(buf, buf_size, src);
        size_t len = strlen(buf);
        buf[len - (len > 0)] = 0;
        lines[i] = strdup(buf);
    }

    benchmark(lines, line_count);

    // for (int i = 0; i < line_count; i++) {
    //     char *line = lines[i];
    //     printf("=========================\n"
    //            "input := %s\"\n"
    //            "output := ",
    //            line);
    //     int error = uri_parse_segments_switch(line, seg);

    //     if (error)
    //         printf("%s\n", strerror(error));
    //     else
    //         print_uri_segments(stdout, seg);
    // }

    // while(fgets(),)

    // struct uri_segments *seg = malloc(sizeof(*seg));

    // for (int i = 0; i < array_size(input); i++) {
    //     const char *src = input[i];
    //     int error = uri_parse_segments_switch(src, seg);

    //     if (error) {
    //         fprintf(stderr, "Error: %s\n", strerror(error));
    //     } else {
    //         printf("input := %s\noutput := ", src);
    //         print_uri_segments(stdout, seg);
    //     }
    // }
}
