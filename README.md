# RandomX
RandomX is a proof-of-work (PoW) algorithm that is optimized for general-purpose CPUs. RandomX uses random code execution (hence the name) together with several memory-hard techniques to achieve the following goals:

* Prevent the development of a single-chip [ASIC](https://en.wikipedia.org/wiki/Application-specific_integrated_circuit)
* Minimize the efficiency advantage of specialized hardware compared to a general-purpose CPU

## Specification

Full specification available in [specs.md](doc/specs.md).

## Design

Design notes available in [design.md](doc/design.md).

## Build

Build using `make`. Requires a C++11 compliant compiler. There are no dependencies.

Precompiled test binaries are available on the [Releases page](https://github.com/tevador/RandomX/releases).

## Usage

```
Usage: randomx [OPTIONS]
Supported options:
  --help        shows this message
  --mine        mining mode: 2 GiB, x86-64 JIT compiled VM
  --verify      verification mode: 256 MiB
  --jit         x86-64 JIT compiled verification mode (default: interpreter)
  --largePages  use large pages
  --softAes     use software AES (default: x86 AES-NI)
  --threads T   use T threads (default: 1)
  --init Q      initialize dataset with Q threads (default: 1)
  --nonces N    run N nonces (default: 1000)
  --genAsm      generate x86-64 asm code for nonce N
  --genNative   generate RandomX code for nonce N
```

### Mining mode
Mining mode requires >2 GiB of RAM and optimal performance should be obtained with at least 16 KiB of L1 cache, 256 KiB of L2 cache and 2 MiB of L3 cache per mining thread.

The reference miner supports only x86 64-bit CPUs at the moment. [AES-NI](https://en.wikipedia.org/wiki/AES_instruction_set) support is not required, but using the `--softAes` option reduces mining performance by about 40%.

It is recommended to use [large pages](https://en.wikipedia.org/wiki/Page_(computer_memory)#Multiple_page_sizes) with the `--largePages` option. Using the default page size can reduce performance by up to 50% due to [TLB thrashing](https://en.wikipedia.org/wiki/Thrashing_(computer_science)#TLB_thrashing).

[NUMA](https://en.wikipedia.org/wiki/Non-uniform_memory_access) systems should run one instance of RandomX per NUMA node.

### Light mode

Verification is done in the 'light' mode, which requires only 256 MiB of memory, but runs much slower than the mining mode. Use the `--jit` option on x86-64 CPUs for maximum verification performance. 

### Performance
Preliminary performance using the optimal number of threads and large pages (if possible):

|CPU|RAM|OS|AES|RandomX (mining)|RandomX (light)|
|---|---|--|---|---------|--------------|
AMD Ryzen 7 1700|16 GB DDR4|Ubuntu 16.04|HW|4250 H/s (8T)|640 H/s (16T)|
Intel Core i7-8550U|16 GB DDR4|Windows 10|HW|1660 H/s (4T)|128 H/s (4T)|
Intel Core i3-3220|2 GB DDR3|Ubuntu 16.04|software|-|187 H/s (4T)|
Raspberry Pi 3|1 GB DDR2|Ubuntu 16.04|software|-|12.3 H/s (4T)|

# FAQ

### Can RandomX run on a GPU?

RandomX was designed to be efficient on CPUs. Designing an algorithm compatible with both CPUs and GPUs brings too many limitations and ultimately decreases ASIC resistance.

GPUs are expected to be at a disadvantage when running RandomX, but the exact performance has not been determined yet due to lack of a working GPU implementation.

A rough estimate for AMD Vega 56 GPU gave an upper limit of 1200 H/s, comparable to a quad core CPU (details in issue [#24](https://github.com/tevador/RandomX/issues/24)).

### Does RandomX facilitate botnets/malware mining or web mining?
Quite the opposite. Efficient mining requires 2 GiB of memory, which is difficult to hide in an infected computer and disqualifies many low-end machines such as IoT devices. Web mining is nearly impossible due to the large memory requirements and low performance in interpreted mode.

### Since RandomX uses floating point calculations, how can it give reproducible results on different platforms?

RandomX uses only operations that are guaranteed to give correctly rounded results by the [IEEE 754](https://en.wikipedia.org/wiki/IEEE_754) standard: addition, subtraction, multiplication, division and square root. Special care is taken to avoid corner cases such as NaN values or denormals.

## Acknowledgements
The following people have contributed to the design of RandomX:
* [SChernykh](https://github.com/SChernykh)
* [hyc](https://github.com/hyc)

RandomX uses some source code from the following 3rd party repositories:
* Argon2d, Blake2b hashing functions: https://github.com/P-H-C/phc-winner-argon2

## Donations
XMR:
```
845xHUh5GvfHwc2R8DVJCE7BT2sd4YEcmjG8GNSdmeNsP5DTEjXd1CNgxTcjHjiFuthRHAoVEJjM7GyKzQKLJtbd56xbh7V
```
