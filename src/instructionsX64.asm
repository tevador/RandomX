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

PUBLIC ADD_64
PUBLIC ADD_32
PUBLIC SUB_64
PUBLIC SUB_32
PUBLIC MUL_64
PUBLIC MULH_64
PUBLIC MUL_32
PUBLIC IMUL_32
PUBLIC IMULH_64
PUBLIC DIV_64
PUBLIC IDIV_64
PUBLIC AND_64
PUBLIC AND_32
PUBLIC OR_64
PUBLIC OR_32
PUBLIC XOR_64
PUBLIC XOR_32
PUBLIC SHL_64
PUBLIC SHR_64
PUBLIC SAR_64
PUBLIC ROL_64
PUBLIC ROR_64
PUBLIC FPINIT
PUBLIC FPADD
PUBLIC FPSUB
PUBLIC FPMUL
PUBLIC FPDIV
PUBLIC FPSQRT
PUBLIC FPROUND

.code

ADD_64 PROC
	mov	rax, QWORD PTR [rcx]
	add	rax, QWORD PTR [rdx]
	mov	QWORD PTR [r8], rax
	ret	0
ADD_64 ENDP

ADD_32 PROC
	mov	eax, DWORD PTR [rcx]
	add	eax, DWORD PTR [rdx]
	mov	QWORD PTR [r8], rax
	ret	0
ADD_32 ENDP

SUB_64 PROC
	mov	rax, QWORD PTR [rcx]
	sub	rax, QWORD PTR [rdx]
	mov	QWORD PTR [r8], rax
	ret	0
SUB_64 ENDP

SUB_32 PROC
	mov	eax, DWORD PTR [rcx]
	sub	eax, DWORD PTR [rdx]
	mov	QWORD PTR [r8], rax
	ret	0
SUB_32 ENDP

MUL_64 PROC
	mov	rax, QWORD PTR [rcx]
	imul	rax, QWORD PTR [rdx]
	mov	QWORD PTR [r8], rax
	ret	0
MUL_64 ENDP

MULH_64 PROC
	mov	rax, QWORD PTR [rdx]
	mul	QWORD PTR [rcx]
	mov	QWORD PTR [r8], rdx
	ret	0
MULH_64 ENDP

MUL_32 PROC
	mov	r9d, DWORD PTR [rcx]
	mov	eax, DWORD PTR [rdx]
	imul	r9, rax
	mov	QWORD PTR [r8], r9
	ret	0
MUL_32 ENDP

IMUL_32 PROC
	movsxd	r9, DWORD PTR [rcx]
	movsxd	rax, DWORD PTR [rdx]
	imul	r9, rax
	mov	QWORD PTR [r8], r9
	ret	0
IMUL_32 ENDP

IMULH_64 PROC
	mov	rax, QWORD PTR [rdx]
	imul	QWORD PTR [rcx]
	mov	QWORD PTR [r8], rdx
	ret	0
IMULH_64 ENDP

DIV_64 PROC
	mov	r9d, DWORD PTR [rdx]
	mov	eax, 1
	test	r9d, r9d
	cmovne	eax, r9d
	xor	edx, edx
	mov	r9d, eax
	mov	rax, QWORD PTR [rcx]
	div	r9
	mov	QWORD PTR [r8], rax
	ret 0
DIV_64 ENDP

IDIV_64 PROC
	mov	rax, QWORD PTR [rcx]
	cmp	DWORD PTR [rdx], -1
	jne	SHORT SAFE_IDIV_64
;	mov	rcx, -9223372036854775808
;	cmp	rax, rcx
	mov rcx, rax
	rol	rcx, 1
	dec	rcx
	jnz	SHORT SAFE_IDIV_64
	mov	QWORD PTR [r8], rax
	ret	0
SAFE_IDIV_64:
	mov	ecx, DWORD PTR [rdx]
	test	ecx, ecx
	mov	edx, 1
	cmovne	edx, ecx
	movsxd	rcx, edx
	cqo
	idiv	rcx
	mov	QWORD PTR [r8], rax
	ret 0
IDIV_64 ENDP

AND_64 PROC
	mov	rax, QWORD PTR [rcx]
	and	rax, QWORD PTR [rdx]
	mov	QWORD PTR [r8], rax
	ret	0
AND_64 ENDP

AND_32 PROC
	mov	eax, DWORD PTR [rcx]
	and	eax, DWORD PTR [rdx]
	mov	QWORD PTR [r8], rax
	ret	0
AND_32 ENDP

OR_64 PROC
	mov	rax, QWORD PTR [rcx]
	or	rax, QWORD PTR [rdx]
	mov	QWORD PTR [r8], rax
	ret	0
OR_64 ENDP

OR_32 PROC
	mov	eax, DWORD PTR [rcx]
	or	eax, DWORD PTR [rdx]
	mov	QWORD PTR [r8], rax
	ret	0
OR_32 ENDP

XOR_64 PROC
	mov	rax, QWORD PTR [rcx]
	xor	rax, QWORD PTR [rdx]
	mov	QWORD PTR [r8], rax
	ret	0
XOR_64 ENDP

XOR_32 PROC
	mov	eax, DWORD PTR [rcx]
	xor	eax, DWORD PTR [rdx]
	mov	QWORD PTR [r8], rax
	ret	0
XOR_32 ENDP

SHL_64 PROC
	mov	rax, QWORD PTR [rcx]
	mov	rcx, QWORD PTR [rdx]
	shl	rax, cl
	mov	QWORD PTR [r8], rax
	ret	0
SHL_64 ENDP

SHR_64 PROC
	mov	rax, QWORD PTR [rcx]
	mov	rcx, QWORD PTR [rdx]
	shr	rax, cl
	mov	QWORD PTR [r8], rax
	ret	0
SHR_64 ENDP

SAR_64 PROC
	mov	rax, QWORD PTR [rcx]
	mov	rcx, QWORD PTR [rdx]
	sar	rax, cl
	mov	QWORD PTR [r8], rax
	ret	0
SAR_64 ENDP

ROL_64 PROC
	mov	rax, QWORD PTR [rcx]
	mov	rcx, QWORD PTR [rdx]
	rol	rax, cl
	mov	QWORD PTR [r8], rax
	ret	0
ROL_64 ENDP

ROR_64 PROC
	mov	rax, QWORD PTR [rcx]
	mov	rcx, QWORD PTR [rdx]
	ror	rax, cl
	mov	QWORD PTR [r8], rax
	ret	0
ROR_64 ENDP

FPINIT PROC
	mov	DWORD PTR [rsp+8], 40896
	ldmxcsr	DWORD PTR [rsp+8]
	ret	0
FPINIT ENDP

FPADD PROC
	mov	rax, QWORD PTR [rcx]
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd	xmm0, xmm1
	movsd	QWORD PTR [r8], xmm0
	ret	0
FPADD ENDP

FPSUB PROC
	mov	rax, QWORD PTR [rcx]
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd	xmm0, xmm1
	movsd	QWORD PTR [r8], xmm0
	ret	0
FPSUB ENDP

FPMUL PROC
	mov	rax, QWORD PTR [rcx]
	and rax, -2048
	or	rax, 2048
	cvtsi2sd xmm0, rax
	mulsd	xmm0, xmm1
	movsd	QWORD PTR [r8], xmm0
	ret	0
FPMUL ENDP

FPDIV PROC
	mov	rax, QWORD PTR [rcx]
	and rax, -2048
	or	rax, 2048
	cvtsi2sd xmm0, rax
	divsd	xmm0, xmm1
	movsd	QWORD PTR [r8], xmm0
	ret	0
FPDIV ENDP

FPSQRT PROC
	mov	rax, QWORD PTR [rcx]
	mov rcx, 9223372036854773760
	and rax, rcx
	cvtsi2sd xmm0, rax
	sqrtsd	xmm1, xmm0
	movsd	QWORD PTR [r8], xmm1
	ret	0
FPSQRT ENDP

FPROUND PROC
	mov	rax, QWORD PTR [rcx]
	mov rcx, rax
	shl	rax, 13
	and rcx, -2048
	and	eax, 24576
	cvtsi2sd xmm0, rcx	
	movsd	QWORD PTR [r8], xmm0
	or	eax, 40896
	mov	DWORD PTR [rsp+8], eax
	ldmxcsr	DWORD PTR [rsp+8]
	ret	0
FPROUND ENDP

END