#!/usr/bin/env python3
from typing import Iterable
from pathlib import Path
import subprocess
import shutil
import re


def transform1(lines: Iterable[str]):
    marks = [
        "#ifdef __cplusplus",
        "#if defined(__cplusplus)",
        "#if defined(__cplusplus) && (__cplusplus >= 201703)",
        "#elif (defined(__cplusplus) && (__cplusplus >= 202002L)) || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)",
        "#elif defined(__cplusplus)",
        "#if defined(MI_MALLOC_OVERRIDE)",
        "#if defined(MI_MALLOC_OVERRIDE) && defined(_WIN32) && !(defined(MI_SHARED_LIB) && defined(_DLL))",
        "#if defined(MI_MALLOC_OVERRIDE) && !(defined(_WIN32))",
        "#ifdef _MSC_VER",
        "#if defined(_MSC_VER)",
        "#elif defined(_MSC_VER)",
        "#elif (_MSC_VER >= 1700)",
        "#if defined(_MSC_VER) || defined(__MINGW32__)",
        "#if defined(__sun)",
        "#if MI_MALLOC_VERSION >= 182",
        "#if MI_OSX_ZONE",
        "#elif MI_USE_BUILTIN_THREAD_POINTER",
    ]

    def is_discarded(line: str):
        return line.strip() in marks

    def is_header_guard(line: str):
        return re.match(r"#(ifndef|define) MIMALLOC[A-Z_]+H", line.strip())

    stack = []
    for lineno, line in enumerate(lines, start=1):
        if line.strip().startswith("#define"):
            if is_header_guard(line):
                yield f"// {line}"
                continue

        if line.strip().startswith("#if"):
            stack.append([lineno, line])
            if is_header_guard(line):
                yield f"// {line}"
            elif any(is_discarded(line) for _, line in stack):
                yield f"#if 0 // {line}"
            else:
                yield line
        elif line.strip().startswith("#elif"):
            stack[-1][1] = line
            if any(is_discarded(line) for _, line in stack):
                yield f"#elif 0 // {line}"
            else:
                yield line
        elif line.strip().startswith("#else"):
            stack[-1][1] = line
            yield line
        elif line.strip().startswith("#endif"):
            lineno, prev = stack[-1]
            stack.pop()
            if is_header_guard(prev):
                yield f"// {line} // {lineno}"
            else:
                yield f"{line} // {lineno}"
        else:
            if any(is_discarded(line) for _, line in stack):
                yield f"// {line}"
            else:
                yield line


def transform2(stage1: str, stage2: str):
    # https://stackoverflow.com/a/2394040
    subprocess.run(
        f"gcc -fpreprocessed -dD -E -P {stage1} 1>{stage2} 2>/dev/null",
        check=True,
        shell=True,
    )


def transform3(lines: Iterable[str]):
    for line in lines:
        if line.strip().startswith("#include <"):
            continue
        else:
            yield line


def transform_lines(from_path: str, to_path: str, f):
    with open(from_path) as src:
        with open(to_path, "w") as dst:
            for line in f(src.read().splitlines()):
                if line is None:
                    dst.write("\n")
                else:
                    dst.write(line + "\n")


def main():
    stage0 = "preprocess/mimalloc.stage0.c"
    stage1 = "preprocess/mimalloc.stage1.c"
    stage2 = "preprocess/mimalloc.stage2.c"
    stage3 = "preprocess/mimalloc.stage3.c"

    Path("preprocess").mkdir(exist_ok=True)
    shutil.copyfile("single/mimalloc.c", stage0)

    transform_lines(stage0, stage1, transform1)

    transform2(stage1, stage2)

    transform_lines(stage2, stage3, transform3)

    shutil.copyfile(stage2, "preprocess/mimalloc.c")


if __name__ == "__main__":
    main()
