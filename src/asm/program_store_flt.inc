	and eax, 1048512
	lea rcx, [rsi+rax]
	mulpd xmm0, xmm4
	mulpd xmm1, xmm5
	mulpd xmm2, xmm6
	mulpd xmm3, xmm7
	movapd xmmword ptr [rcx+0], xmm0
	movapd xmmword ptr [rcx+16], xmm1
	movapd xmmword ptr [rcx+32], xmm2
	movapd xmmword ptr [rcx+48], xmm3

