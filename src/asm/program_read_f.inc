	mov edx, dword ptr [rbx]      ;# ma
	mov rax, qword ptr [rbx+8]    ;# dataset
	cvtdq2pd xmm0, qword ptr [rax+rdx]
	add dword ptr [rbx], 8
	xor ecx, dword ptr [rbx+4]    ;# mx
	mov dword ptr [rbx+4], ecx
	test ecx, 65528
	jne short rx_read_dataset_f_ret
	and ecx, -8
	mov dword ptr [rbx], ecx
	prefetcht0 byte ptr [rax+rcx]
rx_read_dataset_f_ret:
	ret 0