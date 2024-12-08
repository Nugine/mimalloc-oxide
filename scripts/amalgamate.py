#!/usr/bin/env python3
from pathlib import Path
import shutil


def find_include(from_: Path, file):
    candidates = [
        Path("mimalloc/src") / file,
        Path("mimalloc/include") / file,
        from_.parent / file,
    ]
    for candidate in candidates:
        if candidate.exists():
            return candidate
    raise FileNotFoundError(f"Could not find {file} from {from_}")


def expand_source(path, vis):
    if path in vis:
        return
    vis.add(path)

    with open(path) as f:
        content = f.read()

    for line in content.splitlines():
        if line == "#pragma once":
            continue

        if not line.startswith("#include"):
            yield line
            continue

        file = line.split()[1]

        if file.endswith('.c"'):
            next_path = find_include(path, file[1:-1])
        elif file.endswith('.h"'):
            next_path = find_include(path, file[1:-1])
        else:
            next_path = None

        if next_path is not None:
            yield ""
            yield f"// --- Amalgamation START --- {next_path}"
            yield from expand_source(next_path, vis)
            yield f"// --- Amalgamation END --- {next_path}"
            yield ""
        else:
            yield line


def main():
    shutil.copyfile("mimalloc/include/mimalloc.h", "single/mimalloc.h")

    with open("single/mimalloc.c", "w") as f:
        entry = Path("mimalloc/src/static.c")
        vis = set()
        gen = expand_source(entry, vis)
        for line in gen:
            f.write(line + "\n")


if __name__ == "__main__":
    main()
