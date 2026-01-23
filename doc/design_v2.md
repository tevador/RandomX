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

## 4. Prefetch two main loop iterations ahead instead of just one

RandomX v1 prefetches data from the dataset one iteration ahead. RandomX v2 increases it to two iterations by redefining the prefetch logic (see the `mp` register in specs.md).

This change complements the program size increase tweak and has the same purpose.

## 4. Performance impact

**TODO: the information below is outdated because the tests were done before the prefetch tweak.**

Tests show that RandomX v2, while being more than 1.5 times "heavier" than RandomX v1, results in only a slight hashrate reduction but massive efficiency improvements (in terms of VM+AES instructions per Joule):

### Ryzen 9 9950X (Zen 5) @ 100W
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|19883.4|100.0%|198.83|88.61e9|886.08e6|100.0%|
RandomX v2|17240.2|86.7%|172.4|117.5e9|1175.03e6|**132.6%**|

### Ryzen 9 9950X (Zen 5) @ 256W (PBO max)
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|26621 h/s|100.0%|104|118.63e9|463.47e6|100.0%|
RandomX v2|24091 h/s|90.5%|94.1|164.2e9|641.36e6|**138.4%**|

### Ryzen 9 7945HX (Zen 4) @ 85W at the wall
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|18712 h/s|100.0%|220.14|83.39e9|981.05e6|100.0%|
RandomX v2|16108 h/s|86.08%|189.51|109.79e9|1291.62e6|**131.66%**|

### Ryzen 9 7945HX (Zen 4) @ 62W (CPU TDP)
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|18712 h/s|100.0%|301.81|83.39e9|1344.98e6|100.0%|
RandomX v2|16108 h/s|86.08%|259.81|109.79e9|1770.77e6|**131.66%**|

### Ryzen 5 8600G (Zen 4) @ 45W
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|5806 h/s|100.0%|129|25.87e9|574.88e6|100.0%|
RandomX v2|5153 h/s|88.75%|114.5|35.12e9|780.4e6|**135.75%**|

### Ryzen 9 5900X (Zen 3) @ 132.25W
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|12029.1 h/s|100.0%|90.95|53.61e9|405.35e6|100.0%|
RandomX v2|10392.85 h/s|86.4%|78.58|70.84e9|535.61e6|**132.14%**|

### Ryzen 7 4700U (Zen 2) @ 25W
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|2539 h/s|100.0%|101.56|11.31e9|452.6e6|100.0%|
RandomX v2|2294 h/s|90.35%|91.76|15.64e9|625.41e6|**138.18%**|

### Ryzen 9 3950X (Zen 2) @ 127-132W
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|15110 h/s (131.27W)|100.0%|115.1|67.34e9|512.94e6|100.0%|
RandomX v2|13236 h/s (127.59W)|87.6%|103.7|90.21e9|706.8e6|**137.8%**|

### Intel Core i7-6820HQ (Skylake) 4C/8T 2.7 GHz 45W
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|1930.11 h/s|100.0%|42.9|8.601e9|191.14e6|100.0%|
RandomX v2|1469.72 h/s|76.15%|32.66|10.017e9|222.61e6|**116.5%**|
