	mov eax, dword ptr [rbx]      ;# ma
	mov rdx, qword ptr [rbx+8]    ;# dataset
	mov rax, qword ptr [rdx+rax]
	add dword ptr [rbx], 8
	xor ecx, dword ptr [rbx+4]    ;# mx
	mov dword ptr [rbx+4], ecx
	test ecx, 65528
	jne short rx_read_dataset_r_ret
	and ecx, -8
	mov dword ptr [rbx], ecx
	prefetcht0 byte ptr [rdx+rcx]
rx_read_dataset_r_ret:
	ret 0