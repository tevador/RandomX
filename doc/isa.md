
## RandomX instruction set
RandomX uses a simple low-level language (instruction set), which was designed so that any random bitstring forms a valid program.

Each RandomX instruction has a length of 128 bits. The encoding is following:

![Imgur](https://i.imgur.com/mbndESz.png)

*All flags are aligned to an 8-bit boundary for easier decoding.*

#### Opcode
There are 256 opcodes, which are distributed between 30 instructions based on their weight (how often they will occur in the program on average). Instructions are divided into 5 groups:

|group|number of opcodes||comment|
|---------|-----------------|----|------|
|IA|115|44.9%|integer arithmetic operations
|IS|21|8.2%|bitwise shift and rotate
|FA|70|27.4%|floating point arithmetic operations
|FS|8|3.1%|floating point single-input operations
|CF|42|16.4%|control flow instructions (branches)
||**256**|**100%**

#### Operand A
The first 64-bit operand is read from memory. The location is determined by the `loc(a)` flag:

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
The second operand is loaded either from a register or from an immediate value encoded within the instruction. The `reg(b)` flag encodes an integer register (instruction groups IA and IS) or a floating point register (instruction group FA). Instruction group FS doesn't use operand B.

|loc(b)[2:0]|B (IA)|B (IS)|B (FA)|B (FS)
|---------|-|-|-|-|
|000|integer `reg(b)`|integer `reg(b)`|floating point `reg(b)`|-
|001|integer `reg(b)`|integer `reg(b)`|floating point `reg(b)`|-
|010|integer `reg(b)`|integer `reg(b)`|floating point `reg(b)`|-
|011|integer `reg(b)`|integer `reg(b)`|floating point `reg(b)`|-
|100|integer `reg(b)`|`imm8`|floating point `reg(b)`|-
|101|integer `reg(b)`|`imm8`|floating point `reg(b)`|-
|110|`imm32`|`imm8`|floating point `reg(b)`|-
|111|`imm32`|`imm8`|floating point `reg(b)`|-

`imm8` is an 8-bit immediate value, which is used for shift and rotate integer instructions (group IS). Only bits 0-5 are used.

`imm32` is a 32-bit immediate value which is used for integer instructions from group IA.

Floating point instructions don't use immediate values.

#### Operand C
The third operand is the location where the result is stored. It can be a register or a 64-bit scratchpad location, depending on the value of flag `loc(c)`.

|loc\(c\)[2:0]|address size (W)| C (IA, IS)|C (FA, FS)
|---------|-|-|-|-|-|
|000|15 bits|scratchpad|floating point `reg(c)`
|001|11 bits|scratchpad|floating point `reg(c)`
|010|11 bits|scratchpad|floating point `reg(c)`
|011|11 bits|scratchpad|floating point `reg(c)`
|100|15 bits|integer `reg(c)`|floating point `reg(c)`, scratchpad
|101|11 bits|integer `reg(c)`|floating point `reg(c)`, scratchpad
|110|11 bits|integer `reg(c)`|floating point `reg(c)`, scratchpad
|111|11 bits|integer `reg(c)`|floating point `reg(c)`, scratchpad

Integer operations write either to the scratchpad or to a register. Floating point operations always write to a register and can also write to the scratchpad. In that case, bit 3 of the `loc(c)` flag determines if the low or high half of the register is written:

|loc\(c\)[3]|write to scratchpad|
|------------|-----------------------|
|0|floating point `reg(c)[63:0]`
|1|floating point `reg(c)[127:64]`

The FPROUND instruction is an exception and always writes the low half of the register.

For writing to the scratchpad, an integer register is always used to calculate the address:
```
write_addr = 8 * (addr(c) XOR reg(c)[31:0])[W-1:0]
```
*CPUs are typically designed for a 2:1 load:store ratio, so each VM instruction performs on average 1 memory read and 0.5 writes to memory.*

#### imm8
An 8-bit immediate value that is used as the shift/rotate count by group IS instructions and as the jump offset of the CALL instruction.

#### addr(a)
A 32-bit address mask that is used to calculate the read address for the A operand. It's sign-extended to 64 bits.

#### addr\(c\)
A 32-bit address mask that is used to calculate the write address for the C operand. `addr(c)` is equal to `imm32`.

### ALU instructions

|weight|instruction|group|signed|A width|B width|C|C width|
|-|-|-|-|-|-|-|-|
|10|ADD_64|IA|no|64|64|`A + B`|64|
|2|ADD_32|IA|no|32|32|`A + B`|32|
|10|SUB_64|IA|no|64|64|`A - B`|64|
|2|SUB_32|IA|no|32|32|`A - B`|32|
|21|MUL_64|IA|no|64|64|`A * B`|64|
|10|MULH_64|IA|no|64|64|`A * B`|64|
|15|MUL_32|IA|no|32|32|`A * B`|64|
|15|IMUL_32|IA|yes|32|32|`A * B`|64|
|10|IMULH_64|IA|yes|64|64|`A * B`|64|
|1|DIV_64|IA|no|64|32|`A / B`|32|
|1|IDIV_64|IA|yes|64|32|`A / B`|32|
|4|AND_64|IA|no|64|64|`A & B`|64|
|2|AND_32|IA|no|32|32|`A & B`|32|
|4|OR_64|IA|no|64|64|`A | B`|64|
|2|OR_32|IA|no|32|32|`A | B`|32|
|4|XOR_64|IA|no|64|64|`A ^ B`|64|
|2|XOR_32|IA|no|32|32|`A ^ B`|32|
|3|SHL_64|IS|no|64|6|`A << B`|64|
|3|SHR_64|IS|no|64|6|`A >> B`|64|
|3|SAR_64|IS|yes|64|6|`A >> B`|64|
|6|ROL_64|IS|no|64|6|`A <<< B`|64|
|6|ROR_64|IS|no|64|6|`A >>> B`|64|

##### 32-bit operations
Instructions ADD_32, SUB_32, AND_32, OR_32, XOR_32 only use the low-order 32 bits of the input operands. The result of these operations is 32 bits long and bits 32-63 of C are set to zero.

##### Multiplication
There are 5 different multiplication operations. MUL_64 and MULH_64 both take 64-bit unsigned operands, but MUL_64 produces the low 64 bits of the result and MULH_64 produces the high 64 bits. MUL_32 and IMUL_32 use only the low-order 32 bits of the operands and produce a 64-bit result. The signed variant interprets the arguments as signed integers. IMULH_64 takes two 64-bit signed operands and produces the high-order 64 bits of the result.

##### Division
For the division instructions, the dividend is 64 bits long and the divisor 32 bits long. The IDIV_64 instruction interprets both operands as signed integers. In case of division by zero or signed overflow, the result is equal to the dividend `A`.

*Division by zero can be handled without branching by a conditional move. Signed overflow happens only for the signed variant when the minimum negative value is divided by -1. This rare case must be handled in x86 (ARM produces the "correct" result).*

##### Shift and rotate
The shift/rotate instructions use just the bottom 6 bits of the `B` operand (`imm8` is used as the immediate value). All treat `A` as unsigned except SAR_64, which performs an arithmetic right shift by copying the sign bit.

### FPU instructions

|weight|instruction|group|C|
|-|-|-|-|
|20|FPADD|FA|`A + B`|
|20|FPSUB|FA|`A - B`|
|22|FPMUL|FA|`A * B`|
|8|FPDIV|FA|`A / B`|
|6|FPSQRT|FS|`sqrt(abs(A))`|
|2|FPROUND|FS|`convertSigned52(A)`|

All floating point instructions apart FPROUND are vector instructions that operate on two packed double precision floating point values.

#### Conversion of operand A
Operand A is loaded from memory as a 64-bit value. All floating point instructions apart FPROUND interpret A as two packed 32-bit signed integers and convert them into two packed double precision floating point values.

The FPROUND instruction has a scalar output and interprets A as a 64-bit signed integer. The 11 least-significant bits are cleared before conversion to a double precision format. This is done so the number fits exactly into the 52-bit mantissa without rounding. Output of FPROUND is always written into the lower half of the result register and only this lower half may be written into the scratchpad.

#### Rounding
FPU instructions conform to the IEEE-754 specification, so they must give correctly rounded results. Initial rounding mode is *roundTiesToEven*. Rounding mode can be changed by the `FPROUND` instruction. Denormal values must be flushed to zero.

#### NaN
If an operation produces NaN, the result is converted into positive zero. NaN results may never be written into registers or memory. Only division and multiplication must be checked for NaN results (`0.0 / 0.0` and `0.0 * Infinity` result in NaN).

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

### Control instructions
The following 2 control instructions are supported:

|weight|instruction|function|condition|
|-|-|-|-|
|20|CALL|near procedure call|(see condition table below)
|22|RET|return from procedure|stack is not empty

Both instructions are conditional. If the condition evaluates to `false`, CALL and RET behave as "arithmetic no-op" and simply copy operand A into destination C without jumping.

##### CALL
The CALL instruction uses a condition function, which takes the lower 32 bits of integer register `reg(b)` and the value `imm32` and evaluates a condition based on the `loc(b)` flag: 

|loc(b)[2:0]|signed|jump condition|probability|*x86*|*ARM*
|---|---|----------|-----|--|----|
|000|no|`reg(b)[31:0] <= imm32`|0% - 100%|`JBE`|`BLS`
|001|no|`reg(b)[31:0] > imm32`|0% - 100%|`JA`|`BHI`
|010|yes|`reg(b)[31:0] - imm32 < 0`|50%|`JS`|`BMI`
|011|yes|`reg(b)[31:0] - imm32 >= 0`|50%|`JNS`|`BPL`
|100|yes|`reg(b)[31:0] - imm32` overflows|0% - 50%|`JO`|`BVS`
|101|yes|`reg(b)[31:0] - imm32` doesn't overflow|50% - 100%|`JNO`|`BVC`
|110|yes|`reg(b)[31:0] < imm32`|0% - 100%|`JL`|`BLT`
|111|yes|`reg(b)[31:0] >= imm32`|0% - 100%|`JGE`|`BGE`

The 'signed' column specifies if the operands are interpreted as signed or unsigned 32-bit numbers. Column 'probability' lists the expected jump probability (range means that the actual value for a specific instruction depends on `imm32`). *Columns 'x86' and 'ARM' list the corresponding hardware instructions (following a `CMP` instruction).*

Taken CALL instruction pushes the values `A` and `pc` (program counter) onto the stack and then performs a forward jump relative to the value of `pc`. The forward offset is equal to `16 * (imm8[6:0] + 1)`. Maximum jump distance is therefore 128 instructions forward (this means that at least 4 correctly spaced CALL instructions are needed to form a loop in the program).

##### RET
The RET instruction is taken only if the stack is not empty. Taken RET instruction pops the return address `raddr` from the stack (it's the instruction following the previous CALL), then pops a return value `retval` from the stack and sets `C = A XOR retval`. Finally, the instruction jumps back to `raddr`.

## Reference implementation
A portable C++ implementation of all ALU and FPU instructions is available in [instructionsPortable.cpp](../src/instructionsPortable.cpp).