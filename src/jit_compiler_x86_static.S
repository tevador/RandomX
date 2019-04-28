;# Copyright (c) 2018 tevador
;#
;# This file is part of RandomX.
;#
;# RandomX is free software: you can redistribute it and/or modify
;# it under the terms of the GNU General Public License as published by
;# the Free Software Foundation, either version 3 of the License, or
;# (at your option) any later version.
;#
;# RandomX is distributed in the hope that it will be useful,
;# but WITHOUT ANY WARRANTY; without even the implied warranty of
;# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;# GNU General Public License for more details.
;#
;# You should have received a copy of the GNU General Public License
;# along with RandomX.  If not, see<http://www.gnu.org/licenses/>.

.intel_syntax noprefix
#if defined(__APPLE__)
.text
#else
.section .text
#endif
#if defined(__WIN32__) || defined(__APPLE__)
#define DECL(x) _##x
#else
#define DECL(x) x
#endif
.global DECL(randomx_program_prologue)
.global DECL(randomx_program_loop_begin)
.global DECL(randomx_program_loop_load)
.global DECL(randomx_program_start)
.global DECL(randomx_program_read_dataset)
.global DECL(randomx_program_read_dataset_light)
.global DECL(randomx_program_read_dataset_sshash_init)
.global DECL(randomx_program_read_dataset_sshash_fin)
.global DECL(randomx_program_loop_store)
.global DECL(randomx_program_loop_end)
.global DECL(randomx_dataset_init)
.global DECL(randomx_program_epilogue)
.global DECL(randomx_sshash_load)
.global DECL(randomx_sshash_prefetch)
.global DECL(randomx_sshash_end)
.global DECL(randomx_sshash_init)
.global DECL(randomx_program_end)

#define db .byte

.balign 64
DECL(randomx_program_prologue):
	#include "asm/program_prologue_linux.inc"

.balign 64
	#include "asm/program_xmm_constants.inc"

.balign 64
DECL(randomx_program_loop_begin):
	nop

DECL(randomx_program_loop_load):
	#include "asm/program_loop_load.inc"

DECL(randomx_program_start):
	nop

DECL(randomx_program_read_dataset):
	#include "asm/program_read_dataset.inc"

DECL(randomx_program_read_dataset_light):
	#include "asm/program_read_dataset_light.inc"

DECL(randomx_program_read_dataset_sshash_init):
	#include "asm/program_read_dataset_sshash_init.inc"

DECL(randomx_program_read_dataset_sshash_fin):
	#include "asm/program_read_dataset_sshash_fin.inc"

DECL(randomx_program_loop_store):
	#include "asm/program_loop_store.inc"

DECL(randomx_program_loop_end):
	nop

.balign 64
DECL(randomx_dataset_init):
	push rbx
	push rbp
	push r12
	push r13
	push r14
	push r15
	mov rdi, qword ptr [rdi] ;# cache->memory
	;# dataset in rsi
	mov rbp, rdx  ;# block index
	push rcx      ;# max. block index
init_block_loop:
	prefetchw byte ptr [rsi]
	mov rbx, rbp
	.byte 232 ;# 0xE8 = call
	;# .set CALL_LOC, 
	.int 32768 - (call_offset - DECL(randomx_dataset_init))
call_offset:
	mov qword ptr [rsi+0], r8
	mov qword ptr [rsi+8], r9
	mov qword ptr [rsi+16], r10
	mov qword ptr [rsi+24], r11
	mov qword ptr [rsi+32], r12
	mov qword ptr [rsi+40], r13
	mov qword ptr [rsi+48], r14
	mov qword ptr [rsi+56], r15
	add rbp, 1
	add rsi, 64
	cmp rbp, qword ptr [rsp]
	jb init_block_loop
	pop rcx
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbp
	pop rbx
	ret

.balign 64
DECL(randomx_program_epilogue):
	#include "asm/program_epilogue_linux.inc"

.balign 64
DECL(randomx_sshash_load):
	#include "asm/program_sshash_load.inc"

DECL(randomx_sshash_prefetch):
	#include "asm/program_sshash_prefetch.inc"

DECL(randomx_sshash_end):
	nop

.balign 64
DECL(randomx_sshash_init):
	lea r8, [rbx+1]
	#include "asm/program_sshash_prefetch.inc"
	imul r8, qword ptr r0_mul[rip]
	mov r9, qword ptr r1_add[rip]
	xor r9, r8
	mov r10, qword ptr r2_add[rip]
	xor r10, r8
	mov r11, qword ptr r3_add[rip]
	xor r11, r8
	mov r12, qword ptr r4_add[rip]
	xor r12, r8
	mov r13, qword ptr r5_add[rip]
	xor r13, r8
	mov r14, qword ptr r6_add[rip]
	xor r14, r8
	mov r15, qword ptr r7_add[rip]
	xor r15, r8
	jmp DECL(randomx_program_end)

.balign 64
	#include "asm/program_sshash_constants.inc"
	
.balign 64
DECL(randomx_program_end):
	nop
