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
	sub rsp, 80
	movdqu xmmword ptr [rsp+64], xmm11
	movdqu xmmword ptr [rsp+48], xmm12
	movdqu xmmword ptr [rsp+32], xmm13
	movdqu xmmword ptr [rsp+16], xmm14
	movdqu xmmword ptr [rsp+0], xmm15

	; function arguments
	push rcx                    ; RegisterFile& registerFile
	mov rbp, qword ptr [rdx]    ; "mx", "ma"
	mov rdi, qword ptr [rdx+8]  ; uint8_t* dataset
	mov rsi, r8                 ; convertible_t* scratchpad
	mov rbx, r9                 ; loop counter

	include program_prologue_load.inc

	jmp randomx_program_loop_begin