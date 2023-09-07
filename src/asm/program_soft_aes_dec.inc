	mov eax, dword ptr [rsi+0]
	mov ebx, dword ptr [rsi+4]
	mov ecx, dword ptr [rsi+8]
	mov edx, dword ptr [rsi+12]

	mov rsi, qword ptr [aes_lut_dec+RIP_REL]

	movzx ebp, byte ptr [rdi+0]
	xor eax, dword ptr [rsi+rbp*4]
	movzx ebp, byte ptr [rdi+1]
	xor ebx, dword ptr [rsi+rbp*4+1024]
	movzx ebp, byte ptr [rdi+2]
	xor ecx, dword ptr [rsi+rbp*4+2048]
	movzx ebp, byte ptr [rdi+3]
	xor edx, dword ptr [rsi+rbp*4+3072]

	movzx ebp, byte ptr [rdi+4]
	xor ebx, dword ptr [rsi+rbp*4]
	movzx ebp, byte ptr [rdi+5]
	xor ecx, dword ptr [rsi+rbp*4+1024]
	movzx ebp, byte ptr [rdi+6]
	xor edx, dword ptr [rsi+rbp*4+2048]
	movzx ebp, byte ptr [rdi+7]
	xor eax, dword ptr [rsi+rbp*4+3072]

	movzx ebp, byte ptr [rdi+8]
	xor ecx, dword ptr [rsi+rbp*4]
	movzx ebp, byte ptr [rdi+9]
	xor edx, dword ptr [rsi+rbp*4+1024]
	movzx ebp, byte ptr [rdi+10]
	xor eax, dword ptr [rsi+rbp*4+2048]
	movzx ebp, byte ptr [rdi+11]
	xor ebx, dword ptr [rsi+rbp*4+3072]

	movzx ebp, byte ptr [rdi+12]
	xor edx, dword ptr [rsi+rbp*4]
	movzx ebp, byte ptr [rdi+13]
	xor eax, dword ptr [rsi+rbp*4+1024]
	movzx ebp, byte ptr [rdi+14]
	xor ebx, dword ptr [rsi+rbp*4+2048]
	movzx ebp, byte ptr [rdi+15]
	xor ecx, dword ptr [rsi+rbp*4+3072]

	mov dword ptr [rdi+0], eax
	mov dword ptr [rdi+4], ebx
	mov dword ptr [rdi+8], ecx
	mov dword ptr [rdi+12], edx

	ret
