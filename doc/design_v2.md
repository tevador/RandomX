# RandomX v2 changes and their rationale

## 1. CFROUND tweak

In RandomX v1, CFROUND instruction changes the rounding mode on each main loop iteration. Unfortunately, x86 CPUs were not designed for the rounding mode changing that often. As a result, this single instruction costs up to 10% of hashrate on Ryzen CPUs. This is where an ASIC or a specially designed CPU can get an easy advantage.

RandomX v2 reduces the impact massively: CFROUND will now change the rounding mode only every 16th time it executes (on average).

## 2. AES tweak

F and E registers are now mixed together with AES instead of XOR (step 10 in chapter 4.6.2 of the spec).

- AES tweak doubles the amount of AES computations per hash without hurting the hashrate (it uses the gap in RandomX main loop where the CPU was sitting idle, waiting for scratchpad data).
- AES tweak also introduces AES in the main RandomX loop which makes it harder for specialized hardware to get away with just a dedicated circuit for scratchpad intialization - AES must be implemented as a part of RandomX VM and work with RandomX VM's registers.
- AES tweak also improves data entropy (makes it more random) before it's written to the scratchpad.

## 3. Program size increase from 256 to 384

CPUs got much faster since the original RandomX was released. Back in 2019, Ryzen 9 3950X was the fastest desktop CPU for RandomX, and at the time of writing (January 2026) it's Ryzen 9 9950X. In most CPU benchmarks, 9950X is more than 1.5x faster on average - thanks to clock speed increase from < 4 to > 5 GHz, and to IPC improvements.

But in RandomX it's only 20-25% faster, because it's bottlenecked by the RAM latency. While CPU cores got faster over the years, RAM latency stayed basically the same - a tuned DDR4 memory from 2019, and a tuned DDR5 memory from 2026 will both have the same access latency of around 50-55 ns.

This disbalance is the main reason of the program size increase - Zen5 and newer CPUs need more work to keep themselves busy while they're waiting for data from memory.

## 4. Performance impact

Tests show that RandomX v2, while being more than 1.5 times "heavier" than RandomX v1, results in only a slight hashrate reduction but massive efficiency improvements (in terms of VM+AES instructions per Joule):

### Ryzen 9 9950X @ 100W
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|19883.4|100.0%|198.83|88.61e9|886.08e6|100.0%|
RandomX v2|17240.2|86.7%|172.4|117.5e9|1175.03e6|**132.6%**|

### Ryzen 9 9950X @ 256W (PBO max)
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|26621 h/s|100.0%|104|118.63e9|463.47e6|100.0%|
RandomX v2|24091 h/s|90.5%|94.1|164.2e9|641.36e6|**138.4%**|

### Ryzen 5 8600G @ 45W
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|5806 h/s|100.0%|129|25.87e9|574.88e6|100.0%|
RandomX v2|5153 h/s|88.75%|114.5|35.12e9|780.4e6|**135.75%**|
