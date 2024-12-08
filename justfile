compile source out:
    gcc -O3 -std=c17 -shared -fPIC -o {{out}} {{source}}

build-single:
    ./scripts/amalgamate.py
    rm -rf build/single
    mkdir -p build/single
    just compile single/mimalloc.c build/single/libmimalloc.so

build-preprocess:
    ./scripts/preprocess.py
    mkdir -p build/preprocess
    just compile preprocess/mimalloc.c build/preprocess/libmimalloc.so

transpile:
    mkdir -p temp
    cargo run -p transpile -- preprocess/mimalloc.stage3.c | tee temp/transpile.stdout.log

c2rust:
    ./scripts/c2rust.sh

postprocess:
    ./scripts/postprocess.py
