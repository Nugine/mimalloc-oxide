#!/bin/bash -ex

pushd c2rust
    cargo build -p c2rust --bins
popd

C2RUST="$(pwd)/c2rust/target/debug/c2rust"

rm -rf transpile
mkdir -p transpile

rm compile_commands.json
bear --output compile_commands.json -- just build-preprocess

$C2RUST transpile compile_commands.json
mv preprocess/mimalloc.rs transpile/mimalloc.stage0.rs
