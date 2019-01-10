	;# callee-saved registers - Microsoft x64 calling convention
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

	;# function arguments
	push rcx                    ;# RegisterFile& registerFile
	mov rbp, qword ptr [rdx]    ;# "mx", "ma"
	mov rax, qword ptr [rdx+8]  ;# uint8_t* dataset
	push rax
	mov rsi, r8                 ;# convertible_t* scratchpad

	include program_prologue_load.inc

	jmp randomx_program_begin