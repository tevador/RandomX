	include program_epilogue_store.inc

	;# restore callee-saved registers - Microsoft x64 calling convention
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

	;# program finished
	ret	0