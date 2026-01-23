	;# callee-saved registers - System V AMD64 ABI
	sub rsp, 456
	mov qword ptr [rsp+224], rbx
	mov qword ptr [rsp+232], rbp
	mov qword ptr [rsp+256], r12
	mov qword ptr [rsp+264], r13
	mov qword ptr [rsp+272], r14
	mov qword ptr [rsp+280], r15

	;# function arguments
	mov rbx, rcx                  ;# loop counter
	mov qword ptr [rsp+448], rdi  ;# RegisterFile& registerFile
	mov rcx, rdi
	mov rbp, qword ptr [rsi]      ;# "mx", "ma"
	mov rdi, qword ptr [rsi+8]    ;# uint8_t* dataset
	mov rsi, rdx                  ;# uint8_t* scratchpad

	mov rax, rbp
	mov r8d, ebp
	ror rbp, 32
	mov r9d, ebp

	and r8d, RANDOMX_DATASET_BASE_MASK
	and r9d, RANDOMX_DATASET_BASE_MASK
	prefetchnta byte ptr [rdi+r8]
	prefetchnta byte ptr [rdi+r9]

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
