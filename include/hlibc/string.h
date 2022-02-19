/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIBC_STRING_H_
#define HLIBC_STRING_H_

#include <string.h>
#include <errno.h>
#include <hlibc/def.h>

struct string_view {
    const char *str;
    size_t size;
};

/*
 * Allocated length `n` zero-d buffer.
 */
void *zalloc(size_t n);

/*
 * Allocates `n + 1` bytes and null terminates buffer. Returns `NULL` on error.
 */
char *stralloc(size_t n);

/*
 * Copies `n` bytes to new buffer starting at `src`. Returns `NULL` on failure.
 */
void *memdup(const void *src, size_t n);

/*
 * Swaps `n` bytes between `a` and `b`.
 *
 * Notes:
 * Compiling with sse/sse2 or avx/avx2 and level 3 optimization will vectorize
 * the loop, and make it as fast (if not faster) as a partially unrolled loop,
 * and faster than copying to a local buffer (static storage or not).
 */
void memswap(void *restrict a, void *restrict b, size_t n);

/*
 * Returns hex value of char, or -1 if invalid character.
 */
int hex_val(char c);

/*
 * Hex-encoded size of `n` bytes of memory.
 */
size_t hex_encode_size(size_t n);

/*
 * Decoded size of `n` byte hex-encoded string.
 */
size_t hex_decode_size(size_t n);

/*
 * Encodes `n` bytes from source into a hex encoded string at `dest`.
 */
void hex_encode(const void *restrict src, size_t n, char *restrict dest);

/*
 * Encodes a single byte 'src' to 'dest'. Does not append null terminator.
 */
void hex_encode_byte(uint8_t src, char *dest);

/*
 * Writes at most `size` hex-decoded bytes from string `src` to `dest`.
 *
 * Errors:
 * `EINVAL` The input was not a valid hex encoded string.
 * `ENOMEM` The decoded representation of `src` is larger than `size`.
 */
int hex_decode(const char *restrict src, void *restrict dest, size_t size);

/*
 * Returns Base 64 value of `c`.
 */
int b64_val(char c);

/*
 * Returns the size of a length `n` buffer when encoded to Base 64.
 */
size_t b64_encode_size(size_t n);

/*
 * Returns the size of the decoded buffer that `n` bytes of the Base 64 encoded
 * string `src` represents.
 */
size_t b64_decode_size(const char *src, size_t n);

/*
 * Writes a the Base 64 representation of the first `n` bytes from `src`
 * to `dest`. The buffers may not overlap. `dest` will be null terminated.
 */
void b64_encode(const void *restrict src, size_t n, char *restrict dest);

/*
 * Writes at most `size` bytes of the base64 decoded representation of `src`
 * to `dest`.
 *
 * Allows both correctly padded, and unpadded inputs.
 *
 * Errors:
 * `EINVAL` The input was not a valid base64 encoded string.
 * `ENOMEM` The decoded representation of `src` is larger than `size`.
 */
int b64_decode(const char *restrict src, void *restrict dest, size_t size);

/*
 * Stringifies a `__int128`.
 *
 * Errors:
 *  `ENOMEM` The output buffer is too small to fit the representation of `val`.
 */
int int128_to_string(int128_t val, char *buf, size_t buf_size);

/*
 * Stringifies an `unsigned __int128`.
 *
 * Errors:
 *  `ENOMEM` The output buffer is too small to fit the representation of `val`.
 */
int uint128_to_string(uint128_t val, char *buf, size_t buf_size);

/*
 * Retrieve the name of an `errno` value specified by `code`. If the name is
 * not known, this function returns `NULL`. Attempts to conform to POSIX
 * specification.
 *
 * Reference:
 * https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html
 */
static inline const char *strerror_name(int code)
{
    static const char *const names[] = {
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

    if (code < 0 || code >= array_size(names))
        return NULL;

    return names[code];
}

#endif
