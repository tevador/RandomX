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
	; FPSUB_M f1, L1[r4]
	mov eax, r12d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm1, xmm12
	; IROL_R r5, r3
	mov ecx, r11d
	rol r13, cl
	; FPMUL_R e2, a0
	mulpd xmm6, xmm8
	; FPSUB_R f3, a0
	subpd xmm3, xmm8
	; IXOR_R r0, r4
	xor r8, r12
	; ISMULH_M r3, L1[r7]
	mov ecx, r15d
	and ecx, 16376
	mov rax, r11
	imul qword ptr [rsi+rcx]
	mov r11, rdx
	; FPSWAP_R f2
	shufpd xmm2, xmm2, 1
	; IDIV_C r6, 1248528248
	mov rax, 15864311168205210203
	mul r14
	shr rdx, 30
	add r14, rdx
	; FPMUL_R e0, a2
	mulpd xmm4, xmm10
	; IADD_RC r3, r4, -52260428
	lea r11, [r11+r12-52260428]
	; IADD_R r7, -1138617760
	add r15, -1138617760
	; IROL_R r2, r6
	mov ecx, r14d
	rol r10, cl
	; FPNEG_R f2
	xorps xmm2, xmm15
	; IROR_R r7, r1
	mov ecx, r9d
	ror r15, cl
	; COND_R r2, lt(r7, -41618808)
	xor ecx, ecx
	cmp r15d, -41618808
	setl cl
	add r10, rcx
	; FPMUL_R e3, a0
	mulpd xmm7, xmm8
	; CFROUND r1, 43
	mov rax, r9
	rol rax, 34
	and eax, 24576
	or eax, 40896
	mov dword ptr [rsp-8], eax
	ldmxcsr dword ptr [rsp-8]
	; FPADD_R f2, a1
	addpd xmm2, xmm9
	; FPSUB_M f0, L1[r7]
	mov eax, r15d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm0, xmm12
	; ISTORE L1[r6], r2
	mov eax, r14d
	and eax, 16376
	mov qword ptr [rsi+rax], r10
	; ISUB_R r6, r5
	sub r14, r13
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
	; FPSQRT_R e0
	sqrtpd xmm4, xmm4
	; IXOR_R r2, r5
	xor r10, r13
	; FPSQRT_R e1
	sqrtpd xmm5, xmm5
	; FPMUL_R e1, a3
	mulpd xmm5, xmm11
	; IMULH_R r7, r6
	mov rax, r15
	mul r14
	mov r15, rdx
	; ISDIV_C r0, -1706892622
	mov rax, -5802075764249827661
	imul r8
	xor eax, eax
	sar rdx, 29
	sets al
	add rdx, rax
	add r8, rdx
	; IMUL_R r5, r3
	imul r13, r11
	; FPSQRT_R e2
	sqrtpd xmm6, xmm6
	; FPADD_M f3, L1[r4]
	mov eax, r12d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm3, xmm12
	; IADD_R r3, r2
	add r11, r10
	; FPADD_R f1, a0
	addpd xmm1, xmm8
	; FPDIV_R e3, a2
	divpd xmm7, xmm10
	maxpd xmm7, xmm13
	; FPSUB_R f0, a1
	subpd xmm0, xmm9
	; IMUL_M r5, L1[r6]
	mov eax, r14d
	and eax, 16376
	imul r13, qword ptr [rsi+rax]
	; IADD_RC r1, r2, -1263285243
	lea r9, [r9+r10-1263285243]
	; IMUL_9C r4, 1994773931
	lea r12, [r12+r12*8+1994773931]
	; FPSWAP_R e3
	shufpd xmm7, xmm7, 1
	; IMUL_M r0, L1[r7]
	mov eax, r15d
	and eax, 16376
	imul r8, qword ptr [rsi+rax]
	; IROR_R r1, r6
	mov ecx, r14d
	ror r9, cl
	; IROL_R r2, r4
	mov ecx, r12d
	rol r10, cl
	; FPSUB_R f3, a1
	subpd xmm3, xmm9
	; ISTORE L1[r0], r5
	mov eax, r8d
	and eax, 16376
	mov qword ptr [rsi+rax], r13
	; FPDIV_M e2, L2[r3]
	mov eax, r11d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	divpd xmm6, xmm12
	maxpd xmm6, xmm13
	; FPSWAP_R f2
	shufpd xmm2, xmm2, 1
	; IADD_R r7, r5
	add r15, r13
	; FPDIV_M e0, L1[r4]
	mov eax, r12d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	divpd xmm4, xmm12
	maxpd xmm4, xmm13
	; FPADD_M f3, L1[r5]
	mov eax, r13d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm3, xmm12
	; FPADD_R f0, a3
	addpd xmm0, xmm11
	; IADD_R r2, r0
	add r10, r8
	; ISTORE L1[r3], r6
	mov eax, r11d
	and eax, 16376
	mov qword ptr [rsi+rax], r14
	; IROR_R r1, r7
	mov ecx, r15d
	ror r9, cl
	; IMUL_9C r5, 301671287
	lea r13, [r13+r13*8+301671287]
	; IXOR_R r7, 266992378
	xor r15, 266992378
	; FPSQRT_R e3
	sqrtpd xmm7, xmm7
	; IMUL_M r2, L2[r0]
	mov eax, r8d
	and eax, 262136
	imul r10, qword ptr [rsi+rax]
	; FPMUL_R e3, a2
	mulpd xmm7, xmm10
	; IMUL_R r0, r6
	imul r8, r14
	; ISTORE L1[r0], r7
	mov eax, r8d
	and eax, 16376
	mov qword ptr [rsi+rax], r15
	; FPNEG_R f0
	xorps xmm0, xmm15
	; FPADD_M f3, L1[r5]
	mov eax, r13d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm3, xmm12
	; IROR_R r5, r4
	mov ecx, r12d
	ror r13, cl
	; ISTORE L2[r7], r2
	mov eax, r15d
	and eax, 262136
	mov qword ptr [rsi+rax], r10
	; FPADD_R f2, a3
	addpd xmm2, xmm11
	; FPADD_M f3, L1[r2]
	mov eax, r10d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm3, xmm12
	; ISDIV_C r5, -2076168315
	mov rax, -4770095103914078469
	imul r13
	xor eax, eax
	sar rdx, 29
	sets al
	add rdx, rax
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
	; ISMULH_R r0, -1500893068
	mov rax, -1500893068
	imul r8
	add r8, rdx
	; IADD_R r2, r3
	add r10, r11
	; FPSQRT_R e2
	sqrtpd xmm6, xmm6
	; IROL_R r7, r4
	mov ecx, r12d
	rol r15, cl
	; IMUL_R r4, r2
	imul r12, r10
	; ISUB_R r3, r7
	sub r11, r15
	; IADD_R r2, r7
	add r10, r15
	; FPDIV_R e3, a0
	divpd xmm7, xmm8
	maxpd xmm7, xmm13
	; ISUB_R r6, 540663146
	sub r14, 540663146
	; IROL_R r5, 58
	rol r13, 58
	; FPADD_R f2, a1
	addpd xmm2, xmm9
	; FPADD_R f2, a2
	addpd xmm2, xmm10
	; FPDIV_R e1, a2
	divpd xmm5, xmm10
	maxpd xmm5, xmm13
	; FPADD_R f1, a2
	addpd xmm1, xmm10
	; IADD_R r5, r3
	add r13, r11
	; IADD_R r7, -1780268176
	add r15, -1780268176
	; ISUB_R r7, r0
	sub r15, r8
	; ISTORE L2[r0], r7
	mov eax, r8d
	and eax, 262136
	mov qword ptr [rsi+rax], r15
	; INEG_R r2
	neg r10
	; FPNEG_R f0
	xorps xmm0, xmm15
	; INEG_R r2
	neg r10
	; IADD_R r0, r3
	add r8, r11
	; IMUL_9C r7, -2124093035
	lea r15, [r15+r15*8-2124093035]
	; FPADD_M f2, L1[r0]
	mov eax, r8d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm2, xmm12
	; FPMUL_M e0, L1[r6]
	mov eax, r14d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	mulpd xmm4, xmm12
	maxpd xmm4, xmm13
	; FPSUB_R f2, a3
	subpd xmm2, xmm11
	; IMUL_R r1, r2
	imul r9, r10
	; IDIV_C r7, 3214009572
	mov rax, 12325439725582798855
	mul r15
	shr rdx, 31
	add r15, rdx
	; IMULH_R r3, r2
	mov rax, r11
	mul r10
	mov r11, rdx
	; IROR_R r1, r0
	mov ecx, r8d
	ror r9, cl
	; FPMUL_R e0, a1
	mulpd xmm4, xmm9
	; IADD_RC r4, r4, 1456841848
	lea r12, [r12+r12+1456841848]
	; IROR_R r3, r2
	mov ecx, r10d
	ror r11, cl
	; COND_M r0, of(L1[r4], 1678513610)
	xor ecx, ecx
	mov eax, r12d
	and eax, 16376
	cmp dword ptr [rsi+rax], 1678513610
	seto cl
	add r8, rcx
	; INEG_R r4
	neg r12
	; IMUL_R r4, r1
	imul r12, r9
	; FPADD_R f1, a2
	addpd xmm1, xmm10
	; FPSUB_R f2, a0
	subpd xmm2, xmm8
	; FPMUL_R e1, a2
	mulpd xmm5, xmm10
	; FPSUB_R f0, a3
	subpd xmm0, xmm11
	; IROR_R r0, r7
	mov ecx, r15d
	ror r8, cl
	; ISTORE L2[r1], r4
	mov eax, r9d
	and eax, 262136
	mov qword ptr [rsi+rax], r12
	; IROL_R r7, r6
	mov ecx, r14d
	rol r15, cl
	; IMUL_9C r2, 266593902
	lea r10, [r10+r10*8+266593902]
	; IMUL_R r4, r6
	imul r12, r14
	; FPSUB_R f2, a2
	subpd xmm2, xmm10
	; FPNEG_R f3
	xorps xmm3, xmm15
	; IROR_R r7, r2
	mov ecx, r10d
	ror r15, cl
	; IROR_R r0, r5
	mov ecx, r13d
	ror r8, cl
