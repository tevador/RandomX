	; COND_M r1, sg(L1[r3], -2004237569)
	xor ecx, ecx
	mov eax, r11d
	and eax, 16376
	cmp dword ptr [rsi+rax], -2004237569
	sets cl
	add r9, rcx
	; IXOR_R r7, -1379425991
	xor r15, -1379425991
	; IXOR_R r2, r6
	xor r10, r14
	; FSWAP_R f3
	shufpd xmm3, xmm3, 1
	; FADD_R f1, a1
	addpd xmm1, xmm9
	; IMUL_R r0, r5
	imul r8, r13
	; FMUL_R e1, a3
	mulpd xmm5, xmm11
	; IADD_R r3, r2
	add r11, r10
	; COND_M r1, ab(L2[r6], -724006934)
	xor ecx, ecx
	mov eax, r14d
	and eax, 262136
	cmp dword ptr [rsi+rax], -724006934
	seta cl
	add r9, rcx
	; IADD_RC r2, r7, -854121467
	lea r10, [r10+r15-854121467]
	; IADD_RC r5, r6, 1291744030
	lea r13, [r13+r14+1291744030]
	; ISTORE L2[r6], r4
	mov eax, r14d
	and eax, 262136
	mov qword ptr [rsi+rax], r12
	; IMUL_R r6, r7
	imul r14, r15
	; FSUB_R f0, a3
	subpd xmm0, xmm11
	; IADD_M r3, L1[r0]
	mov eax, r8d
	and eax, 16376
	add r11, qword ptr [rsi+rax]
	; ISDIV_C r4, -692911499
	mov rax, -893288710803585809
	imul r12
	xor eax, eax
	sar rdx, 25
	sets al
	add rdx, rax
	add r12, rdx
	; FMUL_R e0, a0
	mulpd xmm4, xmm8
	; FDIV_M e1, L1[r0]
	mov eax, r8d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	andps xmm12, xmm14
	divpd xmm5, xmm12
	maxpd xmm5, xmm13
	; FMUL_R e0, a1
	mulpd xmm4, xmm9
	; COND_M r0, no(L1[r1], -540292380)
	xor ecx, ecx
	mov eax, r9d
	and eax, 16376
	cmp dword ptr [rsi+rax], -540292380
	setno cl
	add r8, rcx
	; FSUB_R f1, a1
	subpd xmm1, xmm9
	; IADD_RC r0, r2, 310371682
	lea r8, [r8+r10+310371682]
	; COND_R r3, lt(r0, -1067603143)
	xor ecx, ecx
	cmp r8d, -1067603143
	setl cl
	add r11, rcx
	; FMUL_R e0, a0
	mulpd xmm4, xmm8
	; FADD_R f0, a3
	addpd xmm0, xmm11
	; COND_R r4, sg(r3, -389806289)
	xor ecx, ecx
	cmp r11d, -389806289
	sets cl
	add r12, rcx
	; FMUL_R e0, a3
	mulpd xmm4, xmm11
	; ISTORE L2[r7], r4
	mov eax, r15d
	and eax, 262136
	mov qword ptr [rsi+rax], r12
	; IADD_RC r4, r2, 1888908452
	lea r12, [r12+r10+1888908452]
	; IADD_R r1, r2
	add r9, r10
	; IXOR_R r6, r5
	xor r14, r13
	; IADD_M r7, L1[r0]
	mov eax, r8d
	and eax, 16376
	add r15, qword ptr [rsi+rax]
	; IADD_R r5, r6
	add r13, r14
	; FSUB_R f0, a1
	subpd xmm0, xmm9
	; IMULH_R r5, r4
	mov rax, r13
	mul r12
	mov r13, rdx
	; IMUL_9C r7, 753606235
	lea r15, [r15+r15*8+753606235]
	; FSWAP_R e2
	shufpd xmm6, xmm6, 1
	; IMUL_M r7, L1[r1]
	mov eax, r9d
	and eax, 16376
	imul r15, qword ptr [rsi+rax]
	; IMUL_R r5, 1431156245
	imul r13, 1431156245
	; IADD_RC r4, r2, 1268508410
	lea r12, [r12+r10+1268508410]
	; FSWAP_R f2
	shufpd xmm2, xmm2, 1
	; ISDIV_C r0, -845194077
	mov rax, -5858725577819591251
	imul r8
	xor eax, eax
	sar rdx, 28
	sets al
	add rdx, rax
	add r8, rdx
	; COND_R r0, ab(r5, 1644043355)
	xor ecx, ecx
	cmp r13d, 1644043355
	seta cl
	add r8, rcx
	; COND_R r5, lt(r0, 1216385844)
	xor ecx, ecx
	cmp r8d, 1216385844
	setl cl
	add r13, rcx
	; IMUL_R r5, r2
	imul r13, r10
	; ISTORE L1[r4], r6
	mov eax, r12d
	and eax, 16376
	mov qword ptr [rsi+rax], r14
	; IXOR_R r4, r3
	xor r12, r11
	; IXOR_R r6, r2
	xor r14, r10
	; FSQRT_R e1
	sqrtpd xmm5, xmm5
	; COND_R r5, be(r1, 1781435695)
	xor ecx, ecx
	cmp r9d, 1781435695
	setbe cl
	add r13, rcx
	; ISDIV_C r0, 1367038890
	mov rax, 1811126293978922977
	imul r8
	xor eax, eax
	sar rdx, 27
	sets al
	add rdx, rax
	add r8, rdx
	; FDIV_M e1, L1[r3]
	mov eax, r11d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	andps xmm12, xmm14
	divpd xmm5, xmm12
	maxpd xmm5, xmm13
	; FMUL_R e2, a0
	mulpd xmm6, xmm8
	; ISTORE L1[r5], r4
	mov eax, r13d
	and eax, 16376
	mov qword ptr [rsi+rax], r12
	; IXOR_R r0, r4
	xor r8, r12
	; IMUL_R r5, r1
	imul r13, r9
	; FDIV_M e0, L1[r2]
	mov eax, r10d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	andps xmm12, xmm14
	divpd xmm4, xmm12
	maxpd xmm4, xmm13
	; IMUL_R r6, r1
	imul r14, r9
	; FSUB_M f1, L1[r0]
	mov eax, r8d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm1, xmm12
	; COND_R r2, ns(r1, 392878356)
	xor ecx, ecx
	cmp r9d, 392878356
	setns cl
	add r10, rcx
	; IADD_R r6, r5
	add r14, r13
	; FMUL_R e2, a0
	mulpd xmm6, xmm8
	; ISTORE L1[r0], r3
	mov eax, r8d
	and eax, 16376
	mov qword ptr [rsi+rax], r11
	; IMUL_R r1, r3
	imul r9, r11
	; IMUL_R r5, r2
	imul r13, r10
	; FADD_R f0, a0
	addpd xmm0, xmm8
	; FADD_R f0, a1
	addpd xmm0, xmm9
	; FSUB_R f0, a0
	subpd xmm0, xmm8
	; IMUL_R r3, r5
	imul r11, r13
	; IADD_R r1, r5
	add r9, r13
	; IXOR_M r0, L1[r5]
	mov eax, r13d
	and eax, 16376
	xor r8, qword ptr [rsi+rax]
	; FNEG_R f2
	xorps xmm2, xmm15
	; IDIV_C r5, 2577129788
	mov rax, 15371395512010654233
	mul r13
	shr rdx, 31
	add r13, rdx
	; COND_R r5, be(r5, -999219370)
	xor ecx, ecx
	cmp r13d, -999219370
	setbe cl
	add r13, rcx
	; ISTORE L2[r0], r2
	mov eax, r8d
	and eax, 262136
	mov qword ptr [rsi+rax], r10
	; FSUB_R f3, a3
	subpd xmm3, xmm11
	; IROR_R r7, r6
	mov ecx, r14d
	ror r15, cl
	; COND_R r6, ab(r4, 1309137534)
	xor ecx, ecx
	cmp r12d, 1309137534
	seta cl
	add r14, rcx
	; FMUL_R e3, a0
	mulpd xmm7, xmm8
	; COND_M r3, no(L2[r5], 483660199)
	xor ecx, ecx
	mov eax, r13d
	and eax, 262136
	cmp dword ptr [rsi+rax], 483660199
	setno cl
	add r11, rcx
	; IMUL_R r1, r6
	imul r9, r14
	; IADD_RC r7, r2, -1340630490
	lea r15, [r15+r10-1340630490]
	; IADD_M r0, L3[1554088]
	add r8, qword ptr [rsi+1554088]
	; FMUL_R e2, a3
	mulpd xmm6, xmm11
	; IDIV_C r0, 1566192452
	mov rax, 12646619898641986559
	mul r8
	shr rdx, 30
	add r8, rdx
	; FADD_R f0, a1
	addpd xmm0, xmm9
	; ISWAP_R r6, r0
	xchg r14, r8
	; IMUL_9C r4, 1340891034
	lea r12, [r12+r12*8+1340891034]
	; IROR_R r7, r2
	mov ecx, r10d
	ror r15, cl
	; FSQRT_R e2
	sqrtpd xmm6, xmm6
	; FADD_R f2, a1
	addpd xmm2, xmm9
	; IMUL_R r4, r3
	imul r12, r11
	; IADD_RC r6, r3, -1584624397
	lea r14, [r14+r11-1584624397]
	; IROR_R r1, r7
	mov ecx, r15d
	ror r9, cl
	; IXOR_R r4, r7
	xor r12, r15
	; FSWAP_R f0
	shufpd xmm0, xmm0, 1
	; FSWAP_R f3
	shufpd xmm3, xmm3, 1
	; IROR_R r5, 3
	ror r13, 3
	; FADD_R f3, a0
	addpd xmm3, xmm8
	; FMUL_R e0, a0
	mulpd xmm4, xmm8
	; IADD_R r4, r1
	add r12, r9
	; COND_M r4, ge(L1[r6], -1612023931)
	xor ecx, ecx
	mov eax, r14d
	and eax, 16376
	cmp dword ptr [rsi+rax], -1612023931
	setge cl
	add r12, rcx
	; FSWAP_R e2
	shufpd xmm6, xmm6, 1
	; IADD_R r3, r7
	add r11, r15
	; COND_R r5, be(r2, -1083018923)
	xor ecx, ecx
	cmp r10d, -1083018923
	setbe cl
	add r13, rcx
	; IADD_R r3, r7
	add r11, r15
	; ISTORE L2[r6], r0
	mov eax, r14d
	and eax, 262136
	mov qword ptr [rsi+rax], r8
	; IXOR_R r2, r3
	xor r10, r11
	; FMUL_R e2, a3
	mulpd xmm6, xmm11
	; FMUL_R e3, a3
	mulpd xmm7, xmm11
	; FADD_R f0, a2
	addpd xmm0, xmm10
	; ISTORE L1[r5], r1
	mov eax, r13d
	and eax, 16376
	mov qword ptr [rsi+rax], r9
	; FMUL_R e3, a3
	mulpd xmm7, xmm11
	; ISWAP_R r1, r2
	xchg r9, r10
	; FSWAP_R e0
	shufpd xmm4, xmm4, 1
	; FSUB_R f1, a2
	subpd xmm1, xmm10
	; FSUB_R f0, a0
	subpd xmm0, xmm8
	; IROR_R r7, r0
	mov ecx, r8d
	ror r15, cl
	; IADD_RC r5, r4, 283260945
	lea r13, [r13+r12+283260945]
	; ISDIV_C r6, -340125851
	mov rax, -3639652898025032137
	imul r14
	xor eax, eax
	sar rdx, 26
	sets al
	add rdx, rax
	add r14, rdx
	; ISTORE L2[r2], r3
	mov eax, r10d
	and eax, 262136
	mov qword ptr [rsi+rax], r11
	; IADD_RC r6, r6, -935765909
	lea r14, [r14+r14-935765909]
	; ISDIV_C r3, -701703430
	mov rax, -7056770631919985199
	imul r11
	xor eax, eax
	sar rdx, 28
	sets al
	add rdx, rax
	add r11, rdx
	; IXOR_M r3, L2[r1]
	mov eax, r9d
	and eax, 262136
	xor r11, qword ptr [rsi+rax]
	; FADD_R f2, a1
	addpd xmm2, xmm9
	; ISTORE L1[r5], r7
	mov eax, r13d
	and eax, 16376
	mov qword ptr [rsi+rax], r15
	; FSUB_R f2, a0
	subpd xmm2, xmm8
	; FMUL_R e3, a2
	mulpd xmm7, xmm10
	; IADD_R r2, r5
	add r10, r13
	; IADD_RC r2, r5, -1056770544
	lea r10, [r10+r13-1056770544]
	; ISTORE L2[r2], r3
	mov eax, r10d
	and eax, 262136
	mov qword ptr [rsi+rax], r11
	; ISMULH_R r7, r1
	mov rax, r15
	imul r9
	mov r15, rdx
	; IXOR_R r0, r5
	xor r8, r13
	; ISTORE L1[r4], r0
	mov eax, r12d
	and eax, 16376
	mov qword ptr [rsi+rax], r8
	; INEG_R r5
	neg r13
	; FSUB_R f0, a1
	subpd xmm0, xmm9
	; IMUL_R r6, -244261682
	imul r14, -244261682
	; IMUL_R r1, r0
	imul r9, r8
	; IMUL_9C r3, -985744277
	lea r11, [r11+r11*8-985744277]
	; IROR_R r2, r1
	mov ecx, r9d
	ror r10, cl
	; ISUB_R r4, -1079131550
	sub r12, -1079131550
	; FNEG_R f3
	xorps xmm3, xmm15
	; COND_R r4, ns(r5, -362284631)
	xor ecx, ecx
	cmp r13d, -362284631
	setns cl
	add r12, rcx
	; FSUB_R f2, a0
	subpd xmm2, xmm8
	; IXOR_R r4, r5
	xor r12, r13
	; FNEG_R f1
	xorps xmm1, xmm15
	; FADD_R f0, a0
	addpd xmm0, xmm8
	; IADD_RC r3, r3, -173615832
	lea r11, [r11+r11-173615832]
	; IMUL_R r0, 928402279
	imul r8, 928402279
	; ISUB_R r2, r0
	sub r10, r8
	; IXOR_R r6, r3
	xor r14, r11
	; ISUB_R r2, 2106401471
	sub r10, 2106401471
	; FADD_R f0, a2
	addpd xmm0, xmm10
	; IMUL_R r4, r6
	imul r12, r14
	; IADD_RC r4, r0, -373491513
	lea r12, [r12+r8-373491513]
	; ISDIV_C r0, -1739042721
	mov rax, 7057121271817449967
	imul r8
	xor eax, eax
	sub rdx, r8
	sar rdx, 30
	sets al
	add rdx, rax
	add r8, rdx
	; IADD_R r3, r1
	add r11, r9
	; ISUB_M r7, L1[r5]
	mov eax, r13d
	and eax, 16376
	sub r15, qword ptr [rsi+rax]
	; IMUL_R r1, r2
	imul r9, r10
	; ISUB_R r0, 722465116
	sub r8, 722465116
	; IADD_RC r0, r0, -1919541169
	lea r8, [r8+r8-1919541169]
	; ISUB_M r2, L1[r3]
	mov eax, r11d
	and eax, 16376
	sub r10, qword ptr [rsi+rax]
	; IADD_R r7, -1183581468
	add r15, -1183581468
	; FMUL_R e1, a3
	mulpd xmm5, xmm11
	; FSUB_R f0, a0
	subpd xmm0, xmm8
	; FADD_R f0, a3
	addpd xmm0, xmm11
	; IMUL_9C r6, 1241113238
	lea r14, [r14+r14*8+1241113238]
	; FSUB_R f3, a3
	subpd xmm3, xmm11
	; IADD_M r0, L1[r3]
	mov eax, r11d
	and eax, 16376
	add r8, qword ptr [rsi+rax]
	; IROR_R r3, r7
	mov ecx, r15d
	ror r11, cl
	; FADD_R f2, a1
	addpd xmm2, xmm9
	; IMUL_M r3, L1[r2]
	mov eax, r10d
	and eax, 16376
	imul r11, qword ptr [rsi+rax]
	; IMUL_9C r7, -2080412544
	lea r15, [r15+r15*8-2080412544]
	; IMUL_R r0, r3
	imul r8, r11
	; FADD_R f1, a1
	addpd xmm1, xmm9
	; IROR_R r6, 21
	ror r14, 21
	; FDIV_M e3, L1[r1]
	mov eax, r9d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	andps xmm12, xmm14
	divpd xmm7, xmm12
	maxpd xmm7, xmm13
	; FSUB_R f0, a1
	subpd xmm0, xmm9
	; FSWAP_R e1
	shufpd xmm5, xmm5, 1
	; COND_M r0, no(L1[r5], -1627153829)
	xor ecx, ecx
	mov eax, r13d
	and eax, 16376
	cmp dword ptr [rsi+rax], -1627153829
	setno cl
	add r8, rcx
	; FADD_R f2, a3
	addpd xmm2, xmm11
	; FSUB_R f1, a2
	subpd xmm1, xmm10
	; FSUB_M f1, L1[r4]
	mov eax, r12d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm1, xmm12
	; ISTORE L1[r5], r1
	mov eax, r13d
	and eax, 16376
	mov qword ptr [rsi+rax], r9
	; ISUB_M r2, L2[r7]
	mov eax, r15d
	and eax, 262136
	sub r10, qword ptr [rsi+rax]
	; ISTORE L1[r2], r3
	mov eax, r10d
	and eax, 16376
	mov qword ptr [rsi+rax], r11
	; FADD_R f0, a3
	addpd xmm0, xmm11
	; ISUB_M r1, L1[r7]
	mov eax, r15d
	and eax, 16376
	sub r9, qword ptr [rsi+rax]
	; IDIV_C r5, 624165039
	mov rax, 15866829597104432181
	mul r13
	shr rdx, 29
	add r13, rdx
	; FMUL_R e3, a0
	mulpd xmm7, xmm8
	; IMUL_R r5, r4
	imul r13, r12
	; FMUL_R e3, a1
	mulpd xmm7, xmm9
	; FMUL_R e3, a3
	mulpd xmm7, xmm11
	; IXOR_R r0, -2064879200
	xor r8, -2064879200
	; FADD_R f1, a3
	addpd xmm1, xmm11
	; IADD_M r0, L1[r3]
	mov eax, r11d
	and eax, 16376
	add r8, qword ptr [rsi+rax]
	; ISMULH_R r7, r3
	mov rax, r15
	imul r11
	mov r15, rdx
	; IMUL_R r5, -1645503310
	imul r13, -1645503310
	; IMUL_R r7, r3
	imul r15, r11
	; FMUL_R e2, a2
	mulpd xmm6, xmm10
	; IADD_R r6, 1769041191
	add r14, 1769041191
	; FSUB_M f1, L1[r4]
	mov eax, r12d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm1, xmm12
	; ISTORE L2[r1], r0
	mov eax, r9d
	and eax, 262136
	mov qword ptr [rsi+rax], r8
	; FNEG_R f0
	xorps xmm0, xmm15
	; FMUL_R e0, a3
	mulpd xmm4, xmm11
	; IMUL_R r2, r7
	imul r10, r15
	; IADD_R r5, r1
	add r13, r9
	; IROR_R r3, r6
	mov ecx, r14d
	ror r11, cl
	; FADD_R f0, a0
	addpd xmm0, xmm8
	; FMUL_R e1, a2
	mulpd xmm5, xmm10
	; FNEG_R f3
	xorps xmm3, xmm15
	; FADD_R f1, a1
	addpd xmm1, xmm9
	; IMULH_R r2, r5
	mov rax, r10
	mul r13
	mov r10, rdx
	; ISTORE L1[r4], r0
	mov eax, r12d
	and eax, 16376
	mov qword ptr [rsi+rax], r8
	; ISWAP_R r7, r0
	xchg r15, r8
	; FSWAP_R f0
	shufpd xmm0, xmm0, 1
	; ISUB_R r2, r0
	sub r10, r8
	; FSUB_R f1, a3
	subpd xmm1, xmm11
	; ISUB_M r5, L1[r3]
	mov eax, r11d
	and eax, 16376
	sub r13, qword ptr [rsi+rax]
	; IXOR_R r7, r0
	xor r15, r8
	; IMUL_R r4, r1
	imul r12, r9
	; IADD_RC r0, r2, -1102648763
	lea r8, [r8+r10-1102648763]
	; FMUL_R e3, a3
	mulpd xmm7, xmm11
	; IXOR_R r4, r1
	xor r12, r9
	; IXOR_R r6, r0
	xor r14, r8
	; FSQRT_R e1
	sqrtpd xmm5, xmm5
	; IMUL_M r6, L2[r1]
	mov eax, r9d
	and eax, 262136
	imul r14, qword ptr [rsi+rax]
	; ISMULH_M r5, L3[353552]
	mov rax, r13
	imul qword ptr [rsi+353552]
	mov r13, rdx
	; ISUB_M r1, L1[r6]
	mov eax, r14d
	and eax, 16376
	sub r9, qword ptr [rsi+rax]
	; FADD_R f0, a3
	addpd xmm0, xmm11
	; FMUL_R e3, a3
	mulpd xmm7, xmm11
	; FSUB_M f3, L2[r7]
	mov eax, r15d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm3, xmm12
	; IMUL_R r0, r2
	imul r8, r10
	; FMUL_R e1, a0
	mulpd xmm5, xmm8
	; COND_R r5, sg(r3, -1392293091)
	xor ecx, ecx
	cmp r11d, -1392293091
	sets cl
	add r13, rcx
	; FSWAP_R e3
	shufpd xmm7, xmm7, 1
	; IMUL_R r7, r4
	imul r15, r12
	; IXOR_R r7, r5
	xor r15, r13
	; FMUL_R e3, a3
	mulpd xmm7, xmm11
	; IMUL_R r4, r3
	imul r12, r11
	; FADD_M f1, L1[r1]
	mov eax, r9d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm1, xmm12
	; IMUL_R r5, r0
	imul r13, r8
	; ISUB_R r7, r0
	sub r15, r8
	; IADD_M r5, L1[r4]
	mov eax, r12d
	and eax, 16376
	add r13, qword ptr [rsi+rax]
	; IADD_R r6, r2
	add r14, r10
	; FMUL_R e1, a1
	mulpd xmm5, xmm9
	; IADD_M r2, L3[1073640]
	add r10, qword ptr [rsi+1073640]
	; IMUL_R r3, r2
	imul r11, r10
	; IXOR_R r1, r0
	xor r9, r8
	; IROR_R r7, r4
	mov ecx, r12d
	ror r15, cl
	; FSUB_R f1, a1
	subpd xmm1, xmm9
	; IMUL_R r7, r5
	imul r15, r13
	; ISUB_R r1, 866191482
	sub r9, 866191482
	; IMUL_M r7, L1[r4]
	mov eax, r12d
	and eax, 16376
	imul r15, qword ptr [rsi+rax]
	; FADD_R f2, a0
	addpd xmm2, xmm8
	; IADD_R r2, r1
	add r10, r9
