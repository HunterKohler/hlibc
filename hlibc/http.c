#include <stdlib.h>
#include <ctype.h>
#include <regex.h>
#include <pthread.h>
#include <hlibc/def.h>
#include <hlibc/string.h>
#include <hlibc/regex.h>
#include <hlibc/http.h>

regex_t re_uri;
regex_t re_ip4;
regex_t re_hostname;

static void http_module_init_routine()
{
    regcomp(&re_uri,
            "^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?$",
            REG_EXTENDED);
    regcomp(
        &re_ip4,
        "^((25[0-5]|2[0-4]\\d|1?\\d{1,2})\\.){3}(25[0-5]|2[0-4]\\d|1?\\d{1,2})$",
        REG_EXTENDED);
}

static void http_module_init()
{
    static pthread_once_t once = PTHREAD_ONCE_INIT;
    pthread_once(&once, http_module_init_routine);
}

int is_percent_encoded(const char *str)
{
    return str[0] == '%' && isxdigit(str[1]) && isxdigit(str[2]);
}

bool uri_equal(const struct URI *a, const struct URI *b)
{
    return a == b ||
           (!!a == !!b && strequal(a->scheme, b->scheme) &&
            strequal(a->host, b->host) && strequal(a->userinfo, b->userinfo) &&
            strequal(a->path, b->path) && strequal(a->query, b->query) &&
            strequal(a->fragment, b->fragment) && a->port == b->port);
}

int normalize_uri(struct URI *uri)
{
    return 0;
}

char *uri_repr(const struct URI *uri)
{
    if (!uri)
        return strdup("NULL");

    char *ret;
    asprintf(&ret,
             "(struct URI) {\n"
             "    scheme: %s\n"
             "    userinfo: %s\n"
             "    host: %s\n"
             "    port: %u\n"
             "    path: %s\n"
             "    query: %s\n"
             "    fragment: %s\n"
             "}",
             uri->scheme, uri->userinfo, uri->host, uri->port, uri->path,
             uri->query, uri->fragment);
    return ret;
}

void destroy_uri(struct URI *uri)
{
    free(uri->scheme);
    free(uri->host);
    free(uri->path);
    free(uri->query);
    free(uri->fragment);
    free(uri->userinfo);
    memset(uri, 0, sizeof(*uri));
}

int parse_uri(struct URI *restrict uri, const char *str,
              const struct URI *restrict base)
{
    http_module_init();

    int err;
    regmatch_t matches[10];
    regmatch_t *match_scheme = &matches[2];
    regmatch_t *match_authority = &matches[4];
    regmatch_t *match_path = &matches[5];
    regmatch_t *match_query = &matches[7];
    regmatch_t *match_fragment = &matches[9];

    memset(uri, 0, sizeof(*uri));

    if ((err = regexec(&re_uri, str, ARRAY_SIZE(matches), matches, 0)))
        goto error;

    if (regmatch_len(match_scheme)) {
        size_t start = match_scheme->rm_so;
        size_t len = match_scheme->rm_eo - start;
        char *scheme = stralloc(len);
        uri->scheme = scheme;

        if (!scheme)
            goto error_memory;

        if (!isalpha(str[start]))
            goto error_invalid;

        for (int i = 0; i < len; i++) {
            char c = str[start + i];
            if (isalnum(c) || strchr("+-.", c))
                scheme[i] = tolower(c);
            else
                goto error_invalid;
        }
    }

    if (regmatch_len(match_authority)) {
        size_t start = match_authority->rm_so;
        size_t len = 0;

        while (isalnum(str[start + len]) ||
               strchr("-._~:!$&'()*+,;=", str[start + len]) ||
               is_percent_encoded(str + start + len))
            len++;

        if (str[start + len] == '@') {
            if (len > 0) {
                if (!(uri->userinfo = memdup(str + start, len + 1)))
                    goto error_memory;

                uri->userinfo[len] = 0;
            }

            start += len + 1;
        }

        len = 0;
        if (str[start] == '[') {
            while (isxdigit(str[start + len]) ||
                   strchr(":.]", str[start + len]))
                len++;

            if (str[start + len - 1] != ']')
                goto error_invalid;

            uri->host = memdup(str + start, len + 1);
            if (!uri->host)
                goto error_memory;

            uri->host[len] = 0;
            uri->host_type = URI_HOST_IP6;
            start += len;
            len = 0;
        } else {
            /* Max hostname length */
            if ((len = strcspn(str + start, ":/?#")) > 253)
                goto error_invalid;

            char *host = memdup(str + start, len + 1);
            if (!host)
                goto error_memory;

            host[len] = 0;
            uri->host = host;

            if (!regexec(&re_ip4, host, 0, NULL, 0)) {
                uri->host_type = URI_HOST_IP4;
            } else {
                uri->host_type = URI_HOST_NAMED;

                if (!isalnum(host[0]))
                    goto error_invalid;

                for (int i = 0; i < len; i++) {
                    if (host[i] == '.' ?
                            !isalnum(host[i - 1]) || !isalnum(host[i + 1]) :
                            !isalnum(host[i]) && host[i] != '-')
                        goto error_invalid;

                    host[i] = tolower(host[i]);
                }
            }

            start += len;
            len = 0;
        }

        if (str[start] == ':') {
            start++;

            int n = 0;
            int max_len = len + 5;

            while (len < max_len && isdigit(str[start + len]))
                n = 10 * n + str[start + len++] - '0';

            if (n >= 1 << 16)
                goto error_invalid;

            uri->port = n;
            start += len;
        }

        if (start != match_authority->rm_eo)
            goto error_invalid;
    }

    if (regmatch_len(match_path)) {
        size_t start = match_path->rm_so;
        size_t len = match_path->rm_eo - start;
        char *path = memdup(str + start, len + 1);
        uri->path = path;

        if (!path)
            goto error_memory;

        path[len] = 0;
    }

    if (regmatch_len(match_query)) {
        size_t start = match_query->rm_so;
        size_t len = match_query->rm_eo - start;
        char *query = memdup(str + start, len + 1);
        uri->query = query;

        if (!query)
            goto error_memory;

        query[len] = 0;
    }

    if (regmatch_len(match_fragment)) {
        size_t start = match_fragment->rm_so;
        size_t len = match_fragment->rm_eo - start;
        char *fragment = memdup(str + start, len + 1);
        uri->fragment = fragment;

        if (!fragment)
            goto error_memory;

        fragment[len] = 0;
    }

    return 0;

error_invalid:
    err = EINVAL;
    goto error;
error_memory:
    err = ENOMEM;
    goto error;
error:
    destroy_uri(uri);
    return err ? err : -1;
}

static char *remove_dot_segments(const char *input)
{
    size_t len = strlen(input);
    char *in = memdup(input, len + 1);
    if (!in)
        return NULL;

    char *out = malloc(len + 1);
    if (!out) {
        free(in);
        return NULL;
    }

    char *wpos = out;
    char *rpos = in;

    while (*rpos) {
        if (!memcmp(rpos, "../", 3)) {
            rpos += 3;
        } else if (!memcmp(rpos, "./", 2) || !memcmp(rpos, "/./", 3)) {
            rpos += 2;
        } else if (!memcmp(rpos, "/.", 3)) {
            *(++rpos) = '/';
        } else if (!memcmp(rpos, "/../", 4)) {
            rpos += 3;
            wpos -= wpos > out;
            while (wpos > out && *wpos != '/')
                wpos--;
        } else if (!memcmp(rpos, "/..", 4)) {
            *(rpos += 2) = '/';
            wpos -= wpos > out;
            while (wpos > out && *wpos != '/')
                wpos--;
        } else if (!memcmp(rpos, ".", 2) || !memcmp(rpos, "..", 3)) {
            break;
        } else {
            size_t seglen = strcspn(rpos + 1, "./") + 1;
            memcpy(wpos, rpos, seglen);
            rpos += seglen;
            wpos += seglen;
        }
    }

    *wpos = 0;
    free(in);
    return out;
}
