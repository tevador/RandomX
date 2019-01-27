	; FPMUL_R e0, a2
	mulpd xmm4, xmm10
	; IADD_RC r2, r5, -1621224194
	lea r10, [r10+r13-1621224194]
	; ISTORE L2[r2], r7
	mov eax, r10d
	and eax, 262136
	mov qword ptr [rsi+rax], r15
	; FPMUL_R e2, a2
	mulpd xmm6, xmm10
	; IMUL_R r6, r3
	imul r14, r11
	; FPMUL_R e1, a0
	mulpd xmm5, xmm8
	; IROR_R r5, r3
	mov ecx, r11d
	ror r13, cl
	; FPMUL_R e2, a0
	mulpd xmm6, xmm8
	; FPNEG_R f3
	xorps xmm3, xmm15
	; IXOR_R r0, r4
	xor r8, r12
	; ISMULH_R r3, r7
	mov rax, r11
	imul r15
	mov r11, rdx
	; FPSWAP_R f2
	shufpd xmm2, xmm2, 1
	; ISMULH_R r6, r0
	mov rax, r14
	imul r8
	mov r14, rdx
	; FPMUL_R e0, a2
	mulpd xmm4, xmm10
	; ISUB_R r3, r4
	sub r11, r12
	; IADD_R r7, -1138617760
	add r15, -1138617760
	; IROR_R r2, r6
	mov ecx, r14d
	ror r10, cl
	; FPMUL_R e2, a1
	mulpd xmm6, xmm9
	; IROR_R r7, r1
	mov ecx, r9d
	ror r15, cl
	; COND_M r2, lt(L1[r7], -41618808)
	xor ecx, ecx
	mov eax, r15d
	and eax, 16376
	cmp dword ptr [rsi+rax], -41618808
	setl cl
	add r10, rcx
	; FPMUL_M e3, L1[r0]
	mov eax, r8d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	mulpd xmm7, xmm12
	maxpd xmm7, xmm13
	; CFROUND r1, 43
	mov rax, r9
	rol rax, 34
	and eax, 24576
	or eax, 40896
	mov dword ptr [rsp-8], eax
	ldmxcsr dword ptr [rsp-8]
	; FPADD_R f2, a1
	addpd xmm2, xmm9
	; FPNEG_R f0
	xorps xmm0, xmm15
	; FSTORE L1[r6], f2
	mov eax, r14d
	and eax, 16368
	movapd xmmword ptr [rsi+rax], xmm2
	; IMUL_9C r6, -45112665
	lea r14, [r14+r14*8-45112665]
	; IADD_M r0, L1[r4]
	mov eax, r12d
	and eax, 16376
	add r8, qword ptr [rsi+rax]
	; ISTORE L1[r4], r3
	mov eax, r12d
	and eax, 16376
	mov qword ptr [rsi+rax], r11
	; ISTORE L1[r6], r6
	mov eax, r14d
	and eax, 16376
	mov qword ptr [rsi+rax], r14
	; COND_R r4, sg(r1, -1189096105)
	xor ecx, ecx
	cmp r9d, -1189096105
	sets cl
	add r12, rcx
	; IXOR_R r2, r5
	xor r10, r13
	; COND_R r1, be(r5, -965180434)
	xor ecx, ecx
	cmp r13d, -965180434
	setbe cl
	add r9, rcx
	; FPMUL_M e1, L2[r3]
	mov eax, r11d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	mulpd xmm5, xmm12
	maxpd xmm5, xmm13
	; IMULH_R r7, r6
	mov rax, r15
	mul r14
	mov r15, rdx
	; ISMULH_M r0, L1[r4]
	mov ecx, r12d
	and ecx, 16376
	mov rax, r8
	imul qword ptr [rsi+rcx]
	mov r8, rdx
	; IMUL_R r5, r3
	imul r13, r11
	; COND_R r2, of(r0, -1045938770)
	xor ecx, ecx
	cmp r8d, -1045938770
	seto cl
	add r10, rcx
	; FPADD_M f3, L1[r4]
	mov eax, r12d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm3, xmm12
	; IADD_R r3, r2
	add r11, r10
	; FPADD_R f1, a0
	addpd xmm1, xmm8
	; FPSQRT_R e3
	sqrtpd xmm7, xmm7
	; FPSUB_R f0, a1
	subpd xmm0, xmm9
	; IMUL_M r5, L1[r6]
	mov eax, r14d
	and eax, 16376
	imul r13, qword ptr [rsi+rax]
	; ISUB_R r1, r2
	sub r9, r10
	; IMUL_R r4, r6
	imul r12, r14
	; FPSWAP_R e3
	shufpd xmm7, xmm7, 1
	; IMUL_M r0, L1[r7]
	mov eax, r15d
	and eax, 16376
	imul r8, qword ptr [rsi+rax]
	; IROR_R r1, r6
	mov ecx, r14d
	ror r9, cl
	; IROR_R r2, r4
	mov ecx, r12d
	ror r10, cl
	; FPSUB_R f3, a1
	subpd xmm3, xmm9
	; FSTORE L1[r0], e1
	mov eax, r8d
	and eax, 16368
	movapd xmmword ptr [rsi+rax], xmm5
	; COND_R r2, sg(r3, 1269153133)
	xor ecx, ecx
	cmp r11d, 1269153133
	sets cl
	add r10, rcx
	; FPSWAP_R f2
	shufpd xmm2, xmm2, 1
	; IADD_R r7, r5
	add r15, r13
	; COND_R r0, be(r4, -1486502150)
	xor ecx, ecx
	cmp r12d, -1486502150
	setbe cl
	add r8, rcx
	; FPSUB_R f3, a1
	subpd xmm3, xmm9
	; FPADD_R f0, a3
	addpd xmm0, xmm11
	; IADD_R r2, r0
	add r10, r8
	; FSTORE L1[r3], e2
	mov eax, r11d
	and eax, 16368
	movapd xmmword ptr [rsi+rax], xmm6
	; IXOR_R r1, r7
	xor r9, r15
	; IMUL_R r5, r7
	imul r13, r15
	; IXOR_R r7, 266992378
	xor r15, 266992378
	; COND_R r7, no(r4, 1983804692)
	xor ecx, ecx
	cmp r12d, 1983804692
	setno cl
	add r15, rcx
	; IMUL_M r2, L2[r0]
	mov eax, r8d
	and eax, 262136
	imul r10, qword ptr [rsi+rax]
	; FPDIV_R e3, a2
	divpd xmm7, xmm10
	maxpd xmm7, xmm13
	; IMUL_M r0, L2[r6]
	mov eax, r14d
	and eax, 262136
	imul r8, qword ptr [rsi+rax]
	; ISTORE L1[r0], r7
	mov eax, r8d
	and eax, 16376
	mov qword ptr [rsi+rax], r15
	; FPMUL_R e0, a1
	mulpd xmm4, xmm9
	; FPSUB_R f3, a1
	subpd xmm3, xmm9
	; IROR_R r5, r4
	mov ecx, r12d
	ror r13, cl
	; ISTORE L2[r7], r2
	mov eax, r15d
	and eax, 262136
	mov qword ptr [rsi+rax], r10
	; FPSWAP_R e2
	shufpd xmm6, xmm6, 1
	; FPADD_M f3, L1[r2]
	mov eax, r10d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm3, xmm12
	; IDIV_C r5, 2218798981
	mov rax, 17853839665672790751
	mul r13
	shr rdx, 31
	add r13, rdx
	; IADD_RC r0, r4, -1321374359
	lea r8, [r8+r12-1321374359]
	; CFROUND r6, 28
	mov rax, r14
	rol rax, 49
	and eax, 24576
	or eax, 40896
	mov dword ptr [rsp-8], eax
	ldmxcsr dword ptr [rsp-8]
	; FPADD_R f2, a2
	addpd xmm2, xmm10
	; IROL_R r7, r6
	mov ecx, r14d
	rol r15, cl
	; ISUB_R r2, r4
	sub r10, r12
	; IMULH_M r0, L1[12400]
	mov rax, r8
	mul qword ptr [rsi+12400]
	mov r8, rdx
	; IADD_R r2, r3
	add r10, r11
	; COND_R r6, lt(r1, -1124202227)
	xor ecx, ecx
	cmp r9d, -1124202227
	setl cl
	add r14, rcx
	; IROR_R r7, r4
	mov ecx, r12d
	ror r15, cl
	; IMUL_R r4, r2
	imul r12, r10
	; ISUB_R r3, r7
	sub r11, r15
	; IADD_R r2, r7
	add r10, r15
	; FPSQRT_R e3
	sqrtpd xmm7, xmm7
	; ISUB_R r6, 540663146
	sub r14, 540663146
	; IROL_R r5, 58
	rol r13, 58
	; FPADD_R f2, a1
	addpd xmm2, xmm9
	; FPADD_R f2, a2
	addpd xmm2, xmm10
	; FPSQRT_R e1
	sqrtpd xmm5, xmm5
	; FPADD_R f1, a2
	addpd xmm1, xmm10
	; IADD_R r5, r3
	add r13, r11
	; IADD_M r7, L1[880]
	add r15, qword ptr [rsi+880]
	; ISUB_R r7, r0
	sub r15, r8
	; ISTORE L2[r0], r7
	mov eax, r8d
	and eax, 262136
	mov qword ptr [rsi+rax], r15
	; IDIV_C r2, 1014940364
	mov rax, r10
	shr rax, 2
	mov rcx, 1219717022984988185
	mul rcx
	shr rdx, 24
	add r10, rdx
	; FPMUL_R e0, a2
	mulpd xmm4, xmm10
	; IDIV_C r2, 3059159304
	mov rax, 12949335853590502915
	mul r10
	shr rdx, 31
	add r10, rdx
	; IADD_R r0, r3
	add r8, r11
	; IMUL_9C r7, -2124093035
	lea r15, [r15+r15*8-2124093035]
	; FPSUB_R f2, a0
	subpd xmm2, xmm8
	; FPDIV_R e0, a2
	divpd xmm4, xmm10
	maxpd xmm4, xmm13
	; FPSUB_R f2, a3
	subpd xmm2, xmm11
	; IMUL_R r1, r2
	imul r9, r10
	; ISMULH_R r7, r5
	mov rax, r15
	imul r13
	mov r15, rdx
	; IMULH_R r3, r2
	mov rax, r11
	mul r10
	mov r11, rdx
	; IXOR_M r1, L2[r0]
	mov eax, r8d
	and eax, 262136
	xor r9, qword ptr [rsi+rax]
	; FPMUL_R e0, a1
	mulpd xmm4, xmm9
	; ISUB_R r4, 1456841848
	sub r12, 1456841848
	; IXOR_M r3, L2[r2]
	mov eax, r10d
	and eax, 262136
	xor r11, qword ptr [rsi+rax]
	; COND_M r0, of(L1[r4], 1678513610)
	xor ecx, ecx
	mov eax, r12d
	and eax, 16376
	cmp dword ptr [rsi+rax], 1678513610
	seto cl
	add r8, rcx
	; IDIV_C r4, 2674394209
	mov rax, 925772300223658071
	mul r12
	shr rdx, 27
	add r12, rdx
	; IMUL_R r4, r1
	imul r12, r9
	; FPADD_R f1, a2
	addpd xmm1, xmm10
	; FPSUB_R f2, a0
	subpd xmm2, xmm8
	; FPMUL_M e1, L2[r6]
	mov eax, r14d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	mulpd xmm5, xmm12
	maxpd xmm5, xmm13
	; FPSUB_M f0, L2[r3]
	mov eax, r11d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm0, xmm12
	; IROR_R r0, r7
	mov ecx, r15d
	ror r8, cl
	; FSTORE L2[r1], e0
	mov eax, r9d
	and eax, 262128
	movapd xmmword ptr [rsi+rax], xmm4
	; IROR_R r7, r6
	mov ecx, r14d
	ror r15, cl
	; IMUL_9C r2, 266593902
	lea r10, [r10+r10*8+266593902]
	; IMUL_R r4, r6
	imul r12, r14
	; FPSUB_R f2, a2
	subpd xmm2, xmm10
	; FPMUL_R e3, a0
	mulpd xmm7, xmm8
	; IXOR_M r7, L1[r2]
	mov eax, r10d
	and eax, 16376
	xor r15, qword ptr [rsi+rax]
	; IROR_R r0, r5
	mov ecx, r13d
	ror r8, cl
	; FPADD_R f1, a2
	addpd xmm1, xmm10
	; FPSQRT_R e3
	sqrtpd xmm7, xmm7
	; FPADD_R f3, a1
	addpd xmm3, xmm9
	; FPADD_R f1, a0
	addpd xmm1, xmm8
	; COND_M r2, ge(L2[r2], -226330940)
	xor ecx, ecx
	mov eax, r10d
	and eax, 262136
	cmp dword ptr [rsi+rax], -226330940
	setge cl
	add r10, rcx
	; FPDIV_R e2, a3
	divpd xmm6, xmm11
	maxpd xmm6, xmm13
	; FPMUL_R e2, a1
	mulpd xmm6, xmm9
	; FPSUB_R f1, a0
	subpd xmm1, xmm8
	; IMUL_R r7, r5
	imul r15, r13
	; IMUL_R r0, r1
	imul r8, r9
	; FPSUB_R f3, a1
	subpd xmm3, xmm9
	; IROL_R r3, r5
	mov ecx, r13d
	rol r11, cl
	; IADD_RC r5, r2, 795784298
	lea r13, [r13+r10+795784298]
	; ISUB_R r0, r4
	sub r8, r12
	; IMUL_R r5, r4
	imul r13, r12
	; FPSUB_R f0, a2
	subpd xmm0, xmm10
	; FPMUL_R e3, a1
	mulpd xmm7, xmm9
	; ISDIV_C r3, 1662492575
	mov rax, 2978515652703905219
	imul r11
	xor eax, eax
	sar rdx, 28
	sets al
	add rdx, rax
	add r11, rdx
	; ISMULH_R r5, r0
	mov rax, r13
	imul r8
	mov r13, rdx
	; ISDIV_C r4, 1963597892
	mov rax, -8359627607928540073
	imul r12
	xor eax, eax
	add rdx, r12
	sar rdx, 30
	sets al
	add rdx, rax
	add r12, rdx
	; IMUL_R r7, r0
	imul r15, r8
	; IMULH_M r0, L1[r3]
	mov ecx, r11d
	and ecx, 16376
	mov rax, r8
	mul qword ptr [rsi+rcx]
	mov r8, rdx
	; IXOR_R r3, r7
	xor r11, r15
	; IDIV_C r4, 1146125335
	mov rax, 8640870253760721727
	mul r12
	shr rdx, 29
	add r12, rdx
	; FPSWAP_R f3
	shufpd xmm3, xmm3, 1
	; IXOR_M r2, L1[r0]
	mov eax, r8d
	and eax, 16376
	xor r10, qword ptr [rsi+rax]
	; IROR_R r0, r1
	mov ecx, r9d
	ror r8, cl
	; IXOR_R r7, r4
	xor r15, r12
	; ISMULH_R r6, r2
	mov rax, r14
	imul r10
	mov r14, rdx
	; FPMUL_R e3, a2
	mulpd xmm7, xmm10
	; IADD_RC r4, r2, 1704868083
	lea r12, [r12+r10+1704868083]
	; FPSUB_R f2, a0
	subpd xmm2, xmm8
	; ISTORE L1[r0], r0
	mov eax, r8d
	and eax, 16376
	mov qword ptr [rsi+rax], r8
	; FPSUB_R f0, a3
	subpd xmm0, xmm11
	; FPDIV_R e0, a3
	divpd xmm4, xmm11
	maxpd xmm4, xmm13
	; FPMUL_R e3, a2
	mulpd xmm7, xmm10
	; ISUB_R r7, 1302457878
	sub r15, 1302457878
	; IMUL_9C r1, 1330165941
	lea r9, [r9+r9*8+1330165941]
	; FPMUL_R e1, a3
	mulpd xmm5, xmm11
	; IROL_R r0, r4
	mov ecx, r12d
	rol r8, cl
	; FPSUB_M f1, L1[r0]
	mov eax, r8d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm1, xmm12
	; IROL_R r5, r6
	mov ecx, r14d
	rol r13, cl
	; COND_M r0, ab(L1[r1], -310933871)
	xor ecx, ecx
	mov eax, r9d
	and eax, 16376
	cmp dword ptr [rsi+rax], -310933871
	seta cl
	add r8, rcx
	; CFROUND r7, 39
	mov rax, r15
	rol rax, 38
	and eax, 24576
	or eax, 40896
	mov dword ptr [rsp-8], eax
	ldmxcsr dword ptr [rsp-8]
	; FPDIV_R e0, a1
	divpd xmm4, xmm9
	maxpd xmm4, xmm13
	; IMUL_M r1, L1[r3]
	mov eax, r11d
	and eax, 16376
	imul r9, qword ptr [rsi+rax]
	; IMUL_9C r3, 1573236728
	lea r11, [r11+r11*8+1573236728]
	; FPNEG_R f3
	xorps xmm3, xmm15
	; COND_R r1, lt(r4, -1805702334)
	xor ecx, ecx
	cmp r12d, -1805702334
	setl cl
	add r9, rcx
	; FPSWAP_R f1
	shufpd xmm1, xmm1, 1
	; IADD_R r7, -1421188024
	add r15, -1421188024
	; FPMUL_R e3, a2
	mulpd xmm7, xmm10
	; FPSUB_M f2, L2[r7]
	mov eax, r15d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm2, xmm12
	; FPSUB_R f3, a1
	subpd xmm3, xmm9
	; FPSQRT_R e1
	sqrtpd xmm5, xmm5
	; ISUB_R r2, r4
	sub r10, r12
	; ISMULH_R r4, r5
	mov rax, r12
	imul r13
	mov r12, rdx
	; COND_R r1, of(r7, 1294727006)
	xor ecx, ecx
	cmp r15d, 1294727006
	seto cl
	add r9, rcx
	; IADD_M r5, L2[r2]
	mov eax, r10d
	and eax, 262136
	add r13, qword ptr [rsi+rax]
	; IMUL_9C r4, 401020510
	lea r12, [r12+r12*8+401020510]
	; IROL_R r3, r0
	mov ecx, r8d
	rol r11, cl
	; ISTORE L1[r7], r0
	mov eax, r15d
	and eax, 16376
	mov qword ptr [rsi+rax], r8
	; FPSUB_R f2, a1
	subpd xmm2, xmm9
	; FPSQRT_R e3
	sqrtpd xmm7, xmm7
	; IMUL_R r3, 720965215
	imul r11, 720965215
	; IMUL_R r6, r2
	imul r14, r10
	; ISTORE L1[r7], r3
	mov eax, r15d
	and eax, 16376
	mov qword ptr [rsi+rax], r11
	; IROR_R r2, r6
	mov ecx, r14d
	ror r10, cl
	; FPSQRT_R e3
	sqrtpd xmm7, xmm7
	; IMUL_9C r4, 788211341
	lea r12, [r12+r12*8+788211341]
	; IMUL_9C r3, -67993446
	lea r11, [r11+r11*8-67993446]
	; FPSWAP_R e3
	shufpd xmm7, xmm7, 1
	; IMUL_M r2, L1[r6]
	mov eax, r14d
	and eax, 16376
	imul r10, qword ptr [rsi+rax]
	; COND_M r2, ge(L1[r2], -1892157506)
	xor ecx, ecx
	mov eax, r10d
	and eax, 16376
	cmp dword ptr [rsi+rax], -1892157506
	setge cl
	add r10, rcx
	; FPADD_M f1, L1[r3]
	mov eax, r11d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm1, xmm12
	; IADD_M r7, L1[r0]
	mov eax, r8d
	and eax, 16376
	add r15, qword ptr [rsi+rax]
	; ISDIV_C r1, 624867857
	mov rax, 7924491717200811467
	imul r9
	xor eax, eax
	sar rdx, 28
	sets al
	add rdx, rax
	add r9, rdx
	; FPADD_R f0, a1
	addpd xmm0, xmm9
	; ISUB_R r5, r7
	sub r13, r15
	; FPNEG_R f0
	xorps xmm0, xmm15
	; IMUL_R r6, r2
	imul r14, r10
	; FPMUL_M e3, L1[r1]
	mov eax, r9d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	mulpd xmm7, xmm12
	maxpd xmm7, xmm13
	; IADD_R r0, r4
	add r8, r12
	; FPSUB_M f3, L1[r1]
	mov eax, r9d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm3, xmm12
	; FPMUL_R e2, a0
	mulpd xmm6, xmm8
	; INEG_R r2
	neg r10
	; FPMUL_R e2, a2
	mulpd xmm6, xmm10
	; FPSUB_M f3, L1[r6]
	mov eax, r14d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm3, xmm12
	; FPADD_R f1, a3
	addpd xmm1, xmm11
	; IMULH_R r3, r2
	mov rax, r11
	mul r10
	mov r11, rdx
	; FPSUB_R f0, a3
	subpd xmm0, xmm11
	; IDIV_C r5, 2887845607
	mov rax, 13717520480010955377
	mul r13
	shr rdx, 31
	add r13, rdx
	; ISMULH_M r6, L1[r2]
	mov ecx, r10d
	and ecx, 16376
	mov rax, r14
	imul qword ptr [rsi+rcx]
	mov r14, rdx
	; FPSUB_R f3, a3
	subpd xmm3, xmm11
	; IMUL_M r6, L1[r7]
	mov eax, r15d
	and eax, 16376
	imul r14, qword ptr [rsi+rax]
	; FPNEG_R f0
	xorps xmm0, xmm15
	; FPMUL_R e2, a0
	mulpd xmm6, xmm8
	; IMUL_9C r6, 295130073
	lea r14, [r14+r14*8+295130073]
	; FPADD_R f1, a1
	addpd xmm1, xmm9
	; IXOR_R r0, r5
	xor r8, r13
	; FPADD_R f2, a1
	addpd xmm2, xmm9
	; FPSWAP_R e3
	shufpd xmm7, xmm7, 1
	; FPSQRT_R e3
	sqrtpd xmm7, xmm7
	; IADD_RC r3, r6, -1317630728
	lea r11, [r11+r14-1317630728]
	; IMUL_M r2, L1[r3]
	mov eax, r11d
	and eax, 16376
	imul r10, qword ptr [rsi+rax]
	; IADD_RC r1, r4, 894105694
	lea r9, [r9+r12+894105694]
	; IMUL_R r7, r0
	imul r15, r8
	; FPSUB_R f1, a0
	subpd xmm1, xmm8
	; IMUL_M r7, L1[r1]
	mov eax, r9d
	and eax, 16376
	imul r15, qword ptr [rsi+rax]
	; IXOR_R r2, r4
	xor r10, r12
	; ISUB_M r0, L1[r1]
	mov eax, r9d
	and eax, 16376
	sub r8, qword ptr [rsi+rax]
	; INEG_R r4
	neg r12
	; IMUL_9C r4, -285272388
	lea r12, [r12+r12*8-285272388]
	; IMUL_R r7, r4
	imul r15, r12
	; IMULH_M r5, L1[r7]
	mov ecx, r15d
	and ecx, 16376
	mov rax, r13
	mul qword ptr [rsi+rcx]
	mov r13, rdx
	; IROL_R r1, r7
	mov ecx, r15d
	rol r9, cl
	; IXOR_R r4, -757532727
	xor r12, -757532727
	; IMUL_R r3, 1863959234
	imul r11, 1863959234
	; IROL_R r4, 59
	rol r12, 59
	; ISMULH_R r1, 2122681086
	mov rax, 2122681086
	imul r9
	add r9, rdx
	; ISTORE L2[r6], r7
	mov eax, r14d
	and eax, 262136
	mov qword ptr [rsi+rax], r15
	; ISTORE L1[r1], r5
	mov eax, r9d
	and eax, 16376
	mov qword ptr [rsi+rax], r13
	; FPMUL_R e0, a1
	mulpd xmm4, xmm9
	; COND_R r2, ns(r1, 486049737)
	xor ecx, ecx
	cmp r9d, 486049737
	setns cl
	add r10, rcx
	; FPMUL_M e0, L2[r7]
	mov eax, r15d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	mulpd xmm4, xmm12
	maxpd xmm4, xmm13
	; FPMUL_R e3, a2
	mulpd xmm7, xmm10
	; IROL_R r5, r2
	mov ecx, r10d
	rol r13, cl
	; IADD_M r0, L1[r4]
	mov eax, r12d
	and eax, 16376
	add r8, qword ptr [rsi+rax]
