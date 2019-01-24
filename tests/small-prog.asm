	; ISTORE L1[r6], r4
	mov eax, r14d
	and eax, 16376
	mov qword ptr [rsi+rax], r12
	; IROR_R r1, r2
	mov ecx, r10d
	ror r9, cl
	; FPNEG_R f2
	xorps xmm2, xmm15
	; FPSUB_R f1, a1
	subpd xmm1, xmm9
	; FPMUL_M e3, L2[r5]
	mov eax, r13d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	mulpd xmm7, xmm12
	maxpd xmm7, xmm13
	; IMUL_R r7, r1
	imul r15, r9
	; IMUL_R r3, r6
	imul r11, r14
	; IMUL_R r5, r4
	imul r13, r12
	; IADD_R r2, r3
	add r10, r11
	; IMUL_R r3, r4
	imul r11, r12
	; ISDIV_C r7, 1531724965
	mov rax, 3232799797802813183
	imul r15
	xor eax, eax
	sar rdx, 28
	sets al
	add rdx, rax
	add r15, rdx
	; IADD_R r7, r1
	add r15, r9
	; IMUL_R r7, r3
	imul r15, r11
	; ISUB_R r6, r7
	sub r14, r15
	; ISTORE L1[r4], r6
	mov eax, r12d
	and eax, 16376
	mov qword ptr [rsi+rax], r14
	; IADD_M r4, L1[r6]
	mov eax, r14d
	and eax, 16376
	add r12, qword ptr [rsi+rax]
	; FPSWAP_R f0
	shufpd xmm0, xmm0, 1
	; FPSUB_R f1, a0
	subpd xmm1, xmm8
	; ISUB_R r2, r4
	sub r10, r12
	; FPNEG_R f2
	xorps xmm2, xmm15
	; IMUL_R r3, 1367232543
	imul r11, 1367232543
	; IROL_R r1, r7
	mov ecx, r15d
	rol r9, cl
	; IMUL_9C r2, 1164637590
	lea r10, [r10+r10*8+1164637590]
	; FPADD_M f0, L2[r3]
	mov eax, r11d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm0, xmm12
	; FPMUL_R e1, a2
	mulpd xmm5, xmm10
	; IMUL_M r0, L1[8240]
	imul r8, qword ptr [rsi+8240]
	; IMULH_M r4, L2[186792]
	mov rax, r12
	mul qword ptr [rsi+186792]
	mov r12, rdx
	; IDIV_C r3, 973927932
	mov rax, r11
	shr rax, 2
	mov rcx, 5084318864305373573
	mul rcx
	shr rdx, 26
	add r11, rdx
	; IROL_R r2, r5
	mov ecx, r13d
	rol r10, cl
	; ISMULH_R r3, r0
	mov rax, r11
	imul r8
	mov r11, rdx
	; ISTORE L2[r3], r6
	mov eax, r11d
	and eax, 262136
	mov qword ptr [rsi+rax], r14
	; FPSQRT_R e1
	sqrtpd xmm5, xmm5
	; ISTORE L2[r0], r3
	mov eax, r8d
	and eax, 262136
	mov qword ptr [rsi+rax], r11
	; FPDIV_R e2, a0
	divpd xmm6, xmm8
	maxpd xmm6, xmm13
	; COND_R r2, lt(r2, 809935569)
	xor ecx, ecx
	cmp r10d, 809935569
	setl cl
	add r10, rcx
	; IDIV_C r3, 3449361310
	mov rax, r11
	shr rax, 1
	mov rcx, 11484468484727153387
	mul rcx
	shr rdx, 30
	add r11, rdx
	; FPSWAP_R f2
	shufpd xmm2, xmm2, 1
	; IROL_R r6, 42
	rol r14, 42
	; IMULH_M r1, L2[r2]
	mov ecx, r10d
	and ecx, 262136
	mov rax, r9
	mul qword ptr [rsi+rcx]
	mov r9, rdx
	; FPADD_R f2, a2
	addpd xmm2, xmm10
	; IROR_R r4, r7
	mov ecx, r15d
	ror r12, cl
	; FPMUL_R e3, a0
	mulpd xmm7, xmm8
	; ISTORE L2[r7], r4
	mov eax, r15d
	and eax, 262136
	mov qword ptr [rsi+rax], r12
	; IMUL_9C r7, -7511892
	lea r15, [r15+r15*8-7511892]
	; IROL_R r7, r5
	mov ecx, r13d
	rol r15, cl
	; FPMUL_R e1, a2
	mulpd xmm5, xmm10
	; FPSUB_R f1, a1
	subpd xmm1, xmm9
	; FPSUB_R f3, a2
	subpd xmm3, xmm10
	; IADD_RC r0, r0, 636102408
	lea r8, [r8+r8+636102408]
	; ISUB_R r4, r0
	sub r12, r8
	; IADD_M r3, L1[r0]
	mov eax, r8d
	and eax, 16376
	add r11, qword ptr [rsi+rax]
	; FPADD_R f3, a3
	addpd xmm3, xmm11
	; IADD_R r0, r2
	add r8, r10
	; FPSUB_R f0, a3
	subpd xmm0, xmm11
	; IADD_R r2, r1
	add r10, r9
	; COND_R r4, ge(r7, 295314673)
	xor ecx, ecx
	cmp r15d, 295314673
	setge cl
	add r12, rcx
	; IMUL_R r2, r5
	imul r10, r13
	; ISMULH_R r4, -160066964
	mov rax, -160066964
	imul r12
	add r12, rdx
	; IMUL_R r2, r6
	imul r10, r14
	; IMUL_9C r1, 526734458
	lea r9, [r9+r9*8+526734458]
	; IMUL_R r3, r5
	imul r11, r13
	; FPSUB_M f0, L1[r6]
	mov eax, r14d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm0, xmm12
	; FPMUL_R e3, a2
	mulpd xmm7, xmm10
	; IXOR_R r3, r1
	xor r11, r9
	; ISTORE L2[r0], r0
	mov eax, r8d
	and eax, 262136
	mov qword ptr [rsi+rax], r8
	; FPSWAP_R f3
	shufpd xmm3, xmm3, 1
	; ISUB_M r2, L1[14864]
	sub r10, qword ptr [rsi+14864]
	; IDIV_C r6, 4274620060
	mov rax, r14
	shr rax, 2
	mov rcx, 4633637691899589411
	mul rcx
	shr rdx, 28
	add r14, rdx
	; ISUB_R r7, r6
	sub r15, r14
	; ISMULH_R r3, r0
	mov rax, r11
	imul r8
	mov r11, rdx
	; IADD_R r5, r3
	add r13, r11
	; IROR_R r5, r3
	mov ecx, r11d
	ror r13, cl
	; ISUB_R r2, r5
	sub r10, r13
	; COND_R r7, sg(r5, 1569330334)
	xor ecx, ecx
	cmp r13d, 1569330334
	sets cl
	add r15, rcx
	; IROR_R r1, r6
	mov ecx, r14d
	ror r9, cl
	; FPADD_R f0, a1
	addpd xmm0, xmm9
	; FPSQRT_R e3
	sqrtpd xmm7, xmm7
	; FPSUB_R f2, a3
	subpd xmm2, xmm11
	; ISMULH_M r2, L1[r3]
	mov ecx, r11d
	and ecx, 16376
	mov rax, r10
	imul qword ptr [rsi+rcx]
	mov r10, rdx
	; IMUL_9C r5, -1590168006
	lea r13, [r13+r13*8-1590168006]
	; IMUL_9C r4, 1994845080
	lea r12, [r12+r12*8+1994845080]
	; IADD_R r1, r3
	add r9, r11
	; IROL_R r4, r6
	mov ecx, r14d
	rol r12, cl
	; IMUL_R r1, -1333414368
	imul r9, -1333414368
	; ISTORE L1[r5], r0
	mov eax, r13d
	and eax, 16376
	mov qword ptr [rsi+rax], r8
	; IROL_R r5, r7
	mov ecx, r15d
	rol r13, cl
	; ISUB_M r0, L1[r5]
	mov eax, r13d
	and eax, 16376
	sub r8, qword ptr [rsi+rax]
	; COND_R r6, sg(r5, 43404748)
	xor ecx, ecx
	cmp r13d, 43404748
	sets cl
	add r14, rcx
	; IMULH_R r4, r5
	mov rax, r12
	mul r13
	mov r12, rdx
	; FPDIV_R e3, a3
	divpd xmm7, xmm11
	maxpd xmm7, xmm13
	; INEG_R r6
	neg r14
	; IROL_R r1, r2
	mov ecx, r10d
	rol r9, cl
	; ISUB_M r3, L1[r7]
	mov eax, r15d
	and eax, 16376
	sub r11, qword ptr [rsi+rax]
	; FPSUB_R f2, a0
	subpd xmm2, xmm8
	; IMUL_R r7, 1436360085
	imul r15, 1436360085
	; ISMULH_M r7, L2[r3]
	mov ecx, r11d
	and ecx, 262136
	mov rax, r15
	imul qword ptr [rsi+rcx]
	mov r15, rdx
	; IXOR_R r6, r1
	xor r14, r9
	; FPSUB_M f0, L1[r2]
	mov eax, r10d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm0, xmm12
	; ISMULH_R r1, -1626920972
	mov rax, -1626920972
	imul r9
	add r9, rdx
	; IMUL_R r1, -1587440282
	imul r9, -1587440282
	; FPADD_R f3, a3
	addpd xmm3, xmm11
	; FPDIV_R e3, a0
	divpd xmm7, xmm8
	maxpd xmm7, xmm13
	; FPSUB_R f2, a1
	subpd xmm2, xmm9
	; FPMUL_R e2, a0
	mulpd xmm6, xmm8
	; FPSQRT_R e2
	sqrtpd xmm6, xmm6
	; FPMUL_R e3, a2
	mulpd xmm7, xmm10
	; FPMUL_R e0, a3
	mulpd xmm4, xmm11
	; FPMUL_R e1, a2
	mulpd xmm5, xmm10
	; FPSUB_R f0, a2
	subpd xmm0, xmm10
	; IMUL_R r6, r4
	imul r14, r12
	; IMUL_R r6, r3
	imul r14, r11
	; FPMUL_R e0, a1
	mulpd xmm4, xmm9
	; IMUL_R r2, r6
	imul r10, r14
	; IXOR_R r1, r0
	xor r9, r8
	; IMUL_M r4, L1[r7]
	mov eax, r15d
	and eax, 16376
	imul r12, qword ptr [rsi+rax]
	; FPSUB_R f2, a0
	subpd xmm2, xmm8
	; IROL_R r3, r0
	mov ecx, r8d
	rol r11, cl
	; IROR_R r2, 61
	ror r10, 61
	; FPADD_R f2, a2
	addpd xmm2, xmm10
	; COND_R r5, sg(r1, -1184956925)
	xor ecx, ecx
	cmp r9d, -1184956925
	sets cl
	add r13, rcx
	; ISTORE L1[r2], r5
	mov eax, r10d
	and eax, 16376
	mov qword ptr [rsi+rax], r13
	; FPSWAP_R e1
	shufpd xmm5, xmm5, 1
	; IADD_R r4, r5
	add r12, r13
	; IADD_R r4, r3
	add r12, r11
	; FPDIV_R e3, a1
	divpd xmm7, xmm9
	maxpd xmm7, xmm13
	; IADD_RC r6, r5, 1890583833
	lea r14, [r14+r13+1890583833]
	; ISTORE L1[r3], r1
	mov eax, r11d
	and eax, 16376
	mov qword ptr [rsi+rax], r9
	; IADD_RC r3, r2, 1329347581
	lea r11, [r11+r10+1329347581]
	; FPMUL_R e3, a1
	mulpd xmm7, xmm9
	; IMUL_9C r3, -676169110
	lea r11, [r11+r11*8-676169110]
	; COND_R r5, ns(r3, 531330698)
	xor ecx, ecx
	cmp r11d, 531330698
	setns cl
	add r13, rcx
	; ISUB_R r7, r2
	sub r15, r10
	; IADD_RC r7, r0, 974749728
	lea r15, [r15+r8+974749728]
	; IMULH_R r6, 1083698437
	mov eax, 1083698437
	mul r14
	add r14, rdx
	; FPNEG_R f2
	xorps xmm2, xmm15
	; IMUL_R r6, r7
	imul r14, r15
	; FPSQRT_R e3
	sqrtpd xmm7, xmm7
	; IADD_RC r2, r2, 1044563079
	lea r10, [r10+r10+1044563079]
	; IADD_RC r4, r1, -2016187742
	lea r12, [r12+r9-2016187742]
	; FPDIV_R e0, a1
	divpd xmm4, xmm9
	maxpd xmm4, xmm13
	; IMUL_R r3, r4
	imul r11, r12
	; ISDIV_C r3, 1398200496
	mov rax, -4280649378971233915
	imul r11
	xor eax, eax
	add rdx, r11
	sar rdx, 30
	sets al
	add rdx, rax
	add r11, rdx
	; COND_R r5, ge(r7, -596284511)
	xor ecx, ecx
	cmp r15d, -596284511
	setge cl
	add r13, rcx
	; ISTORE L1[r0], r0
	mov eax, r8d
	and eax, 16376
	mov qword ptr [rsi+rax], r8
	; ISMULH_M r6, L2[r7]
	mov ecx, r15d
	and ecx, 262136
	mov rax, r14
	imul qword ptr [rsi+rcx]
	mov r14, rdx
	; IMUL_R r5, r4
	imul r13, r12
	; IROR_R r7, r4
	mov ecx, r12d
	ror r15, cl
	; FPADD_M f3, L1[r0]
	mov eax, r8d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm3, xmm12
	; IMUL_R r2, r5
	imul r10, r13
	; IXOR_R r7, r1
	xor r15, r9
	; ISTORE L1[r4], r3
	mov eax, r12d
	and eax, 16376
	mov qword ptr [rsi+rax], r11
	; INEG_R r5
	neg r13
	; IADD_R r4, r7
	add r12, r15
	; IMUL_R r6, r4
	imul r14, r12
	; IMUL_M r5, L1[r2]
	mov eax, r10d
	and eax, 16376
	imul r13, qword ptr [rsi+rax]
	; FPMUL_M e1, L1[r7]
	mov eax, r15d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	mulpd xmm5, xmm12
	maxpd xmm5, xmm13
	; IROR_R r2, r7
	mov ecx, r15d
	ror r10, cl
	; COND_M r5, ab(L1[r3], -1085209087)
	xor ecx, ecx
	mov eax, r11d
	and eax, 16376
	cmp dword ptr [rsi+rax], -1085209087
	seta cl
	add r13, rcx
	; FPMUL_R e3, a3
	mulpd xmm7, xmm11
	; IMUL_9C r6, 898607426
	lea r14, [r14+r14*8+898607426]
	; IMUL_9C r1, -1816383392
	lea r9, [r9+r9*8-1816383392]
	; IROR_R r3, r4
	mov ecx, r12d
	ror r11, cl
	; FPSUB_R f2, a3
	subpd xmm2, xmm11
	; FPSQRT_R e3
	sqrtpd xmm7, xmm7
	; FPDIV_R e3, a0
	divpd xmm7, xmm8
	maxpd xmm7, xmm13
	; FPMUL_M e3, L2[r2]
	mov eax, r10d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	mulpd xmm7, xmm12
	maxpd xmm7, xmm13
	; FPADD_R f2, a1
	addpd xmm2, xmm9
	; IROL_R r3, r6
	mov ecx, r14d
	rol r11, cl
	; FPSUB_R f3, a0
	subpd xmm3, xmm8
	; IMUL_M r5, L1[r2]
	mov eax, r10d
	and eax, 16376
	imul r13, qword ptr [rsi+rax]
	; COND_R r7, ge(r4, 1959614002)
	xor ecx, ecx
	cmp r12d, 1959614002
	setge cl
	add r15, rcx
	; IADD_RC r5, r2, 1887914017
	lea r13, [r13+r10+1887914017]
	; FSTORE L2[r4], f0
	mov eax, r12d
	and eax, 262128
	movapd xmmword ptr [rsi+rax], xmm0
	; IMUL_R r6, r4
	imul r14, r12
	; IMULH_M r2, L1[r6]
	mov ecx, r14d
	and ecx, 16376
	mov rax, r10
	mul qword ptr [rsi+rcx]
	mov r10, rdx
	; IADD_RC r0, r6, 723017482
	lea r8, [r8+r14+723017482]
	; ISUB_R r6, r5
	sub r14, r13
	; IROR_R r3, r2
	mov ecx, r10d
	ror r11, cl
	; IADD_R r6, r2
	add r14, r10
	; FPSQRT_R e2
	sqrtpd xmm6, xmm6
	; ISDIV_C r3, -1434854386
	mov rax, -3451054131664006427
	imul r11
	xor eax, eax
	sar rdx, 28
	sets al
	add rdx, rax
	add r11, rdx
	; IROL_R r7, r5
	mov ecx, r13d
	rol r15, cl
	; FPSQRT_R e3
	sqrtpd xmm7, xmm7
	; IMUL_R r4, r7
	imul r12, r15
	; FPSUB_R f2, a2
	subpd xmm2, xmm10
	; FPSUB_R f2, a1
	subpd xmm2, xmm9
	; IMUL_9C r0, -43443857
	lea r8, [r8+r8*8-43443857]
	; COND_M r2, of(L2[r7], -1059200178)
	xor ecx, ecx
	mov eax, r15d
	and eax, 262136
	cmp dword ptr [rsi+rax], -1059200178
	seto cl
	add r10, rcx
	; IMUL_M r5, L1[r2]
	mov eax, r10d
	and eax, 16376
	imul r13, qword ptr [rsi+rax]
	; FPMUL_R e0, a2
	mulpd xmm4, xmm10
	; IXOR_R r1, r3
	xor r9, r11
	; ISUB_R r5, r1
	sub r13, r9
	; FPSQRT_R e1
	sqrtpd xmm5, xmm5
	; IDIV_C r3, 531478046
	mov rax, 2329240217168594533
	mul r11
	shr rdx, 26
	add r11, rdx
	; IMULH_M r7, L2[r2]
	mov ecx, r10d
	and ecx, 262136
	mov rax, r15
	mul qword ptr [rsi+rcx]
	mov r15, rdx
	; IMUL_9C r1, -1546338561
	lea r9, [r9+r9*8-1546338561]
	; FSTORE L2[r6], e2
	mov eax, r14d
	and eax, 262128
	movapd xmmword ptr [rsi+rax], xmm6
	; IROR_R r6, r7
	mov ecx, r15d
	ror r14, cl
	; FPMUL_R e1, a1
	mulpd xmm5, xmm9
	; FPMUL_R e3, a3
	mulpd xmm7, xmm11
	; IROR_R r2, r3
	mov ecx, r11d
	ror r10, cl
	; FPADD_R f1, a1
	addpd xmm1, xmm9
	; IXOR_R r2, r4
	xor r10, r12
	; FPNEG_R f0
	xorps xmm0, xmm15
	; FPDIV_R e2, a2
	divpd xmm6, xmm10
	maxpd xmm6, xmm13
	; IXOR_M r6, L1[r7]
	mov eax, r15d
	and eax, 16376
	xor r14, qword ptr [rsi+rax]
	; FPSWAP_R f1
	shufpd xmm1, xmm1, 1
	; FSTORE L1[r7], e0
	mov eax, r15d
	and eax, 16368
	movapd xmmword ptr [rsi+rax], xmm4
	; FPADD_M f1, L1[r2]
	mov eax, r10d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm1, xmm12
	; IXOR_R r2, r0
	xor r10, r8
	; COND_M r7, no(L1[r7], 995954598)
	xor ecx, ecx
	mov eax, r15d
	and eax, 16376
	cmp dword ptr [rsi+rax], 995954598
	setno cl
	add r15, rcx
	; ISTORE L1[r0], r1
	mov eax, r8d
	and eax, 16376
	mov qword ptr [rsi+rax], r9
	; IADD_R r4, r0
	add r12, r8
	; FPSUB_R f2, a3
	subpd xmm2, xmm11
	; FPNEG_R f1
	xorps xmm1, xmm15
	; FPSUB_R f0, a0
	subpd xmm0, xmm8
	; FPADD_R f2, a0
	addpd xmm2, xmm8
	; COND_R r7, ns(r2, -772621280)
	xor ecx, ecx
	cmp r10d, -772621280
	setns cl
	add r15, rcx
	; IMULH_R r3, -531436276
	mov eax, -531436276
	mul r11
	add r11, rdx
	; COND_R r0, lt(r4, -1228919974)
	xor ecx, ecx
	cmp r12d, -1228919974
	setl cl
	add r8, rcx
	; ISTORE L1[r0], r0
	mov eax, r8d
	and eax, 16376
	mov qword ptr [rsi+rax], r8
	; IROR_R r4, 25
	ror r12, 25
	; FPMUL_M e3, L1[r0]
	mov eax, r8d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	mulpd xmm7, xmm12
	maxpd xmm7, xmm13
	; FPSUB_R f3, a2
	subpd xmm3, xmm10
	; IXOR_R r7, r0
	xor r15, r8
	; IROL_R r6, r2
	mov ecx, r10d
	rol r14, cl
	; FPMUL_R e1, a2
	mulpd xmm5, xmm10
	; FPSQRT_R e0
	sqrtpd xmm4, xmm4
	; IMUL_M r3, L1[r4]
	mov eax, r12d
	and eax, 16376
	imul r11, qword ptr [rsi+rax]
	; FPADD_R f2, a2
	addpd xmm2, xmm10
	; ISMULH_R r6, r2
	mov rax, r14
	imul r10
	mov r14, rdx
	; FPMUL_M e3, L1[r5]
	mov eax, r13d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	mulpd xmm7, xmm12
	maxpd xmm7, xmm13
	; ISUB_R r3, r5
	sub r11, r13
	; IADD_R r2, r3
	add r10, r11
	; COND_R r3, of(r6, -566248014)
	xor ecx, ecx
	cmp r14d, -566248014
	seto cl
	add r11, rcx
	; ISTORE L1[r3], r2
	mov eax, r11d
	and eax, 16376
	mov qword ptr [rsi+rax], r10
	; FPADD_R f1, a0
	addpd xmm1, xmm8
	; IXOR_M r2, L2[r1]
	mov eax, r9d
	and eax, 262136
	xor r10, qword ptr [rsi+rax]
	; FPSUB_R f2, a2
	subpd xmm2, xmm10
	; FPADD_R f1, a1
	addpd xmm1, xmm9
	; IMUL_M r5, L1[6488]
	imul r13, qword ptr [rsi+6488]
	; IROR_R r0, 50
	ror r8, 50
	; IMUL_9C r7, 1313192705
	lea r15, [r15+r15*8+1313192705]
	; FPADD_R f0, a1
	addpd xmm0, xmm9
	; IMUL_9C r0, 611229050
	lea r8, [r8+r8*8+611229050]
	; FPADD_R f0, a0
	addpd xmm0, xmm8
	; FPMUL_R e2, a2
	mulpd xmm6, xmm10
	; IMUL_R r6, r5
	imul r14, r13
	; IADD_M r3, L1[r5]
	mov eax, r13d
	and eax, 16376
	add r11, qword ptr [rsi+rax]
	; COND_M r1, lt(L1[r7], -248613240)
	xor ecx, ecx
	mov eax, r15d
	and eax, 16376
	cmp dword ptr [rsi+rax], -248613240
	setl cl
	add r9, rcx
	; ISUB_R r2, r4
	sub r10, r12
	; FPADD_R f3, a3
	addpd xmm3, xmm11
	; FSTORE L1[r0], e3
	mov eax, r8d
	and eax, 16368
	movapd xmmword ptr [rsi+rax], xmm7
	; IMUL_R r6, r0
	imul r14, r8
	; FPADD_R f2, a2
	addpd xmm2, xmm10
	; FPDIV_R e1, a2
	divpd xmm5, xmm10
	maxpd xmm5, xmm13
