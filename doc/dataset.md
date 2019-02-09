
## Dataset

The dataset is randomly accessed 16384 times during each hash calculation, which significantly increases memory-hardness of RandomX. The size of the dataset is fixed at 4 GiB and it's divided into 67108864 block of 64 bytes.

In order to allow PoW verification with less than 4 GiB of memory, the dataset is constructed from a 256 MiB cache, which can be used to calculate dataset rows on the fly.

Because the initialization of the dataset is computationally intensive, it is recalculated only every 1024 blocks (~34 hours). The following figure visualizes the construction of the dataset:

![Imgur](https://i.imgur.com/b9WHOwo.png)

### Seed block
The whole dataset is constructed from a 256-bit hash of the last block whose height is divisible by 1024 **and** has at least 64 confirmations.

|block|Seed block|
|------|---------------------------------|
|1-1088|Genesis block|
|1088-2112|1024|
|2113-3136|2048|
|...|...

### Cache construction

The 32-byte seed block hash is expanded into the 256 MiB cache using the "memory fill" function of Argon2d. [Argon2](https://github.com/P-H-C/phc-winner-argon2) is a memory-hard password hashing function, which is highly customizable. The variant with "d" suffix uses a data-dependent memory access pattern and provides the highest resistance against time-memory tradeoffs.

Argon2 is used with the following parameters:

|parameter|value|
|------------|--|
|parallelism|1|
|output size|0|
|memory|262144 (256 MiB)|
|iterations|3|
|version|`0x13`|
|hash type|0 (Argon2d)
|password|seed block hash (32 bytes)
|salt|`4d 6f 6e 65 72 6f 1a 24` (8 bytes)
|secret size|0|
|assoc. data size|0|

The finalizer and output calculation steps of Argon2 are omitted. The output is the filled memory array.

The use of 3 iterations makes time-memory tradeoffs infeasible and thus 256 MiB is the minimum amount of memory required by RandomX.

### Dataset block generation
The full 4 GiB dataset can be generated from the 256 MiB cache. Each 64-byte block is generated independently by XORing 16 pseudorandom Cache blocks selected by the `SquareHash` function.

#### SquareHash
`SquareHash` is a custom hash function with 64-bit input and 64-bit output. It is calculated by repeatedly squaring the input, splitting the 128-bit result in to two 64-bit halves and subtracting the high half from the low half. This is repeated 42 times. It's available as a [portable C implementation](../src/squareHash.h) and [x86-64 assembly version](../src/asm/squareHash.inc).

Properties of `SquareHash`:

* It achieves full [Avalanche effect](https://en.wikipedia.org/wiki/Avalanche_effect).
* Since the whole calculation is a long dependency chain, which uses only multiplication and subtraction, the performance gains by using custom hardware are very limited.
* A single `SquareHash` calculation takes 40-80 ns, which is about the same time as DRAM access latency. Devices using low-latency memory will be bottlenecked by `SquareHash`, while CPUs will finish the hash calculation in about the same time it takes to fetch data from RAM.

The output of 16 chained SquareHash calculations is used to determine Cache blocks that are XORed together to produce a Dataset block:

```c++
void initBlock(const uint8_t* cache, uint8_t* out, uint32_t blockNumber) {
  uint64_t r0, r1, r2, r3, r4, r5, r6, r7;

  r0 = 4ULL * blockNumber;
  r1 = r2 = r3 = r4 = r5 = r6 = r7 = 0;

  constexpr uint32_t mask = (CacheSize - 1) & CacheLineAlignMask;

  for (auto i = 0; i < DatasetIterations; ++i) {
    const uint8_t* mixBlock = cache + (r0 & mask);
    PREFETCHNTA(mixBlock);
    r0 = squareHash(r0);
    r0 ^= load64(mixBlock + 0);
    r1 ^= load64(mixBlock + 8);
    r2 ^= load64(mixBlock + 16);
    r3 ^= load64(mixBlock + 24);
    r4 ^= load64(mixBlock + 32);
    r5 ^= load64(mixBlock + 40);
    r6 ^= load64(mixBlock + 48);
    r7 ^= load64(mixBlock + 56);
  }

  store64(out + 0, r0);
  store64(out + 8, r1);
  store64(out + 16, r2);
  store64(out + 24, r3);
  store64(out + 32, r4);
  store64(out + 40, r5);
  store64(out + 48, r6);
  store64(out + 56, r7);
}
```

*Note: `SquareHash` doesn't calculate squaring modulo 2<sup>64</sup>+1 because the subtraction is performed modulo 2<sup>64</sup>. Squaring modulo 2<sup>64</sup>+1 can be calculated by adding the carry bit in every iteration (i.e. the sequence in x86-64 assembly would have to be: `mul rax; sub rax, rdx; adc rax, 0`), but this would decrease ASIC-resistance of `SquareHash`.*

### Performance
The initial 256-MiB cache construction using Argon2d takes around 1 second using an older laptop with an Intel i5-3230M CPU (Ivy Bridge). Cache generation is strictly serial and cannot be parallelized.

On the same laptop, full Dataset initialization takes around 100 seconds using a single thread (1.5 µs per block).

While the generation of a single block is strictly serial, multiple blocks can be easily generated in parallel, so the Dataset generation time decreases linearly with the number of threads. Using an 8-core AMD Ryzen CPU, the whole dataset can be generated in under 10 seconds.

Moreover, the seed block hash is known up to 64 blocks in advance, so miners can slowly precalculate the whole dataset by generating 524288 dataset blocks per minute (corresponds to about 1% utilization of a single CPU core).

### Light clients
Light clients, who cannot or do not want to generate and keep the whole dataset in memory, can generate just the cache and then generate blocks on the fly during hash calculation. In this case, the hash calculation time will be increased by 16384 times the single block generation time. For the Intel Ivy Bridge laptop, this amounts to around 24.5 milliseconds per hash.