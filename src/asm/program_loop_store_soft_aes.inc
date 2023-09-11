	mov rcx, qword ptr [rsp+16]
	mov qword ptr [rcx+0], r8
	mov qword ptr [rcx+8], r9
	mov qword ptr [rcx+16], r10
	mov qword ptr [rcx+24], r11
	mov qword ptr [rcx+32], r12
	mov qword ptr [rcx+40], r13
	mov qword ptr [rcx+48], r14
	mov qword ptr [rcx+56], r15
	mov rcx, qword ptr [rsp+8]

	movapd xmmword ptr [rsp+96], xmm0
	movapd xmmword ptr [rsp+112], xmm1
	movapd xmmword ptr [rsp+128], xmm2
	movapd xmmword ptr [rsp+144], xmm3
	movapd xmmword ptr [rsp+160], xmm4
	movapd xmmword ptr [rsp+176], xmm5
	movapd xmmword ptr [rsp+192], xmm6
	movapd xmmword ptr [rsp+208], xmm7

	mov qword ptr [rsp+24], rax
	mov qword ptr [rsp+40], rcx
	mov qword ptr [rsp+48], rdx
	mov qword ptr [rsp+56], rbp
	mov qword ptr [rsp+72], rsi
	mov qword ptr [rsp+80], rdi
	mov qword ptr [rsp+88], rbx

	lea rsi, [rsp+160]
	lea rdi, [rsp+96]
	call soft_aes_enc
	lea rsi, [rsp+160]
	lea rdi, [rsp+112]
	call soft_aes_dec
	lea rsi, [rsp+160]
	lea rdi, [rsp+128]
	call soft_aes_enc
	lea rsi, [rsp+160]
	lea rdi, [rsp+144]
	call soft_aes_dec

	lea rsi, [rsp+176]
	lea rdi, [rsp+96]
	call soft_aes_enc
	lea rsi, [rsp+176]
	lea rdi, [rsp+112]
	call soft_aes_dec
	lea rsi, [rsp+176]
	lea rdi, [rsp+128]
	call soft_aes_enc
	lea rsi, [rsp+176]
	lea rdi, [rsp+144]
	call soft_aes_dec

	lea rsi, [rsp+192]
	lea rdi, [rsp+96]
	call soft_aes_enc
	lea rsi, [rsp+192]
	lea rdi, [rsp+112]
	call soft_aes_dec
	lea rsi, [rsp+192]
	lea rdi, [rsp+128]
	call soft_aes_enc
	lea rsi, [rsp+192]
	lea rdi, [rsp+144]
	call soft_aes_dec

	lea rsi, [rsp+208]
	lea rdi, [rsp+96]
	call soft_aes_enc
	lea rsi, [rsp+208]
	lea rdi, [rsp+112]
	call soft_aes_dec
	lea rsi, [rsp+208]
	lea rdi, [rsp+128]
	call soft_aes_enc
	lea rsi, [rsp+208]
	lea rdi, [rsp+144]
	call soft_aes_dec

	mov rax, qword ptr [rsp+24]
	mov rcx, qword ptr [rsp+40]
	mov rdx, qword ptr [rsp+48]
	mov rbp, qword ptr [rsp+56]
	mov rsi, qword ptr [rsp+72]
	mov rdi, qword ptr [rsp+80]
	mov rbx, qword ptr [rsp+88]

	movapd xmm0, xmmword ptr [rsp+96]
	movapd xmm1, xmmword ptr [rsp+112]
	movapd xmm2, xmmword ptr [rsp+128]
	movapd xmm3, xmmword ptr [rsp+144]

	movapd xmmword ptr [rcx+0], xmm0
	movapd xmmword ptr [rcx+16], xmm1
	movapd xmmword ptr [rcx+32], xmm2
	movapd xmmword ptr [rcx+48], xmm3
