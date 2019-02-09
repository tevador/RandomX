
# RandomX instruction set architecture
RandomX VM is a complex instruction set computer ([CISC](https://en.wikipedia.org/wiki/Complex_instruction_set_computer)). All data are loaded and stored in little-endian byte order. Signed integer numbers are represented using [two's complement](https://en.wikipedia.org/wiki/Two%27s_complement). Floating point numbers are represented using the [IEEE 754 double precision format](https://en.wikipedia.org/wiki/Double-precision_floating-point_format).

## Registers

RandomX has 8 integer registers `r0`-`r7` (group R) and a total of 12 floating point registers split into 3 groups: `a0`-`a3` (group A), `f0`-`f3` (group F) and `e0`-`e3` (group E). Integer registers are 64 bits wide, while floating point registers are 128 bits wide and contain a pair of floating point numbers. The lower and upper half of floating point registers are not separately addressable.

*Table 1: Addressable register groups*

|index|R|A|F|E|F+E|
|--|--|--|--|--|--|
|0|`r0`|`a0`|`f0`|`e0`|`f0`|
|1|`r1`|`a1`|`f1`|`e1`|`f1`|
|2|`r2`|`a2`|`f2`|`e2`|`f2`|
|3|`r3`|`a3`|`f3`|`e3`|`f3`|
|4|`r4`||||`e0`|
|5|`r5`||||`e1`|
|6|`r6`||||`e2`|
|7|`r7`||||`e3`|

Besides the directly addressable registers above, there is a 2-bit `fprc` register for rounding control, which is an implicit destination register of the `CFROUND` instruction, and two architectural 32-bit registers `ma` and `mx`, which are not accessible to any instruction. 

Integer registers `r0`-`r7` can be the source or the destination operands of integer instructions or may be used as address registers for loading the source operand from the memory (scratchpad).

Floating point registers `a0`-`a3` are read-only and may not be written to except at the moment a program is loaded into the VM. They can be the source operand of any floating point instruction. The value of these registers is restricted to the interval `[1, 4294967296)`.

Floating point registers `f0`-`f3` are the *additive* registers, which can be the destination of floating point addition and subtraction instructions. The absolute value of these registers will not exceed `1.0e+12`.

Floating point registers `e0`-`e3` are the *multiplicative* registers, which can be the destination of floating point multiplication, division and square root instructions. Their value is always positive.

## Instruction encoding

Each instruction word is 64 bits long and has the following format:

![Imgur](https://i.imgur.com/FtkWRwe.png)

### opcode
There are 256 opcodes, which are distributed between 32 distinct instructions. Each instruction can be encoded using multiple opcodes (the number of opcodes specifies the frequency of the instruction in a random program).

*Table 2: Instruction groups*

|group|# instructions|# opcodes||
|---------|-----------------|----|-|
|integer |19|137|53.5%|
|floating point |9|94|36.7%|
|other |4|25|9.8%|
||**32**|**256**|**100%**

Full description of all instructions: [isa-ops.md](isa-ops.md).

### dst
Destination register. Only bits 0-1 (register groups A, F, E) or 0-2 (groups R, F+E) are used to encode a register according to Table 1.

### src

The `src` flag encodes a source operand register according to Table 1 (only bits 0-1 or 0-2 are used).

Immediate value `imm32` is used as the source operand in cases when `dst` and `src` encode the same register.

For register-memory instructions, the source operand determines the `address_base` value for calculating the memory address (see below).

### mod

The `mod` flag is encoded as:

*Table 3: mod flag encoding*

|`mod`|description|
|----|--------|
|0-1|`mod.mem` flag|
|2-4|`mod.cond` flag|
|5-7|Reserved|

The `mod.mem` flag determines the address mask when reading from or writing to memory:

*Table 3: memory address mask*

|`mod.mem`|`address_mask`|(scratchpad level)|
|---------|-|---|
|0|262136|(L2)|
|1-3|16376|(L1)|

Table 3 applies to all memory accesses except for cases when the source operand is an immediate value. In that case, `address_mask` is equal to 2097144 (L3). 

The address for reading/writing is calculated by applying bitwise AND operation to `address_base` and `address_mask`.

The `mod.cond` flag is used only by the `COND` instruction to select a condition to be tested.

### imm32
A 32-bit immediate value that can be used as the source operand. The immediate value is sign-extended to 64 bits unless specified otherwise.
