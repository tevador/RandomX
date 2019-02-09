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
