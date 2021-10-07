#!/usr/bin/env python3

import sys
import functools
import dataclasses
import glob
import pprint
import re

__COPYRIGHT__ = "Copyright (C) 2021 John Hunter Kohler <jhunterkohler@gmail.com>"
__VERSION__ = "1.0.0"
__USAGE__ = f"""
Usage:
    {sys.argv[0]} [options] [files...]

Lint files for copyright header and optionally insert in-place.

Options:
    -                    Read from stdin. Assumed if no files are specified
    --                   Stop parsing options. Assume all further input
                         arguments are files
    -b, --bail=n         Bail after `n` missing headers
    -c, --count          Count number of files missing headers
    -e, --exit=code      Use exit-code `code` when some headers are missing.
                         Default: 1
    -f, --file=file      Read list of files to check from `path`
    -h, --help           Display this help message
    -i, --insert         Insert headers in place
    -l, --list           Only list files. Will not emit any other output
    -q, --quite          Suppress normal output
    -r, --regex=pattern  Specify files by regex `pattern`. Can be used in
                         addition to normal input. Multiple -r flags may be
                         used to specify mutliple file patterns
    --only-count         Count number of files missing headers and supress other
                         non-error output
    --no-recurse         Disabled auto recursion into input directories
    --no-glob            Disables parsing arguments as globs
    --version            Output versioning information

Copyright:
    {__COPYRIGHT__}
"""[1:-1]


def head(file_path, n=10):
    with open(file_path, "r") as fp:
        return "".join(next(fp) for i in range(n))


def has_copyright(file_path):
    return __COPYRIGHT__ in head(file_path)


def static(**kwargs):
    def decorator(fn):
        @functools.wraps(fn)
        def wrapper(*a, **kw):
            return fn(*a, **kw)

        for key in kwargs:
            setattr(wrapper, key, kwargs[key])

        return wrapper
    return decorator


def inv(fn):
    @functools.wraps(fn)
    def inner(*args, **kwargs):
        return not fn(*args, **kwargs)
    return inner


    # -b, --bail=n        Bail after `n` missing headers
    # -c, --count         Count number of files missing headers
    # -e, --exit=code     Use exit-code `code` when some headers are missing.
    #                     Default: 1
    # -f, --file=path     Read list of files to check from `path`
    # -h, --help          Display this help message
    # -i, --insert        Insert headers in place
    # -l, --list          Only list files. Will not emit any other output
    # -q, --quite         Suppress normal output
    # -r, --regex         Specify files by regex
    # --only-count        Count number of files missing headers and supress other
    #                     non-error output
    # --no-recurse        Disabled auto recursion into input directories
    # --no-glob           Disables parsing arguments as globs
    # --version           Output versioning information

@dataclasses.dataclass
class Options:
    bail = float("inf")
    count = False
    exit_code = 1
    file = None
    insert = False
    list = False
    quiet = False
    regex = []
    only_count = False
    no_recurse = False
    no_glob = False


def main(argv):
    if len(argv) == 1:
        print("Error: No input files", file=sys.stderr)
        sys.exit(1)

    files = []
    opts = Options()

    # for arg in (it := iter(argv)):
    #     if arg == "-":
    #         raise NotImplementedError
    #     elif arg == "--":
    #         files.extend(it)
    #     elif arg[0] == "-":
    #         flag, value = re.search("(?s)^-{1,2}([^=]*?)(?:=(.+))?$", arg)
    #         if flag == "b" or flag == "bail":
    #             opts.bail = True
    #         elif flag == "-c" or flag == "count":
    #             opts.count = True
    #         elif flag == "-e" or flag == "exit":
    #             if value != None:
    #                 try:
    #                     value = next(it)
    #                 except StopIteration:
    #                     raise NotImplementedError
    #             try:
    #                 opts.exit = int(value)
    #             except ValueError:
    #                 raise NotImplementedError
    #         elif flag == ""




        # else:
        #     files.append(arg)

    missing = list(filter(inv(has_copyright), argv[1:]))

    if len(missing):
        print(f"Missing copyright header:")
        for file_path in missing:
            print(file_path)
        sys.exit(1)
    else:
        print("All files contain copyright header")


if __name__ == "__main__":
    main(sys.argv)
