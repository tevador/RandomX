	;# rdi -> Cache pointer
	;# rcx -> Dataset block number
	;# rax, rbx, rcx, rdx -> scratch registers
	sub rsp, 72
	mov qword ptr [rsp+64], rbx
	mov qword ptr [rsp+56], r8
	mov qword ptr [rsp+48], r9
	mov qword ptr [rsp+40], r10
	mov qword ptr [rsp+32], r11
	mov qword ptr [rsp+24], r12
	mov qword ptr [rsp+16], r13
	mov qword ptr [rsp+8], r14
	mov qword ptr [rsp+0], r15
	mov r8, rcx
	xor r9, r9
	xor r10, r10
	xor r11, r11
	xor r12, r12
	xor r13, r13
	xor r14, r14
	xor r15, r15
	;# iteration 0
	;# c0
	mov rbx, r8
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r8+r9]
	call squareHashSub
	mov r9, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	;# c1
	mov rbx, r9
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r9+r10]
	call squareHashSub
	mov r10, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	;# c2
	mov rbx, r10
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r10+r11]
	call squareHashSub
	mov r11, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	;# c3
	mov rbx, r11
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r11+r12]
	call squareHashSub
	mov r12, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	;# c4
	mov rbx, r12
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r12+r13]
	call squareHashSub
	mov r13, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	;# c5
	mov rbx, r13
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r13+r14]
	call squareHashSub
	mov r14, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	;# c6
	mov rbx, r14
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r14+r15]
	call squareHashSub
	mov r15, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	;# c7
	mov rbx, r15
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r15+r8]
	call squareHashSub
	mov r8, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	;# iteration 1
	;# c0
	mov rbx, r8
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r8+r9]
	call squareHashSub
	mov r9, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	;# c1
	mov rbx, r9
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r9+r10]
	call squareHashSub
	mov r10, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	;# c2
	mov rbx, r10
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r10+r11]
	call squareHashSub
	mov r11, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	;# c3
	mov rbx, r11
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r11+r12]
	call squareHashSub
	mov r12, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	;# c4
	mov rbx, r12
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r12+r13]
	call squareHashSub
	mov r13, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	;# c5
	mov rbx, r13
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r13+r14]
	call squareHashSub
	mov r14, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	;# c6
	mov rbx, r14
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r14+r15]
	call squareHashSub
	mov r15, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	;# c7
	mov rbx, r15
	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]
	lea rcx, [r15+r8]
	call squareHashSub
	mov r8, rax
	xor r8, qword ptr [rbx+0]
	xor r9, qword ptr [rbx+8]
	xor r10, qword ptr [rbx+16]
	xor r11, qword ptr [rbx+24]
	xor r12, qword ptr [rbx+32]
	xor r13, qword ptr [rbx+40]
	xor r14, qword ptr [rbx+48]
	xor r15, qword ptr [rbx+56]
	;# --------------------------
	mov rbx, qword ptr [rsp+64]
	xor r8, qword ptr [rsp+56]
	xor r9, qword ptr [rsp+48]
	xor r10, qword ptr [rsp+40]
	xor r11, qword ptr [rsp+32]
	xor r12, qword ptr [rsp+24]
	xor r13, qword ptr [rsp+16]
	xor r14, qword ptr [rsp+8]
	xor r15, qword ptr [rsp+0]
	add rsp, 72
	;# xor eax, eax
	ret