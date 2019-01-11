rx_i_0: ;CALL
	dec ebx
	jz rx_finish
	xor r9, 0ca9788ah
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_0
	call rx_read_l1
rx_body_0:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r12d
	xor eax, 01a8e4171h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp r11d, 445530481
	ja short rx_i_1
	call rx_i_30

rx_i_1: ;IMULH_64
	dec ebx
	jz rx_finish
	xor r15, 06afc2fa4h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_1
	call rx_read_l1
rx_body_1:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r10
	imul rcx
	mov rax, rdx
	mov r12, rax

rx_i_2: ;JUMP
	dec ebx
	jz rx_finish
	xor r15, 097210f7bh
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_2
	call rx_read_l1
rx_body_2:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r9d
	xor eax, 05060ccf7h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp r11d, 1348521207
	jno rx_i_47

rx_i_3: ;FPDIV
	dec ebx
	jz rx_finish
	xor r13, 082c73195h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_3
	call rx_read_l1
rx_body_3:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	divpd xmm0, xmm9
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm8, xmm0
	mov eax, r8d
	xor eax, 06bb1a0b2h
	and eax, 32767
	movhpd qword ptr [rsi + rax * 8], xmm8

rx_i_4: ;MULH_64
	dec ebx
	jz rx_finish
	xor r14, 077daefb4h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_4
	call rx_read_l1
rx_body_4:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r14
	mul rcx
	mov rax, rdx
	mov rcx, rax
	mov eax, r9d
	xor eax, 06ce10c20h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_5: ;MUL_32
	dec ebx
	jz rx_finish
	xor r15, 0379f9ee0h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_5
	call rx_read_l2
rx_body_5:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, eax
	mov eax, r12d
	imul rax, rcx
	mov r12, rax

rx_i_6: ;MUL_64
	dec ebx
	jz rx_finish
	xor r8, 03bae7272h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_6
	call rx_read_l1
rx_body_6:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r15
	mov rcx, rax
	mov eax, r9d
	xor eax, 098a649d1h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_7: ;FPADD
	dec ebx
	jz rx_finish
	xor r10, 0e264ed81h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_7
	call rx_read_l1
rx_body_7:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm6
	movaps xmm6, xmm0
	mov eax, r14d
	xor eax, 057c8c41bh
	and eax, 32767
	movlpd qword ptr [rsi + rax * 8], xmm6

rx_i_8: ;XOR_64
	dec ebx
	jz rx_finish
	xor r13, 068c1e5d2h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_8
	call rx_read_l1
rx_body_8:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	xor rax, 1344700093
	mov rcx, rax
	mov eax, r12d
	xor eax, 050267ebdh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_9: ;IMULH_64
	dec ebx
	jz rx_finish
	xor r14, 085121c54h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_9
	call rx_read_l1
rx_body_9:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, 565870810
	imul rcx
	mov rax, rdx
	mov r10, rax

rx_i_10: ;AND_64
	dec ebx
	jz rx_finish
	xor r8, 052efde3eh
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_10
	call rx_read_l1
rx_body_10:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	and rax, -727859809
	mov r13, rax

rx_i_11: ;FPADD
	dec ebx
	jz rx_finish
	xor r10, 0a9bf8aa1h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_11
	call rx_read_l2
rx_body_11:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm5
	movaps xmm4, xmm0
	mov eax, r12d
	xor eax, 0852d40d8h
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm4

rx_i_12: ;FPSQRT
	dec ebx
	jz rx_finish
	xor r10, 0db2691ch
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_12
	call rx_read_l2
rx_body_12:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	andps xmm0, xmm10
	sqrtpd xmm8, xmm0
	mov eax, r8d
	xor eax, 096dc67c9h
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm8

rx_i_13: ;FPADD
	dec ebx
	jz rx_finish
	xor r12, 061c0d34dh
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_13
	call rx_read_l1
rx_body_13:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm3
	movaps xmm9, xmm0

rx_i_14: ;XOR_64
	dec ebx
	jz rx_finish
	xor r10, 0e761d1beh
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_14
	call rx_read_l1
rx_body_14:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	xor rax, r9
	mov rcx, rax
	mov eax, r10d
	xor eax, 03c1a72f8h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_15: ;RET
	dec ebx
	jz rx_finish
	xor r11, 074ddb688h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_15
	call rx_read_l2
rx_body_15:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r14d
	xor eax, 0468b38b8h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx
	cmp rsp, rdi
	je short rx_i_16
	ret

rx_i_16: ;ADD_64
	dec ebx
	jz rx_finish
	xor r14, 06be90627h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_16
	call rx_read_l1
rx_body_16:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, r10
	mov rcx, rax
	mov eax, r9d
	xor eax, 0d7e75aeh
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_17: ;FPMUL
	dec ebx
	jz rx_finish
	xor r11, 0fbc6fc35h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_17
	call rx_read_l1
rx_body_17:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm4
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm4, xmm0
	mov eax, r12d
	xor eax, 0f77ffe16h
	and eax, 32767
	movlpd qword ptr [rsi + rax * 8], xmm4

rx_i_18: ;FPSUB
	dec ebx
	jz rx_finish
	xor r14, 0c28ca080h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_18
	call rx_read_l1
rx_body_18:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm4
	movaps xmm3, xmm0
	mov eax, r11d
	xor eax, 0869baa81h
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm3

rx_i_19: ;FPSUB
	dec ebx
	jz rx_finish
	xor r13, 0ac009c30h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_19
	call rx_read_l1
rx_body_19:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm8
	movaps xmm7, xmm0

rx_i_20: ;FPSUB
	dec ebx
	jz rx_finish
	xor r13, 0ecca967dh
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_20
	call rx_read_l1
rx_body_20:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm2
	movaps xmm7, xmm0
	mov eax, r15d
	xor eax, 0aad81365h
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm7

rx_i_21: ;ROR_64
	dec ebx
	jz rx_finish
	xor r8, 0977f0284h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_21
	call rx_read_l2
rx_body_21:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r9
	ror rax, cl
	mov rcx, rax
	mov eax, r15d
	xor eax, 0db5e0aafh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_22: ;ADD_64
	dec ebx
	jz rx_finish
	xor r13, 080bdfefah
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_22
	call rx_read_l1
rx_body_22:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, r8
	mov rcx, rax
	mov eax, r10d
	xor eax, 0cfa09799h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_23: ;MUL_64
	dec ebx
	jz rx_finish
	xor r15, 0e1e0d3c4h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_23
	call rx_read_l1
rx_body_23:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r11
	mov r8, rax

rx_i_24: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r8, 070d3b8c7h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_24
	call rx_read_l1
rx_body_24:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r15d
	imul rax, rcx
	mov rcx, rax
	mov eax, r15d
	xor eax, 099b77a68h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_25: ;FPMUL
	dec ebx
	jz rx_finish
	xor r12, 01cf77a04h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_25
	call rx_read_l2
rx_body_25:
	xor rbp, rcx
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm9
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm6, xmm0
	mov eax, r14d
	xor eax, 0baf5c2d4h
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm6

rx_i_26: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r11, 0e311468ch
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_26
	call rx_read_l1
rx_body_26:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r13d
	imul rax, rcx
	mov rcx, rax
	mov eax, r9d
	xor eax, 0306ff9ech
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_27: ;FPMUL
	dec ebx
	jz rx_finish
	xor r12, 01fd9911ah
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_27
	call rx_read_l2
rx_body_27:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm3
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm6, xmm0

rx_i_28: ;AND_32
	dec ebx
	jz rx_finish
	xor r13, 067df757eh
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_28
	call rx_read_l1
rx_body_28:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	and eax, r13d
	mov r14, rax

rx_i_29: ;ADD_64
	dec ebx
	jz rx_finish
	xor r12, 0be2e7c42h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_29
	call rx_read_l2
rx_body_29:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	add rax, 1944166515
	mov r14, rax

rx_i_30: ;FPADD
	dec ebx
	jz rx_finish
	xor r11, 084d067f7h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_30
	call rx_read_l1
rx_body_30:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm3
	movaps xmm7, xmm0

rx_i_31: ;ROR_64
	dec ebx
	jz rx_finish
	xor r14, 0d352ce37h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_31
	call rx_read_l2
rx_body_31:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r11
	ror rax, cl
	mov r14, rax

rx_i_32: ;AND_32
	dec ebx
	jz rx_finish
	xor r12, 0a1f248dah
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_32
	call rx_read_l1
rx_body_32:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	and eax, -1936869641
	mov r9, rax

rx_i_33: ;MUL_64
	dec ebx
	jz rx_finish
	xor r9, 0554720fch
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_33
	call rx_read_l2
rx_body_33:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r15
	mov r12, rax

rx_i_34: ;CALL
	dec ebx
	jz rx_finish
	xor r13, 0665e91f1h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_34
	call rx_read_l1
rx_body_34:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov r15, rax
	cmp r14d, -380224718
	jns short rx_i_35
	call rx_i_108

rx_i_35: ;CALL
	dec ebx
	jz rx_finish
	xor r15, 05ef1be79h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_35
	call rx_read_l1
rx_body_35:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov r8, rax
	cmp r9d, -2040787098
	jns short rx_i_36
	call rx_i_58

rx_i_36: ;FPMUL
	dec ebx
	jz rx_finish
	xor r8, 012ec7e3ah
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_36
	call rx_read_l1
rx_body_36:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm6
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm7, xmm0

rx_i_37: ;FPSUB
	dec ebx
	jz rx_finish
	xor r12, 0d0706601h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_37
	call rx_read_l1
rx_body_37:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm2
	movaps xmm9, xmm0
	mov eax, r9d
	xor eax, 0bca81c78h
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm9

rx_i_38: ;SUB_64
	dec ebx
	jz rx_finish
	xor r9, 064056913h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_38
	call rx_read_l1
rx_body_38:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	sub rax, r14
	mov r10, rax

rx_i_39: ;ADD_64
	dec ebx
	jz rx_finish
	xor r14, 02c1f1eb0h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_39
	call rx_read_l1
rx_body_39:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, r14
	mov r14, rax

rx_i_40: ;CALL
	dec ebx
	jz rx_finish
	xor r10, 068fd9009h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_40
	call rx_read_l1
rx_body_40:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r9d
	xor eax, 0b2a27eceh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp r12d, -1297973554
	jns short rx_i_41
	call rx_i_90

rx_i_41: ;JUMP
	dec ebx
	jz rx_finish
	xor r9, 037a30933h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_41
	call rx_read_l1
rx_body_41:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov r9, rax
	cmp r14d, -1070581824
	jo rx_i_127

rx_i_42: ;FPADD
	dec ebx
	jz rx_finish
	xor r15, 0bc1de9f6h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_42
	call rx_read_l1
rx_body_42:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm6
	movaps xmm6, xmm0

rx_i_43: ;ADD_32
	dec ebx
	jz rx_finish
	xor r12, 02b2a2eech
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_43
	call rx_read_l1
rx_body_43:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add eax, 1693705407
	mov rcx, rax
	mov eax, r11d
	xor eax, 064f3e4bfh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_44: ;SAR_64
	dec ebx
	jz rx_finish
	xor r11, 0685817abh
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_44
	call rx_read_l1
rx_body_44:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r9
	sar rax, cl
	mov r15, rax

rx_i_45: ;FPSUB
	dec ebx
	jz rx_finish
	xor r12, 08cd244ebh
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_45
	call rx_read_l2
rx_body_45:
	xor rbp, rcx
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm2
	movaps xmm5, xmm0

rx_i_46: ;ADD_64
	dec ebx
	jz rx_finish
	xor r8, 06d8f4254h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_46
	call rx_read_l2
rx_body_46:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	add rax, r9
	mov rcx, rax
	mov eax, r8d
	xor eax, 0e9f58436h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_47: ;JUMP
	dec ebx
	jz rx_finish
	xor r12, 05ba232c6h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_47
	call rx_read_l2
rx_body_47:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r13d
	xor eax, 071ba231h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx
	cmp r10d, 119251505
	jbe rx_i_131

rx_i_48: ;FPDIV
	dec ebx
	jz rx_finish
	xor r8, 0aaed618fh
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_48
	call rx_read_l1
rx_body_48:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	divpd xmm0, xmm3
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm9, xmm0
	mov eax, r9d
	xor eax, 020e5d9e9h
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm9

rx_i_49: ;FPSUB
	dec ebx
	jz rx_finish
	xor r8, 0f96c6a45h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_49
	call rx_read_l1
rx_body_49:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm3
	movaps xmm5, xmm0

rx_i_50: ;AND_64
	dec ebx
	jz rx_finish
	xor r9, 0da3e4842h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_50
	call rx_read_l1
rx_body_50:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	and rax, r10
	mov rcx, rax
	mov eax, r15d
	xor eax, 06ac56a2ah
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_51: ;SUB_64
	dec ebx
	jz rx_finish
	xor r10, 0302b676ah
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_51
	call rx_read_l2
rx_body_51:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	sub rax, 419241919
	mov r15, rax

rx_i_52: ;FPSQRT
	dec ebx
	jz rx_finish
	xor r11, 0fa88f48bh
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_52
	call rx_read_l2
rx_body_52:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	andps xmm0, xmm10
	sqrtpd xmm7, xmm0

rx_i_53: ;RET
	dec ebx
	jz rx_finish
	xor r13, 03dff9b9eh
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_53
	call rx_read_l1
rx_body_53:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov r13, rax
	cmp rsp, rdi
	je short rx_i_54
	ret

rx_i_54: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r11, 060638de0h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_54
	call rx_read_l2
rx_body_54:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	mov rax, 282209221
	imul rax, rcx
	mov rcx, rax
	mov eax, r12d
	xor eax, 010d22bc5h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_55: ;FPMUL
	dec ebx
	jz rx_finish
	xor r10, 0dda983d4h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_55
	call rx_read_l1
rx_body_55:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm5
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm3, xmm0
	mov eax, r11d
	xor eax, 07c79cddh
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm3

rx_i_56: ;IMULH_64
	dec ebx
	jz rx_finish
	xor r14, 0f1456b8eh
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_56
	call rx_read_l1
rx_body_56:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r15
	imul rcx
	mov rax, rdx
	mov rcx, rax
	mov eax, r8d
	xor eax, 0fcf95491h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_57: ;SUB_32
	dec ebx
	jz rx_finish
	xor r9, 010dc4571h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_57
	call rx_read_l2
rx_body_57:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	sub eax, r14d
	mov rcx, rax
	mov eax, r15d
	xor eax, 0a426387h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_58: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r14, 0bcec0ebah
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_58
	call rx_read_l2
rx_body_58:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r13d
	imul rax, rcx
	mov r8, rax

rx_i_59: ;FPSUB
	dec ebx
	jz rx_finish
	xor r11, 0980dd402h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_59
	call rx_read_l1
rx_body_59:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm8
	movaps xmm7, xmm0

rx_i_60: ;CALL
	dec ebx
	jz rx_finish
	xor r15, 03de14d1eh
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_60
	call rx_read_l1
rx_body_60:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r11d
	xor eax, 07bb60f45h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp r11d, 2075529029
	jo short rx_i_61
	call rx_i_116

rx_i_61: ;JUMP
	dec ebx
	jz rx_finish
	xor r13, 05058ce64h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_61
	call rx_read_l1
rx_body_61:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov r11, rax
	cmp r15d, 1933164545
	jns rx_i_120

rx_i_62: ;FPSUB
	dec ebx
	jz rx_finish
	xor r15, 0c3089414h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_62
	call rx_read_l1
rx_body_62:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm8
	movaps xmm2, xmm0
	mov eax, r10d
	xor eax, 05c4789e3h
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm2

rx_i_63: ;FPSUB
	dec ebx
	jz rx_finish
	xor r9, 065cf272eh
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_63
	call rx_read_l1
rx_body_63:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm7
	movaps xmm8, xmm0

rx_i_64: ;SUB_64
	dec ebx
	jz rx_finish
	xor r13, 0ae54dfbfh
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_64
	call rx_read_l1
rx_body_64:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	sub rax, r15
	mov r9, rax

rx_i_65: ;JUMP
	dec ebx
	jz rx_finish
	xor r13, 07b366ce6h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_65
	call rx_read_l1
rx_body_65:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov r11, rax
	cmp r8d, 1498056607
	js rx_i_129

rx_i_66: ;FPDIV
	dec ebx
	jz rx_finish
	xor r15, 015a1b689h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_66
	call rx_read_l2
rx_body_66:
	xor rbp, rcx
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	divpd xmm0, xmm3
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm9, xmm0
	mov eax, r9d
	xor eax, 07305e78h
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm9

rx_i_67: ;JUMP
	dec ebx
	jz rx_finish
	xor r14, 088393ba0h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_67
	call rx_read_l1
rx_body_67:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov r9, rax
	cmp r13d, 2031541081
	jns rx_i_79

rx_i_68: ;FPADD
	dec ebx
	jz rx_finish
	xor r13, 03aa5c3a4h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_68
	call rx_read_l1
rx_body_68:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm2
	movaps xmm4, xmm0
	mov eax, r12d
	xor eax, 03c51ef39h
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm4

rx_i_69: ;FPADD
	dec ebx
	jz rx_finish
	xor r15, 0376c9c27h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_69
	call rx_read_l2
rx_body_69:
	xor rbp, rcx
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm5
	movaps xmm8, xmm0

rx_i_70: ;MUL_64
	dec ebx
	jz rx_finish
	xor r8, 0bbbec3fah
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_70
	call rx_read_l2
rx_body_70:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r9
	mov r13, rax

rx_i_71: ;FPMUL
	dec ebx
	jz rx_finish
	xor r14, 0e9efb350h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_71
	call rx_read_l1
rx_body_71:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm5
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm7, xmm0

rx_i_72: ;JUMP
	dec ebx
	jz rx_finish
	xor r13, 0f4e51e28h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_72
	call rx_read_l1
rx_body_72:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r11d
	xor eax, 0da624dd9h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp r9d, -631091751
	jno rx_i_191

rx_i_73: ;FPDIV
	dec ebx
	jz rx_finish
	xor r12, 0c24ddbd4h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_73
	call rx_read_l2
rx_body_73:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	divpd xmm0, xmm3
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm2, xmm0

rx_i_74: ;MUL_64
	dec ebx
	jz rx_finish
	xor r8, 04c4b0c7fh
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_74
	call rx_read_l1
rx_body_74:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, rax, -1431647438
	mov rcx, rax
	mov eax, r9d
	xor eax, 0aaaacb32h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_75: ;CALL
	dec ebx
	jz rx_finish
	xor r14, 03bcc02e3h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_75
	call rx_read_l2
rx_body_75:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov r13, rax
	cmp r11d, -1160798683
	jno short rx_i_76
	call rx_i_202

rx_i_76: ;FPADD
	dec ebx
	jz rx_finish
	xor r11, 04b0ff63eh
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_76
	call rx_read_l1
rx_body_76:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm3
	movaps xmm7, xmm0
	mov eax, r15d
	xor eax, 083bc0396h
	and eax, 32767
	movlpd qword ptr [rsi + rax * 8], xmm7

rx_i_77: ;RET
	dec ebx
	jz rx_finish
	xor r14, 0b956b3e8h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_77
	call rx_read_l2
rx_body_77:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r11d
	xor eax, 03a92bc7ah
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp rsp, rdi
	je short rx_i_78
	ret

rx_i_78: ;MULH_64
	dec ebx
	jz rx_finish
	xor r9, 0edeca680h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_78
	call rx_read_l1
rx_body_78:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r8
	mul rcx
	mov rax, rdx
	mov r15, rax

rx_i_79: ;CALL
	dec ebx
	jz rx_finish
	xor r11, 0fbdddcb5h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_79
	call rx_read_l1
rx_body_79:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r11d
	xor eax, 06b4a7b43h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp r13d, 1800043331
	ja short rx_i_80
	call rx_i_93

rx_i_80: ;ROR_64
	dec ebx
	jz rx_finish
	xor r13, 09cec97a1h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_80
	call rx_read_l2
rx_body_80:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	ror rax, 4
	mov rcx, rax
	mov eax, r11d
	xor eax, 01a681d13h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_81: ;AND_64
	dec ebx
	jz rx_finish
	xor r15, 078228167h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_81
	call rx_read_l1
rx_body_81:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	and rax, r13
	mov r8, rax

rx_i_82: ;JUMP
	dec ebx
	jz rx_finish
	xor r11, 078cae1ffh
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_82
	call rx_read_l1
rx_body_82:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r10d
	xor eax, 0fbe39afbh
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx
	cmp r12d, -68969733
	jo rx_i_145

rx_i_83: ;IMULH_64
	dec ebx
	jz rx_finish
	xor r10, 0d9b6a533h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_83
	call rx_read_l1
rx_body_83:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r10
	imul rcx
	mov rax, rdx
	mov r12, rax

rx_i_84: ;SAR_64
	dec ebx
	jz rx_finish
	xor r15, 0e9e75336h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_84
	call rx_read_l2
rx_body_84:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r10
	sar rax, cl
	mov rcx, rax
	mov eax, r13d
	xor eax, 0ec5c52e6h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_85: ;MUL_64
	dec ebx
	jz rx_finish
	xor r13, 04c0d378ah
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_85
	call rx_read_l1
rx_body_85:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r8
	mov r10, rax

rx_i_86: ;AND_64
	dec ebx
	jz rx_finish
	xor r11, 04386e368h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_86
	call rx_read_l1
rx_body_86:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	and rax, r8
	mov rcx, rax
	mov eax, r12d
	xor eax, 0a90410e4h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_87: ;ADD_32
	dec ebx
	jz rx_finish
	xor r9, 0d75a0ecfh
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_87
	call rx_read_l1
rx_body_87:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add eax, r12d
	mov r8, rax

rx_i_88: ;ROR_64
	dec ebx
	jz rx_finish
	xor r9, 031bb7f7ah
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_88
	call rx_read_l1
rx_body_88:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r14
	ror rax, cl
	mov r9, rax

rx_i_89: ;MUL_64
	dec ebx
	jz rx_finish
	xor r9, 03b45ecebh
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_89
	call rx_read_l2
rx_body_89:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r8
	mov rcx, rax
	mov eax, r10d
	xor eax, 0e67532afh
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_90: ;FPADD
	dec ebx
	jz rx_finish
	xor r12, 0ee08e76bh
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_90
	call rx_read_l1
rx_body_90:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm3
	movaps xmm6, xmm0

rx_i_91: ;FPMUL
	dec ebx
	jz rx_finish
	xor r9, 042e28e94h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_91
	call rx_read_l1
rx_body_91:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm2
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm4, xmm0

rx_i_92: ;JUMP
	dec ebx
	jz rx_finish
	xor r8, 0729260e1h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_92
	call rx_read_l2
rx_body_92:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov r12, rax
	cmp r14d, 1288893603
	jge rx_i_170

rx_i_93: ;FPADD
	dec ebx
	jz rx_finish
	xor r8, 0bfcebaf4h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_93
	call rx_read_l1
rx_body_93:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm2
	movaps xmm2, xmm0
	mov eax, r10d
	xor eax, 07e48a0d8h
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm2

rx_i_94: ;CALL
	dec ebx
	jz rx_finish
	xor r13, 0ea326630h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_94
	call rx_read_l1
rx_body_94:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov r8, rax
	cmp r13d, -343122976
	jns short rx_i_95
	call rx_i_157

rx_i_95: ;MUL_64
	dec ebx
	jz rx_finish
	xor r13, 0b5451a2dh
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_95
	call rx_read_l1
rx_body_95:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r10
	mov rcx, rax
	mov eax, r15d
	xor eax, 01023aa04h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_96: ;MUL_32
	dec ebx
	jz rx_finish
	xor r11, 04f912ef8h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_96
	call rx_read_l2
rx_body_96:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, eax
	mov eax, -1354397081
	imul rax, rcx
	mov r11, rax

rx_i_97: ;FPDIV
	dec ebx
	jz rx_finish
	xor r15, 0acc45b3bh
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_97
	call rx_read_l1
rx_body_97:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	divpd xmm0, xmm9
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm5, xmm0
	mov eax, r13d
	xor eax, 0c477e850h
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm5

rx_i_98: ;SUB_64
	dec ebx
	jz rx_finish
	xor r14, 09900a4e8h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_98
	call rx_read_l1
rx_body_98:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	sub rax, r15
	mov r14, rax

rx_i_99: ;FPMUL
	dec ebx
	jz rx_finish
	xor r9, 0841b2984h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_99
	call rx_read_l2
rx_body_99:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm6
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm4, xmm0
	mov eax, r12d
	xor eax, 04c21df83h
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm4

rx_i_100: ;ADD_64
	dec ebx
	jz rx_finish
	xor r15, 07ebea48fh
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_100
	call rx_read_l1
rx_body_100:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, r9
	mov r14, rax

rx_i_101: ;SUB_64
	dec ebx
	jz rx_finish
	xor r10, 0631209d3h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_101
	call rx_read_l1
rx_body_101:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	sub rax, r8
	mov r11, rax

rx_i_102: ;FPMUL
	dec ebx
	jz rx_finish
	xor r10, 0e50bf07ah
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_102
	call rx_read_l1
rx_body_102:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm3
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm7, xmm0

rx_i_103: ;MUL_64
	dec ebx
	jz rx_finish
	xor r10, 02b7096f1h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_103
	call rx_read_l1
rx_body_103:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r13
	mov rcx, rax
	mov eax, r15d
	xor eax, 0e4dd92b6h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_104: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r11, 075deaf71h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_104
	call rx_read_l1
rx_body_104:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	mov rax, -1913070089
	imul rax, rcx
	mov rcx, rax
	mov eax, r15d
	xor eax, 08df8ddf7h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_105: ;MULH_64
	dec ebx
	jz rx_finish
	xor r13, 036a51f72h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_105
	call rx_read_l1
rx_body_105:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r15
	mul rcx
	mov rax, rdx
	mov rcx, rax
	mov eax, r14d
	xor eax, 09c8724edh
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_106: ;FPMUL
	dec ebx
	jz rx_finish
	xor r11, 07b512986h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_106
	call rx_read_l1
rx_body_106:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm3
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm4, xmm0
	mov eax, r12d
	xor eax, 03cb2505h
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm4

rx_i_107: ;JUMP
	dec ebx
	jz rx_finish
	xor r12, 0f1d2e50h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_107
	call rx_read_l1
rx_body_107:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r14d
	xor eax, 07243ab81h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp r11d, 1917037441
	jl rx_i_143

rx_i_108: ;FPMUL
	dec ebx
	jz rx_finish
	xor r9, 07327ba60h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_108
	call rx_read_l1
rx_body_108:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm5
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm9, xmm0
	mov eax, r9d
	xor eax, 0678b65beh
	and eax, 32767
	movhpd qword ptr [rsi + rax * 8], xmm9

rx_i_109: ;ROR_64
	dec ebx
	jz rx_finish
	xor r15, 0594e37deh
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_109
	call rx_read_l1
rx_body_109:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r10
	ror rax, cl
	mov rcx, rax
	mov eax, r11d
	xor eax, 094ab5a5ch
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_110: ;SHR_64
	dec ebx
	jz rx_finish
	xor r9, 04cdf5ebah
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_110
	call rx_read_l1
rx_body_110:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r9
	shr rax, cl
	mov rcx, rax
	mov eax, r14d
	xor eax, 0ec68532fh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_111: ;CALL
	dec ebx
	jz rx_finish
	xor r8, 02e16c97ch
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_111
	call rx_read_l1
rx_body_111:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r12d
	xor eax, 05d237d0bh
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx
	cmp r14d, 1562606859
	jl short rx_i_112
	call rx_i_212

rx_i_112: ;SUB_64
	dec ebx
	jz rx_finish
	xor r12, 0d42ddbd4h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_112
	call rx_read_l2
rx_body_112:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	sub rax, r13
	mov rcx, rax
	mov eax, r14d
	xor eax, 0c2d8d431h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_113: ;MUL_64
	dec ebx
	jz rx_finish
	xor r10, 07a4f8cbbh
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_113
	call rx_read_l1
rx_body_113:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r9
	mov r13, rax

rx_i_114: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r13, 06e83e2cdh
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_114
	call rx_read_l1
rx_body_114:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r15d
	imul rax, rcx
	mov r14, rax

rx_i_115: ;IDIV_64
	dec ebx
	jz rx_finish
	xor r14, 0336c980eh
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_115
	call rx_read_l2
rx_body_115:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov edx, r10d
	cmp edx, -1
	jne short safe_idiv_115
	mov rcx, rax
	rol rcx, 1
	dec rcx
	jz short result_idiv_115
safe_idiv_115:
	mov ecx, 1
	test edx, edx
	cmovne ecx, edx
	movsxd rcx, ecx
	cqo
	idiv rcx
result_idiv_115:
	mov r14, rax

rx_i_116: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r10, 0d122702eh
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_116
	call rx_read_l1
rx_body_116:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	mov rax, -1850776691
	imul rax, rcx
	mov rcx, rax
	mov eax, r8d
	xor eax, 091af638dh
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_117: ;IMULH_64
	dec ebx
	jz rx_finish
	xor r11, 015f2012bh
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_117
	call rx_read_l1
rx_body_117:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, -1205826972
	imul rcx
	mov rax, rdx
	mov rcx, rax
	mov eax, r15d
	xor eax, 0b8208a64h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_118: ;FPSUB
	dec ebx
	jz rx_finish
	xor r9, 037ddf43dh
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_118
	call rx_read_l2
rx_body_118:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm5
	movaps xmm6, xmm0

rx_i_119: ;FPSUB
	dec ebx
	jz rx_finish
	xor r9, 0bba475f3h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_119
	call rx_read_l1
rx_body_119:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm3
	movaps xmm5, xmm0

rx_i_120: ;FPADD
	dec ebx
	jz rx_finish
	xor r12, 0e5561e3eh
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_120
	call rx_read_l1
rx_body_120:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm4
	movaps xmm8, xmm0

rx_i_121: ;FPSUB
	dec ebx
	jz rx_finish
	xor r9, 03ab8f73h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_121
	call rx_read_l1
rx_body_121:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm5
	movaps xmm8, xmm0

rx_i_122: ;CALL
	dec ebx
	jz rx_finish
	xor r10, 04e0dbd40h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_122
	call rx_read_l1
rx_body_122:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r14d
	xor eax, 078f6ec29h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp r11d, 2029448233
	jno short rx_i_123
	call rx_i_192

rx_i_123: ;ADD_64
	dec ebx
	jz rx_finish
	xor r13, 073e9f58ah
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_123
	call rx_read_l1
rx_body_123:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, r15
	mov r13, rax

rx_i_124: ;JUMP
	dec ebx
	jz rx_finish
	xor r12, 0e3fa3670h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_124
	call rx_read_l2
rx_body_124:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r11d
	xor eax, 0667d921ch
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp r11d, 1719505436
	jns rx_i_237

rx_i_125: ;MUL_32
	dec ebx
	jz rx_finish
	xor r8, 0ebec27cdh
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_125
	call rx_read_l2
rx_body_125:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, eax
	mov eax, r14d
	imul rax, rcx
	mov r14, rax

rx_i_126: ;FPMUL
	dec ebx
	jz rx_finish
	xor r8, 01feb5264h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_126
	call rx_read_l1
rx_body_126:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm6
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm2, xmm0

rx_i_127: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r9, 0405f500fh
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_127
	call rx_read_l1
rx_body_127:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r10d
	imul rax, rcx
	mov r8, rax

rx_i_128: ;MUL_64
	dec ebx
	jz rx_finish
	xor r13, 0459f1154h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_128
	call rx_read_l2
rx_body_128:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r9
	mov r9, rax

rx_i_129: ;JUMP
	dec ebx
	jz rx_finish
	xor r9, 081918b4ch
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_129
	call rx_read_l1
rx_body_129:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov r9, rax
	cmp r13d, -590624856
	jge rx_i_154

rx_i_130: ;DIV_64
	dec ebx
	jz rx_finish
	xor r9, 077c3b332h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_130
	call rx_read_l1
rx_body_130:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, -281794782
	xor edx, edx
	div rcx
	mov rcx, rax
	mov eax, r11d
	xor eax, 0ef342722h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_131: ;RET
	dec ebx
	jz rx_finish
	xor r12, 05792310bh
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_131
	call rx_read_l1
rx_body_131:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r15d
	xor eax, 0dff06f75h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp rsp, rdi
	je short rx_i_132
	ret

rx_i_132: ;FPADD
	dec ebx
	jz rx_finish
	xor r10, 0ebc6e10h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_132
	call rx_read_l1
rx_body_132:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm6
	movaps xmm7, xmm0

rx_i_133: ;OR_64
	dec ebx
	jz rx_finish
	xor r14, 0822f8b60h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_133
	call rx_read_l1
rx_body_133:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	or rax, -1000526796
	mov rcx, rax
	mov eax, r15d
	xor eax, 0c45d2c34h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_134: ;ADD_64
	dec ebx
	jz rx_finish
	xor r10, 0d0f18593h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_134
	call rx_read_l1
rx_body_134:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, 1516102347
	mov r13, rax

rx_i_135: ;FPMUL
	dec ebx
	jz rx_finish
	xor r11, 088212ef9h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_135
	call rx_read_l1
rx_body_135:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm3
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm8, xmm0

rx_i_136: ;FPDIV
	dec ebx
	jz rx_finish
	xor r8, 01ae56e03h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_136
	call rx_read_l1
rx_body_136:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	divpd xmm0, xmm8
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm5, xmm0
	mov eax, r13d
	xor eax, 0efd7799dh
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm5

rx_i_137: ;SHR_64
	dec ebx
	jz rx_finish
	xor r11, 015a24231h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_137
	call rx_read_l1
rx_body_137:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r9
	shr rax, cl
	mov r11, rax

rx_i_138: ;RET
	dec ebx
	jz rx_finish
	xor r13, 02fd380c5h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_138
	call rx_read_l1
rx_body_138:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov r10, rax
	cmp rsp, rdi
	je short rx_i_139
	ret

rx_i_139: ;ADD_64
	dec ebx
	jz rx_finish
	xor r9, 093172470h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_139
	call rx_read_l1
rx_body_139:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, 515364082
	mov rcx, rax
	mov eax, r11d
	xor eax, 01eb7d4f2h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_140: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r14, 052543553h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_140
	call rx_read_l2
rx_body_140:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r11d
	imul rax, rcx
	mov r14, rax

rx_i_141: ;FPADD
	dec ebx
	jz rx_finish
	xor r8, 02f636da1h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_141
	call rx_read_l1
rx_body_141:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm2
	movaps xmm9, xmm0
	mov eax, r9d
	xor eax, 099ff9ffdh
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm9

rx_i_142: ;JUMP
	dec ebx
	jz rx_finish
	xor r11, 0b11a4f2ch
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_142
	call rx_read_l2
rx_body_142:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r10d
	xor eax, 0516a9452h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx
	cmp r12d, 1365939282
	js rx_i_257

rx_i_143: ;MUL_32
	dec ebx
	jz rx_finish
	xor r15, 037f4b5d0h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_143
	call rx_read_l2
rx_body_143:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, eax
	mov eax, r11d
	imul rax, rcx
	mov r9, rax

rx_i_144: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r10, 02e59e00ah
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_144
	call rx_read_l2
rx_body_144:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r11d
	imul rax, rcx
	mov r15, rax

rx_i_145: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r13, 08d5c798h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_145
	call rx_read_l1
rx_body_145:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r11d
	imul rax, rcx
	mov rcx, rax
	mov eax, r10d
	xor eax, 0dd491985h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_146: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r13, 02327e6e2h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_146
	call rx_read_l1
rx_body_146:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r12d
	imul rax, rcx
	mov r10, rax

rx_i_147: ;MUL_64
	dec ebx
	jz rx_finish
	xor r13, 03a7df043h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_147
	call rx_read_l1
rx_body_147:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, rax, 1784404616
	mov rcx, rax
	mov eax, r12d
	xor eax, 06a5bda88h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_148: ;ADD_32
	dec ebx
	jz rx_finish
	xor r10, 0783e5c4eh
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_148
	call rx_read_l1
rx_body_148:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add eax, r14d
	mov rcx, rax
	mov eax, r10d
	xor eax, 08c783d2ch
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_149: ;MUL_32
	dec ebx
	jz rx_finish
	xor r12, 0aa0f5b2fh
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_149
	call rx_read_l1
rx_body_149:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, eax
	mov eax, r14d
	imul rax, rcx
	mov rcx, rax
	mov eax, r8d
	xor eax, 09046b787h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_150: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r9, 01504ca7ah
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_150
	call rx_read_l1
rx_body_150:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r8d
	imul rax, rcx
	mov rcx, rax
	mov eax, r9d
	xor eax, 0c854a524h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_151: ;AND_64
	dec ebx
	jz rx_finish
	xor r9, 0ea72a7cfh
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_151
	call rx_read_l1
rx_body_151:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	and rax, r13
	mov rcx, rax
	mov eax, r11d
	xor eax, 087aed7f2h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_152: ;SAR_64
	dec ebx
	jz rx_finish
	xor r13, 0ad0e7a88h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_152
	call rx_read_l1
rx_body_152:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r10
	sar rax, cl
	mov r10, rax

rx_i_153: ;FPMUL
	dec ebx
	jz rx_finish
	xor r15, 0fd95ab87h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_153
	call rx_read_l1
rx_body_153:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm2
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm8, xmm0
	mov eax, r8d
	xor eax, 09111c981h
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm8

rx_i_154: ;MUL_32
	dec ebx
	jz rx_finish
	xor r10, 0256697b0h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_154
	call rx_read_l2
rx_body_154:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, eax
	mov eax, r13d
	imul rax, rcx
	mov r10, rax

rx_i_155: ;ROL_64
	dec ebx
	jz rx_finish
	xor r11, 0d23f3b78h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_155
	call rx_read_l1
rx_body_155:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r10
	rol rax, cl
	mov rcx, rax
	mov eax, r13d
	xor eax, 01c5d3ebeh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_156: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r10, 098917533h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_156
	call rx_read_l2
rx_body_156:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r15d
	imul rax, rcx
	mov r15, rax

rx_i_157: ;ADD_64
	dec ebx
	jz rx_finish
	xor r10, 0dfac3efch
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_157
	call rx_read_l1
rx_body_157:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, r12
	mov r14, rax

rx_i_158: ;ADD_64
	dec ebx
	jz rx_finish
	xor r15, 0a64de090h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_158
	call rx_read_l1
rx_body_158:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, 1233402159
	mov r10, rax

rx_i_159: ;CALL
	dec ebx
	jz rx_finish
	xor r13, 0952a3abbh
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_159
	call rx_read_l1
rx_body_159:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r13d
	xor eax, 0ff7d3697h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp r15d, -8571241
	ja short rx_i_160
	call rx_i_181

rx_i_160: ;ADD_32
	dec ebx
	jz rx_finish
	xor r14, 0b1685b90h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_160
	call rx_read_l1
rx_body_160:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add eax, 1518778665
	mov rcx, rax
	mov eax, r10d
	xor eax, 05a86b929h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_161: ;IDIV_64
	dec ebx
	jz rx_finish
	xor r15, 0ea992531h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_161
	call rx_read_l1
rx_body_161:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov edx, r14d
	cmp edx, -1
	jne short safe_idiv_161
	mov rcx, rax
	rol rcx, 1
	dec rcx
	jz short result_idiv_161
safe_idiv_161:
	mov ecx, 1
	test edx, edx
	cmovne ecx, edx
	movsxd rcx, ecx
	cqo
	idiv rcx
result_idiv_161:
	mov r8, rax

rx_i_162: ;SHL_64
	dec ebx
	jz rx_finish
	xor r9, 01fd57a4ah
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_162
	call rx_read_l1
rx_body_162:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r10
	shl rax, cl
	mov r13, rax

rx_i_163: ;ADD_32
	dec ebx
	jz rx_finish
	xor r12, 0e3486c0ah
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_163
	call rx_read_l2
rx_body_163:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	add eax, -2101130488
	mov rcx, rax
	mov eax, r14d
	xor eax, 082c34b08h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_164: ;MULH_64
	dec ebx
	jz rx_finish
	xor r12, 01f0c2737h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_164
	call rx_read_l1
rx_body_164:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r9
	mul rcx
	mov rax, rdx
	mov rcx, rax
	mov eax, r13d
	xor eax, 09aa6da19h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_165: ;RET
	dec ebx
	jz rx_finish
	xor r12, 0debb493eh
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_165
	call rx_read_l1
rx_body_165:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r9d
	xor eax, 06450685ch
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx
	cmp rsp, rdi
	je short rx_i_166
	ret

rx_i_166: ;SHR_64
	dec ebx
	jz rx_finish
	xor r9, 0fe684081h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_166
	call rx_read_l2
rx_body_166:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r8
	shr rax, cl
	mov rcx, rax
	mov eax, r13d
	xor eax, 0bb67f8abh
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_167: ;FPMUL
	dec ebx
	jz rx_finish
	xor r11, 0d10371ch
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_167
	call rx_read_l1
rx_body_167:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm4
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm2, xmm0
	mov eax, r10d
	xor eax, 02a58510fh
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm2

rx_i_168: ;FPDIV
	dec ebx
	jz rx_finish
	xor r12, 071b15effh
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_168
	call rx_read_l1
rx_body_168:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	divpd xmm0, xmm5
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm7, xmm0

rx_i_169: ;CALL
	dec ebx
	jz rx_finish
	xor r11, 072790347h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_169
	call rx_read_l1
rx_body_169:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r14d
	xor eax, 0b353bf8dh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp r10d, -1286357107
	ja short rx_i_170
	call rx_i_197

rx_i_170: ;FPSQRT
	dec ebx
	jz rx_finish
	xor r8, 04ae8a020h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_170
	call rx_read_l2
rx_body_170:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	andps xmm0, xmm10
	sqrtpd xmm6, xmm0
	mov eax, r14d
	xor eax, 0a4256a99h
	and eax, 32767
	movlpd qword ptr [rsi + rax * 8], xmm6

rx_i_171: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r15, 09901e05bh
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_171
	call rx_read_l1
rx_body_171:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r12d
	imul rax, rcx
	mov r12, rax

rx_i_172: ;SUB_64
	dec ebx
	jz rx_finish
	xor r13, 050e8c510h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_172
	call rx_read_l1
rx_body_172:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	sub rax, r11
	mov r12, rax

rx_i_173: ;MUL_64
	dec ebx
	jz rx_finish
	xor r14, 05422cf8fh
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_173
	call rx_read_l1
rx_body_173:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r12
	mov rcx, rax
	mov eax, r12d
	xor eax, 0ad60ae9ch
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_174: ;FPDIV
	dec ebx
	jz rx_finish
	xor r12, 0a025c3dbh
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_174
	call rx_read_l1
rx_body_174:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	divpd xmm0, xmm9
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm6, xmm0
	mov eax, r14d
	xor eax, 02be6989fh
	and eax, 32767
	movlpd qword ptr [rsi + rax * 8], xmm6

rx_i_175: ;XOR_32
	dec ebx
	jz rx_finish
	xor r13, 08f74c11h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_175
	call rx_read_l1
rx_body_175:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	xor eax, r8d
	mov r8, rax

rx_i_176: ;SUB_64
	dec ebx
	jz rx_finish
	xor r9, 01f2ed5f1h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_176
	call rx_read_l2
rx_body_176:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	sub rax, r14
	mov r10, rax

rx_i_177: ;ADD_64
	dec ebx
	jz rx_finish
	xor r10, 0d2072c79h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_177
	call rx_read_l2
rx_body_177:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	add rax, r10
	mov rcx, rax
	mov eax, r13d
	xor eax, 02f5713b7h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_178: ;RET
	dec ebx
	jz rx_finish
	xor r15, 0a8e51933h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_178
	call rx_read_l1
rx_body_178:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r12d
	xor eax, 0c366b275h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp rsp, rdi
	je short rx_i_179
	ret

rx_i_179: ;FPADD
	dec ebx
	jz rx_finish
	xor r12, 0934ad492h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_179
	call rx_read_l1
rx_body_179:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm2
	movaps xmm8, xmm0

rx_i_180: ;AND_32
	dec ebx
	jz rx_finish
	xor r15, 01cb3ce1fh
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_180
	call rx_read_l2
rx_body_180:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	and eax, 1995308563
	mov rcx, rax
	mov eax, r9d
	xor eax, 076edfe13h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_181: ;CALL
	dec ebx
	jz rx_finish
	xor r10, 023c7845fh
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_181
	call rx_read_l2
rx_body_181:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov r10, rax
	cmp r12d, -1612576918
	jbe short rx_i_182
	call rx_i_211

rx_i_182: ;FPSUB
	dec ebx
	jz rx_finish
	xor r8, 0f8884327h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_182
	call rx_read_l1
rx_body_182:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm7
	movaps xmm6, xmm0

rx_i_183: ;ADD_64
	dec ebx
	jz rx_finish
	xor r13, 013070461h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_183
	call rx_read_l1
rx_body_183:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, 137260710
	mov r10, rax

rx_i_184: ;XOR_32
	dec ebx
	jz rx_finish
	xor r12, 04764cdf7h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_184
	call rx_read_l2
rx_body_184:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	xor eax, 790123591
	mov r12, rax

rx_i_185: ;JUMP
	dec ebx
	jz rx_finish
	xor r10, 03c41026fh
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_185
	call rx_read_l1
rx_body_185:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r9d
	xor eax, 0a5fae4a3h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp r15d, -1510284125
	jbe rx_i_246

rx_i_186: ;OR_64
	dec ebx
	jz rx_finish
	xor r9, 0cded414bh
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_186
	call rx_read_l1
rx_body_186:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	or rax, r15
	mov rcx, rax
	mov eax, r10d
	xor eax, 0b55bfba0h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_187: ;FPMUL
	dec ebx
	jz rx_finish
	xor r13, 05c6d64a8h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_187
	call rx_read_l2
rx_body_187:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm6
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm5, xmm0

rx_i_188: ;FPSUB
	dec ebx
	jz rx_finish
	xor r9, 04659becbh
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_188
	call rx_read_l1
rx_body_188:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm3
	movaps xmm4, xmm0

rx_i_189: ;FPDIV
	dec ebx
	jz rx_finish
	xor r11, 0c52741d5h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_189
	call rx_read_l1
rx_body_189:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	divpd xmm0, xmm7
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm5, xmm0

rx_i_190: ;RET
	dec ebx
	jz rx_finish
	xor r12, 0217bf5f3h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_190
	call rx_read_l2
rx_body_190:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov r13, rax
	cmp rsp, rdi
	je short rx_i_191
	ret

rx_i_191: ;FPSQRT
	dec ebx
	jz rx_finish
	xor r15, 0884f3526h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_191
	call rx_read_l1
rx_body_191:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	andps xmm0, xmm10
	sqrtpd xmm6, xmm0

rx_i_192: ;FPSQRT
	dec ebx
	jz rx_finish
	xor r8, 0d76edad3h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_192
	call rx_read_l1
rx_body_192:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	andps xmm0, xmm10
	sqrtpd xmm8, xmm0
	mov eax, r8d
	xor eax, 0f8fd2040h
	and eax, 32767
	movlpd qword ptr [rsi + rax * 8], xmm8

rx_i_193: ;MULH_64
	dec ebx
	jz rx_finish
	xor r12, 0e9939ach
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_193
	call rx_read_l1
rx_body_193:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r12
	mul rcx
	mov rax, rdx
	mov rcx, rax
	mov eax, r15d
	xor eax, 074e097dch
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_194: ;FPMUL
	dec ebx
	jz rx_finish
	xor r12, 0f21ca520h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_194
	call rx_read_l2
rx_body_194:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm2
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm5, xmm0
	mov eax, r13d
	xor eax, 040eb9f47h
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm5

rx_i_195: ;SHL_64
	dec ebx
	jz rx_finish
	xor r10, 09405152ch
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_195
	call rx_read_l1
rx_body_195:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r8
	shl rax, cl
	mov r9, rax

rx_i_196: ;ADD_32
	dec ebx
	jz rx_finish
	xor r8, 0c2a9f41bh
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_196
	call rx_read_l2
rx_body_196:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	add eax, -1907903895
	mov rcx, rax
	mov eax, r13d
	xor eax, 08e47b269h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_197: ;MUL_64
	dec ebx
	jz rx_finish
	xor r12, 0229208efh
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_197
	call rx_read_l1
rx_body_197:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r15
	mov r11, rax

rx_i_198: ;MUL_64
	dec ebx
	jz rx_finish
	xor r14, 0c8d95bbbh
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_198
	call rx_read_l1
rx_body_198:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r14
	mov rcx, rax
	mov eax, r8d
	xor eax, 01149cba0h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_199: ;MUL_64
	dec ebx
	jz rx_finish
	xor r13, 050049e2eh
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_199
	call rx_read_l2
rx_body_199:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r10
	mov rcx, rax
	mov eax, r10d
	xor eax, 0d0e71e9ah
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_200: ;FPSUB
	dec ebx
	jz rx_finish
	xor r10, 0c63b99e8h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_200
	call rx_read_l1
rx_body_200:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm2
	movaps xmm4, xmm0
	mov eax, r12d
	xor eax, 0b05ce8abh
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm4

rx_i_201: ;FPADD
	dec ebx
	jz rx_finish
	xor r8, 0cdda801dh
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_201
	call rx_read_l1
rx_body_201:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm9
	movaps xmm4, xmm0
	mov eax, r12d
	xor eax, 040cfe68eh
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm4

rx_i_202: ;FPADD
	dec ebx
	jz rx_finish
	xor r13, 0fa44b04ah
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_202
	call rx_read_l2
rx_body_202:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm9
	movaps xmm5, xmm0

rx_i_203: ;FPSUB
	dec ebx
	jz rx_finish
	xor r10, 0d73e472ch
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_203
	call rx_read_l1
rx_body_203:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm2
	movaps xmm7, xmm0
	mov eax, r15d
	xor eax, 09bdff355h
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm7

rx_i_204: ;MUL_64
	dec ebx
	jz rx_finish
	xor r9, 01af8ab1dh
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_204
	call rx_read_l1
rx_body_204:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r15
	mov rcx, rax
	mov eax, r8d
	xor eax, 0eb8fc30fh
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_205: ;FPMUL
	dec ebx
	jz rx_finish
	xor r14, 094e997c5h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_205
	call rx_read_l1
rx_body_205:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm8
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm5, xmm0

rx_i_206: ;FPSUB
	dec ebx
	jz rx_finish
	xor r11, 0e836a177h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_206
	call rx_read_l1
rx_body_206:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm7
	movaps xmm4, xmm0

rx_i_207: ;IMULH_64
	dec ebx
	jz rx_finish
	xor r9, 039ccdd30h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_207
	call rx_read_l1
rx_body_207:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r12
	imul rcx
	mov rax, rdx
	mov rcx, rax
	mov eax, r9d
	xor eax, 012bbcc84h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_208: ;MUL_64
	dec ebx
	jz rx_finish
	xor r9, 0f4f126c5h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_208
	call rx_read_l1
rx_body_208:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r12
	mov r10, rax

rx_i_209: ;XOR_64
	dec ebx
	jz rx_finish
	xor r8, 0b84811f1h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_209
	call rx_read_l1
rx_body_209:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	xor rax, -1016364182
	mov rcx, rax
	mov eax, r12d
	xor eax, 0c36b836ah
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_210: ;MUL_32
	dec ebx
	jz rx_finish
	xor r12, 0c5efc90ah
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_210
	call rx_read_l2
rx_body_210:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, eax
	mov eax, -1027162400
	imul rax, rcx
	mov rcx, rax
	mov eax, r15d
	xor eax, 0c2c6bee0h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_211: ;ROR_64
	dec ebx
	jz rx_finish
	xor r12, 0ce533072h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_211
	call rx_read_l2
rx_body_211:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	ror rax, 27
	mov rcx, rax
	mov eax, r11d
	xor eax, 0212e615h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_212: ;MUL_64
	dec ebx
	jz rx_finish
	xor r13, 06b465fdbh
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_212
	call rx_read_l1
rx_body_212:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r13
	mov rcx, rax
	mov eax, r15d
	xor eax, 067d81043h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_213: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r13, 02dd1d503h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_213
	call rx_read_l1
rx_body_213:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	mov rax, 129993589
	imul rax, rcx
	mov r14, rax

rx_i_214: ;SHL_64
	dec ebx
	jz rx_finish
	xor r9, 0a159f313h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_214
	call rx_read_l1
rx_body_214:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r14
	shl rax, cl
	mov r14, rax

rx_i_215: ;ADD_64
	dec ebx
	jz rx_finish
	xor r15, 08359265eh
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_215
	call rx_read_l1
rx_body_215:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, r12
	mov r10, rax

rx_i_216: ;MUL_64
	dec ebx
	jz rx_finish
	xor r12, 080696de3h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_216
	call rx_read_l1
rx_body_216:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r13
	mov rcx, rax
	mov eax, r15d
	xor eax, 03b609d2bh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_217: ;MUL_32
	dec ebx
	jz rx_finish
	xor r8, 040d5b526h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_217
	call rx_read_l1
rx_body_217:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, eax
	mov eax, r9d
	imul rax, rcx
	mov rcx, rax
	mov eax, r10d
	xor eax, 017e667h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_218: ;FPSQRT
	dec ebx
	jz rx_finish
	xor r11, 083c0bd93h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_218
	call rx_read_l2
rx_body_218:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	andps xmm0, xmm10
	sqrtpd xmm3, xmm0
	mov eax, r11d
	xor eax, 0dd192e86h
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm3

rx_i_219: ;OR_64
	dec ebx
	jz rx_finish
	xor r8, 0ca37f668h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_219
	call rx_read_l1
rx_body_219:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	or rax, -740915304
	mov rcx, rax
	mov eax, r15d
	xor eax, 0d3d68798h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_220: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r9, 0bb44c384h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_220
	call rx_read_l1
rx_body_220:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r11d
	imul rax, rcx
	mov rcx, rax
	mov eax, r11d
	xor eax, 0903fd173h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_221: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r9, 0a3deb512h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_221
	call rx_read_l1
rx_body_221:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r15d
	imul rax, rcx
	mov rcx, rax
	mov eax, r11d
	xor eax, 07feab351h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_222: ;FPMUL
	dec ebx
	jz rx_finish
	xor r9, 084a02d64h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_222
	call rx_read_l2
rx_body_222:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm5
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm7, xmm0
	mov eax, r15d
	xor eax, 0d7601963h
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm7

rx_i_223: ;FPSUB
	dec ebx
	jz rx_finish
	xor r8, 01e5cc085h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_223
	call rx_read_l1
rx_body_223:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm3
	movaps xmm2, xmm0
	mov eax, r10d
	xor eax, 07fca59eeh
	and eax, 32767
	movhpd qword ptr [rsi + rax * 8], xmm2

rx_i_224: ;XOR_32
	dec ebx
	jz rx_finish
	xor r12, 053982440h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_224
	call rx_read_l1
rx_body_224:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	xor eax, r14d
	mov rcx, rax
	mov eax, r11d
	xor eax, 0e500c69dh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_225: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r13, 0c558367eh
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_225
	call rx_read_l2
rx_body_225:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r10d
	imul rax, rcx
	mov rcx, rax
	mov eax, r12d
	xor eax, 0fe304a4ah
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_226: ;JUMP
	dec ebx
	jz rx_finish
	xor r10, 040139b65h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_226
	call rx_read_l1
rx_body_226:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r8d
	xor eax, 0978b2498h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp r8d, -1752488808
	jno rx_i_328

rx_i_227: ;FPMUL
	dec ebx
	jz rx_finish
	xor r11, 0fa312dbdh
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_227
	call rx_read_l1
rx_body_227:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm7
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm3, xmm0
	mov eax, r11d
	xor eax, 0aabe2a0ah
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm3

rx_i_228: ;FPSQRT
	dec ebx
	jz rx_finish
	xor r11, 0b64246c0h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_228
	call rx_read_l1
rx_body_228:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	andps xmm0, xmm10
	sqrtpd xmm7, xmm0

rx_i_229: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r11, 05c535836h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_229
	call rx_read_l2
rx_body_229:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r12d
	imul rax, rcx
	mov rcx, rax
	mov eax, r13d
	xor eax, 013e8b2e0h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_230: ;FPMUL
	dec ebx
	jz rx_finish
	xor r15, 0f394972eh
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_230
	call rx_read_l1
rx_body_230:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm6
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm5, xmm0
	mov eax, r13d
	xor eax, 01dc2b4f6h
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm5

rx_i_231: ;RET
	dec ebx
	jz rx_finish
	xor r9, 0bb56428dh
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_231
	call rx_read_l1
rx_body_231:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r9d
	xor eax, 0e6c9edaah
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp rsp, rdi
	je short rx_i_232
	ret

rx_i_232: ;FPMUL
	dec ebx
	jz rx_finish
	xor r15, 09ab46ab3h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_232
	call rx_read_l1
rx_body_232:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm3
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm7, xmm0

rx_i_233: ;JUMP
	dec ebx
	jz rx_finish
	xor r13, 08eb2cd76h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_233
	call rx_read_l1
rx_body_233:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov r14, rax
	cmp r12d, 392389867
	jo rx_i_268

rx_i_234: ;FPDIV
	dec ebx
	jz rx_finish
	xor r15, 0ba687578h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_234
	call rx_read_l1
rx_body_234:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	divpd xmm0, xmm4
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm4, xmm0

rx_i_235: ;MUL_32
	dec ebx
	jz rx_finish
	xor r13, 0b6cb9ff2h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_235
	call rx_read_l2
rx_body_235:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, eax
	mov eax, r12d
	imul rax, rcx
	mov rcx, rax
	mov eax, r15d
	xor eax, 0ca73a89h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_236: ;FPADD
	dec ebx
	jz rx_finish
	xor r15, 03ad196ach
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_236
	call rx_read_l2
rx_body_236:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm4
	movaps xmm3, xmm0

rx_i_237: ;JUMP
	dec ebx
	jz rx_finish
	xor r15, 0fab4600h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_237
	call rx_read_l2
rx_body_237:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov r11, rax
	cmp r12d, -121899164
	jge rx_i_295

rx_i_238: ;FPADD
	dec ebx
	jz rx_finish
	xor r8, 0158f119fh
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_238
	call rx_read_l2
rx_body_238:
	xor rbp, rcx
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm6
	movaps xmm7, xmm0
	mov eax, r15d
	xor eax, 0331bbf8h
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm7

rx_i_239: ;ADD_64
	dec ebx
	jz rx_finish
	xor r13, 044f30b3fh
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_239
	call rx_read_l1
rx_body_239:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, r10
	mov r10, rax

rx_i_240: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r9, 0d65d29f9h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_240
	call rx_read_l1
rx_body_240:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	mov rax, -423830277
	imul rax, rcx
	mov r8, rax

rx_i_241: ;FPADD
	dec ebx
	jz rx_finish
	xor r11, 0ce5260adh
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_241
	call rx_read_l1
rx_body_241:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm3
	movaps xmm7, xmm0
	mov eax, r15d
	xor eax, 0bc2423ebh
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm7

rx_i_242: ;MULH_64
	dec ebx
	jz rx_finish
	xor r12, 01119b0f9h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_242
	call rx_read_l2
rx_body_242:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, 319324914
	mul rcx
	mov rax, rdx
	mov rcx, rax
	mov eax, r10d
	xor eax, 0130882f2h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_243: ;OR_64
	dec ebx
	jz rx_finish
	xor r12, 0d6c2ce3dh
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_243
	call rx_read_l1
rx_body_243:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	or rax, 1198180774
	mov r14, rax

rx_i_244: ;ROR_64
	dec ebx
	jz rx_finish
	xor r11, 0c6a6248h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_244
	call rx_read_l2
rx_body_244:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r14
	ror rax, cl
	mov rcx, rax
	mov eax, r9d
	xor eax, 0b4a1fad6h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_245: ;AND_32
	dec ebx
	jz rx_finish
	xor r13, 084505739h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_245
	call rx_read_l1
rx_body_245:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	and eax, -1546539637
	mov rcx, rax
	mov eax, r12d
	xor eax, 0a3d1ad8bh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_246: ;IMULH_64
	dec ebx
	jz rx_finish
	xor r15, 027eeaa2eh
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_246
	call rx_read_l2
rx_body_246:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r9
	imul rcx
	mov rax, rdx
	mov r12, rax

rx_i_247: ;MUL_32
	dec ebx
	jz rx_finish
	xor r10, 0c4de0296h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_247
	call rx_read_l1
rx_body_247:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, eax
	mov eax, r14d
	imul rax, rcx
	mov rcx, rax
	mov eax, r9d
	xor eax, 03814cf80h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_248: ;MULH_64
	dec ebx
	jz rx_finish
	xor r8, 0649df46fh
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_248
	call rx_read_l2
rx_body_248:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r15
	mul rcx
	mov rax, rdx
	mov rcx, rax
	mov eax, r9d
	xor eax, 07b10fc32h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_249: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r15, 0499552cch
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_249
	call rx_read_l2
rx_body_249:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r11d
	imul rax, rcx
	mov rcx, rax
	mov eax, r13d
	xor eax, 0e1afcff9h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_250: ;SUB_32
	dec ebx
	jz rx_finish
	xor r13, 083eafe6fh
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_250
	call rx_read_l2
rx_body_250:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	sub eax, r8d
	mov rcx, rax
	mov eax, r14d
	xor eax, 031115b87h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_251: ;FPMUL
	dec ebx
	jz rx_finish
	xor r13, 0a25a4d8ah
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_251
	call rx_read_l2
rx_body_251:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm2
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm4, xmm0
	mov eax, r12d
	xor eax, 05ed767a3h
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm4

rx_i_252: ;SHL_64
	dec ebx
	jz rx_finish
	xor r14, 08a75ad41h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_252
	call rx_read_l2
rx_body_252:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r8
	shl rax, cl
	mov r14, rax

rx_i_253: ;CALL
	dec ebx
	jz rx_finish
	xor r14, 057f3f596h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_253
	call rx_read_l1
rx_body_253:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r13d
	xor eax, 0654b460bh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp r15d, 1699431947
	js short rx_i_254
	call rx_i_367

rx_i_254: ;FPADD
	dec ebx
	jz rx_finish
	xor r14, 04cfb709eh
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_254
	call rx_read_l1
rx_body_254:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm4
	movaps xmm8, xmm0
	mov eax, r8d
	xor eax, 0c251872eh
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm8

rx_i_255: ;FPADD
	dec ebx
	jz rx_finish
	xor r9, 0b96ec9ech
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_255
	call rx_read_l1
rx_body_255:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm5
	movaps xmm6, xmm0
	mov eax, r14d
	xor eax, 0ae781d10h
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm6

rx_i_256: ;MUL_64
	dec ebx
	jz rx_finish
	xor r8, 08375472ch
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_256
	call rx_read_l1
rx_body_256:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r15
	mov rcx, rax
	mov eax, r9d
	xor eax, 0f8942c0h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_257: ;FPADD
	dec ebx
	jz rx_finish
	xor r12, 0d75a8c3fh
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_257
	call rx_read_l2
rx_body_257:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm5
	movaps xmm3, xmm0
	mov eax, r11d
	xor eax, 0373b1b6fh
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm3

rx_i_258: ;MUL_32
	dec ebx
	jz rx_finish
	xor r11, 064fdbda0h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_258
	call rx_read_l1
rx_body_258:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, eax
	mov eax, r14d
	imul rax, rcx
	mov rcx, rax
	mov eax, r9d
	xor eax, 01c58ef2dh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_259: ;FPADD
	dec ebx
	jz rx_finish
	xor r11, 02e36a073h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_259
	call rx_read_l1
rx_body_259:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm9
	movaps xmm3, xmm0

rx_i_260: ;FPSUB
	dec ebx
	jz rx_finish
	xor r13, 0f94e9fa9h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_260
	call rx_read_l2
rx_body_260:
	xor rbp, rcx
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm5
	movaps xmm9, xmm0

rx_i_261: ;FPDIV
	dec ebx
	jz rx_finish
	xor r14, 02346171ch
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_261
	call rx_read_l2
rx_body_261:
	xor rbp, rcx
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	divpd xmm0, xmm3
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm3, xmm0
	mov eax, r11d
	xor eax, 0745a48e9h
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm3

rx_i_262: ;AND_64
	dec ebx
	jz rx_finish
	xor r10, 01c42baa6h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_262
	call rx_read_l1
rx_body_262:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	and rax, r13
	mov rcx, rax
	mov eax, r11d
	xor eax, 0a271ff06h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_263: ;FPMUL
	dec ebx
	jz rx_finish
	xor r11, 0b39b140h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_263
	call rx_read_l1
rx_body_263:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm8
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm6, xmm0

rx_i_264: ;FPMUL
	dec ebx
	jz rx_finish
	xor r11, 01a07d201h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_264
	call rx_read_l2
rx_body_264:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm3
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm7, xmm0

rx_i_265: ;FPADD
	dec ebx
	jz rx_finish
	xor r13, 07a3eb340h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_265
	call rx_read_l2
rx_body_265:
	xor rbp, rcx
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm8
	movaps xmm2, xmm0
	mov eax, r10d
	xor eax, 04c559414h
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm2

rx_i_266: ;CALL
	dec ebx
	jz rx_finish
	xor r13, 03d0a3a89h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_266
	call rx_read_l1
rx_body_266:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov r10, rax
	cmp r12d, 136160027
	jbe short rx_i_267
	call rx_i_295

rx_i_267: ;ROL_64
	dec ebx
	jz rx_finish
	xor r8, 0c6c7b37h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_267
	call rx_read_l1
rx_body_267:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	rol rax, 56
	mov r11, rax

rx_i_268: ;JUMP
	dec ebx
	jz rx_finish
	xor r12, 0c2510cebh
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_268
	call rx_read_l2
rx_body_268:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov r13, rax
	cmp r15d, -2062812966
	jl rx_i_381

rx_i_269: ;ROL_64
	dec ebx
	jz rx_finish
	xor r11, 0c80cc899h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_269
	call rx_read_l1
rx_body_269:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r8
	rol rax, cl
	mov rcx, rax
	mov eax, r10d
	xor eax, 01ba81447h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_270: ;FPMUL
	dec ebx
	jz rx_finish
	xor r11, 0eb355caah
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_270
	call rx_read_l1
rx_body_270:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm9
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm7, xmm0

rx_i_271: ;MUL_32
	dec ebx
	jz rx_finish
	xor r13, 0c6f12299h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_271
	call rx_read_l1
rx_body_271:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, eax
	mov eax, -2032281772
	imul rax, rcx
	mov rcx, rax
	mov eax, r9d
	xor eax, 086ddd754h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_272: ;AND_64
	dec ebx
	jz rx_finish
	xor r12, 0695a5dd2h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_272
	call rx_read_l2
rx_body_272:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	and rax, r12
	mov r13, rax

rx_i_273: ;JUMP
	dec ebx
	jz rx_finish
	xor r9, 0d315e4dch
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_273
	call rx_read_l1
rx_body_273:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r13d
	xor eax, 063972038h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx
	cmp r12d, 1670848568
	jl rx_i_372

rx_i_274: ;FPADD
	dec ebx
	jz rx_finish
	xor r15, 0b66ca7e0h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_274
	call rx_read_l2
rx_body_274:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm4
	movaps xmm6, xmm0
	mov eax, r14d
	xor eax, 06a2b2b5bh
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm6

rx_i_275: ;DIV_64
	dec ebx
	jz rx_finish
	xor r10, 0788eceb7h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_275
	call rx_read_l2
rx_body_275:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, 1
	mov edx, r11d
	test edx, edx
	cmovne ecx, edx
	xor edx, edx
	div rcx
	mov r13, rax

rx_i_276: ;JUMP
	dec ebx
	jz rx_finish
	xor r9, 0c6ac5edah
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_276
	call rx_read_l1
rx_body_276:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r12d
	xor eax, 0b65161a6h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp r11d, -1236180570
	jns rx_i_404

rx_i_277: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r11, 0c9549789h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_277
	call rx_read_l2
rx_body_277:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r10d
	imul rax, rcx
	mov rcx, rax
	mov eax, r9d
	xor eax, 01aca20a3h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_278: ;FPSUB
	dec ebx
	jz rx_finish
	xor r9, 0a2bc66c9h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_278
	call rx_read_l1
rx_body_278:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm7
	movaps xmm4, xmm0
	mov eax, r12d
	xor eax, 02d00ad10h
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm4

rx_i_279: ;FPADD
	dec ebx
	jz rx_finish
	xor r15, 0f1a91458h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_279
	call rx_read_l1
rx_body_279:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm5
	movaps xmm9, xmm0
	mov eax, r9d
	xor eax, 0475ade01h
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm9

rx_i_280: ;IMULH_64
	dec ebx
	jz rx_finish
	xor r12, 066246b43h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_280
	call rx_read_l2
rx_body_280:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r11
	imul rcx
	mov rax, rdx
	mov rcx, rax
	mov eax, r13d
	xor eax, 0211aeb00h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_281: ;SUB_64
	dec ebx
	jz rx_finish
	xor r10, 05a762727h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_281
	call rx_read_l1
rx_body_281:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	sub rax, r10
	mov rcx, rax
	mov eax, r11d
	xor eax, 0f3e6c946h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_282: ;SUB_64
	dec ebx
	jz rx_finish
	xor r15, 0de1ab603h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_282
	call rx_read_l1
rx_body_282:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	sub rax, 1367326224
	mov r11, rax

rx_i_283: ;ADD_64
	dec ebx
	jz rx_finish
	xor r9, 0df4d084fh
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_283
	call rx_read_l1
rx_body_283:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, -1156732976
	mov rcx, rax
	mov eax, r12d
	xor eax, 0bb0da7d0h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_284: ;FPADD
	dec ebx
	jz rx_finish
	xor r15, 0e68f36ach
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_284
	call rx_read_l1
rx_body_284:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm6
	movaps xmm9, xmm0
	mov eax, r9d
	xor eax, 0936f2960h
	and eax, 32767
	movlpd qword ptr [rsi + rax * 8], xmm9

rx_i_285: ;MUL_32
	dec ebx
	jz rx_finish
	xor r8, 09adb333bh
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_285
	call rx_read_l1
rx_body_285:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, eax
	mov eax, r8d
	imul rax, rcx
	mov r14, rax

rx_i_286: ;ROL_64
	dec ebx
	jz rx_finish
	xor r14, 082f5e36ch
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_286
	call rx_read_l1
rx_body_286:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r9
	rol rax, cl
	mov rcx, rax
	mov eax, r15d
	xor eax, 0546e75d1h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_287: ;IDIV_64
	dec ebx
	jz rx_finish
	xor r11, 049547c9ch
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_287
	call rx_read_l1
rx_body_287:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov edx, r15d
	cmp edx, -1
	jne short safe_idiv_287
	mov rcx, rax
	rol rcx, 1
	dec rcx
	jz short result_idiv_287
safe_idiv_287:
	mov ecx, 1
	test edx, edx
	cmovne ecx, edx
	movsxd rcx, ecx
	cqo
	idiv rcx
result_idiv_287:
	mov rcx, rax
	mov eax, r8d
	xor eax, 04926c7fah
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_288: ;MUL_64
	dec ebx
	jz rx_finish
	xor r10, 08716ac8bh
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_288
	call rx_read_l1
rx_body_288:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r8
	mov rcx, rax
	mov eax, r9d
	xor eax, 062eafa1bh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_289: ;FPMUL
	dec ebx
	jz rx_finish
	xor r14, 0efef52b5h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_289
	call rx_read_l2
rx_body_289:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm9
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm8, xmm0

rx_i_290: ;FPSUB
	dec ebx
	jz rx_finish
	xor r15, 060665748h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_290
	call rx_read_l2
rx_body_290:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm8
	movaps xmm9, xmm0

rx_i_291: ;RET
	dec ebx
	jz rx_finish
	xor r13, 0ddf4bd1ah
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_291
	call rx_read_l2
rx_body_291:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r14d
	xor eax, 0768a9d75h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx
	cmp rsp, rdi
	je short rx_i_292
	ret

rx_i_292: ;ROL_64
	dec ebx
	jz rx_finish
	xor r13, 05a87cc3dh
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_292
	call rx_read_l1
rx_body_292:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	rol rax, 23
	mov r10, rax

rx_i_293: ;FPADD
	dec ebx
	jz rx_finish
	xor r9, 0c61f4279h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_293
	call rx_read_l2
rx_body_293:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm5
	movaps xmm8, xmm0

rx_i_294: ;RET
	dec ebx
	jz rx_finish
	xor r14, 0f3b9d85h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_294
	call rx_read_l2
rx_body_294:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r8d
	xor eax, 0ef8571b7h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp rsp, rdi
	je short rx_i_295
	ret

rx_i_295: ;FPSUB
	dec ebx
	jz rx_finish
	xor r9, 0f42798fdh
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_295
	call rx_read_l1
rx_body_295:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm8
	movaps xmm7, xmm0

rx_i_296: ;FPSQRT
	dec ebx
	jz rx_finish
	xor r14, 018738758h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_296
	call rx_read_l1
rx_body_296:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	andps xmm0, xmm10
	sqrtpd xmm8, xmm0

rx_i_297: ;ADD_64
	dec ebx
	jz rx_finish
	xor r15, 0de3b9d9bh
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_297
	call rx_read_l1
rx_body_297:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, r10
	mov r14, rax

rx_i_298: ;FPSUB
	dec ebx
	jz rx_finish
	xor r14, 084f53637h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_298
	call rx_read_l1
rx_body_298:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm7
	movaps xmm6, xmm0

rx_i_299: ;ADD_64
	dec ebx
	jz rx_finish
	xor r12, 042f4897h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_299
	call rx_read_l1
rx_body_299:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, 21400308
	mov rcx, rax
	mov eax, r12d
	xor eax, 01468af4h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_300: ;FPSUB
	dec ebx
	jz rx_finish
	xor r12, 095765693h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_300
	call rx_read_l2
rx_body_300:
	xor rbp, rcx
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm8
	movaps xmm2, xmm0

rx_i_301: ;FPMUL
	dec ebx
	jz rx_finish
	xor r8, 0a0ec5eech
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_301
	call rx_read_l1
rx_body_301:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm5
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm7, xmm0
	mov eax, r15d
	xor eax, 0433cf2d6h
	and eax, 32767
	movhpd qword ptr [rsi + rax * 8], xmm7

rx_i_302: ;ADD_64
	dec ebx
	jz rx_finish
	xor r15, 0f6f8c345h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_302
	call rx_read_l1
rx_body_302:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, r10
	mov r11, rax

rx_i_303: ;FPADD
	dec ebx
	jz rx_finish
	xor r14, 082a3e965h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_303
	call rx_read_l1
rx_body_303:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm3
	movaps xmm9, xmm0
	mov eax, r9d
	xor eax, 0bb9ee490h
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm9

rx_i_304: ;MUL_64
	dec ebx
	jz rx_finish
	xor r12, 04940c652h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_304
	call rx_read_l1
rx_body_304:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r15
	mov r13, rax

rx_i_305: ;SUB_32
	dec ebx
	jz rx_finish
	xor r11, 03c6c62b8h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_305
	call rx_read_l2
rx_body_305:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	sub eax, -65873120
	mov r10, rax

rx_i_306: ;ADD_64
	dec ebx
	jz rx_finish
	xor r15, 08b34cdfch
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_306
	call rx_read_l2
rx_body_306:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	add rax, r15
	mov r13, rax

rx_i_307: ;SHL_64
	dec ebx
	jz rx_finish
	xor r15, 04c36adb1h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_307
	call rx_read_l1
rx_body_307:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r8
	shl rax, cl
	mov r10, rax

rx_i_308: ;MUL_64
	dec ebx
	jz rx_finish
	xor r11, 0a4213b21h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_308
	call rx_read_l2
rx_body_308:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r13
	mov r15, rax

rx_i_309: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r9, 090c42304h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_309
	call rx_read_l1
rx_body_309:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	mov rax, -1652850028
	imul rax, rcx
	mov rcx, rax
	mov eax, r9d
	xor eax, 09d7b8294h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_310: ;FPMUL
	dec ebx
	jz rx_finish
	xor r9, 0f78e1c8ch
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_310
	call rx_read_l1
rx_body_310:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm6
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm7, xmm0
	mov eax, r15d
	xor eax, 07c9816c0h
	and eax, 32767
	movhpd qword ptr [rsi + rax * 8], xmm7

rx_i_311: ;FPMUL
	dec ebx
	jz rx_finish
	xor r8, 0ff8848cfh
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_311
	call rx_read_l1
rx_body_311:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm4
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm4, xmm0

rx_i_312: ;MULH_64
	dec ebx
	jz rx_finish
	xor r13, 0b18904cdh
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_312
	call rx_read_l1
rx_body_312:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, -1147928648
	mul rcx
	mov rax, rdx
	mov r10, rax

rx_i_313: ;ROR_64
	dec ebx
	jz rx_finish
	xor r8, 0a0d0befh
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_313
	call rx_read_l1
rx_body_313:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	ror rax, 62
	mov rcx, rax
	mov eax, r14d
	xor eax, 09500d514h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_314: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r15, 01e3c65f7h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_314
	call rx_read_l1
rx_body_314:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r9d
	imul rax, rcx
	mov rcx, rax
	mov eax, r9d
	xor eax, 07fc7f955h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_315: ;XOR_64
	dec ebx
	jz rx_finish
	xor r9, 02e36ddafh
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_315
	call rx_read_l1
rx_body_315:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	xor rax, r15
	mov r9, rax

rx_i_316: ;RET
	dec ebx
	jz rx_finish
	xor r14, 05b0cb5bbh
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_316
	call rx_read_l1
rx_body_316:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r8d
	xor eax, 03602c513h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp rsp, rdi
	je short rx_i_317
	ret

rx_i_317: ;FPADD
	dec ebx
	jz rx_finish
	xor r9, 0c74e7415h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_317
	call rx_read_l1
rx_body_317:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm7
	movaps xmm5, xmm0

rx_i_318: ;ROR_64
	dec ebx
	jz rx_finish
	xor r9, 057621d9ah
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_318
	call rx_read_l1
rx_body_318:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	ror rax, 41
	mov rcx, rax
	mov eax, r15d
	xor eax, 061cb9db8h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_319: ;SHR_64
	dec ebx
	jz rx_finish
	xor r13, 08ee02d99h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_319
	call rx_read_l2
rx_body_319:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r15
	shr rax, cl
	mov rcx, rax
	mov eax, r11d
	xor eax, 01f931a08h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_320: ;FPADD
	dec ebx
	jz rx_finish
	xor r15, 013461188h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_320
	call rx_read_l1
rx_body_320:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm4
	movaps xmm2, xmm0
	mov eax, r10d
	xor eax, 02bdc7349h
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm2

rx_i_321: ;MUL_32
	dec ebx
	jz rx_finish
	xor r11, 0a7bae383h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_321
	call rx_read_l1
rx_body_321:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, eax
	mov eax, r9d
	imul rax, rcx
	mov rcx, rax
	mov eax, r12d
	xor eax, 0f213dach
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_322: ;CALL
	dec ebx
	jz rx_finish
	xor r14, 08215399bh
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_322
	call rx_read_l1
rx_body_322:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r11d
	xor eax, 054292224h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp r11d, 1411981860
	jno short rx_i_323
	call rx_i_343

rx_i_323: ;MUL_64
	dec ebx
	jz rx_finish
	xor r14, 07b07664bh
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_323
	call rx_read_l1
rx_body_323:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, rax, -696924877
	mov rcx, rax
	mov eax, r14d
	xor eax, 0d675c533h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_324: ;FPDIV
	dec ebx
	jz rx_finish
	xor r9, 0f956baffh
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_324
	call rx_read_l1
rx_body_324:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	divpd xmm0, xmm2
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm9, xmm0
	mov eax, r9d
	xor eax, 0944856d4h
	and eax, 32767
	movhpd qword ptr [rsi + rax * 8], xmm9

rx_i_325: ;OR_32
	dec ebx
	jz rx_finish
	xor r11, 0708ab9d1h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_325
	call rx_read_l1
rx_body_325:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	or eax, -281580460
	mov r13, rax

rx_i_326: ;MUL_64
	dec ebx
	jz rx_finish
	xor r11, 0d1b27540h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_326
	call rx_read_l1
rx_body_326:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r8
	mov rcx, rax
	mov eax, r9d
	xor eax, 0b67623c3h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_327: ;IMULH_64
	dec ebx
	jz rx_finish
	xor r9, 09665f98dh
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_327
	call rx_read_l1
rx_body_327:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r15
	imul rcx
	mov rax, rdx
	mov r12, rax

rx_i_328: ;SHR_64
	dec ebx
	jz rx_finish
	xor r12, 0fb9c32adh
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_328
	call rx_read_l1
rx_body_328:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r13
	shr rax, cl
	mov r9, rax

rx_i_329: ;RET
	dec ebx
	jz rx_finish
	xor r11, 0e1110623h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_329
	call rx_read_l1
rx_body_329:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov r11, rax
	cmp rsp, rdi
	je short rx_i_330
	ret

rx_i_330: ;MUL_32
	dec ebx
	jz rx_finish
	xor r9, 0f6a93f19h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_330
	call rx_read_l1
rx_body_330:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, eax
	mov eax, r13d
	imul rax, rcx
	mov rcx, rax
	mov eax, r11d
	xor eax, 0af8b7117h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_331: ;FPADD
	dec ebx
	jz rx_finish
	xor r9, 0bc9bbe4ah
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_331
	call rx_read_l1
rx_body_331:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm3
	movaps xmm9, xmm0

rx_i_332: ;FPADD
	dec ebx
	jz rx_finish
	xor r12, 0f253cd4eh
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_332
	call rx_read_l1
rx_body_332:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm6
	movaps xmm3, xmm0
	mov eax, r11d
	xor eax, 0116c919eh
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm3

rx_i_333: ;OR_64
	dec ebx
	jz rx_finish
	xor r14, 0f009758bh
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_333
	call rx_read_l2
rx_body_333:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	or rax, -175125848
	mov r11, rax

rx_i_334: ;ADD_64
	dec ebx
	jz rx_finish
	xor r8, 0dda04168h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_334
	call rx_read_l1
rx_body_334:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, r13
	mov r8, rax

rx_i_335: ;SUB_64
	dec ebx
	jz rx_finish
	xor r15, 03e6cfb73h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_335
	call rx_read_l1
rx_body_335:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	sub rax, r8
	mov rcx, rax
	mov eax, r12d
	xor eax, 07ffe4218h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_336: ;ROR_64
	dec ebx
	jz rx_finish
	xor r15, 0aea0a435h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_336
	call rx_read_l1
rx_body_336:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r10
	ror rax, cl
	mov rcx, rax
	mov eax, r11d
	xor eax, 02644c5ah
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_337: ;ADD_64
	dec ebx
	jz rx_finish
	xor r8, 03d6c4ab2h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_337
	call rx_read_l1
rx_body_337:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, r12
	mov rcx, rax
	mov eax, r13d
	xor eax, 0dab07c39h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_338: ;MUL_64
	dec ebx
	jz rx_finish
	xor r12, 0d428a742h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_338
	call rx_read_l2
rx_body_338:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r12
	mov r11, rax

rx_i_339: ;FPADD
	dec ebx
	jz rx_finish
	xor r9, 04596ef73h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_339
	call rx_read_l1
rx_body_339:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm6
	movaps xmm2, xmm0

rx_i_340: ;FPADD
	dec ebx
	jz rx_finish
	xor r15, 0e51629cch
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_340
	call rx_read_l1
rx_body_340:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm5
	movaps xmm5, xmm0

rx_i_341: ;MULH_64
	dec ebx
	jz rx_finish
	xor r12, 019eb9ea5h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_341
	call rx_read_l1
rx_body_341:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r15
	mul rcx
	mov rax, rdx
	mov rcx, rax
	mov eax, r8d
	xor eax, 024736405h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_342: ;FPSUB
	dec ebx
	jz rx_finish
	xor r9, 09ccc7abah
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_342
	call rx_read_l2
rx_body_342:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm2
	movaps xmm3, xmm0

rx_i_343: ;XOR_64
	dec ebx
	jz rx_finish
	xor r14, 056f6cf0bh
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_343
	call rx_read_l1
rx_body_343:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	xor rax, r13
	mov rcx, rax
	mov eax, r15d
	xor eax, 0d9a469a9h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_344: ;FPSUB
	dec ebx
	jz rx_finish
	xor r10, 03ef9bcc4h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_344
	call rx_read_l2
rx_body_344:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm6
	movaps xmm5, xmm0

rx_i_345: ;MUL_64
	dec ebx
	jz rx_finish
	xor r12, 0bbbcdbach
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_345
	call rx_read_l1
rx_body_345:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r13
	mov rcx, rax
	mov eax, r9d
	xor eax, 0ef03b0ddh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_346: ;AND_32
	dec ebx
	jz rx_finish
	xor r12, 0ae9d1e96h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_346
	call rx_read_l1
rx_body_346:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	and eax, r15d
	mov rcx, rax
	mov eax, r13d
	xor eax, 0ed2d3987h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_347: ;ADD_64
	dec ebx
	jz rx_finish
	xor r14, 070c34d69h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_347
	call rx_read_l1
rx_body_347:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, r10
	mov r13, rax

rx_i_348: ;FPSUB
	dec ebx
	jz rx_finish
	xor r13, 0523ff904h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_348
	call rx_read_l1
rx_body_348:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm3
	movaps xmm9, xmm0
	mov eax, r9d
	xor eax, 039c35461h
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm9

rx_i_349: ;OR_64
	dec ebx
	jz rx_finish
	xor r8, 018e0e5ddh
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_349
	call rx_read_l2
rx_body_349:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	or rax, r15
	mov r13, rax

rx_i_350: ;CALL
	dec ebx
	jz rx_finish
	xor r9, 09bd050f0h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_350
	call rx_read_l1
rx_body_350:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r12d
	xor eax, 0c5901b43h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp r9d, -980411581
	ja short rx_i_351
	call rx_i_352

rx_i_351: ;MUL_64
	dec ebx
	jz rx_finish
	xor r11, 0a3a5906fh
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_351
	call rx_read_l2
rx_body_351:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r10
	mov r13, rax

rx_i_352: ;FPADD
	dec ebx
	jz rx_finish
	xor r10, 0afc9af2bh
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_352
	call rx_read_l1
rx_body_352:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm6
	movaps xmm2, xmm0
	mov eax, r10d
	xor eax, 03bf686f2h
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm2

rx_i_353: ;FPSUB
	dec ebx
	jz rx_finish
	xor r13, 02e65278bh
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_353
	call rx_read_l1
rx_body_353:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm2
	movaps xmm7, xmm0
	mov eax, r15d
	xor eax, 0b3c9f7aeh
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm7

rx_i_354: ;MUL_64
	dec ebx
	jz rx_finish
	xor r13, 02412fc10h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_354
	call rx_read_l2
rx_body_354:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r13
	mov r13, rax

rx_i_355: ;MUL_64
	dec ebx
	jz rx_finish
	xor r10, 06bd6e65fh
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_355
	call rx_read_l1
rx_body_355:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r14
	mov rcx, rax
	mov eax, r8d
	xor eax, 0c1062b3ch
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_356: ;SUB_64
	dec ebx
	jz rx_finish
	xor r10, 01cd85d80h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_356
	call rx_read_l2
rx_body_356:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	sub rax, r10
	mov r11, rax

rx_i_357: ;ADD_64
	dec ebx
	jz rx_finish
	xor r10, 0f7daed36h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_357
	call rx_read_l2
rx_body_357:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	add rax, 820073637
	mov r11, rax

rx_i_358: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r13, 088fa6e5ah
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_358
	call rx_read_l2
rx_body_358:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r11d
	imul rax, rcx
	mov r9, rax

rx_i_359: ;FPSUB
	dec ebx
	jz rx_finish
	xor r10, 0714fc2cdh
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_359
	call rx_read_l2
rx_body_359:
	xor rbp, rcx
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm9
	movaps xmm4, xmm0
	mov eax, r12d
	xor eax, 0f16b9be3h
	and eax, 32767
	movlpd qword ptr [rsi + rax * 8], xmm4

rx_i_360: ;FPMUL
	dec ebx
	jz rx_finish
	xor r10, 0c2d110b5h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_360
	call rx_read_l1
rx_body_360:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm8
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm8, xmm0

rx_i_361: ;FPDIV
	dec ebx
	jz rx_finish
	xor r15, 01d125a7fh
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_361
	call rx_read_l1
rx_body_361:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	divpd xmm0, xmm6
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm6, xmm0
	mov eax, r14d
	xor eax, 0ad0b81f5h
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm6

rx_i_362: ;SUB_64
	dec ebx
	jz rx_finish
	xor r9, 0ed8954bdh
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_362
	call rx_read_l1
rx_body_362:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	sub rax, 1082179469
	mov rcx, rax
	mov eax, r15d
	xor eax, 04080bf8dh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_363: ;FPMUL
	dec ebx
	jz rx_finish
	xor r12, 09f75887bh
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_363
	call rx_read_l1
rx_body_363:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm6
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm3, xmm0

rx_i_364: ;MULH_64
	dec ebx
	jz rx_finish
	xor r11, 0badaf867h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_364
	call rx_read_l1
rx_body_364:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r8
	mul rcx
	mov rax, rdx
	mov r8, rax

rx_i_365: ;MUL_32
	dec ebx
	jz rx_finish
	xor r15, 02db4444ah
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_365
	call rx_read_l2
rx_body_365:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, eax
	mov eax, r9d
	imul rax, rcx
	mov rcx, rax
	mov eax, r12d
	xor eax, 0bfd87d37h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_366: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r12, 0bff7218fh
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_366
	call rx_read_l2
rx_body_366:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r8d
	imul rax, rcx
	mov rcx, rax
	mov eax, r15d
	xor eax, 0c3d6bcb7h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_367: ;ROR_64
	dec ebx
	jz rx_finish
	xor r9, 04d14cb3ah
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_367
	call rx_read_l2
rx_body_367:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r9
	ror rax, cl
	mov r12, rax

rx_i_368: ;SUB_64
	dec ebx
	jz rx_finish
	xor r10, 0a14836bah
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_368
	call rx_read_l1
rx_body_368:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	sub rax, r10
	mov r8, rax

rx_i_369: ;IMULH_64
	dec ebx
	jz rx_finish
	xor r9, 053fe22e2h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_369
	call rx_read_l1
rx_body_369:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r13
	imul rcx
	mov rax, rdx
	mov r9, rax

rx_i_370: ;FPSUB
	dec ebx
	jz rx_finish
	xor r15, 010e1fb24h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_370
	call rx_read_l1
rx_body_370:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm6
	movaps xmm6, xmm0
	mov eax, r14d
	xor eax, 0a120e0edh
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm6

rx_i_371: ;FPADD
	dec ebx
	jz rx_finish
	xor r8, 0ebbd5cc9h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_371
	call rx_read_l1
rx_body_371:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm9
	movaps xmm5, xmm0
	mov eax, r13d
	xor eax, 0c40fe413h
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm5

rx_i_372: ;SHL_64
	dec ebx
	jz rx_finish
	xor r10, 098ab79d7h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_372
	call rx_read_l2
rx_body_372:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r13
	shl rax, cl
	mov r9, rax

rx_i_373: ;FPMUL
	dec ebx
	jz rx_finish
	xor r15, 056438b3h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_373
	call rx_read_l2
rx_body_373:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm8
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm4, xmm0

rx_i_374: ;FPMUL
	dec ebx
	jz rx_finish
	xor r11, 0dbcce604h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_374
	call rx_read_l1
rx_body_374:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm2
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm2, xmm0

rx_i_375: ;ADD_64
	dec ebx
	jz rx_finish
	xor r9, 0edea6200h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_375
	call rx_read_l1
rx_body_375:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, r15
	mov rcx, rax
	mov eax, r12d
	xor eax, 0ec359be9h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_376: ;ADD_64
	dec ebx
	jz rx_finish
	xor r14, 05e61b279h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_376
	call rx_read_l1
rx_body_376:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, 476136066
	mov rcx, rax
	mov eax, r8d
	xor eax, 01c614282h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_377: ;FPSUB
	dec ebx
	jz rx_finish
	xor r14, 0fc1fb433h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_377
	call rx_read_l1
rx_body_377:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm3
	movaps xmm7, xmm0

rx_i_378: ;MULH_64
	dec ebx
	jz rx_finish
	xor r12, 082aa21ach
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_378
	call rx_read_l1
rx_body_378:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, 547725353
	mul rcx
	mov rax, rdx
	mov r15, rax

rx_i_379: ;ROR_64
	dec ebx
	jz rx_finish
	xor r10, 05dba41fbh
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_379
	call rx_read_l2
rx_body_379:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	ror rax, 56
	mov r13, rax

rx_i_380: ;SUB_32
	dec ebx
	jz rx_finish
	xor r11, 0229e3d6eh
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_380
	call rx_read_l1
rx_body_380:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	sub eax, -1443002912
	mov rcx, rax
	mov eax, r13d
	xor eax, 0a9fd85e0h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_381: ;XOR_32
	dec ebx
	jz rx_finish
	xor r8, 019816ff9h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_381
	call rx_read_l2
rx_body_381:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	xor eax, r14d
	mov r9, rax

rx_i_382: ;ROL_64
	dec ebx
	jz rx_finish
	xor r14, 036b5b81fh
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_382
	call rx_read_l1
rx_body_382:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	rol rax, 55
	mov r11, rax

rx_i_383: ;FPSUB
	dec ebx
	jz rx_finish
	xor r15, 05f798ec3h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_383
	call rx_read_l1
rx_body_383:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm4
	movaps xmm5, xmm0
	mov eax, r13d
	xor eax, 0c9f5cc22h
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm5

rx_i_384: ;XOR_64
	dec ebx
	jz rx_finish
	xor r10, 05b459fd7h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_384
	call rx_read_l1
rx_body_384:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	xor rax, r11
	mov rcx, rax
	mov eax, r9d
	xor eax, 054439464h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_385: ;MUL_64
	dec ebx
	jz rx_finish
	xor r15, 0c91749bbh
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_385
	call rx_read_l1
rx_body_385:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r12
	mov rcx, rax
	mov eax, r13d
	xor eax, 0fb9b50b9h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_386: ;FPADD
	dec ebx
	jz rx_finish
	xor r9, 0575b4bdch
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_386
	call rx_read_l2
rx_body_386:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm8
	movaps xmm9, xmm0

rx_i_387: ;SUB_64
	dec ebx
	jz rx_finish
	xor r9, 0d4f7bc6ah
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_387
	call rx_read_l2
rx_body_387:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	sub rax, r15
	mov r9, rax

rx_i_388: ;RET
	dec ebx
	jz rx_finish
	xor r8, 08a949356h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_388
	call rx_read_l2
rx_body_388:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r11d
	xor eax, 0a0985cc2h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp rsp, rdi
	je short rx_i_389
	ret

rx_i_389: ;JUMP
	dec ebx
	jz rx_finish
	xor r11, 06531ad2eh
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_389
	call rx_read_l1
rx_body_389:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov r14, rax
	cmp r9d, -350609584
	jge rx_i_421

rx_i_390: ;FPADD
	dec ebx
	jz rx_finish
	xor r15, 02914abeah
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_390
	call rx_read_l1
rx_body_390:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm4
	movaps xmm3, xmm0

rx_i_391: ;FPADD
	dec ebx
	jz rx_finish
	xor r8, 0473a41f0h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_391
	call rx_read_l1
rx_body_391:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm3
	movaps xmm6, xmm0

rx_i_392: ;SAR_64
	dec ebx
	jz rx_finish
	xor r14, 01ebc1f0dh
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_392
	call rx_read_l2
rx_body_392:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	sar rax, 0
	mov rcx, rax
	mov eax, r13d
	xor eax, 08c4a0f0dh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_393: ;AND_64
	dec ebx
	jz rx_finish
	xor r14, 0742e95b1h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_393
	call rx_read_l1
rx_body_393:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	and rax, 552339548
	mov rcx, rax
	mov eax, r13d
	xor eax, 020ec085ch
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_394: ;FPADD
	dec ebx
	jz rx_finish
	xor r12, 0db885c2ch
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_394
	call rx_read_l2
rx_body_394:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm9
	movaps xmm6, xmm0

rx_i_395: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r8, 04ae4fe8ch
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_395
	call rx_read_l1
rx_body_395:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r13d
	imul rax, rcx
	mov r8, rax

rx_i_396: ;ROR_64
	dec ebx
	jz rx_finish
	xor r10, 07b41862bh
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_396
	call rx_read_l1
rx_body_396:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	ror rax, 62
	mov rcx, rax
	mov eax, r12d
	xor eax, 01ee1c837h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_397: ;SUB_64
	dec ebx
	jz rx_finish
	xor r8, 0916f3819h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_397
	call rx_read_l1
rx_body_397:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	sub rax, r12
	mov rcx, rax
	mov eax, r10d
	xor eax, 0146db5dfh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_398: ;SHR_64
	dec ebx
	jz rx_finish
	xor r8, 04eb6fd2ah
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_398
	call rx_read_l1
rx_body_398:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	shr rax, 44
	mov rcx, rax
	mov eax, r11d
	xor eax, 0724e7136h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_399: ;FPMUL
	dec ebx
	jz rx_finish
	xor r11, 0899a98cfh
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_399
	call rx_read_l1
rx_body_399:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm2
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm6, xmm0

rx_i_400: ;AND_64
	dec ebx
	jz rx_finish
	xor r13, 0aae75db6h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_400
	call rx_read_l1
rx_body_400:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	and rax, r11
	mov rcx, rax
	mov eax, r14d
	xor eax, 094ac538ch
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_401: ;FPSUB
	dec ebx
	jz rx_finish
	xor r13, 032e81f25h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_401
	call rx_read_l1
rx_body_401:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm4
	movaps xmm6, xmm0
	mov eax, r14d
	xor eax, 03ea60344h
	and eax, 32767
	movlpd qword ptr [rsi + rax * 8], xmm6

rx_i_402: ;RET
	dec ebx
	jz rx_finish
	xor r9, 0fa1a07ffh
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_402
	call rx_read_l1
rx_body_402:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov r14, rax
	cmp rsp, rdi
	je short rx_i_403
	ret

rx_i_403: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r9, 0e59500f7h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_403
	call rx_read_l1
rx_body_403:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r12d
	imul rax, rcx
	mov rcx, rax
	mov eax, r11d
	xor eax, 01ff394a0h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_404: ;MULH_64
	dec ebx
	jz rx_finish
	xor r15, 05b8ceb2fh
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_404
	call rx_read_l1
rx_body_404:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r8
	mul rcx
	mov rax, rdx
	mov r15, rax

rx_i_405: ;CALL
	dec ebx
	jz rx_finish
	xor r8, 0f61082a3h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_405
	call rx_read_l1
rx_body_405:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r12d
	xor eax, 06b0af6c1h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp r10d, 1795880641
	ja short rx_i_406
	call rx_i_494

rx_i_406: ;FPDIV
	dec ebx
	jz rx_finish
	xor r9, 0af6886b7h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_406
	call rx_read_l2
rx_body_406:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	divpd xmm0, xmm7
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm9, xmm0
	mov eax, r9d
	xor eax, 09862adefh
	and eax, 32767
	movlpd qword ptr [rsi + rax * 8], xmm9

rx_i_407: ;FPSUB
	dec ebx
	jz rx_finish
	xor r14, 09699566fh
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_407
	call rx_read_l2
rx_body_407:
	xor rbp, rcx
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm9
	movaps xmm8, xmm0

rx_i_408: ;MUL_64
	dec ebx
	jz rx_finish
	xor r15, 066e79fa6h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_408
	call rx_read_l1
rx_body_408:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r9
	mov rcx, rax
	mov eax, r10d
	xor eax, 0295004c9h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_409: ;MUL_64
	dec ebx
	jz rx_finish
	xor r11, 04b6caa9ah
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_409
	call rx_read_l1
rx_body_409:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r15
	mov r8, rax

rx_i_410: ;RET
	dec ebx
	jz rx_finish
	xor r15, 0d17f245eh
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_410
	call rx_read_l1
rx_body_410:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov r8, rax
	cmp rsp, rdi
	je short rx_i_411
	ret

rx_i_411: ;RET
	dec ebx
	jz rx_finish
	xor r12, 0364f10e7h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_411
	call rx_read_l1
rx_body_411:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov r12, rax
	cmp rsp, rdi
	je short rx_i_412
	ret

rx_i_412: ;FPDIV
	dec ebx
	jz rx_finish
	xor r10, 0ac90e7ah
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_412
	call rx_read_l1
rx_body_412:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	divpd xmm0, xmm4
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm3, xmm0
	mov eax, r11d
	xor eax, 0bbd2640ah
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm3

rx_i_413: ;FPMUL
	dec ebx
	jz rx_finish
	xor r11, 04b6037abh
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_413
	call rx_read_l1
rx_body_413:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm2
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm4, xmm0

rx_i_414: ;AND_64
	dec ebx
	jz rx_finish
	xor r14, 06c01554dh
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_414
	call rx_read_l1
rx_body_414:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	and rax, r8
	mov rcx, rax
	mov eax, r10d
	xor eax, 0e973b3b1h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_415: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r8, 08c3e59a1h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_415
	call rx_read_l1
rx_body_415:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	mov rax, -538093385
	imul rax, rcx
	mov r9, rax

rx_i_416: ;FPADD
	dec ebx
	jz rx_finish
	xor r12, 0f3fafde9h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_416
	call rx_read_l1
rx_body_416:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm3
	movaps xmm5, xmm0
	mov eax, r13d
	xor eax, 0f84b5382h
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm5

rx_i_417: ;SUB_64
	dec ebx
	jz rx_finish
	xor r10, 03c6481fah
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_417
	call rx_read_l1
rx_body_417:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	sub rax, r12
	mov r10, rax

rx_i_418: ;MUL_64
	dec ebx
	jz rx_finish
	xor r10, 02bd61c5fh
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_418
	call rx_read_l1
rx_body_418:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r11
	mov r10, rax

rx_i_419: ;OR_64
	dec ebx
	jz rx_finish
	xor r9, 0b6ab9d32h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_419
	call rx_read_l1
rx_body_419:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	or rax, r14
	mov rcx, rax
	mov eax, r14d
	xor eax, 0beeca8dbh
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_420: ;ROR_64
	dec ebx
	jz rx_finish
	xor r9, 0f9690ceah
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_420
	call rx_read_l1
rx_body_420:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r11
	ror rax, cl
	mov r9, rax

rx_i_421: ;CALL
	dec ebx
	jz rx_finish
	xor r12, 01ada0f39h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_421
	call rx_read_l2
rx_body_421:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov r10, rax
	cmp r8d, -1600409762
	jo short rx_i_422
	call rx_i_31

rx_i_422: ;MUL_32
	dec ebx
	jz rx_finish
	xor r11, 04dd16ca4h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_422
	call rx_read_l2
rx_body_422:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, eax
	mov eax, r10d
	imul rax, rcx
	mov r13, rax

rx_i_423: ;SUB_64
	dec ebx
	jz rx_finish
	xor r12, 04df5ce05h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_423
	call rx_read_l1
rx_body_423:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	sub rax, r10
	mov rcx, rax
	mov eax, r15d
	xor eax, 0a5d40d0ah
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_424: ;FPADD
	dec ebx
	jz rx_finish
	xor r13, 01ad12ce2h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_424
	call rx_read_l2
rx_body_424:
	xor rbp, rcx
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm7
	movaps xmm9, xmm0
	mov eax, r9d
	xor eax, 0565ae8aah
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm9

rx_i_425: ;MUL_32
	dec ebx
	jz rx_finish
	xor r8, 0a3c5391dh
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_425
	call rx_read_l1
rx_body_425:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, eax
	mov eax, r10d
	imul rax, rcx
	mov r14, rax

rx_i_426: ;IMULH_64
	dec ebx
	jz rx_finish
	xor r12, 09dd55ba0h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_426
	call rx_read_l2
rx_body_426:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r9
	imul rcx
	mov rax, rdx
	mov rcx, rax
	mov eax, r14d
	xor eax, 0dcca31efh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_427: ;MULH_64
	dec ebx
	jz rx_finish
	xor r11, 0d6cae9aeh
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_427
	call rx_read_l1
rx_body_427:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r11
	mul rcx
	mov rax, rdx
	mov rcx, rax
	mov eax, r9d
	xor eax, 0801190f4h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_428: ;RET
	dec ebx
	jz rx_finish
	xor r11, 0f807a961h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_428
	call rx_read_l1
rx_body_428:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r8d
	xor eax, 0e3b86b2fh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp rsp, rdi
	je short rx_i_429
	ret

rx_i_429: ;MUL_64
	dec ebx
	jz rx_finish
	xor r12, 0650a4102h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_429
	call rx_read_l2
rx_body_429:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, rax, 1990438276
	mov r15, rax

rx_i_430: ;FPADD
	dec ebx
	jz rx_finish
	xor r14, 019cc0e5h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_430
	call rx_read_l1
rx_body_430:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm8
	movaps xmm5, xmm0
	mov eax, r13d
	xor eax, 058891433h
	and eax, 32767
	movhpd qword ptr [rsi + rax * 8], xmm5

rx_i_431: ;ROR_64
	dec ebx
	jz rx_finish
	xor r12, 0ed17ab58h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_431
	call rx_read_l1
rx_body_431:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r13
	ror rax, cl
	mov r13, rax

rx_i_432: ;SUB_64
	dec ebx
	jz rx_finish
	xor r10, 01c3b321fh
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_432
	call rx_read_l2
rx_body_432:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	sub rax, r10
	mov r8, rax

rx_i_433: ;ADD_64
	dec ebx
	jz rx_finish
	xor r13, 0bbb88499h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_433
	call rx_read_l1
rx_body_433:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, r12
	mov rcx, rax
	mov eax, r12d
	xor eax, 04722b36fh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_434: ;FPDIV
	dec ebx
	jz rx_finish
	xor r13, 0167edabdh
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_434
	call rx_read_l2
rx_body_434:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	divpd xmm0, xmm3
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm9, xmm0
	mov eax, r9d
	xor eax, 08c1cfc74h
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm9

rx_i_435: ;MUL_64
	dec ebx
	jz rx_finish
	xor r15, 0b940480ah
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_435
	call rx_read_l1
rx_body_435:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r15
	mov rcx, rax
	mov eax, r9d
	xor eax, 0758605ffh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_436: ;FPADD
	dec ebx
	jz rx_finish
	xor r15, 0bfc3ca8bh
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_436
	call rx_read_l2
rx_body_436:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm2
	movaps xmm7, xmm0
	mov eax, r15d
	xor eax, 0bfa76c43h
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm7

rx_i_437: ;FPMUL
	dec ebx
	jz rx_finish
	xor r8, 098a6bcf7h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_437
	call rx_read_l1
rx_body_437:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm3
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm8, xmm0

rx_i_438: ;FPMUL
	dec ebx
	jz rx_finish
	xor r10, 0325b38ebh
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_438
	call rx_read_l1
rx_body_438:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm9
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm4, xmm0

rx_i_439: ;OR_64
	dec ebx
	jz rx_finish
	xor r13, 05e807e81h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_439
	call rx_read_l2
rx_body_439:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	or rax, r15
	mov rcx, rax
	mov eax, r10d
	xor eax, 0b28e6e01h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_440: ;CALL
	dec ebx
	jz rx_finish
	xor r10, 062f83728h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_440
	call rx_read_l1
rx_body_440:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov r9, rax
	cmp r12d, 2127765370
	jns short rx_i_441
	call rx_i_41

rx_i_441: ;ADD_64
	dec ebx
	jz rx_finish
	xor r14, 0d18ec075h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_441
	call rx_read_l1
rx_body_441:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, 529736748
	mov rcx, rax
	mov eax, r9d
	xor eax, 01f93242ch
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_442: ;FPSQRT
	dec ebx
	jz rx_finish
	xor r14, 0a53dd1bh
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_442
	call rx_read_l1
rx_body_442:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	andps xmm0, xmm10
	sqrtpd xmm3, xmm0

rx_i_443: ;RET
	dec ebx
	jz rx_finish
	xor r14, 0232d1285h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_443
	call rx_read_l1
rx_body_443:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r9d
	xor eax, 04f71c419h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp rsp, rdi
	je short rx_i_444
	ret

rx_i_444: ;FPSUB
	dec ebx
	jz rx_finish
	xor r8, 042455dd8h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_444
	call rx_read_l2
rx_body_444:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm7
	movaps xmm5, xmm0
	mov eax, r13d
	xor eax, 0ce416070h
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm5

rx_i_445: ;ADD_64
	dec ebx
	jz rx_finish
	xor r13, 09ae009b2h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_445
	call rx_read_l1
rx_body_445:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, r11
	mov rcx, rax
	mov eax, r9d
	xor eax, 084d1f575h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_446: ;MULH_64
	dec ebx
	jz rx_finish
	xor r12, 01734708eh
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_446
	call rx_read_l1
rx_body_446:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r15
	mul rcx
	mov rax, rdx
	mov rcx, rax
	mov eax, r13d
	xor eax, 03166163h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_447: ;FPADD
	dec ebx
	jz rx_finish
	xor r8, 01596d0e8h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_447
	call rx_read_l1
rx_body_447:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm7
	movaps xmm5, xmm0
	mov eax, r13d
	xor eax, 0b384d4afh
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm5

rx_i_448: ;FPSUB
	dec ebx
	jz rx_finish
	xor r9, 0390cfdb0h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_448
	call rx_read_l1
rx_body_448:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm3
	movaps xmm9, xmm0

rx_i_449: ;ROL_64
	dec ebx
	jz rx_finish
	xor r8, 04f27744bh
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_449
	call rx_read_l1
rx_body_449:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	rol rax, 28
	mov r8, rax

rx_i_450: ;SAR_64
	dec ebx
	jz rx_finish
	xor r8, 04e2c76ffh
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_450
	call rx_read_l1
rx_body_450:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r12
	sar rax, cl
	mov rcx, rax
	mov eax, r8d
	xor eax, 0f6de92ach
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_451: ;ADD_64
	dec ebx
	jz rx_finish
	xor r8, 0c4d99ac9h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_451
	call rx_read_l1
rx_body_451:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, -287502157
	mov r8, rax

rx_i_452: ;RET
	dec ebx
	jz rx_finish
	xor r13, 040130b88h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_452
	call rx_read_l1
rx_body_452:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r11d
	xor eax, 0e27dea25h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp rsp, rdi
	je short rx_i_453
	ret

rx_i_453: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r11, 0a2096aa4h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_453
	call rx_read_l1
rx_body_453:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r14d
	imul rax, rcx
	mov r8, rax

rx_i_454: ;FPADD
	dec ebx
	jz rx_finish
	xor r13, 081314291h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_454
	call rx_read_l1
rx_body_454:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm9
	movaps xmm4, xmm0
	mov eax, r12d
	xor eax, 07e41c60fh
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm4

rx_i_455: ;OR_64
	dec ebx
	jz rx_finish
	xor r8, 059263cdbh
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_455
	call rx_read_l1
rx_body_455:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	or rax, r9
	mov r8, rax

rx_i_456: ;AND_64
	dec ebx
	jz rx_finish
	xor r9, 010e8fe6h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_456
	call rx_read_l2
rx_body_456:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	and rax, r11
	mov rcx, rax
	mov eax, r9d
	xor eax, 017f52c3fh
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_457: ;SUB_64
	dec ebx
	jz rx_finish
	xor r9, 09de1a3efh
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_457
	call rx_read_l1
rx_body_457:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	sub rax, r10
	mov rcx, rax
	mov eax, r10d
	xor eax, 058584136h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_458: ;SAR_64
	dec ebx
	jz rx_finish
	xor r11, 05c79df6eh
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_458
	call rx_read_l1
rx_body_458:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	sar rax, 22
	mov r14, rax

rx_i_459: ;SUB_64
	dec ebx
	jz rx_finish
	xor r9, 0346f46adh
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_459
	call rx_read_l1
rx_body_459:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	sub rax, 381354340
	mov rcx, rax
	mov eax, r13d
	xor eax, 016bb0164h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_460: ;ADD_64
	dec ebx
	jz rx_finish
	xor r11, 098ab71fch
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_460
	call rx_read_l1
rx_body_460:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, r14
	mov rcx, rax
	mov eax, r12d
	xor eax, 0eb453a97h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_461: ;XOR_64
	dec ebx
	jz rx_finish
	xor r11, 0c814e926h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_461
	call rx_read_l2
rx_body_461:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	xor rax, r13
	mov rcx, rax
	mov eax, r12d
	xor eax, 062ef5b99h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_462: ;ADD_64
	dec ebx
	jz rx_finish
	xor r10, 0c64b4a9eh
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_462
	call rx_read_l2
rx_body_462:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	add rax, -1734323376
	mov r15, rax

rx_i_463: ;ADD_64
	dec ebx
	jz rx_finish
	xor r9, 08c29341h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_463
	call rx_read_l1
rx_body_463:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, r15
	mov r10, rax

rx_i_464: ;MUL_64
	dec ebx
	jz rx_finish
	xor r12, 06ff587fdh
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_464
	call rx_read_l1
rx_body_464:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r15
	mov rcx, rax
	mov eax, r13d
	xor eax, 0d0673df8h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_465: ;FPADD
	dec ebx
	jz rx_finish
	xor r12, 0b62c0003h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_465
	call rx_read_l2
rx_body_465:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm5
	movaps xmm2, xmm0

rx_i_466: ;MUL_32
	dec ebx
	jz rx_finish
	xor r13, 05c541c42h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_466
	call rx_read_l1
rx_body_466:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, eax
	mov eax, 282682508
	imul rax, rcx
	mov r9, rax

rx_i_467: ;FPADD
	dec ebx
	jz rx_finish
	xor r8, 0cbb33f81h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_467
	call rx_read_l1
rx_body_467:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm9
	movaps xmm8, xmm0

rx_i_468: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r8, 091044dc3h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_468
	call rx_read_l1
rx_body_468:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	mov rax, -13394825
	imul rax, rcx
	mov rcx, rax
	mov eax, r8d
	xor eax, 0ff339c77h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_469: ;MUL_32
	dec ebx
	jz rx_finish
	xor r9, 0c0186beh
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_469
	call rx_read_l1
rx_body_469:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, eax
	mov eax, 294019485
	imul rax, rcx
	mov rcx, rax
	mov eax, r9d
	xor eax, 01186619dh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_470: ;OR_64
	dec ebx
	jz rx_finish
	xor r14, 090849e3eh
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_470
	call rx_read_l1
rx_body_470:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	or rax, r11
	mov rcx, rax
	mov eax, r14d
	xor eax, 090d56b4ch
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_471: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r14, 0cedba9b6h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_471
	call rx_read_l1
rx_body_471:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r13d
	imul rax, rcx
	mov r14, rax

rx_i_472: ;JUMP
	dec ebx
	jz rx_finish
	xor r9, 038f4b9d6h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_472
	call rx_read_l2
rx_body_472:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov r10, rax
	cmp r10d, 1738497427
	jl rx_i_8

rx_i_473: ;MUL_64
	dec ebx
	jz rx_finish
	xor r14, 01fb7637dh
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_473
	call rx_read_l1
rx_body_473:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, rax, -751043211
	mov r12, rax

rx_i_474: ;JUMP
	dec ebx
	jz rx_finish
	xor r9, 0b5c0b4d4h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_474
	call rx_read_l2
rx_body_474:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov r15, rax
	cmp r15d, -233120543
	jo rx_i_69

rx_i_475: ;FPSUB
	dec ebx
	jz rx_finish
	xor r10, 0910dcdeeh
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_475
	call rx_read_l2
rx_body_475:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm9
	movaps xmm7, xmm0

rx_i_476: ;FPADD
	dec ebx
	jz rx_finish
	xor r8, 07ab3b5a4h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_476
	call rx_read_l1
rx_body_476:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm2
	movaps xmm9, xmm0

rx_i_477: ;FPADD
	dec ebx
	jz rx_finish
	xor r12, 07a29ec63h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_477
	call rx_read_l1
rx_body_477:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm9
	movaps xmm6, xmm0
	mov eax, r14d
	xor eax, 0e81fc7a6h
	and eax, 2047
	movhpd qword ptr [rsi + rax * 8], xmm6

rx_i_478: ;MUL_64
	dec ebx
	jz rx_finish
	xor r14, 02d3d7e7fh
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_478
	call rx_read_l1
rx_body_478:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r10
	mov r12, rax

rx_i_479: ;MUL_64
	dec ebx
	jz rx_finish
	xor r12, 09b49c793h
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_479
	call rx_read_l1
rx_body_479:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	imul rax, r14
	mov rcx, rax
	mov eax, r13d
	xor eax, 0c42735ech
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_480: ;FPADD
	dec ebx
	jz rx_finish
	xor r9, 0a9cc4f01h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_480
	call rx_read_l1
rx_body_480:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm4
	movaps xmm6, xmm0

rx_i_481: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r14, 0225ba1f9h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_481
	call rx_read_l1
rx_body_481:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	movsxd rax, r13d
	imul rax, rcx
	mov r12, rax

rx_i_482: ;AND_32
	dec ebx
	jz rx_finish
	xor r14, 044a0f592h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_482
	call rx_read_l2
rx_body_482:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	and eax, r12d
	mov r11, rax

rx_i_483: ;FPADD
	dec ebx
	jz rx_finish
	xor r11, 07f71f219h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_483
	call rx_read_l1
rx_body_483:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm6
	movaps xmm6, xmm0

rx_i_484: ;SHR_64
	dec ebx
	jz rx_finish
	xor r12, 07027bacdh
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_484
	call rx_read_l1
rx_body_484:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	shr rax, 37
	mov r11, rax

rx_i_485: ;JUMP
	dec ebx
	jz rx_finish
	xor r13, 03a04647h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_485
	call rx_read_l2
rx_body_485:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r15d
	xor eax, 02112cbaeh
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx
	cmp r8d, 554879918
	jno rx_i_58

rx_i_486: ;ADD_64
	dec ebx
	jz rx_finish
	xor r15, 0ad072937h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_486
	call rx_read_l1
rx_body_486:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	add rax, 942846898
	mov rcx, rax
	mov eax, r8d
	xor eax, 03832b3b2h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_487: ;SUB_64
	dec ebx
	jz rx_finish
	xor r11, 07f78ad34h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_487
	call rx_read_l1
rx_body_487:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	sub rax, -333279706
	mov r11, rax

rx_i_488: ;IMUL_32
	dec ebx
	jz rx_finish
	xor r12, 0d8b1788eh
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_488
	call rx_read_l1
rx_body_488:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	movsxd rcx, eax
	mov rax, 297357073
	imul rax, rcx
	mov r12, rax

rx_i_489: ;JUMP
	dec ebx
	jz rx_finish
	xor r10, 0b2ec9f3ah
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_489
	call rx_read_l1
rx_body_489:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r8d
	xor eax, 0bcd0a942h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp r15d, -1127175870
	jge rx_i_75

rx_i_490: ;ROR_64
	dec ebx
	jz rx_finish
	xor r11, 015c7f598h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_490
	call rx_read_l2
rx_body_490:
	xor rbp, rcx
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r9
	ror rax, cl
	mov rcx, rax
	mov eax, r15d
	xor eax, 0ab8bd68h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_491: ;FPADD
	dec ebx
	jz rx_finish
	xor r8, 0902da6bdh
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_491
	call rx_read_l2
rx_body_491:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm9
	movaps xmm7, xmm0
	mov eax, r15d
	xor eax, 0b0f0fca4h
	and eax, 32767
	movhpd qword ptr [rsi + rax * 8], xmm7

rx_i_492: ;IDIV_64
	dec ebx
	jz rx_finish
	xor r9, 0491090d9h
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_492
	call rx_read_l2
rx_body_492:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov edx, r9d
	cmp edx, -1
	jne short safe_idiv_492
	mov rcx, rax
	rol rcx, 1
	dec rcx
	jz short result_idiv_492
safe_idiv_492:
	mov ecx, 1
	test edx, edx
	cmovne ecx, edx
	movsxd rcx, ecx
	cqo
	idiv rcx
result_idiv_492:
	mov r12, rax

rx_i_493: ;FPSUB
	dec ebx
	jz rx_finish
	xor r8, 09de81282h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_493
	call rx_read_l1
rx_body_493:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm9
	movaps xmm4, xmm0

rx_i_494: ;MULH_64
	dec ebx
	jz rx_finish
	xor r10, 0b0d50e46h
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_494
	call rx_read_l2
rx_body_494:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r11
	mul rcx
	mov rax, rdx
	mov r14, rax

rx_i_495: ;FPMUL
	dec ebx
	jz rx_finish
	xor r11, 0e276cad1h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_495
	call rx_read_l1
rx_body_495:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm2
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm8, xmm0

rx_i_496: ;DIV_64
	dec ebx
	jz rx_finish
	xor r14, 0fe757b73h
	mov ecx, r14d
	test bl, 63
	jnz short rx_body_496
	call rx_read_l1
rx_body_496:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, -359802064
	xor edx, edx
	div rcx
	mov r9, rax

rx_i_497: ;FPMUL
	dec ebx
	jz rx_finish
	xor r8, 08d25742eh
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_497
	call rx_read_l1
rx_body_497:
	xor rbp, rcx
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm3
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm8, xmm0

rx_i_498: ;FPMUL
	dec ebx
	jz rx_finish
	xor r15, 0e066fd15h
	mov ecx, r15d
	test bl, 63
	jnz short rx_body_498
	call rx_read_l1
rx_body_498:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	mulpd xmm0, xmm9
	movaps xmm1, xmm0
	cmpeqpd xmm1, xmm1
	andps xmm0, xmm1
	movaps xmm8, xmm0
	mov eax, r8d
	xor eax, 09dc5a1f9h
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm8

rx_i_499: ;MUL_32
	dec ebx
	jz rx_finish
	xor r12, 08925556bh
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_499
	call rx_read_l2
rx_body_499:
	and ecx, 32767
	mov rax, qword ptr [rsi+rcx*8]
	mov ecx, eax
	mov eax, -1795485757
	imul rax, rcx
	mov r8, rax

rx_i_500: ;FPSQRT
	dec ebx
	jz rx_finish
	xor r10, 04bc870ebh
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_500
	call rx_read_l1
rx_body_500:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	andps xmm0, xmm10
	sqrtpd xmm2, xmm0

rx_i_501: ;XOR_64
	dec ebx
	jz rx_finish
	xor r8, 07d46c503h
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_501
	call rx_read_l1
rx_body_501:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	xor rax, r10
	mov rcx, rax
	mov eax, r12d
	xor eax, 03e22874bh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_502: ;RET
	dec ebx
	jz rx_finish
	xor r10, 09e70b20ch
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_502
	call rx_read_l1
rx_body_502:
	xor rbp, rcx
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	mov eax, r9d
	xor eax, 08d85312h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	cmp rsp, rdi
	je short rx_i_503
	ret

rx_i_503: ;FPSUB
	dec ebx
	jz rx_finish
	xor r13, 0442e4850h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_503
	call rx_read_l1
rx_body_503:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm2
	movaps xmm9, xmm0
	mov eax, r9d
	xor eax, 080465282h
	and eax, 2047
	movlpd qword ptr [rsi + rax * 8], xmm9

rx_i_504: ;FPADD
	dec ebx
	jz rx_finish
	xor r13, 099d48347h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_504
	call rx_read_l1
rx_body_504:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm9
	movaps xmm4, xmm0
	mov eax, r12d
	xor eax, 0be8cbb18h
	and eax, 32767
	movhpd qword ptr [rsi + rax * 8], xmm4

rx_i_505: ;FPSUB
	dec ebx
	jz rx_finish
	xor r12, 032c0a28ah
	mov ecx, r12d
	test bl, 63
	jnz short rx_body_505
	call rx_read_l2
rx_body_505:
	and ecx, 32767
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm4
	movaps xmm8, xmm0
	mov eax, r8d
	xor eax, 021b54eaeh
	and eax, 32767
	movhpd qword ptr [rsi + rax * 8], xmm8

rx_i_506: ;FPSUB
	dec ebx
	jz rx_finish
	xor r9, 0a973d58ch
	mov ecx, r9d
	test bl, 63
	jnz short rx_body_506
	call rx_read_l1
rx_body_506:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	subpd xmm0, xmm9
	movaps xmm3, xmm0

rx_i_507: ;RET
	dec ebx
	jz rx_finish
	xor r10, 0d3b7165ch
	mov ecx, r10d
	test bl, 63
	jnz short rx_body_507
	call rx_read_l1
rx_body_507:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov r14, rax
	cmp rsp, rdi
	je short rx_i_508
	ret

rx_i_508: ;RET
	dec ebx
	jz rx_finish
	xor r13, 0da34d818h
	mov ecx, r13d
	test bl, 63
	jnz short rx_body_508
	call rx_read_l1
rx_body_508:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov r8, rax
	cmp rsp, rdi
	je short rx_i_509
	ret

rx_i_509: ;FPROUND
	dec ebx
	jz rx_finish
	xor r11, 01b2873f2h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_509
	call rx_read_l1
rx_body_509:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, rax
	rol rax, 34
	and eax, 24576
	or eax, 40896
	mov dword ptr [rsp - 8], eax
	ldmxcsr dword ptr [rsp - 8]
	mov r10, rcx

rx_i_510: ;FPADD
	dec ebx
	jz rx_finish
	xor r8, 0db65513ch
	mov ecx, r8d
	test bl, 63
	jnz short rx_body_510
	call rx_read_l1
rx_body_510:
	and ecx, 2047
	cvtdq2pd xmm0, qword ptr [rsi+rcx*8]
	addpd xmm0, xmm2
	movaps xmm9, xmm0

rx_i_511: ;SHR_64
	dec ebx
	jz rx_finish
	xor r11, 02bd79286h
	mov ecx, r11d
	test bl, 63
	jnz short rx_body_511
	call rx_read_l1
rx_body_511:
	and ecx, 2047
	mov rax, qword ptr [rsi+rcx*8]
	mov rcx, r10
	shr rax, cl
	mov r11, rax

	jmp rx_i_0
