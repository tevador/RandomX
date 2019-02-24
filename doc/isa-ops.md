# RandomX instruction listing

## Integer instructions
For integer instructions, the destination is always an integer register (register group R). Source operand (if applicable) can be either an integer register or memory value. If `dst` and `src` refer to the same register, most instructions use `imm32` as the source operand instead of the register. This is indicated in the 'src == dst' column.

Memory operands are loaded as 8-byte values from the address indicated by `src`.  This indirect addressing is marked with square brackets: `[src]`.

|frequency|instruction|dst|src|`src == dst ?`|operation|
|-|-|-|-|-|-|
|12/256|IADD_R|R|R|`src = imm32`|`dst = dst + src`|
|7/256|IADD_M|R|mem|`src = imm32`|`dst = dst + [src]`|
|16/256|IADD_RC|R|R|`src = dst`|`dst = dst + src + imm32`|
|12/256|ISUB_R|R|R|`src = imm32`|`dst = dst - src`|
|7/256|ISUB_M|R|mem|`src = imm32`|`dst = dst - [src]`|
|9/256|IMUL_9C|R|-|-|`dst = 9 * dst + imm32`|
|16/256|IMUL_R|R|R|`src = imm32`|`dst = dst * src`|
|4/256|IMUL_M|R|mem|`src = imm32`|`dst = dst * [src]`|
|4/256|IMULH_R|R|R|`src = dst`|`dst = (dst * src) >> 64`|
|1/256|IMULH_M|R|mem|`src = imm32`|`dst = (dst * [src]) >> 64`|
|4/256|ISMULH_R|R|R|`src = dst`|`dst = (dst * src) >> 64` (signed)|
|1/256|ISMULH_M|R|mem|`src = imm32`|`dst = (dst * [src]) >> 64` (signed)|
|8/256|IMUL_RCP|R|-|-|<code>dst = 2<sup>x</sup> / imm32 * dst</code>|
|2/256|INEG_R|R|-|-|`dst = -dst`|
|16/256|IXOR_R|R|R|`src = imm32`|`dst = dst ^ src`|
|4/256|IXOR_M|R|mem|`src = imm32`|`dst = dst ^ [src]`|
|10/256|IROR_R|R|R|`src = imm32`|`dst = dst >>> src`|
|4/256|ISWAP_R|R|R|`src = dst`|`temp = src; src = dst; dst = temp`|

#### IMULH and ISMULH
These instructions output the high 64 bits of the whole 128-bit multiplication result. The result differs for signed and unsigned multiplication (`IMULH` is unsigned, `ISMULH` is signed). The variants with a register source operand do not use `imm32` (they perform a squaring operation if `dst` equals `src`).

#### IMUL_RCP
This instruction multiplies the destination register by a reciprocal of `imm32`. The reciprocal is calculated as <code>rcp = 2<sup>x</sup> / imm32</code> by choosing the largest integer `x` such that <code>rcp < 2<sup>64</sup></code>. If `imm32` equals 0, this instruction is a no-op.

#### ISWAP_R
This instruction swaps the values of two registers. If source and destination refer to the same register, the result is a no-op.

## Floating point instructions
For floating point instructions, the destination can be a group F or group E register. Source operand is either a group A register or a memory value.

Memory operands are loaded as 8-byte values from the address indicated by `src`. The 8 byte value is interpreted as two 32-bit signed integers and implicitly converted to floating point format. The lower and upper memory operands are marked as `[src][0]` and `[src][1]`.

Memory operands for group E registers are loaded as described above, then their sign bit is cleared and their exponent value is set to `0x30F` (corresponds to 2<sup>-240</sup>).

|frequency|instruction|dst|src|operation|
|-|-|-|-|-|
|8/256|FSWAP_R|F+E|-|`(dst0, dst1) = (dst1, dst0)`|
|20/256|FADD_R|F|A|`(dst0, dst1) = (dst0 + src0, dst1 + src1)`|
|5/256|FADD_M|F|mem|`(dst0, dst1) = (dst0 + [src][0], dst1 + [src][1])`|
|20/256|FSUB_R|F|A|`(dst0, dst1) = (dst0 - src0, dst1 - src1)`|
|5/256|FSUB_M|F|mem|`(dst0, dst1) = (dst0 - [src][0], dst1 - [src][1])`|
|6/256|FSCAL_R|F|-|<code>(dst0, dst1) = (-2<sup>x0</sup> * dst0, -2<sup>x1</sup> * dst1)</code>|
|20/256|FMUL_R|E|A|`(dst0, dst1) = (dst0 * src0, dst1 * src1)`|
|4/256|FDIV_M|E|mem|`(dst0, dst1) = (dst0 / [src][0], dst1 / [src][1])`|
|6/256|FSQRT_R|E|-|`(dst0, dst1) = (√dst0, √dst1)`|

#### FSCAL_R
This instruction negates the number and multiplies it by <code>2<sup>x</sup></code>. `x` is calculated by taking the 5 least significant digits of the biased exponent and interpreting them as a binary number using the digit set `{+1, -1}` as opposed to the traditional `{0, 1}`. The possible values of `x` are all odd numbers from -31 to +31.

The mathematical operation described above is equivalent to a bitwise XOR of the binary representation with the value of `0x81F0000000000000`.

#### Denormal and NaN values
Due to restrictions on the values of the floating point registers, no operation results in `NaN` or a denormal number.

#### Rounding
All floating point instructions give correctly rounded results. The rounding mode depends on the value of the `fprc` register:

|`fprc`|rounding mode|
|-------|------------|
|0|roundTiesToEven|
|1|roundTowardNegative|
|2|roundTowardPositive|
|3|roundTowardZero|

The rounding modes are defined by the IEEE 754 standard.

## Other instructions
There are 4 special instructions that have more than one source operand or the destination operand is a memory value.

|frequency|instruction|dst|src|operation|
|-|-|-|-|-|
|7/256|COND_R|R|R|`if(condition(src, imm32)) dst = dst + 1`
|1/256|COND_M|R|mem|`if(condition([src], imm32)) dst = dst + 1`
|1/256|CFROUND|`fprc`|R|`fprc = src >>> imm32`
|16/256|ISTORE|mem|R|`[dst] = src`

#### COND

These instructions conditionally increment the destination register. The condition function depends on the `mod.cond` flag and takes the lower 32 bits of the source operand and the value `imm32`.

|`mod.cond`|signed|`condition`|probability|*x86*|*ARM*
|---|---|----------|-----|--|----|
|0|no|`src <= imm32`|0% - 100%|`JBE`|`BLS`
|1|no|`src > imm32`|0% - 100%|`JA`|`BHI`
|2|yes|`src - imm32 < 0`|50%|`JS`|`BMI`
|3|yes|`src - imm32 >= 0`|50%|`JNS`|`BPL`
|4|yes|`src - imm32` overflows|0% - 50%|`JO`|`BVS`
|5|yes|`src - imm32` doesn't overflow|50% - 100%|`JNO`|`BVC`
|6|yes|`src < imm32`|0% - 100%|`JL`|`BLT`
|7|yes|`src >= imm32`|0% - 100%|`JGE`|`BGE`

The 'signed' column specifies if the operands are interpreted as signed or unsigned 32-bit numbers. Column 'probability' lists the expected probability the condition is true (range means that the actual value for a specific instruction depends on `imm32`). *Columns 'x86' and 'ARM' list the corresponding hardware instructions (following a `CMP` instruction).*

#### CFROUND
This instruction sets the value of the `fprc` register to the 2 least significant bits of the source register rotated right by `imm32`. This changes the rounding mode of all subsequent floating point instructions.

#### ISTORE
The `ISTORE` instruction stores the value of the source integer register to the memory at the address specified by the destination register. The `src` and `dst` register can be the same.
