#!/usr/bin/env python3
from typing import Iterable
from pathlib import Path
import shutil
import json
import re

REPLACEMENTS = [
    (
        "&_mi_page_empty as *const mi_page_t as *mut mi_page_t",  #
        "&raw mut _mi_page_empty",
    ),
    (
        "&_mi_heap_empty as *const mi_heap_t as *mut mi_heap_t",  #
        "&raw mut _mi_heap_empty",
    ),
]


def dict_get(d, *keys):
    for key in keys:
        if d is None:
            return None
        if isinstance(d, dict):
            d = d.get(key)
        elif isinstance(d, list):
            d = d[key]
        else:
            return None
    return d


def transform1(lines: Iterable[str]):
    if Path("temp/oxide.jsonl").exists():
        with open("temp/oxide.jsonl") as f:
            for msg in f.read().splitlines():
                msg = json.loads(msg)
                if dict_get(msg, "message", "code", "code") == "static_mut_refs":
                    text = dict_get(msg, "message", "spans", 0, "text", 0, "text")
                    if text is None:
                        continue
                    assert isinstance(text, str)
                    patterns = [
                        r"^ *(.+?): &mut (.+?)$",
                        r"^ *&mut ([^:.]+?)$",
                    ]
                    if any(re.match(pat, text) is not None for pat in patterns):
                        correct = text.replace("&mut ", "&raw mut ")
                        REPLACEMENTS.append((text.strip(), correct))
                        print(f"REPLACEMENT: {text.strip()} -> {correct}")

    for line in lines:
        if line.startswith("#![feature"):  # injection point
            line = line.replace("stdsimd,", "")
            line = line.replace("asm,", "")
            yield line

            yield "#![allow(internal_features)]"
            yield

            yield "use c2rust_bitfields::BitfieldStruct;"
            yield

            yield '#[cfg(not(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu", target_pointer_width = "64")))]'
            yield 'compile_error!("mimalloc_oxide only supports x86_64-unknown-linux-gnu now");'
            yield

            yield 'include!("./extra.rs");'
            yield

            continue

        if line.strip() == "::core::arch::x86_64::_mm_pause();":
            yield "_mm_pause();"
            continue

        if line.strip().startswith('extern "C"'):
            yield line.replace('extern "C"', 'unsafe extern "C"')
            continue

        if line.strip() == "#[no_mangle]":
            yield "#[unsafe(no_mangle)]"
            continue

        if "link_section" in line:
            line = re.sub(r"(link_section = \".+\")", r"unsafe(\1)", line)
            yield line
            continue

        for old, new in REPLACEMENTS:
            if old in line:
                line = line.replace(old, new)

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
    stage0 = "transpile/mimalloc.stage0.rs"
    stage1 = "transpile/mimalloc.stage1.rs"

    transform_lines(stage0, stage1, transform1)

    shutil.copyfile(stage1, "crates/mimalloc-oxide/src/lib.rs")


if __name__ == "__main__":
    main()
