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

IFDEF RAX

_RANDOMX_JITX86_STATIC SEGMENT PAGE READ EXECUTE

PUBLIC randomx_program_prologue
PUBLIC randomx_program_loop_begin
PUBLIC randomx_program_loop_load
PUBLIC randomx_program_start
PUBLIC randomx_program_read_dataset
PUBLIC randomx_program_read_dataset_light
PUBLIC randomx_program_read_dataset_sshash_init
PUBLIC randomx_program_read_dataset_sshash_fin
PUBLIC randomx_dataset_init
PUBLIC randomx_program_loop_store
PUBLIC randomx_program_loop_end
PUBLIC randomx_program_epilogue
PUBLIC randomx_sshash_load
PUBLIC randomx_sshash_prefetch
PUBLIC randomx_sshash_end
PUBLIC randomx_sshash_init
PUBLIC randomx_program_end

ALIGN 64
randomx_program_prologue PROC
	include asm/program_prologue_win64.inc
randomx_program_prologue ENDP

ALIGN 64
	include asm/program_xmm_constants.inc

ALIGN 64
randomx_program_loop_begin PROC
	nop
randomx_program_loop_begin ENDP

randomx_program_loop_load PROC
	include asm/program_loop_load.inc
randomx_program_loop_load ENDP

randomx_program_start PROC
	nop
randomx_program_start ENDP

randomx_program_read_dataset PROC
	include asm/program_read_dataset.inc
randomx_program_read_dataset ENDP

randomx_program_read_dataset_light PROC
	include asm/program_read_dataset_light.inc
randomx_program_read_dataset_light ENDP

randomx_program_read_dataset_sshash_init PROC
	include asm/program_read_dataset_sshash_init.inc
randomx_program_read_dataset_sshash_init ENDP

randomx_program_read_dataset_sshash_fin PROC
	include asm/program_read_dataset_sshash_fin.inc
randomx_program_read_dataset_sshash_fin ENDP

randomx_program_loop_store PROC
	include asm/program_loop_store.inc
randomx_program_loop_store ENDP

randomx_program_loop_end PROC
	nop
randomx_program_loop_end ENDP

ALIGN 64
randomx_dataset_init PROC
	push rbx
	push rbp
	push rdi
	push rsi
	push r12
	push r13
	push r14
	push r15
	mov rdi, qword ptr [rcx] ;# cache->memory
	mov rsi, rdx ;# dataset
	mov rbp, r8  ;# block index
	push r9      ;# max. block index
init_block_loop:
	prefetchw byte ptr [rsi]
	mov rbx, rbp
	db 232 ;# 0xE8 = call
	dd 32768 - distance
	distance equ $ - offset randomx_dataset_init
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
	pop r9
	pop r15
	pop r14
	pop r13
	pop r12
	pop rsi
	pop rdi
	pop rbp
	pop rbx
	ret
randomx_dataset_init ENDP

ALIGN 64
randomx_program_epilogue PROC
	include asm/program_epilogue_win64.inc
randomx_program_epilogue ENDP

ALIGN 64
randomx_sshash_load PROC
	include asm/program_sshash_load.inc
randomx_sshash_load ENDP

randomx_sshash_prefetch PROC
	include asm/program_sshash_prefetch.inc
randomx_sshash_prefetch ENDP

randomx_sshash_end PROC
	nop
randomx_sshash_end ENDP

ALIGN 64
randomx_sshash_init PROC
	lea r8, [rbx+1]
	include asm/program_sshash_prefetch.inc
	imul r8, qword ptr [r0_mul]
	mov r9, qword ptr [r1_add]
	xor r9, r8
	mov r10, qword ptr [r2_add]
	xor r10, r8
	mov r11, qword ptr [r3_add]
	xor r11, r8
	mov r12, qword ptr [r4_add]
	xor r12, r8
	mov r13, qword ptr [r5_add]
	xor r13, r8
	mov r14, qword ptr [r6_add]
	xor r14, r8
	mov r15, qword ptr [r7_add]
	xor r15, r8
	jmp randomx_program_end
randomx_sshash_init ENDP

ALIGN 64
	include asm/program_sshash_constants.inc

ALIGN 64
randomx_program_end PROC
	nop
randomx_program_end ENDP

_RANDOMX_JITX86_STATIC ENDS

ENDIF

END