## RandomX vs CryptoNight

The original [CryptoNight algorithm](https://cryptonote.org/cns/cns008.txt) was designed in 2013 as part of the CryptoNote Standard. It was claimed to be inefficient on GPUs, FPGAs and ASICs. The CryptoNight algorithm specifically targeted the following properties of mainstream CPUs:

- 2 MiB of L3 cache per core
- 64-bit multiplication with 128-bit result
- hardware-accelerated AES encryption

However, these properties were not sufficient to prevent the development of specialized mining hardware. ASIC miners up to 300x more efficient than CPUs were developed in late 2017.

There are 3 main reasons why CryptoNight was not as CPU-friendly as the authors had intended:

1) It uses a fixed sequence of operations. An ASIC can hardwire the whole algorithm, while a CPU has to waste resources on decoding instructions.
2) It uses only a handful of primitive operations: XOR, addition, multiplication and AES round. All other CPU circuits are wasted, most notably the whole floating point unit and all the sophisticated control flow circuits that enable out-of-order and speculative execution in modern CPUs.
3) It relies on the relatively slow L3 cache, which has access latencies of around 30-40 cycles. Most CPU cycles are therefore wasted waiting for data to be propagated through the cache hierarchy.

The RandomX algorithm addresses all of the above points:

1) The sequence of operations is completely different for each hash calculation.
2) It uses the majority of primitive operations supported by most CPUs. The list of operations consists of integer math (including division), bitwise operations (AND, OR, XOR, shift and rotation) and floating point math (including division and square root). Additionally, it uses random branch instructions and utilizes the call stack.
3) The L1 cache is used as the primary data source, while the L2 cache is used occasionally. This is close to an optimal CPU workload and enables high instruction throughput.

These design choices ensure that any custom hardware created for RandomX will not be much more efficient than a CPU, since it will have to include basically the same circuits.
The fact that RandomX uses CPU capabilities much better than CryptoNight can be seen when comparing power consumption:

|algorithm|threads|power [W] <sup>Note a</sup>|
|---------|---------|---------|
|CryptoNight|8 <sup>Note b</sup>|41|
|CryptoNight v2 <sup>Note c</sup>|8 <sup>Note b</sup>|48|
|Prime95 <sup>Note d</sup>|16|82|
|RandomX|16|92|

*CPU: AMD Ryzen 7 1700 (8 cores, 3350 MHz, 1.05 Vcore).*

*a. System power consumption measured using a wall socket wattmeter (±1W). Difference over idle power is listed.*  
*b. CryptoNight is limited to 8 threads due to L3 cache size.*  
*c. [CryptoNight v2](https://github.com/monero-project/monero/pull/4218) is a modified version of CryptoNight currently used by Monero.*  
*d. [Prime95](https://en.wikipedia.org/wiki/Prime95#Use_for_stress_testing) is a popular CPU stress utility. Small-FFT torture test was used.*

