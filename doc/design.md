# RandomX design
To minimize the performance advantage of specialized hardware, a proof of work (PoW) algorithm must achieve *device binding* by targeting specific features of existing general-purpose hardware. This is complicated by the fact that we have to target a large class of devices with different architectures from different manufacturers.


There are two distinct classes of general processing devices: CPUs and GPUs. RandomX targets CPUs for the following reasons:

* CPUs, being less specialized devices, are more prevalent and widely accessible. A CPU-bound algorithm is more egalitarian and allows more participants to join the network. This is one of the goals stated in the original CryptoNote whitepaper [[1](https://cryptonote.org/whitepaper.pdf)]. 
* A large common subset of native hardware instructions exists among different CPU architectures. The same cannot be said about GPUs. For example, there is no common integer multiplication instruction for nVidia and AMD GPUs [[2](https://github.com/ifdefelse/ProgPOW/issues/16)].
* All major CPU instruction sets are well documented with multiple open source compilers available. In comparison, GPU instruction sets are usually proprietary and may require vendor specific closed-source drivers for maximum performance.

## Design considerations

A CPU-bound algorithm must make use of as many CPU components as possible. Typical CPU contains the following building blocks:

* Instruction fetch/decode
* Branch processing unit
* Caches
* Floating point unit
* Arithmetic logic unit
* Load-store unit
* Memory controller

Modern CPUs also contain other circuits, such as PCI-E controllers, SATA, USB or Ethernet. Unfortunately, these cannot be used to perform any useful work.

### Instruction fetch/decode

The ability to execute an arbitrary stream of instructions is the most important property of a CPU. The CPU execution units are not hardwired, but can be selected dynamically by software.

Algorithms with a fixed or slowly changing sequence of operations can be implemented more efficiency in specialized hardware, while CPUs always have to expend energy on decoding instructions. Therefore, RandomX executes a unique sequence of instructions for every hash calculation.

### Branches

Modern CPUs include a sophisticated branch predictor unit [[3](https://en.wikipedia.org/wiki/Branch_predictor)] to ensure uninterrupted flow of instructions. If a branch prediction fails, the speculatively executed instructions are thrown away, which results in a certain amount of wasted energy with each misprediction. To maximize the amount of useful work per unit of energy, mispredictions must be minimized.

The best way to maximize CPU efficiency is not to have any branches at all. However, CPUs invest a lot of die area and energy to handle branches. Without branches, CPU design can be significantly simplified because there is no need for commit/retire stages, which must be part of all speculative-execution designs to be able to recover from branch mispredictions.

RandomX therefore uses random branches with a jump probability of 1/256. These branches will be predicted as "not taken" by the CPU. Such branches are "free" in most CPU designs unless they are taken. The branching conditions and jump targets are chosen in such way that infinite loops in RandomX code are impossible because the register controlling the branch will never be modified in the repeated code block. Each CBRANCH instruction can jump at most twice in a row. The additional instructions executed due to branches represent less than 1% of all instructions.

Additionally, branches in the code significantly reduce the possibilities of static optimizations. For example, the ISWAP_R instruction could be optimized away by renaming registers if it wasn't for branches.

### CPU Caches

#### L1 and L2 cache

L1 and L2 caches are located close to the CPU execution units and provide the best random access latency of around 3-15 cycles. This latency can be efficiently hidden by out of order execution [[5](https://en.wikipedia.org/wiki/Out-of-order_execution)]. RandomX programs access almost exclusively "L1" and "L2" parts of the Scratchpad with a 3:1 access ratio to take advantage of the low latency caches.

#### L3 cache
The L3 cache is much larger and located further from the CPU core. As a result, its access latencies are usually around 30-50 cycles. While this is much faster than reading data from DRAM, it can cause stalls in program execution.

RandomX therefore performs only 2 random accesses into "L3" Scratchpad per program iteration. Register values from a given iteration are written into the same location, which guarantees that the required cache lines have been moved to the faster L1 or L2 caches.

Additionally, integer instructions that read from a fixed address also use the whole "L3" Scratchpad (Table 5.1.4 of the Specification) because repetitive accesses will ensure that the cache line will be placed in the L1 cache of the CPU.

#### μop cache

Modern x86 CPUs decode complex instructions into simpler micro operations (μops). Repeatedly executed instructions are not decoded from memory, but are executed directly from the μop cache [[6](https://en.wikipedia.org/wiki/CPU_cache#Micro-operation_(%CE%BCop_or_uop)_cache)]. RandomX programs are relatively short loops (typical program compiles into 600-700 μops on Intel Skylake or AMD Ryzen), so they fit entirely into the μop cache, which allows the CPU to power down the relatively power-hungry x86 instruction decoders. This should help to equalize the power efficiency between x86 and simpler architectures like ARM.

### Registers

RandomX uses 8 integer registers and 12 floating point registers. This is the maximum that can be allocated as physical registers in x86-64, which has the fewest architectural registers among existing 64-bit CPU architectures.

### Floating point operations

RandomX uses double precision floating point operations, which are supported by the majority of CPUs and require more complex hardware than single precision. All operations are performed as vector operations, which is also supported by all major CPU architectures.

RandomX uses five operations that are guaranteed by the IEEE 754 standard to give correctly rounded results: addition, subtraction, multiplication, division and square root. All 4 rounding modes defined by the standard are used.

The domains of floating point operations are separated into "additive" operations, which use register group F and "multiplicative" operations, which use register group E. This is done to prevent addition/subtraction from becoming no-op when a small number is added to a large number. Since the range of the F group registers is limited to around `±3.0e+14`, adding or subtracting a floating point number with absolute value larger than 1 always changes at least 5 fraction bits.

Because the limited range of group F registers would allow the use of a more efficient fixed-point representation (with 80-bit numbers), the FSCAL instruction manipulates the binary representation of the floating point format to make this optimization more difficult.

Group E registers are restricted to positive values, which avoids `NaN` results (such as square root of a negative number or `0 * ∞`). Division uses only memory source operand to avoid being optimized into multiplication by constant reciprocal. The exponent of group E operands is set to a value between -255 and 0 to avoid division and multiplication by 0 and to increase the range of numbers that can be obtained. The approximate range of possible group E register values is `1.7E-77` to `infinity`.

While all register-register operations use only 4 static source operands (registers `a0`-`a1`), the vast majority of operations cannot be optimized because floating point math is not associative (for example `x - a0 + a0` generally doesn't equal `x`). Additionally, optimizations are more difficult due to branches in the code (for example, the sequence of operations `x - a0; CBRANCH; x + a0` can produce a value close to `x` or `x - a0` depending on if the branch was taken or not).

To maximize entropy and also to fit into one 64-byte cache line, floating point registers are combined using the XOR operation at the end of each iteration before being stored into the Scratchpad.

### Integer operations

RandomX uses all primitive integer operations that preserve entropy: addition, subtraction, multiplication, XOR and rotation.

The IADD_RS instruction utilizes the address calculation logic of CPUs and can be performed in a single hardware instruction by most CPUs.

Because integer division is not fully pipelined in CPUs and can be made faster in ASICs, the IMUL_RCP instruction requires only one division per program to calculate the reciprocal. This forces an ASIC to include a hardware divider without giving them a performance advantage during program execution.

### Memory access

RandomX randomly reads from large buffer of data (Dataset) 16384 times for each hash calculation. Since the Dataset must be stored in DRAM, it provides a natural parallelization limit, because DRAM cannot do more than about 25 million random accesses per second per bank group. Each separately addressable bank group allows a throughput of around 1500 H/s.

All Dataset accesses read whole CPU cache line (64 bytes) and are fully prefetched. The time to execute one program iteration described in chapter 4.6.2 of the Specification is about the same as typical DRAM access latency.

#### Cache

The Cache, which is used for light verification and Dataset construction, is about 8 times smaller than the Dataset. To keep a constant area-time product, each Dataset item is constructed from 8 random Cache accesses. The Dataset is 32 MiB larger than 8 times the Cache size. These additional 32 MiB compensate for chip area needed by SuperscalarHash.

Because 256 MiB is small enough to be included on-chip, RandomX uses a high-latency, high-power mixing function (SuperscalarHash) which defeats the benefits of using low-latency memory and the energy required to calculate SuperscalarHash makes light mode very inefficient for mining.

Using less than 256 MiB of memory is not possible due to the use of tradeoff-resistant Argon2d with 3 iterations. When using 3 iterations (passes), halving the memory usage increases computational cost 3423 times for the best tradeoff attack [[7](https://eprint.iacr.org/2015/430.pdf)].

#### Scratchpad

The Scratchpad is used as read-write memory. Its size was selected to fit entirely into CPU cache. Programs make, on average, 39 reads (instructions IADD_M, ISUB_M, IMUL_M, IMULH_M, ISMULH_M, IXOR_M, FADD_M, FSUB_M, FDIV_M, COND_M) and 16 writes (instruction ISTORE) to the Scratchpad per program iteration. This is close to a 2:1 read/write ratio, which CPUs are optimized for.

Additionally, Scratchpad operations require write-read coherency, because when a write to L1 Scratchpad is in progress, a read has a 1/2048 chance of being from the same address. This is handled by the load-store unit (LSU) inside the CPU and requires every read to be checked against the addresses of all pending writes. Hardware without these coherency checks will produce >99% of invalid results.

While most writes to the Scratchpad are into L1 and L2, 144 bytes of data are written into L3 scratchpad per iteration, on average (64 bytes for integer registers, 64 bytes for floating point registers and 16 bytes by ISTORE instruction).

The image below visualizes writes to the Scratchpad. Each pixel in this image represents 8 bytes of the Scratchpad. Red pixels represent portions of the Scratchpad that have been overwritten at least once during hash calculation. The L1 and L2 levels are on the left side (almost completely overwritten). The right side of the scratchpad represents the bottom 1792 KiB. Only about 66% of it are overwritten, but the writes are spread uniformly and randomly.

![Imgur](https://i.imgur.com/pRz6aBG.png)

### Choice of hashing function
RandomX uses Blake2b as its main cryptographically secure hashing function. Blake2b was specifically designed to be fast in software, especially on modern 64-bit processors, where it's around three times faster than SHA-3 and can run at a speed of around 3 clock cycles per byte of input.

### Custom functions

#### SuperscalarHash

SuperscalarHash was designed to burn as much power as possible while the CPU is waiting for data to be loaded from DRAM. The target latency of 170 cycles corresponds to the usual DRAM latency of 40-80 ns. ASIC devices designed for light-mode mining with low-latency memory will be bottlenecked by SuperscalarHash when calculating Dataset items and their efficiency will be destroyed by the high power usage of SuperscalarHash.

The average SuperscalarHash function contains a total of 450 instructions, out of which 155 are 64-bit multiplications. On average, the longest dependency chain is 95 instructions long. An ASIC design for light-mode mining, with 256 MiB of on-die memory and the ability to execute 1 instruction per cycle, will need on average 760 cycles to construct a Dataset item, assuming unlimited parallelization. It will have to execute 1240 64-bit multiplications per item, which will consume energy comparable to loading data from DRAM.

#### AesGenerator

AesGenerator was designed for fastest possible generation of pseudorandom data. It takes advantage of hardware accelerated AES in modern CPUs. Only one AES round is performed per 16 bytes of output, which results in throughput exceeding 20 GB/s. The Scratchpad can be filled in under 100 μs. The AesGenerator state is initialized from the output of Blake2b.

#### AesHash

AesHash was designed for fastest possible calculation of the Scratchpad fingerprint. It interprets the Scratchpad as a set of AES round keys, so it's equivalent to AES encryption with 32768 rounds. Two extra rounds are performed at the end to ensure avalanche of all Scratchpad bits in each lane. The output of the AesHash is fed into the Blake2b hashing function to calculate the final PoW hash.

### Chaining of VM executions

RandomX chains 8 VM initializations and executions to prevent mining strategies that search for 'easy' programs.

For example, let's calculate the cost of avoiding the CFROUND instruction (frequency 1/256). There is a chance <code>Q = (255/256)<sup>256</sup> = 0.368</code> that a generated RandomX program doesn't contain any CFROUND instructions. If we assume that program generation is 'free', we can easily find such program. However, after we execute the program, there is a chance `1-Q` that the next program *has* a CFROUND instruction and we have wasted one program execution.

For 8 chained executions, the chance is only <code>Q<sup>7</sup> = 0.0009</code> that all programs in the chain contain no CFROUND instruction. However, during each attempt to find such chain, we will waste the execution of <code>(Q-1)*(1+2\*Q+3\*Q<sup>2</sup>+4\*Q<sup>3</sup>+5\*Q<sup>4</sup>+6\*Q<sup>5</sup>+7\*Q<sup>6</sup>) = 1.58</code> programs. In the end, we will have to execute, on average, 1734 programs to calculate a single hash! Compared to an honest miner that has to execute just 8 programs per hash, our hashrate is decreased more than 216 times, which is a lot more than the efficiency gain from avoiding 3 rounding modes.

## References

[1] CryptoNote whitepaper - https://cryptonote.org/whitepaper.pdf

[2] ProgPoW: Inefficient integer multiplications - https://github.com/ifdefelse/ProgPOW/issues/16

[3] Branch predictor - https://en.wikipedia.org/wiki/Branch_predictor

[4] Agner Fog: The microarchitecture of Intel, AMD and VIA CPUs - https://www.agner.org/optimize/microarchitecture.pdf page 15

[5] Out of order execution - https://en.wikipedia.org/wiki/Out-of-order_execution

[6] μop cache - https://en.wikipedia.org/wiki/CPU_cache#Micro-operation_(%CE%BCop_or_uop)_cache

[7] Biryukov et al.: Fast and Tradeoff-Resilient Memory-Hard Functions for
Cryptocurrencies and Password Hashing - https://eprint.iacr.org/2015/430.pdf Table 2, page 8

[8] Avalanche effect - https://en.wikipedia.org/wiki/Avalanche_effect

[9] Exponentiation by squaring - https://en.wikipedia.org/wiki/Exponentiation_by_squaring
