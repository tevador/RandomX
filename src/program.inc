	; IMULH_R r1, r0
	mov rax, r9
	mul r8
	mov r9, rdx
	; IMULH_R r4, r5
	mov rax, r12
	mul r13
	mov r12, rdx
	; FMUL_R e0, a1
	mulpd xmm4, xmm9
	; IMUL_9C r6, 933674225
	lea r14, [r14+r14*8+933674225]
	; IROR_R r7, r6
	mov ecx, r14d
	ror r15, cl
	; FSQRT_R e1
	sqrtpd xmm5, xmm5
	; IADD_R r1, r0
	add r9, r8
	; FSCAL_R f1
	xorps xmm1, xmm15
	; IMUL_R r6, r5
	imul r14, r13
	; FSCAL_R f3
	xorps xmm3, xmm15
	; IADD_M r5, L1[r0]
	mov eax, r8d
	and eax, 16376
	add r13, qword ptr [rsi+rax]
	; IMUL_RCP r0, 3332750793
	mov rax, 11886301652177618669
	imul r8, rax
	; ISTORE L1[r3], r0
	mov eax, r11d
	and eax, 16376
	mov qword ptr [rsi+rax], r8
	; FSUB_R f3, a0
	subpd xmm3, xmm8
	; ISUB_R r1, r3
	sub r9, r11
	; ISMULH_R r4, r1
	mov rax, r12
	imul r9
	mov r12, rdx
	; IADD_RC r3, r0, 1262539428
	lea r11, [r11+r8+1262539428]
	; FSWAP_R e1
	shufpd xmm5, xmm5, 1
	; FMUL_R e1, a3
	mulpd xmm5, xmm11
	; FMUL_R e3, a3
	mulpd xmm7, xmm11
	; ISWAP_R r0, r2
	xchg r8, r10
	; COND_R r5, of(r4, 137305269)
	xor ecx, ecx
	cmp r12d, 137305269
	seto cl
	add r13, rcx
	; IMUL_R r6, r4
	imul r14, r12
	; FMUL_R e3, a0
	mulpd xmm7, xmm8
	; FSCAL_R f0
	xorps xmm0, xmm15
	; FADD_R f1, a0
	addpd xmm1, xmm8
	; IADD_R r6, r3
	add r14, r11
	; ISMULH_M r1, L3[777112]
	mov rax, r9
	imul qword ptr [rsi+777112]
	mov r9, rdx
	; FADD_R f1, a1
	addpd xmm1, xmm9
	; FSUB_M f2, L2[r3]
	mov eax, r11d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm2, xmm12
	; IMUL_R r5, r7
	imul r13, r15
	; ISUB_M r1, L1[r3]
	mov eax, r11d
	and eax, 16376
	sub r9, qword ptr [rsi+rax]
	; IXOR_M r1, L1[r6]
	mov eax, r14d
	and eax, 16376
	xor r9, qword ptr [rsi+rax]
	; COND_R r2, ns(r3, 1727033430)
	xor ecx, ecx
	cmp r11d, 1727033430
	setns cl
	add r10, rcx
	; FADD_R f3, a1
	addpd xmm3, xmm9
	; FADD_R f2, a2
	addpd xmm2, xmm10
	; IADD_R r5, -1048707993
	add r13, -1048707993
	; COND_R r2, ge(r5, -1016934677)
	xor ecx, ecx
	cmp r13d, -1016934677
	setge cl
	add r10, rcx
	; FSUB_R f2, a3
	subpd xmm2, xmm11
	; ISUB_M r1, L2[r4]
	mov eax, r12d
	and eax, 262136
	sub r9, qword ptr [rsi+rax]
	; IMUL_R r5, r3
	imul r13, r11
	; FSUB_R f1, a3
	subpd xmm1, xmm11
	; IROR_R r1, r3
	mov ecx, r11d
	ror r9, cl
	; FADD_R f3, a2
	addpd xmm3, xmm10
	; ISUB_R r0, -28376526
	sub r8, -28376526
	; IROR_R r6, r0
	mov ecx, r8d
	ror r14, cl
	; FADD_R f1, a0
	addpd xmm1, xmm8
	; FMUL_R e1, a0
	mulpd xmm5, xmm8
	; IXOR_R r2, r4
	xor r10, r12
	; FSUB_M f1, L1[r2]
	mov eax, r10d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm1, xmm12
	; FSWAP_R f3
	shufpd xmm3, xmm3, 1
	; FSUB_R f3, a0
	subpd xmm3, xmm8
	; ISUB_R r7, r6
	sub r15, r14
	; FADD_R f3, a1
	addpd xmm3, xmm9
	; ISUB_R r1, r7
	sub r9, r15
	; IADD_M r5, L2[r7]
	mov eax, r15d
	and eax, 262136
	add r13, qword ptr [rsi+rax]
	; IADD_RC r1, r3, 145589392
	lea r9, [r9+r11+145589392]
	; FADD_R f2, a1
	addpd xmm2, xmm9
	; FSUB_R f1, a1
	subpd xmm1, xmm9
	; FADD_M f0, L1[r3]
	mov eax, r11d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm0, xmm12
	; FADD_R f3, a1
	addpd xmm3, xmm9
	; FSUB_R f0, a3
	subpd xmm0, xmm11
	; FMUL_R e2, a2
	mulpd xmm6, xmm10
	; FADD_R f2, a1
	addpd xmm2, xmm9
	; IXOR_R r7, r4
	xor r15, r12
	; FSUB_R f1, a3
	subpd xmm1, xmm11
	; IMUL_RCP r0, 3339947118
	mov rax, 11860691159940745144
	imul r8, rax
	; FSCAL_R f2
	xorps xmm2, xmm15
	; IMUL_9C r0, 850304074
	lea r8, [r8+r8*8+850304074]
	; IADD_R r2, r4
	add r10, r12
	; IADD_R r0, -1929760745
	add r8, -1929760745
	; ISTORE L2[r4], r7
	mov eax, r12d
	and eax, 262136
	mov qword ptr [rsi+rax], r15
	; IROR_R r2, r7
	mov ecx, r15d
	ror r10, cl
	; FMUL_R e1, a1
	mulpd xmm5, xmm9
	; FSQRT_R e3
	sqrtpd xmm7, xmm7
	; IXOR_R r0, -1150923249
	xor r8, -1150923249
	; IMUL_9C r7, 586146619
	lea r15, [r15+r15*8+586146619]
	; FSWAP_R f2
	shufpd xmm2, xmm2, 1
	; FSUB_M f3, L1[r6]
	mov eax, r14d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm3, xmm12
	; IXOR_R r0, 292938237
	xor r8, 292938237
	; COND_R r6, no(r6, -2142285576)
	xor ecx, ecx
	cmp r14d, -2142285576
	setno cl
	add r14, rcx
	; IMUL_RCP r3, 670137279
	mov rax, 14778345608621248183
	imul r11, rax
	; ISTORE L1[r1], r5
	mov eax, r9d
	and eax, 16376
	mov qword ptr [rsi+rax], r13
	; COND_R r3, sg(r1, 1638220289)
	xor ecx, ecx
	cmp r9d, 1638220289
	sets cl
	add r11, rcx
	; IXOR_R r4, r2
	xor r12, r10
	; COND_R r2, be(r2, 1131588253)
	xor ecx, ecx
	cmp r10d, 1131588253
	setbe cl
	add r10, rcx
	; IMULH_R r3, r1
	mov rax, r11
	mul r9
	mov r11, rdx
	; COND_R r3, sg(r6, 1528901692)
	xor ecx, ecx
	cmp r14d, 1528901692
	sets cl
	add r11, rcx
	; IMUL_M r6, L2[r4]
	mov eax, r12d
	and eax, 262136
	imul r14, qword ptr [rsi+rax]
	; ISMULH_M r1, L1[r2]
	mov ecx, r10d
	and ecx, 16376
	mov rax, r9
	imul qword ptr [rsi+rcx]
	mov r9, rdx
	; ISUB_M r5, L1[r4]
	mov eax, r12d
	and eax, 16376
	sub r13, qword ptr [rsi+rax]
	; IMUL_RCP r1, 1612208358
	mov rax, 12285658072842024305
	imul r9, rax
	; COND_R r2, lt(r6, -1712049035)
	xor ecx, ecx
	cmp r14d, -1712049035
	setl cl
	add r10, rcx
	; IMUL_RCP r2, 2888266520
	mov rax, 13715521397634789187
	imul r10, rax
	; IADD_M r1, L2[r6]
	mov eax, r14d
	and eax, 262136
	add r9, qword ptr [rsi+rax]
	; FMUL_R e0, a3
	mulpd xmm4, xmm11
	; ISTORE L1[r7], r1
	mov eax, r15d
	and eax, 16376
	mov qword ptr [rsi+rax], r9
	; ISTORE L1[r0], r3
	mov eax, r8d
	and eax, 16376
	mov qword ptr [rsi+rax], r11
	; FSUB_R f0, a1
	subpd xmm0, xmm9
	; FADD_R f2, a2
	addpd xmm2, xmm10
	; FMUL_R e0, a1
	mulpd xmm4, xmm9
	; FMUL_R e2, a0
	mulpd xmm6, xmm8
	; FMUL_R e3, a2
	mulpd xmm7, xmm10
	; IROR_R r5, 21
	ror r13, 21
	; FSQRT_R e1
	sqrtpd xmm5, xmm5
	; ISTORE L1[r3], r1
	mov eax, r11d
	and eax, 16376
	mov qword ptr [rsi+rax], r9
	; IMUL_9C r2, -290275273
	lea r10, [r10+r10*8-290275273]
	; ISUB_M r7, L1[r3]
	mov eax, r11d
	and eax, 16376
	sub r15, qword ptr [rsi+rax]
	; IMUL_R r6, 1301522739
	imul r14, 1301522739
	; ISWAP_R r2, r4
	xchg r10, r12
	; FMUL_R e3, a2
	mulpd xmm7, xmm10
	; IMUL_9C r2, 877307769
	lea r10, [r10+r10*8+877307769]
	; IMUL_R r0, r3
	imul r8, r11
	; IMUL_9C r0, 1293318220
	lea r8, [r8+r8*8+1293318220]
	; FSQRT_R e0
	sqrtpd xmm4, xmm4
	; ISTORE L1[r0], r2
	mov eax, r8d
	and eax, 16376
	mov qword ptr [rsi+rax], r10
	; IMUL_RCP r5, 2071364883
	mov rax, 9562313618003962461
	imul r13, rax
	; FMUL_R e1, a2
	mulpd xmm5, xmm10
	; FSUB_R f1, a3
	subpd xmm1, xmm11
	; FSUB_R f0, a1
	subpd xmm0, xmm9
	; IMULH_R r6, r1
	mov rax, r14
	mul r9
	mov r14, rdx
	; ISTORE L1[r6], r5
	mov eax, r14d
	and eax, 16376
	mov qword ptr [rsi+rax], r13
	; ISTORE L2[r1], r2
	mov eax, r9d
	and eax, 262136
	mov qword ptr [rsi+rax], r10
	; ISUB_M r1, L2[r4]
	mov eax, r12d
	and eax, 262136
	sub r9, qword ptr [rsi+rax]
	; IADD_M r7, L1[r6]
	mov eax, r14d
	and eax, 16376
	add r15, qword ptr [rsi+rax]
	; IADD_RC r2, r0, -1705364403
	lea r10, [r10+r8-1705364403]
	; ISTORE L1[r6], r5
	mov eax, r14d
	and eax, 16376
	mov qword ptr [rsi+rax], r13
	; FSUB_M f0, L1[r5]
	mov eax, r13d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm0, xmm12
	; IXOR_R r1, r3
	xor r9, r11
	; FADD_R f2, a0
	addpd xmm2, xmm8
	; FSCAL_R f2
	xorps xmm2, xmm15
	; ISUB_R r6, -789651909
	sub r14, -789651909
	; COND_R r4, sg(r1, -1404926795)
	xor ecx, ecx
	cmp r9d, -1404926795
	sets cl
	add r12, rcx
	; FSCAL_R f2
	xorps xmm2, xmm15
	; ISUB_R r6, r7
	sub r14, r15
	; IXOR_R r5, r2
	xor r13, r10
	; IROR_R r6, r5
	mov ecx, r13d
	ror r14, cl
	; FSUB_R f1, a2
	subpd xmm1, xmm10
	; IMUL_M r4, L1[r5]
	mov eax, r13d
	and eax, 16376
	imul r12, qword ptr [rsi+rax]
	; FSUB_R f3, a0
	subpd xmm3, xmm8
	; FSWAP_R e1
	shufpd xmm5, xmm5, 1
	; IADD_RC r6, r5, 1744830258
	lea r14, [r14+r13+1744830258]
	; FSUB_R f3, a0
	subpd xmm3, xmm8
	; ISUB_R r7, r0
	sub r15, r8
	; FSUB_R f1, a3
	subpd xmm1, xmm11
	; IMUL_9C r4, 241775739
	lea r12, [r12+r12*8+241775739]
	; FADD_R f0, a0
	addpd xmm0, xmm8
	; IMUL_R r4, r3
	imul r12, r11
	; IMUL_RCP r4, 2389176791
	mov rax, 16580640414036304271
	imul r12, rax
	; FSCAL_R f1
	xorps xmm1, xmm15
	; FSUB_R f2, a1
	subpd xmm2, xmm9
	; ISTORE L2[r2], r0
	mov eax, r10d
	and eax, 262136
	mov qword ptr [rsi+rax], r8
	; IXOR_M r5, L1[r7]
	mov eax, r15d
	and eax, 16376
	xor r13, qword ptr [rsi+rax]
	; IMULH_M r4, L1[r1]
	mov ecx, r9d
	and ecx, 16376
	mov rax, r12
	mul qword ptr [rsi+rcx]
	mov r12, rdx
	; FMUL_R e2, a1
	mulpd xmm6, xmm9
	; IXOR_R r0, r5
	xor r8, r13
	; IROR_R r0, r7
	mov ecx, r15d
	ror r8, cl
	; IADD_RC r6, r5, 472588845
	lea r14, [r14+r13+472588845]
	; FADD_R f0, a0
	addpd xmm0, xmm8
	; FSCAL_R f0
	xorps xmm0, xmm15
	; IROR_R r2, r1
	mov ecx, r9d
	ror r10, cl
	; IADD_RC r2, r1, 1968510355
	lea r10, [r10+r9+1968510355]
	; FMUL_R e0, a0
	mulpd xmm4, xmm8
	; ISUB_R r7, r1
	sub r15, r9
	; IADD_RC r4, r7, 1111936914
	lea r12, [r12+r15+1111936914]
	; IADD_RC r7, r3, 373642756
	lea r15, [r15+r11+373642756]
	; FSUB_R f0, a0
	subpd xmm0, xmm8
	; IMUL_RCP r6, 3388328460
	mov rax, 11691334451422153092
	imul r14, rax
	; FSWAP_R e1
	shufpd xmm5, xmm5, 1
	; IADD_RC r7, r5, -644292398
	lea r15, [r15+r13-644292398]
	; IMUL_9C r7, -1398596563
	lea r15, [r15+r15*8-1398596563]
	; FADD_R f0, a3
	addpd xmm0, xmm11
	; FDIV_M e1, L1[r5]
	mov eax, r13d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	andps xmm12, xmm14
	divpd xmm5, xmm12
	maxpd xmm5, xmm13
	; IXOR_M r2, L1[r5]
	mov eax, r13d
	and eax, 16376
	xor r10, qword ptr [rsi+rax]
	; IADD_R r5, r6
	add r13, r14
	; IROR_R r4, r0
	mov ecx, r8d
	ror r12, cl
	; IXOR_R r0, r6
	xor r8, r14
	; IMUL_RCP r1, 1035942442
	mov rax, 9559913671615977868
	imul r9, rax
	; IMUL_9C r1, 105267179
	lea r9, [r9+r9*8+105267179]
	; IMUL_M r1, L1[r2]
	mov eax, r10d
	and eax, 16376
	imul r9, qword ptr [rsi+rax]
	; COND_R r6, be(r7, 1344676209)
	xor ecx, ecx
	cmp r15d, 1344676209
	setbe cl
	add r14, rcx
	; IADD_R r6, r1
	add r14, r9
	; IROR_R r5, r1
	mov ecx, r9d
	ror r13, cl
	; ISMULH_R r0, r6
	mov rax, r8
	imul r14
	mov r8, rdx
	; IXOR_R r6, r7
	xor r14, r15
	; FSUB_R f1, a3
	subpd xmm1, xmm11
	; IMUL_9C r1, 1991866007
	lea r9, [r9+r9*8+1991866007]
	; IMUL_RCP r2, 4139294400
	mov rax, 9570249764581173254
	imul r10, rax
	; FSWAP_R f0
	shufpd xmm0, xmm0, 1
	; ISUB_R r5, r2
	sub r13, r10
	; COND_R r6, lt(r1, -834783176)
	xor ecx, ecx
	cmp r9d, -834783176
	setl cl
	add r14, rcx
	; ISTORE L2[r7], r3
	mov eax, r15d
	and eax, 262136
	mov qword ptr [rsi+rax], r11
	; FADD_R f2, a2
	addpd xmm2, xmm10
	; FSCAL_R f1
	xorps xmm1, xmm15
	; IMUL_R r7, r4
	imul r15, r12
	; IMUL_RCP r4, 3027698566
	mov rax, 13083892069700893994
	imul r12, rax
	; IMULH_M r2, L1[r3]
	mov ecx, r11d
	and ecx, 16376
	mov rax, r10
	mul qword ptr [rsi+rcx]
	mov r10, rdx
	; IADD_M r6, L1[r1]
	mov eax, r9d
	and eax, 16376
	add r14, qword ptr [rsi+rax]
	; IMUL_M r3, L1[r1]
	mov eax, r9d
	and eax, 16376
	imul r11, qword ptr [rsi+rax]
	; ISTORE L1[r7], r5
	mov eax, r15d
	and eax, 16376
	mov qword ptr [rsi+rax], r13
	; IADD_RC r3, r1, -183791073
	lea r11, [r11+r9-183791073]
	; IMUL_9C r6, 1353963989
	lea r14, [r14+r14*8+1353963989]
	; ISUB_R r2, r3
	sub r10, r11
	; IMUL_R r2, r1
	imul r10, r9
	; IMULH_R r6, r4
	mov rax, r14
	mul r12
	mov r14, rdx
	; ISMULH_R r6, r4
	mov rax, r14
	imul r12
	mov r14, rdx
	; IADD_R r7, r4
	add r15, r12
	; FMUL_R e3, a1
	mulpd xmm7, xmm9
	; FADD_R f1, a2
	addpd xmm1, xmm10
	; IADD_R r5, r6
	add r13, r14
	; IADD_RC r4, r0, -1810659257
	lea r12, [r12+r8-1810659257]
	; IROR_R r2, r5
	mov ecx, r13d
	ror r10, cl
	; FADD_R f2, a2
	addpd xmm2, xmm10
	; FSWAP_R e2
	shufpd xmm6, xmm6, 1
	; FADD_M f0, L1[r2]
	mov eax, r10d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm0, xmm12
	; IADD_R r0, 52817665
	add r8, 52817665
	; IMUL_RCP r6, 3388141601
	mov rax, 11691979238837063231
	imul r14, rax
	; IMUL_RCP r3, 1356467790
	mov rax, 14601924774465956466
	imul r11, rax
	; IADD_RC r7, r4, -2056421852
	lea r15, [r15+r12-2056421852]
	; FSUB_M f1, L2[r4]
	mov eax, r12d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm1, xmm12
	; ISWAP_R r1, r5
	xchg r9, r13
	; ISTORE L2[r3], r5
	mov eax, r11d
	and eax, 262136
	mov qword ptr [rsi+rax], r13
	; FMUL_R e0, a3
	mulpd xmm4, xmm11
	; IADD_RC r1, r4, -129008866
	lea r9, [r9+r12-129008866]
	; COND_R r6, no(r4, 311828213)
	xor ecx, ecx
	cmp r12d, 311828213
	setno cl
	add r14, rcx
	; FSWAP_R e2
	shufpd xmm6, xmm6, 1
	; IADD_RC r2, r2, 498744396
	lea r10, [r10+r10+498744396]
	; IADD_RC r2, r3, 1515945097
	lea r10, [r10+r11+1515945097]
	; FMUL_R e0, a2
	mulpd xmm4, xmm10
	; ISTORE L2[r5], r7
	mov eax, r13d
	and eax, 262136
	mov qword ptr [rsi+rax], r15
	; IMUL_M r7, L2[r0]
	mov eax, r8d
	and eax, 262136
	imul r15, qword ptr [rsi+rax]
	; IADD_R r0, r2
	add r8, r10
	; IADD_RC r7, r3, 1081450346
	lea r15, [r15+r11+1081450346]
	; FADD_R f1, a3
	addpd xmm1, xmm11
	; FSCAL_R f3
	xorps xmm3, xmm15
	; FADD_M f3, L2[r7]
	mov eax, r15d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm3, xmm12
	; FSUB_R f3, a0
	subpd xmm3, xmm8
	; COND_M r2, of(L1[r5], -255033167)
	xor ecx, ecx
	mov eax, r13d
	and eax, 16376
	cmp dword ptr [rsi+rax], -255033167
	seto cl
	add r10, rcx
	; FSUB_R f1, a1
	subpd xmm1, xmm9
	; IADD_R r2, r5
	add r10, r13
	; FSQRT_R e2
	sqrtpd xmm6, xmm6
	; IMUL_9C r2, 1521722302
	lea r10, [r10+r10*8+1521722302]
	; FADD_R f0, a3
	addpd xmm0, xmm11
	; ISUB_R r0, r5
	sub r8, r13
	; FADD_R f2, a0
	addpd xmm2, xmm8
	; ISWAP_R r6, r0
	xchg r14, r8
	; IADD_RC r1, r4, -693164762
	lea r9, [r9+r12-693164762]
	; FDIV_M e0, L2[r2]
	mov eax, r10d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	andps xmm12, xmm14
	divpd xmm4, xmm12
	maxpd xmm4, xmm13
	; IMUL_9C r4, -1849458799
	lea r12, [r12+r12*8-1849458799]
	; IADD_RC r1, r4, -651820510
	lea r9, [r9+r12-651820510]
	; IMULH_R r6, r6
	mov rax, r14
	mul r14
	mov r14, rdx
	; FSUB_M f3, L2[r0]
	mov eax, r8d
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm3, xmm12
	; FSUB_R f0, a2
	subpd xmm0, xmm10
	; FDIV_M e3, L1[r0]
	mov eax, r8d
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	andps xmm12, xmm14
	divpd xmm7, xmm12
	maxpd xmm7, xmm13
	; IADD_M r3, L1[r7]
	mov eax, r15d
	and eax, 16376
	add r11, qword ptr [rsi+rax]
	; IXOR_M r2, L2[r6]
	mov eax, r14d
	and eax, 262136
	xor r10, qword ptr [rsi+rax]
