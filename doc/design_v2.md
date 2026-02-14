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

This imbalance is the main reason of the program size increase - Zen5 and newer CPUs need more work to keep themselves busy while they're waiting for data from memory.

## 4. Prefetch two main loop iterations ahead instead of just one

RandomX v1 prefetches data from the dataset one iteration ahead. RandomX v2 increases it to two iterations by redefining the prefetch logic (see the `mp` register in specs.md).

This change complements the program size increase tweak and has the same purpose.

## 4. Performance impact

Tests show that RandomX v2, while being more than 1.5 times "heavier" than RandomX v1, results in only a slight hashrate reduction but massive efficiency improvements (in terms of VM+AES instructions per Joule):

### AMD Ryzen 9 9950X (Zen 5) @ 285W (PBO max)
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|27186.1|100.0%|95.38|121.15e9|425.1e6|100.0%|
RandomX v2|26791.7|98.55%|94.01|182.61e9|640.72e6|**150.72%**|

### AMD Ryzen 9 9950X (Zen 5) @ 100W
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|19912.2|100.0%|199.122|88.74e9|887.38e6|100.0%|
RandomX v2|17346.2|87.11%|173.462|118.23e9|1182.27e6|**133.23%**|

### AMD Ryzen AI 9 HX 370 (Zen 5), DDR5-5600 @ 28W
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|6597.15|100.0%|235.61|29.4e9|1050e6|100.0%|
RandomX v2|7121.69|107.95%|254.35|48.54e9|1733.56e6|**165.1%**|

### Ryzen AI 9 365 (Zen 5) @ 28W
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|6091|100.0%|217.5|27.14e9|969.44e6|100.0%|
RandomX v2|6649|109.2%|237.5|45.32e9|1618.5e6|**166.95%**|

### Ryzen 9 7945HX (Zen 4) @ 62W
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|16126|100.0%|260.1|71.86e9|1159.11e6|100.0%|
RandomX v2|15308|94.9%|246.9|104.33e9|1682.83e6|**145.18%**|

### Ryzen 5 8600G (Zen 4) @ 45W
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|5876.47|100.0%|130.59|26.19e9|581.96e6|100.0%|
RandomX v2|5375.29|91.5%|119.45|36.64e9|814.15e6|**139.9%**|

### Ryzen 9 5950X (Zen 3) @ 122-126W
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|14745.9 @ 126W|100.0%|117.03|65.71e9|521.54e6|100.0%|
RandomX v2|12905.3 @ 122W|87.5%|105.78|87.96e9|720.98e6|**138.2%**|

### Ryzen 9 3950X (Zen 2) @ 131W
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|15049.34|100.0%|114.88|67.07e9|511.96e6|100.0%|
RandomX v2|13868.64|92.15%|105.87|94.53e9|721.57e6|**140.94%**|

### Ryzen 7 3700X (Zen 2) @ 88W
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|8624|100.0%|98|38.43e9|436.73e6|100.0%|
RandomX v2|7361|85.35%|83.65|50.17e9|570.12e6|**130.54%**|

### Ryzen 7 1700X (Zen 1) @ 95W
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|4832.73|100.0%|50.87|21.54e9|226.7e6|100.0%|
RandomX v2|4870.41|100.78%|51.27|33.2e9|349.43e6|**154.13%**|

### Intel Core i9-12900K @ 125W
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|8644.47|100.0%|69.16|38.52e9|308.19e6|100.0%|
RandomX v2|8310.78|96.14%|66.49|56.64e9|453.15e6|**147.04%**|

### Intel Core i7-8650U @ 15W
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|1831.15|100.0%|122.08|8.16e9|544.03e6|100.0%|
RandomX v2|1415|77.27%|94.33|9.64e9|642.95e6|**118.18%**|

### Intel Core i7-6820HQ @ 45W
|Algorithm|Hashrate|Relative speed|Hash/Joule|VM+AES/s|VM+AES/Joule|Relative work/Joule|
|-|-|-|-|-|-|-|
RandomX v1|1968.56|100.0%|43.75|8.77e9|194.95e6|100.0%|
RandomX v2|1488.25|75.6%|33.07|10.14e9|225.41e6|**115.62%**|
