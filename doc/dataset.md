
## Dataset

The dataset serves as the source of the first operand of all instructions and provides the memory-hardness of RandomX. The size of the dataset is fixed at 4 GiB and it's divided into 65536 blocks, each 64 KiB in size.

In order to allow PoW verification with less than 4 GiB of memory, the dataset is constructed from a 64 MiB cache, which can be used to calculate dataset blocks on the fly. To facilitate this, all random reads from the dataset are aligned to the beginning of a block.

Because the initialization of the dataset is computationally intensive, it's recalculated on average every 1024 blocks (~34 hours). The following figure visualizes the construction of the dataset:

![Imgur](https://i.imgur.com/JgLCjeq.png)

### Seed block
The whole dataset is constructed from a 256-bit hash of the last block whose height is divisible by 1024 **and** has at least 64 confirmations.

|block|Seed block|
|------|---------------------------------|
|1-1088|Genesis block|
|1088-2112|1024|
|2113-3136|2048|
|...|...

### Cache construction

The 32-byte seed block hash is expanded into the 64 MiB cache using the "memory fill" function of Argon2d. [Argon2](https://github.com/P-H-C/phc-winner-argon2) is a memory-hard password hashing function, which is highly customizable. The variant with "d" suffix uses a data-dependent memory access pattern and provides the highest resistance against time-memory tradeoffs.

Argon2 is used with the following parameters:

|parameter|value|
|------------|--|
|parallelism|1|
|output size|0|
|memory|65536 (64 MiB)|
|iterations|12|
|version|`0x13`|
|hash type|0 (Argon2d)
|password|seed block hash (32 bytes)
|salt|`4d 6f 6e 65 72 6f 1a 24` (8 bytes)
|secret size|0|
|assoc. data size|0|

The finalizer and output calculation steps of Argon2 are omitted. The output is the filled memory array.

The use of 12 iterations makes time-memory tradeoffs infeasible and thus 64 MiB is the minimum amount of memory required by RandomX.

When the memory fill is complete, the whole memory array is cyclically shifted backwards by 512 bytes (i.e. bytes 0-511 are moved to the end of the array). This is done to misalign the array so that each 1024-byte cache block spans two subsequent Argon2 blocks.

### Dataset block generation
The full 4 GiB dataset can be generated from the 64 MiB cache. Each block is generated separately: a 1024 byte block of the cache is expanded into 64 KiB of the dataset. The algorithm has 3 steps: expansion, AES and shuffle.

#### Expansion
The 1024 cache bytes are split into 128 quadwords and interleaved with 504-byte chunks of null bytes. The resulting sequence is: 8 cache bytes + 504 null bytes + 8 cache bytes + 504 null bytes etc. Total length of the expanded block is 65536 bytes.

#### AES
The 256-bit seed block hash is expanded into 10 AES round keys `k0`-`k9`. Let `i = 0...65535` be the index of the block that is being expanded. If `i` is an even number, this step uses AES *decryption* and if `i` is an odd number, it uses AES *encryption*.  Since both encryption and decryption scramble random data, no distinction is made between them in the text below.

The AES encryption is performed with 10 identical rounds using round keys `k0`-`k9`. Note that this is different from the typical AES procedure, which uses a different key schedule for decryption and a modified last round.

Before the AES encryption is applied, each 16-byte chunk is XORed with the ciphertext of the previous chunk. This is similar to the [AES-CBC](https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#Cipher_Block_Chaining_%28CBC%29) mode of operation and forces the encryption to be sequential. For XORing the initial block, an initialization vector is formed by zero-extending `i` to 128 bits.

#### Shuffle
When the AES step is complete, the last 16-byte chunk of the block is used to initialize a PCG32 random number generator. Bits 0-63 are used as the initial state and bits 64-127 are used as the increment. The least-significant bit of the increment is always set to 1 to form an odd number.

The whole block is then divided into 16384 doublewords (4 bytes) and the [Fisher–Yates shuffle](https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle) algorithm is applied to it. The algorithm generates a random in-place permutation of the 16384 doublewords. The result of the shuffle is the `i`-th block of the dataset.

The shuffle algorithm requires a uniform distribution of random numbers. The output of the PCG32 generator is always properly filtered to avoid the [modulo bias](https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle#Modulo_bias).

### Performance
The initial 64-MiB cache construction using Argon2d takes around 1 second using an older laptop with an Intel i5-3230M CPU (Ivy Bridge). Cache generation is strictly serial and cannot be easily parallelized.

Dataset generation performance depends on the support of the AES-NI instruction set. The following table lists the generation runtimes using the same Ivy Bridge laptop with a single thread:

|AES|4 GiB dataset generation|single block generation|
|-----|-----------------------------|----------------|
|hardware (AES-NI)|25 s|380 µs|
|software|53 s|810 µs|

While the generation of a single block is strictly serial, multiple blocks can be easily generated in parallel, so the dataset generation time decreases linearly with the number of threads. Using a recent 6-core CPU with AES-NI support, the whole dataset can be generated in about 4 seconds.

Moreover, the seed block hash is known up to 64 blocks in advance, so miners can slowly precalculate the whole dataset by generating ~512 dataset blocks per minute (corresponds to less than 1% utilization of a single CPU core).

### Light clients
Light clients, who cannot or do not want to generate and keep the whole dataset in memory, can generate just the cache and then generate blocks on the fly as the program is being executed. In this case, the program execution time will be increased by roughly 100 times the single block generation time. For the Intel Ivy Bridge laptop, this amounts to around 40 milliseconds per program.