	;# restore callee-saved registers - System V AMD64 ABI
	mov r15, qword ptr [rsp+280]
	mov r14, qword ptr [rsp+272]
	mov r13, qword ptr [rsp+264]
	mov r12, qword ptr [rsp+256]
	mov rbp, qword ptr [rsp+232]
	mov rbx, qword ptr [rsp+224]

	add rsp, 456

	;# program finished
	ret
