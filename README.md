# RandomX
RandomX is an experimental proof of work (PoW) algorithm that uses random code execution.

### Key features

* Memory-hard (requires  >4 GiB of memory)
* CPU-friendly (especially for x86 and ARM architectures)
* arguably ASIC-resistant
* inefficient on GPUs
* unusable for web-mining

## Virtual machine

RandomX is intended to be run efficiently on a general-purpose CPU. The virtual machine (VM) which runs RandomX code attempts to simulate a generic CPU using the following set of components:

![Imgur](https://i.imgur.com/ZAfbX9m.png)

Full description: [vm.md](doc/vm.md).

## Dataset

RandomX uses a 4 GiB read-only dataset. The dataset is constructed using a combination of the [Argon2d](https://en.wikipedia.org/wiki/Argon2) hashing function, [AES](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard) encryption/decryption and a random permutation. The dataset is regenerated every ~34 hours.

Full description: [dataset.md](doc/dataset.md).

## Instruction set

RandomX uses a simple low-level language (instruction set), which was designed so that any random bitstring forms a valid program. Each RandomX instruction has a length of 128 bits.

Full description: [isa.md](doc/isa.md).

## Implementation
Proof-of-concept implementation is written in C++.
```
> bin/randomx --help
Usage: bin/randomx [OPTIONS]
Supported options:
        --help                  shows this message
        --compiled              use x86-64 JIT-compiled VM (default: interpreted VM)
        --lightClient           use 'light-client' mode (default: full dataset mode)
        --softAes               use software AES (default: x86 AES-NI)
        --threads T             use T threads (default: 1)
        --nonces N              run N nonces (default: 1000)
        --genAsm                generate x86 asm code for nonce N
```

Two RandomX virtual machines are implemented:

### Interpreted VM
The interpreted VM is the reference implementation, which aims for maximum portability.

The VM has been tested for correctness on the following platforms:
* Linux: x86-64, ARMv7 (32-bit), ARMv8 (64-bit)
* Windows: x86, x86-64
* MacOS: x86-64

The interpreted VM supports two modes: "full dataset" mode, which requires more than 4 GiB of virtual memory, and a "light-client" mode, which requires about 64 MiB of memory, but runs significantly slower because dataset blocks are created on the fly rather than simply fetched from memory.

Software AES implementation is available for CPUs which don't support [AES-NI](https://en.wikipedia.org/wiki/AES_instruction_set).

The following table lists the performance for Intel Core i5-3230M (Ivy Bridge) CPU using a single core on Windows 64-bit, compiled with Visual Studio 2017:

|mode|required memory|AES|initialization time [s]|performance [programs/s]|
|------|----|-----|-------------------------|------------------|
|light client|64 MiB|software|1.0|9.2|
|light client|64 MiB|AES-NI|1.0|16|
|full dataset|4 GiB|software|54|40|
|full dataset|4 GiB|AES-NI|26|40|

### JIT-compiled VM
A JIT compiler is available for x86-64 CPUs. This implementation shows the approximate performance that can be achieved using optimized mining software. The JIT compiler generates generic x86-64 code without any architecture-specific optimizations. Only "full dataset" mode is supported.

For optimal performance, an x86-64 CPU needs:
* 32 KiB of L1 instruction cache per thread
* 16 KiB of L1 data cache per thread
* 240 KiB of L2 cache (exclusive) per thread

The following table lists the performance of AMD Ryzen 7 1700 (clock fixed at 3350 MHz, 1.05 Vcore, dual channel DDR4 2400 MHz) on Linux 64-bit (compiled with GCC 5.4.0).

Power consumption was measured for the whole system using a wall socket wattmeter (±1W). Table lists difference over idle power consumption. [Prime95](https://en.wikipedia.org/wiki/Prime95#Use_for_stress_testing)  (small/in-place FFT) and [Cryptonight V2](https://github.com/monero-project/monero/pull/4218) power consumption are listed for comparison.

||threads|initialization time [s]|performance [programs/s]|power [W]
|-|------|----|-----|-------------------------|
|RandomX (interpreted)|1|27|52|16|
|RandomX (interpreted)|8|4.0|390|63|
|RandomX (interpreted)|16|3.5|620|74|
|RandomX (compiled)|1|27|407|17|
|RandomX (compiled)|2|14|810|26|
|RandomX (compiled)|4|7.3|1620|42|
|RandomX (compiled)|6|5.1|2410|56|
|RandomX (compiled)|8|4.0|3200|71|
|RandomX (compiled)|12|4.0|3670|82|
|RandomX (compiled)|16|3.5|4110|92|
|Cryptonight v2|8|-|-|47|
|Prime95|8|-|-|77|
|Prime95|16|-|-|81|

## Proof of work

RandomX VM can be used for PoW using the following steps:

1. Initialize the VM using a 256-bit hash of any data.
2. Execute the RandomX program.
3. Calculate `blake2b(RegisterFile || t1ha2(Scratchpad))`*

\* [blake2b](https://en.wikipedia.org/wiki/BLAKE_%28hash_function%29#BLAKE2) is a cryptographic hash function, [t1ha2](https://github.com/leo-yuriev/t1ha) is a fast hashing function.

The above steps can be chained multiple times to prevent mining strategies that search for programs with particular properties (for example, without division).

## Acknowledgements
The following people have contributed to the design of RandomX:
* [SChernykh](https://github.com/SChernykh)
* [hyc](https://github.com/hyc)

RandomX uses some source code from the following 3rd party repositories:
* Argon2d, Blake2b hashing functions: https://github.com/P-H-C/phc-winner-argon2
* PCG32 random number generator: https://github.com/imneme/pcg-c-basic
* Software AES implementation https://github.com/fireice-uk/xmr-stak
* t1ha2 hashing function: https://github.com/leo-yuriev/t1ha

## Donations

XMR:
```
4B9nWtGhZfAWsTxWujPDGoWfVpJvADxkxJJTmMQp3zk98n8PdLkEKXA5g7FEUjB8JPPHdP959WDWMem3FPDTK2JUU1UbVHo
```
