# RandomX design
To minimize the performance advantage of specialized hardware, a proof of work (PoW) algorithm must achieve *device binding* by targeting specific features of existing general-purpose hardware. This is complicated by the fact that we have to target a large class of devices with different architectures from different manufacturers.


There are two distinct classes of general processing devices: CPUs and GPUs. RandomX targets CPUs for the following reasons:

* CPUs, being less specialized devices, are more prevalent and widely accessible. A CPU-bound algorithm is more egalitarian and allows more participants to join the network. This is one of the goals stated in the original [CryptoNote whitepaper](https://cryptonote.org/whitepaper.pdf). 
* A large common subset of native hardware instructions exists among different CPU architectures. The same cannot be said about GPUs. For example, there is no common integer multiplication instruction for nVidia and AMD GPUs ([reference](https://github.com/ifdefelse/ProgPOW/issues/16)).
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

Modern CPUs include a sophisticated [branch predictor](https://en.wikipedia.org/wiki/Branch_predictor) unit to ensure uninterrupted flow of instructions. If a branch prediction fails, the speculatively executed instructions are thrown away, which results in a certain amount of wasted energy with each misprediction. 

To maximize the amount of useful work per unit of energy, mispredictions must be avoided. The best way to maximize CPU efficiency is therefore to not have any branches at all. Therefore, RandomX programs are branchless.

### CPU Caches

#### L1 and L2 cache

L1 and L2 caches are located close to the CPU execution units and provide the best random access latency of around 3-15 cycles. This latency can be efficiently hidden by [out of order execution](https://en.wikipedia.org/wiki/Out-of-order_execution). RandomX programs access almost exclusively "L1" and "L2" parts of the Scratchpad with a 3:1 access ratio to take advantage of the low latency caches.

#### L3 cache
The L3 cache is much larger and located further from the CPU core. As a result, its access latencies are usually around 30-50 cycles. While this is much faster than reading data from DRAM, it can cause stalls in program execution.

RandomX therefore performs only 2 random accesses into "L3" Scratchpad per program iteration. Register values from a given iteration are written into the same location, which guarantees that the required cache lines have been moved to the faster L1 or L2 caches.

Additionally, integer instructions that read from a fixed address also use the whole "L3" Scratchpad (Table 5.1.4 of the Specification) because repetitive accesses will ensure that the cache line will be placed in the L1 cache of the CPU.

#### μop cache

Modern x86 CPUs decode complex instructions into simpler micro operations (μops). Repeatedly executed instructions are not decoded from memory, but are executed directly from the [μop cache](https://en.wikipedia.org/wiki/CPU_cache#Micro-operation_(%CE%BCop_or_uop)_cache). RandomX programs are relatively short loops (typical program compiles into 600-700 μops on Intel Skylake or AMD Ryzen), so they fit entirely into the μop cache, which allows the CPU to power down the relatively power-hungry x86 instruction decoders. This should help to equalize the power efficiency between x86 and simpler architectures like ARM.

### Registers

RandomX uses 8 integer registers and 12 floating point registers. This is the maximum that can be allocated as physical registers in x86-64, which has the fewest architectural registers among existing 64-bit CPU architectures.

### Floating point operations

RandomX uses double precision floating point operations, which are supported by the majority of CPUs and require more complex hardware than single precision. All operations are performed as vector operations, which is also supported by all major CPU architectures.

RandomX uses five operations that are guaranteed by the IEEE 754 standard to give correctly rounded results: addition, subtraction, multiplication, division and square root. All 4 rounding modes defined by the standard are used.

The domains of floating point operations are separated into "additive" operations, which use register group F and "multiplicative" operations, which use register group E. This is done to prevent addition/subtraction from becoming no-op when a small number is added to a large number. Since the range of the F group registers is limited to around `±1.0E+12`, adding or subtracting a floating point number with absolute value larger than 1 always changes at least 12 mantissa bits.

Because the limited range of group F registers allows more efficient fixed-point implementation (with 85-bit numbers), the FSCAL instruction manipulates the binary representation of the floating point format to make this optimization more difficult.

Group E registers are restricted to positive values, which avoids `NaN` results (such as square root of a negative number or `0 * ∞`). Division uses only memory source operand to avoid being optimized into multiplication by constant reciprocal. The exponent of group E operands is set to -240 to avoid division and multiplication by 0 and to increase the range of numbers that can be obtained. The approximate range of possible group E register values is `6.0E-73` to `infinity`.

To maximize entropy and also to fit into one 64-byte cache line, floating point registers are combined using the XOR operation at the end of each iteration before being stored into the Scratchpad.

### Integer operations

RandomX uses all primitive integer operations that preserve entropy: addition, subtraction, multiplication, XOR and shift.

The IADD_RC and IMUL_9C instructions utilize the address calculation logic of CPUs and can be performed in a single instruction by most CPUs.

Because integer division is not fully pipelined in CPUs and can be made faster in ASICs, the IMUL_RCP instruction requires only one division per program to calculate the reciprocal. This forces an ASIC to include a hardware divider without giving them a performance advantage during program execution.

The ISWAP_R instruction can be performed efficiently by CPUs that utilize register renaming.

The COND instructions use the common condition flags that are supported by most CPU architectures. Conditional execution is used to avoid branches.

### Memory access

RandomX randomly accesses a large buffer of data (Dataset) 16384 times for each hash calculation. Since the Dataset must be stored in DRAM, it provides a natural parallelization limit, because DRAM cannot do more than about 25 million random accesses per second per channel. Each separately addressable DRAM channel allows a throughput of around 1500 H/s.

All Dataset accesses read whole CPU cache line (64 bytes) and are fully prefetched. The time to execute one program iteration described in chapter 4.6.2 of the Specification is about the same as typical DRAM access latency.

#### Cache

The Cache, which is used for light verification and Dataset construction, is 16 times smaller than the Dataset. To keep a constant area * time product, each Dataset item is construction from 16 Cache accesses (16 * 256 MiB = 1 * 4 GiB).

Because 256 MiB is small enough to be included on-chip, RandomX uses a high-latency mixing function (SquareHash) which defeats the benefits of using low-latency memory for mining in tradeoff mode.

Using less than 256 MiB of memory is not possible due to the use of tradeoff-resistant Argon2d with 3 iterations. When using 3 iterations (passes), halving the memory usage increases computational cost 3423 times for the best tradeoff attack ([Reference](https://eprint.iacr.org/2015/430.pdf) in Table 2 on page 8).

### Choice of hashing function
RandomX uses Blake2b as its main cryptographically secure hashing function. Blake2b was specifically designed to be fast in software, especially on modern 64-bit processors, where it's around three times faster than SHA-3 and can run at a speed of around 3 clock cycles per byte of input.

### Custom functions

#### SquareHash

SquareHash was chosen for its relative simplicity (uses only two operations - multiplication and subtraction) and high latency. A single SquareHash calculation takes 40-80 ns on a CPU, which is about the same time as DRAM access latency. ASIC devices using low-latency memory will be bottlenecked by SquareHash when calculating Dataset items, while CPUs will finish the hash calculation in about the same time it takes to fetch data from RAM.

From a cryptographic standpoint, SquareHash achieves full [Avalanche effect](https://en.wikipedia.org/wiki/Avalanche_effect). SquareHash was originally based on [exponentiation by squaring](https://en.wikipedia.org/wiki/Exponentiation_by_squaring). In the [x86 assembly implementation](https://github.com/tevador/RandomX/blob/master/src/asm/squareHash.inc), if `adc rax, 0` is added after each subtraction, SquareHash becomes the following operation:
<code>
(x+9507361525245169745)<sup>4398046511104</sup> mod 2<sup>64</sup>+1
</code>,
where <code>4398046511104 = 2<sup>42</sup></code>. The addition of the carry was removed to improve CPU performance. The constant `9507361525245169745` is added to make SquareHash sensitive to zero.

#### Generator

Generator was designed for fastest possible generation of pseudorandom data. It takes advantage of hardware accelerated AES in modern CPUs. Only one AES round is performed per 16 bytes of output, which results in throughput exceeding 20 GB/s. The Scratchpad can be filled in under 100 μs. The Generator state is initialized from the output of Blake2b.

#### Finalizer

The Finalizer was designed for fastest possible calculation of the Scratchpad fingerprint. It interprets the Scratchpad as a set of AES round keys, so it's equivalent to AES encryption with 32768 rounds. Two extra rounds are performed at the end to ensure avalanche of all Scratchpad bits in each lane. The output of the Finalizer is fed into the Blake2b hashing function to calculate the final proof hash.

### Chaining of VM executions

RandomX chains 8 VM initializations and executions to prevent mining strategies that search for 'easy' programs.