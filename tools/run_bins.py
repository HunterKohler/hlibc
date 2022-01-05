#!/usr/bin/env python3

import getopt
import os
import subprocess
import sys

usage = """
Usage: run_bins [options] paths...
Options:
    -h, --help      Display this help message.
""".strip()


def main():
    try:
        (opts, paths) = getopt.gnu_getopt(sys.argv[1:], "h", ["help"])
    except getopt.GetoptError as err:
        sys.stderr.write(f"Error: {err.msg}\n")
        sys.exit(1)

    for opt, optarg in opts:
        if opt in ("-h", "--help"):
            print(usage)
            sys.exit(0)

    if not paths:
        fatal(f"Error: Expected 'paths' argument.\n")

    results: list[tuple[str, int]] = []
    input_paths: list[tuple[str, bool]] = [(path, False) for path in paths]
    seen_paths = set()

    for path, synthetic in input_paths:
        relpath = os.path.relpath(path)
        abspath = os.path.abspath(path)

        if os.path.isdir(path):
            input_paths.extend((os.path.join(dirpath, file), True)
                               for dirpath, _, files in os.walk(path)
                               for file in files)
            continue
        elif not abspath in seen_paths:
            seen_paths.add(abspath)

            try:
                returncode = subprocess.run(
                    abspath, capture_output=True, check=False).returncode
                results.append((relpath, returncode))
            except FileNotFoundError:
                if not synthetic:
                    fatal(f"Error: File or directory not found: '{path}'\n")
            except PermissionError:
                if not synthetic:
                    fatal(f"Error: Permission denied: '{path}'\n")

    if not results:
        fatal(f"Error: No executables found.\n")

    results.sort()
    max_width = max(len(path) for path, _ in results)

    def print_row(left, right):
        print(left.ljust(max_width), right, sep="   ")

    print_row("path", "status")
    print_row("----", "------")

    for path, returncode in results:
        print_row(path, "fail" if returncode else "pass")


def fatal(msg: str):
    sys.stderr.write(msg)
    sys.exit(1)


if __name__ == "__main__":
    main()
