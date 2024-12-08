#!/usr/bin/env python3
from typing import Iterable
import shutil


def transform1(lines: Iterable[str]):
    for line in lines:
        if line.startswith("#![feature"):
            line = line.replace("stdsimd,", "")
            yield line

            yield "use c2rust_bitfields::BitfieldStruct;"
            yield ""

            yield '#[cfg(not(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu", target_pointer_width = "64")))]'
            yield 'compile_error!("mimalloc_oxide only supports x86_64-unknown-linux-gnu now");'
            yield ""

            yield 'include!("./extra.rs");'
            yield ""

            continue

        if line.strip() == "::core::arch::x86_64::_mm_pause();":
            yield "_mm_pause();"
            continue

        yield line


def transform_lines(from_path: str, to_path: str, f):
    with open(from_path) as src:
        with open(to_path, "w") as dst:
            for line in f(src.read().splitlines()):
                dst.write(line + "\n")


def main():
    stage0 = "transpile/mimalloc.stage0.rs"
    stage1 = "transpile/mimalloc.stage1.rs"

    transform_lines(stage0, stage1, transform1)

    shutil.copyfile(stage1, "crates/mimalloc-oxide/src/lib.rs")


if __name__ == "__main__":
    main()
