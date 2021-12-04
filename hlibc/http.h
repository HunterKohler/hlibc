#ifndef HLIBC_HTTP_H_
#define HLIBC_HTTP_H_

// int URL_init(struct URL *url, const char *src)
// {
//     static bool init = false;
//     static regex_t re;

//     if (!init) {
//         regcomp(
//             &re,
//             "^\\s*" /* Leading whitespace */
//             "([a-z][a-z0-9+.-]*)" /* Scheme (match 1) */
//             "://" /* Scheme delimiter */
//             "(([^:@]*)(:([^:@]*))?@)?" /* Authentication (username, match 3), (pass, match 5) */
//             "(([a-z]([a-z0-9-]{0,61}[a-z0-9])?\\.)+[a-z]{2,63})?" /* Hostname (match 6) */
//             "(:([0-9]*))?" /* Port (match 10) */
//             "(/[^?]*)?" /* Path (match 11) */
//             "(\\?[^#]*)?" /* Querystring (match 12) */
//             "(#.*)?" /* Fragment (match 13) */
//             "\\s*$", /* Trailing whitespace */
//             REG_ICASE | REG_EXTENDED);
//     }

//     regmatch_t matches[14];
//     if (regexec(&re, str, n, ARRAY_SIZE(matches), 0))
//         return -1;

//     /* clang-format off */
//     if (!(url->scheme = substr(str, matches[1].rm_so, matches[1].rm_eo))) goto error_scheme;
//     if (!(url->username = substr(str, matches[3].rm_so, matches[3].rm_eo))) goto error_username;
//     if (!(url->password = substr(str, matches[5].rm_so, matches[5].rm_eo))) goto error_password;
//     if (!(url->hostname = substr(str, matches[6].rm_so, matches[6].rm_eo))) goto error_hostname;
//     if (!(url->port = substr(str, matches[10].rm_so, matches[10].rm_eo))) goto error_port;
//     if (!(url->path = substr(src, matches[11].rm_so, matches[11].rm_eo))) goto error_path;
//     if (!(url->query = substr(src, matches[12].rm_so, matches[12].rm_eo))) goto error_query;
//     if (!(url->fragment = substr(src, matches[13].rm_so, matches[13].rm_eo))) goto error_fragment;

// error_fragment: free(url->query);
// error_query: free(url->path);
// error_path: free(url->port);
// error_port: free(url->hostname);
// error_hostname: free(url->password);
// error_password: free(url->username);
// error_username: free(url->scheme);
// error_scheme: return -1;
//     /* clang-format on */
// }

// int from_hex_digit(int c)
// {
//     return c >= '0' && c <= '9' ? c - '0' :
//            c >= 'a' && c <= 'f' ? c - 'a' :
//            c >= 'A' && c <= 'F' ? c - 'A' :
//                                   -1;
// }

// int percent_decode(char *dest, const char *src)
// {
//     int i = 0;
//     int j = 0;

//     while (src[i]) {
//         if (src[i] == '%') {
//             int a = from_hex_digit(src[++i]);
//             if (a < 0)
//                 return i;

//             int b = from_hex_digit(src[++i]);
//             if (b < 0)
//                 return i;

//             dest[j++] = 16 * a + b;
//             i += 1;
//         } else {
//             dest[j++] = src[i++];
//         }
//     }

//     dest[j] = '\0';
//     return -1;
// }

#endif
