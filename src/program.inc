	; ISUB_R r0, r4
	sub r8, r12
	; IROR_R r5, 15
	ror r13, 15
	; ISUB_M r6, L1[r5]
	mov eax, r13d
	and eax, 16376
	sub r14, qword ptr [rsi+rax]
	; IMUL_R r7, r6
	imul r15, r14
	; FPADD_R f3, a1
	addpd xmm3, xmm9
	; FPMUL_R e1, a3
	mulpd xmm5, xmm11
	; IMUL_R r2, r4
	imul r10, r12
	; IADD_RC r4, r5, 1789610138
	lea r12, [r12+r13+1789610138]
	; IADD_R r1, r4
	add r9, r12
	; IADD_R r6, r0
	add r14, r8
	; IXOR_R r7, r2
	xor r15, r10
	; ISMULH_M r6, L1[6816]
	mov rax, r14
	imul qword ptr [rsi+6816]
	mov r14, rdx
	; ISUB_R r0, r4
	sub r8, r12
	; IXOR_R r7, r2
	xor r15, r10
	; INEG_R r4
	neg r12
	; IROL_R r3, r0
	mov ecx, r8d
	rol r11, cl
	; IADD_RC r2, r5, -1667142135
	lea r10, [r10+r13-1667142135]
	; ISUB_R r6, r2
	sub r14, r10
	; IDIV_C r3, 2650709570
	mov rax, 3736177069856446853
	mul r11
	shr rdx, 29
	add r11, rdx
	; IMULH_R r3, r0
	mov rax, r11
	mul r8
	mov r11, rdx
	; FPSUB_R f0, a2
	subpd xmm0, xmm10
	; FPADD_M f3, L2[r4]
	mov eax, r12d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm3, xmm12
	; FPMUL_M e1, L1[r5]
	mov eax, r13d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	mulpd xmm5, xmm12
	maxpd xmm5, xmm13
	; IMUL_9C r7, -778247271
	lea r15, [r15+r15*8-778247271]
	; IXOR_R r4, 1846379510
	xor r12, 1846379510
	; COND_M r6, of(L1[r1], -397786451)
	xor ecx, ecx
	mov eax, r9d
	and eax, 16376
	cmp dword ptr [rsi+rax], -397786451
	seto cl
	add r14, rcx
	; COND_R r6, of(r3, -1033710571)
	xor ecx, ecx
	cmp r11d, -1033710571
	seto cl
	add r14, rcx
	; COND_M r6, sg(L1[r6], 1413230028)
	xor ecx, ecx
	mov eax, r14d
	and eax, 16376
	cmp dword ptr [rsi+rax], 1413230028
	sets cl
	add r14, rcx
	; IDIV_C r0, 2791108943
	mov rax, 1774119268816201525
	mul r8
	shr rdx, 28
	add r8, rdx
	; FPSUB_M f1, L1[r6]
	mov eax, r14d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm1, xmm12
	; FPSWAP_R f0
	shufpd xmm0, xmm0, 1
	; IADD_RC r6, r5, -640194892
	lea r14, [r14+r13-640194892]
	; FPADD_M f0, L1[r2]
	mov eax, r10d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm0, xmm12
	; IMUL_R r6, r5
	imul r14, r13
	; IROL_R r4, r1
	mov ecx, r9d
	rol r12, cl
	; FPDIV_R e2, a0
	divpd xmm6, xmm8
	maxpd xmm6, xmm13
	; IADD_RC r0, r2, -487084195
	lea r8, [r8+r10-487084195]
	; FPADD_R f0, a0
	addpd xmm0, xmm8
	; IXOR_R r5, r3
	xor r13, r11
	; IMUL_R r2, r4
	imul r10, r12
	; FPMUL_R e0, a0
	mulpd xmm4, xmm8
	; FPSUB_R f3, a3
	subpd xmm3, xmm11
	; IMUL_M r4, L1[4856]
	imul r12, qword ptr [rsi+4856]
	; IMUL_9C r2, 7951348
	lea r10, [r10+r10*8+7951348]
	; COND_R r3, ab(r7, 984532162)
	xor ecx, ecx
	cmp r15d, 984532162
	seta cl
	add r11, rcx
	; IXOR_M r7, L1[r4]
	mov eax, r12d
	and eax, 16376
	xor r15, qword ptr [rsi+rax]
	; IMUL_R r4, 248971329
	imul r12, 248971329
	; IXOR_R r3, r1
	xor r11, r9
	; IMUL_R r3, 2098482639
	imul r11, 2098482639
	; IXOR_R r6, r3
	xor r14, r11
	; IXOR_R r5, r4
	xor r13, r12
	; IADD_R r5, r4
	add r13, r12
	; IMUL_9C r7, 66530302
	lea r15, [r15+r15*8+66530302]
	; IMULH_R r0, r5
	mov rax, r8
	mul r13
	mov r8, rdx
	; IMUL_R r2, r7
	imul r10, r15
	; IMUL_R r1, 770985098
	imul r9, 770985098
	; COND_R r7, be(r5, 58538265)
	xor ecx, ecx
	cmp r13d, 58538265
	setbe cl
	add r15, rcx
	; IMUL_9C r3, 245704334
	lea r11, [r11+r11*8+245704334]
	; ISMULH_R r2, r4
	mov rax, r10
	imul r12
	mov r10, rdx
	; FPDIV_R e3, a3
	divpd xmm7, xmm11
	maxpd xmm7, xmm13
	; IMULH_R r5, r2
	mov rax, r13
	mul r10
	mov r13, rdx
	; ISUB_M r7, L1[r5]
	mov eax, r13d
	and eax, 16376
	sub r15, qword ptr [rsi+rax]
	; FPMUL_R e3, a3
	mulpd xmm7, xmm11
	; IMUL_R r3, r4
	imul r11, r12
	; FPSWAP_R f1
	shufpd xmm1, xmm1, 1
	; IMULH_R r1, 633797287
	mov eax, 633797287
	mul r9
	add r9, rdx
	; IADD_R r4, r3
	add r12, r11
	; IROR_R r2, r7
	mov ecx, r15d
	ror r10, cl
	; FPSUB_R f0, a2
	subpd xmm0, xmm10
	; FPSUB_R f2, a2
	subpd xmm2, xmm10
	; FPMUL_R e0, a2
	mulpd xmm4, xmm10
	; IMUL_M r4, L1[r3]
	mov eax, r11d
	and eax, 16376
	imul r12, qword ptr [rsi+rax]
	; IMUL_9C r1, -1901091890
	lea r9, [r9+r9*8-1901091890]
	; IROR_R r2, r6
	mov ecx, r14d
	ror r10, cl
	; IMULH_R r5, r3
	mov rax, r13
	mul r11
	mov r13, rdx
	; FPSUB_M f1, L1[r7]
	mov eax, r15d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm1, xmm12
	; IMUL_M r2, L1[r1]
	mov eax, r9d
	and eax, 16376
	imul r10, qword ptr [rsi+rax]
	; IMUL_R r6, r0
	imul r14, r8
	; IADD_R r7, r6
	add r15, r14
	; FPSUB_R f2, a3
	subpd xmm2, xmm11
	; COND_R r5, no(r2, -1589295370)
	xor ecx, ecx
	cmp r10d, -1589295370
	setno cl
	add r13, rcx
	; IMUL_9C r7, 420978486
	lea r15, [r15+r15*8+420978486]
	; IROL_R r4, r2
	mov ecx, r10d
	rol r12, cl
	; IMUL_9C r0, -1084530831
	lea r8, [r8+r8*8-1084530831]
	; FPNEG_R f3
	xorps xmm3, xmm15
	; IROR_R r6, r4
	mov ecx, r12d
	ror r14, cl
	; IROL_R r4, r5
	mov ecx, r13d
	rol r12, cl
	; FPSUB_R f2, a3
	subpd xmm2, xmm11
	; FPMUL_R e2, a2
	mulpd xmm6, xmm10
	; ISMULH_M r6, L2[98600]
	mov rax, r14
	imul qword ptr [rsi+98600]
	mov r14, rdx
	; IXOR_R r0, r6
	xor r8, r14
	; FPSWAP_R f1
	shufpd xmm1, xmm1, 1
	; FPADD_R f0, a1
	addpd xmm0, xmm9
	; COND_R r1, ab(r3, -991705199)
	xor ecx, ecx
	cmp r11d, -991705199
	seta cl
	add r9, rcx
	; IMULH_M r4, L2[r2]
	mov ecx, r10d
	and ecx, 262136
	mov rax, r12
	mul qword ptr [rsi+rcx]
	mov r12, rdx
	; IROR_R r2, r6
	mov ecx, r14d
	ror r10, cl
	; FPDIV_R e0, a1
	divpd xmm4, xmm9
	maxpd xmm4, xmm13
	; IMUL_R r1, r7
	imul r9, r15
	; COND_R r6, ns(r2, 939392855)
	xor ecx, ecx
	cmp r10d, 939392855
	setns cl
	add r14, rcx
	; FPMUL_R e3, a1
	mulpd xmm7, xmm9
	; COND_R r2, ab(r2, -499266314)
	xor ecx, ecx
	cmp r10d, -499266314
	seta cl
	add r10, rcx
	; COND_M r7, lt(L1[r1], -1624420482)
	xor ecx, ecx
	mov eax, r9d
	and eax, 16376
	cmp dword ptr [rsi+rax], -1624420482
	setl cl
	add r15, rcx
	; COND_R r1, lt(r1, 1525413977)
	xor ecx, ecx
	cmp r9d, 1525413977
	setl cl
	add r9, rcx
	; IMUL_R r4, r5
	imul r12, r13
	; IMUL_R r4, r2
	imul r12, r10
	; FPSQRT_R e1
	sqrtpd xmm5, xmm5
	; ISUB_R r2, r6
	sub r10, r14
	; FPDIV_R e1, a0
	divpd xmm5, xmm8
	maxpd xmm5, xmm13
	; FPMUL_R e2, a3
	mulpd xmm6, xmm11
	; IADD_R r6, 671627590
	add r14, 671627590
	; COND_M r6, sg(L1[r4], -780452820)
	xor ecx, ecx
	mov eax, r12d
	and eax, 16376
	cmp dword ptr [rsi+rax], -780452820
	sets cl
	add r14, rcx
	; IMULH_R r4, r7
	mov rax, r12
	mul r15
	mov r12, rdx
	; FPMUL_R e3, a1
	mulpd xmm7, xmm9
	; FPADD_R f0, a0
	addpd xmm0, xmm8
	; FPMUL_R e0, a1
	mulpd xmm4, xmm9
	; IMUL_R r7, r3
	imul r15, r11
	; IROL_R r0, r7
	mov ecx, r15d
	rol r8, cl
	; IMUL_R r1, r7
	imul r9, r15
	; COND_R r0, no(r7, 449007464)
	xor ecx, ecx
	cmp r15d, 449007464
	setno cl
	add r8, rcx
	; ISMULH_M r6, L2[134288]
	mov rax, r14
	imul qword ptr [rsi+134288]
	mov r14, rdx
	; IMULH_R r5, r2
	mov rax, r13
	mul r10
	mov r13, rdx
	; IMULH_R r7, r4
	mov rax, r15
	mul r12
	mov r15, rdx
	; FPDIV_R e3, a0
	divpd xmm7, xmm8
	maxpd xmm7, xmm13
	; IXOR_R r3, r4
	xor r11, r12
	; IDIV_C r1, 72349044
	mov rax, 8555331009525020641
	mul r9
	shr rdx, 25
	add r9, rdx
	; IADD_R r5, r4
	add r13, r12
	; IROR_R r2, r4
	mov ecx, r12d
	ror r10, cl
	; FPSUB_M f1, L1[r2]
	mov eax, r10d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm1, xmm12
	; FPMUL_R e2, a3
	mulpd xmm6, xmm11
	; IADD_R r5, r6
	add r13, r14
	; IXOR_M r1, L1[r4]
	mov eax, r12d
	and eax, 16376
	xor r9, qword ptr [rsi+rax]
	; ISUB_R r2, -1544880589
	sub r10, -1544880589
	; FPNEG_R f0
	xorps xmm0, xmm15
	; IROR_R r1, r6
	mov ecx, r14d
	ror r9, cl
	; IMUL_R r6, r4
	imul r14, r12
	; IMULH_M r4, L2[r1]
	mov ecx, r9d
	and ecx, 262136
	mov rax, r12
	mul qword ptr [rsi+rcx]
	mov r12, rdx
	; IXOR_R r3, r0
	xor r11, r8
	; FPSWAP_R f0
	shufpd xmm0, xmm0, 1
	; FPSWAP_R f0
	shufpd xmm0, xmm0, 1
	; COND_R r0, ns(r2, -308295242)
	xor ecx, ecx
	cmp r10d, -308295242
	setns cl
	add r8, rcx
	; IMUL_9C r1, 591587965
	lea r9, [r9+r9*8+591587965]
	; FPADD_R f3, a1
	addpd xmm3, xmm9
	; IMUL_R r5, r4
	imul r13, r12
	; IMUL_M r7, L1[r0]
	mov eax, r8d
	and eax, 16376
	imul r15, qword ptr [rsi+rax]
	; COND_R r6, sg(r5, -1119525789)
	xor ecx, ecx
	cmp r13d, -1119525789
	sets cl
	add r14, rcx
	; IMUL_M r0, L1[r1]
	mov eax, r9d
	and eax, 16376
	imul r8, qword ptr [rsi+rax]
	; IADD_M r3, L2[r7]
	mov eax, r15d
	and eax, 262136
	add r11, qword ptr [rsi+rax]
	; IADD_R r0, r1
	add r8, r9
	; FPSUB_R f2, a1
	subpd xmm2, xmm9
	; IXOR_M r0, L2[r7]
	mov eax, r15d
	and eax, 262136
	xor r8, qword ptr [rsi+rax]
	; COND_R r6, be(r6, 1481939391)
	xor ecx, ecx
	cmp r14d, 1481939391
	setbe cl
	add r14, rcx
	; FPADD_R f0, a1
	addpd xmm0, xmm9
	; IXOR_R r3, r2
	xor r11, r10
	; FPSUB_R f0, a1
	subpd xmm0, xmm9
	; IXOR_R r7, r3
	xor r15, r11
	; IXOR_M r6, L1[r4]
	mov eax, r12d
	and eax, 16376
	xor r14, qword ptr [rsi+rax]
	; IMULH_R r2, r7
	mov rax, r10
	mul r15
	mov r10, rdx
	; ISUB_R r5, r1
	sub r13, r9
	; FPMUL_R e1, a3
	mulpd xmm5, xmm11
	; FPADD_R f3, a2
	addpd xmm3, xmm10
	; FPSWAP_R f1
	shufpd xmm1, xmm1, 1
	; FPSUB_R f1, a3
	subpd xmm1, xmm11
	; FPSUB_M f0, L1[r4]
	mov eax, r12d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm0, xmm12
	; FPMUL_R e1, a2
	mulpd xmm5, xmm10
	; FPADD_R f3, a0
	addpd xmm3, xmm8
	; IROL_R r2, r4
	mov ecx, r12d
	rol r10, cl
	; COND_M r7, ab(L2[r7], -2012390318)
	xor ecx, ecx
	mov eax, r15d
	and eax, 262136
	cmp dword ptr [rsi+rax], -2012390318
	seta cl
	add r15, rcx
	; IMUL_9C r4, -38079585
	lea r12, [r12+r12*8-38079585]
	; IXOR_R r0, r1
	xor r8, r9
	; FPMUL_R e1, a3
	mulpd xmm5, xmm11
	; FPMUL_R e1, a1
	mulpd xmm5, xmm9
	; FPSUB_R f1, a2
	subpd xmm1, xmm10
	; IMUL_9C r4, -847745598
	lea r12, [r12+r12*8-847745598]
	; FPSQRT_R e1
	sqrtpd xmm5, xmm5
	; IADD_R r7, r6
	add r15, r14
	; FPSUB_R f3, a0
	subpd xmm3, xmm8
	; FPSUB_R f1, a1
	subpd xmm1, xmm9
	; IADD_R r7, r6
	add r15, r14
	; IROL_R r2, r5
	mov ecx, r13d
	rol r10, cl
	; IADD_RC r4, r2, 1338806320
	lea r12, [r12+r10+1338806320]
	; FPSQRT_R e3
	sqrtpd xmm7, xmm7
	; IMUL_R r5, r0
	imul r13, r8
	; FPADD_R f2, a1
	addpd xmm2, xmm9
	; INEG_R r6
	neg r14
	; IXOR_M r6, L1[r2]
	mov eax, r10d
	and eax, 16376
	xor r14, qword ptr [rsi+rax]
	; FPSUB_R f2, a2
	subpd xmm2, xmm10
	; FPADD_R f2, a2
	addpd xmm2, xmm10
	; FPADD_R f1, a2
	addpd xmm1, xmm10
	; COND_R r3, be(r4, 174667458)
	xor ecx, ecx
	cmp r12d, 174667458
	setbe cl
	add r11, rcx
	; INEG_R r6
	neg r14
	; IXOR_R r6, r3
	xor r14, r11
	; COND_M r5, sg(L1[r0], -864345921)
	xor ecx, ecx
	mov eax, r8d
	and eax, 16376
	cmp dword ptr [rsi+rax], -864345921
	sets cl
	add r13, rcx
	; IROL_R r7, r3
	mov ecx, r11d
	rol r15, cl
	; FPSUB_R f1, a2
	subpd xmm1, xmm10
	; IADD_M r1, L1[r0]
	mov eax, r8d
	and eax, 16376
	add r9, qword ptr [rsi+rax]
	; IMULH_R r1, r3
	mov rax, r9
	mul r11
	mov r9, rdx
	; IMUL_R r0, -1489192296
	imul r8, -1489192296
	; FPMUL_R e0, a2
	mulpd xmm4, xmm10
	; COND_R r1, ge(r1, -1358904097)
	xor ecx, ecx
	cmp r9d, -1358904097
	setge cl
	add r9, rcx
	; FPSUB_R f1, a1
	subpd xmm1, xmm9
	; FPADD_R f2, a3
	addpd xmm2, xmm11
	; IROR_R r4, r7
	mov ecx, r15d
	ror r12, cl
	; ISDIV_C r1, -1368098113
	mov rax, -7238896260565957085
	imul r9
	xor eax, eax
	sar rdx, 29
	sets al
	add rdx, rax
	add r9, rdx
	; IADD_M r4, L1[r1]
	mov eax, r9d
	and eax, 16376
	add r12, qword ptr [rsi+rax]
	; IMUL_R r0, -1011605520
	imul r8, -1011605520
	; FPSUB_R f3, a1
	subpd xmm3, xmm9
	; IADD_RC r1, r4, 272540736
	lea r9, [r9+r12+272540736]
	; FPSWAP_R f2
	shufpd xmm2, xmm2, 1
	; IROR_R r3, r2
	mov ecx, r10d
	ror r11, cl
	; IMUL_R r3, 2085105439
	imul r11, 2085105439
	; FPMUL_R e0, a0
	mulpd xmm4, xmm8
	; IMUL_9C r6, -483723153
	lea r14, [r14+r14*8-483723153]
	; FPSUB_M f3, L1[r7]
	mov eax, r15d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm3, xmm12
	; IMUL_R r3, r2
	imul r11, r10
	; ISMULH_R r7, r1
	mov rax, r15
	imul r9
	mov r15, rdx
	; COND_R r1, of(r7, 778804236)
	xor ecx, ecx
	cmp r15d, 778804236
	seto cl
	add r9, rcx
	; FPSUB_R f3, a2
	subpd xmm3, xmm10
	; IROL_R r5, r7
	mov ecx, r15d
	rol r13, cl
	; FPADD_R f1, a0
	addpd xmm1, xmm8
	; FPADD_R f2, a3
	addpd xmm2, xmm11
	; IMUL_R r6, r0
	imul r14, r8
	; ISUB_M r2, L2[r4]
	mov eax, r12d
	and eax, 262136
	sub r10, qword ptr [rsi+rax]
	; IXOR_R r0, r6
	xor r8, r14
	; INEG_R r6
	neg r14
	; FPMUL_R e2, a3
	mulpd xmm6, xmm11
	; IADD_RC r4, r6, -1312075035
	lea r12, [r12+r14-1312075035]
	; IMUL_R r1, r5
	imul r9, r13
	; IXOR_M r7, L2[r6]
	mov eax, r14d
	and eax, 262136
	xor r15, qword ptr [rsi+rax]
	; IROR_R r2, 23
	ror r10, 23
	; FPMUL_R e0, a2
	mulpd xmm4, xmm10
	; ISMULH_M r5, L1[r2]
	mov ecx, r10d
	and ecx, 16376
	mov rax, r13
	imul qword ptr [rsi+rcx]
	mov r13, rdx
	; ISUB_M r7, L1[r4]
	mov eax, r12d
	and eax, 16376
	sub r15, qword ptr [rsi+rax]
	; COND_R r0, sg(r2, 1538841628)
	xor ecx, ecx
	cmp r10d, 1538841628
	sets cl
	add r8, rcx
	; IMUL_R r6, r2
	imul r14, r10
	; ISUB_R r0, r1
	sub r8, r9
	; IMUL_R r5, r7
	imul r13, r15
	; IADD_RC r1, r0, 516706834
	lea r9, [r9+r8+516706834]
	; INEG_R r5
	neg r13
	; FPSQRT_R e3
	sqrtpd xmm7, xmm7
	; IADD_RC r5, r4, -1679394922
	lea r13, [r13+r12-1679394922]
	; FPSUB_R f1, a1
	subpd xmm1, xmm9
	; IMUL_R r0, r2
	imul r8, r10
	; ISUB_R r3, r2
	sub r11, r10
	; FPDIV_R e0, a3
	divpd xmm4, xmm11
	maxpd xmm4, xmm13
	; ISUB_R r1, r5
	sub r9, r13
	; COND_M r2, be(L2[r2], 1840094725)
	xor ecx, ecx
	mov eax, r10d
	and eax, 262136
	cmp dword ptr [rsi+rax], 1840094725
	setbe cl
	add r10, rcx
	; IMUL_M r6, L1[r7]
	mov eax, r15d
	and eax, 16376
	imul r14, qword ptr [rsi+rax]
	; IMULH_M r6, L1[r5]
	mov ecx, r13d
	and ecx, 16376
	mov rax, r14
	mul qword ptr [rsi+rcx]
	mov r14, rdx
	; IMUL_9C r7, -1048659408
	lea r15, [r15+r15*8-1048659408]
	; IMUL_R r6, r3
	imul r14, r11
	; FPADD_R f3, a0
	addpd xmm3, xmm8
	; IMULH_R r0, r3
	mov rax, r8
	mul r11
	mov r8, rdx
	; FPSWAP_R f0
	shufpd xmm0, xmm0, 1
	; FPSQRT_R e3
	sqrtpd xmm7, xmm7
	; IMULH_R r2, r0
	mov rax, r10
	mul r8
	mov r10, rdx
	; FPDIV_R e1, a1
	divpd xmm5, xmm9
	maxpd xmm5, xmm13
