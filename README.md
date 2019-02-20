# RandomX
RandomX is a proof-of-work (PoW) algorithm that is optimized for general-purpose CPUs. RandomX uses random code execution (hence the name) together with several memory-hard techniques to achieve the following goals:

* Prevent the development of a single-chip [ASIC](https://en.wikipedia.org/wiki/Application-specific_integrated_circuit)
* Minimize the efficiency advantage of specialized hardware compared to a general-purpose CPU

## Design

The core of RandomX is a virtual machine (VM), which can be summarized by the following schematic:

![Imgur](https://i.imgur.com/8RYNWLk.png)

Notable parts of the RandomX VM are:

* a large read-only 4 GiB dataset
* a 2 MiB scratchpad (read/write), which is structured into three levels L1, L2 and L3
* 8 integer and 12 floating point registers
* an arithmetic logic unit (ALU)
* a floating point unit (FPU)
* a 2 KiB program buffer

The structure of the VM mimics the components that are found in a typical general purpose computer equipped with a CPU and a large amount of DRAM. The scratchpad is designed to fit into the CPU cache. The first 16 KiB and 256 KiB of the scratchpad are used more often take advantage of the faster L1 and L2 caches. The ratio of random reads from L1/L2/L3 is approximately 9:3:1, which matches the inverse latencies of typical CPU caches.

The VM executes programs in a special instruction set, which was designed in such way that any random 8-byte word is a valid instruction and any sequence of valid instructions is a valid program. For more details see [RandomX ISA documentation](doc/isa.md). Because there are no "syntax" rules, generating a random program is as easy as filling the program buffer with random data. A RandomX program consists of 256 instructions. See [program.inc](src/program.inc) as an example of a RandomX program translated into x86-64 assembly.

### Hash calculation

Calculating a RandomX hash consists of initializing the 2 MiB scratchpad with random data, executing 8 RandomX loops and calculating a hash of the scratchpad.

Each RandomX loop is repeated 2048 times. The loop body has 4 parts:
1. The values of all registers are loaded randomly from the scratchpad (L3)
2. The RandomX program is executed
3. A random block is loaded from the dataset and mixed with integer registers
4. All register values are stored into the scratchpad (L3)

Hash of the register state after 2048 interations is used to initialize the random program for the next loop. The use of 8 different programs in the course of a single hash calculation prevents mining strategies that search for "easy" programs.

The loads from the dataset are fully prefetched, so they don't slow down the loop.

RandomX uses the [Blake2b](https://en.wikipedia.org/wiki/BLAKE_%28hash_function%29#BLAKE2) cryptographic hash function. Special hashing functions `fillAes1Rx4` and `hashAes1Rx4` based on [AES](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard) encryption are used to initialize and hash the scratchpad ([hashAes1Rx4.cpp](src/hashAes1Rx4.cpp)).

### Hash verification

RandomX is a symmetric PoW algorithm, so the verifying party has to repeat the same steps as when a hash is calculated.

However, to allow hash verification on devices that cannot store the whole 4 GiB dataset, RandomX allows a time-memory tradeoff by using just 256 MiB of memory at the cost of 16 times more random memory accesses. See [Dataset initialization](doc/dataset.md) for more details.

### Performance
Preliminary mining performance with the x86-64 JIT compiled VM:

|CPU|RAM|threads|hashrate [H/s]|comment|
|-----|-----|----|----------|-----|
|AMD Ryzen 1700 (desktop)|DDR4-2933|8|4100|
|Intel i5-3230M (laptop)|DDR3-1333|1|280|without large pages
|Intel i7-8550U (laptop)|DDR4-2400|4|1650|limited by thermals
|Intel i5-2500K (desktop)|DDR3-1333|3|1350|

Hash verification is performed using the portable interpreter in "light-client mode" and takes 30-70 ms depending on RAM latency and CPU clock speed. Hash verification in "mining mode" takes 2-4 ms.

### Documentation
* [RandomX ISA](doc/isa.md)
* [RandomX instruction listing](doc/isa-ops.md)
* [Dataset initialization](doc/dataset.md)

# FAQ

### Can RandomX run on a GPU?

We don't expect GPUs will ever be competitive in mining RandomX. The reference miner is CPU-only.

A rough estimate for AMD Vega 56 GPU gave an upper limit of 1200 H/s, or slightly less than a quad core CPU (details in issue [#24](https://github.com/tevador/RandomX/issues/24)).

RandomX was designed to be efficient on CPUs. Designing an algorithm compatible with both CPUs and GPUs brings too many limitations and ultimately decreases ASIC resistance. CPUs have the advantage of not needing proprietary drivers and most CPU architectures support a large common subset of native operations.

Additionally, targeting CPUs allows for more decentralized mining for several reasons:

* Every computer has a CPU and even laptops will be able to mine efficiently.
* CPU mining is easier to set up - no driver compatibility issues, BIOS flashing etc.
* CPU mining is more difficult to centralize because computers can usually have only one CPU except for expensive server parts.

### Does RandomX facilitate botnets/malware mining or web mining?
Quite the opposite. Efficient mining requires 4 GiB of memory, which is very difficult to hide in an infected computer and disqualifies many low-end machines. Web mining is nearly impossible due to the large memory requirement and the need for a rather lengthy initialization of the dataset.

### Since RandomX uses floating point calculations, how can it give reproducible results on different platforms?

RandomX uses only operations that are guaranteed to give correctly rounded results by the [IEEE 754](https://en.wikipedia.org/wiki/IEEE_754) standard: addition, subtraction, multiplication, division and square root. Special care is taken to avoid corner cases such as NaN values or denormals.

## Acknowledgements
The following people have contributed to the design of RandomX:
* [SChernykh](https://github.com/SChernykh)
* [hyc](https://github.com/hyc)

RandomX uses some source code from the following 3rd party repositories:
* Argon2d, Blake2b hashing functions: https://github.com/P-H-C/phc-winner-argon2
* Software AES implementation https://github.com/fireice-uk/xmr-stak

## Donations
XMR:
```
845xHUh5GvfHwc2R8DVJCE7BT2sd4YEcmjG8GNSdmeNsP5DTEjXd1CNgxTcjHjiFuthRHAoVEJjM7GyKzQKLJtbd56xbh7V
```
