

## RandomX instruction set
RandomX uses a simple low-level language (instruction set), which was designed so that any random bitstring forms a valid program.

Each RandomX instruction has a length of 128 bits. The encoding is following:

![Imgur](https://i.imgur.com/mbndESz.png)

*All flags are aligned to an 8-bit boundary for easier decoding.*

#### Opcode
There are 256 opcodes, which are distributed between various operations depending on their weight (how often they will occur in the program on average). The distribution of opcodes is following:

|operation|number of opcodes||
|---------|-----------------|----|
|ALU operations|146|57.0%|
|FPU operations|78|30.5%|
|Control flow |32|12.5%|

#### Operand A
The first operand is read from memory. The location is determined by the `loc(a)` flag:

|loc(a)[2:0]|read A from|address size (W)
|---------|-|-|
|000|dataset|32 bits|
|001|dataset|32 bits|
|010|dataset|32 bits|
|011|dataset|32 bits|
|100|scratchpad|15 bits|
|101|scratchpad|11 bits|
|110|scratchpad|11 bits|
|111|scratchpad|11 bits|

Flag `reg(a)` encodes an integer register `r0`-`r7`.  The read address is calculated as:
```
reg(a) = reg(a) XOR signExtend(addr(a))
read_addr = reg(a)[W-1:0]
```
`W` is the address width from the above table. For reading from the scratchpad, `read_addr` is multiplied by 8 for 8-byte aligned access.

#### Operand B
The second operand is loaded either from a register or from an immediate value encoded within the instruction. The `reg(b)` flag encodes an integer register (ALU operations) or a floating point register (FPU operations).

|loc(b)[2:0]|read B from|
|---------|-|
|000|register `reg(b)`|
|001|register `reg(b)`|
|010|register `reg(b)`|
|011|register `reg(b)`|
|100|register `reg(b)`|
|101|register `reg(b)`|
|110|`imm8` or `imm32`|
|111|`imm8` or `imm32`|

`imm8` is an 8-bit immediate value, which is used for shift and rotate ALU operations.

`imm32` is a 32-bit immediate value which is used for most operations. For operands larger than 32 bits, the value is sign-extended. For FPU instructions, the value is considered a signed 32-bit integer and then converted to a double precision floating point format.

#### Operand C
The third operand is the location where the result is stored.

|loc\(c\)[2:0]|write C to|address size (W)
|---------|-|-|
|000|scratchpad|15 bits|
|001|scratchpad|11 bits|
|010|scratchpad|11 bits|
|011|scratchpad|11 bits|
|100|register `reg(c)`|-|
|101|register `reg(c)`|-|
|110|register `reg(c)`|-|
|111|register `reg(c)`|-|

The `reg(c)` flag encodes an integer register (ALU operations) or a floating point register (FPU operations).  For writing to the scratchpad, an integer register is always used and the write address is calculated as:
```
write_addr = 8 * (addr(c) XOR reg(c)[31:0])[W-1:0]
```
*CPUs are typically designed for a 2:1 load:store ratio, so each VM instruction performs on average 1 memory read and 0.5 write to memory.*

#### imm8
An 8-bit immediate value that is used as the shift/rotate count by some ALU instructions and as the jump offset of the CALL instruction.

#### addr(a)
A 32-bit address mask that is used to calculate the read address for the A operand. It's sign-extended to 64 bits.

#### addr\(c\)
A 32-bit address mask that is used to calculate the write address for the C operand. `addr(c)` is equal to `imm32`.

### ALU instructions

|weight|instruction|signed|A width|B width|C|C width|
|-|-|-|-|-|-|-|
|16|ADD_64|no|64|64|A + B|64|
|4|ADD_32|no|32|32|A + B|32|
|16|SUB_64|no|64|64|A - B|64|
|4|SUB_32|no|32|32|A - B|32|
|15|MUL_64|no|64|64|A * B|64|
|11|MULH_64|no|64|64|A * B|64|
|11|MUL_32|no|32|32|A * B|64|
|11|IMUL_32|yes|32|32|A * B|64|
|11|IMULH_64|yes|64|64|A * B|64|
|1|DIV_64|no|64|32|A / B|32|
|1|IDIV_64|yes|64|32|A / B|32|
|4|AND_64|no|64|64|A & B|64|
|2|AND_32|no|32|32|A & B|32|
|4|OR_64|no|64|64|A &#124; B|64|
|2|OR_32|no|32|32|A &#124; B|32|
|4|XOR_64|no|64|64|A ^ B|64|
|2|XOR_32|no|32|32|A ^ B|32|
|3|SHL_64|no|64|6|A << B|64|
|3|SHR_64|no|64|6|A >> B|64|
|3|SAR_64|yes|64|6|A >> B|64|
|9|ROL_64|no|64|6|A <<< B|64|
|9|ROR_64|no|64|6|A >>> B|64|

##### 32-bit operations
Instructions ADD_32, SUB_32, AND_32, OR_32, XOR_32 only use the low-order 32 bits of the input operands. The result of these operations is 32 bits long and bits 32-63 of C are zero.

##### Multiplication
There are 5 different multiplication operations. MUL_64 and MULH_64 both take 64-bit unsigned operands, but MUL_64 produces the low 64 bits of the result and MULH_64 produces the high 64 bits. MUL_32 and IMUL_32 use only the low-order 32 bits of the operands and produce a 64-bit result. The signed variant interprets the arguments as signed integers. IMULH_64 takes two 64-bit signed operands and produces the high-order 64 bits of the result.

##### Division
For the division instructions, the dividend is 64 bits long and the divisor 32 bits long. The IDIV_64 instruction interprets both operands as signed integers. In case of division by zero or signed overflow, the result is equal to the dividend `A`.

*Division by zero can be handled without branching by a conditional move. Signed overflow happens only for the signed variant when the minimum negative value is divided by -1. This rare case must be handled in x86 (ARM produces the "correct" result).*

##### Shift and rotate
The shift/rotate instructions use just the bottom 6 bits of the `B` operand (`imm8` is used as the immediate value). All treat `A` as unsigned except SAR_64, which performs an arithmetic right shift by copying the sign bit.

### FPU instructions

|weight|instruction|conversion method|C|
|-|-|-|-|
|20|FPADD|`convertSigned52`|A + B|
|20|FPSUB|`convertSigned52`|A - B|
|22|FPMUL|`convertSigned51`|A * B|
|8|FPDIV|`convertSigned51`|A / B|
|6|FPSQRT|`convert52`|sqrt(A)|
|2|FPROUND|`convertSigned52`|A|

#### Rounding
FPU instructions conform to the IEEE-754 specification, so they must give correctly rounded results. Initial rounding mode is *roundTiesToEven*. Rounding mode can be changed by the `FPROUND` instruction. Denormal values are not be produced by any operation.

#### Conversion of operand A
Operand A is loaded from memory as a 64-bit signed integer and then converted to a double-precision floating point format using one of the following 3 methods:

* *convertSigned52* - Clears the 11 least-significant bits before conversion. This is done so the number fits exactly into the 52-bit mantissa without rounding.
* *convertSigned51* - Clears the 11 least-significant bits and sets the 12th bit before conversion. This is done so the number fits exactly into the 52-bit mantissa without rounding and avoids 0.
* *convert52* - Clears the 11 least-significant bits and the sign bit before conversion. This is done so the number fits exactly into the 52-bit mantissa without rounding and avoids negative values.

##### FPROUND
The FPROUND instruction changes the rounding mode for all subsequent FPU operations depending on the two least-significant bits of A.

|A[1:0]|rounding mode|
|-------|------------|
|00|roundTiesToEven|
|01|roundTowardNegative|
|10|roundTowardPositive|
|11|roundTowardZero|

The rounding modes are defined by the IEEE-754 standard.

*The two-bit flag value exactly corresponds to bits 13-14 of the x86 `MXCSR` register and bits 23 and 22 (reversed) of the ARM `FPSCR` register.*

### Control flow instructions
The following 2 control flow instructions are supported:

|weight|instruction|function|
|-|-|-|
|17|CALL|near procedure call|
|15|RET|return from procedure|

Both instructions are conditional in 75% of cases. The jump is taken only if `B <= imm32`. For the 25% of cases when `B` is equal to `imm32`, the jump is unconditional. In case the branch is not taken, both instructions become "arithmetic no-op" `C = A`.

##### CALL
Taken CALL instruction pushes the values `A` and `pc` (program counter) onto the stack and then performs a forward jump relative to the value of `pc`. The forward offset is equal to `16 * (imm8[6:0] + 1)`. Maximum jump distance is therefore 128 instructions forward (this means that at least 4 correctly spaced CALL instructions are needed to form a loop in the program).

##### RET
The RET instruction behaves like "not taken" when the stack is empty. Taken RET instruction pops the return address `raddr` from the stack (it's the instruction following the previous CALL), then pops a return value `retval` from the stack and sets `C = A XOR retval`. Finally, the instruction jumps back to `raddr`.

## Reference implementation
A portable C++ implementation of all ALU and FPU instructions is available in [instructionsPortable.cpp](../src/instructionsPortable.cpp).