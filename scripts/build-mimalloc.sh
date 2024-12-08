#!/bin/bash -ex
pushd mimalloc
    mkdir -p out/release
    cd out/release
    cmake ../..
    make
popd
