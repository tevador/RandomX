	mov rbp, rsp      ;# beginning of VM stack
	mov rdi, 1048577  ;# number of VM instructions to execute + 1

	xorps xmm10, xmm10
	cmpeqpd xmm10, xmm10
	psrlq xmm10, 1    ;# mask for absolute value = 0x7fffffffffffffff7fffffffffffffff

	;# reset rounding mode
	mov dword ptr [rsp-8], 40896
	ldmxcsr dword ptr [rsp-8]

	;# load integer registers
	mov r8, qword ptr [rcx+0]
	mov r9, qword ptr [rcx+8]
	mov r10, qword ptr [rcx+16]
	mov r11, qword ptr [rcx+24]
	mov r12, qword ptr [rcx+32]
	mov r13, qword ptr [rcx+40]
	mov r14, qword ptr [rcx+48]
	mov r15, qword ptr [rcx+56]

	;# initialize floating point registers
	xorps xmm8, xmm8
	cvtsi2sd xmm8, qword ptr [rcx+72]
	pslldq xmm8, 8
	cvtsi2sd xmm8, qword ptr [rcx+64]

	xorps xmm9, xmm9
	cvtsi2sd xmm9, qword ptr [rcx+88]
	pslldq xmm9, 8
	cvtsi2sd xmm9, qword ptr [rcx+80]

	xorps xmm2, xmm2
	cvtsi2sd xmm2, qword ptr [rcx+104]
	pslldq xmm2, 8
	cvtsi2sd xmm2, qword ptr [rcx+96]

	xorps xmm3, xmm3
	cvtsi2sd xmm3, qword ptr [rcx+120]
	pslldq xmm3, 8
	cvtsi2sd xmm3, qword ptr [rcx+112]

	lea rcx, [rcx+64]

	xorps xmm4, xmm4
	cvtsi2sd xmm4, qword ptr [rcx+72]
	pslldq xmm4, 8
	cvtsi2sd xmm4, qword ptr [rcx+64]

	xorps xmm5, xmm5
	cvtsi2sd xmm5, qword ptr [rcx+88]
	pslldq xmm5, 8
	cvtsi2sd xmm5, qword ptr [rcx+80]

	xorps xmm6, xmm6
	cvtsi2sd xmm6, qword ptr [rcx+104]
	pslldq xmm6, 8
	cvtsi2sd xmm6, qword ptr [rcx+96]

	xorps xmm7, xmm7
	cvtsi2sd xmm7, qword ptr [rcx+120]
	pslldq xmm7, 8
	cvtsi2sd xmm7, qword ptr [rcx+112]