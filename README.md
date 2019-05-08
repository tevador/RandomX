# RandomX
RandomX is a proof-of-work (PoW) algorithm that is optimized for general-purpose CPUs. RandomX uses random code execution (hence the name) together with several memory-hard techniques to achieve the following goals:

* Prevent the development of a single-chip [ASIC](https://en.wikipedia.org/wiki/Application-specific_integrated_circuit)
* Minimize the efficiency advantage of specialized hardware compared to a general-purpose CPU

## Overview

RandomX behaves like a keyed hashing function: it accepts a key `K` and arbitrary input `H` and produces a 256-bit result `R`. Under the hood, RandomX utilizes a virtual machine that executes programs in a special instruction set that consists of a mix of integer math, floating point math and branches. These programs can be translated into the CPU's native machine code on the fly. Example of a RandomX program translated into x86-64 assembly is [program.asm](doc/program.asm). A portable interpreter mode is also provided.

RandomX can operate in two main modes with different memory requirements:

* **Fast mode** - requires 2080 MiB of shared memory.
* **Light mode** - requires only 256 MiB of shared memory, but runs significantly slower

## Documentation

Full specification available in [specs.md](doc/specs.md).

Design notes available in [design.md](doc/design.md).

## Build

RandomX is written in C++11 and builds a static library with a C API provided by header file [randomx.h](src/randomx.h). Minimal API usage example is provided in [api-example1.c](src/tests/api-example1.c). The reference code includes a `benchmark` executable for testing.

### Ubuntu/Debian

Build dependencies: `make` and `gcc` (minimum version 4.8, but version 7+ is recommended).

Build using the provided makefile.

### Windows

Build dependencies: Visual Studio 2017.

A solution file is provided.

### Precompiled binaries

Precompiled `benchmark` binaries are available on the [Releases page](https://github.com/tevador/RandomX/releases).

## Proof of work

RandomX was primarily designed as a PoW algorithm for [Monero](https://www.getmonero.org/). The recommended usage is following:

* The key `K` is selected to be the hash of a block in the blockchain - this block is called the 'key block'. For optimal mining and verification performance, the key should change every 2048 blocks (~2.8 days) and there should be a delay of 64 blocks (~2 hours) between the key block and the change of the key `K`. This can be achieved by changing the key when `blockHeight % 2048 == 64` and selecting key block such that `keyBlockHeight % 2048 == 0`.
* The input `H` is the standard hashing blob.

If you wish to use RandomX as a PoW algorithm for your cryptocurrency, we strongly recommend not using the [default parameters](src/configuration.h) and change at least the following:

* Size of the Dataset (`RANDOMX_DATASET_BASE_SIZE` and `RANDOMX_DATASET_EXTRA_SIZE`).
* Scratchpad size (`RANDOMX_SCRATCHPAD_L3`, `RANDOMX_SCRATCHPAD_L2` and `RANDOMX_SCRATCHPAD_L1`).
* Instruction frequencies (parameters starting with `RANDOMX_FREQ_`).

### CPU mining performance
Preliminary performance of selected CPUs using the optimal number of threads (T) and large pages (if possible), in hashes per second (H/s):

|CPU|RAM|OS|AES|Fast mode|Light mode|
|---|---|--|---|---------|--------------|
AMD Ryzen 7 1700|16 GB DDR4|Ubuntu 16.04|hardware|4090 H/s (8T)|620 H/s (16T)|
Intel Core i7-8550U|16 GB DDR4|Windows 10|hardware|1700 H/s (4T)|350 H/s (8T)|
Intel Core i3-3220|2 GB DDR3|Ubuntu 16.04|software|-|145 H/s (4T)|
Raspberry Pi 3|1 GB DDR2|Ubuntu 16.04|software|-|2.0 H/s (4T) †|

† Using the interpreter mode. Compiled mode is expected to increase performance by a factor of 10.

### GPU mining performance

SChernykh has developed a CUDA miner for NVIDIA GPUs. [Benchmarks are listed here](https://github.com/SChernykh/RandomX_CUDA).

Note that GPUs are at a disadvantage when running RandomX since the algorithm was designed to be efficient on CPUs.

# FAQ

### Does RandomX facilitate botnets/malware mining or web mining?
Efficient mining requires more than 2 GiB of memory, which is difficult to hide in an infected computer and disqualifies many low-end machines such as IoT devices. Web mining is nearly impossible due to the large memory requirement and low performance in interpreted mode.

### Since RandomX uses floating point math, does it give reproducible results on different platforms?

RandomX uses only operations that are guaranteed to give correctly rounded results by the [IEEE 754](https://en.wikipedia.org/wiki/IEEE_754) standard: addition, subtraction, multiplication, division and square root. Special care is taken to avoid corner cases such as NaN values or denormals.

The reference implementation has been validated on the following platforms:
* x86+SSE2 (32-bit, little-endian)
* x86-64 (64-bit, little-endian)
* ARMv7+NEON (32-bit, little-endian)
* ARMv8 (64-bit, little-endian)
* PPC64 (64-bit, big-endian)

## Acknowledgements
* [SChernykh](https://github.com/SChernykh) - contributed significantly to the design of RandomX
* [hyc](https://github.com/hyc) - original idea of using random code execution for PoW
* [nioroso-x3](https://github.com/nioroso-x3) - provided access to PowerPC for testing purposes

RandomX uses some source code from the following 3rd party repositories:
* Argon2d, Blake2b hashing functions: https://github.com/P-H-C/phc-winner-argon2

The author of RandomX declares no competing financial interest in RandomX adoption, other than being a holder or Monero. The development of RandomX was funded from the author's own pocket with only the help listed above.

## Donations

If you'd like to use RandomX, please consider donating to help cover the development cost of the algorithm.

Author's XMR address:
```
845xHUh5GvfHwc2R8DVJCE7BT2sd4YEcmjG8GNSdmeNsP5DTEjXd1CNgxTcjHjiFuthRHAoVEJjM7GyKzQKLJtbd56xbh7V
```
