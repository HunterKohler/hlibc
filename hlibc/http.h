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
 * Integrates RFC 3986's `remove_dot_segments` routine with general
 * path-normalization: percent encode/decoding, removeing slashes, etc.
 * Allocated a new buffer of necessary length. Note the buffer may not always
 * be smaller than the input if some characters must be percent encoded.
 *
 * Reference:
 * https://datatracker.ietf.org/doc/html/rfc3986#section-5.2.4
 */
char *normalize_path(const char *path);

void destroy_uri(struct URI *uri);
char *uri_repr(const struct URI *uri);
bool uri_equal(const struct URI *a, const struct URI *b);

/*
 * Returns the (technically 'a') reason message for some known HTTP status
 * `code`. If not known, returns NULL.
 */
const char *http_status_reason(int code);

bool http_status_informational(int code);
bool http_status_successful(int code);
bool http_status_redirect(int code);
bool http_status_client_error(int code);
bool http_status_server_error(int code);

#endif
