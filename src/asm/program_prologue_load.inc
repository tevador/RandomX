	mov rdi, rsp      ;# beginning of VM stack
	mov ebx, 262145   ;# number of VM instructions to execute + 1

	xorps xmm10, xmm10
	cmpeqpd xmm10, xmm10
	psrlq xmm10, 1    ;# mask for absolute value = 0x7fffffffffffffff7fffffffffffffff

	;# load integer registers
	mov r8, qword ptr [rcx+0]
	mov r9, qword ptr [rcx+8]
	mov r10, qword ptr [rcx+16]
	mov r11, qword ptr [rcx+24]
	mov r12, qword ptr [rcx+32]
	mov r13, qword ptr [rcx+40]
	mov r14, qword ptr [rcx+48]
	mov r15, qword ptr [rcx+56]

	;# load floating point registers
	movapd xmm8, xmmword ptr [rcx+64]
	movapd xmm9, xmmword ptr [rcx+80]
	movapd xmm2, xmmword ptr [rcx+96]
	movapd xmm3, xmmword ptr [rcx+112]
	lea rcx, [rcx+64]
	movapd xmm4, xmmword ptr [rcx+64]
	movapd xmm5, xmmword ptr [rcx+80]
	movapd xmm6, xmmword ptr [rcx+96]
	movapd xmm7, xmmword ptr [rcx+112]
