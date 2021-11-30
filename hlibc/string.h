/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIBC_STRING_H_
#define HLIBC_STRING_H_

#include <string.h>
#include <errno.h>
#include <hlibc/def.h>

/*
 * Allocates `n + 1` bytes and null terminates buffer. Returns `NULL` and sets
 * `errno` on failure if not already set.
 */
char *stralloc(size_t n);

/*
 * Allocates enough memory to store concatenated strings. Returns `NULL` and
 * sets `errno` on failure if not already set.
 */
char *astrcat(const char *src1, const char *src2);

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

static inline const char *error_name(int err)
{
    return err < 0 || err >= ARRAY_SIZE(errno_name_table) ?
               NULL :
               errno_name_table[err];
}

#endif
