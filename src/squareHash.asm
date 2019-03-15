IFDEF RAX

PUBLIC squareHash
PUBLIC initBlock

.code

squareHash PROC
	include asm/squareHash.inc
squareHash ENDP

; rcx = cache
; rdx = out
; r8 = blockNumber
; r9 = iterations
initBlock PROC
	push rbx
	push rbp
	push rsi
	push rdi
	push r12
	push r13
	push r14
	push r15
	mov rsi, r9
	mov rdi, qword ptr [rcx]
	mov rbp, rdx
	prefetcht0 byte ptr [rbp]
	; r8 = blockNumber
	xor r9, r9
	xor r10, r10
	xor r11, r11
	xor r12, r12
	xor r13, r13
	xor r14, r14
	xor r15, r15
initBlock_loop:
	; c0
	mov rbx, r8
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r8+r9]
	call squareHash
	mov r9, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	; c1
	mov rbx, r9
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r9+r10]
	call squareHash
	mov r10, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	; c2
	mov rbx, r10
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r10+r11]
	call squareHash
	mov r11, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	; c3
	mov rbx, r11
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r11+r12]
	call squareHash
	mov r12, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	; c4
	mov rbx, r12
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r12+r13]
	call squareHash
	mov r13, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	; c5
	mov rbx, r13
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r13+r14]
	call squareHash
	mov r14, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	; c6
	mov rbx, r14
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r14+r15]
	call squareHash
	mov r15, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	; c7
	mov rbx, r15
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r15+r8]
	call squareHash
	mov r8, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	sub rsi, 1
	jnz initBlock_loop
	mov qword ptr [rbp+0], r8
	mov qword ptr [rbp+8], r9
	mov qword ptr [rbp+16], r10
	mov qword ptr [rbp+24], r11
	mov qword ptr [rbp+32], r12
	mov qword ptr [rbp+40], r13
	mov qword ptr [rbp+48], r14
	mov qword ptr [rbp+56], r15
	pop r15
	pop r14
	pop r13
	pop r12
	pop rdi
	pop rsi
	pop rbp
	pop rbx
	ret
initBlock ENDP

ENDIF

END