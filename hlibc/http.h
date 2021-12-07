#ifndef HLIBC_HTTP_H_
#define HLIBC_HTTP_H_

#include <stdint.h>
#include <hlibc/def.h>
#include <hlibc/array.h>

enum uri_host_type {
    URI_HOST_NONE = 0,
    URI_HOST_NAMED,
    URI_HOST_IP4,
    URI_HOST_IP6,
};

/*
 * Reference:
 * https://datatracker.ietf.org/doc/html/rfc3986
 */
struct URI {
    char *scheme;
    /*
     * Generic interpretation of the 'userinfo' section of a URI is deprecated.
     * Will not parse 'userinfo' apart from verifying it when interpreting a
     * non-scheme-specific URI.
     *
     * Reference:
     * https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.1
     */
    char *userinfo;
    enum uri_host_type host_type;
    char *host;
    uint16_t port;
    char *path;
    char *query;
    char *fragment;
};

int parse_uri(struct URI *restrict uri, const char *input,
              const struct URI *restrict base);

/*
 * Applies semantic-preserving URI normalizations:
 *
 * - Lowercase scheme and host.
 * - Uppercase hexadecimal in percent encodings.
 * - Decode percent encoding of unreserved characters.
 * - Normalizes path.
 */
int normalize_uri(struct URI *uri);

/*
 * `remove_dot_segments` routine. Changes `path` in place. The new size of
 * the null-terminated-byte-string pointed to by `path` is returne.d
 *
 * Reference:
 * https://datatracker.ietf.org/doc/html/rfc3986#section-5.2.4
 */
size_t remove_dot_segments(char *path);

void destroy_uri(struct URI *uri);
char *uri_repr(const struct URI *uri);
bool uri_equal(const struct URI *a, const struct URI *b);

#endif
