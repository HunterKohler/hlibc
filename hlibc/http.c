#include <ctype.h>
#include <hlibc/def.h>
#include <hlibc/regex.h>
#include <hlibc/string.h>

struct URI {
    char *scheme;

    char *host;
    uint16_t port;
    char *path;
    char *query;
    char *fragment;

    /*
     * Generic interpretation of the 'userinfo' section of a URI is deprecated.
     * Will not parse 'userinfo' apart from verifying it when interpreting a
     * non-scheme-specific URI.
     *
     * Reference:
     * https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.1
     */
    char *userinfo;
};

bool is_reserved(char c)
{
    return is_gendelim(c) || is_subdelim(c);
}

bool is_unreserved(char c)
{
    return isalnum(c) || c == '-' || c == '.' || c == '_' || c == '~';
}

bool is_gendelim(char c)
{
    return c == ':' || c == '/' || c == '?' || c == '#' || c == '[' ||
           c == ']' || c == '@';
}

bool is_subdelim(char c)
{
    return c == '!' || c == '$' || c == '&' || c == '\'' || c == '(' ||
           c == ')' || c == '*' || c == '+' || c == ',' || c == ';' || c == '=';
}

/*
 * Reference:
 * https://datatracker.ietf.org/doc/html/rfc3986#appendix-B
 *
 * Groups:
 * scheme    = $2
 * authority = $4
 * path      = $5
 * query     = $7
 * fragment  = $9
 *
 * Must use flag `REG_EXTENDED` for posix.
 */
#define URI_COMPONENTS_PATTERN \
    "^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?"

/*
 * Returns 'clean' duplicate of input string. Removes trailing and leading
 * whitespace and control chars, as well as embedded tabs and newlines.
 */
char *parse_uri_clean_input(const char *input)
{
    const char *begin = input;
    const char *end = input + strlen(input);

    while (begin < end && *begin >= 0 && *begin <= 32)
        begin++;
    while (begin < end && *end >= 0 && *end <= 32)
        end--;

    size_t ret_len = 0;
    char *ret = malloc(end - begin + 2);

    if (!ret)
        return NULL;

    for (const char *ptr = begin; ptr <= end; ptr++) {
        if (*ptr != '\t' && *ptr != '\n' && *ptr != '\r')
            ret[ret_len++] = *ptr;
    }

    ret[ret_len] = 0;
    char *tmp = realloc(ret, ret_len + 1);

    if (tmp)
        return tmp;
    else {
        free(ret);
        return NULL;
    }
}

int parse_uri(struct URI *restrict uri, const char *input,
              const struct URI *restrict base)
{
    char *clean = parse_uri_clean_input(input);
    if (!clean)
        return NULL;

    static regex_t re_uri;
    static regcomp_once_t once = REGCOMP_ONCE_INIT;

    regmatch_t matches[10];

    regcomp_once(&once, &re_uri, URI_COMPONENTS_PATTERN, REG_EXTENDED);
    regexec(&re_uri, clean, ARRAY_SIZE(matches), matches, 0);

    regmatch_t *scheme = &matches[2];
    regmatch_t *authority = &matches[4];
    regmatch_t *path = &matches[5];
    regmatch_t *query = &matches[7];
    regmatch_t *fragment = &matches[9];

    if (scheme->rm_so >= 0) {
        size_t scheme_len = scheme->rm_eo - scheme->rm_so + 1;
        uri->scheme = stralloc(scheme_len);

        if (!uri->scheme || !isalnum(clean[scheme->rm_so]))
            goto error;

        for (int i = 0; i < scheme_len; i++) {
            char c = clean[scheme->rm_so + i];
            if (isalnum(c) || c == '-' || c == '+' || c == '.')
                uri->scheme[i] = tolower(c);
            else
                goto error;
        }
    }

    if (authority->rm_so >= 0) {
        size_t authority_len = authority->rm_eo - authority->rm_so + 1;
        size_t userinfo_len = strcspn(clean + authority->rm_so, "@");

        if (userinfo_len <= authority_len) {
            uri->userinfo = stralloc(userinfo_len);

            if (!uri->userinfo)
                goto error;

            for (int i = 0; i < userinfo_len; i++) {
                char *pos = clean + authority->rm_so + i;

                if (is_unreserved(*pos) || is_subdelim(*pos) || *pos == ':' ||
                    (*pos == '%' && isxdigit(pos[1]) && isxdigit(pos[2])))
                    uri->userinfo[i] = *pos;
                else
                    goto error;
            }
        } else {
            userinfo_len = 0;
        }
    }
}
