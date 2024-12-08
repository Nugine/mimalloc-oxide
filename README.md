# mimalloc-oxide

An experiment on transpiling [mimalloc](https://github.com/microsoft/mimalloc) to pure Rust using [c2rust](https://github.com/immunant/c2rust).

## Toolchain

OS: Ubuntu 22.04

|               Toolchain               | Version  |
| :-----------------------------------: | :------: |
|      [Rust](https://rustup.rs/)       | ^1.81.0  |
|                Python                 | ^3.10.12 |
| [just](https://github.com/casey/just) | ^1.36.0  |
| [bear](https://github.com/rizsotto/Bear) | - |
| c2rust requirements | https://github.com/immunant/c2rust |

## Reproduction

Download repository:

```bash
git clone https://github.com/Nugine/mimalloc-oxide.git
cd mimalloc-oxide
```

Download submodules:

```bash
git submodule update --init --recursive
```

Merge mimalloc sources:

```bash
just single
```

Transpile mimalloc to Rust:

```bash
just preprocess
just c2rust
just postprocess
```

Run tests:

```bash
just test
```

## Limitations

+ Only supports x86_64-unknown-linux-gnu now
+ Potential bugs and undefined behaviors
