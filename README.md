# RandomX
RandomX is an experimental proof of work (PoW) algorithm that uses random code execution to achieve ASIC resistance.

### Key features

* CPU-friendly (especially for x86 and ARM architectures)
* ASIC-resistant, FPGA-resistant, GPU-resistant
* Memory-hard (requires  >4 GiB of memory)
* Web-mining resistant due to high memory requirement

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

## Proof of work

RandomX produces a 256-bit final hash value to be used for a Hashcash-style proof evaluation.
The hash of the block header is used for the first VM initialization. The program initialization and execution are chained multiple times to prevent mining strategies that search for programs with particular properties (for example, without division).

The final result is obtained by calculating a Blake2b hash of the Register file and a checkum of the Scratchpad.

## Acknowledgements
The following people have contributed to the design of RandomX:
* [SChernykh](https://github.com/SChernykh)
* [hyc](https://github.com/hyc)

RandomX uses some source code from the following 3rd party repositories:
* Argon2d, Blake2b hashing functions: https://github.com/P-H-C/phc-winner-argon2
* PCG32 random number generator: https://github.com/imneme/pcg-c-basic
* Software AES implementation https://github.com/fireice-uk/xmr-stak
* t1ha2 hashing function: https://github.com/leo-yuriev/t1ha