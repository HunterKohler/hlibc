/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIBC_STRING_H_
#define HLIBC_STRING_H_

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <hlibc/def.h>

/*
 * Allocates `n + 1` bytes and null terminates buffer. Returns `NULL` on error.
 */
char *stralloc(size_t n);

/*
 * Allocates enough memory to store concatenated strings. Returns `NULL` on
 * error.
 */
char *astrcat(const char *src1, const char *src2);

/*
 * Allocates and returns memory for the concat of `n` strings in an array
 * pointed to by `src`. Will ignore null strings.
 */
char *strcat_n(const char **src, size_t n);

/*
 * Allocate and copy substring in `src` on `[begin, end)`, or an
 * empty-null-terminated string when `end < begin`.
 */
char *substr(const char *src, size_t begin, size_t end);

/*
 * Library `strcmp()` with null checking.
 */
int strcmp_safe(const char *a, const char *b);

/*
 * Compares strings for case-insensitive equality including `NULL` checks.
 * `strcmp()` may cause a segmentation fault if passed `NULL`.
 */
int strcasecmp_safe(const char *a, const char *b);

/*
 * Library `memcmp()` with null checking.
 */
int memcmp_safe(const void *a, const void *b, size_t n);

/*
 * Copies `n` bytes to new buffer starting at `src`. Returns `NULL` on failure.
 */
void *memdup(const void *src, size_t n);

/*
 * Returns hex value of char, or -1 if invalid character.
 */
int hex_val(char c);

/*
 * Removes leading and trailing whitespace from string in-place. Returns
 * new length.
 */
size_t strtrim(char *c);

/*
 * Hex-encoded size of `n` bytes of memory.
 */
size_t hex_encode_size(size_t n);

/*
 * Decoded size of `n` byte hex-encoded string.
 */
size_t hex_decode_size(size_t n);

/*
 * Writes hex representation as a null terminated string  of `n` bytes of
 * memory from `ptr` into `dest`. `dest` should point to a buffer of at least
 * `2 * n + 1` bytes.
 */
void hex_encode(const void *restrict src, size_t n, char *restrict dest);

/*
 * Writes the values of a hex encoded string `str` to memory at `dest`. `dest`
 * should point to a buffer at least `ceil(strlen(str) / 2.0) + 1` bytes long.
 */
int hex_decode(const char *restrict src, void *restrict dest);

int b64_val(char c);
size_t b64_encode_size(size_t n);
size_t b64_decode_size(const char *src, size_t n);
void b64_encode(const void *restrict src, size_t n, char *restrict dest);
int b64_decode(const char *restrict src, void *restrict dest);

char *__to_string_ll(long long src, char *dest, unsigned int base);
char *__to_string_ull(unsigned long long src, char *dest, unsigned int base);
char *__to_string_c(char src, char *dest);
char *__to_string_b(bool src, char *dest);
char *__to_string_ld(long double src, char *dest);
char *__to_string_cld(complex long double src, char *dest);

/*
 * Generic stringification for primitives. 128-bit-wide integer types not
 * supported.
 */
// clang-format off
#define to_string(src, dest, ...)             \
    _Generic((src),                           \
        signed char: __to_string_ll,          \
        short: __to_string_ll,                \
        int: __to_string_ll,                  \
        long: __to_string_ll,                 \
        long long: __to_string_ll,            \
        unsigned char: __to_string_ull,       \
        unsigned short: __to_string_ull,      \
        unsigned int: __to_string_ull,        \
        unsigned long: __to_string_ull,       \
        unsigned long long: __to_string_ull,  \
        float: __to_string_ld,                \
        double: __to_string_ld,               \
        long double: __to_string_ld,          \
        complex float: __to_string_cld,       \
        complex double: __to_string_cld,      \
        complex long double: __to_string_cld, \
        char: __to_string_c,                  \
        bool: __to_string_b                   \
    )(src, dest, ## __VA_ARGS__)
// clang-format on

static const char *const errno_name_table[] = {
#ifdef E2BIG
    [E2BIG] = "E2BIG",
#endif
#ifdef EACCES
    [EACCES] = "EACCES",
#endif
#ifdef EADDRINUSE
    [EADDRINUSE] = "EADDRINUSE",
#endif
#ifdef EADDRNOTAVAIL
    [EADDRNOTAVAIL] = "EADDRNOTAVAIL",
#endif
#ifdef EAFNOSUPPORT
    [EAFNOSUPPORT] = "EAFNOSUPPORT",
#endif
#ifdef EAGAIN
    [EAGAIN] = "EAGAIN",
#endif
#ifdef EALREADY
    [EALREADY] = "EALREADY",
#endif
#ifdef EBADF
    [EBADF] = "EBADF",
#endif
#ifdef EBADMSG
    [EBADMSG] = "EBADMSG",
#endif
#ifdef EBUSY
    [EBUSY] = "EBUSY",
#endif
#ifdef ECANCELED
    [ECANCELED] = "ECANCELED",
#endif
#ifdef ECHILD
    [ECHILD] = "ECHILD",
#endif
#ifdef ECONNABORTED
    [ECONNABORTED] = "ECONNABORTED",
#endif
#ifdef ECONNREFUSED
    [ECONNREFUSED] = "ECONNREFUSED",
#endif
#ifdef ECONNRESET
    [ECONNRESET] = "ECONNRESET",
#endif
#ifdef EDEADLK
    [EDEADLK] = "EDEADLK",
#endif
#ifdef EDESTADDRREQ
    [EDESTADDRREQ] = "EDESTADDRREQ",
#endif
#ifdef EDOM
    [EDOM] = "EDOM",
#endif
#ifdef EDQUOT
    [EDQUOT] = "EDQUOT",
#endif
#ifdef EEXIST
    [EEXIST] = "EEXIST",
#endif
#ifdef EFAULT
    [EFAULT] = "EFAULT",
#endif
#ifdef EFBIG
    [EFBIG] = "EFBIG",
#endif
#ifdef EHOSTUNREACH
    [EHOSTUNREACH] = "EHOSTUNREACH",
#endif
#ifdef EIDRM
    [EIDRM] = "EIDRM",
#endif
#ifdef EILSEQ
    [EILSEQ] = "EILSEQ",
#endif
#ifdef EINPROGRESS
    [EINPROGRESS] = "EINPROGRESS",
#endif
#ifdef EINTR
    [EINTR] = "EINTR",
#endif
#ifdef EINVAL
    [EINVAL] = "EINVAL",
#endif
#ifdef EIO
    [EIO] = "EIO",
#endif
#ifdef EISCONN
    [EISCONN] = "EISCONN",
#endif
#ifdef EISDIR
    [EISDIR] = "EISDIR",
#endif
#ifdef ELOOP
    [ELOOP] = "ELOOP",
#endif
#ifdef EMFILE
    [EMFILE] = "EMFILE",
#endif
#ifdef EMLINK
    [EMLINK] = "EMLINK",
#endif
#ifdef EMSGSIZE
    [EMSGSIZE] = "EMSGSIZE",
#endif
#ifdef EMULTIHOP
    [EMULTIHOP] = "EMULTIHOP",
#endif
#ifdef ENAMETOOLONG
    [ENAMETOOLONG] = "ENAMETOOLONG",
#endif
#ifdef ENETDOWN
    [ENETDOWN] = "ENETDOWN",
#endif
#ifdef ENETRESET
    [ENETRESET] = "ENETRESET",
#endif
#ifdef ENETUNREACH
    [ENETUNREACH] = "ENETUNREACH",
#endif
#ifdef ENFILE
    [ENFILE] = "ENFILE",
#endif
#ifdef ENOBUFS
    [ENOBUFS] = "ENOBUFS",
#endif
#ifdef ENODATA
    [ENODATA] = "ENODATA",
#endif
#ifdef ENODEV
    [ENODEV] = "ENODEV",
#endif
#ifdef ENOENT
    [ENOENT] = "ENOENT",
#endif
#ifdef ENOEXEC
    [ENOEXEC] = "ENOEXEC",
#endif
#ifdef ENOLCK
    [ENOLCK] = "ENOLCK",
#endif
#ifdef ENOLINK
    [ENOLINK] = "ENOLINK",
#endif
#ifdef ENOMEM
    [ENOMEM] = "ENOMEM",
#endif
#ifdef ENOMSG
    [ENOMSG] = "ENOMSG",
#endif
#ifdef ENOPROTOOPT
    [ENOPROTOOPT] = "ENOPROTOOPT",
#endif
#ifdef ENOSPC
    [ENOSPC] = "ENOSPC",
#endif
#ifdef ENOSR
    [ENOSR] = "ENOSR",
#endif
#ifdef ENOSTR
    [ENOSTR] = "ENOSTR",
#endif
#ifdef ENOSYS
    [ENOSYS] = "ENOSYS",
#endif
#ifdef ENOTCONN
    [ENOTCONN] = "ENOTCONN",
#endif
#ifdef ENOTDIR
    [ENOTDIR] = "ENOTDIR",
#endif
#ifdef ENOTEMPTY
    [ENOTEMPTY] = "ENOTEMPTY",
#endif
#ifdef ENOTRECOVERABLE
    [ENOTRECOVERABLE] = "ENOTRECOVERABLE",
#endif
#ifdef ENOTSOCK
    [ENOTSOCK] = "ENOTSOCK",
#endif
#ifdef ENOTSUP
    [ENOTSUP] = "ENOTSUP",
#endif
#ifdef ENOTTY
    [ENOTTY] = "ENOTTY",
#endif
#ifdef ENXIO
    [ENXIO] = "ENXIO",
#endif
#ifdef EOPNOTSUPP
    [EOPNOTSUPP] = "EOPNOTSUPP",
#endif
#ifdef EOVERFLOW
    [EOVERFLOW] = "EOVERFLOW",
#endif
#ifdef EOWNERDEAD
    [EOWNERDEAD] = "EOWNERDEAD",
#endif
#ifdef EPERM
    [EPERM] = "EPERM",
#endif
#ifdef EPIPE
    [EPIPE] = "EPIPE",
#endif
#ifdef EPROTO
    [EPROTO] = "EPROTO",
#endif
#ifdef EPROTONOSUPPORT
    [EPROTONOSUPPORT] = "EPROTONOSUPPORT",
#endif
#ifdef EPROTOTYPE
    [EPROTOTYPE] = "EPROTOTYPE",
#endif
#ifdef ERANGE
    [ERANGE] = "ERANGE",
#endif
#ifdef EROFS
    [EROFS] = "EROFS",
#endif
#ifdef ESPIPE
    [ESPIPE] = "ESPIPE",
#endif
#ifdef ESRCH
    [ESRCH] = "ESRCH",
#endif
#ifdef ESTALE
    [ESTALE] = "ESTALE",
#endif
#ifdef ETIME
    [ETIME] = "ETIME",
#endif
#ifdef ETIMEDOUT
    [ETIMEDOUT] = "ETIMEDOUT",
#endif
#ifdef ETXTBSY
    [ETXTBSY] = "ETXTBSY",
#endif
#ifdef EWOULDBLOCK
    [EWOULDBLOCK] = "EWOULDBLOCK",
#endif
#ifdef EXDEV
    [EXDEV] = "EXDEV",
#endif
};

/*
 * Retrieve the name of an 'errno-value'. On unknown, `NULL` is returned.
 */
static inline const char *strerror_name(int err)
{
    return err < 0 || err >= ARRAY_SIZE(errno_name_table) ?
               NULL :
               errno_name_table[err];
}

#endif
