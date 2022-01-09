#!/usr/bin/env python3

import pathlib
import sys

MESSAGE = "Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>"


def rglob(*patterns):
    for pattern in patterns:
        yield from pathlib.Path().rglob(pattern)


def main():
    exit_code = False

    for path in rglob("*.c", "*.h", "*.cpp", "*.hpp"):
        with open(path) as file:
            if MESSAGE not in "\n".join(file.readline() for i in range(5)):
                print(f"missing header: {path}")
                exit_code = 1

    for path in rglob("*.md"):
        with open(path) as file:
            if MESSAGE not in file.read():
                print(f"missing message: {path}")
                exit_code = 1

    sys.exit(exit_code)


if __name__ == "__main__":
    main()
