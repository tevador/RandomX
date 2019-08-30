randomx_isn_0:
	; ISMULH_R r0, r3
	mov rax, r8
	imul r11
	mov r8, rdx
randomx_isn_1:
	; IROR_R r0, r6
	mov ecx, r14d
	ror r8, cl
randomx_isn_2:
	; FADD_R f1, a2
	addpd xmm1, xmm10
randomx_isn_3:
	; IXOR_M r1, L1[r5+1954652011]
	lea eax, [r13d+1954652011]
	and eax, 16376
	xor r9, qword ptr [rsi+rax]
randomx_isn_4:
	; FMUL_R e2, a3
	mulpd xmm6, xmm11
randomx_isn_5:
	; FADD_M f0, L2[r0-772804104]
	lea eax, [r8d-772804104]
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm0, xmm12
randomx_isn_6:
	; IMUL_R r6, r4
	imul r14, r12
randomx_isn_7:
	; CBRANCH r5, 1674196118, COND 2
	add r13, 1674196118
	test r13, 261120
	jz randomx_isn_0
randomx_isn_8:
	; ISWAP_R r7, r6
	xchg r15, r14
randomx_isn_9:
	; ISTORE L1[r1-439821682], r3
	lea eax, [r9d-439821682]
	and eax, 16376
	mov qword ptr [rsi+rax], r11
randomx_isn_10:
	; IXOR_R r2, r4
	xor r10, r12
randomx_isn_11:
	; FADD_R f2, a1
	addpd xmm2, xmm9
randomx_isn_12:
	; IXOR_M r0, L1[r1+952699079]
	lea eax, [r9d+952699079]
	and eax, 16376
	xor r8, qword ptr [rsi+rax]
randomx_isn_13:
	; ISMULH_R r5, r2
	mov rax, r13
	imul r10
	mov r13, rdx
randomx_isn_14:
	; INEG_R r4
	neg r12
randomx_isn_15:
	; INEG_R r1
	neg r9
randomx_isn_16:
	; IMUL_M r3, L1[r2+620091535]
	lea eax, [r10d+620091535]
	and eax, 16376
	imul r11, qword ptr [rsi+rax]
randomx_isn_17:
	; FADD_R f1, a0
	addpd xmm1, xmm8
randomx_isn_18:
	; IMUL_RCP r5, 2611385784
	mov rax, 15169754503470242065
	imul r13, rax
randomx_isn_19:
	; IXOR_R r2, 922368940
	xor r10, 922368940
randomx_isn_20:
	; FADD_R f3, a1
	addpd xmm3, xmm9
randomx_isn_21:
	; IXOR_R r3, r6
	xor r11, r14
randomx_isn_22:
	; FSWAP_R e1
	shufpd xmm5, xmm5, 1
randomx_isn_23:
	; ISUB_R r0, r5
	sub r8, r13
randomx_isn_24:
	; ISTORE L1[r6-1574415460], r7
	lea eax, [r14d-1574415460]
	and eax, 16376
	mov qword ptr [rsi+rax], r15
randomx_isn_25:
	; FADD_M f3, L1[r3+1766115210]
	lea eax, [r11d+1766115210]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm3, xmm12
randomx_isn_26:
	; FSCAL_R f1
	xorps xmm1, xmm15
randomx_isn_27:
	; CBRANCH r2, 1731738265, COND 6
	add r10, 1731746457
	test r10, 4177920
	jz randomx_isn_20
randomx_isn_28:
	; IXOR_R r4, r1
	xor r12, r9
randomx_isn_29:
	; CBRANCH r4, 1937048537, COND 3
	add r12, 1937050585
	test r12, 522240
	jz randomx_isn_29
randomx_isn_30:
	; ISWAP_R r3, r5
	xchg r11, r13
randomx_isn_31:
	; ISMULH_R r7, r5
	mov rax, r15
	imul r13
	mov r15, rdx
randomx_isn_32:
	; IMULH_M r6, L1[r2+1879111790]
	lea ecx, [r10d+1879111790]
	and ecx, 16376
	mov rax, r14
	mul qword ptr [rsi+rcx]
	mov r14, rdx
randomx_isn_33:
	; IMUL_R r5, r0
	imul r13, r8
randomx_isn_34:
	; ISWAP_R r5, r0
	xchg r13, r8
randomx_isn_35:
	; CBRANCH r4, 1174490916, COND 5
	add r12, 1174499108
	test r12, 2088960
	jz randomx_isn_30
randomx_isn_36:
	; CBRANCH r6, -1852457840, COND 8
	add r14, -1852490608
	test r14, 16711680
	jz randomx_isn_36
randomx_isn_37:
	; ISMULH_R r2, r0
	mov rax, r10
	imul r8
	mov r10, rdx
randomx_isn_38:
	; ISUB_R r2, r0
	sub r10, r8
randomx_isn_39:
	; ISTORE L1[r0-38118463], r5
	lea eax, [r8d-38118463]
	and eax, 16376
	mov qword ptr [rsi+rax], r13
randomx_isn_40:
	; IXOR_R r0, r1
	xor r8, r9
randomx_isn_41:
	; IMUL_R r6, r4
	imul r14, r12
randomx_isn_42:
	; ISUB_R r7, r5
	sub r15, r13
randomx_isn_43:
	; FDIV_M e0, L1[r2+1052956160]
	lea eax, [r10d+1052956160]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	andps xmm12, xmm13
	orps xmm12, xmm14
	divpd xmm4, xmm12
randomx_isn_44:
	; CBRANCH r1, 1870241002, COND 11
	add r9, 1870241002
	test r9, 133693440
	jz randomx_isn_37
randomx_isn_45:
	; IXOR_R r1, r4
	xor r9, r12
randomx_isn_46:
	; FMUL_R e3, a1
	mulpd xmm7, xmm9
randomx_isn_47:
	; IXOR_M r0, L1[r2+839895331]
	lea eax, [r10d+839895331]
	and eax, 16376
	xor r8, qword ptr [rsi+rax]
randomx_isn_48:
	; CBRANCH r2, -2128896196, COND 6
	add r10, -2128879812
	test r10, 4177920
	jz randomx_isn_45
randomx_isn_49:
	; CFROUND r1, 13
	mov rax, r9
	and eax, 24576
	or eax, 40896
	push rax
	ldmxcsr dword ptr [rsp]
	pop rax
randomx_isn_50:
	; ISWAP_R r3, r1
	xchg r11, r9
randomx_isn_51:
	; IMUL_RCP r1, 4205062916
	mov rax, 9420568026795290117
	imul r9, rax
randomx_isn_52:
	; FSUB_R f0, a0
	subpd xmm0, xmm8
randomx_isn_53:
	; IMUL_R r7, r6
	imul r15, r14
randomx_isn_54:
	; IADD_RS r1, r2, SHFT 3
	lea r9, [r9+r10*8]
randomx_isn_55:
	; FSQRT_R e3
	sqrtpd xmm7, xmm7
randomx_isn_56:
	; FMUL_R e1, a0
	mulpd xmm5, xmm8
randomx_isn_57:
	; IMUL_RCP r3, 303101651
	mov rax, 16336962008634921950
	imul r11, rax
randomx_isn_58:
	; IMUL_RCP r1, 3375482677
	mov rax, 11735827153567160432
	imul r9, rax
randomx_isn_59:
	; CBRANCH r6, 2116776661, COND 12
	add r14, 2117300949
	test r14, 267386880
	jz randomx_isn_49
randomx_isn_60:
	; IMUL_R r3, r4
	imul r11, r12
randomx_isn_61:
	; FMUL_R e3, a0
	mulpd xmm7, xmm8
randomx_isn_62:
	; ISUB_R r3, 1514378938
	sub r11, 1514378938
randomx_isn_63:
	; FMUL_R e2, a0
	mulpd xmm6, xmm8
randomx_isn_64:
	; ISUB_R r4, r6
	sub r12, r14
randomx_isn_65:
	; FDIV_M e2, L1[r0+1496571595]
	lea eax, [r8d+1496571595]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	andps xmm12, xmm13
	orps xmm12, xmm14
	divpd xmm6, xmm12
randomx_isn_66:
	; FSUB_R f0, a2
	subpd xmm0, xmm10
randomx_isn_67:
	; FDIV_M e3, L2[r7-2139079025]
	lea eax, [r15d-2139079025]
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	andps xmm12, xmm13
	orps xmm12, xmm14
	divpd xmm7, xmm12
randomx_isn_68:
	; FSUB_R f2, a2
	subpd xmm2, xmm10
randomx_isn_69:
	; CBRANCH r3, -1165095866, COND 7
	add r11, -1165063098
	test r11, 8355840
	jz randomx_isn_63
randomx_isn_70:
	; IMULH_R r0, r7
	mov rax, r8
	mul r15
	mov r8, rdx
randomx_isn_71:
	; FMUL_R e2, a0
	mulpd xmm6, xmm8
randomx_isn_72:
	; FMUL_R e0, a3
	mulpd xmm4, xmm11
randomx_isn_73:
	; IMUL_RCP r6, 1636610180
	mov rax, 12102479179596746977
	imul r14, rax
randomx_isn_74:
	; FMUL_R e2, a2
	mulpd xmm6, xmm10
randomx_isn_75:
	; ISTORE L2[r2+473418592], r3
	lea eax, [r10d+473418592]
	and eax, 262136
	mov qword ptr [rsi+rax], r11
randomx_isn_76:
	; IADD_M r1, L1[r3-989917936]
	lea eax, [r11d-989917936]
	and eax, 16376
	add r9, qword ptr [rsi+rax]
randomx_isn_77:
	; CBRANCH r2, 1519854177, COND 7
	add r10, 1519886945
	test r10, 8355840
	jz randomx_isn_70
randomx_isn_78:
	; IMUL_R r2, r6
	imul r10, r14
randomx_isn_79:
	; IMUL_R r4, r1
	imul r12, r9
randomx_isn_80:
	; FMUL_R e2, a1
	mulpd xmm6, xmm9
randomx_isn_81:
	; FSCAL_R f2
	xorps xmm2, xmm15
randomx_isn_82:
	; IXOR_M r2, L1[r1+192323103]
	lea eax, [r9d+192323103]
	and eax, 16376
	xor r10, qword ptr [rsi+rax]
randomx_isn_83:
	; IMUL_R r7, r4
	imul r15, r12
randomx_isn_84:
	; FADD_R f2, a0
	addpd xmm2, xmm8
randomx_isn_85:
	; FSUB_M f1, L2[r6-1549504487]
	lea eax, [r14d-1549504487]
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm1, xmm12
randomx_isn_86:
	; FSUB_R f0, a3
	subpd xmm0, xmm11
randomx_isn_87:
	; CFROUND r3, 31
	mov rax, r11
	rol rax, 46
	and eax, 24576
	or eax, 40896
	push rax
	ldmxcsr dword ptr [rsp]
	pop rax
randomx_isn_88:
	; IXOR_R r5, r6
	xor r13, r14
randomx_isn_89:
	; FADD_R f3, a2
	addpd xmm3, xmm10
randomx_isn_90:
	; FADD_R f3, a0
	addpd xmm3, xmm8
randomx_isn_91:
	; FSQRT_R e1
	sqrtpd xmm5, xmm5
randomx_isn_92:
	; ISUB_R r6, r2
	sub r14, r10
randomx_isn_93:
	; ISUB_R r0, r4
	sub r8, r12
randomx_isn_94:
	; FADD_R f1, a2
	addpd xmm1, xmm10
randomx_isn_95:
	; IMUL_R r1, r2
	imul r9, r10
randomx_isn_96:
	; FSCAL_R f1
	xorps xmm1, xmm15
randomx_isn_97:
	; ISTORE L1[r7-1901001017], r7
	lea eax, [r15d-1901001017]
	and eax, 16376
	mov qword ptr [rsi+rax], r15
randomx_isn_98:
	; FADD_R f1, a3
	addpd xmm1, xmm11
randomx_isn_99:
	; CBRANCH r2, -425599201, COND 9
	add r10, -425533665
	test r10, 33423360
	jz randomx_isn_83
randomx_isn_100:
	; IXOR_R r4, r6
	xor r12, r14
randomx_isn_101:
	; FMUL_R e0, a3
	mulpd xmm4, xmm11
randomx_isn_102:
	; FADD_M f0, L1[r0+1590646897]
	lea eax, [r8d+1590646897]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm0, xmm12
randomx_isn_103:
	; FMUL_R e0, a1
	mulpd xmm4, xmm9
randomx_isn_104:
	; IMUL_R r4, r7
	imul r12, r15
randomx_isn_105:
	; ISUB_R r1, r0
	sub r9, r8
randomx_isn_106:
	; FSUB_R f1, a2
	subpd xmm1, xmm10
randomx_isn_107:
	; FMUL_R e1, a1
	mulpd xmm5, xmm9
randomx_isn_108:
	; FMUL_R e1, a2
	mulpd xmm5, xmm10
randomx_isn_109:
	; FADD_R f3, a2
	addpd xmm3, xmm10
randomx_isn_110:
	; IXOR_R r0, r3
	xor r8, r11
randomx_isn_111:
	; IMUL_R r0, 1421329412
	imul r8, 1421329412
randomx_isn_112:
	; FSUB_R f0, a2
	subpd xmm0, xmm10
randomx_isn_113:
	; IMUL_R r5, r4
	imul r13, r12
randomx_isn_114:
	; IADD_RS r7, r3, SHFT 2
	lea r15, [r15+r11*4]
randomx_isn_115:
	; FADD_R f3, a3
	addpd xmm3, xmm11
randomx_isn_116:
	; ISTORE L1[r3-160363922], r0
	lea eax, [r11d-160363922]
	and eax, 16376
	mov qword ptr [rsi+rax], r8
randomx_isn_117:
	; IMULH_R r0, r6
	mov rax, r8
	mul r14
	mov r8, rdx
randomx_isn_118:
	; FSWAP_R f2
	shufpd xmm2, xmm2, 1
randomx_isn_119:
	; FMUL_R e1, a0
	mulpd xmm5, xmm8
randomx_isn_120:
	; IROR_R r0, 12
	ror r8, 12
randomx_isn_121:
	; FADD_M f0, L1[r0+282806289]
	lea eax, [r8d+282806289]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm0, xmm12
randomx_isn_122:
	; FADD_M f3, L1[r7+1601529113]
	lea eax, [r15d+1601529113]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm3, xmm12
randomx_isn_123:
	; IMUL_RCP r2, 2522040806
	mov rax, 15707153176462985744
	imul r10, rax
randomx_isn_124:
	; ISUB_M r0, L1[r3+974906597]
	lea eax, [r11d+974906597]
	and eax, 16376
	sub r8, qword ptr [rsi+rax]
randomx_isn_125:
	; CBRANCH r2, 1508706439, COND 14
	add r10, 1506609287
	test r10, 1069547520
	jz randomx_isn_124
randomx_isn_126:
	; IXOR_R r4, r5
	xor r12, r13
randomx_isn_127:
	; IMUL_R r7, r2
	imul r15, r10
randomx_isn_128:
	; IROR_R r4, r0
	mov ecx, r8d
	ror r12, cl
randomx_isn_129:
	; CBRANCH r0, -497803311, COND 3
	add r8, -497804335
	test r8, 522240
	jz randomx_isn_126
randomx_isn_130:
	; FSUB_M f0, L1[r3+1789853646]
	lea eax, [r11d+1789853646]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm0, xmm12
randomx_isn_131:
	; ISMULH_R r6, r3
	mov rax, r14
	imul r11
	mov r14, rdx
randomx_isn_132:
	; FMUL_R e0, a3
	mulpd xmm4, xmm11
randomx_isn_133:
	; FSUB_R f2, a1
	subpd xmm2, xmm9
randomx_isn_134:
	; CBRANCH r3, -1567551204, COND 11
	add r11, -1567026916
	test r11, 133693440
	jz randomx_isn_130
randomx_isn_135:
	; FSUB_M f2, L2[r5+1167508659]
	lea eax, [r13d+1167508659]
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm2, xmm12
randomx_isn_136:
	; IMUL_R r4, r0
	imul r12, r8
randomx_isn_137:
	; IMULH_R r7, r6
	mov rax, r15
	mul r14
	mov r15, rdx
randomx_isn_138:
	; FMUL_R e3, a2
	mulpd xmm7, xmm10
randomx_isn_139:
	; IMUL_R r2, r6
	imul r10, r14
randomx_isn_140:
	; ISTORE L1[r0+1277653290], r3
	lea eax, [r8d+1277653290]
	and eax, 16376
	mov qword ptr [rsi+rax], r11
randomx_isn_141:
	; IXOR_M r0, L1[r6-2131931958]
	lea eax, [r14d-2131931958]
	and eax, 16376
	xor r8, qword ptr [rsi+rax]
randomx_isn_142:
	; FSUB_R f3, a3
	subpd xmm3, xmm11
randomx_isn_143:
	; IROL_R r6, r1
	mov ecx, r9d
	rol r14, cl
randomx_isn_144:
	; FADD_R f1, a3
	addpd xmm1, xmm11
randomx_isn_145:
	; FMUL_R e0, a3
	mulpd xmm4, xmm11
randomx_isn_146:
	; FSQRT_R e0
	sqrtpd xmm4, xmm4
randomx_isn_147:
	; IADD_RS r7, r4, SHFT 0
	lea r15, [r15+r12*1]
randomx_isn_148:
	; FSUB_R f3, a1
	subpd xmm3, xmm9
randomx_isn_149:
	; ISTORE L2[r1-1073333533], r3
	lea eax, [r9d-1073333533]
	and eax, 262136
	mov qword ptr [rsi+rax], r11
randomx_isn_150:
	; FMUL_R e3, a3
	mulpd xmm7, xmm11
randomx_isn_151:
	; ISUB_R r6, r3
	sub r14, r11
randomx_isn_152:
	; IMULH_M r7, L2[r1+1647843648]
	lea ecx, [r9d+1647843648]
	and ecx, 262136
	mov rax, r15
	mul qword ptr [rsi+rcx]
	mov r15, rdx
randomx_isn_153:
	; FMUL_R e0, a0
	mulpd xmm4, xmm8
randomx_isn_154:
	; IROR_R r3, r0
	mov ecx, r8d
	ror r11, cl
randomx_isn_155:
	; IADD_M r3, L1[r7-1322060518]
	lea eax, [r15d-1322060518]
	and eax, 16376
	add r11, qword ptr [rsi+rax]
randomx_isn_156:
	; CBRANCH r3, 608981196, COND 1
	add r11, 608981708
	test r11, 130560
	jz randomx_isn_156
randomx_isn_157:
	; FSUB_M f0, L2[r7-252644586]
	lea eax, [r15d-252644586]
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm0, xmm12
randomx_isn_158:
	; CBRANCH r2, 868397474, COND 15
	add r10, 864203170
	test r10, 2139095040
	jz randomx_isn_157
randomx_isn_159:
	; ISUB_R r5, r3
	sub r13, r11
randomx_isn_160:
	; FMUL_R e0, a0
	mulpd xmm4, xmm8
randomx_isn_161:
	; FMUL_R e2, a1
	mulpd xmm6, xmm9
randomx_isn_162:
	; CBRANCH r0, 887338591, COND 6
	add r8, 887346783
	test r8, 4177920
	jz randomx_isn_159
randomx_isn_163:
	; IADD_RS r3, r3, SHFT 3
	lea r11, [r11+r11*8]
randomx_isn_164:
	; IMUL_RCP r7, 3593878304
	mov rax, 11022655166993703745
	imul r15, rax
randomx_isn_165:
	; CBRANCH r0, 1452880957, COND 13
	add r8, 1453929533
	test r8, 534773760
	jz randomx_isn_163
randomx_isn_166:
	; ISUB_M r6, L2[r3+1539038396]
	lea eax, [r11d+1539038396]
	and eax, 262136
	sub r14, qword ptr [rsi+rax]
randomx_isn_167:
	; IMUL_RCP r3, 1202036339
	mov rax, 16477905023274079568
	imul r11, rax
randomx_isn_168:
	; CBRANCH r1, -1295757940, COND 13
	add r9, -1293660788
	test r9, 534773760
	jz randomx_isn_166
randomx_isn_169:
	; FADD_M f2, L1[r2+876697387]
	lea eax, [r10d+876697387]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm2, xmm12
randomx_isn_170:
	; IMUL_R r0, r6
	imul r8, r14
randomx_isn_171:
	; FMUL_R e1, a3
	mulpd xmm5, xmm11
randomx_isn_172:
	; FMUL_R e0, a2
	mulpd xmm4, xmm10
randomx_isn_173:
	; FSUB_M f3, L1[r2-1083472792]
	lea eax, [r10d-1083472792]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm3, xmm12
randomx_isn_174:
	; CBRANCH r1, -1476890738, COND 14
	add r9, -1478987890
	test r9, 1069547520
	jz randomx_isn_169
randomx_isn_175:
	; ISUB_R r4, r7
	sub r12, r15
randomx_isn_176:
	; ISUB_R r0, 1685118604
	sub r8, 1685118604
randomx_isn_177:
	; FMUL_R e0, a1
	mulpd xmm4, xmm9
randomx_isn_178:
	; ISUB_M r0, L1[r7-1897974312]
	lea eax, [r15d-1897974312]
	and eax, 16376
	sub r8, qword ptr [rsi+rax]
randomx_isn_179:
	; IXOR_R r4, r0
	xor r12, r8
randomx_isn_180:
	; IXOR_R r7, r2
	xor r15, r10
randomx_isn_181:
	; FSCAL_R f1
	xorps xmm1, xmm15
randomx_isn_182:
	; ISWAP_R r6, r2
	xchg r14, r10
randomx_isn_183:
	; IADD_RS r3, r1, SHFT 3
	lea r11, [r11+r9*8]
randomx_isn_184:
	; ISTORE L1[r6-1997634426], r7
	lea eax, [r14d-1997634426]
	and eax, 16376
	mov qword ptr [rsi+rax], r15
randomx_isn_185:
	; IXOR_R r2, r7
	xor r10, r15
randomx_isn_186:
	; IMUL_R r4, r3
	imul r12, r11
randomx_isn_187:
	; IMUL_RCP r7, 1830833174
	mov rax, 10818593911149047378
	imul r15, rax
randomx_isn_188:
	; FMUL_R e0, a2
	mulpd xmm4, xmm10
randomx_isn_189:
	; FADD_R f1, a3
	addpd xmm1, xmm11
randomx_isn_190:
	; CBRANCH r7, 121030040, COND 15
	add r15, 129418648
	test r15, 2139095040
	jz randomx_isn_188
randomx_isn_191:
	; IADD_RS r6, r1, SHFT 0
	lea r14, [r14+r9*1]
randomx_isn_192:
	; FSUB_R f3, a2
	subpd xmm3, xmm10
randomx_isn_193:
	; CBRANCH r5, 1139434462, COND 11
	add r13, 1139434462
	test r13, 133693440
	jz randomx_isn_191
randomx_isn_194:
	; FMUL_R e2, a0
	mulpd xmm6, xmm8
randomx_isn_195:
	; FMUL_R e2, a3
	mulpd xmm6, xmm11
randomx_isn_196:
	; CBRANCH r4, 429294077, COND 2
	add r12, 429295101
	test r12, 261120
	jz randomx_isn_194
randomx_isn_197:
	; IMUL_R r1, r2
	imul r9, r10
randomx_isn_198:
	; FMUL_R e3, a0
	mulpd xmm7, xmm8
randomx_isn_199:
	; IMUL_R r2, r3
	imul r10, r11
randomx_isn_200:
	; IMUL_RCP r1, 193535702
	mov rax, 12792885514067893012
	imul r9, rax
randomx_isn_201:
	; IMUL_R r0, r5
	imul r8, r13
randomx_isn_202:
	; ISUB_R r1, r2
	sub r9, r10
randomx_isn_203:
	; FSUB_R f0, a3
	subpd xmm0, xmm11
randomx_isn_204:
	; FSQRT_R e3
	sqrtpd xmm7, xmm7
randomx_isn_205:
	; FMUL_R e0, a3
	mulpd xmm4, xmm11
randomx_isn_206:
	; IMUL_R r2, r1
	imul r10, r9
randomx_isn_207:
	; IADD_RS r1, r1, SHFT 3
	lea r9, [r9+r9*8]
randomx_isn_208:
	; ISUB_R r6, r4
	sub r14, r12
randomx_isn_209:
	; ISUB_R r0, r7
	sub r8, r15
randomx_isn_210:
	; IADD_M r6, L1[r1+313140284]
	lea eax, [r9d+313140284]
	and eax, 16376
	add r14, qword ptr [rsi+rax]
randomx_isn_211:
	; CBRANCH r4, 1358359929, COND 11
	add r12, 1358622073
	test r12, 133693440
	jz randomx_isn_197
randomx_isn_212:
	; FSQRT_R e0
	sqrtpd xmm4, xmm4
randomx_isn_213:
	; ISTORE L1[r3+18641493], r5
	lea eax, [r11d+18641493]
	and eax, 16376
	mov qword ptr [rsi+rax], r13
randomx_isn_214:
	; CBRANCH r2, 1232471888, COND 7
	add r10, 1232504656
	test r10, 8355840
	jz randomx_isn_212
randomx_isn_215:
	; IADD_M r1, L1[r3+1138069575]
	lea eax, [r11d+1138069575]
	and eax, 16376
	add r9, qword ptr [rsi+rax]
randomx_isn_216:
	; FSQRT_R e0
	sqrtpd xmm4, xmm4
randomx_isn_217:
	; IMUL_R r3, r4
	imul r11, r12
randomx_isn_218:
	; FMUL_R e3, a3
	mulpd xmm7, xmm11
randomx_isn_219:
	; IROL_R r7, r1
	mov ecx, r9d
	rol r15, cl
randomx_isn_220:
	; FMUL_R e2, a1
	mulpd xmm6, xmm9
randomx_isn_221:
	; IXOR_M r2, L3[697832]
	xor r10, qword ptr [rsi+697832]
randomx_isn_222:
	; IADD_RS r1, r6, SHFT 2
	lea r9, [r9+r14*4]
randomx_isn_223:
	; ISWAP_R r6, r2
	xchg r14, r10
randomx_isn_224:
	; ISUB_R r0, r1
	sub r8, r9
randomx_isn_225:
	; FSQRT_R e3
	sqrtpd xmm7, xmm7
randomx_isn_226:
	; ISUB_R r5, r1
	sub r13, r9
randomx_isn_227:
	; ISTORE L1[r0+238217802], r2
	lea eax, [r8d+238217802]
	and eax, 16376
	mov qword ptr [rsi+rax], r10
randomx_isn_228:
	; IMUL_RCP r5, 324261767
	mov rax, 15270872674734795667
	imul r13, rax
randomx_isn_229:
	; FSCAL_R f0
	xorps xmm0, xmm15
randomx_isn_230:
	; FSQRT_R e3
	sqrtpd xmm7, xmm7
randomx_isn_231:
	; IROL_R r1, r5
	mov ecx, r13d
	rol r9, cl
randomx_isn_232:
	; ISUB_R r6, r1
	sub r14, r9
randomx_isn_233:
	; FADD_R f2, a0
	addpd xmm2, xmm8
randomx_isn_234:
	; FADD_R f1, a3
	addpd xmm1, xmm11
randomx_isn_235:
	; IXOR_R r3, 1240450588
	xor r11, 1240450588
randomx_isn_236:
	; FSUB_R f1, a2
	subpd xmm1, xmm10
randomx_isn_237:
	; IMULH_R r6, r3
	mov rax, r14
	mul r11
	mov r14, rdx
randomx_isn_238:
	; FSUB_R f1, a3
	subpd xmm1, xmm11
randomx_isn_239:
	; FSUB_R f1, a2
	subpd xmm1, xmm10
randomx_isn_240:
	; FSUB_M f1, L1[r7+1330184615]
	lea eax, [r15d+1330184615]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm1, xmm12
randomx_isn_241:
	; FMUL_R e2, a3
	mulpd xmm6, xmm11
randomx_isn_242:
	; CBRANCH r3, -427325404, COND 11
	add r11, -427063260
	test r11, 133693440
	jz randomx_isn_236
randomx_isn_243:
	; IMUL_R r5, r7
	imul r13, r15
randomx_isn_244:
	; FMUL_R e3, a3
	mulpd xmm7, xmm11
randomx_isn_245:
	; ISMULH_M r7, L1[r0-84959236]
	lea ecx, [r8d-84959236]
	and ecx, 16376
	mov rax, r15
	imul qword ptr [rsi+rcx]
	mov r15, rdx
randomx_isn_246:
	; IMUL_R r6, r1
	imul r14, r9
randomx_isn_247:
	; FMUL_R e2, a1
	mulpd xmm6, xmm9
randomx_isn_248:
	; IADD_M r1, L2[r3+1223504721]
	lea eax, [r11d+1223504721]
	and eax, 262136
	add r9, qword ptr [rsi+rax]
randomx_isn_249:
	; FADD_R f1, a2
	addpd xmm1, xmm10
randomx_isn_250:
	; IXOR_M r4, L1[r2-1447740505]
	lea eax, [r10d-1447740505]
	and eax, 16376
	xor r12, qword ptr [rsi+rax]
randomx_isn_251:
	; IXOR_R r0, r5
	xor r8, r13
randomx_isn_252:
	; CBRANCH r4, -1337905977, COND 4
	add r12, -1337903929
	test r12, 1044480
	jz randomx_isn_251
randomx_isn_253:
	; FSUB_R f1, a1
	subpd xmm1, xmm9
randomx_isn_254:
	; FMUL_R e0, a0
	mulpd xmm4, xmm8
randomx_isn_255:
	; CBRANCH r5, 437071043, COND 11
	add r13, 436808899
	test r13, 133693440
	jz randomx_isn_253
