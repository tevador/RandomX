	; IMUL_R r0, r7
	imul r8, r15
	; ISMULH_R r2, r1
	mov rax, r10
	imul r9
	mov r10, rdx
	; IMUL_R r2, r4
	imul r10, r12
	; IADD_R r7, r0
	add r15, r8
	; FPSQRT_R e0
	sqrtpd xmm4, xmm4
	; IMUL_R r3, r6
	imul r11, r14
	; FPMUL_R e3, a1
	mulpd xmm7, xmm9
	; IMULH_M r6, L1[r3]
	mov ecx, r11d
	and ecx, 16376
	mov rax, r14
	mul qword ptr [rsi+rcx]
	mov r14, rdx
	; IMUL_R r5, r1
	imul r13, r9
	; FPADD_M f0, L2[r6]
	mov eax, r14d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm0, xmm12
	; IROR_R r4, r3
	mov ecx, r11d
	ror r12, cl
	; IXOR_M r4, L3[984888]
	xor r12, qword ptr [rsi+984888]
	; IROR_R r0, r3
	mov ecx, r11d
	ror r8, cl
	; IROR_R r0, r4
	mov ecx, r12d
	ror r8, cl
	; FPMUL_R e0, a1
	mulpd xmm4, xmm9
	; IMUL_R r0, r2
	imul r8, r10
	; ISUB_M r0, L1[r3]
	mov eax, r11d
	and eax, 16376
	sub r8, qword ptr [rsi+rax]
	; FPSUB_R f3, a1
	subpd xmm3, xmm9
	; ISWAP_R r7, r4
	xchg r15, r12
	; IDIV_C r1, 3690475308
	mov rax, r9
	shr rax, 2
	mov rcx, 5367070356934653253
	mul rcx
	shr rdx, 28
	add r9, rdx
	; IROL_R r4, r2
	mov ecx, r10d
	rol r12, cl
	; IMUL_M r5, L1[r4]
	mov eax, r12d
	and eax, 16376
	imul r13, qword ptr [rsi+rax]
	; IROL_R r4, r7
	mov ecx, r15d
	rol r12, cl
	; ISUB_R r3, r1
	sub r11, r9
	; IADD_R r7, r0
	add r15, r8
	; IADD_M r1, L1[r3]
	mov eax, r11d
	and eax, 16376
	add r9, qword ptr [rsi+rax]
	; FPMUL_R e2, a2
	mulpd xmm6, xmm10
	; IADD_R r6, -1115286770
	add r14, -1115286770
	; FPDIV_R e2, a3
	divpd xmm6, xmm11
	maxpd xmm6, xmm13
	; FPADD_R f1, a2
	addpd xmm1, xmm10
	; IXOR_R r3, r7
	xor r11, r15
	; ISTORE L1[r4], r3
	mov eax, r12d
	and eax, 16376
	mov qword ptr [rsi+rax], r11
	; IROR_R r3, r6
	mov ecx, r14d
	ror r11, cl
	; ISMULH_R r0, r6
	mov rax, r8
	imul r14
	mov r8, rdx
	; IROR_R r6, r5
	mov ecx, r13d
	ror r14, cl
	; IMULH_M r6, L2[r0]
	mov ecx, r8d
	and ecx, 262136
	mov rax, r14
	mul qword ptr [rsi+rcx]
	mov r14, rdx
	; ISUB_R r2, 1512125960
	sub r10, 1512125960
	; IMUL_R r7, r6
	imul r15, r14
	; IMULH_R r6, r7
	mov rax, r14
	mul r15
	mov r14, rdx
	; ISUB_R r4, r1
	sub r12, r9
	; FPMUL_R e3, a2
	mulpd xmm7, xmm10
	; FPSQRT_R e1
	sqrtpd xmm5, xmm5
	; IXOR_R r5, r2
	xor r13, r10
	; FPADD_M f2, L1[r0]
	mov eax, r8d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm2, xmm12
	; IMULH_R r6, r1
	mov rax, r14
	mul r9
	mov r14, rdx
	; ISUB_M r5, L1[r0]
	mov eax, r8d
	and eax, 16376
	sub r13, qword ptr [rsi+rax]
	; FPMUL_R e2, a3
	mulpd xmm6, xmm11
	; IMUL_R r4, r6
	imul r12, r14
	; FPMUL_R e3, a2
	mulpd xmm7, xmm10
	; ISUB_R r3, r2
	sub r11, r10
	; FPMUL_R e3, a2
	mulpd xmm7, xmm10
	; IROL_R r7, r0
	mov ecx, r8d
	rol r15, cl
	; FPSUB_R f3, a2
	subpd xmm3, xmm10
	; IROL_R r3, r7
	mov ecx, r15d
	rol r11, cl
	; ISWAP_R r5, r7
	xchg r13, r15
	; IDIV_C r5, 749951529
	mov rax, 13205547200481862341
	mul r13
	shr rdx, 29
	add r13, rdx
	; FPADD_R f3, a0
	addpd xmm3, xmm8
	; IMUL_M r0, L1[r4]
	mov eax, r12d
	and eax, 16376
	imul r8, qword ptr [rsi+rax]
	; FPADD_R f1, a1
	addpd xmm1, xmm9
	; IROR_R r2, 60
	ror r10, 60
	; IROR_R r5, r4
	mov ecx, r12d
	ror r13, cl
	; FPADD_R f2, a0
	addpd xmm2, xmm8
	; IXOR_M r4, L1[r6]
	mov eax, r14d
	and eax, 16376
	xor r12, qword ptr [rsi+rax]
	; IXOR_R r2, r6
	xor r10, r14
	; FPADD_M f3, L1[r0]
	mov eax, r8d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm3, xmm12
	; ISUB_R r7, r6
	sub r15, r14
	; IMUL_9C r2, -962375579
	lea r10, [r10+r10*8-962375579]
	; FPSUB_R f3, a2
	subpd xmm3, xmm10
	; FPSUB_R f3, a0
	subpd xmm3, xmm8
	; IMUL_R r1, r5
	imul r9, r13
	; IMUL_R r6, r4
	imul r14, r12
	; ISWAP_R r0, r2
	xchg r8, r10
	; ISUB_R r6, r5
	sub r14, r13
	; FPSUB_R f2, a1
	subpd xmm2, xmm9
	; ISDIV_C r6, 652931802
	mov rax, -3278972671018643631
	imul r14
	xor eax, eax
	add rdx, r14
	sar rdx, 29
	sets al
	add rdx, rax
	add r14, rdx
	; IMUL_9C r5, -1142924545
	lea r13, [r13+r13*8-1142924545]
	; ISUB_R r7, 1085161834
	sub r15, 1085161834
	; IMUL_R r4, r6
	imul r12, r14
	; FPMUL_M e1, L1[r4]
	mov eax, r12d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	mulpd xmm5, xmm12
	maxpd xmm5, xmm13
	; FPMUL_M e3, L2[r1]
	mov eax, r9d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	mulpd xmm7, xmm12
	maxpd xmm7, xmm13
	; COND_R r2, lt(r5, 1635027096)
	xor ecx, ecx
	cmp r13d, 1635027096
	setl cl
	add r10, rcx
	; IMUL_R r5, -1219696062
	imul r13, -1219696062
	; IXOR_R r5, r0
	xor r13, r8
	; FPNEG_R f2
	xorps xmm2, xmm15
	; FPADD_R f3, a2
	addpd xmm3, xmm10
	; FPSUB_R f1, a3
	subpd xmm1, xmm11
	; FPADD_R f1, a2
	addpd xmm1, xmm10
	; FPDIV_R e1, a3
	divpd xmm5, xmm11
	maxpd xmm5, xmm13
	; IXOR_M r6, L1[r0]
	mov eax, r8d
	and eax, 16376
	xor r14, qword ptr [rsi+rax]
	; ISUB_R r7, r4
	sub r15, r12
	; ISUB_M r6, L1[r1]
	mov eax, r9d
	and eax, 16376
	sub r14, qword ptr [rsi+rax]
	; ISTORE L1[r5], r3
	mov eax, r13d
	and eax, 16376
	mov qword ptr [rsi+rax], r11
	; IMUL_R r5, r1
	imul r13, r9
	; IROR_R r3, r2
	mov ecx, r10d
	ror r11, cl
	; IMUL_R r4, r7
	imul r12, r15
	; ISDIV_C r6, -54134756
	mov rax, 7012869325244995177
	imul r14
	xor eax, eax
	sub rdx, r14
	sar rdx, 25
	sets al
	add rdx, rax
	add r14, rdx
	; FPMUL_R e1, a2
	mulpd xmm5, xmm10
	; FPSUB_M f2, L2[r4]
	mov eax, r12d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm2, xmm12
	; IMUL_R r0, r5
	imul r8, r13
	; FPMUL_R e3, a0
	mulpd xmm7, xmm8
	; COND_R r5, be(r4, 1545677311)
	xor ecx, ecx
	cmp r12d, 1545677311
	setbe cl
	add r13, rcx
	; IMUL_R r6, r3
	imul r14, r11
	; IROL_R r6, r2
	mov ecx, r10d
	rol r14, cl
	; FPDIV_R e3, a1
	divpd xmm7, xmm9
	maxpd xmm7, xmm13
	; IXOR_M r5, L1[r1]
	mov eax, r9d
	and eax, 16376
	xor r13, qword ptr [rsi+rax]
	; COND_R r3, ab(r2, 1734636060)
	xor ecx, ecx
	cmp r10d, 1734636060
	seta cl
	add r11, rcx
	; ISTORE L1[r2], r7
	mov eax, r10d
	and eax, 16376
	mov qword ptr [rsi+rax], r15
	; IADD_R r5, r6
	add r13, r14
	; FPSUB_R f1, a2
	subpd xmm1, xmm10
	; FPADD_R f2, a1
	addpd xmm2, xmm9
	; FPSWAP_R f1
	shufpd xmm1, xmm1, 1
	; IROL_R r2, r6
	mov ecx, r14d
	rol r10, cl
	; IMUL_R r0, r4
	imul r8, r12
	; FPSUB_R f0, a2
	subpd xmm0, xmm10
	; ISUB_R r6, r7
	sub r14, r15
	; IROL_R r4, r7
	mov ecx, r15d
	rol r12, cl
	; FPMUL_R e2, a0
	mulpd xmm6, xmm8
	; ISUB_R r1, r3
	sub r9, r11
	; FPDIV_R e0, a1
	divpd xmm4, xmm9
	maxpd xmm4, xmm13
	; FPADD_R f0, a1
	addpd xmm0, xmm9
	; FPMUL_R e0, a2
	mulpd xmm4, xmm10
	; FPSUB_R f2, a2
	subpd xmm2, xmm10
	; FPSUB_M f2, L1[r6]
	mov eax, r14d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm2, xmm12
	; FPMUL_R e0, a0
	mulpd xmm4, xmm8
	; IXOR_M r4, L2[r7]
	mov eax, r15d
	and eax, 262136
	xor r12, qword ptr [rsi+rax]
	; FPSUB_R f3, a3
	subpd xmm3, xmm11
	; ISMULH_R r1, r6
	mov rax, r9
	imul r14
	mov r9, rdx
	; COND_R r4, be(r7, 224524971)
	xor ecx, ecx
	cmp r15d, 224524971
	setbe cl
	add r12, rcx
	; FPADD_M f2, L1[r1]
	mov eax, r9d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm2, xmm12
	; IMUL_R r5, r4
	imul r13, r12
	; IADD_RC r1, r5, 370966979
	lea r9, [r9+r13+370966979]
	; IADD_RC r7, r3, -1762209698
	lea r15, [r15+r11-1762209698]
	; FPMUL_M e3, L2[r2]
	mov eax, r10d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	mulpd xmm7, xmm12
	maxpd xmm7, xmm13
	; ISUB_R r2, r7
	sub r10, r15
	; IMUL_9C r3, 171157280
	lea r11, [r11+r11*8+171157280]
	; ISUB_R r3, r5
	sub r11, r13
	; FPNEG_R f3
	xorps xmm3, xmm15
	; FPNEG_R f2
	xorps xmm2, xmm15
	; ISTORE L1[r4], r1
	mov eax, r12d
	and eax, 16376
	mov qword ptr [rsi+rax], r9
	; IADD_R r0, r2
	add r8, r10
	; IXOR_R r7, r6
	xor r15, r14
	; IROR_R r0, r4
	mov ecx, r12d
	ror r8, cl
	; FPMUL_R e3, a2
	mulpd xmm7, xmm10
	; IXOR_M r4, L1[r7]
	mov eax, r15d
	and eax, 16376
	xor r12, qword ptr [rsi+rax]
	; ISTORE L1[r5], r7
	mov eax, r13d
	and eax, 16376
	mov qword ptr [rsi+rax], r15
	; IMUL_9C r7, -1206742834
	lea r15, [r15+r15*8-1206742834]
	; ISMULH_R r0, r4
	mov rax, r8
	imul r12
	mov r8, rdx
	; FPADD_R f2, a0
	addpd xmm2, xmm8
	; FPSUB_R f1, a0
	subpd xmm1, xmm8
	; INEG_R r7
	neg r15
	; COND_M r0, of(L1[r5], -2056260506)
	xor ecx, ecx
	mov eax, r13d
	and eax, 16376
	cmp dword ptr [rsi+rax], -2056260506
	seto cl
	add r8, rcx
	; FPSQRT_R e2
	sqrtpd xmm6, xmm6
	; IMUL_R r3, r4
	imul r11, r12
	; FPNEG_R f1
	xorps xmm1, xmm15
	; FPADD_M f2, L1[r5]
	mov eax, r13d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm2, xmm12
	; FPSUB_R f3, a0
	subpd xmm3, xmm8
	; FPNEG_R f3
	xorps xmm3, xmm15
	; FPMUL_M e3, L2[r5]
	mov eax, r13d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	mulpd xmm7, xmm12
	maxpd xmm7, xmm13
	; ISTORE L1[r2], r2
	mov eax, r10d
	and eax, 16376
	mov qword ptr [rsi+rax], r10
	; IMUL_M r3, L2[r4]
	mov eax, r12d
	and eax, 262136
	imul r11, qword ptr [rsi+rax]
	; IROL_R r5, r6
	mov ecx, r14d
	rol r13, cl
	; IADD_RC r4, r3, -904431293
	lea r12, [r12+r11-904431293]
	; FPSUB_R f1, a1
	subpd xmm1, xmm9
	; IROL_R r7, r0
	mov ecx, r8d
	rol r15, cl
	; ISTORE L2[r1], r7
	mov eax, r9d
	and eax, 262136
	mov qword ptr [rsi+rax], r15
	; IROL_R r4, r3
	mov ecx, r11d
	rol r12, cl
	; IADD_R r5, r2
	add r13, r10
	; COND_R r3, ge(r6, -444806705)
	xor ecx, ecx
	cmp r14d, -444806705
	setge cl
	add r11, rcx
	; FPADD_R f0, a1
	addpd xmm0, xmm9
	; IROL_R r0, 57
	rol r8, 57
	; IADD_R r0, r2
	add r8, r10
	; IADD_R r7, r4
	add r15, r12
	; IROL_R r1, r7
	mov ecx, r15d
	rol r9, cl
	; IXOR_M r7, L2[r5]
	mov eax, r13d
	and eax, 262136
	xor r15, qword ptr [rsi+rax]
	; ISTORE L1[r2], r0
	mov eax, r10d
	and eax, 16376
	mov qword ptr [rsi+rax], r8
	; FPADD_R f1, a2
	addpd xmm1, xmm10
	; ISUB_R r1, r4
	sub r9, r12
	; IXOR_R r5, r0
	xor r13, r8
	; IXOR_M r7, L2[r1]
	mov eax, r9d
	and eax, 262136
	xor r15, qword ptr [rsi+rax]
	; FPSUB_R f0, a0
	subpd xmm0, xmm8
	; IXOR_M r1, L1[r4]
	mov eax, r12d
	and eax, 16376
	xor r9, qword ptr [rsi+rax]
	; FPMUL_R e3, a0
	mulpd xmm7, xmm8
	; ISDIV_C r1, 1473744194
	mov rax, -5006799265644655925
	imul r9
	xor eax, eax
	add rdx, r9
	sar rdx, 30
	sets al
	add rdx, rax
	add r9, rdx
	; IMUL_9C r1, 1626151459
	lea r9, [r9+r9*8+1626151459]
	; IXOR_M r6, L1[r4]
	mov eax, r12d
	and eax, 16376
	xor r14, qword ptr [rsi+rax]
	; FPADD_R f0, a0
	addpd xmm0, xmm8
	; FPADD_R f3, a2
	addpd xmm3, xmm10
	; ISUB_R r6, r7
	sub r14, r15
	; IADD_RC r1, r5, 2075955307
	lea r9, [r9+r13+2075955307]
	; IROL_R r6, r3
	mov ecx, r11d
	rol r14, cl
	; IMULH_R r2, -1135671124
	mov eax, -1135671124
	mul r10
	add r10, rdx
	; ISUB_R r5, r2
	sub r13, r10
	; IMULH_R r3, r5
	mov rax, r11
	mul r13
	mov r11, rdx
	; IADD_M r4, L3[386040]
	add r12, qword ptr [rsi+386040]
	; COND_R r6, ge(r4, 1518758207)
	xor ecx, ecx
	cmp r12d, 1518758207
	setge cl
	add r14, rcx
	; FPDIV_R e3, a1
	divpd xmm7, xmm9
	maxpd xmm7, xmm13
	; FPNEG_R f2
	xorps xmm2, xmm15
	; FPADD_M f1, L1[r4]
	mov eax, r12d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm1, xmm12
	; FPMUL_M e0, L1[r4]
	mov eax, r12d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	mulpd xmm4, xmm12
	maxpd xmm4, xmm13
	; FPSQRT_R e2
	sqrtpd xmm6, xmm6
	; IROL_R r5, r1
	mov ecx, r9d
	rol r13, cl
	; FPADD_R f3, a0
	addpd xmm3, xmm8
	; IROL_R r3, r0
	mov ecx, r8d
	rol r11, cl
	; FPMUL_R e3, a1
	mulpd xmm7, xmm9
	; IROR_R r0, r7
	mov ecx, r15d
	ror r8, cl
	; FPADD_R f2, a2
	addpd xmm2, xmm10
	; IXOR_R r7, r0
	xor r15, r8
	; ISTORE L1[r4], r1
	mov eax, r12d
	and eax, 16376
	mov qword ptr [rsi+rax], r9
	; ISTORE L2[r0], r4
	mov eax, r8d
	and eax, 262136
	mov qword ptr [rsi+rax], r12
	; FPDIV_R e3, a3
	divpd xmm7, xmm11
	maxpd xmm7, xmm13
	; ISTORE L2[r4], r6
	mov eax, r12d
	and eax, 262136
	mov qword ptr [rsi+rax], r14
	; IMUL_R r3, r1
	imul r11, r9
	; IXOR_R r2, r4
	xor r10, r12
	; ISTORE L2[r3], r5
	mov eax, r11d
	and eax, 262136
	mov qword ptr [rsi+rax], r13
	; FPMUL_M e2, L2[r4]
	mov eax, r12d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	mulpd xmm6, xmm12
	maxpd xmm6, xmm13
	; FPSUB_R f3, a0
	subpd xmm3, xmm8
	; COND_R r1, ab(r7, -229570354)
	xor ecx, ecx
	cmp r15d, -229570354
	seta cl
	add r9, rcx
	; IROR_R r7, r3
	mov ecx, r11d
	ror r15, cl
	; FPDIV_R e2, a0
	divpd xmm6, xmm8
	maxpd xmm6, xmm13
	; IADD_R r2, r5
	add r10, r13
	; FPDIV_R e1, a3
	divpd xmm5, xmm11
	maxpd xmm5, xmm13
	; FPSQRT_R e2
	sqrtpd xmm6, xmm6
	; ISUB_R r3, r7
	sub r11, r15
	; FPADD_R f0, a0
	addpd xmm0, xmm8
	; IMUL_M r0, L3[98136]
	imul r8, qword ptr [rsi+98136]
	; IMUL_9C r5, -895487055
	lea r13, [r13+r13*8-895487055]
	; IMULH_R r2, r7
	mov rax, r10
	mul r15
	mov r10, rdx
	; IADD_R r4, r1
	add r12, r9
	; ISDIV_C r0, 494395999
	mov rax, 5007888582388710937
	imul r8
	xor eax, eax
	sar rdx, 27
	sets al
	add rdx, rax
	add r8, rdx
	; FPSWAP_R e0
	shufpd xmm4, xmm4, 1
	; IXOR_R r1, r5
	xor r9, r13
	; COND_R r2, ab(r3, 1932234501)
	xor ecx, ecx
	cmp r11d, 1932234501
	seta cl
	add r10, rcx
	; FPMUL_R e1, a0
	mulpd xmm5, xmm8
	; FPSUB_M f1, L1[r1]
	mov eax, r9d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm1, xmm12
	; FPSUB_R f0, a0
	subpd xmm0, xmm8
	; IROL_R r1, r7
	mov ecx, r15d
	rol r9, cl
	; IADD_RC r0, r5, -2051588680
	lea r8, [r8+r13-2051588680]
	; COND_R r6, of(r5, -795593984)
	xor ecx, ecx
	cmp r13d, -795593984
	seto cl
	add r14, rcx
	; FPADD_R f1, a0
	addpd xmm1, xmm8
	; IMULH_R r7, r3
	mov rax, r15
	mul r11
	mov r15, rdx
	; ISUB_R r7, r4
	sub r15, r12
	; IROL_R r0, r6
	mov ecx, r14d
	rol r8, cl
	; ISDIV_C r1, -675825513
	mov rax, -7326980207007250257
	imul r9
	xor eax, eax
	sar rdx, 28
	sets al
	add rdx, rax
	add r9, rdx
	; ISTORE L1[r6], r3
	mov eax, r14d
	and eax, 16376
	mov qword ptr [rsi+rax], r11
	; IROR_R r4, r3
	mov ecx, r11d
	ror r12, cl
	; IDIV_C r4, 3919226376
	mov rax, r12
	shr rax, 3
	mov rcx, 2526906936258851663
	mul rcx
	shr rdx, 26
	add r12, rdx
	; FPSUB_R f1, a1
	subpd xmm1, xmm9
	; FPSUB_R f0, a0
	subpd xmm0, xmm8
	; IADD_R r0, r2
	add r8, r10
	; IADD_M r4, L1[r2]
	mov eax, r10d
	and eax, 16376
	add r12, qword ptr [rsi+rax]
	; ISTORE L1[r7], r2
	mov eax, r15d
	and eax, 16376
	mov qword ptr [rsi+rax], r10
	; FPSQRT_R e1
	sqrtpd xmm5, xmm5
	; IADD_R r5, r4
	add r13, r12
	; IXOR_R r6, r7
	xor r14, r15
	; ISMULH_R r4, r7
	mov rax, r12
	imul r15
	mov r12, rdx
	; FPSQRT_R e1
	sqrtpd xmm5, xmm5
