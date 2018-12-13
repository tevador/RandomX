; Copyright (c) 2018 tevador
;
; This file is part of RandomX.
;
; RandomX is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; RandomX is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with RandomX.  If not, see<http://www.gnu.org/licenses/>.

PUBLIC executeProgram

.code

executeProgram PROC
	; REGISTER ALLOCATION:
	; rax -> temporary
	; rbx -> "r0"
	; rcx -> temporary
	; rdx -> MemoryRegisters& memory
	; rsi -> convertible_t& scratchpad
	; rdi -> "ic" (instruction counter)
	; rbp	-> beginning of VM stack
	; rsp -> end of VM stack
	; r8 	-> temporary
	; r9 	-> "r1"
	; r10 -> "r2"
	; r11 -> "r3"
	; r12 -> "r4"
	; r13 -> "r5"
	; r14 -> "r6"
	; r15 -> "r7"
	; xmm0 -> temporary
	; xmm1 -> "f1"
	; xmm2 -> "f2"
	; xmm3 -> "f3"
	; xmm4 -> "f4"
	; xmm5 -> "f5"
	; xmm6 -> "f6"
	; xmm7 -> "f7"
	; xmm8 -> "f0"
	; xmm9 -> temporary

	; STACK STRUCTURE:
	;   |
	;   |
	;   | saved registers
	;   |
	;   v
	; [rbp+8] RegisterFile& registerFile
	; [rbp] 	DatasetReadFunc readFunc
	;   |
	;   |
	;   | VM stack
	;   |
	;   v
	; [rsp] last element of VM stack

	; store callee-saved registers
	push rbx
	push rbp
	push rdi
	push rsi
	push r12
	push r13
	push r14
	push r15
	sub rsp, 64
	movdqu xmmword ptr [rsp+48], xmm6
	movdqu xmmword ptr [rsp+32], xmm7
	movdqu xmmword ptr [rsp+16], xmm8
	movdqu xmmword ptr [rsp+0], xmm9

	; function arguments
	push rcx 				; RegisterFile& registerFile
	; mov rdx, rdx	; MemoryRegisters& memory
	push r8 				; DatasetReadFunc readFunc
	mov rsi, r9			; convertible_t& scratchpad

	mov rbp, rsp		; beginning of VM stack
	mov rdi, 1048576	; number of VM instructions to execute

	; load VM register values
	mov rbx, qword ptr [rcx+0]
	mov r9, qword ptr [rcx+8]
	mov r10, qword ptr [rcx+16]
	mov r11, qword ptr [rcx+24]
	mov r12, qword ptr [rcx+32]
	mov r13, qword ptr [rcx+40]
	mov r14, qword ptr [rcx+48]
	mov r15, qword ptr [rcx+56]
	movd xmm8, qword ptr [rcx+64]
	movd xmm1, qword ptr [rcx+72]
	movd xmm2, qword ptr [rcx+80]
	movd xmm3, qword ptr [rcx+88]
	movd xmm4, qword ptr [rcx+96]
	movd xmm5, qword ptr [rcx+104]
	movd xmm6, qword ptr [rcx+112]
	movd xmm7, qword ptr [rcx+120]

	; program body

	include program.inc

rx_finish:
	; unroll the stack
	mov rsp, rbp
	add	rsp, 16

	; save VM register values
	mov rcx, qword ptr [rbp+8]
	mov qword ptr [rcx+0], rbx
	mov qword ptr [rcx+8], r9
	mov qword ptr [rcx+16], r10
	mov qword ptr [rcx+24], r11
	mov qword ptr [rcx+32], r12
	mov qword ptr [rcx+40], r13
	mov qword ptr [rcx+48], r14
	mov qword ptr [rcx+56], r15
	movd qword ptr [rcx+64], xmm8
	movd qword ptr [rcx+72], xmm1
	movd qword ptr [rcx+80], xmm2
	movd qword ptr [rcx+88], xmm3
	movd qword ptr [rcx+96], xmm4
	movd qword ptr [rcx+104], xmm5
	movd qword ptr [rcx+112], xmm6
	movd qword ptr [rcx+120], xmm7

	; load callee-saved registers
	movdqu xmm9, xmmword ptr [rsp+0]
	movdqu xmm8, xmmword ptr [rsp+16]
	movdqu xmm7, xmmword ptr [rsp+32]
	movdqu xmm6, xmmword ptr [rsp+48]
	add rsp, 64
	pop r15
	pop r14
	pop r13
	pop r12
	pop rsi
	pop rdi
	pop rbp
	pop rbx

	; return
	ret	0

rx_read_dataset:
	push rdx
	push r9
	push r10
	push r11
	sub rsp, 32
	call qword ptr [rbp]
	add	rsp, 32
	pop r11
	pop r10
	pop r9
	pop rdx
	ret 0
executeProgram ENDP

END
