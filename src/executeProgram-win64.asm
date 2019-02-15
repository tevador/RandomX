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

_RANDOMX_EXECUTE_PROGRAM SEGMENT PAGE READ EXECUTE

PUBLIC executeProgram

executeProgram PROC
	; REGISTER ALLOCATION:
	; rax -> temporary
	; rbx -> "ic"
	; rcx -> temporary
	; rdx -> temporary
	; rsi -> scratchpad pointer
	; rdi -> dataset pointer
	; rbp -> "ma", "mx"
	; rsp -> stack pointer
	; r8 	-> "r0"
	; r9 	-> "r1"
	; r10 -> "r2"
	; r11 -> "r3"
	; r12 -> "r4"
	; r13 -> "r5"
	; r14 -> "r6"
	; r15 -> "r7"
	; xmm0 -> "f0"
	; xmm1 -> "f1"
	; xmm2 -> "f2"
	; xmm3 -> "f3"
	; xmm4 -> "e0"
	; xmm5 -> "e1"
	; xmm6 -> "e2"
	; xmm7 -> "e3"
	; xmm8 -> "a0"
	; xmm9 -> "a1"
	; xmm10 -> "a2"
	; xmm11 -> "a3"
	; xmm12 -> temporary
	; xmm13 -> DBL_MIN
	; xmm14 -> absolute value mask
	; xmm15 -> sign mask

	; store callee-saved registers
	push rbx
	push rbp
	push rdi
	push rsi
	push r12
	push r13
	push r14
	push r15
	sub rsp, 80
	movdqu xmmword ptr [rsp+64], xmm6
	movdqu xmmword ptr [rsp+48], xmm7
	movdqu xmmword ptr [rsp+32], xmm8
	movdqu xmmword ptr [rsp+16], xmm9
	movdqu xmmword ptr [rsp+0], xmm10
	sub rsp, 80
	movdqu xmmword ptr [rsp+64], xmm11
	movdqu xmmword ptr [rsp+48], xmm12
	movdqu xmmword ptr [rsp+32], xmm13
	movdqu xmmword ptr [rsp+16], xmm14
	movdqu xmmword ptr [rsp+0], xmm15

	;# function arguments
	push rcx                    ;# RegisterFile& registerFile
	mov rbp, qword ptr [rdx]    ;# "mx", "ma"
	mov rdi, qword ptr [rdx+8]  ;# uint8_t* dataset
	mov rsi, r8                 ;# uint8_t* scratchpad
	mov rbx, r9                 ;# loop counter

	mov rax, rbp
	
	;# zero integer registers
	xor r8, r8
	xor r9, r9
	xor r10, r10
	xor r11, r11
	xor r12, r12
	xor r13, r13
	xor r14, r14
	xor r15, r15
	
	;# load constant registers
	lea rcx, [rcx+120]
	movapd xmm8, xmmword ptr [rcx+72]
	movapd xmm9, xmmword ptr [rcx+88]
	movapd xmm10, xmmword ptr [rcx+104]
	movapd xmm11, xmmword ptr [rcx+120]
	movapd xmm13, xmmword ptr [minDbl]
	movapd xmm14, xmmword ptr [absMask]
	movapd xmm15, xmmword ptr [signMask]

	jmp program_begin

ALIGN 64
minDbl:
	db 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 16, 0
absMask:
	db 255, 255, 255, 255, 255, 255, 255, 127, 255, 255, 255, 255, 255, 255, 255, 127
signMask:
	db 0, 0, 0, 0, 0, 0, 240, 129, 0, 0, 0, 0, 0, 0, 240, 129

ALIGN 64
program_begin:
	xor rax, r8                      ;# read address register 1
	xor rax, r10
	mov rdx, rax
	and eax, 2097088
	lea rcx, [rsi+rax]
	push rcx
	xor r8,  qword ptr [rcx+0]
	xor r9,  qword ptr [rcx+8]
	xor r10, qword ptr [rcx+16]
	xor r11, qword ptr [rcx+24]
	xor r12, qword ptr [rcx+32]
	xor r13, qword ptr [rcx+40]
	xor r14, qword ptr [rcx+48]
	xor r15, qword ptr [rcx+56]
	ror rdx, 32
	and edx, 2097088
	lea rcx, [rsi+rdx]
	push rcx
	cvtdq2pd xmm0, qword ptr [rcx+0]
	cvtdq2pd xmm1, qword ptr [rcx+8]
	cvtdq2pd xmm2, qword ptr [rcx+16]
	cvtdq2pd xmm3, qword ptr [rcx+24]
	cvtdq2pd xmm4, qword ptr [rcx+32]
	cvtdq2pd xmm5, qword ptr [rcx+40]
	cvtdq2pd xmm6, qword ptr [rcx+48]
	cvtdq2pd xmm7, qword ptr [rcx+56]
	andps xmm4, xmm14
	andps xmm5, xmm14
	andps xmm6, xmm14
	andps xmm7, xmm14

	;# 256 instructions
	include program.inc

	mov eax, r12d                      ;# read address register 1
	xor eax, r15d                      ;# read address register 2
	xor rbp, rax                       ;# modify "mx"
	xor eax, eax
	and rbp, -64                       ;# align "mx" to the start of a cache line
	mov edx, ebp                       ;# edx = mx
	prefetchnta byte ptr [rdi+rdx]
	ror rbp, 32                        ;# swap "ma" and "mx"
	mov edx, ebp                       ;# edx = ma
	lea rcx, [rdi+rdx]                 ;# dataset cache line
	xor r8,  qword ptr [rcx+0]
	xor r9,  qword ptr [rcx+8]
	xor r10, qword ptr [rcx+16]
	xor r11, qword ptr [rcx+24]
	xor r12, qword ptr [rcx+32]
	xor r13, qword ptr [rcx+40]
	xor r14, qword ptr [rcx+48]
	xor r15, qword ptr [rcx+56]
	pop rcx
	mov qword ptr [rcx+0], r8
	mov qword ptr [rcx+8], r9
	mov qword ptr [rcx+16], r10
	mov qword ptr [rcx+24], r11
	mov qword ptr [rcx+32], r12
	mov qword ptr [rcx+40], r13
	mov qword ptr [rcx+48], r14
	mov qword ptr [rcx+56], r15
	pop rcx
	mulpd xmm0, xmm4
	mulpd xmm1, xmm5
	mulpd xmm2, xmm6
	mulpd xmm3, xmm7
	movapd xmmword ptr [rcx+0], xmm0
	movapd xmmword ptr [rcx+16], xmm1
	movapd xmmword ptr [rcx+32], xmm2
	movapd xmmword ptr [rcx+48], xmm3
	sub ebx, 1
	jnz program_begin
	
rx_finish:
	; save VM register values
	pop rcx
	mov qword ptr [rcx+0], r8
	mov qword ptr [rcx+8], r9
	mov qword ptr [rcx+16], r10
	mov qword ptr [rcx+24], r11
	mov qword ptr [rcx+32], r12
	mov qword ptr [rcx+40], r13
	mov qword ptr [rcx+48], r14
	mov qword ptr [rcx+56], r15
	movdqa xmmword ptr [rcx+64], xmm0
	movdqa xmmword ptr [rcx+80], xmm1
	movdqa xmmword ptr [rcx+96], xmm2
	movdqa xmmword ptr [rcx+112], xmm3
	lea rcx, [rcx+64]
	movdqa xmmword ptr [rcx+64], xmm4
	movdqa xmmword ptr [rcx+80], xmm5
	movdqa xmmword ptr [rcx+96], xmm6
	movdqa xmmword ptr [rcx+112], xmm7

	; load callee-saved registers
	movdqu xmm15, xmmword ptr [rsp]
	movdqu xmm14, xmmword ptr [rsp+16]
	movdqu xmm13, xmmword ptr [rsp+32]
	movdqu xmm12, xmmword ptr [rsp+48]
	movdqu xmm11, xmmword ptr [rsp+64]
	add rsp, 80
	movdqu xmm10, xmmword ptr [rsp]
	movdqu xmm9, xmmword ptr [rsp+16]
	movdqu xmm8, xmmword ptr [rsp+32]
	movdqu xmm7, xmmword ptr [rsp+48]
	movdqu xmm6, xmmword ptr [rsp+64]
	add rsp, 80
	pop r15
	pop r14
	pop r13
	pop r12
	pop rsi
	pop rdi
	pop rbp
	pop rbx

	; return
	ret
	
TransformAddress MACRO reg32, reg64
;# Transforms the address in the register so that the transformed address
;# lies in a different cache line than the original address (mod 2^N).
;# This is done to prevent a load-store dependency.
;# There are 3 different transformations that can be used: x -> 9*x+C, x -> x+C, x -> x^C
	;lea reg32, [reg64+reg64*8+127]  ;# C = -119 -110 -101 -92 -83 -74 -65 -55 -46 -37 -28 -19 -10 -1 9 18 27 36 45 54 63 73 82 91 100 109 118 127
	db 64
	add reg32, -39                   ;# C = all except -7 to +7
	;xor reg32, -8                   ;# C = all except 0 to 7
ENDM

ALIGN 64
rx_read:
;# IN     eax = random 32-bit address
;# GLOBAL rdi = address of the dataset address
;# GLOBAL rsi = address of the scratchpad
;# GLOBAL rbp = low 32 bits = "mx", high 32 bits = "ma"
;# MODIFY rcx, rdx
	TransformAddress eax, rax       ;# TransformAddress function
	mov rcx, qword ptr [rdi]        ;# load the dataset address
	xor rbp, rax                    ;# modify "mx"
	;# prefetch cacheline "mx"
	and rbp, -64                    ;# align "mx" to the start of a cache line
	mov edx, ebp                    ;# edx = mx
	prefetchnta byte ptr [rcx+rdx]
	;# read cacheline "ma"
	ror rbp, 32                     ;# swap "ma" and "mx"
	mov edx, ebp                    ;# edx = ma
	lea rcx, [rcx+rdx]              ;# dataset cache line
	xor r8,  qword ptr [rcx+0]
	xor r9,  qword ptr [rcx+8]
	xor r10, qword ptr [rcx+16]
	xor r11, qword ptr [rcx+24]
	xor r12, qword ptr [rcx+32]
	xor r13, qword ptr [rcx+40]
	xor r14, qword ptr [rcx+48]
	xor r15, qword ptr [rcx+56]
	ret
executeProgram ENDP

_RANDOMX_EXECUTE_PROGRAM ENDS

ENDIF

END
