# RVV Check

A simple Pure C (+ some inline assembly) program to check if the RISC-V CPU supports the specified extensions or instructions (RVV 0.7.1 and 1.0).  
It uses `fork()` to launch the check functions (specified inside the `check_funcs` array) in the separate processes and checks if the assembly code snippet in these functions has crashed with `SIGILL` or not.

# How to build on real hardware

`gcc -o rvvcheck rvvcheck.c`

# How to build using cross-compiler toolchain
You need to download RISC-V toolchain (e.g., [riscv-gnu-toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain/releases/tag/2024.08.03))

### Build:
`/opt/toolchains/riscv64-glibc-ubuntu-22.04-gcc-nightly-2024.08.03-nightly/bin/riscv64-unknown-linux-gnu-gcc -o rvvcheck rvvcheck.c`

### Setup QEMU from toolchain:
`export QEMU_LD_PREFIX=/opt/toolchains/riscv64-glibc-ubuntu-22.04-gcc-nightly-2024.08.03-nightly/sysroot`

### List available CPUs:
`/opt/toolchains/riscv64-glibc-ubuntu-22.04-gcc-nightly-2024.08.03-nightly/bin/qemu-riscv64 -cpu help`

### Launch on QEMU:
```
$ /opt/toolchains/riscv64-glibc-ubuntu-22.04-gcc-nightly-2024.08.03-nightly/bin/qemu-riscv64 -cpu max rvvcheck
scalar
rvv1.0

$ /opt/toolchains/riscv64-glibc-ubuntu-22.04-gcc-nightly-2024.08.03-nightly/bin/qemu-riscv64 -cpu rv64 rvvcheck
scalar

$ /opt/toolchains/riscv64-glibc-ubuntu-22.04-gcc-nightly-2024.08.03-nightly/bin/qemu-riscv64 -cpu rv64 rvvcheck -v
scalar
Child #3 PID 201661 exited with status 0
Child #2 PID 201665 exited with signal Illegal instruction
Child #1 PID 201663 exited with signal Illegal instruction
```
