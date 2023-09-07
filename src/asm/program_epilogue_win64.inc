	;# restore callee-saved registers - Microsoft x64 calling convention
	movdqa xmm15, xmmword ptr [rsp+432]
	movdqa xmm14, xmmword ptr [rsp+416]
	movdqa xmm13, xmmword ptr [rsp+400]
	movdqa xmm12, xmmword ptr [rsp+384]
	movdqa xmm11, xmmword ptr [rsp+368]
	movdqa xmm10, xmmword ptr [rsp+352]
	movdqa xmm9, xmmword ptr [rsp+336]
	movdqa xmm8, xmmword ptr [rsp+320]
	movdqa xmm7, xmmword ptr [rsp+304]
	movdqa xmm6, xmmword ptr [rsp+288]
	mov r15, qword ptr [rsp+280]
	mov r14, qword ptr [rsp+272]
	mov r13, qword ptr [rsp+264]
	mov r12, qword ptr [rsp+256]
	mov rdi, qword ptr [rsp+248]
	mov rsi, qword ptr [rsp+240]
	mov rbp, qword ptr [rsp+232]
	mov rbx, qword ptr [rsp+224]

	add rsp, 456

	;# program finished
	ret
