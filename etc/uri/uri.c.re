#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <regex.h>
#include <time.h>
#include <assert.h>
#include "uri.h"

int uri_parse_segments_re(const char *src, struct uri_segments *dest)
{
    const char *cursor = src;
    const char *marker;
    const char *q0, *q1;
    const char *f0, *f1;
    const char *u0, *u1;
    const char *s0, *s1;
    const char *prt0, *prt1;
    const char *h0, *h1;
    const char *rname, *ip4, *ip6, *ipf;
    const char *p0, *p1, *p2, *p3;
    bool relative = false;

    memset(dest, 0, sizeof(*dest));

    /*!stags:re2c:formal format = "const char *@@;"; */
    /*!local:re2c:formal
        re2c:yyfill:enable = 0;
        re2c:define:YYCTYPE = char;
        re2c:define:YYCURSOR = cursor;
        re2c:define:YYMARKER = marker;
        re2c:tags = 1;

        alpha = [a-zA-Z];
        digit = [0-9];
        hexdigit = [a-fA-F0-9];

        sub_delims = [!$&'()*+,;=];
        unreserved = alpha | digit | [-._~];

        pct_encoded = "%" hexdigit{2};
        pchar = unreserved | pct_encoded | sub_delims | [:@];

        query = @q0 (pchar | [/?])* @q1;
        fragment = @f0 (pchar | [/?])* @f1;

        segment = pchar*;
        segment_nz = pchar+;
        segment_nz_nc = (unreserved | pct_encoded  sub_delims | "@")*;

        path_abempty = ("/" segment)*;
        path_absolute = "/" (segment_nz ("/" segment)*)?;
        path_noscheme = segment_nz_nc ("/" segment)*;
        path_rootless = segment_nz ("/" segment)*;
        path_empty = "";

        reg_name = (unreserved | pct_encoded | sub_delims)*;

        dec_octet = digit | [1-9] digit | "1" digit {2} | "2" [0-4] digit |
            "25" [0-5];

        IPv4address = (dec_octet "."){3} dec_octet;

        h16 = hexdigit{1,4};
        ls32 = h16 ":" h16 | IPv4address;

        IPv6address = (h16 ":"){6} ls32 | "::" (h16 ":"){5} ls32 |
            h16? "::" (h16 ":"){4} ls32 |
            ((h16 ":"){0,1} h16)? "::" (h16 ":"){3} ls32 |
            ((h16 ":"){0,2} h16)? "::" (h16 ":"){2} ls32 |
            ((h16 ":"){0,3} h16)? "::" h16 ":" ls32 |
            ((h16 ":"){0,4} h16)? "::" ls32 |
            ((h16 ":"){0,5} h16)? "::" h16 |
            ((h16 ":"){0,6} h16)? "::";

        IPvFuture = "v" hexdigit+ "." (unreserved | sub_delims | ":")+;
        IP_literal = "[" (@ip6 IPv6address | @ipf IPvFuture) "]";

        port = @prt0 digit* @prt1;
        host = @h0 (IP_literal | @ip4 IPv4address | @rname reg_name) @h1;
        userinfo = @u0 (unreserved | pct_encoded | sub_delims | ":")* @u1;
        authority = (userinfo "@")? host (":" port)?;

        scheme = @s0 alpha (alpha | digit | [+.-])* @s1;

        relative_part = "//" authority @p0 path_abempty @p1 | @p2 (path_absolute |
            path_noscheme | path_empty) @p3;

        hier_part = "//" authority @p0 path_abempty @p1 | @p2 (path_absolute |
            path_rootless | path_empty) @p3;

        relative_ref = relative_part ("?" query)? ("#" fragment)?;

        uri = scheme ":" hier_part ("?" query)? ("#" fragment)?;

        relative_ref "\x00" {
            s0 = NULL;
            s1 = NULL;
            relative = true;
            goto parsed;
        }

        uri "\x00" { goto parsed; }

        * { goto error; }
    */
parsed:
    if (s0) {
        dest->scheme.str = s0;
        dest->scheme.size = s1 - s0;
    }

    if (u0) {
        dest->userinfo.str = u0;
        dest->userinfo.size = u1 - u0;
    }

    if (h0) {
        dest->host.str = h0;
        dest->host.size = h1 - h0;

        if (rname) {
            dest->host_type = URI_HOST_NAME;
        } else if (ip4) {
            dest->host_type = URI_HOST_IPV4;
        } else if (ip6) {
            dest->host_type = URI_HOST_IPV6;
        } else if (ipf) {
            dest->host_type = URI_HOST_IPVF;
        }
    }

    if (prt0) {
        dest->port.str = prt0;
        dest->port.size = prt1 - prt0;
    }

    if (p0) {
        dest->path.str = p0;
        dest->path.size = p1 - p0;
    } else if (p2) {
        dest->path.str = p2;
        dest->path.size = p3 - p2;
    }

    if (q0) {
        dest->query.str = q0;
        dest->query.size = q1 - q0;
    }

    return 0;
error:
    return EINVAL;
}
