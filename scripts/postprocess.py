#!/usr/bin/env python3
from typing import Iterable
from pathlib import Path
import subprocess
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
    (
        "&_mi_heap_empty as *const mi_heap_t as *const libc::c_void",  #
        "(&raw const _mi_heap_empty) as *const mi_heap_t as *const libc::c_void",
    ),
    (
        "&_mi_heap_empty.pages as *const [mi_page_queue_t; 75] as *const libc::c_void",  #
        "(&raw const _mi_heap_empty.pages) as *const [mi_page_queue_t; 75] as *const libc::c_void",
    ),
    (
        "&mut *options.as_mut_ptr().offset(option as isize) as *mut mi_option_desc_t",  #
        "(&raw mut options[option as usize]) as *mut mi_option_desc_t",
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
                    text = dict_get(msg, "message", "spans", 0, "text", 0)
                    if text is None:
                        continue
                    text = text["text"][
                        text["highlight_start"] - 1 : text["highlight_end"] - 1
                    ]
                    assert isinstance(text, str)
                    print(f"TEXT: {text}")

                    patterns_type1 = [
                        r"^ *(.+?): &mut (.+?)$",
                        r"^ *&mut ([A-Za-z_]+?).+$",
                    ]
                    if any(re.match(pat, text) is not None for pat in patterns_type1):
                        correct = text.replace("&mut ", "&raw mut ")
                        REPLACEMENTS.append((text.strip(), correct))
                        print(f"REPLACEMENT: {text.strip()} -> {correct}")
                        continue

                    if re.match(r"^ *([A-Za-z_]+?)\.([A-Za-z_]+?)\(\).*$", text):
                        correct = re.sub(
                            r"([A-Za-z_]+?)\.([A-Za-z_]+?)\(\)",
                            r"(*(&raw mut \1)).\2()",
                            text,
                        )
                        REPLACEMENTS.append((text.strip(), correct))
                        print(f"REPLACEMENT: {text.strip()} -> {correct}")
                        continue

    for line in lines:
        if line.startswith("#![feature") and "stdsimd" in line:
            line = line.replace("stdsimd,", "link_llvm_intrinsics,")
            yield line

            yield "#![no_std]"
            yield "#![allow(internal_features)]"
            yield "#![allow(unsafe_op_in_unsafe_fn)]"
            yield "#![allow(path_statements)]"
            yield

            yield "#![allow(clippy::missing_safety_doc)]"
            yield "#![allow(clippy::needless_return)]"
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

        if line.strip().startswith('unsafe extern "C" fn mi_'):
            yield line.replace('unsafe extern "C"', 'pub unsafe extern "C"')
            continue

        if line.strip() == "#[no_mangle]":
            yield "#[unsafe(no_mangle)]"
            continue

        if "link_section" in line and "unsafe(link_section" not in line:
            line = re.sub(r"(link_section = \".+\")", r"unsafe(\1)", line)
            yield line
            continue

        for old, new in REPLACEMENTS:
            if old in line:
                line = line.replace(old, new)

        yield line


def transform2(content: str):
    content = content.replace(
        '#[inline]\npub unsafe extern "C" fn mi_',
        '#[unsafe(no_mangle)]\npub unsafe extern "C" fn mi_',
    )
    return content


def transform_lines(from_path: str, to_path: str, f):
    with open(from_path) as src:
        with open(to_path, "w") as dst:
            for line in f(src.read().splitlines()):
                if line is None:
                    dst.write("\n")
                else:
                    dst.write(line + "\n")


def transform_whole(from_path: str, to_path: str, f):
    with open(from_path) as src:
        content = src.read()

    with open(to_path, "w") as dst:
        dst.write(f(content))


def main():
    from_path = "transpile/mimalloc.stage0.rs"
    lib_path = "crates/mimalloc-oxide/src/lib.rs"
    shutil.copyfile(from_path, lib_path)

    for stage_id in [1, 2, 3]:
        from_path = "crates/mimalloc-oxide/src/lib.rs"
        to_path = f"transpile/mimalloc.stage{stage_id}.rs"
        transform_lines(from_path, to_path, transform1)

        shutil.copyfile(to_path, lib_path)
        subprocess.run("cargo fmt", shell=True, check=True)

        transform_whole(lib_path, lib_path, transform2)
        subprocess.run("cargo fmt", shell=True, check=True)

        subprocess.run(
            "cargo build -p mimalloc-oxide --message-format=json 1>temp/oxide.jsonl",
            shell=True,
            check=False,
        )


if __name__ == "__main__":
    main()
