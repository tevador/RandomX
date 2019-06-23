randomx_isn_0:
	; IROR_R r6, r1
	mov ecx, r9d
	ror r14, cl
randomx_isn_1:
	; IMUL_R r6, r0
	imul r14, r8
randomx_isn_2:
	; IADD_RS r1, r5, SHFT 3
	lea r9, [r9+r13*8]
randomx_isn_3:
	; CBRANCH r7, 1640213454, COND 13
	add r15, 1642310606
	test r15, 534773760
	jz randomx_isn_0
randomx_isn_4:
	; FADD_M f1, L1[r5+47767428]
	lea eax, [r13d+47767428]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm1, xmm12
randomx_isn_5:
	; IXOR_M r3, L1[r0+142785922]
	lea eax, [r8d+142785922]
	and eax, 16376
	xor r11, qword ptr [rsi+rax]
randomx_isn_6:
	; IMUL_RCP r4, 107807738
	mov rax, 11482849582516797401
	imul r12, rax
randomx_isn_7:
	; FMUL_R e2, a1
	mulpd xmm6, xmm9
randomx_isn_8:
	; ISUB_R r3, -349499415
	sub r11, -349499415
randomx_isn_9:
	; FSCAL_R f0
	xorps xmm0, xmm15
randomx_isn_10:
	; FADD_M f2, L1[r0+141880309]
	lea eax, [r8d+141880309]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm2, xmm12
randomx_isn_11:
	; ISWAP_R r5, r5
randomx_isn_12:
	; CBRANCH r4, -1547477139, COND 7
	add r12, -1547460755
	test r12, 8355840
	jz randomx_isn_7
randomx_isn_13:
	; IADD_RS r3, r4, SHFT 2
	lea r11, [r11+r12*4]
randomx_isn_14:
	; FMUL_R e1, a1
	mulpd xmm5, xmm9
randomx_isn_15:
	; FMUL_R e1, a0
	mulpd xmm5, xmm8
randomx_isn_16:
	; IADD_M r3, L3[1242616]
	add r11, qword ptr [rsi+1242616]
randomx_isn_17:
	; FADD_R f0, a0
	addpd xmm0, xmm8
randomx_isn_18:
	; ISMULH_R r2, r3
	mov rax, r10
	imul r11
	mov r10, rdx
randomx_isn_19:
	; FSUB_R f1, a3
	subpd xmm1, xmm11
randomx_isn_20:
	; FADD_R f2, a1
	addpd xmm2, xmm9
randomx_isn_21:
	; FMUL_R e1, a2
	mulpd xmm5, xmm10
randomx_isn_22:
	; CBRANCH r5, 1217943740, COND 2
	add r13, 1217944764
	test r13, 261120
	jz randomx_isn_13
randomx_isn_23:
	; FADD_R f2, a2
	addpd xmm2, xmm10
randomx_isn_24:
	; ISUB_R r3, r4
	sub r11, r12
randomx_isn_25:
	; IADD_M r6, L2[r0+507902624]
	lea eax, [r8d+507902624]
	and eax, 262136
	add r14, qword ptr [rsi+rax]
randomx_isn_26:
	; ISTORE L1[r6+994770102], r3
	lea eax, [r14d+994770102]
	and eax, 16376
	mov qword ptr [rsi+rax], r11
randomx_isn_27:
	; FSUB_R f0, a0
	subpd xmm0, xmm8
randomx_isn_28:
	; IMUL_R r6, r2
	imul r14, r10
randomx_isn_29:
	; FSQRT_R e0
	sqrtpd xmm4, xmm4
randomx_isn_30:
	; IXOR_R r6, r7
	xor r14, r15
randomx_isn_31:
	; IROL_R r3, 34
	rol r11, 34
randomx_isn_32:
	; ISUB_R r1, r0
	sub r9, r8
randomx_isn_33:
	; ISTORE L2[r3-11196796], r2
	lea eax, [r11d-11196796]
	and eax, 262136
	mov qword ptr [rsi+rax], r10
randomx_isn_34:
	; IMUL_R r1, r5
	imul r9, r13
randomx_isn_35:
	; FSUB_R f3, a0
	subpd xmm3, xmm8
randomx_isn_36:
	; FSWAP_R f3
	shufpd xmm3, xmm3, 1
randomx_isn_37:
	; IMUL_M r5, L1[r1+292730526]
	lea eax, [r9d+292730526]
	and eax, 16376
	imul r13, qword ptr [rsi+rax]
randomx_isn_38:
	; ISUB_R r2, r3
	sub r10, r11
randomx_isn_39:
	; FSUB_R f1, a1
	subpd xmm1, xmm9
randomx_isn_40:
	; ISTORE L1[r5-1314552015], r1
	lea eax, [r13d-1314552015]
	and eax, 16376
	mov qword ptr [rsi+rax], r9
randomx_isn_41:
	; FSUB_M f3, L1[r4+1497617497]
	lea eax, [r12d+1497617497]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm3, xmm12
randomx_isn_42:
	; IADD_RS r4, r7, SHFT 0
	lea r12, [r12+r15*1]
randomx_isn_43:
	; IADD_M r1, L1[r5+1803177253]
	lea eax, [r13d+1803177253]
	and eax, 16376
	add r9, qword ptr [rsi+rax]
randomx_isn_44:
	; IADD_RS r5, r2, 818119872, SHFT 1
	lea r13, [r13+r10*2+818119872]
randomx_isn_45:
	; IROL_R r1, r2
	mov ecx, r10d
	rol r9, cl
randomx_isn_46:
	; IXOR_R r4, r2
	xor r12, r10
randomx_isn_47:
	; FMUL_R e1, a1
	mulpd xmm5, xmm9
randomx_isn_48:
	; IMUL_RCP r2, 3470537647
	mov rax, 11414393182386408729
	imul r10, rax
randomx_isn_49:
	; FMUL_R e3, a2
	mulpd xmm7, xmm10
randomx_isn_50:
	; FADD_R f1, a2
	addpd xmm1, xmm10
randomx_isn_51:
	; ISUB_R r2, r5
	sub r10, r13
randomx_isn_52:
	; FADD_M f0, L2[r0-1471265265]
	lea eax, [r8d-1471265265]
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm0, xmm12
randomx_isn_53:
	; IADD_RS r5, r7, 1775830425, SHFT 1
	lea r13, [r13+r15*2+1775830425]
randomx_isn_54:
	; ISUB_R r1, r3
	sub r9, r11
randomx_isn_55:
	; IROL_R r7, r0
	mov ecx, r8d
	rol r15, cl
randomx_isn_56:
	; FMUL_R e0, a3
	mulpd xmm4, xmm11
randomx_isn_57:
	; IMUL_R r6, r4
	imul r14, r12
randomx_isn_58:
	; FMUL_R e3, a3
	mulpd xmm7, xmm11
randomx_isn_59:
	; ISTORE L1[r7-1893493599], r3
	lea eax, [r15d-1893493599]
	and eax, 16376
	mov qword ptr [rsi+rax], r11
randomx_isn_60:
	; ISMULH_R r5, r2
	mov rax, r13
	imul r10
	mov r13, rdx
randomx_isn_61:
	; IADD_RS r1, r1, SHFT 1
	lea r9, [r9+r9*2]
randomx_isn_62:
	; IMUL_RCP r3, 2733088411
	mov rax, 14494255325841403524
	imul r11, rax
randomx_isn_63:
	; ISUB_R r6, r3
	sub r14, r11
randomx_isn_64:
	; FMUL_R e1, a0
	mulpd xmm5, xmm8
randomx_isn_65:
	; IADD_RS r2, r6, SHFT 3
	lea r10, [r10+r14*8]
randomx_isn_66:
	; FSUB_R f3, a0
	subpd xmm3, xmm8
randomx_isn_67:
	; CBRANCH r4, 1930797839, COND 5
	add r12, 1930797839
	test r12, 2088960
	jz randomx_isn_47
randomx_isn_68:
	; ISUB_R r3, r4
	sub r11, r12
randomx_isn_69:
	; IROR_R r1, r4
	mov ecx, r12d
	ror r9, cl
randomx_isn_70:
	; IADD_RS r0, r4, SHFT 2
	lea r8, [r8+r12*4]
randomx_isn_71:
	; FSUB_M f0, L1[r2+1122756483]
	lea eax, [r10d+1122756483]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm0, xmm12
randomx_isn_72:
	; IMUL_R r4, r6
	imul r12, r14
randomx_isn_73:
	; ISTORE L1[r0-1218068599], r0
	lea eax, [r8d-1218068599]
	and eax, 16376
	mov qword ptr [rsi+rax], r8
randomx_isn_74:
	; IXOR_R r7, r3
	xor r15, r11
randomx_isn_75:
	; ISTORE L2[r6+1269773314], r7
	lea eax, [r14d+1269773314]
	and eax, 262136
	mov qword ptr [rsi+rax], r15
randomx_isn_76:
	; FSUB_R f1, a3
	subpd xmm1, xmm11
randomx_isn_77:
	; ISUB_R r1, -869107550
	sub r9, -869107550
randomx_isn_78:
	; FMUL_R e0, a0
	mulpd xmm4, xmm8
randomx_isn_79:
	; ISUB_R r4, r0
	sub r12, r8
randomx_isn_80:
	; IROR_R r6, r5
	mov ecx, r13d
	ror r14, cl
randomx_isn_81:
	; FSUB_R f3, a0
	subpd xmm3, xmm8
randomx_isn_82:
	; ISUB_R r5, r6
	sub r13, r14
randomx_isn_83:
	; ISMULH_M r1, L1[r4-767260125]
	lea ecx, [r12d-767260125]
	and ecx, 16376
	mov rax, r9
	imul qword ptr [rsi+rcx]
	mov r9, rdx
randomx_isn_84:
	; IMULH_M r7, L1[r0+2131157979]
	lea ecx, [r8d+2131157979]
	and ecx, 16376
	mov rax, r15
	mul qword ptr [rsi+rcx]
	mov r15, rdx
randomx_isn_85:
	; FMUL_R e2, a3
	mulpd xmm6, xmm11
randomx_isn_86:
	; IMUL_RCP r3, 1990691228
	mov rax, 9949830666841158350
	imul r11, rax
randomx_isn_87:
	; FADD_R f1, a1
	addpd xmm1, xmm9
randomx_isn_88:
	; FMUL_R e3, a2
	mulpd xmm7, xmm10
randomx_isn_89:
	; CBRANCH r4, -1169509022, COND 2
	add r12, -1169509022
	test r12, 261120
	jz randomx_isn_80
randomx_isn_90:
	; ISMULH_R r4, r6
	mov rax, r12
	imul r14
	mov r12, rdx
randomx_isn_91:
	; IROR_R r3, r1
	mov ecx, r9d
	ror r11, cl
randomx_isn_92:
	; ISUB_R r7, r5
	sub r15, r13
randomx_isn_93:
	; IXOR_R r3, r1
	xor r11, r9
randomx_isn_94:
	; FSUB_M f1, L1[r5-1131822026]
	lea eax, [r13d-1131822026]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm1, xmm12
randomx_isn_95:
	; FADD_R f2, a1
	addpd xmm2, xmm9
randomx_isn_96:
	; IMUL_M r1, L1[r4-1513325359]
	lea eax, [r12d-1513325359]
	and eax, 16376
	imul r9, qword ptr [rsi+rax]
randomx_isn_97:
	; FSUB_R f1, a1
	subpd xmm1, xmm9
randomx_isn_98:
	; FADD_R f3, a0
	addpd xmm3, xmm8
randomx_isn_99:
	; FSUB_R f3, a2
	subpd xmm3, xmm10
randomx_isn_100:
	; ISUB_R r1, r5
	sub r9, r13
randomx_isn_101:
	; FMUL_R e3, a2
	mulpd xmm7, xmm10
randomx_isn_102:
	; FMUL_R e1, a0
	mulpd xmm5, xmm8
randomx_isn_103:
	; IROR_R r0, r4
	mov ecx, r12d
	ror r8, cl
randomx_isn_104:
	; IMUL_R r2, r0
	imul r10, r8
randomx_isn_105:
	; ISUB_M r2, L3[858120]
	sub r10, qword ptr [rsi+858120]
randomx_isn_106:
	; FSCAL_R f1
	xorps xmm1, xmm15
randomx_isn_107:
	; CFROUND r5, 15
	mov rax, r13
	rol rax, 62
	and eax, 24576
	or eax, 40896
	push rax
	ldmxcsr dword ptr [rsp]
	pop rax
randomx_isn_108:
	; CBRANCH r5, 1726686711, COND 12
	add r13, 1727210999
	test r13, 267386880
	jz randomx_isn_90
randomx_isn_109:
	; ISTORE L1[r5-1659809127], r2
	lea eax, [r13d-1659809127]
	and eax, 16376
	mov qword ptr [rsi+rax], r10
randomx_isn_110:
	; ISUB_R r6, r3
	sub r14, r11
randomx_isn_111:
	; FMUL_R e0, a3
	mulpd xmm4, xmm11
randomx_isn_112:
	; IMUL_R r5, r4
	imul r13, r12
randomx_isn_113:
	; FSUB_M f3, L2[r7-1773511057]
	lea eax, [r15d-1773511057]
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm3, xmm12
randomx_isn_114:
	; FMUL_R e0, a3
	mulpd xmm4, xmm11
randomx_isn_115:
	; IADD_RS r0, r6, SHFT 2
	lea r8, [r8+r14*4]
randomx_isn_116:
	; IADD_M r6, L1[r5+1159120461]
	lea eax, [r13d+1159120461]
	and eax, 16376
	add r14, qword ptr [rsi+rax]
randomx_isn_117:
	; ISUB_R r3, 1826659015
	sub r11, 1826659015
randomx_isn_118:
	; IADD_RS r1, r2, SHFT 0
	lea r9, [r9+r10*1]
randomx_isn_119:
	; CBRANCH r5, 684384225, COND 8
	add r13, 684416993
	test r13, 16711680
	jz randomx_isn_113
randomx_isn_120:
	; FSUB_R f0, a2
	subpd xmm0, xmm10
randomx_isn_121:
	; FSQRT_R e3
	sqrtpd xmm7, xmm7
randomx_isn_122:
	; FADD_R f0, a0
	addpd xmm0, xmm8
randomx_isn_123:
	; IMULH_R r4, r6
	mov rax, r12
	mul r14
	mov r12, rdx
randomx_isn_124:
	; IADD_RS r0, r2, SHFT 2
	lea r8, [r8+r10*4]
randomx_isn_125:
	; FMUL_R e1, a2
	mulpd xmm5, xmm10
randomx_isn_126:
	; IMULH_R r7, r3
	mov rax, r15
	mul r11
	mov r15, rdx
randomx_isn_127:
	; IMULH_R r4, r4
	mov rax, r12
	mul r12
	mov r12, rdx
randomx_isn_128:
	; IADD_RS r5, r3, -1705157619, SHFT 0
	lea r13, [r13+r11*1-1705157619]
randomx_isn_129:
	; CBRANCH r5, -1470853628, COND 7
	add r13, -1470853628
	test r13, 8355840
	jz randomx_isn_129
randomx_isn_130:
	; FMUL_R e2, a2
	mulpd xmm6, xmm10
randomx_isn_131:
	; IMUL_M r6, L1[r1+2122146383]
	lea eax, [r9d+2122146383]
	and eax, 16376
	imul r14, qword ptr [rsi+rax]
randomx_isn_132:
	; CBRANCH r2, -303744735, COND 14
	add r10, -305841887
	test r10, 1069547520
	jz randomx_isn_130
randomx_isn_133:
	; FSQRT_R e2
	sqrtpd xmm6, xmm6
randomx_isn_134:
	; IADD_RS r5, r2, -1473186449, SHFT 0
	lea r13, [r13+r10*1-1473186449]
randomx_isn_135:
	; ISUB_R r7, -463214201
	sub r15, -463214201
randomx_isn_136:
	; IROR_R r3, 41
	ror r11, 41
randomx_isn_137:
	; ISUB_R r4, r1
	sub r12, r9
randomx_isn_138:
	; IADD_M r3, L1[r5-1849564621]
	lea eax, [r13d-1849564621]
	and eax, 16376
	add r11, qword ptr [rsi+rax]
randomx_isn_139:
	; IADD_RS r3, r2, SHFT 2
	lea r11, [r11+r10*4]
randomx_isn_140:
	; FSUB_M f0, L2[r2+1957946605]
	lea eax, [r10d+1957946605]
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm0, xmm12
randomx_isn_141:
	; FMUL_R e0, a3
	mulpd xmm4, xmm11
randomx_isn_142:
	; IMUL_M r2, L3[1791912]
	imul r10, qword ptr [rsi+1791912]
randomx_isn_143:
	; FADD_R f3, a1
	addpd xmm3, xmm9
randomx_isn_144:
	; CBRANCH r1, 594167468, COND 0
	add r9, 594167596
	test r9, 65280
	jz randomx_isn_133
randomx_isn_145:
	; IXOR_M r2, L2[r3-1801032485]
	lea eax, [r11d-1801032485]
	and eax, 262136
	xor r10, qword ptr [rsi+rax]
randomx_isn_146:
	; FADD_R f3, a1
	addpd xmm3, xmm9
randomx_isn_147:
	; ISMULH_R r1, r2
	mov rax, r9
	imul r10
	mov r9, rdx
randomx_isn_148:
	; FADD_M f3, L1[r1-1305269425]
	lea eax, [r9d-1305269425]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm3, xmm12
randomx_isn_149:
	; ISMULH_R r6, r7
	mov rax, r14
	imul r15
	mov r14, rdx
randomx_isn_150:
	; IXOR_R r1, r3
	xor r9, r11
randomx_isn_151:
	; IMUL_RCP r7, 3727394819
	mov rax, 10627820013915238743
	imul r15, rax
randomx_isn_152:
	; FADD_R f1, a1
	addpd xmm1, xmm9
randomx_isn_153:
	; ISTORE L1[r3-1650006450], r2
	lea eax, [r11d-1650006450]
	and eax, 16376
	mov qword ptr [rsi+rax], r10
randomx_isn_154:
	; IMUL_R r1, r4
	imul r9, r12
randomx_isn_155:
	; IADD_RS r7, r4, SHFT 1
	lea r15, [r15+r12*2]
randomx_isn_156:
	; FADD_R f3, a0
	addpd xmm3, xmm8
randomx_isn_157:
	; IROL_R r3, r7
	mov ecx, r15d
	rol r11, cl
randomx_isn_158:
	; ISUB_M r1, L1[r5+1870274033]
	lea eax, [r13d+1870274033]
	and eax, 16376
	sub r9, qword ptr [rsi+rax]
randomx_isn_159:
	; FADD_M f1, L1[r1+676754587]
	lea eax, [r9d+676754587]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm1, xmm12
randomx_isn_160:
	; IXOR_M r2, L1[r4+314141089]
	lea eax, [r12d+314141089]
	and eax, 16376
	xor r10, qword ptr [rsi+rax]
randomx_isn_161:
	; FMUL_R e2, a0
	mulpd xmm6, xmm8
randomx_isn_162:
	; IXOR_R r4, r1
	xor r12, r9
randomx_isn_163:
	; FMUL_R e0, a2
	mulpd xmm4, xmm10
randomx_isn_164:
	; ISUB_M r2, L1[r6-869755487]
	lea eax, [r14d-869755487]
	and eax, 16376
	sub r10, qword ptr [rsi+rax]
randomx_isn_165:
	; IMUL_RCP r6, 476974378
	mov rax, 10381606194246184644
	imul r14, rax
randomx_isn_166:
	; IADD_RS r6, r7, SHFT 0
	lea r14, [r14+r15*1]
randomx_isn_167:
	; ISTORE L1[r3-310360520], r1
	lea eax, [r11d-310360520]
	and eax, 16376
	mov qword ptr [rsi+rax], r9
randomx_isn_168:
	; CBRANCH r7, 770321039, COND 7
	add r15, 770353807
	test r15, 8355840
	jz randomx_isn_156
randomx_isn_169:
	; FMUL_R e2, a3
	mulpd xmm6, xmm11
randomx_isn_170:
	; ISUB_R r2, r7
	sub r10, r15
randomx_isn_171:
	; IADD_RS r7, r6, SHFT 0
	lea r15, [r15+r14*1]
randomx_isn_172:
	; IADD_M r4, L2[r5+590482146]
	lea eax, [r13d+590482146]
	and eax, 262136
	add r12, qword ptr [rsi+rax]
randomx_isn_173:
	; FSCAL_R f2
	xorps xmm2, xmm15
randomx_isn_174:
	; IXOR_M r0, L2[r3+725959150]
	lea eax, [r11d+725959150]
	and eax, 262136
	xor r8, qword ptr [rsi+rax]
randomx_isn_175:
	; IXOR_M r4, L2[r2-819951646]
	lea eax, [r10d-819951646]
	and eax, 262136
	xor r12, qword ptr [rsi+rax]
randomx_isn_176:
	; IADD_RS r0, r6, SHFT 2
	lea r8, [r8+r14*4]
randomx_isn_177:
	; IMUL_M r1, L1[r0+298863646]
	lea eax, [r8d+298863646]
	and eax, 16376
	imul r9, qword ptr [rsi+rax]
randomx_isn_178:
	; CBRANCH r3, -1056632893, COND 10
	add r11, -1056632893
	test r11, 66846720
	jz randomx_isn_169
randomx_isn_179:
	; FMUL_R e3, a1
	mulpd xmm7, xmm9
randomx_isn_180:
	; IXOR_R r0, r1
	xor r8, r9
randomx_isn_181:
	; FMUL_R e0, a2
	mulpd xmm4, xmm10
randomx_isn_182:
	; ISMULH_M r0, L2[r3-482376086]
	lea ecx, [r11d-482376086]
	and ecx, 262136
	mov rax, r8
	imul qword ptr [rsi+rcx]
	mov r8, rdx
randomx_isn_183:
	; ISTORE L1[r0-1702778580], r3
	lea eax, [r8d-1702778580]
	and eax, 16376
	mov qword ptr [rsi+rax], r11
randomx_isn_184:
	; FMUL_R e0, a1
	mulpd xmm4, xmm9
randomx_isn_185:
	; ISUB_M r1, L3[570848]
	sub r9, qword ptr [rsi+570848]
randomx_isn_186:
	; FMUL_R e0, a2
	mulpd xmm4, xmm10
randomx_isn_187:
	; ISTORE L3[r1-861015354], r1
	lea eax, [r9d-861015354]
	and eax, 2097144
	mov qword ptr [rsi+rax], r9
randomx_isn_188:
	; ISWAP_R r6, r5
	xchg r14, r13
randomx_isn_189:
	; ISUB_M r5, L1[r2-416275812]
	lea eax, [r10d-416275812]
	and eax, 16376
	sub r13, qword ptr [rsi+rax]
randomx_isn_190:
	; IADD_M r2, L3[218936]
	add r10, qword ptr [rsi+218936]
randomx_isn_191:
	; FSCAL_R f1
	xorps xmm1, xmm15
randomx_isn_192:
	; ISTORE L1[r7+537126793], r5
	lea eax, [r15d+537126793]
	and eax, 16376
	mov qword ptr [rsi+rax], r13
randomx_isn_193:
	; IADD_RS r7, r1, SHFT 3
	lea r15, [r15+r9*8]
randomx_isn_194:
	; FDIV_M e1, L1[r7+1919976255]
	lea eax, [r15d+1919976255]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	andps xmm12, xmm13
	orps xmm12, xmm14
	divpd xmm5, xmm12
randomx_isn_195:
	; IADD_RS r5, r7, -837273506, SHFT 2
	lea r13, [r13+r15*4-837273506]
randomx_isn_196:
	; IMUL_R r2, r1
	imul r10, r9
randomx_isn_197:
	; ISTORE L1[r3-403444018], r0
	lea eax, [r11d-403444018]
	and eax, 16376
	mov qword ptr [rsi+rax], r8
randomx_isn_198:
	; IADD_RS r4, r5, SHFT 3
	lea r12, [r12+r13*8]
randomx_isn_199:
	; FMUL_R e2, a1
	mulpd xmm6, xmm9
randomx_isn_200:
	; FADD_R f0, a0
	addpd xmm0, xmm8
randomx_isn_201:
	; ISUB_R r3, r6
	sub r11, r14
randomx_isn_202:
	; FSUB_M f1, L1[r6-819374177]
	lea eax, [r14d-819374177]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm1, xmm12
randomx_isn_203:
	; CBRANCH r3, -1519877904, COND 8
	add r11, -1519845136
	test r11, 16711680
	jz randomx_isn_202
randomx_isn_204:
	; FADD_M f3, L1[r3+1945283935]
	lea eax, [r11d+1945283935]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm3, xmm12
randomx_isn_205:
	; IMUL_R r7, r4
	imul r15, r12
randomx_isn_206:
	; ISWAP_R r5, r4
	xchg r13, r12
randomx_isn_207:
	; IROR_R r1, r0
	mov ecx, r8d
	ror r9, cl
randomx_isn_208:
	; FSUB_R f0, a0
	subpd xmm0, xmm8
randomx_isn_209:
	; IXOR_M r3, L1[r2-287183674]
	lea eax, [r10d-287183674]
	and eax, 16376
	xor r11, qword ptr [rsi+rax]
randomx_isn_210:
	; IMUL_R r6, r0
	imul r14, r8
randomx_isn_211:
	; CBRANCH r6, -966126914, COND 13
	add r14, -966126914
	test r14, 534773760
	jz randomx_isn_211
randomx_isn_212:
	; IXOR_R r0, r1
	xor r8, r9
randomx_isn_213:
	; IADD_M r3, L2[r6-2121250186]
	lea eax, [r14d-2121250186]
	and eax, 262136
	add r11, qword ptr [rsi+rax]
randomx_isn_214:
	; FMUL_R e1, a0
	mulpd xmm5, xmm8
randomx_isn_215:
	; FMUL_R e1, a3
	mulpd xmm5, xmm11
randomx_isn_216:
	; IXOR_M r3, L1[r0-1234645306]
	lea eax, [r8d-1234645306]
	and eax, 16376
	xor r11, qword ptr [rsi+rax]
randomx_isn_217:
	; ISUB_M r7, L1[r1+2057705359]
	lea eax, [r9d+2057705359]
	and eax, 16376
	sub r15, qword ptr [rsi+rax]
randomx_isn_218:
	; CBRANCH r2, -908784770, COND 0
	add r10, -908784770
	test r10, 65280
	jz randomx_isn_212
randomx_isn_219:
	; IROR_R r4, r0
	mov ecx, r8d
	ror r12, cl
randomx_isn_220:
	; IADD_RS r2, r5, SHFT 0
	lea r10, [r10+r13*1]
randomx_isn_221:
	; FMUL_R e1, a3
	mulpd xmm5, xmm11
randomx_isn_222:
	; FADD_R f0, a0
	addpd xmm0, xmm8
randomx_isn_223:
	; IADD_RS r1, r5, SHFT 2
	lea r9, [r9+r13*4]
randomx_isn_224:
	; FSUB_R f1, a0
	subpd xmm1, xmm8
randomx_isn_225:
	; FSUB_R f2, a3
	subpd xmm2, xmm11
randomx_isn_226:
	; ISTORE L1[r0+351187172], r6
	lea eax, [r8d+351187172]
	and eax, 16376
	mov qword ptr [rsi+rax], r14
randomx_isn_227:
	; FSQRT_R e3
	sqrtpd xmm7, xmm7
randomx_isn_228:
	; CBRANCH r7, 1435877143, COND 3
	add r15, 1435876119
	test r15, 522240
	jz randomx_isn_219
randomx_isn_229:
	; IROR_R r5, r4
	mov ecx, r12d
	ror r13, cl
randomx_isn_230:
	; IADD_M r5, L1[r2-914219843]
	lea eax, [r10d-914219843]
	and eax, 16376
	add r13, qword ptr [rsi+rax]
randomx_isn_231:
	; FSCAL_R f3
	xorps xmm3, xmm15
randomx_isn_232:
	; IADD_RS r3, r7, SHFT 3
	lea r11, [r11+r15*8]
randomx_isn_233:
	; IXOR_R r0, r1
	xor r8, r9
randomx_isn_234:
	; IADD_RS r3, r5, SHFT 1
	lea r11, [r11+r13*2]
randomx_isn_235:
	; CBRANCH r3, 805071420, COND 9
	add r11, 805202492
	test r11, 33423360
	jz randomx_isn_235
randomx_isn_236:
	; ISTORE L2[r1-1424908761], r5
	lea eax, [r9d-1424908761]
	and eax, 262136
	mov qword ptr [rsi+rax], r13
randomx_isn_237:
	; ISUB_R r7, r3
	sub r15, r11
randomx_isn_238:
	; IADD_RS r6, r3, SHFT 1
	lea r14, [r14+r11*2]
randomx_isn_239:
	; ISUB_M r2, L2[r7+1703163549]
	lea eax, [r15d+1703163549]
	and eax, 262136
	sub r10, qword ptr [rsi+rax]
randomx_isn_240:
	; IADD_RS r2, r7, SHFT 2
	lea r10, [r10+r15*4]
randomx_isn_241:
	; FMUL_R e0, a2
	mulpd xmm4, xmm10
randomx_isn_242:
	; IROL_R r6, r0
	mov ecx, r8d
	rol r14, cl
randomx_isn_243:
	; ISUB_R r6, r0
	sub r14, r8
randomx_isn_244:
	; FMUL_R e2, a3
	mulpd xmm6, xmm11
randomx_isn_245:
	; FMUL_R e0, a1
	mulpd xmm4, xmm9
randomx_isn_246:
	; CBRANCH r7, -1953056568, COND 7
	add r15, -1953056568
	test r15, 8355840
	jz randomx_isn_238
randomx_isn_247:
	; FSUB_M f2, L1[r2-853114604]
	lea eax, [r10d-853114604]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm2, xmm12
randomx_isn_248:
	; FADD_R f2, a0
	addpd xmm2, xmm8
randomx_isn_249:
	; ISMULH_R r0, r5
	mov rax, r8
	imul r13
	mov r8, rdx
randomx_isn_250:
	; INEG_R r0
	neg r8
randomx_isn_251:
	; ISTORE L1[r0+466701967], r3
	lea eax, [r8d+466701967]
	and eax, 16376
	mov qword ptr [rsi+rax], r11
randomx_isn_252:
	; IADD_M r6, L1[r3-866791909]
	lea eax, [r11d-866791909]
	and eax, 16376
	add r14, qword ptr [rsi+rax]
randomx_isn_253:
	; ISUB_R r4, r3
	sub r12, r11
randomx_isn_254:
	; IMUL_R r0, -1948340729
	imul r8, -1948340729
randomx_isn_255:
	; IXOR_R r3, r1
	xor r11, r9
