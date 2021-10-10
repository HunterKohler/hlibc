#!/usr/bin/env python3

import subprocess
import re
import pathlib

re_yes = re.compile(r"^\s*y(es)?\s*$", re.IGNORECASE)
re_no = re.compile(r"^\s*no?\s*$", re.IGNORECASE)


def confirm_prompt(msg):
    while True:
        inp = input(f"\r{msg} [y/n] ")
        if re_yes.match(inp):
            return True
        elif re_no.match(inp):
            return False


def create_pp_h(limit: int):
    LF = "\n"

    def ulist(n: int):
        return [f"_{i}" for i in range(n + 1)]

    def arglist(n: int):
        return ", ".join(ulist(n))

    return f"""\
    /*
     * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
     *
     * WARNING: AUTOGENERATED FILE, DO NOT EDIT DIRECTLY.
     */

    #ifndef HLIBC_GENERATED_PP_H_
    #define HLIBC_GENERATED_PP_H_

    #define PP_MAX {limit - 1}

    #define GET_ARG_0(_0, ...) _0
    {LF.join(f"#define GET_ARG_{n}(_0, ...) GET_ARG_{n - 1}(__VA_ARGS__)"
        for n in range(1, limit))}

    {LF.join(f"#define _PASTE_{n}({arglist(n - 1)}) {'##'.join(ulist(n - 1))}"
        for n in range(limit))}

    {LF.join(f"#define PASTE_{n}(...) _PASTE_{n}(__VA_ARGS__)"
        for n in range(limit))}

    #define SEQ_0() 0
    {LF.join(f"#define SEQ_{n}() SEQ_{n - 1}() {n}"
        for n in range(1, limit))}

    #define REV_SEQ_0() 0
    {LF.join(f"#define REV_SEQ_{n}() {n} REV_SEQ_{n - 1}()"
        for n in range(1, limit))}

    #define EAT_0(...) __VA_ARGS__
    {LF.join(f"#define EAT_{n}(_0, ...) EAT_{n - 1}(__VA_ARGS__)"
        for n in range(1, limit))}

    #define REPEAT_0(...)
    {LF.join(f"#define REPEAT_{n}(...) __VA_ARGS__ REPEAT_{n - 1}(__VA_ARGS__)"
        for n in range(1, limit))}

    #define REPEAT_LIST_0(...)
    #define REPEAT_LIST_1(...) __VA_ARGS__
    {LF.join(f"#define REPEAT_LIST_{n}(...) __VA_ARGS__, REPEAT_LIST_{n - 1}(__VA_ARGS__)"
        for n in range(2, limit))}

    #endif
    """


def main():
    with open(pathlib.Path(__file__).parent / "pp.h", "w") as file:
        file.write(clang_format(create_pp_h(128)))

def clang_format(src: str) -> str:
    return subprocess.run("clang-format", capture_output=True, check=True,
                          input=src.encode()).stdout.decode()


if __name__ == "__main__":
    main()
