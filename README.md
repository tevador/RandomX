# RandomX
RandomX is a proof-of-work (PoW) algorithm that is optimized for general-purpose CPUs. RandomX uses random code execution (hence the name) together with several memory-hard techniques to minimize the efficiency advantage of specialized hardware.

## Overview

RandomX utilizes a virtual machine that executes programs in a special instruction set that consists of integer math, floating point math and branches. These programs can be translated into the CPU's native machine code on the fly (example: [program.asm](doc/program.asm)). At the end, the outputs of the executed programs are consolidated into a 256-bit result using a cryptographic hashing function ([Blake2b](https://blake2.net/)).

RandomX can operate in two main modes with different memory requirements:

* **Fast mode** - requires 2080 MiB of shared memory.
* **Light mode** - requires only 256 MiB of shared memory, but runs significantly slower

Both modes are interchangeable as they give the same results. The fast mode is suitable for "mining", while the light mode is expected to be used only for proof verification.

## Documentation

Full specification is available in [specs.md](doc/specs.md).

Design description and analysis is available in [design.md](doc/design.md).

## Audits

Between May and August 2019, RandomX was audited by 4 independent security research teams:

* [Trail of Bits](https://www.trailofbits.com/) (28 000 USD)
* [X41 D-SEC](https://www.x41-dsec.de/) (42 000 EUR)
* [Kudelski Security](https://www.kudelskisecurity.com/) (18 250 CHF)
* [QuarksLab](https://quarkslab.com/en/) (52 800 USD)

The first audit was generously funded by [Arweave](https://www.arweave.org/), one of the early adopters of RandomX. The remaining three audits were funded by donations from the [Monero community](https://ccs.getmonero.org/proposals/RandomX-audit.html). All four audits were coordinated by [OSTIF](https://ostif.org/).

Final reports from all four audits are available in the [audits](audits/) directory. None of the audits found any critical vulnerabilities, but several changes in the algorithm and the code were made as a direct result of the audits. More details can be found in the [final report by OSTIF](https://ostif.org/four-audits-of-randomx-for-monero-and-arweave-have-been-completed-results/).

## Build

RandomX is written in C++11 and builds a static library with a C API provided by header file [randomx.h](src/randomx.h). Minimal API usage example is provided in [api-example1.c](src/tests/api-example1.c). The reference code includes a `randomx-benchmark` and `randomx-tests` executables for testing.

### Linux

Build dependencies: `cmake` (minimum 2.8.7) and `gcc` (minimum version 4.8, but version 7+ is recommended).

To build optimized binaries for your machine, run:
```
git clone https://github.com/tevador/RandomX.git
cd RandomX
mkdir build && cd build
cmake -DARCH=native ..
make
```

### Windows

On Windows, it is possible to build using MinGW (same procedure as on Linux) or using Visual Studio (solution file is provided).

### Precompiled binaries

Precompiled `randomx-benchmark` binaries are available on the [Releases page](https://github.com/tevador/RandomX/releases).

## Proof of work

RandomX was primarily designed as a PoW algorithm for [Monero](https://www.getmonero.org/). The recommended usage is following:

* The key `K` is selected to be the hash of a block in the blockchain - this block is called the 'key block'. For optimal mining and verification performance, the key should change every 2048 blocks (~2.8 days) and there should be a delay of 64 blocks (~2 hours) between the key block and the change of the key `K`. This can be achieved by changing the key when `blockHeight % 2048 == 64` and selecting key block such that `keyBlockHeight % 2048 == 0`.
* The input `H` is the standard hashing blob with a selected nonce value.

If you wish to use RandomX as a PoW algorithm for your cryptocurrency, please follow the [configuration guidelines](doc/configuration.md).

### CPU performance
The table below lists the performance of selected CPUs using the optimal number of threads (T) and large pages (if possible), in hashes per second (H/s). "CNv4" refers to the CryptoNight variant 4 (CN/R) hashrate measured using [XMRig](https://github.com/xmrig/xmrig) v2.14.1. "Fast mode" and "Light mode" are the two modes of RandomX.

|CPU|RAM|OS|AES|CNv4|Fast mode|Light mode|
|---|---|--|---|-----|------|--------------|
Intel Core i9-9900K|32G DDR4-3200|Windows 10|hw|660 (8T)|5770 (8T)|1160 (16T)|
AMD Ryzen 7 1700|16G DDR4-2666|Ubuntu 16.04|hw|520 (8T)|4100 (8T)|620 (16T)|
Intel Core i7-8550U|16G DDR4-2400|Windows 10|hw|200 (4T)|1700  (4T)|350 (8T)|
Intel Core i3-3220|4G DDR3-1333|Ubuntu 16.04|soft|42 (4T)|510 (4T)|150 (4T)|
Raspberry Pi 3|1G LPDDR2|Ubuntu 16.04|soft|3.5 (4T)|-|20 (4T)|

Note that RandomX currently includes a JIT compiler for x86-64 and ARM64. Other architectures have to use the portable interpreter, which is much slower.

### GPU performance

SChernykh is developing GPU mining code for RandomX. Benchmarks are included in the following repositories:

* [CUDA miner](https://github.com/SChernykh/RandomX_CUDA) - NVIDIA GPUs.
* [OpenCL miner](https://github.com/SChernykh/RandomX_OpenCL) - only for AMD Vega and AMD Polaris GPUs (uses GCN machine code).

The code from the above repositories is included in the open source miner [XMRig](https://github.com/xmrig/xmrig).

Note that GPUs are at a disadvantage when running RandomX since the algorithm was designed to be efficient on CPUs.

# FAQ

### Which CPU is best for mining RandomX?

Most Intel and AMD CPUs made since 2011 should be fairly efficient at RandomX. More specifically, efficient mining requires:

* 64-bit architecture
* IEEE 754 compliant floating point unit
* Hardware AES support ([AES-NI](https://en.wikipedia.org/wiki/AES_instruction_set) extension for x86, Cryptography extensions for ARMv8)
* 16 KiB of L1 cache, 256 KiB of L2 cache and 2 MiB of L3 cache per mining thread
* Support for large memory pages
* At least 2.5 GiB of free RAM per NUMA node
* Multiple memory channels may be required:
    * DDR3 memory is limited to about 1500-2000 H/s per channel (depending on frequency and timings)
    * DDR4 memory is limited to about 4000-6000 H/s per channel  (depending on frequency and timings)

### Does RandomX facilitate botnets/malware mining or web mining?
Efficient mining requires more than 2 GiB of memory, which is difficult to hide in an infected computer and disqualifies many low-end machines such as IoT devices. Web mining is infeasible due to the large memory requirement and the lack of directed rounding support for floating point operations in both Javascript and WebAssembly.

### Since RandomX uses floating point math, does it give reproducible results on different platforms?

RandomX uses only operations that are guaranteed to give correctly rounded results by the [IEEE 754](https://en.wikipedia.org/wiki/IEEE_754) standard: addition, subtraction, multiplication, division and square root. Special care is taken to avoid corner cases such as NaN values or denormals.

The reference implementation has been validated on the following platforms:
* x86 (32-bit, little-endian)
* x86-64 (64-bit, little-endian)
* ARMv7+VFPv3 (32-bit, little-endian)
* ARMv8 (64-bit, little-endian)
* PPC64 (64-bit, big-endian)

### Can FPGAs mine RandomX?

RandomX generates multiple unique programs for every hash, so FPGAs cannot dynamically reconfigure their circuitry because typical FPGA takes tens of seconds to load a bitstream. It is also not possible to generate bitstreams for RandomX programs in advance due to the sheer number of combinations (there are 2<sup>512</sup> unique programs).

Sufficiently large FPGAs can mine RandomX in a [soft microprocessor](https://en.wikipedia.org/wiki/Soft_microprocessor) configuration by emulating a CPU. Under these circumstances, an FPGA will be much less efficient than a CPU or a specialized chip (ASIC).

## Acknowledgements
* [tevador](https://github.com/tevador) - author
* [SChernykh](https://github.com/SChernykh) - contributed significantly to the design of RandomX
* [hyc](https://github.com/hyc) - original idea of using random code execution for PoW
* [Other contributors](https://github.com/tevador/RandomX/graphs/contributors)

RandomX uses some source code from the following 3rd party repositories:
* Argon2d, Blake2b hashing functions: https://github.com/P-H-C/phc-winner-argon2

The author of RandomX declares no competing financial interest.

## Donations

If you'd like to use RandomX, please consider donating to help cover the development cost of the algorithm.

Author's XMR address:
```
845xHUh5GvfHwc2R8DVJCE7BT2sd4YEcmjG8GNSdmeNsP5DTEjXd1CNgxTcjHjiFuthRHAoVEJjM7GyKzQKLJtbd56xbh7V
```
Total donations received: ~3.86 XMR (as of 30th August 2019). Thanks to all contributors.
