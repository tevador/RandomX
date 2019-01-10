
# RandomX instruction listing
There are 31 unique instructions divided into 3 groups:

|group|# operations|# opcodes||
|---------|-----------------|----|-|
|integer (IA)|22|144|56.3%|
|floating point (FP)|5|76|29.7%|
|control (CL)|4|36|14.0%
||**31**|**256**|**100%**


## Integer instructions
There are 22 integer instructions. They are divided into 3 classes (MATH, DIV, SHIFT) with different B operand selection rules.
|# opcodes|instruction|class|signed|A width|B width|C|C width|
|-|-|-|-|-|-|-|-|
|12|ADD_64|MATH|no|64|64|`A + B`|64|
|2|ADD_32|MATH|no|32|32|`A + B`|32|
|12|SUB_64|MATH|no|64|64|`A - B`|64|
|2|SUB_32|MATH|no|32|32|`A - B`|32|
|21|MUL_64|MATH|no|64|64|`A * B`|64|
|10|MULH_64|MATH|no|64|64|`A * B`|64|
|15|MUL_32|MATH|no|32|32|`A * B`|64|
|15|IMUL_32|MATH|yes|32|32|`A * B`|64|
|10|IMULH_64|MATH|yes|64|64|`A * B`|64|
|4|DIV_64|DIV|no|64|32|`A / B`|64|
|4|IDIV_64|DIV|yes|64|32|`A / B`|64|
|4|AND_64|MATH|no|64|64|`A & B`|64|
|2|AND_32|MATH|no|32|32|`A & B`|32|
|4|OR_64|MATH|no|64|64|`A | B`|64|
|2|OR_32|MATH|no|32|32|`A | B`|32|
|4|XOR_64|MATH|no|64|64|`A ^ B`|64|
|2|XOR_32|MATH|no|32|32|`A ^ B`|32|
|3|SHL_64|SHIFT|no|64|6|`A << B`|64|
|3|SHR_64|SHIFT|no|64|6|`A >> B`|64|
|3|SAR_64|SHIFT|yes|64|6|`A >> B`|64|
|6|ROL_64|SHIFT|no|64|6|`A <<< B`|64|
|6|ROR_64|SHIFT|no|64|6|`A >>> B`|64|

#### 32-bit operations
Instructions ADD_32, SUB_32, AND_32, OR_32, XOR_32 only use the low-order 32 bits of the input operands. The result of these operations is 32 bits long and bits 32-63 of C are set to zero.

#### Multiplication
There are 5 different multiplication operations. MUL_64 and MULH_64 both take 64-bit unsigned operands, but MUL_64 produces the low 64 bits of the result and MULH_64 produces the high 64 bits. MUL_32 and IMUL_32 use only the low-order 32 bits of the operands and produce a 64-bit result. The signed variant interprets the arguments as signed integers. IMULH_64 takes two 64-bit signed operands and produces the high-order 64 bits of the result.

#### Division
For the division instructions, the dividend is 64 bits long and the divisor 32 bits long. The IDIV_64 instruction interprets both operands as signed integers. In case of division by zero or signed overflow, the result is equal to the dividend `A`.

75% of division instructions use a runtime-constant divisor and can be optimized using a multiplication and shifts.

#### Shift and rotate
The shift/rotate instructions use just the bottom 6 bits of the `B` operand (`imm8` is used as the immediate value). All treat `A` as unsigned except SAR_64, which performs an arithmetic right shift by copying the sign bit.

## Floating point instructions
There are 5 floating point instructions. All floating point instructions are vector instructions that operate on two packed double precision floating point values.

|# opcodes|instruction|C|
|-|-|-|-|
|20|FPADD|`A + B`|
|20|FPSUB|`A - B`|
|22|FPMUL|`A * B`|
|8|FPDIV|`A / B`|
|6|FPSQRT|`sqrt(abs(A))`|

#### Conversion of operand A
Operand A is loaded from memory as a 64-bit value. All floating point instructions interpret A as two packed 32-bit signed integers and convert them into two packed double precision floating point values.

#### Rounding
FPU instructions conform to the IEEE-754 specification, so they must give correctly rounded results. Initial rounding mode is *roundTiesToEven*. Rounding mode can be changed by the `FPROUND` control instruction. Denormal values must be always flushed to zero.

#### NaN
If an operation produces NaN, the result is converted into positive zero. NaN results may never be written into registers or memory. Only division and multiplication must be checked for NaN results (`0.0 / 0.0` and `0.0 * Infinity` result in NaN).

## Control instructions
There are 4 control instructions.

|# opcodes|instruction|description|condition|
|-|-|-|-|
|2|FPROUND|change floating point rounding mode|-
|11|JUMP|conditional jump|(see condition table below)
|11|CALL|conditional procedure call|(see condition table below)
|12|RET|return from procedure|stack is not empty

All control instructions behave as 'arithmetic no-op' and simply copy the input operand A into the destination C.

The JUMP and CALL instructions use a condition function, which takes the lower 32 bits of operand B (register) and the value `imm32` and evaluates a condition based on the `B.LOC.C` flag: 

|`B.LOC.C`|signed|jump condition|probability|*x86*|*ARM*
|---|---|----------|-----|--|----|
|0|no|`B <= imm32`|0% - 100%|`JBE`|`BLS`
|1|no|`B > imm32`|0% - 100%|`JA`|`BHI`
|2|yes|`B - imm32 < 0`|50%|`JS`|`BMI`
|3|yes|`B - imm32 >= 0`|50%|`JNS`|`BPL`
|4|yes|`B - imm32` overflows|0% - 50%|`JO`|`BVS`
|5|yes|`B - imm32` doesn't overflow|50% - 100%|`JNO`|`BVC`
|6|yes|`B < imm32`|0% - 100%|`JL`|`BLT`
|7|yes|`B >= imm32`|0% - 100%|`JGE`|`BGE`

The 'signed' column specifies if the operands are interpreted as signed or unsigned 32-bit numbers. Column 'probability' lists the expected jump probability (range means that the actual value for a specific instruction depends on `imm32`). *Columns 'x86' and 'ARM' list the corresponding hardware instructions (following a `CMP` instruction).*

### FPROUND
The FPROUND instruction changes the rounding mode for all subsequent FPU operations depending on a two-bit flag. The flag is calculated by rotating A `imm8` bits to the right and taking the two least-significant bits:

```
rounding flag = (A >>> imm8)[1:0]
```

|rounding flag|rounding mode|
|-------|------------|
|00|roundTiesToEven|
|01|roundTowardNegative|
|10|roundTowardPositive|
|11|roundTowardZero|

The rounding modes are defined by the IEEE-754 standard.

*The two-bit flag value exactly corresponds to bits 13-14 of the x86 `MXCSR` register and bits 23 and 22 (reversed) of the ARM `FPSCR` register.*

### JUMP
If the jump condition is `true`, the JUMP instruction performs a forward jump relative to the value of `pc`. The forward offset is equal to `16 * (imm8[6:0] + 1)` bytes (1-128 instructions forward).

### CALL
If the jump condition is `true`, the CALL instruction pushes the value of `pc` (program counter) onto the stack and then performs a forward jump relative to the value of `pc`. The forward offset is equal to `16 * (imm8[6:0] + 1)` bytes (1-128 instructions forward).

### RET
If the stack is not empty, the RET instruction pops the return address from the stack (it's the instruction following the previous CALL) and jumps to it.

## Reference implementation
A portable C++ implementation of all integer and floating point instructions is available in [instructionsPortable.cpp](../src/instructionsPortable.cpp).