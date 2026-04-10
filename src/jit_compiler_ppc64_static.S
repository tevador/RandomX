/*
Copyright (c) 2026, Forest Crossman <cyrozap@gmail.com>

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	* Neither the name of the copyright holder nor the
	  names of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

	.machine power7
	.machine altivec
	.section ".rodata"  // Not .text because it's not meant to be executed in-place.

#include "configuration.h"

#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	#define PPC_BIG_ENDIAN 1
#else
	#define PPC_BIG_ENDIAN 0
#endif

#if (defined(_CALL_ELF) && _CALL_ELF == 2) || (!defined(_CALL_ELF) && !PPC_BIG_ENDIAN)
	#define PPC_ABI_V2 1
#else
	#define PPC_ABI_V2 0
#endif

#if PPC_ABI_V2
	.abiversion 2
	#define C_FUNCTION(name) \
		.global name; \
		name:
#else
	.abiversion 1
	#define C_FUNCTION(name) \
		.section ".opd","aw"; \
		.align 3; \
		.global name; \
		name: \
		.quad .name, .TOC.@tocbase, 0; \
		.previous; \
		.global .name; \
		.name:
#endif

	.global randomx_ppc64_constants
	.global randomx_ppc64_constant_lut_fprc_to_fpscr
	.global randomx_ppc64_constants_end

	.global randomx_ppc64_dataset_init
	.global randomx_ppc64_dataset_init_fix_call
	.global randomx_ppc64_dataset_init_end

	.global randomx_ppc64_sshash_single_item_prologue
	.global randomx_ppc64_sshash_single_item_prologue_end
	.global randomx_ppc64_sshash_single_item_epilogue
	.global randomx_ppc64_sshash_single_item_epilogue_end
	.global randomx_ppc64_sshash_cache_prefetch
	.global randomx_ppc64_sshash_cache_prefetch_end
	.global randomx_ppc64_sshash_xor
	.global randomx_ppc64_sshash_xor_end

	.global randomx_ppc64_vm_prologue
	.global randomx_ppc64_vm_prologue_end
	.global randomx_ppc64_vm_epilogue
	.global randomx_ppc64_vm_fix_loop
	.global randomx_ppc64_vm_epilogue_end
	.global randomx_ppc64_vm_loop_prologue
	.global randomx_ppc64_vm_loop_prologue_end
	.global randomx_ppc64_vm_data_read
	.global randomx_ppc64_vm_data_read_end
	.global randomx_ppc64_vm_data_read_light
	.global randomx_ppc64_vm_data_read_light_fix_call
	.global randomx_ppc64_vm_data_read_light_end
	.global randomx_ppc64_vm_spad_store_prologue
	.global randomx_ppc64_vm_spad_store_prologue_end
	.global randomx_ppc64_vm_spad_store_mix_v1
	.global randomx_ppc64_vm_spad_store_mix_v1_end
	.global randomx_ppc64_vm_spad_store_epilogue
	.global randomx_ppc64_vm_spad_store_epilogue_end
	.global randomx_ppc64_vm_spad_store_mix_v2_hard_aes
	.global randomx_ppc64_vm_spad_store_mix_v2_hard_aes_end

// Macro to store a VR containing a RandomX Group F/E/A register to memory
.macro STORE_LE_VR vr_src, vr_temp, offset_reg, base_reg
#if PPC_BIG_ENDIAN
	vperm \vr_temp, \vr_src, \vr_src, %v15  // Reverse the bytes so they're arranged as [ 0123 4567 ]
	stvx \vr_temp, \offset_reg, \base_reg   // Store the two doubles to memory
#else
	stvx \vr_src, \offset_reg, \base_reg    // Store the two doubles to memory
#endif
.endm

// Macro to shuffle a VR after being loaded with lxsdx.
.macro SHUFFLE_VR vr_reg
#if PPC_BIG_ENDIAN
	vperm \vr_reg, \vr_reg, \vr_reg, %v16
#else
	xxmrghw \vr_reg + 32, \vr_reg + 32, \vr_reg + 32
#endif
.endm

// Macro to load a GPR from little-endian bytes in memory
// Clobbers (BE only): r0
.macro LOAD_LE_GPR reg, offset, base_reg
#if PPC_BIG_ENDIAN
	li %r0, \offset
	ldbrx \reg, \base_reg, %r0
#else
	ld \reg, \offset(\base_reg)
#endif
.endm

// Macro to store a GPR to memory as little-endian bytes
// Clobbers (BE only): r0
.macro STORE_LE_GPR reg, offset, base_reg
#if PPC_BIG_ENDIAN
	li %r0, \offset
	stdbrx \reg, \base_reg, %r0
#else
	std \reg, \offset(\base_reg)
#endif
.endm

	// Align constants to 128 bytes (lowest 7 bits masked)
	.align 7

randomx_ppc64_constants:

sshash_constant_0:  .8byte 6364136223846793005
sshash_constant_1:  .8byte 9298411001130361340
sshash_constant_2:  .8byte 12065312585734608966
sshash_constant_3:  .8byte 9306329213124626780
sshash_constant_4:  .8byte 5281919268842080866
sshash_constant_5:  .8byte 10536153434571861004
sshash_constant_6:  .8byte 3398623926847679864
sshash_constant_7:  .8byte 9549104520008361294

randomx_ppc64_constant_lut_fprc_to_fpscr:
	// RandomX fprc to PPC64 FPSCR lookup table
	.8byte 0  /* 00 Round to Nearest */
	.8byte 3  /* 11 Round toward -Infinity */
	.8byte 2  /* 10 Round toward +Infinity */
	.8byte 1  /* 01 Round toward Zero */

	// Align vector constants to 16 bytes (lowest 4 bits masked)
	.align 4

constant_vector_group_e_and_mask:
	.8byte 0x00FFFFFFFFFFFFFF
	.8byte 0x00FFFFFFFFFFFFFF

constant_vector_fscal_xor_mask:
	.8byte 0x80F0000000000000
	.8byte 0x80F0000000000000

constant_vector_be_byte_reverse_mask:
	// Big-endian vector byte reverse mask
	.8byte 0x0F0E0D0C0B0A0908
	.8byte 0x0706050403020100

#if PPC_BIG_ENDIAN
constant_vector_be_permutation_mask:
	// Big-endian vector permutation mask
	.byte 7, 6, 5, 4, 7, 6, 5, 4
	.byte 3, 2, 1, 0, 3, 2, 1, 0
#endif

randomx_ppc64_constants_end:

literal_vector_group_e_or_mask:
	// Program generator will write the vector here


// Register allocations: dataset_init
//
// Passed on entry:
//
// r1 (non-volatile) -> stack pointer (sp)
// r2 (volatile)     -> constants base address (randomx_ppc64_constants), patched in by JIT, restored from stack frame by caller
// r3 (volatile)     -> arg0, pointer to randomx_cache
// r4 (volatile)     -> arg1, pointer to dataset (uint8_t *)
// r5 (volatile)     -> arg2, uint32_t startBlock / itemNumber / initial cacheIndex
// r6 (volatile)     -> arg3, uint32_t endBlock
//
// After prologue:
//
// r0 (volatile)     -> scratch register
// r1 (non-volatile) -> stack pointer (sp)
// r2 (volatile)     -> constants base address (randomx_ppc64_constants), patched in by JIT, restored from stack frame by caller
// r3 (volatile)     -> arg0, pointer to cache memory
// r4 (volatile)     -> arg1, pointer to dataset (uint8_t *)
// r5 (volatile)     -> arg2, uint32_t startBlock / itemNumber / initial cacheIndex
// r6 (volatile)     -> arg3, uint32_t endBlock
// r7-r12 (volatile) -> scratch registers
// r14 (non-volatile) -> saved pointer to dataset (uint8_t *)
// r15 (non-volatile) -> saved itemNumber
// r16-r31 (non-volatile) -> unused

randomx_ppc64_dataset_init:
	// JIT compiler MUST emit immediate load to r2 before this code (ABI v2 only)

	// Standard function prologue
	mflr %r0
	std %r0, 16(%r1)
	stdu %r1, -128(%r1)
	std %r14, 112(%r1)
	std %r15, 120(%r1)

	// Load cache->memory pointer
	ld %r3, 0(%r3)

	// Save the dataset pointer (r4) to r14
	mr %r14, %r4

	// Save the itemNumber (r5) to r15
	mr %r15, %r5

	// Loop setup
	// for (size_t itemNumber = startBlock; itemNumber < endBlock; itemNumber++) { ... }
	sub %r8, %r6, %r5
	mtctr %r8

1:
	// r5 gets clobbered by the item-hashing function, so we need to restore it
	// from r15 before calling the function again.
	mr %r5, %r15

randomx_ppc64_dataset_init_fix_call:
	// JIT compiler MUST patch this to bl to the item hashing function
	b 0

	// Store the 64 computed bytes back in the dataset
	std %r4, 8*0(%r14)
	std %r6, 8*1(%r14)
	std %r7, 8*2(%r14)
	std %r9, 8*3(%r14)
	std %r10, 8*4(%r14)
	std %r11, 8*5(%r14)
	std %r12, 8*6(%r14)
	std %r5, 8*7(%r14)

	// Increment the dataset pointer by 64 bytes
	addi %r14, %r14, 8*8

	// Increment the itemNumber by one
	addi %r15, %r15, 1

	// Loop
	bdnz 1b

	// Standard function epilogue
	ld %r14, 112(%r1)
	ld %r15, 120(%r1)
	addi %r1, %r1, 128
	ld %r0, 16(%r1)
	mtlr %r0
	blr

randomx_ppc64_dataset_init_end:


// Register allocations: sshash_single_item
//
// Passed on entry:
//
// r1 (non-volatile) -> stack pointer (sp)
// r2 (volatile)     -> constants base address (randomx_ppc64_constants), patched in by JIT, restored from stack frame by caller
// r3 (volatile)     -> arg0, pointer to cache memory
// r5 (volatile)     -> arg2, uint32_t itemNumber
//
// After prologue:
//
// r0 (volatile)     -> scratch register
// r1 (non-volatile) -> stack pointer (sp)
// r2 (volatile)     -> constants base address (randomx_ppc64_constants), patched in by JIT, restored from stack frame by caller
// r3 (volatile)     -> arg0, pointer to cache memory
// r4 (volatile)     -> SuperscalarHash r0
// r5 (volatile)     -> cacheIndex, set to SuperscalarHash r7 on return
// r6-r7 (volatile)  -> SuperscalarHash r1-r2
// r8 (volatile)     -> scratch register
// r9-r12 (volatile) -> SuperscalarHash r3-r6
// r14-r21 (non-volatile) -> unused
// r22 (non-volatile) -> SuperscalarHash r7
// r23 (non-volatile) -> cache line address
// r24-r31 (non-volatile) -> unused

randomx_ppc64_sshash_single_item_prologue:
	// Standard function prologue
	mflr %r0
	std %r0, 16(%r1)
	stdu %r1, -128(%r1)
	std %r22, 112(%r1)
	std %r23, 120(%r1)

	// Step 1. Initialize registers

	// r0 = (itemNumber + 1) * 6364136223846793005
	ld %r8, (sshash_constant_0-randomx_ppc64_constants)(%r2)
	addi %r0, %r5, 1
	mulld %r4, %r8, %r0

	// r1 = r0 ^ 9298411001130361340
	ld %r8, (sshash_constant_1-randomx_ppc64_constants)(%r2)
	xor %r6, %r4, %r8

	// r2 = r0 ^ 12065312585734608966
	ld %r8, (sshash_constant_2-randomx_ppc64_constants)(%r2)
	xor %r7, %r4, %r8

	// r3 = r0 ^ 9306329213124626780
	ld %r8, (sshash_constant_3-randomx_ppc64_constants)(%r2)
	xor %r9, %r4, %r8

	// r4 = r0 ^ 5281919268842080866
	ld %r8, (sshash_constant_4-randomx_ppc64_constants)(%r2)
	xor %r10, %r4, %r8

	// r5 = r0 ^ 10536153434571861004
	ld %r8, (sshash_constant_5-randomx_ppc64_constants)(%r2)
	xor %r11, %r4, %r8

	// r6 = r0 ^ 3398623926847679864
	ld %r8, (sshash_constant_6-randomx_ppc64_constants)(%r2)
	xor %r12, %r4, %r8

	// r7 = r0 ^ 9549104520008361294
	ld %r8, (sshash_constant_7-randomx_ppc64_constants)(%r2)
	xor %r22, %r4, %r8

	// Step 2. Use r5 (itemNumber) as cacheIndex so it can be used to generate the initial cache line mask

randomx_ppc64_sshash_single_item_prologue_end:

randomx_ppc64_sshash_single_item_epilogue:
	// Return SuperscalarHash r7 in GPR5
	mr %r5, %r22

	// Standard function epilogue
	ld %r22, 112(%r1)
	ld %r23, 120(%r1)
	addi %r1, %r1, 128
	ld %r0, 16(%r1)
	mtlr %r0
	blr

randomx_ppc64_sshash_single_item_epilogue_end:


// Step 4. Load a 64-byte item from the Cache. The item index is given by cacheIndex modulo the total number of 64-byte items in Cache.
randomx_ppc64_sshash_cache_prefetch:
	// Actual mask MUST be inserted by JIT compiler
	rldic %r8, %r5, 0, 63
	add %r23, %r3, %r8
	dcbt 0, %r23, 0
	// If TH=0b00000, the dcbt/dcbtst instruction provides a
	// hint that the program will probably soon access the
	// block containing the byte addressed by EA.

randomx_ppc64_sshash_cache_prefetch_end:

// Step 6. XOR all registers with data loaded from randomx cache
randomx_ppc64_sshash_xor:
	ld %r8, 0(%r23)
	ld %r0, 8(%r23)
	xor %r4, %r4, %r8
	xor %r6, %r6, %r0
	ld %r8, 16(%r23)
	ld %r0, 24(%r23)
	xor %r7, %r7, %r8
	xor %r9, %r9, %r0
	ld %r8, 32(%r23)
	ld %r0, 40(%r23)
	xor %r10, %r10, %r8
	xor %r11, %r11, %r0
	ld %r8, 48(%r23)
	ld %r0, 56(%r23)
	xor %r12, %r12, %r8
	xor %r22, %r22, %r0

randomx_ppc64_sshash_xor_end:


// Register allocations: vm
//
// Passed on entry:
//
// r1 (non-volatile) -> stack pointer (sp)
// r2 (volatile)     -> constants base address (randomx_ppc64_constants), patched in by JIT, restored from stack frame by caller
// r3 (volatile)     -> arg0, pointer to RegisterFile
// r4 (volatile)     -> arg1, pointer to MemoryRegisters
// r5 (volatile)     -> arg2, pointer to scratchpad (uint8_t *)
// r6 (volatile)     -> arg3, uint64_t loop iterations
//
// After prologue:
//
// r0 (volatile)     -> scratch register
// r1 (non-volatile) -> stack pointer (sp)
// r2 (volatile)     -> constants base address (randomx_ppc64_constants), patched in by JIT, restored from stack frame by caller
// r3 (volatile)     -> arg0 for SuperScalarHash (dataset pointer)
// r4 (volatile)     -> scratch register
// r5 (volatile)     -> arg2 for SuperScalarHash (loop iteration)
// r6-r12 (volatile) -> scratch registers
// r14-r21 (non-volatile) -> RandomX integer registers r0-r7
// r22 (non-volatile) -> dataset pointer (memory)
// r23 (non-volatile) -> unused
// r24 (non-volatile) -> ma
// r25 (non-volatile) -> mx
// r26 (non-volatile) -> spAddr0
// r27 (non-volatile) -> spAddr1
// r28 (non-volatile) -> Saved pointer to RegisterFile
// r29 (non-volatile) -> Saved pointer to MemoryRegisters
// r30 (non-volatile) -> Saved pointer to scratchpad (uint8_t *)
// r31 (non-volatile) -> unused
// f0-f13 / vs0–vs13 (volatile) -> scratch registers
// f14-f31 / vs14–vs31 (non-volatile) -> unused
// v0-v3 / vs32-vs35 (volatile) -> RandomX floating point registers f0-f3
// v4-v7 / vs36-vs39 (volatile) -> RandomX floating point registers e0-e3
// v8-v11 / vs40-vs43 (volatile) -> RandomX floating point registers a0-a3
// v12-v14 / vs44-vs46 (volatile) -> scratch registers
// v15 / vs47 (volatile) -> constant_vector_be_byte_reverse_mask
// v16 / vs48 (volatile) -> constant_vector_be_permutation_mask
// v17 / vs49 (volatile) -> constant_vector_group_e_and_mask
// v18 / vs50 (volatile) -> constant_vector_fscal_xor_mask
// v19 / vs51 (volatile) -> literal_vector_group_e_or_mask
// v20-v31 / vs52-vs63 (non-volatile) -> unused

randomx_ppc64_vm_prologue:
	// JIT compiler MUST emit immediate load to r2 before this code (ABI v2 only)

	// Standard function prologue
	mflr %r0
	std %r0, 16(%r1)
	stdu %r1, -256(%r1)
	std %r14, 112(%r1)
	std %r15, 120(%r1)
	std %r16, 128(%r1)
	std %r17, 136(%r1)
	std %r18, 144(%r1)
	std %r19, 152(%r1)
	std %r20, 160(%r1)
	std %r21, 168(%r1)
	std %r22, 176(%r1)
	std %r23, 184(%r1)
	std %r24, 192(%r1)
	std %r25, 200(%r1)
	std %r26, 208(%r1)
	std %r27, 216(%r1)
	std %r28, 224(%r1)
	std %r29, 232(%r1)
	std %r30, 240(%r1)
	//std %r31, 248(%r1)

	// Save arguments
	mr %r28, %r3
	mr %r29, %r4
	mr %r30, %r5

	// Move the loop iterations into the counter
	mtctr %r6

	// Load the vector constants/literals
	li %r8, constant_vector_group_e_and_mask-randomx_ppc64_constants
	li %r9, constant_vector_fscal_xor_mask-randomx_ppc64_constants
	li %r10, literal_vector_group_e_or_mask-randomx_ppc64_constants
#if PPC_BIG_ENDIAN
	li %r11, constant_vector_be_permutation_mask-randomx_ppc64_constants
#endif
	li %r12, constant_vector_be_byte_reverse_mask-randomx_ppc64_constants
	lxvd2x %vs49, %r8, %r2
	lxvd2x %vs50, %r9, %r2
	lvx %v19, %r10, %r2  // Use lvx to load the vector since it's written [ low word, high word ] in memory
#if PPC_BIG_ENDIAN
	lxvd2x %vs48, %r11, %r2  // Load the BE permutation mask (not needed for LE)
#endif
	lxvd2x %vs47, %r12, %r2
#if PPC_BIG_ENDIAN
	vperm %v19, %v19, %v19, %v15  // Swap the byte order of the Group E OR mask vector
#endif

	// Zero the RandomX integer registers
	li %r14, 0
	li %r15, 0
	li %r16, 0
	li %r17, 0
	li %r18, 0
	li %r19, 0
	li %r20, 0
	li %r21, 0

	// Load MemoryRegisters (r29)
	lwz %r25, 0(%r29)  // mx
	lwz %r24, 4(%r29)  // ma
	ld %r22, 8(%r29)   // memory (dataset pointer)

	// Load a0-a3 from RegisterFile
	.equ registers_a_base, 8*8+16*4+16*4
	addi %r8, %r28, registers_a_base + 16*0
	addi %r9, %r28, registers_a_base + 16*1
	addi %r10, %r28, registers_a_base + 16*2
	addi %r11, %r28, registers_a_base + 16*3
	lvx %v8, 0, %r8
	lvx %v9, 0, %r9
	lvx %v10, 0, %r10
	lvx %v11, 0, %r11
#if PPC_BIG_ENDIAN
	vperm %v8, %v8, %v8, %v15
	vperm %v9, %v9, %v9, %v15
	vperm %v10, %v10, %v10, %v15
	vperm %v11, %v11, %v11, %v15
#endif

	// Instructions to mask mx and ma with Scratchpad L3 mask and set the
	// initial values of spAddr0 and spAddr1 are appended here by the JIT

randomx_ppc64_vm_prologue_end:

randomx_ppc64_vm_epilogue:
	// Loop
	bdz 1f
randomx_ppc64_vm_fix_loop:
	// JIT compiler MUST patch this to b to vm_loop_prologue
	b 0
1:

	// Store RandomX registers back into register file
	STORE_LE_GPR %r14, 8*0, %r28
	STORE_LE_GPR %r15, 8*1, %r28
	STORE_LE_GPR %r16, 8*2, %r28
	STORE_LE_GPR %r17, 8*3, %r28
	STORE_LE_GPR %r18, 8*4, %r28
	STORE_LE_GPR %r19, 8*5, %r28
	STORE_LE_GPR %r20, 8*6, %r28
	STORE_LE_GPR %r21, 8*7, %r28

	.equ registers_f_base, 8*8
	li %r8, registers_f_base + 16*0
	li %r9, registers_f_base + 16*1
	li %r10, registers_f_base + 16*2
	li %r11, registers_f_base + 16*3
	STORE_LE_VR 0, 12, %r8, %r28
	STORE_LE_VR 1, 13, %r9, %r28
	STORE_LE_VR 2, 14, %r10, %r28
	STORE_LE_VR 3, 12, %r11, %r28

	.equ registers_e_base, 8*8+16*4
	li %r8, registers_e_base + 16*0
	li %r9, registers_e_base + 16*1
	li %r10, registers_e_base + 16*2
	li %r11, registers_e_base + 16*3
	STORE_LE_VR 4, 12, %r8, %r28
	STORE_LE_VR 5, 13, %r9, %r28
	STORE_LE_VR 6, 14, %r10, %r28
	STORE_LE_VR 7, 12, %r11, %r28

	// Standard function epilogue
	ld %r14, 112(%r1)
	ld %r15, 120(%r1)
	ld %r16, 128(%r1)
	ld %r17, 136(%r1)
	ld %r18, 144(%r1)
	ld %r19, 152(%r1)
	ld %r20, 160(%r1)
	ld %r21, 168(%r1)
	ld %r22, 176(%r1)
	ld %r23, 184(%r1)
	ld %r24, 192(%r1)
	ld %r25, 200(%r1)
	ld %r26, 208(%r1)
	ld %r27, 216(%r1)
	ld %r28, 224(%r1)
	ld %r29, 232(%r1)
	ld %r30, 240(%r1)
	//ld %r31, 248(%r1)
	addi %r1, %r1,256
	ld %r0, 16(%r1)
	mtlr %r0
	blr

randomx_ppc64_vm_epilogue_end:

randomx_ppc64_vm_loop_prologue:
	// Main loop start

	// Load scratchpad data, mix registers, etc.
	LOAD_LE_GPR %r8, 0, %r26
	LOAD_LE_GPR %r9, 8, %r26
	xor %r14, %r14, %r8
	xor %r15, %r15, %r9
	LOAD_LE_GPR %r8, 16, %r26
	LOAD_LE_GPR %r9, 24, %r26
	xor %r16, %r16, %r8
	xor %r17, %r17, %r9
	LOAD_LE_GPR %r8, 32, %r26
	LOAD_LE_GPR %r9, 40, %r26
	xor %r18, %r18, %r8
	xor %r19, %r19, %r9
	LOAD_LE_GPR %r8, 48, %r26
	LOAD_LE_GPR %r9, 56, %r26
	xor %r20, %r20, %r8
	xor %r21, %r21, %r9

	// Load F registers (v0-v3 / vs32-vs35) from spAddr1 (r27)
	//addi %r8, %r27, 8*0
	addi %r9, %r27, 8*1
	addi %r10, %r27, 8*2
	addi %r11, %r27, 8*3
	lxsdx %vs32, 0, %r27  // Use base address directly to avoid an `addi`
	lxsdx %vs33, 0, %r9
	lxsdx %vs34, 0, %r10
	lxsdx %vs35, 0, %r11
	SHUFFLE_VR 0
	SHUFFLE_VR 1
	SHUFFLE_VR 2
	SHUFFLE_VR 3
	xvcvsxwdp %vs32, %vs32
	xvcvsxwdp %vs33, %vs33
	xvcvsxwdp %vs34, %vs34
	xvcvsxwdp %vs35, %vs35

	// Load E registers (v4-v7 / vs36-vs39) from spAddr1 (r27) and fixup
	addi %r8, %r27, 8*4
	addi %r9, %r27, 8*5
	addi %r10, %r27, 8*6
	addi %r11, %r27, 8*7
	lxsdx %vs36, 0, %r8
	lxsdx %vs37, 0, %r9
	lxsdx %vs38, 0, %r10
	lxsdx %vs39, 0, %r11
	SHUFFLE_VR 4
	SHUFFLE_VR 5
	SHUFFLE_VR 6
	SHUFFLE_VR 7
	xvcvsxwdp %vs36, %vs36
	xvcvsxwdp %vs37, %vs37
	xvcvsxwdp %vs38, %vs38
	xvcvsxwdp %vs39, %vs39
	xxland %vs36, %vs36, %vs49
	xxland %vs37, %vs37, %vs49
	xxland %vs38, %vs38, %vs49
	xxland %vs39, %vs39, %vs49
	xxlor %vs36, %vs36, %vs51
	xxlor %vs37, %vs37, %vs51
	xxlor %vs38, %vs38, %vs51
	xxlor %vs39, %vs39, %vs51

randomx_ppc64_vm_loop_prologue_end:

randomx_ppc64_vm_data_read:
	// Read dataset logic

	// Calculate prefetch address (JIT compiler MUST patch)
	.long 0  // Placeholder for: rlwinm %r8, %mpReg, 0, mask_begin, mask_end
	add %r8, %r8, %r22  // r22 holds dataset base pointer

	// Prefetch the next block with dcbt. This is extremely important--without this
	// we lose >20% performance in V1 and >16% in V2.
	// Setting TH=0b10000 (dcbtt 0, %r8) didn't make any measurable difference in
	// performance.
	dcbt 0, %r8, 0

	// Calculate read address (JIT compiler MUST patch)
	.long 0  // Placeholder for: rlwinm %r8, %mtReg, 0, mask_begin, mask_end
	add %r8, %r8, %r22

	// Read 64 bytes and XOR with integer registers
	ld %r9, 0(%r8)
	ld %r10, 8(%r8)
	xor %r14, %r14, %r9
	xor %r15, %r15, %r10
	ld %r9, 16(%r8)
	ld %r10, 24(%r8)
	xor %r16, %r16, %r9
	xor %r17, %r17, %r10
	ld %r9, 32(%r8)
	ld %r10, 40(%r8)
	xor %r18, %r18, %r9
	xor %r19, %r19, %r10
	ld %r9, 48(%r8)
	ld %r10, 56(%r8)
	xor %r20, %r20, %r9
	xor %r21, %r21, %r10

	// Swap mx and ma
	mr %r8, %r25
	mr %r25, %r24
	mr %r24, %r8

randomx_ppc64_vm_data_read_end:

randomx_ppc64_vm_data_read_light:
	// Light mode read dataset logic
	// (Similar to data_read but uses sshash_single_item)

	// Copy dataset pointer argument for sshash_single_item
	mr %r3, %r22

randomx_ppc64_vm_data_read_light_fix_call:
	// JIT compiler MUST patch this to bl to sshash_single_item
	b 0

	// XOR the result from sshash_single_item with the VM registers
	xor %r14, %r14, %r4
	xor %r15, %r15, %r6
	xor %r16, %r16, %r7
	xor %r17, %r17, %r9
	xor %r18, %r18, %r10
	xor %r19, %r19, %r11
	xor %r20, %r20, %r12
	xor %r21, %r21, %r5

	// Swap mx and ma
	mr %r8, %r25
	mr %r25, %r24
	mr %r24, %r8

randomx_ppc64_vm_data_read_light_end:

randomx_ppc64_vm_spad_store_prologue:
	// Store to scratchpad at spAddr1
	STORE_LE_GPR %r14, 8*0, %r27
	STORE_LE_GPR %r15, 8*1, %r27
	STORE_LE_GPR %r16, 8*2, %r27
	STORE_LE_GPR %r17, 8*3, %r27
	STORE_LE_GPR %r18, 8*4, %r27
	STORE_LE_GPR %r19, 8*5, %r27
	STORE_LE_GPR %r20, 8*6, %r27
	STORE_LE_GPR %r21, 8*7, %r27

randomx_ppc64_vm_spad_store_prologue_end:

randomx_ppc64_vm_spad_store_mix_v1:
	// Mix F and E registers (f0-f3 are vs32-vs35, e0-e3 are vs36-vs39)
	xxlxor %vs32, %vs32, %vs36
	xxlxor %vs33, %vs33, %vs37
	xxlxor %vs34, %vs34, %vs38
	xxlxor %vs35, %vs35, %vs39

randomx_ppc64_vm_spad_store_mix_v1_end:

randomx_ppc64_vm_spad_store_epilogue:
	// Store F registers to scratchpad at spAddr0
	li %r8, 16*0
	li %r9, 16*1
	li %r10, 16*2
	li %r11, 16*3
	STORE_LE_VR 0, 12, %r8, %r26
	STORE_LE_VR 1, 13, %r9, %r26
	STORE_LE_VR 2, 14, %r10, %r26
	STORE_LE_VR 3, 12, %r11, %r26

randomx_ppc64_vm_spad_store_epilogue_end:

randomx_ppc64_vm_spad_store_mix_v2_hard_aes:
	// Mix F and E registers (f0-f3 are v0-v3, e0-e3 are v4-v7)

	// Byte-reverse f0-f3 and e0-e3
	vperm %v0, %v0, %v0, %v15
	vperm %v1, %v1, %v1, %v15
	vperm %v2, %v2, %v2, %v15
	vperm %v3, %v3, %v3, %v15
	vperm %v4, %v4, %v4, %v15
	vperm %v5, %v5, %v5, %v15
	vperm %v6, %v6, %v6, %v15
	vperm %v7, %v7, %v7, %v15

	// We need a zero vector to bypass vncipher's internal key XOR
	vxor %v12, %v12, %v12

	vcipher %v0, %v0, %v4
	vncipher %v1, %v1, %v12  // Pass 0 as the key
	vcipher %v2, %v2, %v4
	vncipher %v3, %v3, %v12
	vxor %v1, %v1, %v4       // XOR the actual key afterwards
	vxor %v3, %v3, %v4

	vcipher %v0, %v0, %v5
	vncipher %v1, %v1, %v12
	vcipher %v2, %v2, %v5
	vncipher %v3, %v3, %v12
	vxor %v1, %v1, %v5
	vxor %v3, %v3, %v5

	vcipher %v0, %v0, %v6
	vncipher %v1, %v1, %v12
	vcipher %v2, %v2, %v6
	vncipher %v3, %v3, %v12
	vxor %v1, %v1, %v6
	vxor %v3, %v3, %v6

	vcipher %v0, %v0, %v7
	vncipher %v1, %v1, %v12
	vcipher %v2, %v2, %v7
	vncipher %v3, %v3, %v12
	vxor %v1, %v1, %v7
	vxor %v3, %v3, %v7

	// Byte-reverse f0-f3 and e0-e3
	vperm %v0, %v0, %v0, %v15
	vperm %v1, %v1, %v1, %v15
	vperm %v2, %v2, %v2, %v15
	vperm %v3, %v3, %v3, %v15
	vperm %v4, %v4, %v4, %v15
	vperm %v5, %v5, %v5, %v15
	vperm %v6, %v6, %v6, %v15
	vperm %v7, %v7, %v7, %v15

randomx_ppc64_vm_spad_store_mix_v2_hard_aes_end:


	.section ".text"
C_FUNCTION(randomx_reciprocal_fast)
	cntlzd  %r4, %r3       // r4 = 63 - k (count leading zeros)
	li      %r5, 1         // r5 = 1
	subfic  %r4, %r4, 63   // r4 = 63 - (63 - k) = k
	sld     %r4, %r5, %r4  // r4 = 1 << k (this is the upper 64 bits of the dividend)
	divdeu  %r3, %r4, %r3  // r3 = (r4 || 0x0000000000000000) / divisor
	blr
