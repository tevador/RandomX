rx_i_0: ;ADD_64
	dec edi
	js rx_finish
	xor r14, 087d93944h
	mov eax, r14d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	add rax, r10
	mov r14, rax

rx_i_1: ;IMULH_64
	dec edi
	js rx_finish
	xor rbx, 0d7a5aadbh
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, -615634046
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, r14d
	xor eax, 0db4e2b82h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_2: ;RET
	dec edi
	js rx_finish
	xor r10, 06ffcedb1h
	mov eax, r10d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	cmp rsp, rbp
	je short not_taken_ret_2
	cmp ebx, 330026357
	ja short not_taken_ret_2
	xor rax, qword ptr [rsp + 8]
	mov r13, rax
	ret 8
not_taken_ret_2:
	mov r13, rax

rx_i_3: ;RET
	dec edi
	js rx_finish
	xor r9, 0a35d739ch
	mov eax, r9d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	cmp rsp, rbp
	je short not_taken_ret_3
	cmp r14d, 2014518380
	ja short not_taken_ret_3
	xor rax, qword ptr [rsp + 8]
	mov rcx, rax
	mov eax, r11d
	xor eax, 078131c6ch
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	ret 8
not_taken_ret_3:
	mov rcx, rax
	mov eax, r11d
	xor eax, 078131c6ch
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_4: ;RET
	dec edi
	js rx_finish
	xor r9, 0ca9788ah
	mov eax, r9d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	cmp rsp, rbp
	je short not_taken_ret_4
	cmp r11d, 445530481
	ja short not_taken_ret_4
	xor rax, qword ptr [rsp + 8]
	mov rcx, rax
	mov eax, r12d
	xor eax, 01a8e4171h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	ret 8
not_taken_ret_4:
	mov rcx, rax
	mov eax, r12d
	xor eax, 01a8e4171h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_5: ;IMULH_64
	dec edi
	js rx_finish
	xor r15, 06afc2fa4h
	mov ecx, r15d
	call rx_read_dataset
	mov r8, rdx
	mov rcx, r10
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov r12, rax

rx_i_6: ;CALL
	dec edi
	js rx_finish
	xor r15, 097210f7bh
	mov eax, r15d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	cmp r11d, 1348521207
	jbe short taken_call_6
	mov rcx, rax
	mov eax, r9d
	xor eax, 05060ccf7h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	jmp rx_i_7
taken_call_6:
	push rax
	call rx_i_51

rx_i_7: ;FPDIV
	dec edi
	js rx_finish
	xor r13, 082c73195h
	mov ecx, r13d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	divsd xmm0, xmm1
	movsd xmm8, xmm0

rx_i_8: ;MULH_64
	dec edi
	js rx_finish
	xor r14, 077daefb4h
	mov eax, r14d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, r14
	mul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, r9d
	xor eax, 06ce10c20h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_9: ;MUL_32
	dec edi
	js rx_finish
	xor r15, 0379f9ee0h
	mov ecx, r15d
	call rx_read_dataset
	mov ecx, eax
	mov eax, r12d
	imul rax, rcx
	mov r12, rax

rx_i_10: ;MUL_64
	dec edi
	js rx_finish
	xor rbx, 03bae7272h
	mov ecx, ebx
	call rx_read_dataset
	imul rax, r15
	mov rcx, rax
	mov eax, r9d
	xor eax, 098a649d1h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_11: ;FPADD
	dec edi
	js rx_finish
	xor r10, 0e264ed81h
	mov eax, r10d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm6
	movsd xmm6, xmm0

rx_i_12: ;XOR_32
	dec edi
	js rx_finish
	xor r13, 068c1e5d2h
	mov ecx, r13d
	call rx_read_dataset
	xor eax, 1344700093
	mov rcx, rax
	mov eax, r12d
	xor eax, 050267ebdh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_13: ;DIV_64
	dec edi
	js rx_finish
	xor r14, 085121c54h
	mov eax, r14d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov ecx, 565870810
	xor edx, edx
	div rcx
	mov rdx, r8
	mov r10, rax

rx_i_14: ;AND_32
	dec edi
	js rx_finish
	xor rbx, 052efde3eh
	mov ecx, ebx
	call rx_read_dataset
	and eax, -727859809
	mov r13, rax

rx_i_15: ;FPADD
	dec edi
	js rx_finish
	xor r10, 0a9bf8aa1h
	mov ecx, r10d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm5
	movsd xmm4, xmm0

rx_i_16: ;FPSQRT
	dec edi
	js rx_finish
	xor r10, 0db2691ch
	mov ecx, r10d
	call rx_read_dataset
	mov rcx, 9223372036854773760
	and rax, rcx
	cvtsi2sd xmm0, rax
	sqrtsd xmm0, xmm0
	movsd xmm8, xmm0

rx_i_17: ;FPADD
	dec edi
	js rx_finish
	xor r12, 061c0d34dh
	mov ecx, r12d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm3
	mov eax, r9d
	xor eax, 04f2f223ch
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_18: ;SHL_64
	dec edi
	js rx_finish
	xor r10, 0e761d1beh
	mov ecx, r10d
	call rx_read_dataset
	mov rcx, r9
	shl rax, cl
	mov rcx, rax
	mov eax, r10d
	xor eax, 03c1a72f8h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_19: ;RET
	dec edi
	js rx_finish
	xor r11, 074ddb688h
	mov ecx, r11d
	call rx_read_dataset
	cmp rsp, rbp
	je short not_taken_ret_19
	cmp r11d, 1183529144
	ja short not_taken_ret_19
	xor rax, qword ptr [rsp + 8]
	mov rcx, rax
	mov eax, r14d
	xor eax, 0468b38b8h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx
	ret 8
not_taken_ret_19:
	mov rcx, rax
	mov eax, r14d
	xor eax, 0468b38b8h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_20: ;ADD_64
	dec edi
	js rx_finish
	xor r14, 06be90627h
	mov eax, r14d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	add rax, r10
	mov rcx, rax
	mov eax, r9d
	xor eax, 0d7e75aeh
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_21: ;FPSUB
	dec edi
	js rx_finish
	xor r11, 0fbc6fc35h
	mov eax, r11d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm4
	movsd xmm4, xmm0

rx_i_22: ;FPSUB
	dec edi
	js rx_finish
	xor r14, 0c28ca080h
	mov eax, r14d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm4
	movsd xmm3, xmm0

rx_i_23: ;FPADD
	dec edi
	js rx_finish
	xor r13, 0ac009c30h
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm8
	mov eax, r15d
	xor eax, 0e92dc022h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_24: ;FPSUB
	dec edi
	js rx_finish
	xor r13, 0ecca967dh
	mov ecx, r13d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm2
	movsd xmm7, xmm0

rx_i_25: ;ROR_64
	dec edi
	js rx_finish
	xor rbx, 0977f0284h
	mov ecx, ebx
	call rx_read_dataset
	mov rcx, r9
	ror rax, cl
	mov rcx, rax
	mov eax, r15d
	xor eax, 0db5e0aafh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_26: ;ADD_64
	dec edi
	js rx_finish
	xor r13, 080bdfefah
	mov ecx, r13d
	call rx_read_dataset
	add rax, rbx
	mov rcx, rax
	mov eax, r10d
	xor eax, 0cfa09799h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_27: ;SUB_32
	dec edi
	js rx_finish
	xor r15, 0e1e0d3c4h
	mov ecx, r15d
	call rx_read_dataset
	sub eax, r11d
	mov rbx, rax

rx_i_28: ;IMULH_64
	dec edi
	js rx_finish
	xor rbx, 070d3b8c7h
	mov ecx, ebx
	call rx_read_dataset
	mov r8, rdx
	mov rcx, r15
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, r15d
	xor eax, 099b77a68h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_29: ;FPMUL
	dec edi
	js rx_finish
	xor r12, 01cf77a04h
	mov ecx, r12d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm1
	movsd xmm6, xmm0

rx_i_30: ;IMUL_32
	dec edi
	js rx_finish
	xor r11, 0e311468ch
	mov ecx, r11d
	call rx_read_dataset
	movsxd rcx, eax
	movsxd rax, r13d
	imul rax, rcx
	mov rcx, rax
	mov eax, r9d
	xor eax, 0306ff9ech
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_31: ;FPMUL
	dec edi
	js rx_finish
	xor r12, 01fd9911ah
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm3
	mov eax, r14d
	xor eax, 04b5d4e80h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_32: ;OR_64
	dec edi
	js rx_finish
	xor r13, 067df757eh
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, r13
	mov r14, rax

rx_i_33: ;ADD_64
	dec edi
	js rx_finish
	xor r12, 0be2e7c42h
	mov ecx, r12d
	call rx_read_dataset
	add rax, 1944166515
	mov r14, rax

rx_i_34: ;ROR_64
	dec edi
	js rx_finish
	xor r11, 084d067f7h
	mov eax, r11d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	ror rax, 27
	mov rcx, rax
	mov eax, r15d
	xor eax, 0dd52e4f9h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_35: ;ROR_64
	dec edi
	js rx_finish
	xor r14, 0d352ce37h
	mov eax, r14d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, r11
	ror rax, cl
	mov r14, rax

rx_i_36: ;OR_64
	dec edi
	js rx_finish
	xor r12, 0a1f248dah
	mov ecx, r12d
	call rx_read_dataset
	or rax, -1936869641
	mov r9, rax

rx_i_37: ;MUL_64
	dec edi
	js rx_finish
	xor r9, 0554720fch
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	imul rax, r15
	mov r12, rax

rx_i_38: ;CALL
	dec edi
	js rx_finish
	xor r13, 0665e91f1h
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	cmp r14d, -380224718
	jbe short taken_call_38
	mov r15, rax
	jmp rx_i_39
taken_call_38:
	push rax
	call rx_i_112

rx_i_39: ;CALL
	dec edi
	js rx_finish
	xor r15, 05ef1be79h
	mov eax, r15d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	cmp r9d, -2040787098
	jbe short taken_call_39
	mov rbx, rax
	jmp rx_i_40
taken_call_39:
	push rax
	call rx_i_62

rx_i_40: ;FPMUL
	dec edi
	js rx_finish
	xor rbx, 012ec7e3ah
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm6
	mov eax, r15d
	xor eax, 07a07ae2ah
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_41: ;FPSUB
	dec edi
	js rx_finish
	xor r12, 0d0706601h
	mov eax, r12d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, -4480846364313387008
	movd xmm9, rax
	subsd xmm0, xmm9
	movsd xmm1, xmm0

rx_i_42: ;SUB_64
	dec edi
	js rx_finish
	xor r9, 064056913h
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	sub rax, r14
	mov r10, rax

rx_i_43: ;ADD_64
	dec edi
	js rx_finish
	xor r14, 02c1f1eb0h
	mov ecx, r14d
	call rx_read_dataset
	add rax, r14
	mov r14, rax

rx_i_44: ;CALL
	dec edi
	js rx_finish
	xor r10, 068fd9009h
	mov eax, r10d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	cmp r12d, -1297973554
	jbe short taken_call_44
	mov rcx, rax
	mov eax, r9d
	xor eax, 0b2a27eceh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	jmp rx_i_45
taken_call_44:
	push rax
	call rx_i_94

rx_i_45: ;FPROUND
	dec edi
	js rx_finish
	xor r9, 037a30933h
	mov eax, r9d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, rax
	shl eax, 13
	and rcx, -2048
	and eax, 24576
	cvtsi2sd xmm0, rcx
	or eax, 40896
	mov dword ptr [rsp - 8], eax
	ldmxcsr dword ptr [rsp - 8]
	movsd xmm1, xmm0

rx_i_46: ;FPADD
	dec edi
	js rx_finish
	xor r15, 0bc1de9f6h
	mov eax, r15d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, 4739074351570092032
	movd xmm9, rax
	addsd xmm0, xmm9
	mov eax, r14d
	xor eax, 029260733h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_47: ;ADD_64
	dec edi
	js rx_finish
	xor r12, 02b2a2eech
	mov ecx, r12d
	call rx_read_dataset
	add rax, 1693705407
	mov rcx, rax
	mov eax, r11d
	xor eax, 064f3e4bfh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_48: ;ROL_64
	dec edi
	js rx_finish
	xor r11, 0685817abh
	mov ecx, r11d
	call rx_read_dataset
	mov rcx, r9
	rol rax, cl
	mov r15, rax

rx_i_49: ;FPSUB
	dec edi
	js rx_finish
	xor r12, 08cd244ebh
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, -4478227626472767488
	movd xmm9, rax
	subsd xmm0, xmm9
	mov eax, r13d
	xor eax, 0977132cdh
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_50: ;ADD_64
	dec edi
	js rx_finish
	xor rbx, 06d8f4254h
	mov ecx, ebx
	call rx_read_dataset
	add rax, r9
	mov rcx, rax
	mov eax, ebx
	xor eax, 0e9f58436h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_51: ;CALL
	dec edi
	js rx_finish
	xor r12, 05ba232c6h
	mov ecx, r12d
	call rx_read_dataset
	cmp r10d, 119251505
	jbe short taken_call_51
	mov rcx, rax
	mov eax, r13d
	xor eax, 071ba231h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx
	jmp rx_i_52
taken_call_51:
	push rax
	call rx_i_135

rx_i_52: ;FPDIV
	dec edi
	js rx_finish
	xor rbx, 0aaed618fh
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	divsd xmm0, xmm3
	movsd xmm1, xmm0

rx_i_53: ;FPSUB
	dec edi
	js rx_finish
	xor rbx, 0f96c6a45h
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm3
	mov eax, r13d
	xor eax, 0c56b47bh
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_54: ;OR_64
	dec edi
	js rx_finish
	xor r9, 0da3e4842h
	mov eax, r9d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	or rax, r10
	mov rcx, rax
	mov eax, r15d
	xor eax, 06ac56a2ah
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_55: ;ADD_32
	dec edi
	js rx_finish
	xor r10, 0302b676ah
	mov ecx, r10d
	call rx_read_dataset
	add eax, 419241919
	mov r15, rax

rx_i_56: ;FPDIV
	dec edi
	js rx_finish
	xor r11, 0fa88f48bh
	mov ecx, r11d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	divsd xmm0, xmm5
	mov eax, r15d
	xor eax, 0e0254dafh
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_57: ;RET
	dec edi
	js rx_finish
	xor r13, 03dff9b9eh
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	cmp rsp, rbp
	je short not_taken_ret_57
	cmp ebx, 2028798189
	ja short not_taken_ret_57
	xor rax, qword ptr [rsp + 8]
	mov r13, rax
	ret 8
not_taken_ret_57:
	mov r13, rax

rx_i_58: ;IMULH_64
	dec edi
	js rx_finish
	xor r11, 060638de0h
	mov eax, r11d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, 282209221
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, r12d
	xor eax, 010d22bc5h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_59: ;FPMUL
	dec edi
	js rx_finish
	xor r10, 0dda983d4h
	mov eax, r10d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm5
	movsd xmm3, xmm0

rx_i_60: ;DIV_64
	dec edi
	js rx_finish
	xor r14, 0f1456b8eh
	mov eax, r14d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
mov ecx, 1
	mov edx, r15d
	test edx, edx
	cmovne ecx, edx
	xor edx, edx
	div rcx
	mov rdx, r8
	mov rcx, rax
	mov eax, ebx
	xor eax, 0fcf95491h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_61: ;SUB_64
	dec edi
	js rx_finish
	xor r9, 010dc4571h
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	sub rax, r14
	mov rcx, rax
	mov eax, r15d
	xor eax, 0a426387h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_62: ;IMULH_64
	dec edi
	js rx_finish
	xor r14, 0bcec0ebah
	mov eax, r14d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, r13
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov rbx, rax

rx_i_63: ;FPADD
	dec edi
	js rx_finish
	xor r11, 0980dd402h
	mov eax, r11d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm8
	mov eax, r15d
	xor eax, 04f4e2c91h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_64: ;RET
	dec edi
	js rx_finish
	xor r15, 03de14d1eh
	mov ecx, r15d
	call rx_read_dataset
	cmp rsp, rbp
	je short not_taken_ret_64
	cmp r11d, 2075529029
	ja short not_taken_ret_64
	xor rax, qword ptr [rsp + 8]
	mov rcx, rax
	mov eax, r11d
	xor eax, 07bb60f45h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	ret 8
not_taken_ret_64:
	mov rcx, rax
	mov eax, r11d
	xor eax, 07bb60f45h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_65: ;CALL
	dec edi
	js rx_finish
	xor r13, 05058ce64h
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	cmp r15d, 1933164545
	jbe short taken_call_65
	mov r11, rax
	jmp rx_i_66
taken_call_65:
	push rax
	call rx_i_124

rx_i_66: ;FPSUB
	dec edi
	js rx_finish
	xor r15, 0c3089414h
	mov ecx, r15d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, 4744280396844236800
	movd xmm9, rax
	subsd xmm0, xmm9
	movsd xmm2, xmm0

rx_i_67: ;FPSUB
	dec edi
	js rx_finish
	xor r9, 065cf272eh
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, -4480946344868970496
	movd xmm9, rax
	subsd xmm0, xmm9
	mov eax, ebx
	xor eax, 0be13d69eh
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_68: ;ADD_32
	dec edi
	js rx_finish
	xor r13, 0ae54dfbfh
	mov ecx, r13d
	call rx_read_dataset
	add eax, r15d
	mov r9, rax

rx_i_69: ;CALL
	dec edi
	js rx_finish
	xor r13, 07b366ce6h
	mov ecx, r13d
	call rx_read_dataset
	cmp ebx, 1498056607
	jbe short taken_call_69
	mov r11, rax
	jmp rx_i_70
taken_call_69:
	push rax
	call rx_i_133

rx_i_70: ;FPDIV
	dec edi
	js rx_finish
	xor r15, 015a1b689h
	mov ecx, r15d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	divsd xmm0, xmm3
	movsd xmm1, xmm0

rx_i_71: ;CALL
	dec edi
	js rx_finish
	xor r14, 088393ba0h
	mov ecx, r14d
	call rx_read_dataset
	cmp r13d, 2031541081
	jbe short taken_call_71
	mov r9, rax
	jmp rx_i_72
taken_call_71:
	push rax
	call rx_i_83

rx_i_72: ;FPADD
	dec edi
	js rx_finish
	xor r13, 03aa5c3a4h
	mov ecx, r13d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm2
	movsd xmm4, xmm0

rx_i_73: ;FPADD
	dec edi
	js rx_finish
	xor r15, 0376c9c27h
	mov ecx, r15d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm5
	mov eax, ebx
	xor eax, 098c2e84dh
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_74: ;MUL_64
	dec edi
	js rx_finish
	xor rbx, 0bbbec3fah
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	imul rax, r9
	mov r13, rax

rx_i_75: ;FPMUL
	dec edi
	js rx_finish
	xor r14, 0e9efb350h
	mov eax, r14d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, 4743866573565984768
	movd xmm9, rax
	mulsd xmm0, xmm9
	mov eax, r15d
	xor eax, 056660eedh
	and eax, 32767
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_76: ;CALL
	dec edi
	js rx_finish
	xor r13, 0f4e51e28h
	mov ecx, r13d
	call rx_read_dataset
	cmp r9d, -631091751
	jbe short taken_call_76
	mov rcx, rax
	mov eax, r11d
	xor eax, 0da624dd9h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	jmp rx_i_77
taken_call_76:
	push rax
	call rx_i_195

rx_i_77: ;FPDIV
	dec edi
	js rx_finish
	xor r12, 0c24ddbd4h
	mov ecx, r12d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, 4742317416992735232
	movd xmm9, rax
	divsd xmm0, xmm9
	mov eax, r10d
	xor eax, 040624270h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_78: ;MUL_64
	dec edi
	js rx_finish
	xor rbx, 04c4b0c7fh
	mov ecx, ebx
	call rx_read_dataset
	imul rax, rax, -1431647438
	mov rcx, rax
	mov eax, r9d
	xor eax, 0aaaacb32h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_79: ;CALL
	dec edi
	js rx_finish
	xor r14, 03bcc02e3h
	mov eax, r14d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	cmp r11d, -1160798683
	jbe short taken_call_79
	mov r13, rax
	jmp rx_i_80
taken_call_79:
	push rax
	call rx_i_206

rx_i_80: ;FPADD
	dec edi
	js rx_finish
	xor r11, 04b0ff63eh
	mov eax, r11d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm3
	movsd xmm7, xmm0

rx_i_81: ;RET
	dec edi
	js rx_finish
	xor r14, 0b956b3e8h
	mov eax, r14d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	cmp rsp, rbp
	je short not_taken_ret_81
	cmp r15d, 982695034
	ja short not_taken_ret_81
	xor rax, qword ptr [rsp + 8]
	mov rcx, rax
	mov eax, r11d
	xor eax, 03a92bc7ah
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	ret 8
not_taken_ret_81:
	mov rcx, rax
	mov eax, r11d
	xor eax, 03a92bc7ah
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_82: ;MULH_64
	dec edi
	js rx_finish
	xor r9, 0edeca680h
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, rbx
	mul rcx
	mov rax, rdx
	mov rdx, r8
	mov r15, rax

rx_i_83: ;CALL
	dec edi
	js rx_finish
	xor r11, 0fbdddcb5h
	mov eax, r11d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	cmp r13d, 1800043331
	jbe short taken_call_83
	mov rcx, rax
	mov eax, r11d
	xor eax, 06b4a7b43h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	jmp rx_i_84
taken_call_83:
	push rax
	call rx_i_97

rx_i_84: ;ROR_64
	dec edi
	js rx_finish
	xor r13, 09cec97a1h
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	ror rax, 4
	mov rcx, rax
	mov eax, r11d
	xor eax, 01a681d13h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_85: ;AND_32
	dec edi
	js rx_finish
	xor r15, 078228167h
	mov ecx, r15d
	call rx_read_dataset
	and eax, r13d
	mov rbx, rax

rx_i_86: ;CALL
	dec edi
	js rx_finish
	xor r11, 078cae1ffh
	mov ecx, r11d
	call rx_read_dataset
	cmp r12d, -68969733
	jbe short taken_call_86
	mov rcx, rax
	mov eax, r10d
	xor eax, 0fbe39afbh
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx
	jmp rx_i_87
taken_call_86:
	push rax
	call rx_i_149

rx_i_87: ;DIV_64
	dec edi
	js rx_finish
	xor r10, 0d9b6a533h
	mov eax, r10d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
mov ecx, 1
	mov edx, r10d
	test edx, edx
	cmovne ecx, edx
	xor edx, edx
	div rcx
	mov rdx, r8
	mov r12, rax

rx_i_88: ;ROL_64
	dec edi
	js rx_finish
	xor r15, 0e9e75336h
	mov ecx, r15d
	call rx_read_dataset
	mov rcx, r10
	rol rax, cl
	mov rcx, rax
	mov eax, r13d
	xor eax, 0ec5c52e6h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_89: ;MUL_64
	dec edi
	js rx_finish
	xor r13, 04c0d378ah
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	imul rax, rbx
	mov r10, rax

rx_i_90: ;AND_32
	dec edi
	js rx_finish
	xor r11, 04386e368h
	mov ecx, r11d
	call rx_read_dataset
	and eax, ebx
	mov rcx, rax
	mov eax, r12d
	xor eax, 0a90410e4h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_91: ;ADD_64
	dec edi
	js rx_finish
	xor r9, 0d75a0ecfh
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	add rax, r12
	mov rbx, rax

rx_i_92: ;ROR_64
	dec edi
	js rx_finish
	xor r9, 031bb7f7ah
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, r14
	ror rax, cl
	mov r9, rax

rx_i_93: ;MUL_64
	dec edi
	js rx_finish
	xor r9, 03b45ecebh
	mov ecx, r9d
	call rx_read_dataset
	imul rax, rbx
	mov rcx, rax
	mov eax, r10d
	xor eax, 0e67532afh
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_94: ;FPADD
	dec edi
	js rx_finish
	xor r12, 0ee08e76bh
	mov eax, r12d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm3
	mov eax, r14d
	xor eax, 0b435cf2dh
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_95: ;FPMUL
	dec edi
	js rx_finish
	xor r9, 042e28e94h
	mov ecx, r9d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm2
	mov eax, r12d
	xor eax, 0b723c20bh
	and eax, 32767
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_96: ;CALL
	dec edi
	js rx_finish
	xor rbx, 0729260e1h
	mov ecx, ebx
	call rx_read_dataset
	push rax
	call rx_i_174

rx_i_97: ;ROR_64
	dec edi
	js rx_finish
	xor rbx, 0bfcebaf4h
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, r10
	ror rax, cl
	mov r10, rax

rx_i_98: ;CALL
	dec edi
	js rx_finish
	xor r13, 0ea326630h
	mov eax, r13d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	cmp r13d, -343122976
	jbe short taken_call_98
	mov rbx, rax
	jmp rx_i_99
taken_call_98:
	push rax
	call rx_i_161

rx_i_99: ;MUL_64
	dec edi
	js rx_finish
	xor r13, 0b5451a2dh
	mov ecx, r13d
	call rx_read_dataset
	imul rax, r10
	mov rcx, rax
	mov eax, r15d
	xor eax, 01023aa04h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_100: ;MUL_32
	dec edi
	js rx_finish
	xor r11, 04f912ef8h
	mov ecx, r11d
	call rx_read_dataset
	mov ecx, eax
	mov eax, -1354397081
	imul rax, rcx
	mov r11, rax

rx_i_101: ;FPDIV
	dec edi
	js rx_finish
	xor r15, 0acc45b3bh
	mov ecx, r15d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	divsd xmm0, xmm1
	movsd xmm5, xmm0

rx_i_102: ;ADD_32
	dec edi
	js rx_finish
	xor r14, 09900a4e8h
	mov ecx, r14d
	call rx_read_dataset
	add eax, r15d
	mov r14, rax

rx_i_103: ;FPMUL
	dec edi
	js rx_finish
	xor r9, 0841b2984h
	mov ecx, r9d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, 4743144143516073984
	movd xmm9, rax
	mulsd xmm0, xmm9
	movsd xmm4, xmm0

rx_i_104: ;ADD_64
	dec edi
	js rx_finish
	xor r15, 07ebea48fh
	mov ecx, r15d
	call rx_read_dataset
	add rax, r9
	mov r14, rax

rx_i_105: ;SUB_64
	dec edi
	js rx_finish
	xor r10, 0631209d3h
	mov eax, r10d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	sub rax, rbx
	mov r11, rax

rx_i_106: ;FPMUL
	dec edi
	js rx_finish
	xor r10, 0e50bf07ah
	mov eax, r10d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm3
	mov eax, r15d
	xor eax, 03ec98420h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_107: ;MUL_64
	dec edi
	js rx_finish
	xor r10, 02b7096f1h
	mov eax, r10d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	imul rax, r13
	mov rcx, rax
	mov eax, r15d
	xor eax, 0e4dd92b6h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_108: ;IMULH_64
	dec edi
	js rx_finish
	xor r11, 075deaf71h
	mov eax, r11d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, -1913070089
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, r15d
	xor eax, 08df8ddf7h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_109: ;MULH_64
	dec edi
	js rx_finish
	xor r13, 036a51f72h
	mov ecx, r13d
	call rx_read_dataset
	mov r8, rdx
	mov rcx, r15
	mul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, r14d
	xor eax, 09c8724edh
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_110: ;FPMUL
	dec edi
	js rx_finish
	xor r11, 07b512986h
	mov ecx, r11d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm3
	movsd xmm4, xmm0

rx_i_111: ;CALL
	dec edi
	js rx_finish
	xor r12, 0f1d2e50h
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	push rax
	call rx_i_147

rx_i_112: ;FPMUL
	dec edi
	js rx_finish
	xor r9, 07327ba60h
	mov ecx, r9d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm5
	movsd xmm1, xmm0

rx_i_113: ;ROR_64
	dec edi
	js rx_finish
	xor r15, 0594e37deh
	mov eax, r15d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, r10
	ror rax, cl
	mov rcx, rax
	mov eax, r11d
	xor eax, 094ab5a5ch
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_114: ;ROL_64
	dec edi
	js rx_finish
	xor r9, 04cdf5ebah
	mov ecx, r9d
	call rx_read_dataset
	mov rcx, r9
	rol rax, cl
	mov rcx, rax
	mov eax, r14d
	xor eax, 0ec68532fh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_115: ;CALL
	dec edi
	js rx_finish
	xor rbx, 02e16c97ch
	mov ecx, ebx
	call rx_read_dataset
	push rax
	call rx_i_216

rx_i_116: ;ADD_32
	dec edi
	js rx_finish
	xor r12, 0d42ddbd4h
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	add eax, r13d
	mov rcx, rax
	mov eax, r14d
	xor eax, 0c2d8d431h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_117: ;MULH_64
	dec edi
	js rx_finish
	xor r10, 07a4f8cbbh
	mov ecx, r10d
	call rx_read_dataset
	mov r8, rdx
	mov rcx, r9
	mul rcx
	mov rax, rdx
	mov rdx, r8
	mov r13, rax

rx_i_118: ;IMULH_64
	dec edi
	js rx_finish
	xor r13, 06e83e2cdh
	mov ecx, r13d
	call rx_read_dataset
	mov r8, rdx
	mov rcx, r15
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov r14, rax

rx_i_119: ;AND_64
	dec edi
	js rx_finish
	xor r14, 0336c980eh
	mov ecx, r14d
	call rx_read_dataset
	and rax, r10
	mov r14, rax

rx_i_120: ;IMULH_64
	dec edi
	js rx_finish
	xor r10, 0d122702eh
	mov ecx, r10d
	call rx_read_dataset
	mov r8, rdx
	mov rcx, -1850776691
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, ebx
	xor eax, 091af638dh
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_121: ;IMULH_64
	dec edi
	js rx_finish
	xor r11, 015f2012bh
	mov eax, r11d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, -1205826972
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, r15d
	xor eax, 0b8208a64h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_122: ;FPSUB
	dec edi
	js rx_finish
	xor r9, 037ddf43dh
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm5
	mov eax, r14d
	xor eax, 0d0b219d0h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_123: ;FPSUB
	dec edi
	js rx_finish
	xor r9, 0bba475f3h
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm3
	mov eax, r13d
	xor eax, 02401488h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_124: ;FPADD
	dec edi
	js rx_finish
	xor r12, 0e5561e3eh
	mov eax, r12d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm4
	mov eax, ebx
	xor eax, 04d46f867h
	and eax, 32767
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_125: ;FPSUB
	dec edi
	js rx_finish
	xor r9, 03ab8f73h
	mov ecx, r9d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm5
	mov eax, ebx
	xor eax, 0808a2d8bh
	and eax, 32767
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_126: ;CALL
	dec edi
	js rx_finish
	xor r10, 04e0dbd40h
	mov ecx, r10d
	call rx_read_dataset
	cmp r11d, 2029448233
	jbe short taken_call_126
	mov rcx, rax
	mov eax, r14d
	xor eax, 078f6ec29h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	jmp rx_i_127
taken_call_126:
	push rax
	call rx_i_196

rx_i_127: ;ADD_64
	dec edi
	js rx_finish
	xor r13, 073e9f58ah
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	add rax, r15
	mov r13, rax

rx_i_128: ;CALL
	dec edi
	js rx_finish
	xor r12, 0e3fa3670h
	mov ecx, r12d
	call rx_read_dataset
	cmp r11d, 1719505436
	jbe short taken_call_128
	mov rcx, rax
	mov eax, r11d
	xor eax, 0667d921ch
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	jmp rx_i_129
taken_call_128:
	push rax
	call rx_i_241

rx_i_129: ;MUL_32
	dec edi
	js rx_finish
	xor rbx, 0ebec27cdh
	mov ecx, ebx
	call rx_read_dataset
	mov ecx, eax
	mov eax, r14d
	imul rax, rcx
	mov r14, rax

rx_i_130: ;FPMUL
	dec edi
	js rx_finish
	xor rbx, 01feb5264h
	mov eax, ebx
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm6
	mov eax, r10d
	xor eax, 04b88e021h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_131: ;IMUL_32
	dec edi
	js rx_finish
	xor r9, 0405f500fh
	mov ecx, r9d
	call rx_read_dataset
	movsxd rcx, eax
	movsxd rax, r10d
	imul rax, rcx
	mov rbx, rax

rx_i_132: ;MUL_64
	dec edi
	js rx_finish
	xor r13, 0459f1154h
	mov ecx, r13d
	call rx_read_dataset
	imul rax, r9
	mov r9, rax

rx_i_133: ;CALL
	dec edi
	js rx_finish
	xor r9, 081918b4ch
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	push rax
	call rx_i_158

rx_i_134: ;AND_64
	dec edi
	js rx_finish
	xor r9, 077c3b332h
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -281794782
	mov rcx, rax
	mov eax, r11d
	xor eax, 0ef342722h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_135: ;RET
	dec edi
	js rx_finish
	xor r12, 05792310bh
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	cmp rsp, rbp
	je short not_taken_ret_135
	cmp r15d, -537890955
	ja short not_taken_ret_135
	xor rax, qword ptr [rsp + 8]
	mov rcx, rax
	mov eax, r15d
	xor eax, 0dff06f75h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	ret 8
not_taken_ret_135:
	mov rcx, rax
	mov eax, r15d
	xor eax, 0dff06f75h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_136: ;ROR_64
	dec edi
	js rx_finish
	xor r10, 0ebc6e10h
	mov ecx, r10d
	call rx_read_dataset
	mov rcx, r14
	ror rax, cl
	mov rcx, rax
	mov eax, r15d
	xor eax, 0b0c38959h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_137: ;OR_32
	dec edi
	js rx_finish
	xor r14, 0822f8b60h
	mov ecx, r14d
	call rx_read_dataset
	or eax, -1000526796
	mov rcx, rax
	mov eax, r15d
	xor eax, 0c45d2c34h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_138: ;ADD_64
	dec edi
	js rx_finish
	xor r10, 0d0f18593h
	mov ecx, r10d
	call rx_read_dataset
	add rax, 1516102347
	mov r13, rax

rx_i_139: ;FPMUL
	dec edi
	js rx_finish
	xor r11, 088212ef9h
	mov ecx, r11d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm3
	mov eax, ebx
	xor eax, 0b29f3d2ah
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_140: ;FPDIV
	dec edi
	js rx_finish
	xor rbx, 01ae56e03h
	mov ecx, ebx
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	divsd xmm0, xmm8
	movsd xmm5, xmm0

rx_i_141: ;SAR_64
	dec edi
	js rx_finish
	xor r11, 015a24231h
	mov eax, r11d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, r9
	sar rax, cl
	mov r11, rax

rx_i_142: ;RET
	dec edi
	js rx_finish
	xor r13, 02fd380c5h
	mov ecx, r13d
	call rx_read_dataset
	cmp rsp, rbp
	je short not_taken_ret_142
	cmp r9d, -1910517416
	ja short not_taken_ret_142
	xor rax, qword ptr [rsp + 8]
	mov r10, rax
	ret 8
not_taken_ret_142:
	mov r10, rax

rx_i_143: ;ADD_64
	dec edi
	js rx_finish
	xor r9, 093172470h
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	add rax, 515364082
	mov rcx, rax
	mov eax, r11d
	xor eax, 01eb7d4f2h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_144: ;IMUL_32
	dec edi
	js rx_finish
	xor r14, 052543553h
	mov eax, r14d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	movsxd rcx, eax
	movsxd rax, r11d
	imul rax, rcx
	mov r14, rax

rx_i_145: ;FPADD
	dec edi
	js rx_finish
	xor rbx, 02f636da1h
	mov ecx, ebx
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, -4478407513863094272
	movd xmm9, rax
	addsd xmm0, xmm9
	movsd xmm1, xmm0

rx_i_146: ;CALL
	dec edi
	js rx_finish
	xor r11, 0b11a4f2ch
	mov ecx, r11d
	call rx_read_dataset
	cmp r12d, 1365939282
	jbe short taken_call_146
	mov rcx, rax
	mov eax, r10d
	xor eax, 0516a9452h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx
	jmp rx_i_147
taken_call_146:
	push rax
	call rx_i_261

rx_i_147: ;IMUL_32
	dec edi
	js rx_finish
	xor r15, 037f4b5d0h
	mov eax, r15d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	movsxd rcx, eax
	movsxd rax, r11d
	imul rax, rcx
	mov r9, rax

rx_i_148: ;IMULH_64
	dec edi
	js rx_finish
	xor r10, 02e59e00ah
	mov eax, r10d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, r11
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov r15, rax

rx_i_149: ;IMULH_64
	dec edi
	js rx_finish
	xor r13, 08d5c798h
	mov ecx, r13d
	call rx_read_dataset
	mov r8, rdx
	mov rcx, r11
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, r10d
	xor eax, 0dd491985h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_150: ;IMULH_64
	dec edi
	js rx_finish
	xor r13, 02327e6e2h
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, r12
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov r10, rax

rx_i_151: ;MUL_64
	dec edi
	js rx_finish
	xor r13, 03a7df043h
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	imul rax, rax, 1784404616
	mov rcx, rax
	mov eax, r12d
	xor eax, 06a5bda88h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_152: ;ADD_32
	dec edi
	js rx_finish
	xor r10, 0783e5c4eh
	mov ecx, r10d
	call rx_read_dataset
	add eax, r14d
	mov rcx, rax
	mov eax, r10d
	xor eax, 08c783d2ch
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_153: ;MUL_32
	dec edi
	js rx_finish
	xor r12, 0aa0f5b2fh
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov ecx, eax
	mov eax, r14d
	imul rax, rcx
	mov rcx, rax
	mov eax, ebx
	xor eax, 09046b787h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_154: ;IMULH_64
	dec edi
	js rx_finish
	xor r9, 01504ca7ah
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, rbx
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, r9d
	xor eax, 0c854a524h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_155: ;OR_64
	dec edi
	js rx_finish
	xor r9, 0ea72a7cfh
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, r13
	mov rcx, rax
	mov eax, r11d
	xor eax, 087aed7f2h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_156: ;ROL_64
	dec edi
	js rx_finish
	xor r13, 0ad0e7a88h
	mov ecx, r13d
	call rx_read_dataset
	mov rcx, r10
	rol rax, cl
	mov r10, rax

rx_i_157: ;FPMUL
	dec edi
	js rx_finish
	xor r15, 0fd95ab87h
	mov ecx, r15d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm2
	movsd xmm8, xmm0

rx_i_158: ;MUL_32
	dec edi
	js rx_finish
	xor r10, 0256697b0h
	mov eax, r10d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov ecx, eax
	mov eax, r13d
	imul rax, rcx
	mov r10, rax

rx_i_159: ;ROL_64
	dec edi
	js rx_finish
	xor r11, 0d23f3b78h
	mov ecx, r11d
	call rx_read_dataset
	mov rcx, r10
	rol rax, cl
	mov rcx, rax
	mov eax, r13d
	xor eax, 01c5d3ebeh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_160: ;IMUL_32
	dec edi
	js rx_finish
	xor r10, 098917533h
	mov ecx, r10d
	call rx_read_dataset
	movsxd rcx, eax
	movsxd rax, r15d
	imul rax, rcx
	mov r15, rax

rx_i_161: ;ADD_64
	dec edi
	js rx_finish
	xor r10, 0dfac3efch
	mov ecx, r10d
	call rx_read_dataset
	add rax, r12
	mov r14, rax

rx_i_162: ;ADD_64
	dec edi
	js rx_finish
	xor r15, 0a64de090h
	mov eax, r15d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	add rax, 1233402159
	mov r10, rax

rx_i_163: ;CALL
	dec edi
	js rx_finish
	xor r13, 0952a3abbh
	mov ecx, r13d
	call rx_read_dataset
	cmp r15d, -8571241
	jbe short taken_call_163
	mov rcx, rax
	mov eax, r13d
	xor eax, 0ff7d3697h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	jmp rx_i_164
taken_call_163:
	push rax
	call rx_i_185

rx_i_164: ;ADD_32
	dec edi
	js rx_finish
	xor r14, 0b1685b90h
	mov ecx, r14d
	call rx_read_dataset
	add eax, 1518778665
	mov rcx, rax
	mov eax, r10d
	xor eax, 05a86b929h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_165: ;AND_64
	dec edi
	js rx_finish
	xor r15, 0ea992531h
	mov ecx, r15d
	call rx_read_dataset
	and rax, r14
	mov rbx, rax

rx_i_166: ;SHR_64
	dec edi
	js rx_finish
	xor r9, 01fd57a4ah
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, r10
	shr rax, cl
	mov r13, rax

rx_i_167: ;ADD_64
	dec edi
	js rx_finish
	xor r12, 0e3486c0ah
	mov ecx, r12d
	call rx_read_dataset
	add rax, -2101130488
	mov rcx, rax
	mov eax, r14d
	xor eax, 082c34b08h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_168: ;MULH_64
	dec edi
	js rx_finish
	xor r12, 01f0c2737h
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, r9
	mul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, r13d
	xor eax, 09aa6da19h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_169: ;RET
	dec edi
	js rx_finish
	xor r12, 0debb493eh
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	cmp rsp, rbp
	je short not_taken_ret_169
	cmp ebx, 1682991196
	ja short not_taken_ret_169
	xor rax, qword ptr [rsp + 8]
	mov rcx, rax
	mov eax, r9d
	xor eax, 06450685ch
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx
	ret 8
not_taken_ret_169:
	mov rcx, rax
	mov eax, r9d
	xor eax, 06450685ch
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_170: ;SAR_64
	dec edi
	js rx_finish
	xor r9, 0fe684081h
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, rbx
	sar rax, cl
	mov rcx, rax
	mov eax, r13d
	xor eax, 0bb67f8abh
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_171: ;FPMUL
	dec edi
	js rx_finish
	xor r11, 0d10371ch
	mov eax, r11d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, 4739242735460941824
	movd xmm9, rax
	mulsd xmm0, xmm9
	movsd xmm2, xmm0

rx_i_172: ;FPMUL
	dec edi
	js rx_finish
	xor r12, 071b15effh
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm5
	mov eax, r15d
	xor eax, 08d1a76f8h
	and eax, 32767
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_173: ;CALL
	dec edi
	js rx_finish
	xor r11, 072790347h
	mov ecx, r11d
	call rx_read_dataset
	cmp r10d, -1286357107
	jbe short taken_call_173
	mov rcx, rax
	mov eax, r14d
	xor eax, 0b353bf8dh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	jmp rx_i_174
taken_call_173:
	push rax
	call rx_i_201

rx_i_174: ;FPSQRT
	dec edi
	js rx_finish
	xor rbx, 04ae8a020h
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, 9223372036854773760
	and rax, rcx
	cvtsi2sd xmm0, rax
	sqrtsd xmm0, xmm0
	movsd xmm6, xmm0

rx_i_175: ;IMULH_64
	dec edi
	js rx_finish
	xor r15, 09901e05bh
	mov eax, r15d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, r12
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov r12, rax

rx_i_176: ;SUB_64
	dec edi
	js rx_finish
	xor r13, 050e8c510h
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	sub rax, r11
	mov r12, rax

rx_i_177: ;MUL_64
	dec edi
	js rx_finish
	xor r14, 05422cf8fh
	mov eax, r14d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	imul rax, r12
	mov rcx, rax
	mov eax, r12d
	xor eax, 0ad60ae9ch
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_178: ;FPDIV
	dec edi
	js rx_finish
	xor r12, 0a025c3dbh
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	divsd xmm0, xmm1
	movsd xmm6, xmm0

rx_i_179: ;SHR_64
	dec edi
	js rx_finish
	xor r13, 08f74c11h
	mov ecx, r13d
	call rx_read_dataset
	mov rcx, rbx
	shr rax, cl
	mov rbx, rax

rx_i_180: ;ADD_32
	dec edi
	js rx_finish
	xor r9, 01f2ed5f1h
	mov ecx, r9d
	call rx_read_dataset
	add eax, r14d
	mov r10, rax

rx_i_181: ;ADD_64
	dec edi
	js rx_finish
	xor r10, 0d2072c79h
	mov ecx, r10d
	call rx_read_dataset
	add rax, r10
	mov rcx, rax
	mov eax, r13d
	xor eax, 02f5713b7h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_182: ;RET
	dec edi
	js rx_finish
	xor r15, 0a8e51933h
	mov ecx, r15d
	call rx_read_dataset
	cmp rsp, rbp
	je short not_taken_ret_182
	cmp r12d, -1016679819
	ja short not_taken_ret_182
	xor rax, qword ptr [rsp + 8]
	mov rcx, rax
	mov eax, r12d
	xor eax, 0c366b275h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	ret 8
not_taken_ret_182:
	mov rcx, rax
	mov eax, r12d
	xor eax, 0c366b275h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_183: ;FPADD
	dec edi
	js rx_finish
	xor r12, 0934ad492h
	mov eax, r12d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm2
	mov eax, ebx
	xor eax, 01a997aebh
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_184: ;OR_64
	dec edi
	js rx_finish
	xor r15, 01cb3ce1fh
	mov eax, r15d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 1995308563
	mov rcx, rax
	mov eax, r9d
	xor eax, 076edfe13h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_185: ;CALL
	dec edi
	js rx_finish
	xor r10, 023c7845fh
	mov ecx, r10d
	call rx_read_dataset
	cmp r12d, -1612576918
	jbe short taken_call_185
	mov r10, rax
	jmp rx_i_186
taken_call_185:
	push rax
	call rx_i_215

rx_i_186: ;FPADD
	dec edi
	js rx_finish
	xor rbx, 0f8884327h
	mov ecx, ebx
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, 4746551310076084224
	movd xmm9, rax
	addsd xmm0, xmm9
	mov eax, r14d
	xor eax, 07c8d12a5h
	and eax, 32767
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_187: ;ADD_64
	dec edi
	js rx_finish
	xor r13, 013070461h
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	add rax, 137260710
	mov r10, rax

rx_i_188: ;SHR_64
	dec edi
	js rx_finish
	xor r12, 04764cdf7h
	mov ecx, r12d
	call rx_read_dataset
	shr rax, 40
	mov r12, rax

rx_i_189: ;CALL
	dec edi
	js rx_finish
	xor r10, 03c41026fh
	mov eax, r10d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	cmp r15d, -1510284125
	jbe short taken_call_189
	mov rcx, rax
	mov eax, r9d
	xor eax, 0a5fae4a3h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	jmp rx_i_190
taken_call_189:
	push rax
	call rx_i_250

rx_i_190: ;XOR_64
	dec edi
	js rx_finish
	xor r9, 0cded414bh
	mov ecx, r9d
	call rx_read_dataset
	xor rax, r15
	mov rcx, rax
	mov eax, r10d
	xor eax, 0b55bfba0h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_191: ;FPMUL
	dec edi
	js rx_finish
	xor r13, 05c6d64a8h
	mov ecx, r13d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm6
	mov eax, r13d
	xor eax, 020dcdd88h
	and eax, 32767
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_192: ;FPSUB
	dec edi
	js rx_finish
	xor r9, 04659becbh
	mov eax, r9d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, 4746030173290233856
	movd xmm9, rax
	subsd xmm0, xmm9
	mov eax, r12d
	xor eax, 075253031h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_193: ;FPDIV
	dec edi
	js rx_finish
	xor r11, 0c52741d5h
	mov ecx, r11d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	divsd xmm0, xmm7
	mov eax, r13d
	xor eax, 0e6f1a3b7h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_194: ;RET
	dec edi
	js rx_finish
	xor r12, 0217bf5f3h
	mov ecx, r12d
	call rx_read_dataset
	cmp rsp, rbp
	je short not_taken_ret_194
	cmp ebx, -1901851954
	ja short not_taken_ret_194
	xor rax, qword ptr [rsp + 8]
	mov r13, rax
	ret 8
not_taken_ret_194:
	mov r13, rax

rx_i_195: ;FPSQRT
	dec edi
	js rx_finish
	xor r15, 0884f3526h
	mov ecx, r15d
	call rx_read_dataset
	mov rcx, 9223372036854773760
	and rax, rcx
	cvtsi2sd xmm0, rax
	sqrtsd xmm0, xmm0
	mov eax, r14d
	xor eax, 0648f64e0h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_196: ;FPSQRT
	dec edi
	js rx_finish
	xor rbx, 0d76edad3h
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, 9223372036854773760
	and rax, rcx
	cvtsi2sd xmm0, rax
	sqrtsd xmm0, xmm0
	movsd xmm8, xmm0

rx_i_197: ;MULH_64
	dec edi
	js rx_finish
	xor r12, 0e9939ach
	mov ecx, r12d
	call rx_read_dataset
	mov r8, rdx
	mov rcx, r12
	mul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, r15d
	xor eax, 074e097dch
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_198: ;FPMUL
	dec edi
	js rx_finish
	xor r12, 0f21ca520h
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm2
	movsd xmm5, xmm0

rx_i_199: ;SHR_64
	dec edi
	js rx_finish
	xor r10, 09405152ch
	mov ecx, r10d
	call rx_read_dataset
	mov rcx, rbx
	shr rax, cl
	mov r9, rax

rx_i_200: ;ADD_64
	dec edi
	js rx_finish
	xor rbx, 0c2a9f41bh
	mov ecx, ebx
	call rx_read_dataset
	add rax, -1907903895
	mov rcx, rax
	mov eax, r13d
	xor eax, 08e47b269h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_201: ;MUL_64
	dec edi
	js rx_finish
	xor r12, 0229208efh
	mov ecx, r12d
	call rx_read_dataset
	imul rax, r15
	mov r11, rax

rx_i_202: ;MUL_64
	dec edi
	js rx_finish
	xor r14, 0c8d95bbbh
	mov eax, r14d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	imul rax, r14
	mov rcx, rax
	mov eax, ebx
	xor eax, 01149cba0h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_203: ;MUL_64
	dec edi
	js rx_finish
	xor r13, 050049e2eh
	mov ecx, r13d
	call rx_read_dataset
	imul rax, r10
	mov rcx, rax
	mov eax, r10d
	xor eax, 0d0e71e9ah
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_204: ;FPSUB
	dec edi
	js rx_finish
	xor r10, 0c63b99e8h
	mov ecx, r10d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm2
	movsd xmm4, xmm0

rx_i_205: ;FPADD
	dec edi
	js rx_finish
	xor rbx, 0cdda801dh
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm1
	movsd xmm4, xmm0

rx_i_206: ;FPADD
	dec edi
	js rx_finish
	xor r13, 0fa44b04ah
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm1
	mov eax, r13d
	xor eax, 0b44dbc71h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_207: ;FPSUB
	dec edi
	js rx_finish
	xor r10, 0d73e472ch
	mov ecx, r10d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, -4478539544748097536
	movd xmm9, rax
	subsd xmm0, xmm9
	movsd xmm7, xmm0

rx_i_208: ;SUB_32
	dec edi
	js rx_finish
	xor r9, 01af8ab1dh
	mov eax, r9d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	sub eax, r15d
	mov rcx, rax
	mov eax, ebx
	xor eax, 0eb8fc30fh
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_209: ;FPMUL
	dec edi
	js rx_finish
	xor r14, 094e997c5h
	mov eax, r14d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm8
	mov eax, r13d
	xor eax, 0a1f8d8c7h
	and eax, 32767
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_210: ;FPSUB
	dec edi
	js rx_finish
	xor r11, 0e836a177h
	mov ecx, r11d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, -4483350864878108672
	movd xmm9, rax
	subsd xmm0, xmm9
	mov eax, r12d
	xor eax, 0d01fb731h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_211: ;AND_64
	dec edi
	js rx_finish
	xor r9, 039ccdd30h
	mov ecx, r9d
	call rx_read_dataset
	and rax, r12
	mov rcx, rax
	mov eax, r9d
	xor eax, 012bbcc84h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_212: ;SUB_32
	dec edi
	js rx_finish
	xor r9, 0f4f126c5h
	mov ecx, r9d
	call rx_read_dataset
	sub eax, r12d
	mov r10, rax

rx_i_213: ;SHL_64
	dec edi
	js rx_finish
	xor rbx, 0b84811f1h
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	shl rax, 30
	mov rcx, rax
	mov eax, r12d
	xor eax, 0c36b836ah
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_214: ;MUL_32
	dec edi
	js rx_finish
	xor r12, 0c5efc90ah
	mov ecx, r12d
	call rx_read_dataset
	mov ecx, eax
	mov eax, -1027162400
	imul rax, rcx
	mov rcx, rax
	mov eax, r15d
	xor eax, 0c2c6bee0h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_215: ;ROR_64
	dec edi
	js rx_finish
	xor r12, 0ce533072h
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, r9
	ror rax, cl
	mov rcx, rax
	mov eax, r11d
	xor eax, 0212e615h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_216: ;SUB_32
	dec edi
	js rx_finish
	xor r13, 06b465fdbh
	mov ecx, r13d
	call rx_read_dataset
	sub eax, r13d
	mov rcx, rax
	mov eax, r15d
	xor eax, 067d81043h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_217: ;IMUL_32
	dec edi
	js rx_finish
	xor r13, 02dd1d503h
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	movsxd rcx, eax
	mov rax, 129993589
	imul rax, rcx
	mov r14, rax

rx_i_218: ;SHR_64
	dec edi
	js rx_finish
	xor r9, 0a159f313h
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, r14
	shr rax, cl
	mov r14, rax

rx_i_219: ;ADD_64
	dec edi
	js rx_finish
	xor r15, 08359265eh
	mov ecx, r15d
	call rx_read_dataset
	add rax, r12
	mov r10, rax

rx_i_220: ;SUB_32
	dec edi
	js rx_finish
	xor r12, 080696de3h
	mov ecx, r12d
	call rx_read_dataset
	sub eax, r13d
	mov rcx, rax
	mov eax, r15d
	xor eax, 03b609d2bh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_221: ;IMUL_32
	dec edi
	js rx_finish
	xor rbx, 040d5b526h
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	movsxd rcx, eax
	movsxd rax, r9d
	imul rax, rcx
	mov rcx, rax
	mov eax, r10d
	xor eax, 017e667h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_222: ;FPSQRT
	dec edi
	js rx_finish
	xor r11, 083c0bd93h
	mov ecx, r11d
	call rx_read_dataset
	mov rcx, 9223372036854773760
	and rax, rcx
	cvtsi2sd xmm0, rax
	sqrtsd xmm0, xmm0
	movsd xmm3, xmm0

rx_i_223: ;OR_32
	dec edi
	js rx_finish
	xor rbx, 0ca37f668h
	mov ecx, ebx
	call rx_read_dataset
	or eax, -740915304
	mov rcx, rax
	mov eax, r15d
	xor eax, 0d3d68798h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_224: ;IMUL_32
	dec edi
	js rx_finish
	xor r9, 0bb44c384h
	mov ecx, r9d
	call rx_read_dataset
	movsxd rcx, eax
	movsxd rax, r11d
	imul rax, rcx
	mov rcx, rax
	mov eax, r11d
	xor eax, 0903fd173h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_225: ;IMULH_64
	dec edi
	js rx_finish
	xor r9, 0a3deb512h
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, r15
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, r11d
	xor eax, 07feab351h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_226: ;FPMUL
	dec edi
	js rx_finish
	xor r9, 084a02d64h
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm5
	movsd xmm7, xmm0

rx_i_227: ;FPSUB
	dec edi
	js rx_finish
	xor rbx, 01e5cc085h
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm3
	movsd xmm2, xmm0

rx_i_228: ;SHL_64
	dec edi
	js rx_finish
	xor r12, 053982440h
	mov ecx, r12d
	call rx_read_dataset
	mov rcx, r14
	shl rax, cl
	mov rcx, rax
	mov eax, r11d
	xor eax, 0e500c69dh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_229: ;IMULH_64
	dec edi
	js rx_finish
	xor r13, 0c558367eh
	mov ecx, r13d
	call rx_read_dataset
	mov r8, rdx
	mov rcx, r10
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, r12d
	xor eax, 0fe304a4ah
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_230: ;CALL
	dec edi
	js rx_finish
	xor r10, 040139b65h
	mov ecx, r10d
	call rx_read_dataset
	cmp ebx, -1752488808
	jbe short taken_call_230
	mov rcx, rax
	mov eax, ebx
	xor eax, 0978b2498h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	jmp rx_i_231
taken_call_230:
	push rax
	call rx_i_332

rx_i_231: ;FPMUL
	dec edi
	js rx_finish
	xor r11, 0fa312dbdh
	mov eax, r11d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, -4479585788803153920
	movd xmm9, rax
	mulsd xmm0, xmm9
	movsd xmm3, xmm0

rx_i_232: ;FPSQRT
	dec edi
	js rx_finish
	xor r11, 0b64246c0h
	mov eax, r11d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, 9223372036854773760
	and rax, rcx
	cvtsi2sd xmm0, rax
	sqrtsd xmm0, xmm0
	mov eax, r15d
	xor eax, 0ffdff798h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_233: ;IMUL_32
	dec edi
	js rx_finish
	xor r11, 05c535836h
	mov ecx, r11d
	call rx_read_dataset
	movsxd rcx, eax
	movsxd rax, r12d
	imul rax, rcx
	mov rcx, rax
	mov eax, r13d
	xor eax, 013e8b2e0h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_234: ;FPSUB
	dec edi
	js rx_finish
	xor r15, 0f394972eh
	mov eax, r15d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, 4737156465540726784
	movd xmm9, rax
	subsd xmm0, xmm9
	movsd xmm5, xmm0

rx_i_235: ;RET
	dec edi
	js rx_finish
	xor r9, 0bb56428dh
	mov ecx, r9d
	call rx_read_dataset
	cmp rsp, rbp
	je short not_taken_ret_235
	cmp r9d, -422974038
	ja short not_taken_ret_235
	xor rax, qword ptr [rsp + 8]
	mov rcx, rax
	mov eax, r9d
	xor eax, 0e6c9edaah
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	ret 8
not_taken_ret_235:
	mov rcx, rax
	mov eax, r9d
	xor eax, 0e6c9edaah
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_236: ;FPMUL
	dec edi
	js rx_finish
	xor r15, 09ab46ab3h
	mov eax, r15d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm3
	mov eax, r15d
	xor eax, 07e732935h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_237: ;CALL
	dec edi
	js rx_finish
	xor r13, 08eb2cd76h
	mov ecx, r13d
	call rx_read_dataset
	cmp r12d, 392389867
	jbe short taken_call_237
	mov r14, rax
	jmp rx_i_238
taken_call_237:
	push rax
	call rx_i_272

rx_i_238: ;FPDIV
	dec edi
	js rx_finish
	xor r15, 0ba687578h
	mov eax, r15d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, 4743218015854657536
	movd xmm9, rax
	divsd xmm0, xmm9
	mov eax, r12d
	xor eax, 04d2e9e7dh
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_239: ;MUL_32
	dec edi
	js rx_finish
	xor r13, 0b6cb9ff2h
	mov ecx, r13d
	call rx_read_dataset
	mov ecx, eax
	mov eax, r12d
	imul rax, rcx
	mov rcx, rax
	mov eax, r15d
	xor eax, 0ca73a89h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_240: ;FPADD
	dec edi
	js rx_finish
	xor r15, 03ad196ach
	mov ecx, r15d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm4
	mov eax, r11d
	xor eax, 0b2ab82cdh
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_241: ;CALL
	dec edi
	js rx_finish
	xor r15, 0fab4600h
	mov ecx, r15d
	call rx_read_dataset
	push rax
	call rx_i_299

rx_i_242: ;ROR_64
	dec edi
	js rx_finish
	xor rbx, 0158f119fh
	mov ecx, ebx
	call rx_read_dataset
	mov rcx, r14
	ror rax, cl
	mov r15, rax

rx_i_243: ;ADD_64
	dec edi
	js rx_finish
	xor r13, 044f30b3fh
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	add rax, r10
	mov r10, rax

rx_i_244: ;IMUL_32
	dec edi
	js rx_finish
	xor r9, 0d65d29f9h
	mov ecx, r9d
	call rx_read_dataset
	movsxd rcx, eax
	mov rax, -423830277
	imul rax, rcx
	mov rbx, rax

rx_i_245: ;FPADD
	dec edi
	js rx_finish
	xor r11, 0ce5260adh
	mov ecx, r11d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm3
	movsd xmm7, xmm0

rx_i_246: ;MULH_64
	dec edi
	js rx_finish
	xor r12, 01119b0f9h
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, 319324914
	mul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, r10d
	xor eax, 0130882f2h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_247: ;OR_32
	dec edi
	js rx_finish
	xor r12, 0d6c2ce3dh
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or eax, 1198180774
	mov r14, rax

rx_i_248: ;ROR_64
	dec edi
	js rx_finish
	xor r11, 0c6a6248h
	mov eax, r11d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, r14
	ror rax, cl
	mov rcx, rax
	mov eax, r9d
	xor eax, 0b4a1fad6h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_249: ;OR_64
	dec edi
	js rx_finish
	xor r13, 084505739h
	mov ecx, r13d
	call rx_read_dataset
	or rax, -1546539637
	mov rcx, rax
	mov eax, r12d
	xor eax, 0a3d1ad8bh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_250: ;DIV_64
	dec edi
	js rx_finish
	xor r15, 027eeaa2eh
	mov eax, r15d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
mov ecx, 1
	mov edx, r9d
	test edx, edx
	cmovne ecx, edx
	xor edx, edx
	div rcx
	mov rdx, r8
	mov r12, rax

rx_i_251: ;MUL_32
	dec edi
	js rx_finish
	xor r10, 0c4de0296h
	mov ecx, r10d
	call rx_read_dataset
	mov ecx, eax
	mov eax, r14d
	imul rax, rcx
	mov rcx, rax
	mov eax, r9d
	xor eax, 03814cf80h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_252: ;MUL_32
	dec edi
	js rx_finish
	xor rbx, 0649df46fh
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov ecx, eax
	mov eax, r15d
	imul rax, rcx
	mov rcx, rax
	mov eax, r9d
	xor eax, 07b10fc32h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_253: ;IMULH_64
	dec edi
	js rx_finish
	xor r15, 0499552cch
	mov ecx, r15d
	call rx_read_dataset
	mov r8, rdx
	mov rcx, r11
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, r13d
	xor eax, 0e1afcff9h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_254: ;SUB_64
	dec edi
	js rx_finish
	xor r13, 083eafe6fh
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	sub rax, rbx
	mov rcx, rax
	mov eax, r14d
	xor eax, 031115b87h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_255: ;FPMUL
	dec edi
	js rx_finish
	xor r13, 0a25a4d8ah
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm2
	movsd xmm4, xmm0

rx_i_256: ;SHR_64
	dec edi
	js rx_finish
	xor r14, 08a75ad41h
	mov ecx, r14d
	call rx_read_dataset
	mov rcx, rbx
	shr rax, cl
	mov r14, rax

rx_i_257: ;CALL
	dec edi
	js rx_finish
	xor r14, 057f3f596h
	mov ecx, r14d
	call rx_read_dataset
	cmp r15d, 1699431947
	jbe short taken_call_257
	mov rcx, rax
	mov eax, r13d
	xor eax, 0654b460bh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	jmp rx_i_258
taken_call_257:
	push rax
	call rx_i_371

rx_i_258: ;FPADD
	dec edi
	js rx_finish
	xor r14, 04cfb709eh
	mov ecx, r14d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm4
	movsd xmm8, xmm0

rx_i_259: ;FPADD
	dec edi
	js rx_finish
	xor r9, 0b96ec9ech
	mov ecx, r9d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm5
	movsd xmm6, xmm0

rx_i_260: ;MUL_64
	dec edi
	js rx_finish
	xor rbx, 08375472ch
	mov ecx, ebx
	call rx_read_dataset
	imul rax, r15
	mov rcx, rax
	mov eax, r9d
	xor eax, 0f8942c0h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_261: ;FPADD
	dec edi
	js rx_finish
	xor r12, 0d75a8c3fh
	mov ecx, r12d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, 4741056264732147712
	movd xmm9, rax
	addsd xmm0, xmm9
	movsd xmm3, xmm0

rx_i_262: ;MUL_32
	dec edi
	js rx_finish
	xor r11, 064fdbda0h
	mov eax, r11d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	mov ecx, eax
	mov eax, r14d
	imul rax, rcx
	mov rcx, rax
	mov eax, r9d
	xor eax, 01c58ef2dh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_263: ;ROR_64
	dec edi
	js rx_finish
	xor r11, 02e36a073h
	mov eax, r11d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, r9
	ror rax, cl
	mov rcx, rax
	mov eax, r11d
	xor eax, 06c1856f0h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_264: ;FPSUB
	dec edi
	js rx_finish
	xor r13, 0f94e9fa9h
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, 4743938178866479104
	movd xmm9, rax
	subsd xmm0, xmm9
	mov eax, r9d
	xor eax, 0576a8e8fh
	and eax, 32767
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_265: ;FPDIV
	dec edi
	js rx_finish
	xor r14, 02346171ch
	mov ecx, r14d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	divsd xmm0, xmm3
	movsd xmm3, xmm0

rx_i_266: ;OR_64
	dec edi
	js rx_finish
	xor r10, 01c42baa6h
	mov eax, r10d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, r13
	mov rcx, rax
	mov eax, r11d
	xor eax, 0a271ff06h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_267: ;FPMUL
	dec edi
	js rx_finish
	xor r11, 0b39b140h
	mov ecx, r11d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm8
	mov eax, r14d
	xor eax, 0d8823dc5h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_268: ;FPSUB
	dec edi
	js rx_finish
	xor r11, 01a07d201h
	mov eax, r11d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm3
	mov eax, r15d
	xor eax, 0df89f274h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_269: ;FPADD
	dec edi
	js rx_finish
	xor r13, 07a3eb340h
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm8
	movsd xmm2, xmm0

rx_i_270: ;CALL
	dec edi
	js rx_finish
	xor r13, 03d0a3a89h
	mov eax, r13d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	cmp r12d, 136160027
	jbe short taken_call_270
	mov r10, rax
	jmp rx_i_271
taken_call_270:
	push rax
	call rx_i_299

rx_i_271: ;ROL_64
	dec edi
	js rx_finish
	xor rbx, 0c6c7b37h
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, r10
	rol rax, cl
	mov r11, rax

rx_i_272: ;FPROUND
	dec edi
	js rx_finish
	xor r12, 0c2510cebh
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, rax
	shl eax, 13
	and rcx, -2048
	and eax, 24576
	cvtsi2sd xmm0, rcx
	or eax, 40896
	mov dword ptr [rsp - 8], eax
	ldmxcsr dword ptr [rsp - 8]
	movsd xmm5, xmm0

rx_i_273: ;ROL_64
	dec edi
	js rx_finish
	xor r11, 0c80cc899h
	mov ecx, r11d
	call rx_read_dataset
	mov rcx, rbx
	rol rax, cl
	mov rcx, rax
	mov eax, r10d
	xor eax, 01ba81447h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_274: ;FPMUL
	dec edi
	js rx_finish
	xor r11, 0eb355caah
	mov ecx, r11d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm1
	mov eax, r15d
	xor eax, 03981662bh
	and eax, 32767
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_275: ;MUL_32
	dec edi
	js rx_finish
	xor r13, 0c6f12299h
	mov ecx, r13d
	call rx_read_dataset
	mov ecx, eax
	mov eax, -2032281772
	imul rax, rcx
	mov rcx, rax
	mov eax, r9d
	xor eax, 086ddd754h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_276: ;OR_64
	dec edi
	js rx_finish
	xor r12, 0695a5dd2h
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, r12
	mov r13, rax

rx_i_277: ;CALL
	dec edi
	js rx_finish
	xor r9, 0d315e4dch
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	push rax
	call rx_i_376

rx_i_278: ;FPADD
	dec edi
	js rx_finish
	xor r15, 0b66ca7e0h
	mov ecx, r15d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, 4745257761179172864
	movd xmm9, rax
	addsd xmm0, xmm9
	movsd xmm6, xmm0

rx_i_279: ;AND_64
	dec edi
	js rx_finish
	xor r10, 0788eceb7h
	mov ecx, r10d
	call rx_read_dataset
	and rax, r11
	mov r13, rax

rx_i_280: ;FPROUND
	dec edi
	js rx_finish
	xor r9, 0c6ac5edah
	mov ecx, r9d
	call rx_read_dataset
	mov rcx, rax
	shl eax, 13
	and rcx, -2048
	and eax, 24576
	cvtsi2sd xmm0, rcx
	or eax, 40896
	mov dword ptr [rsp - 8], eax
	ldmxcsr dword ptr [rsp - 8]
	mov eax, r12d
	xor eax, 0b65161a6h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_281: ;IMUL_32
	dec edi
	js rx_finish
	xor r11, 0c9549789h
	mov ecx, r11d
	call rx_read_dataset
	movsxd rcx, eax
	movsxd rax, r10d
	imul rax, rcx
	mov rcx, rax
	mov eax, r9d
	xor eax, 01aca20a3h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_282: ;FPSUB
	dec edi
	js rx_finish
	xor r9, 0a2bc66c9h
	mov ecx, r9d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm7
	movsd xmm4, xmm0

rx_i_283: ;FPADD
	dec edi
	js rx_finish
	xor r15, 0f1a91458h
	mov ecx, r15d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, 4742807966216880128
	movd xmm9, rax
	addsd xmm0, xmm9
	movsd xmm1, xmm0

rx_i_284: ;IMULH_64
	dec edi
	js rx_finish
	xor r12, 066246b43h
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, r11
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, r13d
	xor eax, 0211aeb00h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_285: ;ADD_32
	dec edi
	js rx_finish
	xor r10, 05a762727h
	mov ecx, r10d
	call rx_read_dataset
	add eax, r10d
	mov rcx, rax
	mov eax, r11d
	xor eax, 0f3e6c946h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_286: ;SUB_64
	dec edi
	js rx_finish
	xor r15, 0de1ab603h
	mov ecx, r15d
	call rx_read_dataset
	sub rax, 1367326224
	mov r11, rax

rx_i_287: ;ADD_64
	dec edi
	js rx_finish
	xor r9, 0df4d084fh
	mov eax, r9d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	add rax, -1156732976
	mov rcx, rax
	mov eax, r12d
	xor eax, 0bb0da7d0h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_288: ;FPADD
	dec edi
	js rx_finish
	xor r15, 0e68f36ach
	mov ecx, r15d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, -4477945591619387392
	movd xmm9, rax
	addsd xmm0, xmm9
	movsd xmm1, xmm0

rx_i_289: ;IMUL_32
	dec edi
	js rx_finish
	xor rbx, 09adb333bh
	mov ecx, ebx
	call rx_read_dataset
	movsxd rcx, eax
	movsxd rax, ebx
	imul rax, rcx
	mov r14, rax

rx_i_290: ;ROR_64
	dec edi
	js rx_finish
	xor r14, 082f5e36ch
	mov eax, r14d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, r9
	ror rax, cl
	mov rcx, rax
	mov eax, r15d
	xor eax, 0546e75d1h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_291: ;AND_64
	dec edi
	js rx_finish
	xor r11, 049547c9ch
	mov eax, r11d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, r15
	mov rcx, rax
	mov eax, ebx
	xor eax, 04926c7fah
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_292: ;SUB_32
	dec edi
	js rx_finish
	xor r10, 08716ac8bh
	mov eax, r10d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	sub eax, ebx
	mov rcx, rax
	mov eax, r9d
	xor eax, 062eafa1bh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_293: ;FPMUL
	dec edi
	js rx_finish
	xor r14, 0efef52b5h
	mov eax, r14d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm1
	mov eax, ebx
	xor eax, 046affb49h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_294: ;FPSUB
	dec edi
	js rx_finish
	xor r15, 060665748h
	mov ecx, r15d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm8
	mov eax, r9d
	xor eax, 02f4d18d7h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_295: ;RET
	dec edi
	js rx_finish
	xor r13, 0ddf4bd1ah
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	cmp rsp, rbp
	je short not_taken_ret_295
	cmp r14d, 1988795765
	ja short not_taken_ret_295
	xor rax, qword ptr [rsp + 8]
	mov rcx, rax
	mov eax, r14d
	xor eax, 0768a9d75h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx
	ret 8
not_taken_ret_295:
	mov rcx, rax
	mov eax, r14d
	xor eax, 0768a9d75h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_296: ;ROL_64
	dec edi
	js rx_finish
	xor r13, 05a87cc3dh
	mov eax, r13d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	rol rax, 23
	mov r10, rax

rx_i_297: ;FPADD
	dec edi
	js rx_finish
	xor r9, 0c61f4279h
	mov ecx, r9d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm5
	mov eax, ebx
	xor eax, 014844990h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_298: ;RET
	dec edi
	js rx_finish
	xor r14, 0f3b9d85h
	mov ecx, r14d
	call rx_read_dataset
	cmp rsp, rbp
	je short not_taken_ret_298
	cmp r15d, -276467273
	ja short not_taken_ret_298
	xor rax, qword ptr [rsp + 8]
	mov rcx, rax
	mov eax, ebx
	xor eax, 0ef8571b7h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	ret 8
not_taken_ret_298:
	mov rcx, rax
	mov eax, ebx
	xor eax, 0ef8571b7h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_299: ;FPSUB
	dec edi
	js rx_finish
	xor r9, 0f42798fdh
	mov ecx, r9d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm8
	mov eax, r15d
	xor eax, 08a66e69fh
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_300: ;FPDIV
	dec edi
	js rx_finish
	xor r14, 018738758h
	mov ecx, r14d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	divsd xmm0, xmm1
	mov eax, ebx
	xor eax, 0f3a594cah
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_301: ;ADD_64
	dec edi
	js rx_finish
	xor r15, 0de3b9d9bh
	mov eax, r15d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	add rax, r10
	mov r14, rax

rx_i_302: ;FPADD
	dec edi
	js rx_finish
	xor r14, 084f53637h
	mov eax, r14d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm7
	mov eax, r14d
	xor eax, 0d10f7c42h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_303: ;ADD_64
	dec edi
	js rx_finish
	xor r12, 042f4897h
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	add rax, 21400308
	mov rcx, rax
	mov eax, r12d
	xor eax, 01468af4h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_304: ;FPSUB
	dec edi
	js rx_finish
	xor r12, 095765693h
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, -4478628807791607808
	movd xmm9, rax
	subsd xmm0, xmm9
	mov eax, r10d
	xor eax, 09d24b005h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_305: ;FPMUL
	dec edi
	js rx_finish
	xor rbx, 0a0ec5eech
	mov ecx, ebx
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm5
	movsd xmm7, xmm0

rx_i_306: ;ADD_64
	dec edi
	js rx_finish
	xor r15, 0f6f8c345h
	mov ecx, r15d
	call rx_read_dataset
	add rax, r10
	mov r11, rax

rx_i_307: ;FPADD
	dec edi
	js rx_finish
	xor r14, 082a3e965h
	mov eax, r14d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm3
	movsd xmm1, xmm0

rx_i_308: ;MUL_64
	dec edi
	js rx_finish
	xor r12, 04940c652h
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	imul rax, r15
	mov r13, rax

rx_i_309: ;SUB_64
	dec edi
	js rx_finish
	xor r11, 03c6c62b8h
	mov ecx, r11d
	call rx_read_dataset
	sub rax, -65873120
	mov r10, rax

rx_i_310: ;ADD_64
	dec edi
	js rx_finish
	xor r15, 08b34cdfch
	mov ecx, r15d
	call rx_read_dataset
	add rax, r15
	mov r13, rax

rx_i_311: ;SHR_64
	dec edi
	js rx_finish
	xor r15, 04c36adb1h
	mov ecx, r15d
	call rx_read_dataset
	mov rcx, rbx
	shr rax, cl
	mov r10, rax

rx_i_312: ;MUL_64
	dec edi
	js rx_finish
	xor r11, 0a4213b21h
	mov eax, r11d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	imul rax, r13
	mov r15, rax

rx_i_313: ;IMULH_64
	dec edi
	js rx_finish
	xor r9, 090c42304h
	mov eax, r9d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, -1652850028
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, r9d
	xor eax, 09d7b8294h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_314: ;FPMUL
	dec edi
	js rx_finish
	xor r9, 0f78e1c8ch
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, 4746554338141274112
	movd xmm9, rax
	mulsd xmm0, xmm9
	movsd xmm7, xmm0

rx_i_315: ;FPSUB
	dec edi
	js rx_finish
	xor rbx, 0ff8848cfh
	mov ecx, ebx
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, 4744327281034395648
	movd xmm9, rax
	subsd xmm0, xmm9
	mov eax, r12d
	xor eax, 05cf21a31h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_316: ;MULH_64
	dec edi
	js rx_finish
	xor r13, 0b18904cdh
	mov ecx, r13d
	call rx_read_dataset
	mov r8, rdx
	mov rcx, -1147928648
	mul rcx
	mov rax, rdx
	mov rdx, r8
	mov r10, rax

rx_i_317: ;ROR_64
	dec edi
	js rx_finish
	xor rbx, 0a0d0befh
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	ror rax, 62
	mov rcx, rax
	mov eax, r14d
	xor eax, 09500d514h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_318: ;IMUL_32
	dec edi
	js rx_finish
	xor r15, 01e3c65f7h
	mov ecx, r15d
	call rx_read_dataset
	movsxd rcx, eax
	movsxd rax, r9d
	imul rax, rcx
	mov rcx, rax
	mov eax, r9d
	xor eax, 07fc7f955h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_319: ;XOR_32
	dec edi
	js rx_finish
	xor r9, 02e36ddafh
	mov eax, r9d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	xor eax, r15d
	mov r9, rax

rx_i_320: ;RET
	dec edi
	js rx_finish
	xor r14, 05b0cb5bbh
	mov ecx, r14d
	call rx_read_dataset
	cmp rsp, rbp
	je short not_taken_ret_320
	xor rax, qword ptr [rsp + 8]
	mov rcx, rax
	mov eax, ebx
	xor eax, 03602c513h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	ret 8
not_taken_ret_320:
	mov rcx, rax
	mov eax, ebx
	xor eax, 03602c513h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_321: ;FPADD
	dec edi
	js rx_finish
	xor r9, 0c74e7415h
	mov ecx, r9d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm7
	mov eax, r13d
	xor eax, 0b5bc8h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_322: ;ROR_64
	dec edi
	js rx_finish
	xor r9, 057621d9ah
	mov ecx, r9d
	call rx_read_dataset
	mov rcx, r11
	ror rax, cl
	mov rcx, rax
	mov eax, r15d
	xor eax, 061cb9db8h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_323: ;ROL_64
	dec edi
	js rx_finish
	xor r13, 08ee02d99h
	mov ecx, r13d
	call rx_read_dataset
	mov rcx, r15
	rol rax, cl
	mov rcx, rax
	mov eax, r11d
	xor eax, 01f931a08h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_324: ;FPADD
	dec edi
	js rx_finish
	xor r15, 013461188h
	mov ecx, r15d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm4
	movsd xmm2, xmm0

rx_i_325: ;IMUL_32
	dec edi
	js rx_finish
	xor r11, 0a7bae383h
	mov eax, r11d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	movsxd rcx, eax
	movsxd rax, r9d
	imul rax, rcx
	mov rcx, rax
	mov eax, r12d
	xor eax, 0f213dach
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_326: ;CALL
	dec edi
	js rx_finish
	xor r14, 08215399bh
	mov ecx, r14d
	call rx_read_dataset
	cmp r11d, 1411981860
	jbe short taken_call_326
	mov rcx, rax
	mov eax, r11d
	xor eax, 054292224h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	jmp rx_i_327
taken_call_326:
	push rax
	call rx_i_347

rx_i_327: ;MUL_64
	dec edi
	js rx_finish
	xor r14, 07b07664bh
	mov eax, r14d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	imul rax, rax, -696924877
	mov rcx, rax
	mov eax, r14d
	xor eax, 0d675c533h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_328: ;FPMUL
	dec edi
	js rx_finish
	xor r9, 0f956baffh
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm2
	movsd xmm1, xmm0

rx_i_329: ;XOR_64
	dec edi
	js rx_finish
	xor r11, 0708ab9d1h
	mov eax, r11d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	xor rax, -281580460
	mov r13, rax

rx_i_330: ;MUL_64
	dec edi
	js rx_finish
	xor r11, 0d1b27540h
	mov ecx, r11d
	call rx_read_dataset
	imul rax, rbx
	mov rcx, rax
	mov eax, r9d
	xor eax, 0b67623c3h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_331: ;IMULH_64
	dec edi
	js rx_finish
	xor r9, 09665f98dh
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, r15
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov r12, rax

rx_i_332: ;SAR_64
	dec edi
	js rx_finish
	xor r12, 0fb9c32adh
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, r13
	sar rax, cl
	mov r9, rax

rx_i_333: ;RET
	dec edi
	js rx_finish
	xor r11, 0e1110623h
	mov ecx, r11d
	call rx_read_dataset
	cmp rsp, rbp
	je short not_taken_ret_333
	xor rax, qword ptr [rsp + 8]
	mov r11, rax
	ret 8
not_taken_ret_333:
	mov r11, rax

rx_i_334: ;MUL_32
	dec edi
	js rx_finish
	xor r9, 0f6a93f19h
	mov eax, r9d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	mov ecx, eax
	mov eax, r13d
	imul rax, rcx
	mov rcx, rax
	mov eax, r11d
	xor eax, 0af8b7117h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_335: ;FPADD
	dec edi
	js rx_finish
	xor r9, 0bc9bbe4ah
	mov eax, r9d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm3
	mov eax, r9d
	xor eax, 0ba4d4c0fh
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_336: ;ROR_64
	dec edi
	js rx_finish
	xor r12, 0f253cd4eh
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, r14
	ror rax, cl
	mov r11, rax

rx_i_337: ;OR_32
	dec edi
	js rx_finish
	xor r14, 0f009758bh
	mov eax, r14d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or eax, -175125848
	mov r11, rax

rx_i_338: ;ADD_64
	dec edi
	js rx_finish
	xor rbx, 0dda04168h
	mov ecx, ebx
	call rx_read_dataset
	add rax, r13
	mov rbx, rax

rx_i_339: ;ADD_32
	dec edi
	js rx_finish
	xor r15, 03e6cfb73h
	mov eax, r15d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	add eax, ebx
	mov rcx, rax
	mov eax, r12d
	xor eax, 07ffe4218h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_340: ;ROR_64
	dec edi
	js rx_finish
	xor r15, 0aea0a435h
	mov eax, r15d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, r10
	ror rax, cl
	mov rcx, rax
	mov eax, r11d
	xor eax, 02644c5ah
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_341: ;ADD_64
	dec edi
	js rx_finish
	xor rbx, 03d6c4ab2h
	mov ecx, ebx
	call rx_read_dataset
	add rax, r12
	mov rcx, rax
	mov eax, r13d
	xor eax, 0dab07c39h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_342: ;MUL_64
	dec edi
	js rx_finish
	xor r12, 0d428a742h
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	imul rax, r12
	mov r11, rax

rx_i_343: ;FPADD
	dec edi
	js rx_finish
	xor r9, 04596ef73h
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm6
	mov eax, r10d
	xor eax, 07c8317fah
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_344: ;FPADD
	dec edi
	js rx_finish
	xor r15, 0e51629cch
	mov ecx, r15d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm5
	mov eax, r13d
	xor eax, 038b653beh
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_345: ;MULH_64
	dec edi
	js rx_finish
	xor r12, 019eb9ea5h
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, r15
	mul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, ebx
	xor eax, 024736405h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_346: ;FPSUB
	dec edi
	js rx_finish
	xor r9, 09ccc7abah
	mov ecx, r9d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm2
	mov eax, r11d
	xor eax, 0319de2d3h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_347: ;SHL_64
	dec edi
	js rx_finish
	xor r14, 056f6cf0bh
	mov eax, r14d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, r13
	shl rax, cl
	mov rcx, rax
	mov eax, r15d
	xor eax, 0d9a469a9h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_348: ;FPSUB
	dec edi
	js rx_finish
	xor r10, 03ef9bcc4h
	mov eax, r10d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, 4744717476367958016
	movd xmm9, rax
	subsd xmm0, xmm9
	mov eax, r13d
	xor eax, 0627d9feah
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_349: ;MUL_64
	dec edi
	js rx_finish
	xor r12, 0bbbcdbach
	mov ecx, r12d
	call rx_read_dataset
	imul rax, r13
	mov rcx, rax
	mov eax, r9d
	xor eax, 0ef03b0ddh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_350: ;OR_64
	dec edi
	js rx_finish
	xor r12, 0ae9d1e96h
	mov ecx, r12d
	call rx_read_dataset
	or rax, r15
	mov rcx, rax
	mov eax, r13d
	xor eax, 0ed2d3987h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_351: ;ADD_64
	dec edi
	js rx_finish
	xor r14, 070c34d69h
	mov eax, r14d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	add rax, r10
	mov r13, rax

rx_i_352: ;FPSUB
	dec edi
	js rx_finish
	xor r13, 0523ff904h
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, 4741412628788674560
	movd xmm9, rax
	subsd xmm0, xmm9
	movsd xmm1, xmm0

rx_i_353: ;XOR_64
	dec edi
	js rx_finish
	xor rbx, 018e0e5ddh
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	xor rax, r15
	mov r13, rax

rx_i_354: ;CALL
	dec edi
	js rx_finish
	xor r9, 09bd050f0h
	mov eax, r9d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	cmp r9d, -980411581
	jbe short taken_call_354
	mov rcx, rax
	mov eax, r12d
	xor eax, 0c5901b43h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	jmp rx_i_355
taken_call_354:
	push rax
	call rx_i_356

rx_i_355: ;MUL_64
	dec edi
	js rx_finish
	xor r11, 0a3a5906fh
	mov ecx, r11d
	call rx_read_dataset
	imul rax, r10
	mov r13, rax

rx_i_356: ;FPADD
	dec edi
	js rx_finish
	xor r10, 0afc9af2bh
	mov ecx, r10d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm6
	movsd xmm2, xmm0

rx_i_357: ;FPSUB
	dec edi
	js rx_finish
	xor r13, 02e65278bh
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm2
	movsd xmm7, xmm0

rx_i_358: ;MULH_64
	dec edi
	js rx_finish
	xor r13, 02412fc10h
	mov ecx, r13d
	call rx_read_dataset
	mov r8, rdx
	mov rcx, r13
	mul rcx
	mov rax, rdx
	mov rdx, r8
	mov r13, rax

rx_i_359: ;SUB_32
	dec edi
	js rx_finish
	xor r10, 06bd6e65fh
	mov ecx, r10d
	call rx_read_dataset
	sub eax, r14d
	mov rcx, rax
	mov eax, ebx
	xor eax, 0c1062b3ch
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_360: ;SUB_64
	dec edi
	js rx_finish
	xor r10, 01cd85d80h
	mov ecx, r10d
	call rx_read_dataset
	sub rax, r10
	mov r11, rax

rx_i_361: ;ADD_64
	dec edi
	js rx_finish
	xor r10, 0f7daed36h
	mov eax, r10d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	add rax, 820073637
	mov r11, rax

rx_i_362: ;IMULH_64
	dec edi
	js rx_finish
	xor r13, 088fa6e5ah
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, r11
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov r9, rax

rx_i_363: ;FPSUB
	dec edi
	js rx_finish
	xor r10, 0714fc2cdh
	mov ecx, r10d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm1
	movsd xmm4, xmm0

rx_i_364: ;FPMUL
	dec edi
	js rx_finish
	xor r10, 0c2d110b5h
	mov eax, r10d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm8
	mov eax, ebx
	xor eax, 0c41a4103h
	and eax, 32767
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_365: ;FPDIV
	dec edi
	js rx_finish
	xor r15, 01d125a7fh
	mov ecx, r15d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	divsd xmm0, xmm6
	movsd xmm6, xmm0

rx_i_366: ;ADD_32
	dec edi
	js rx_finish
	xor r9, 0ed8954bdh
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	add eax, 1082179469
	mov rcx, rax
	mov eax, r15d
	xor eax, 04080bf8dh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_367: ;FPSUB
	dec edi
	js rx_finish
	xor r12, 09f75887bh
	mov eax, r12d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm6
	mov eax, r11d
	xor eax, 05415334dh
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_368: ;MULH_64
	dec edi
	js rx_finish
	xor r11, 0badaf867h
	mov ecx, r11d
	call rx_read_dataset
	mov r8, rdx
	mov rcx, rbx
	mul rcx
	mov rax, rdx
	mov rdx, r8
	mov rbx, rax

rx_i_369: ;MUL_32
	dec edi
	js rx_finish
	xor r15, 02db4444ah
	mov eax, r15d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov ecx, eax
	mov eax, r9d
	imul rax, rcx
	mov rcx, rax
	mov eax, r12d
	xor eax, 0bfd87d37h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_370: ;IMUL_32
	dec edi
	js rx_finish
	xor r12, 0bff7218fh
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	movsxd rcx, eax
	movsxd rax, ebx
	imul rax, rcx
	mov rcx, rax
	mov eax, r15d
	xor eax, 0c3d6bcb7h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_371: ;ROR_64
	dec edi
	js rx_finish
	xor r9, 04d14cb3ah
	mov ecx, r9d
	call rx_read_dataset
	mov rcx, r9
	ror rax, cl
	mov r12, rax

rx_i_372: ;SUB_64
	dec edi
	js rx_finish
	xor r10, 0a14836bah
	mov ecx, r10d
	call rx_read_dataset
	sub rax, r10
	mov rbx, rax

rx_i_373: ;IMULH_64
	dec edi
	js rx_finish
	xor r9, 053fe22e2h
	mov eax, r9d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, r13
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov r9, rax

rx_i_374: ;FPADD
	dec edi
	js rx_finish
	xor r15, 010e1fb24h
	mov eax, r15d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm6
	movsd xmm6, xmm0

rx_i_375: ;FPADD
	dec edi
	js rx_finish
	xor rbx, 0ebbd5cc9h
	mov ecx, ebx
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm1
	movsd xmm5, xmm0

rx_i_376: ;SAR_64
	dec edi
	js rx_finish
	xor r10, 098ab79d7h
	mov eax, r10d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, r13
	sar rax, cl
	mov r9, rax

rx_i_377: ;FPMUL
	dec edi
	js rx_finish
	xor r15, 056438b3h
	mov eax, r15d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm8
	mov eax, r12d
	xor eax, 05655fac9h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_378: ;FPMUL
	dec edi
	js rx_finish
	xor r11, 0dbcce604h
	mov ecx, r11d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm2
	mov eax, r10d
	xor eax, 03507e810h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_379: ;ADD_64
	dec edi
	js rx_finish
	xor r9, 0edea6200h
	mov ecx, r9d
	call rx_read_dataset
	add rax, r15
	mov rcx, rax
	mov eax, r12d
	xor eax, 0ec359be9h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_380: ;ADD_64
	dec edi
	js rx_finish
	xor r14, 05e61b279h
	mov eax, r14d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	add rax, 476136066
	mov rcx, rax
	mov eax, ebx
	xor eax, 01c614282h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_381: ;FPSUB
	dec edi
	js rx_finish
	xor r14, 0fc1fb433h
	mov ecx, r14d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm3
	mov eax, r15d
	xor eax, 0d822f28fh
	and eax, 32767
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_382: ;MULH_64
	dec edi
	js rx_finish
	xor r12, 082aa21ach
	mov ecx, r12d
	call rx_read_dataset
	mov r8, rdx
	mov rcx, 547725353
	mul rcx
	mov rax, rdx
	mov rdx, r8
	mov r15, rax

rx_i_383: ;ROR_64
	dec edi
	js rx_finish
	xor r10, 05dba41fbh
	mov ecx, r10d
	call rx_read_dataset
	ror rax, 56
	mov r13, rax

rx_i_384: ;SUB_64
	dec edi
	js rx_finish
	xor r11, 0229e3d6eh
	mov eax, r11d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	sub rax, -1443002912
	mov rcx, rax
	mov eax, r13d
	xor eax, 0a9fd85e0h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_385: ;SHL_64
	dec edi
	js rx_finish
	xor rbx, 019816ff9h
	mov ecx, ebx
	call rx_read_dataset
	mov rcx, r14
	shl rax, cl
	mov r9, rax

rx_i_386: ;ROR_64
	dec edi
	js rx_finish
	xor r14, 036b5b81fh
	mov ecx, r14d
	call rx_read_dataset
	mov rcx, r11
	ror rax, cl
	mov r11, rax

rx_i_387: ;FPSUB
	dec edi
	js rx_finish
	xor r15, 05f798ec3h
	mov eax, r15d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm4
	movsd xmm5, xmm0

rx_i_388: ;SHL_64
	dec edi
	js rx_finish
	xor r10, 05b459fd7h
	mov ecx, r10d
	call rx_read_dataset
	mov rcx, r11
	shl rax, cl
	mov rcx, rax
	mov eax, r9d
	xor eax, 054439464h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_389: ;MUL_64
	dec edi
	js rx_finish
	xor r15, 0c91749bbh
	mov ecx, r15d
	call rx_read_dataset
	imul rax, r12
	mov rcx, rax
	mov eax, r13d
	xor eax, 0fb9b50b9h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_390: ;FPADD
	dec edi
	js rx_finish
	xor r9, 0575b4bdch
	mov ecx, r9d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm8
	mov eax, r9d
	xor eax, 05702d58dh
	and eax, 32767
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_391: ;SUB_64
	dec edi
	js rx_finish
	xor r9, 0d4f7bc6ah
	mov ecx, r9d
	call rx_read_dataset
	sub rax, r15
	mov r9, rax

rx_i_392: ;RET
	dec edi
	js rx_finish
	xor rbx, 08a949356h
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	cmp rsp, rbp
	je short not_taken_ret_392
	cmp r13d, -1600627518
	ja short not_taken_ret_392
	xor rax, qword ptr [rsp + 8]
	mov rcx, rax
	mov eax, r11d
	xor eax, 0a0985cc2h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	ret 8
not_taken_ret_392:
	mov rcx, rax
	mov eax, r11d
	xor eax, 0a0985cc2h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_393: ;FPROUND
	dec edi
	js rx_finish
	xor r11, 06531ad2eh
	mov eax, r11d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, rax
	shl eax, 13
	and rcx, -2048
	and eax, 24576
	cvtsi2sd xmm0, rcx
	or eax, 40896
	mov dword ptr [rsp - 8], eax
	ldmxcsr dword ptr [rsp - 8]
	movsd xmm6, xmm0

rx_i_394: ;ROR_64
	dec edi
	js rx_finish
	xor r15, 02914abeah
	mov eax, r15d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, r12
	ror rax, cl
	mov rcx, rax
	mov eax, r11d
	xor eax, 0e5c5acbbh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_395: ;ROR_64
	dec edi
	js rx_finish
	xor rbx, 0473a41f0h
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, r11
	ror rax, cl
	mov rcx, rax
	mov eax, r14d
	xor eax, 0aba2155fh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_396: ;ROL_64
	dec edi
	js rx_finish
	xor r14, 01ebc1f0dh
	mov eax, r14d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	rol rax, 0
	mov rcx, rax
	mov eax, r13d
	xor eax, 08c4a0f0dh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_397: ;OR_64
	dec edi
	js rx_finish
	xor r14, 0742e95b1h
	mov eax, r14d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 552339548
	mov rcx, rax
	mov eax, r13d
	xor eax, 020ec085ch
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_398: ;FPADD
	dec edi
	js rx_finish
	xor r12, 0db885c2ch
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm1
	mov eax, r14d
	xor eax, 0910e8628h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_399: ;IMULH_64
	dec edi
	js rx_finish
	xor rbx, 04ae4fe8ch
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, r13
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov rbx, rax

rx_i_400: ;ROR_64
	dec edi
	js rx_finish
	xor r10, 07b41862bh
	mov ecx, r10d
	call rx_read_dataset
	ror rax, 62
	mov rcx, rax
	mov eax, r12d
	xor eax, 01ee1c837h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_401: ;SUB_64
	dec edi
	js rx_finish
	xor rbx, 0916f3819h
	mov ecx, ebx
	call rx_read_dataset
	sub rax, r12
	mov rcx, rax
	mov eax, r10d
	xor eax, 0146db5dfh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_402: ;SAR_64
	dec edi
	js rx_finish
	xor rbx, 04eb6fd2ah
	mov eax, ebx
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, rbx
	sar rax, cl
	mov rcx, rax
	mov eax, r11d
	xor eax, 0724e7136h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_403: ;FPMUL
	dec edi
	js rx_finish
	xor r11, 0899a98cfh
	mov ecx, r11d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm2
	mov eax, r14d
	xor eax, 0fb6f7016h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_404: ;OR_64
	dec edi
	js rx_finish
	xor r13, 0aae75db6h
	mov eax, r13d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	or rax, r11
	mov rcx, rax
	mov eax, r14d
	xor eax, 094ac538ch
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_405: ;FPSUB
	dec edi
	js rx_finish
	xor r13, 032e81f25h
	mov eax, r13d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, 4742100199122403328
	movd xmm9, rax
	subsd xmm0, xmm9
	movsd xmm6, xmm0

rx_i_406: ;RET
	dec edi
	js rx_finish
	xor r9, 0fa1a07ffh
	mov ecx, r9d
	call rx_read_dataset
	cmp rsp, rbp
	je short not_taken_ret_406
	xor rax, qword ptr [rsp + 8]
	mov r14, rax
	ret 8
not_taken_ret_406:
	mov r14, rax

rx_i_407: ;IMULH_64
	dec edi
	js rx_finish
	xor r9, 0e59500f7h
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, r12
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, r11d
	xor eax, 01ff394a0h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_408: ;MULH_64
	dec edi
	js rx_finish
	xor r15, 05b8ceb2fh
	mov ecx, r15d
	call rx_read_dataset
	mov r8, rdx
	mov rcx, rbx
	mul rcx
	mov rax, rdx
	mov rdx, r8
	mov r15, rax

rx_i_409: ;CALL
	dec edi
	js rx_finish
	xor rbx, 0f61082a3h
	mov ecx, ebx
	call rx_read_dataset
	cmp r10d, 1795880641
	jbe short taken_call_409
	mov rcx, rax
	mov eax, r12d
	xor eax, 06b0af6c1h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	jmp rx_i_410
taken_call_409:
	push rax
	call rx_i_498

rx_i_410: ;FPDIV
	dec edi
	js rx_finish
	xor r9, 0af6886b7h
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, -4478294004261191680
	movd xmm9, rax
	divsd xmm0, xmm9
	movsd xmm1, xmm0

rx_i_411: ;FPSUB
	dec edi
	js rx_finish
	xor r14, 09699566fh
	mov ecx, r14d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm1
	mov eax, ebx
	xor eax, 0904eec66h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_412: ;SUB_64
	dec edi
	js rx_finish
	xor r15, 066e79fa6h
	mov ecx, r15d
	call rx_read_dataset
	sub rax, r9
	mov rcx, rax
	mov eax, r10d
	xor eax, 0295004c9h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_413: ;MUL_64
	dec edi
	js rx_finish
	xor r11, 04b6caa9ah
	mov ecx, r11d
	call rx_read_dataset
	imul rax, r15
	mov rbx, rax

rx_i_414: ;RET
	dec edi
	js rx_finish
	xor r15, 0d17f245eh
	mov eax, r15d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	cmp rsp, rbp
	je short not_taken_ret_414
	xor rax, qword ptr [rsp + 8]
	mov rbx, rax
	ret 8
not_taken_ret_414:
	mov rbx, rax

rx_i_415: ;RET
	dec edi
	js rx_finish
	xor r12, 0364f10e7h
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	cmp rsp, rbp
	je short not_taken_ret_415
	cmp r13d, -1265436998
	ja short not_taken_ret_415
	xor rax, qword ptr [rsp + 8]
	mov r12, rax
	ret 8
not_taken_ret_415:
	mov r12, rax

rx_i_416: ;FPDIV
	dec edi
	js rx_finish
	xor r10, 0ac90e7ah
	mov eax, r10d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	divsd xmm0, xmm4
	movsd xmm3, xmm0

rx_i_417: ;FPMUL
	dec edi
	js rx_finish
	xor r11, 04b6037abh
	mov eax, r11d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm2
	mov eax, r12d
	xor eax, 043989376h
	and eax, 32767
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_418: ;AND_32
	dec edi
	js rx_finish
	xor r14, 06c01554dh
	mov ecx, r14d
	call rx_read_dataset
	and eax, ebx
	mov rcx, rax
	mov eax, r10d
	xor eax, 0e973b3b1h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_419: ;IMULH_64
	dec edi
	js rx_finish
	xor rbx, 08c3e59a1h
	mov ecx, ebx
	call rx_read_dataset
	mov r8, rdx
	mov rcx, -538093385
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov r9, rax

rx_i_420: ;FPADD
	dec edi
	js rx_finish
	xor r12, 0f3fafde9h
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm3
	movsd xmm5, xmm0

rx_i_421: ;ADD_32
	dec edi
	js rx_finish
	xor r10, 03c6481fah
	mov ecx, r10d
	call rx_read_dataset
	add eax, r12d
	mov r10, rax

rx_i_422: ;MUL_64
	dec edi
	js rx_finish
	xor r10, 02bd61c5fh
	mov ecx, r10d
	call rx_read_dataset
	imul rax, r11
	mov r10, rax

rx_i_423: ;OR_32
	dec edi
	js rx_finish
	xor r9, 0b6ab9d32h
	mov ecx, r9d
	call rx_read_dataset
	or eax, r14d
	mov rcx, rax
	mov eax, r14d
	xor eax, 0beeca8dbh
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_424: ;ROR_64
	dec edi
	js rx_finish
	xor r9, 0f9690ceah
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, r11
	ror rax, cl
	mov r9, rax

rx_i_425: ;CALL
	dec edi
	js rx_finish
	xor r12, 01ada0f39h
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	cmp ebx, -1600409762
	jbe short taken_call_425
	mov r10, rax
	jmp rx_i_426
taken_call_425:
	push rax
	call rx_i_35

rx_i_426: ;IMUL_32
	dec edi
	js rx_finish
	xor r11, 04dd16ca4h
	mov ecx, r11d
	call rx_read_dataset
	movsxd rcx, eax
	movsxd rax, r10d
	imul rax, rcx
	mov r13, rax

rx_i_427: ;SUB_64
	dec edi
	js rx_finish
	xor r12, 04df5ce05h
	mov ecx, r12d
	call rx_read_dataset
	sub rax, r10
	mov rcx, rax
	mov eax, r15d
	xor eax, 0a5d40d0ah
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_428: ;FPADD
	dec edi
	js rx_finish
	xor r13, 01ad12ce2h
	mov ecx, r13d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm7
	movsd xmm1, xmm0

rx_i_429: ;IMUL_32
	dec edi
	js rx_finish
	xor rbx, 0a3c5391dh
	mov ecx, ebx
	call rx_read_dataset
	movsxd rcx, eax
	movsxd rax, r10d
	imul rax, rcx
	mov r14, rax

rx_i_430: ;IMULH_64
	dec edi
	js rx_finish
	xor r12, 09dd55ba0h
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, r9
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, r14d
	xor eax, 0dcca31efh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_431: ;MULH_64
	dec edi
	js rx_finish
	xor r11, 0d6cae9aeh
	mov eax, r11d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, r11
	mul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, r9d
	xor eax, 0801190f4h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_432: ;RET
	dec edi
	js rx_finish
	xor r11, 0f807a961h
	mov eax, r11d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	cmp rsp, rbp
	je short not_taken_ret_432
	xor rax, qword ptr [rsp + 8]
	mov rcx, rax
	mov eax, ebx
	xor eax, 0e3b86b2fh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	ret 8
not_taken_ret_432:
	mov rcx, rax
	mov eax, ebx
	xor eax, 0e3b86b2fh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_433: ;MUL_64
	dec edi
	js rx_finish
	xor r12, 0650a4102h
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	imul rax, rax, 1990438276
	mov r15, rax

rx_i_434: ;FPADD
	dec edi
	js rx_finish
	xor r14, 019cc0e5h
	mov ecx, r14d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, 4744016937443393536
	movd xmm9, rax
	addsd xmm0, xmm9
	movsd xmm5, xmm0

rx_i_435: ;ROR_64
	dec edi
	js rx_finish
	xor r12, 0ed17ab58h
	mov ecx, r12d
	call rx_read_dataset
	mov rcx, r13
	ror rax, cl
	mov r13, rax

rx_i_436: ;ADD_32
	dec edi
	js rx_finish
	xor r10, 01c3b321fh
	mov ecx, r10d
	call rx_read_dataset
	add eax, r10d
	mov rbx, rax

rx_i_437: ;ADD_64
	dec edi
	js rx_finish
	xor r13, 0bbb88499h
	mov ecx, r13d
	call rx_read_dataset
	add rax, r12
	mov rcx, rax
	mov eax, r12d
	xor eax, 04722b36fh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_438: ;FPMUL
	dec edi
	js rx_finish
	xor r13, 0167edabdh
	mov ecx, r13d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm3
	movsd xmm1, xmm0

rx_i_439: ;SUB_32
	dec edi
	js rx_finish
	xor r15, 0b940480ah
	mov eax, r15d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	sub eax, r15d
	mov rcx, rax
	mov eax, r9d
	xor eax, 0758605ffh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_440: ;FPADD
	dec edi
	js rx_finish
	xor r15, 0bfc3ca8bh
	mov eax, r15d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, -4481057281345060864
	movd xmm9, rax
	addsd xmm0, xmm9
	movsd xmm7, xmm0

rx_i_441: ;FPMUL
	dec edi
	js rx_finish
	xor rbx, 098a6bcf7h
	mov ecx, ebx
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm3
	mov eax, ebx
	xor eax, 025dac800h
	and eax, 32767
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_442: ;FPMUL
	dec edi
	js rx_finish
	xor r10, 0325b38ebh
	mov ecx, r10d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm1
	mov eax, r12d
	xor eax, 0b7c490eeh
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_443: ;XOR_64
	dec edi
	js rx_finish
	xor r13, 05e807e81h
	mov ecx, r13d
	call rx_read_dataset
	xor rax, r15
	mov rcx, rax
	mov eax, r10d
	xor eax, 0b28e6e01h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_444: ;RET
	dec edi
	js rx_finish
	xor r10, 062f83728h
	mov eax, r10d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	cmp rsp, rbp
	je short not_taken_ret_444
	cmp r12d, 2127765370
	ja short not_taken_ret_444
	xor rax, qword ptr [rsp + 8]
	mov r9, rax
	ret 8
not_taken_ret_444:
	mov r9, rax

rx_i_445: ;ADD_64
	dec edi
	js rx_finish
	xor r14, 0d18ec075h
	mov ecx, r14d
	call rx_read_dataset
	add rax, 529736748
	mov rcx, rax
	mov eax, r9d
	xor eax, 01f93242ch
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_446: ;FPDIV
	dec edi
	js rx_finish
	xor r14, 0a53dd1bh
	mov ecx, r14d
	call rx_read_dataset
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	divsd xmm0, xmm7
	mov eax, r11d
	xor eax, 02fa7c0f6h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_447: ;RET
	dec edi
	js rx_finish
	xor r14, 0232d1285h
	mov eax, r14d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	cmp rsp, rbp
	je short not_taken_ret_447
	cmp r12d, 1332855833
	ja short not_taken_ret_447
	xor rax, qword ptr [rsp + 8]
	mov rcx, rax
	mov eax, r9d
	xor eax, 04f71c419h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	ret 8
not_taken_ret_447:
	mov rcx, rax
	mov eax, r9d
	xor eax, 04f71c419h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_448: ;FPSUB
	dec edi
	js rx_finish
	xor rbx, 042455dd8h
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm7
	movsd xmm5, xmm0

rx_i_449: ;ADD_64
	dec edi
	js rx_finish
	xor r13, 09ae009b2h
	mov ecx, r13d
	call rx_read_dataset
	add rax, r11
	mov rcx, rax
	mov eax, r9d
	xor eax, 084d1f575h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_450: ;MULH_64
	dec edi
	js rx_finish
	xor r12, 01734708eh
	mov ecx, r12d
	call rx_read_dataset
	mov r8, rdx
	mov rcx, r15
	mul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, r13d
	xor eax, 03166163h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_451: ;FPADD
	dec edi
	js rx_finish
	xor rbx, 01596d0e8h
	mov ecx, ebx
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm7
	movsd xmm5, xmm0

rx_i_452: ;FPSUB
	dec edi
	js rx_finish
	xor r9, 0390cfdb0h
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm3
	mov eax, r9d
	xor eax, 0a700e3f3h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_453: ;ROL_64
	dec edi
	js rx_finish
	xor rbx, 04f27744bh
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	rol rax, 28
	mov rbx, rax

rx_i_454: ;ROL_64
	dec edi
	js rx_finish
	xor rbx, 04e2c76ffh
	mov ecx, ebx
	call rx_read_dataset
	mov rcx, r12
	rol rax, cl
	mov rcx, rax
	mov eax, ebx
	xor eax, 0f6de92ach
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_455: ;ADD_64
	dec edi
	js rx_finish
	xor rbx, 0c4d99ac9h
	mov ecx, ebx
	call rx_read_dataset
	add rax, -287502157
	mov rbx, rax

rx_i_456: ;RET
	dec edi
	js rx_finish
	xor r13, 040130b88h
	mov ecx, r13d
	call rx_read_dataset
	cmp rsp, rbp
	je short not_taken_ret_456
	xor rax, qword ptr [rsp + 8]
	mov rcx, rax
	mov eax, r11d
	xor eax, 0e27dea25h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	ret 8
not_taken_ret_456:
	mov rcx, rax
	mov eax, r11d
	xor eax, 0e27dea25h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_457: ;IMULH_64
	dec edi
	js rx_finish
	xor r11, 0a2096aa4h
	mov ecx, r11d
	call rx_read_dataset
	mov r8, rdx
	mov rcx, r14
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov rbx, rax

rx_i_458: ;FPADD
	dec edi
	js rx_finish
	xor r13, 081314291h
	mov eax, r13d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, 4746671349487894528
	movd xmm9, rax
	addsd xmm0, xmm9
	movsd xmm4, xmm0

rx_i_459: ;OR_32
	dec edi
	js rx_finish
	xor rbx, 059263cdbh
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or eax, r9d
	mov rbx, rax

rx_i_460: ;OR_64
	dec edi
	js rx_finish
	xor r9, 010e8fe6h
	mov ecx, r9d
	call rx_read_dataset
	or rax, r11
	mov rcx, rax
	mov eax, r9d
	xor eax, 017f52c3fh
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_461: ;SUB_64
	dec edi
	js rx_finish
	xor r9, 09de1a3efh
	mov ecx, r9d
	call rx_read_dataset
	sub rax, r10
	mov rcx, rax
	mov eax, r10d
	xor eax, 058584136h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_462: ;ROL_64
	dec edi
	js rx_finish
	xor r11, 05c79df6eh
	mov ecx, r11d
	call rx_read_dataset
	rol rax, 22
	mov r14, rax

rx_i_463: ;SUB_64
	dec edi
	js rx_finish
	xor r9, 0346f46adh
	mov ecx, r9d
	call rx_read_dataset
	sub rax, 381354340
	mov rcx, rax
	mov eax, r13d
	xor eax, 016bb0164h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_464: ;ADD_64
	dec edi
	js rx_finish
	xor r11, 098ab71fch
	mov ecx, r11d
	call rx_read_dataset
	add rax, r14
	mov rcx, rax
	mov eax, r12d
	xor eax, 0eb453a97h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_465: ;SHL_64
	dec edi
	js rx_finish
	xor r11, 0c814e926h
	mov ecx, r11d
	call rx_read_dataset
	mov rcx, r13
	shl rax, cl
	mov rcx, rax
	mov eax, r12d
	xor eax, 062ef5b99h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_466: ;ADD_64
	dec edi
	js rx_finish
	xor r10, 0c64b4a9eh
	mov eax, r10d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	add rax, -1734323376
	mov r15, rax

rx_i_467: ;ADD_64
	dec edi
	js rx_finish
	xor r9, 08c29341h
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	add rax, r15
	mov r10, rax

rx_i_468: ;MUL_64
	dec edi
	js rx_finish
	xor r12, 06ff587fdh
	mov ecx, r12d
	call rx_read_dataset
	imul rax, r15
	mov rcx, rax
	mov eax, r13d
	xor eax, 0d0673df8h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_469: ;FPADD
	dec edi
	js rx_finish
	xor r12, 0b62c0003h
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm5
	mov eax, r10d
	xor eax, 0d11c1242h
	and eax, 32767
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_470: ;IMUL_32
	dec edi
	js rx_finish
	xor r13, 05c541c42h
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	movsxd rcx, eax
	mov rax, 282682508
	imul rax, rcx
	mov r9, rax

rx_i_471: ;FPADD
	dec edi
	js rx_finish
	xor rbx, 0cbb33f81h
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm1
	mov eax, ebx
	xor eax, 0ad38e588h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_472: ;IMULH_64
	dec edi
	js rx_finish
	xor rbx, 091044dc3h
	mov ecx, ebx
	call rx_read_dataset
	mov r8, rdx
	mov rcx, -13394825
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov rcx, rax
	mov eax, ebx
	xor eax, 0ff339c77h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_473: ;MUL_32
	dec edi
	js rx_finish
	xor r9, 0c0186beh
	mov ecx, r9d
	call rx_read_dataset
	mov ecx, eax
	mov eax, 294019485
	imul rax, rcx
	mov rcx, rax
	mov eax, r9d
	xor eax, 01186619dh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_474: ;XOR_64
	dec edi
	js rx_finish
	xor r14, 090849e3eh
	mov ecx, r14d
	call rx_read_dataset
	xor rax, r11
	mov rcx, rax
	mov eax, r14d
	xor eax, 090d56b4ch
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_475: ;IMUL_32
	dec edi
	js rx_finish
	xor r14, 0cedba9b6h
	mov eax, r14d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	movsxd rcx, eax
	movsxd rax, r13d
	imul rax, rcx
	mov r14, rax

rx_i_476: ;CALL
	dec edi
	js rx_finish
	xor r9, 038f4b9d6h
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	push rax
	call rx_i_12

rx_i_477: ;MUL_64
	dec edi
	js rx_finish
	xor r14, 01fb7637dh
	mov eax, r14d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	imul rax, rax, -751043211
	mov r12, rax

rx_i_478: ;CALL
	dec edi
	js rx_finish
	xor r9, 0b5c0b4d4h
	mov ecx, r9d
	call rx_read_dataset
	cmp r15d, -233120543
	jbe short taken_call_478
	mov r15, rax
	jmp rx_i_479
taken_call_478:
	push rax
	call rx_i_73

rx_i_479: ;FPSUB
	dec edi
	js rx_finish
	xor r10, 0910dcdeeh
	mov eax, r10d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm1
	mov eax, r15d
	xor eax, 04a936216h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_480: ;FPADD
	dec edi
	js rx_finish
	xor rbx, 07ab3b5a4h
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm2
	mov eax, r9d
	xor eax, 0b01bb14ch
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_481: ;ROR_64
	dec edi
	js rx_finish
	xor r12, 07a29ec63h
	mov eax, r12d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	ror rax, 15
	mov r14, rax

rx_i_482: ;MUL_64
	dec edi
	js rx_finish
	xor r14, 02d3d7e7fh
	mov ecx, r14d
	call rx_read_dataset
	imul rax, r10
	mov r12, rax

rx_i_483: ;SUB_64
	dec edi
	js rx_finish
	xor r12, 09b49c793h
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	sub rax, r14
	mov rcx, rax
	mov eax, r13d
	xor eax, 0c42735ech
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_484: ;FPADD
	dec edi
	js rx_finish
	xor r9, 0a9cc4f01h
	mov eax, r9d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm4
	mov eax, r14d
	xor eax, 0d8750eeh
	and eax, 32767
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_485: ;IMULH_64
	dec edi
	js rx_finish
	xor r14, 0225ba1f9h
	mov eax, r14d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, r13
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov r12, rax

rx_i_486: ;OR_64
	dec edi
	js rx_finish
	xor r14, 044a0f592h
	mov ecx, r14d
	call rx_read_dataset
	or rax, r12
	mov r11, rax

rx_i_487: ;ROR_64
	dec edi
	js rx_finish
	xor r11, 07f71f219h
	mov ecx, r11d
	call rx_read_dataset
	ror rax, 50
	mov rcx, rax
	mov eax, r14d
	xor eax, 0545908cah
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_488: ;SAR_64
	dec edi
	js rx_finish
	xor r12, 07027bacdh
	mov ecx, r12d
	call rx_read_dataset
	sar rax, 37
	mov r11, rax

rx_i_489: ;CALL
	dec edi
	js rx_finish
	xor r13, 03a04647h
	mov ecx, r13d
	call rx_read_dataset
	cmp ebx, 554879918
	jbe short taken_call_489
	mov rcx, rax
	mov eax, r15d
	xor eax, 02112cbaeh
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx
	jmp rx_i_490
taken_call_489:
	push rax
	call rx_i_62

rx_i_490: ;ADD_64
	dec edi
	js rx_finish
	xor r15, 0ad072937h
	mov eax, r15d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	add rax, 942846898
	mov rcx, rax
	mov eax, ebx
	xor eax, 03832b3b2h
	and eax, 32767
	mov qword ptr [rsi + rax * 8], rcx

rx_i_491: ;SUB_64
	dec edi
	js rx_finish
	xor r11, 07f78ad34h
	mov ecx, r11d
	call rx_read_dataset
	sub rax, -333279706
	mov r11, rax

rx_i_492: ;IMULH_64
	dec edi
	js rx_finish
	xor r12, 0d8b1788eh
	mov eax, r12d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	mov r8, rdx
	mov rcx, 297357073
	imul rcx
	mov rax, rdx
	mov rdx, r8
	mov r12, rax

rx_i_493: ;FPROUND
	dec edi
	js rx_finish
	xor r10, 0b2ec9f3ah
	mov eax, r10d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, rax
	shl eax, 13
	and rcx, -2048
	and eax, 24576
	cvtsi2sd xmm0, rcx
	or eax, 40896
	mov dword ptr [rsp - 8], eax
	ldmxcsr dword ptr [rsp - 8]
	mov eax, ebx
	xor eax, 0bcd0a942h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_494: ;ROR_64
	dec edi
	js rx_finish
	xor r11, 015c7f598h
	mov ecx, r11d
	call rx_read_dataset
	mov rcx, r9
	ror rax, cl
	mov rcx, rax
	mov eax, r15d
	xor eax, 0ab8bd68h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_495: ;ROR_64
	dec edi
	js rx_finish
	xor rbx, 0902da6bdh
	mov ecx, ebx
	call rx_read_dataset
	mov rcx, r9
	ror rax, cl
	mov r15, rax

rx_i_496: ;AND_64
	dec edi
	js rx_finish
	xor r9, 0491090d9h
	mov ecx, r9d
	call rx_read_dataset
	and rax, r9
	mov r12, rax

rx_i_497: ;FPSUB
	dec edi
	js rx_finish
	xor rbx, 09de81282h
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, 4740027165670637568
	movd xmm9, rax
	subsd xmm0, xmm9
	mov eax, r12d
	xor eax, 02feb2fd7h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_498: ;MULH_64
	dec edi
	js rx_finish
	xor r10, 0b0d50e46h
	mov ecx, r10d
	call rx_read_dataset
	mov r8, rdx
	mov rcx, r11
	mul rcx
	mov rax, rdx
	mov rdx, r8
	mov r14, rax

rx_i_499: ;FPMUL
	dec edi
	js rx_finish
	xor r11, 0e276cad1h
	mov eax, r11d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mov rax, 4739626697148596224
	movd xmm9, rax
	mulsd xmm0, xmm9
	mov eax, ebx
	xor eax, 02d12bd27h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_500: ;AND_64
	dec edi
	js rx_finish
	xor r14, 0fe757b73h
	mov ecx, r14d
	call rx_read_dataset
	and rax, -359802064
	mov r9, rax

rx_i_501: ;FPMUL
	dec edi
	js rx_finish
	xor rbx, 08d25742eh
	mov eax, ebx
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm3
	mov eax, ebx
	xor eax, 0a800c041h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_502: ;FPMUL
	dec edi
	js rx_finish
	xor r15, 0e066fd15h
	mov eax, r15d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	or rax, 2048
	and rax, -2048
	cvtsi2sd xmm0, rax
	mulsd xmm0, xmm1
	movsd xmm8, xmm0

rx_i_503: ;IMUL_32
	dec edi
	js rx_finish
	xor r12, 08925556bh
	mov eax, r12d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	movsxd rcx, eax
	mov rax, -1795485757
	imul rax, rcx
	mov rbx, rax

rx_i_504: ;FPSQRT
	dec edi
	js rx_finish
	xor r10, 04bc870ebh
	mov eax, r10d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	mov rcx, 9223372036854773760
	and rax, rcx
	cvtsi2sd xmm0, rax
	sqrtsd xmm0, xmm0
	mov eax, r10d
	xor eax, 04a250342h
	and eax, 32767
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_505: ;XOR_32
	dec edi
	js rx_finish
	xor rbx, 07d46c503h
	mov ecx, ebx
	call rx_read_dataset
	xor eax, r10d
	mov rcx, rax
	mov eax, r12d
	xor eax, 03e22874bh
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_506: ;RET
	dec edi
	js rx_finish
	xor r10, 09e70b20ch
	mov ecx, r10d
	call rx_read_dataset
	cmp rsp, rbp
	je short not_taken_ret_506
	cmp r15d, 148394770
	ja short not_taken_ret_506
	xor rax, qword ptr [rsp + 8]
	mov rcx, rax
	mov eax, r9d
	xor eax, 08d85312h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx
	ret 8
not_taken_ret_506:
	mov rcx, rax
	mov eax, r9d
	xor eax, 08d85312h
	and eax, 2047
	mov qword ptr [rsi + rax * 8], rcx

rx_i_507: ;FPSUB
	dec edi
	js rx_finish
	xor r13, 0442e4850h
	mov eax, r13d
	and eax, 32767
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm2
	movsd xmm1, xmm0

rx_i_508: ;FPADD
	dec edi
	js rx_finish
	xor r13, 099d48347h
	mov eax, r13d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	and rax, -2048
	cvtsi2sd xmm0, rax
	addsd xmm0, xmm1
	movsd xmm4, xmm0

rx_i_509: ;FPSUB
	dec edi
	js rx_finish
	xor r12, 032c0a28ah
	mov ecx, r12d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm4
	movsd xmm8, xmm0

rx_i_510: ;FPSUB
	dec edi
	js rx_finish
	xor r9, 0a973d58ch
	mov ecx, r9d
	call rx_read_dataset
	and rax, -2048
	cvtsi2sd xmm0, rax
	subsd xmm0, xmm1
	mov eax, r11d
	xor eax, 05e890759h
	and eax, 2047
	movd qword ptr [rsi + rax * 8], xmm0

rx_i_511: ;RET
	dec edi
	js rx_finish
	xor r10, 0d3b7165ch
	mov eax, r10d
	and eax, 2047
	mov rax, qword ptr [rsi + rax * 8]
	cmp rsp, rbp
	je short not_taken_ret_511
	cmp r11d, -260506265
	ja short not_taken_ret_511
	xor rax, qword ptr [rsp + 8]
	mov r14, rax
	ret 8
not_taken_ret_511:
	mov r14, rax

	jmp rx_i_0
