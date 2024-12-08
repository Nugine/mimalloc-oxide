#!/bin/bash -ex
OUT_DIR="$(pwd)/build/release"
rm -rf $OUT_DIR
mkdir -p $OUT_DIR

SO=$(pwd)/target/release/libmimalloc_oxide.so
ln -s $SO $OUT_DIR/libmimalloc.so.1
ln -s $SO $OUT_DIR/libmimalloc.so

cp ./mimalloc/out/release/mimalloc-test* $OUT_DIR
cd $OUT_DIR
env LD_PRELOAD=$OUT_DIR/libmimalloc.so.1 ./mimalloc-test-api
env LD_PRELOAD=$OUT_DIR/libmimalloc.so.1 ./mimalloc-test-fill
env LD_PRELOAD=$OUT_DIR/libmimalloc.so.1 ./mimalloc-test-stress
