# RandomX instruction encoding
The instruction set was designed in such way that any random 16-byte word is a valid instruction and any sequence of valid instructions is a valid program. There are no syntax rules.

The encoding of each 128-bit instruction word is following:

![Imgur](https://i.imgur.com/xi8zuAZ.png)

## opcode
There are 256 opcodes, which are distributed between 3 groups of instructions. There are 31 distinct operations (each operation can be encoded using multiple opcodes - for example opcodes `0x00` to `0x0d` correspond to integer addition).

**Table 1: Instruction groups**
|group|# operations|# opcodes||
|---------|-----------------|----|-|
|integer (IA)|22|144|56.3%|
|floating point (FP)|5|76|29.7%|
|control (CL)|4|36|14.0%
||**31**|**256**|**100%**

Full description of all instructions: [isa-ops.md](isa-ops.md).

## A.LOC
**Table 2: `A.LOC` encoding**

|bits|description|
|----|--------|
|0-1|`A.LOC.W` flag|
|2-5|Reserved|
|6-7|`A.LOC.X` flag|

The `A.LOC.W` flag determines the address width when reading operand A from the scratchpad:

**Table 3: Operand A read address width**

|`A.LOC.W`|address width (W)
|---------|-|-|
|0|15 bits (256 KiB)|
|1-3|11 bits (16 KiB)|

If the `A.LOC.W` flag is zero, the address space covers the whole 256 KiB scratchpad. Otherwise, just the first 16 KiB of the scratchpad are addressed.

If the `A.LOC.X` flag is zero, the instruction mixes the scratchpad read address into the `mx` register using XOR. This mixing happens before the address is truncated to W bits (see pseudocode below).

## A.REG
**Table 4: `A.REG` encoding**

|bits|description|
|----|--------|
|0-2|`A.REG.R` flag|
|3-7|Reserved|

The `A.REG.R` flag encodes "readAddressRegister", which is an integer register  `r0`-`r7` to be used for scratchpad read address generation. Read address is generated as follows (pseudocode):

```python
readAddressRegister = IntegerRegister(A.REG.R)
readAddressRegister = readAddressRegister XOR SignExtend(A.mask32)
readAddress = readAddressRegister[31:0]
# dataset is read if the ic register is divisible by 64
IF ic mod 64 == 0:
  DatasetRead(readAddress)
# optional mixing into the mx register
IF A.LOC.X == 0:
  mx = mx XOR readAddress
# truncate to W bits
W = GetAddressWidth(A.LOC.W)
readAddress = readAddress[W-1:0]
```

Note that the value of the read address register is modified during address generation.

## B.LOC
**Table 5: `B.LOC` encoding**

|bits|description|
|----|--------|
|0-1|`B.LOC.L` flag|
|0-2|`B.LOC.C` flag|
|3-7|Reserved|

The `B.LOC.L` flag determines the B operand. It can be either a register or immediate value.

**Table 6: Operand B**

|`B.LOC.L`|IA/DIV|IA/SHIFT|IA/MATH|FP|CL|
|----|--------|----|------|----|---|
|0|register|register|register|register|register|
|1|`imm32`|register|register|register|register|
|2|`imm32`|`imm8`|register|register|register|
|3|`imm32`|`imm8`|`imm32`|register|register|

Integer instructions are split into 3 classes: integer division (IA/DIV), shift and rotate (IA/SHIFT) and other (IA/MATH). Floating point (FP) and control (CL) instructions always use a register operand.

Register to be used as operand B is encoded in the `B.REG.R` flag (see below).

The `B.LOC.C` flag determines the condition for the JUMP and CALL instructions. The flag partially overlaps with the `B.LOC.L` flag.

## B.REG
**Table 7: `B.REG` encoding**

|bits|description|
|----|--------|
|0-2|`B.REG.R` flag|
|3-7|Reserved|

Register encoded by the `B.REG.R` depends on the instruction group:

**Table 8: Register operands by group**

|group|registers|
|----|--------|
|IA|`r0`-`r7`|
|FP|`f0`-`f7`|
|CL|`r0`-`r7`|

##  C.LOC
**Table 9: `C.LOC` encoding**

|bits|description|
|----|--------|
|0-1|`C.LOC.W` flag|
|2|`C.LOC.R` flag|
|3-6|Reserved|
|7|`C.LOC.H` flag|

The `C.LOC.W` flag determines the address width when writing operand C to the scratchpad:

**Table 10: Operand C write address width**

|`C.LOC.W`|address width (W)
|---------|-|-|
|0|15 bits (256 KiB)|
|1-3|11 bits (16 KiB)|

If the `C.LOC.W` flag is zero, the address space covers the whole 256 KiB scratchpad. Otherwise, just the first 16 KiB of the scratchpad are addressed.

The `C.LOC.R` determines the destination where operand C is written:

**Table 11: Operand C destination**

|`C.LOC.R`|groups IA, CL|group FP
|---------|-|-|
|0|scratchpad|register
|1|register|register + scratchpad

Integer and control instructions (groups IA and CL) write either to the scratchpad or to a register. Floating point instructions always write to a register and can also write to the scratchpad. In that case, flag `C.LOC.H` determines if the low or high half of the register is written:

**Table 12: Floating point register write**

|`C.LOC.H`|write bits|
|---------|----------|
|0|0-63|
|1|64-127|

## C.REG
**Table 13: `C.REG` encoding**

|bits|description|
|----|--------|
|0-2|`C.REG.R` flag|
|3-7|Reserved|

The destination register encoded in the `C.REG.R` flag encodes both the write address register (if writing to the scratchpad) and the destination register (if writing to a register). The destination register depends on the instruction group (see Table 8). Write address is always generated from an integer register:

```python
writeAddressRegister = IntegerRegister(C.REG.R)
writeAddress = writeAddressRegister[31:0] XOR C.mask32
# truncate to W bits
W = GetAddressWidth(C.LOC.W)
writeAddress = writeAddress [W-1:0]
```

## imm8
`imm8` is an 8-bit immediate value that is used as the B operand by IA/SHIFT instructions (see Table 6). Additionally, it's used by some control instructions.

## A.mask32
`A.mask32` is a 32-bit address mask that is used to calculate the read address for the A operand. It's sign-extended to 64 bits before use.

## imm32
`imm32` is a 32-bit immediate value which is used for integer instructions from groups IA/DIV and IA/OTHER (see Table 6). The immediate value is sign-extended for instructions that expect 64-bit operands.

## C.mask32
`C.mask32` is a 32-bit address mask that is used to calculate the write address for the C operand. `C.mask32` is equal to `imm32`.
