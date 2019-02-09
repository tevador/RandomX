	; FMUL_R e0, a2
	mulpd xmm4, xmm10
	; IADD_RC r2, r5, 2673743102
	lea r10, [r10+r13-1621224194]
	; ISTORE L2[r2], r7
	mov eax, r10d
	and eax, 262136
	mov qword ptr [rsi+rax], r15
	; FNEG_R f2
	xorps xmm2, xmm15
	; IMUL_9C r6, 3291464084
	lea r14, [r14+r14*8-1003503212]
	; FSUB_R f1, a0
	subpd xmm1, xmm8
	; IXOR_M r5, L2[r3]
	mov eax, r11d
	and eax, 262136
	xor r13, qword ptr [rsi+rax]
	; FNEG_R f2
	xorps xmm2, xmm15
	; FSUB_R f3, a0
	subpd xmm3, xmm8
	; ISDIV_C r0, 1400272688
	mov rax, 7072565507528518045
	imul r8
	xor eax, eax
	sar rdx, 29
	sets al
	add rdx, rax
	add r8, rdx
	; IMUL_M r3, L1[r7]
	mov eax, r15d
	and eax, 16376
	imul r11, qword ptr [rsi+rax]
	; ISWAP_R r2, r3
	xchg r10, r11
	; IMULH_R r6, r0
	mov rax, r14
	mul r8
	mov r14, rdx
	; FMUL_R e0, a2
	mulpd xmm4, xmm10
	; IADD_RC r3, r4, 4242706868
	lea r11, [r11+r12-52260428]
	; IADD_R r7, 3156349536
	add r15, -1138617760
	; IXOR_M r2, L1[r6]
	mov eax, r14d
	and eax, 16376
	xor r10, qword ptr [rsi+rax]
	; FSUB_M f2, L1[r5]
	mov eax, r13d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm2, xmm12
	; IXOR_R r7, r1
	xor r15, r9
	; COND_R r2, lt(r7, 4253348488)
	xor ecx, ecx
	cmp r15d, -41618808
	setl cl
	add r10, rcx
	; FMUL_R e3, a0
	mulpd xmm7, xmm8
	; COND_R r4, sg(r1, 3333776931)
	xor ecx, ecx
	cmp r9d, -961190365
	sets cl
	add r12, rcx
	; FADD_R f2, a1
	addpd xmm2, xmm9
	; FSUB_R f0, a3
	subpd xmm0, xmm11
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
	; COND_M r6, sg(L1[r6], 1048782623)
	xor ecx, ecx
	mov eax, r14d
	and eax, 16376
	cmp dword ptr [rsi+rax], 1048782623
	sets cl
	add r14, rcx
	; FSQRT_R e0
	sqrtpd xmm4, xmm4
	; INEG_R r2
	neg r10
	; FSQRT_R e1
	sqrtpd xmm5, xmm5
	; FMUL_R e1, a3
	mulpd xmm5, xmm11
	; IMUL_R r7, r6
	imul r15, r14
	; IMULH_R r0, r4
	mov rax, r8
	mul r12
	mov r8, rdx
	; IMUL_R r5, r3
	imul r13, r11
	; FSQRT_R e2
	sqrtpd xmm6, xmm6
	; FADD_R f3, a0
	addpd xmm3, xmm8
	; IADD_R r3, r2
	add r11, r10
	; FADD_R f1, a0
	addpd xmm1, xmm8
	; FMUL_R e3, a2
	mulpd xmm7, xmm10
	; FADD_M f0, L2[r5]
	mov eax, r13d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm0, xmm12
	; IMUL_R r5, r6
	imul r13, r14
	; IADD_RC r1, r2, 3031682053
	lea r9, [r9+r10-1263285243]
	; ISUB_M r4, L1[r6]
	mov eax, r14d
	and eax, 16376
	sub r12, qword ptr [rsi+rax]
	; FSWAP_R e3
	shufpd xmm7, xmm7, 1
	; IMUL_R r0, r7
	imul r8, r15
	; IXOR_R r1, r6
	xor r9, r14
	; IXOR_M r2, L1[r4]
	mov eax, r12d
	and eax, 16376
	xor r10, qword ptr [rsi+rax]
	; FSUB_R f3, a1
	subpd xmm3, xmm9
	; ISTORE L1[r0], r5
	mov eax, r8d
	and eax, 16376
	mov qword ptr [rsi+rax], r13
	; FDIV_M e2, L2[r3]
	mov eax, r11d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	andps xmm12, xmm14
	divpd xmm6, xmm12
	maxpd xmm6, xmm13
	; ISWAP_R r2, r0
	xchg r10, r8
	; IADD_R r7, r5
	add r15, r13
	; FDIV_M e0, L1[r4]
	mov eax, r12d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	andps xmm12, xmm14
	divpd xmm4, xmm12
	maxpd xmm4, xmm13
	; FADD_R f3, a1
	addpd xmm3, xmm9
	; FADD_R f0, a3
	addpd xmm0, xmm11
	; IADD_R r2, r0
	add r10, r8
	; ISTORE L1[r3], r6
	mov eax, r11d
	and eax, 16376
	mov qword ptr [rsi+rax], r14
	; IXOR_R r1, r7
	xor r9, r15
	; ISUB_M r5, L2[r7]
	mov eax, r15d
	and eax, 262136
	sub r13, qword ptr [rsi+rax]
	; ISDIV_C r7, 266992378
	mov rax, -9173520256920442565
	imul r15
	xor eax, eax
	add rdx, r15
	sar rdx, 27
	sets al
	add rdx, rax
	add r15, rdx
	; FDIV_M e3, L1[r4]
	mov eax, r12d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	andps xmm12, xmm14
	divpd xmm7, xmm12
	maxpd xmm7, xmm13
	; IMUL_R r2, r0
	imul r10, r8
	; FMUL_R e3, a2
	mulpd xmm7, xmm10
	; IMUL_R r0, r6
	imul r8, r14
	; ISTORE L1[r0], r7
	mov eax, r8d
	and eax, 16376
	mov qword ptr [rsi+rax], r15
	; FSUB_M f0, L2[r1]
	mov eax, r9d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm0, xmm12
	; FADD_R f3, a1
	addpd xmm3, xmm9
	; IXOR_R r5, r4
	xor r13, r12
	; ISTORE L2[r7], r2
	mov eax, r15d
	and eax, 262136
	mov qword ptr [rsi+rax], r10
	; FSWAP_R e2
	shufpd xmm6, xmm6, 1
	; FADD_R f3, a2
	addpd xmm3, xmm10
	; ISMULH_R r5, r0
	mov rax, r13
	imul r8
	mov r13, rdx
	; IADD_M r0, L1[r4]
	mov eax, r12d
	and eax, 16376
	add r8, qword ptr [rsi+rax]
	; COND_R r7, ge(r6, 2322068811)
	xor ecx, ecx
	cmp r14d, -1972898485
	setge cl
	add r15, rcx
	; FADD_R f2, a2
	addpd xmm2, xmm10
	; IROR_R r7, r6
	mov ecx, r14d
	ror r15, cl
	; IADD_RC r2, r4, 4177509323
	lea r10, [r10+r12-117457973]
	; IMUL_R r0, 2794074228
	imul r8, -1500893068
	; IADD_R r2, r3
	add r10, r11
	; FSQRT_R e2
	sqrtpd xmm6, xmm6
	; IROR_R r7, r4
	mov ecx, r12d
	ror r15, cl
	; IMUL_9C r4, 381194890
	lea r12, [r12+r12*8+381194890]
	; IADD_RC r3, r7, 1050899263
	lea r11, [r11+r15+1050899263]
	; IADD_R r2, r7
	add r10, r15
	; FMUL_R e3, a0
	mulpd xmm7, xmm8
	; IADD_RC r6, r6, 540663146
	lea r14, [r14+r14+540663146]
	; IROR_R r5, 58
	ror r13, 58
	; FADD_R f2, a1
	addpd xmm2, xmm9
	; FADD_R f2, a2
	addpd xmm2, xmm10
	; FMUL_R e1, a2
	mulpd xmm5, xmm10
	; FSWAP_R e1
	shufpd xmm5, xmm5, 1
	; IADD_R r5, r3
	add r13, r11
	; IADD_R r7, 2514699120
	add r15, -1780268176
	; IADD_RC r7, r0, 2797210442
	lea r15, [r15+r8-1497756854]
	; ISTORE L2[r0], r7
	mov eax, r8d
	and eax, 262136
	mov qword ptr [rsi+rax], r15
	; ISMULH_R r2, r4
	mov rax, r10
	imul r12
	mov r10, rdx
	; FSUB_M f0, L1[r2]
	mov eax, r10d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm0, xmm12
	; ISMULH_R r2, r3
	mov rax, r10
	imul r11
	mov r10, rdx
	; IADD_R r0, r3
	add r8, r11
	; ISUB_R r7, r2
	sub r15, r10
	; FADD_R f2, a0
	addpd xmm2, xmm8
	; FMUL_R e0, a2
	mulpd xmm4, xmm10
	; FADD_M f2, L1[r3]
	mov eax, r11d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm2, xmm12
	; IMUL_R r1, r2
	imul r9, r10
	; IMUL_M r7, L1[r5]
	mov eax, r13d
	and eax, 16376
	imul r15, qword ptr [rsi+rax]
	; IMUL_R r3, r2
	imul r11, r10
	; IXOR_R r1, r0
	xor r9, r8
	; FNEG_R f0
	xorps xmm0, xmm15
	; IADD_RC r4, r4, 1456841848
	lea r12, [r12+r12+1456841848]
	; IXOR_R r3, r2
	xor r11, r10
	; COND_R r0, of(r4, 1678513610)
	xor ecx, ecx
	cmp r12d, 1678513610
	seto cl
	add r8, rcx
	; ISMULH_R r4, r4
	mov rax, r12
	imul r12
	mov r12, rdx
	; IMUL_R r4, r1
	imul r12, r9
	; FADD_R f1, a2
	addpd xmm1, xmm10
	; FSUB_R f2, a0
	subpd xmm2, xmm8
	; FMUL_R e1, a2
	mulpd xmm5, xmm10
	; FSUB_R f0, a3
	subpd xmm0, xmm11
	; IXOR_R r0, r7
	xor r8, r15
	; ISTORE L2[r1], r4
	mov eax, r9d
	and eax, 262136
	mov qword ptr [rsi+rax], r12
	; IXOR_M r7, L1[r6]
	mov eax, r14d
	and eax, 16376
	xor r15, qword ptr [rsi+rax]
	; ISUB_R r2, r4
	sub r10, r12
	; ISUB_M r4, L1[r6]
	mov eax, r14d
	and eax, 16376
	sub r12, qword ptr [rsi+rax]
	; FADD_R f2, a2
	addpd xmm2, xmm10
	; FSUB_M f3, L2[r4]
	mov eax, r12d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm3, xmm12
	; IXOR_R r7, r2
	xor r15, r10
	; IXOR_R r0, r5
	xor r8, r13
	; FADD_R f1, a2
	addpd xmm1, xmm10
	; FMUL_R e3, a2
	mulpd xmm7, xmm10
	; FSWAP_R e3
	shufpd xmm7, xmm7, 1
	; FSWAP_R f1
	shufpd xmm1, xmm1, 1
	; COND_R r2, ge(r2, 4068636356)
	xor ecx, ecx
	cmp r10d, -226330940
	setge cl
	add r10, rcx
	; FMUL_R e2, a3
	mulpd xmm6, xmm11
	; FSUB_M f2, L2[r1]
	mov eax, r9d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm2, xmm12
	; FADD_R f1, a0
	addpd xmm1, xmm8
	; ISUB_R r7, r5
	sub r15, r13
	; ISUB_M r0, L1[r1]
	mov eax, r9d
	and eax, 16376
	sub r8, qword ptr [rsi+rax]
	; FSUB_R f3, a1
	subpd xmm3, xmm9
	; ISWAP_R r3, r5
	xchg r11, r13
	; IADD_RC r5, r2, 795784298
	lea r13, [r13+r10+795784298]
	; IADD_RC r0, r4, 2244788743
	lea r8, [r8+r12-2050178553]
	; IMUL_9C r5, 1062534001
	lea r13, [r13+r13*8+1062534001]
	; FADD_R f0, a2
	addpd xmm0, xmm10
	; FMUL_R e3, a1
	mulpd xmm7, xmm9
	; IDIV_C r3, 1662492575
	mov rax, 11914062610815620875
	mul r11
	shr rdx, 30
	add r11, rdx
	; IMUL_M r5, L1[r0]
	mov eax, r8d
	and eax, 16376
	imul r13, qword ptr [rsi+rax]
	; IDIV_C r4, 1963597892
	mov rax, r12
	shr rax, 2
	mov rcx, 1260889558222626443
	mul rcx
	shr rdx, 25
	add r12, rdx
	; IMUL_9C r7, 1820045218
	lea r15, [r15+r15*8+1820045218]
	; IMUL_M r0, L1[r3]
	mov eax, r11d
	and eax, 16376
	imul r8, qword ptr [rsi+rax]
	; IXOR_R r3, r7
	xor r11, r15
	; ISMULH_R r4, r2
	mov rax, r12
	imul r10
	mov r12, rdx
	; ISWAP_R r3, r0
	xchg r11, r8
	; IXOR_R r2, r0
	xor r10, r8
	; IXOR_M r0, L2[r1]
	mov eax, r9d
	and eax, 262136
	xor r8, qword ptr [rsi+rax]
	; ISDIV_C r7, 3359520316
	mov rax, 7859804860668271393
	imul r15
	xor eax, eax
	sub rdx, r15
	sar rdx, 29
	sets al
	add rdx, rax
	add r15, rdx
	; IMUL_M r6, L1[r2]
	mov eax, r10d
	and eax, 16376
	imul r14, qword ptr [rsi+rax]
	; FNEG_R f3
	xorps xmm3, xmm15
	; IADD_RC r4, r2, 1704868083
	lea r12, [r12+r10+1704868083]
	; FADD_R f2, a0
	addpd xmm2, xmm8
	; ISTORE L1[r0], r0
	mov eax, r8d
	and eax, 16376
	mov qword ptr [rsi+rax], r8
	; FADD_M f0, L1[r7]
	mov eax, r15d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm0, xmm12
	; FMUL_R e0, a3
	mulpd xmm4, xmm11
	; FSUB_R f3, a2
	subpd xmm3, xmm10
	; IADD_RC r7, r7, 1302457878
	lea r15, [r15+r15+1302457878]
	; ISUB_R r1, 1330165941
	sub r9, 1330165941
	; FNEG_R f1
	xorps xmm1, xmm15
	; IROR_R r0, r4
	mov ecx, r12d
	ror r8, cl
	; FSUB_R f1, a0
	subpd xmm1, xmm8
	; IROR_R r5, r6
	mov ecx, r14d
	ror r13, cl
	; COND_R r0, ab(r1, 3984033425)
	xor ecx, ecx
	cmp r9d, -310933871
	seta cl
	add r8, rcx
	; COND_R r4, ab(r7, 757929676)
	xor ecx, ecx
	cmp r15d, 757929676
	seta cl
	add r12, rcx
	; FMUL_R e0, a1
	mulpd xmm4, xmm9
	; IMUL_R r1, r3
	imul r9, r11
	; ISUB_R r3, r2
	sub r11, r10
	; FSUB_R f3, a2
	subpd xmm3, xmm10
	; FDIV_M e1, L1[r4]
	mov eax, r12d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	andps xmm12, xmm14
	divpd xmm5, xmm12
	maxpd xmm5, xmm13
	; FSWAP_R f1
	shufpd xmm1, xmm1, 1
	; IADD_R r7, 2873779272
	add r15, -1421188024
	; FSUB_M f3, L2[r2]
	mov eax, r10d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm3, xmm12
	; FSUB_R f2, a3
	subpd xmm2, xmm11
	; FSUB_R f3, a1
	subpd xmm3, xmm9
	; FMUL_R e1, a3
	mulpd xmm5, xmm11
	; IADD_RC r2, r4, 3977135268
	lea r10, [r10+r12-317832028]
	; IMUL_M r4, L1[r5]
	mov eax, r13d
	and eax, 16376
	imul r12, qword ptr [rsi+rax]
	; FDIV_M e1, L1[r7]
	mov eax, r15d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	andps xmm12, xmm14
	divpd xmm5, xmm12
	maxpd xmm5, xmm13
	; IADD_R r5, r2
	add r13, r10
	; ISUB_R r4, 401020510
	sub r12, 401020510
	; IROR_R r3, r0
	mov ecx, r8d
	ror r11, cl
	; ISTORE L1[r7], r0
	mov eax, r15d
	and eax, 16376
	mov qword ptr [rsi+rax], r8
	; FSUB_R f2, a1
	subpd xmm2, xmm9
	; FMUL_R e3, a1
	mulpd xmm7, xmm9
	; IMUL_9C r3, 720965215
	lea r11, [r11+r11*8+720965215]
	; IMUL_9C r6, 74948046
	lea r14, [r14+r14*8+74948046]
	; ISTORE L1[r7], r3
	mov eax, r15d
	and eax, 16376
	mov qword ptr [rsi+rax], r11
	; IXOR_R r2, r6
	xor r10, r14
	; FMUL_R e3, a1
	mulpd xmm7, xmm9
	; ISUB_R r4, r1
	sub r12, r9
	; ISUB_R r3, r0
	sub r11, r8
	; ISWAP_R r7, r5
	xchg r15, r13
	; IMUL_R r2, r6
	imul r10, r14
	; COND_R r2, ge(r2, 2402809790)
	xor ecx, ecx
	cmp r10d, -1892157506
	setge cl
	add r10, rcx
	; FADD_R f1, a3
	addpd xmm1, xmm11
	; IADD_R r7, r0
	add r15, r8
	; IDIV_C r1, 624867857
	mov rax, 15848983434401622933
	mul r9
	shr rdx, 29
	add r9, rdx
	; FADD_R f0, a1
	addpd xmm0, xmm9
	; IADD_RC r5, r7, 3817376178
	lea r13, [r13+r15-477591118]
	; FSUB_R f0, a3
	subpd xmm0, xmm11
	; ISUB_M r6, L1[r2]
	mov eax, r10d
	and eax, 16376
	sub r14, qword ptr [rsi+rax]
	; FMUL_R e3, a1
	mulpd xmm7, xmm9
	; IADD_R r0, r4
	add r8, r12
	; FSUB_R f3, a1
	subpd xmm3, xmm9
	; FSUB_M f2, L1[r4]
	mov eax, r12d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm2, xmm12
	; ISDIV_C r2, 3898255608
	mov rax, 5964731804029407733
	imul r10
	xor eax, eax
	sub rdx, r10
	sar rdx, 28
	sets al
	add rdx, rax
	add r10, rdx
	; FNEG_R f2
	xorps xmm2, xmm15
	; FSUB_R f3, a2
	subpd xmm3, xmm10
	; FADD_R f1, a3
	addpd xmm1, xmm11
	; IMUL_R r3, r2
	imul r11, r10
	; FADD_M f0, L1[r3]
	mov eax, r11d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm0, xmm12
	; ISMULH_R r5, r2
	mov rax, r13
	imul r10
	mov r13, rdx
	; IMULH_R r6, r2
	mov rax, r14
	mul r10
	mov r14, rdx
	; FADD_M f3, L1[r3]
	mov eax, r11d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm3, xmm12
	; IMUL_R r6, r7
	imul r14, r15
	; FSUB_R f0, a0
	subpd xmm0, xmm8
	; FNEG_R f2
	xorps xmm2, xmm15
	; ISUB_R r6, r4
	sub r14, r12
	; FADD_R f1, a1
	addpd xmm1, xmm9
	; IXOR_R r0, r5
	xor r8, r13
	; FADD_R f2, a1
	addpd xmm2, xmm9
	; ISWAP_R r7, r5
	xchg r15, r13
	; FMUL_R e3, a2
	mulpd xmm7, xmm10
	; IADD_RC r3, r6, 2977336568
	lea r11, [r11+r14-1317630728]
	; IMUL_R r2, r3
	imul r10, r11
	; IADD_RC r1, r4, 894105694
	lea r9, [r9+r12+894105694]
	; IMUL_9C r7, 504293473
	lea r15, [r15+r15*8+504293473]
	; FSUB_R f1, a0
	subpd xmm1, xmm8
	; IMUL_R r7, r1
	imul r15, r9
	; IXOR_R r2, r4
	xor r10, r12
	; IADD_RC r0, r1, 392362094
	lea r8, [r8+r9+392362094]
	; IDIV_C r4, 1645771433
	mov rax, 376097195048767223
	mul r12
	shr rdx, 25
	add r12, rdx
	; ISUB_R r4, r3
	sub r12, r11
	; ISUB_M r7, L1[r4]
	mov eax, r12d
	and eax, 16376
	sub r15, qword ptr [rsi+rax]
	; IMUL_M r5, L1[r7]
	mov eax, r15d
	and eax, 16376
	imul r13, qword ptr [rsi+rax]
	; IROR_R r1, r7
	mov ecx, r15d
	ror r9, cl
	; INEG_R r4
	neg r12
	; IMUL_R r3, 1863959234
	imul r11, 1863959234
	; IROR_R r4, 59
	ror r12, 59
	; IMUL_M r1, L3[363256]
	imul r9, qword ptr [rsi+363256]
	; ISTORE L2[r6], r7
	mov eax, r14d
	and eax, 262136
	mov qword ptr [rsi+rax], r15
	; ISTORE L1[r1], r5
	mov eax, r9d
	and eax, 16376
	mov qword ptr [rsi+rax], r13
	; FNEG_R f0
	xorps xmm0, xmm15
	; FSQRT_R e2
	sqrtpd xmm6, xmm6
	; FMUL_R e0, a3
	mulpd xmm4, xmm11
	; FMUL_R e3, a2
	mulpd xmm7, xmm10
	; IROR_R r5, r2
	mov ecx, r10d
	ror r13, cl
	; IADD_R r0, r4
	add r8, r12
