randomx_isn_0:
	; IROR_R r3, 30
	ror r11, 30
randomx_isn_1:
	; FSUB_M f1, L1[r7+1640164717]
	lea eax, [r15d+1640164717]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm1, xmm12
randomx_isn_2:
	; IXOR_R r3, 1860630909
	xor r11, 1860630909
randomx_isn_3:
	; IROR_R r5, r7
	mov ecx, r15d
	ror r13, cl
randomx_isn_4:
	; IXOR_R r3, r4
	xor r11, r12
randomx_isn_5:
	; IROR_R r4, r0
	mov ecx, r8d
	ror r12, cl
randomx_isn_6:
	; FSQRT_R e3
	sqrtpd xmm7, xmm7
randomx_isn_7:
	; ISTORE L1[r0-784322734], r3
	lea eax, [r8d-784322734]
	and eax, 16376
	mov qword ptr [rsi+rax], r11
randomx_isn_8:
	; FMUL_R e1, a1
	mulpd xmm5, xmm9
randomx_isn_9:
	; FMUL_R e0, a1
	mulpd xmm4, xmm9
randomx_isn_10:
	; IMUL_M r2, L1[r1+222715267]
	lea eax, [r9d+222715267]
	and eax, 16376
	imul r10, qword ptr [rsi+rax]
randomx_isn_11:
	; ISTORE L1[r3-2088207007], r1
	lea eax, [r11d-2088207007]
	and eax, 16376
	mov qword ptr [rsi+rax], r9
randomx_isn_12:
	; IMUL_R r7, r4
	imul r15, r12
randomx_isn_13:
	; FSQRT_R e2
	sqrtpd xmm6, xmm6
randomx_isn_14:
	; FSQRT_R e2
	sqrtpd xmm6, xmm6
randomx_isn_15:
	; IADD_RS r6, r2, LSH 1
	lea r14, [r14+r10*8]
randomx_isn_16:
	; FSUB_M f2, L1[r1-1890725713]
	lea eax, [r9d-1890725713]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm2, xmm12
randomx_isn_17:
	; IROR_R r4, r3
	mov ecx, r11d
	ror r12, cl
randomx_isn_18:
	; ISTORE L3[r4+1297827817], r4
	lea eax, [r12d+1297827817]
	and eax, 2097144
	mov qword ptr [rsi+rax], r12
randomx_isn_19:
	; FMUL_R e1, a2
	mulpd xmm5, xmm10
randomx_isn_20:
	; COND_R r6, of(r3, 1593588996), LSH 1
	add r8, 2
	test r8, 254
	jz randomx_isn_0
	xor rcx, rcx
	cmp r11d, 1593588996
	seto cl
	add r14, rcx
randomx_isn_21:
	; IXOR_M r7, L1[r2+1680388681]
	lea eax, [r10d+1680388681]
	and eax, 16376
	xor r15, qword ptr [rsi+rax]
randomx_isn_22:
	; IXOR_M r1, L1[r0+1714608757]
	lea eax, [r8d+1714608757]
	and eax, 16376
	xor r9, qword ptr [rsi+rax]
randomx_isn_23:
	; FMUL_R e2, a0
	mulpd xmm6, xmm8
randomx_isn_24:
	; COND_R r6, no(r0, 149087159), LSH 6
	add r8, 64
	test r8, 8128
	jz randomx_isn_21
	xor rcx, rcx
	cmp r8d, 149087159
	setno cl
	add r14, rcx
randomx_isn_25:
	; FADD_R f3, a0
	addpd xmm3, xmm8
randomx_isn_26:
	; IADD_RS r7, r0, LSH 3
	lea r15, [r15+r8*8]
randomx_isn_27:
	; IMUL_R r2, r3
	imul r10, r11
randomx_isn_28:
	; IADD_RS r5, r7, 1345488645, LSH 1
	lea r13, [r13+r15*2+1345488645]
randomx_isn_29:
	; ISTORE L2[r6-950233266], r2
	lea eax, [r14d-950233266]
	and eax, 262136
	mov qword ptr [rsi+rax], r10
randomx_isn_30:
	; ISTORE L1[r7-627845430], r5
	lea eax, [r15d-627845430]
	and eax, 16376
	mov qword ptr [rsi+rax], r13
randomx_isn_31:
	; FSUB_R f1, a2
	subpd xmm1, xmm10
randomx_isn_32:
	; IMUL_R r3, r5
	imul r11, r13
randomx_isn_33:
	; IROR_R r1, 20
	ror r9, 20
randomx_isn_34:
	; FSCAL_R f1
	xorps xmm1, xmm15
randomx_isn_35:
	; IMUL_R r6, 835132161
	imul r14, 835132161
randomx_isn_36:
	; IADD_RS r3, r4, LSH 2
	lea r11, [r11+r12*2]
randomx_isn_37:
	; ISUB_M r6, L2[r4+1885029796]
	lea eax, [r12d+1885029796]
	and eax, 262136
	sub r14, qword ptr [rsi+rax]
randomx_isn_38:
	; FSCAL_R f2
	xorps xmm2, xmm15
randomx_isn_39:
	; ISUB_R r5, r0
	sub r13, r8
randomx_isn_40:
	; IMUL_R r7, r2
	imul r15, r10
randomx_isn_41:
	; FMUL_R e1, a0
	mulpd xmm5, xmm8
randomx_isn_42:
	; IXOR_R r5, r0
	xor r13, r8
randomx_isn_43:
	; FSWAP_R e0
	shufpd xmm4, xmm4, 1
randomx_isn_44:
	; FADD_R f1, a2
	addpd xmm1, xmm10
randomx_isn_45:
	; ISTORE L3[r0+1805562386], r5
	lea eax, [r8d+1805562386]
	and eax, 2097144
	mov qword ptr [rsi+rax], r13
randomx_isn_46:
	; IADD_RS r0, r7, LSH 0
	lea r8, [r8+r15*8]
randomx_isn_47:
	; IXOR_R r5, r2
	xor r13, r10
randomx_isn_48:
	; FSUB_R f3, a3
	subpd xmm3, xmm11
randomx_isn_49:
	; FMUL_R e3, a3
	mulpd xmm7, xmm11
randomx_isn_50:
	; FSUB_R f3, a0
	subpd xmm3, xmm8
randomx_isn_51:
	; COND_R r2, be(r3, -1975981803), LSH 7
	add r12, 128
	test r12, 16256
	jz randomx_isn_25
	xor rcx, rcx
	cmp r11d, -1975981803
	setbe cl
	add r10, rcx
randomx_isn_52:
	; IADD_RS r1, r1, LSH 2
	lea r9, [r9+r9*4]
randomx_isn_53:
	; FSUB_R f2, a0
	subpd xmm2, xmm8
randomx_isn_54:
	; COND_R r5, ns(r1, 1917049931), LSH 6
	add r8, 64
	test r8, 8128
	jz randomx_isn_52
	xor rcx, rcx
	cmp r9d, 1917049931
	setns cl
	add r13, rcx
randomx_isn_55:
	; IXOR_R r2, r3
	xor r10, r11
randomx_isn_56:
	; FSCAL_R f0
	xorps xmm0, xmm15
randomx_isn_57:
	; IMUL_R r5, r1
	imul r13, r9
randomx_isn_58:
	; IADD_RS r5, r1, -999103579, LSH 0
	lea r13, [r13+r9*8-999103579]
randomx_isn_59:
	; FMUL_R e2, a2
	mulpd xmm6, xmm10
randomx_isn_60:
	; IROR_R r2, r6
	mov ecx, r14d
	ror r10, cl
randomx_isn_61:
	; IADD_RS r0, r3, LSH 1
	lea r8, [r8+r11*1]
randomx_isn_62:
	; FSQRT_R e3
	sqrtpd xmm7, xmm7
randomx_isn_63:
	; ISUB_M r6, L1[r5-1165860156]
	lea eax, [r13d-1165860156]
	and eax, 16376
	sub r14, qword ptr [rsi+rax]
randomx_isn_64:
	; ISUB_M r5, L2[r7-1323706896]
	lea eax, [r15d-1323706896]
	and eax, 262136
	sub r13, qword ptr [rsi+rax]
randomx_isn_65:
	; IMUL_RCP r5, 2362240456
	mov rax, 16769707400664451577
	imul r13, rax
randomx_isn_66:
	; ISUB_R r4, 841292629
	sub r12, 841292629
randomx_isn_67:
	; IADD_RS r4, r6, LSH 2
	lea r12, [r12+r14*1]
randomx_isn_68:
	; FSUB_M f3, L1[r4+613549729]
	lea eax, [r12d+613549729]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm3, xmm12
randomx_isn_69:
	; IADD_RS r6, r4, LSH 0
	lea r14, [r14+r12*1]
randomx_isn_70:
	; FSUB_M f1, L1[r5+629563256]
	lea eax, [r13d+629563256]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm1, xmm12
randomx_isn_71:
	; FSWAP_R e1
	shufpd xmm5, xmm5, 1
randomx_isn_72:
	; FADD_R f2, a0
	addpd xmm2, xmm8
randomx_isn_73:
	; FMUL_R e0, a0
	mulpd xmm4, xmm8
randomx_isn_74:
	; COND_R r6, ns(r3, -1200328848), LSH 2
	add r9, 4
	test r9, 508
	jz randomx_isn_55
	xor rcx, rcx
	cmp r11d, -1200328848
	setns cl
	add r14, rcx
randomx_isn_75:
	; FMUL_R e0, a3
	mulpd xmm4, xmm11
randomx_isn_76:
	; FDIV_M e3, L1[r4+1170730568]
	lea eax, [r12d+1170730568]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	andps xmm12, xmm13
	orps xmm12, xmm14
	divpd xmm7, xmm12
randomx_isn_77:
	; FADD_R f1, a2
	addpd xmm1, xmm10
randomx_isn_78:
	; FMUL_R e2, a1
	mulpd xmm6, xmm9
randomx_isn_79:
	; IADD_RS r3, r1, LSH 1
	lea r11, [r11+r9*2]
randomx_isn_80:
	; ISTORE L3[r2+1885666804], r4
	lea eax, [r10d+1885666804]
	and eax, 2097144
	mov qword ptr [rsi+rax], r12
randomx_isn_81:
	; IMULH_R r3, r0
	mov rax, r11
	mul r8
	mov r11, rdx
randomx_isn_82:
	; IXOR_R r2, r6
	xor r10, r14
randomx_isn_83:
	; FSUB_R f0, a2
	subpd xmm0, xmm10
randomx_isn_84:
	; ISMULH_R r1, r7
	mov rax, r9
	imul r15
	mov r9, rdx
randomx_isn_85:
	; FSCAL_R f0
	xorps xmm0, xmm15
randomx_isn_86:
	; ISUB_R r7, r3
	sub r15, r11
randomx_isn_87:
	; IXOR_R r4, r2
	xor r12, r10
randomx_isn_88:
	; IMUL_R r1, r3
	imul r9, r11
randomx_isn_89:
	; COND_R r2, no(r0, -122257389), LSH 6
	add r8, 64
	test r8, 8128
	jz randomx_isn_75
	xor rcx, rcx
	cmp r8d, -122257389
	setno cl
	add r10, rcx
randomx_isn_90:
	; ISTORE L1[r5+228116180], r7
	lea eax, [r13d+228116180]
	and eax, 16376
	mov qword ptr [rsi+rax], r15
randomx_isn_91:
	; ISTORE L1[r6+650356254], r5
	lea eax, [r14d+650356254]
	and eax, 16376
	mov qword ptr [rsi+rax], r13
randomx_isn_92:
	; FSUB_R f2, a0
	subpd xmm2, xmm8
randomx_isn_93:
	; FADD_R f0, a1
	addpd xmm0, xmm9
randomx_isn_94:
	; IXOR_R r6, r1
	xor r14, r9
randomx_isn_95:
	; ISUB_R r0, 307094227
	sub r8, 307094227
randomx_isn_96:
	; FSWAP_R e3
	shufpd xmm7, xmm7, 1
randomx_isn_97:
	; IMUL_M r4, L1[r2-30542523]
	lea eax, [r10d-30542523]
	and eax, 16376
	imul r12, qword ptr [rsi+rax]
randomx_isn_98:
	; ISUB_M r0, L1[r2+2144355962]
	lea eax, [r10d+2144355962]
	and eax, 16376
	sub r8, qword ptr [rsi+rax]
randomx_isn_99:
	; IMULH_R r1, r5
	mov rax, r9
	mul r13
	mov r9, rdx
randomx_isn_100:
	; ISTORE L1[r7+818959056], r3
	lea eax, [r15d+818959056]
	and eax, 16376
	mov qword ptr [rsi+rax], r11
randomx_isn_101:
	; ISWAP_R r0, r0
randomx_isn_102:
	; IMUL_R r2, r7
	imul r10, r15
randomx_isn_103:
	; ISUB_R r2, -1777504751
	sub r10, -1777504751
randomx_isn_104:
	; ISTORE L2[r6-2059767784], r7
	lea eax, [r14d-2059767784]
	and eax, 262136
	mov qword ptr [rsi+rax], r15
randomx_isn_105:
	; FADD_R f3, a1
	addpd xmm3, xmm9
randomx_isn_106:
	; FSUB_R f2, a2
	subpd xmm2, xmm10
randomx_isn_107:
	; ISMULH_R r6, r5
	mov rax, r14
	imul r13
	mov r14, rdx
randomx_isn_108:
	; IADD_RS r7, r0, LSH 1
	lea r15, [r15+r8*4]
randomx_isn_109:
	; IMUL_R r6, r5
	imul r14, r13
randomx_isn_110:
	; IMUL_R r5, r1
	imul r13, r9
randomx_isn_111:
	; FADD_M f2, L1[r0-1280829689]
	lea eax, [r8d-1280829689]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm2, xmm12
randomx_isn_112:
	; IADD_RS r0, r3, LSH 0
	lea r8, [r8+r11*2]
randomx_isn_113:
	; IADD_RS r3, r4, LSH 1
	lea r11, [r11+r12*2]
randomx_isn_114:
	; IADD_RS r2, r4, LSH 2
	lea r10, [r10+r12*8]
randomx_isn_115:
	; IMUL_M r7, L1[r2-106928748]
	lea eax, [r10d-106928748]
	and eax, 16376
	imul r15, qword ptr [rsi+rax]
randomx_isn_116:
	; FADD_R f1, a3
	addpd xmm1, xmm11
randomx_isn_117:
	; FSUB_R f2, a2
	subpd xmm2, xmm10
randomx_isn_118:
	; IADD_RS r2, r2, LSH 0
	lea r10, [r10+r10*2]
randomx_isn_119:
	; ISUB_R r7, -342152774
	sub r15, -342152774
randomx_isn_120:
	; IADD_RS r4, r1, LSH 1
	lea r12, [r12+r9*2]
randomx_isn_121:
	; IADD_RS r4, r7, LSH 2
	lea r12, [r12+r15*1]
randomx_isn_122:
	; FSUB_R f0, a1
	subpd xmm0, xmm9
randomx_isn_123:
	; FSQRT_R e0
	sqrtpd xmm4, xmm4
randomx_isn_124:
	; FSUB_R f2, a2
	subpd xmm2, xmm10
randomx_isn_125:
	; ISMULH_M r2, L2[r1+1421890385]
	lea ecx, [r9d+1421890385]
	and ecx, 262136
	mov rax, r10
	imul qword ptr [rsi+rcx]
	mov r10, rdx
randomx_isn_126:
	; FSUB_M f2, L2[r2+875507660]
	lea eax, [r10d+875507660]
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm2, xmm12
randomx_isn_127:
	; IMUL_R r2, r4
	imul r10, r12
randomx_isn_128:
	; FSUB_R f3, a1
	subpd xmm3, xmm9
randomx_isn_129:
	; IADD_RS r1, r2, LSH 2
	lea r9, [r9+r10*2]
randomx_isn_130:
	; FSUB_R f1, a1
	subpd xmm1, xmm9
randomx_isn_131:
	; IMUL_M r2, L1[r3+63855818]
	lea eax, [r11d+63855818]
	and eax, 16376
	imul r10, qword ptr [rsi+rax]
randomx_isn_132:
	; IXOR_M r5, L3[1438200]
	xor r13, qword ptr [rsi+1438200]
randomx_isn_133:
	; FMUL_R e3, a3
	mulpd xmm7, xmm11
randomx_isn_134:
	; IROR_R r5, r1
	mov ecx, r9d
	ror r13, cl
randomx_isn_135:
	; FMUL_R e1, a2
	mulpd xmm5, xmm10
randomx_isn_136:
	; ISUB_R r3, r6
	sub r11, r14
randomx_isn_137:
	; IADD_RS r4, r1, LSH 0
	lea r12, [r12+r9*8]
randomx_isn_138:
	; ISTORE L1[r0+56684410], r0
	lea eax, [r8d+56684410]
	and eax, 16376
	mov qword ptr [rsi+rax], r8
randomx_isn_139:
	; FADD_M f0, L1[r5+195344615]
	lea eax, [r13d+195344615]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm0, xmm12
randomx_isn_140:
	; ISUB_R r7, r3
	sub r15, r11
randomx_isn_141:
	; IROR_R r3, r2
	mov ecx, r10d
	ror r11, cl
randomx_isn_142:
	; FADD_R f1, a0
	addpd xmm1, xmm8
randomx_isn_143:
	; COND_R r5, ge(r1, 880467599), LSH 2
	add r14, 4
	test r14, 508
	jz randomx_isn_110
	xor rcx, rcx
	cmp r9d, 880467599
	setge cl
	add r13, rcx
randomx_isn_144:
	; FSUB_M f1, L1[r5+1283529302]
	lea eax, [r13d+1283529302]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm1, xmm12
randomx_isn_145:
	; ISUB_R r5, r3
	sub r13, r11
randomx_isn_146:
	; IADD_RS r0, r3, LSH 1
	lea r8, [r8+r11*4]
randomx_isn_147:
	; IADD_RS r1, r3, LSH 1
	lea r9, [r9+r11*1]
randomx_isn_148:
	; FSQRT_R e1
	sqrtpd xmm5, xmm5
randomx_isn_149:
	; IADD_RS r4, r3, LSH 1
	lea r12, [r12+r11*2]
randomx_isn_150:
	; FADD_M f1, L1[r0-1977073973]
	lea eax, [r8d-1977073973]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm1, xmm12
randomx_isn_151:
	; IADD_RS r1, r0, LSH 1
	lea r9, [r9+r8*2]
randomx_isn_152:
	; FSUB_R f1, a0
	subpd xmm1, xmm8
randomx_isn_153:
	; FSUB_R f0, a1
	subpd xmm0, xmm9
randomx_isn_154:
	; IMUL_R r2, r6
	imul r10, r14
randomx_isn_155:
	; CFROUND r3, 46
	mov rax, r11
	rol rax, 31
	and eax, 24576
	or eax, 40896
	mov dword ptr [rsp-8], eax
	ldmxcsr dword ptr [rsp-8]
randomx_isn_156:
	; FSUB_R f3, a2
	subpd xmm3, xmm10
randomx_isn_157:
	; ISTORE L1[r1+883418866], r1
	lea eax, [r9d+883418866]
	and eax, 16376
	mov qword ptr [rsi+rax], r9
randomx_isn_158:
	; ISTORE L1[r6-312458782], r4
	lea eax, [r14d-312458782]
	and eax, 16376
	mov qword ptr [rsi+rax], r12
randomx_isn_159:
	; IADD_RS r7, r2, LSH 3
	lea r15, [r15+r10*4]
randomx_isn_160:
	; IMUL_RCP r7, 2040763167
	mov rax, 9705702723791900149
	imul r15, rax
randomx_isn_161:
	; FADD_R f3, a3
	addpd xmm3, xmm11
randomx_isn_162:
	; IADD_RS r6, r4, LSH 1
	lea r14, [r14+r12*2]
randomx_isn_163:
	; ISWAP_R r3, r5
	xchg r11, r13
randomx_isn_164:
	; FSQRT_R e3
	sqrtpd xmm7, xmm7
randomx_isn_165:
	; FSUB_R f1, a2
	subpd xmm1, xmm10
randomx_isn_166:
	; IROR_R r5, r3
	mov ecx, r11d
	ror r13, cl
randomx_isn_167:
	; ISUB_M r2, L2[r0+805006473]
	lea eax, [r8d+805006473]
	and eax, 262136
	sub r10, qword ptr [rsi+rax]
randomx_isn_168:
	; FDIV_M e0, L1[r4-2098372994]
	lea eax, [r12d-2098372994]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	andps xmm12, xmm13
	orps xmm12, xmm14
	divpd xmm4, xmm12
randomx_isn_169:
	; ISUB_M r3, L2[r4+1773188989]
	lea eax, [r12d+1773188989]
	and eax, 262136
	sub r11, qword ptr [rsi+rax]
randomx_isn_170:
	; FADD_R f0, a3
	addpd xmm0, xmm11
randomx_isn_171:
	; FADD_R f1, a0
	addpd xmm1, xmm8
randomx_isn_172:
	; ISTORE L1[r7-933780249], r6
	lea eax, [r15d-933780249]
	and eax, 16376
	mov qword ptr [rsi+rax], r14
randomx_isn_173:
	; FSUB_M f0, L1[r7-1204687701]
	lea eax, [r15d-1204687701]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm0, xmm12
randomx_isn_174:
	; ISWAP_R r3, r0
	xchg r11, r8
randomx_isn_175:
	; IMULH_R r0, r3
	mov rax, r8
	mul r11
	mov r8, rdx
randomx_isn_176:
	; IMUL_M r2, L3[1439696]
	imul r10, qword ptr [rsi+1439696]
randomx_isn_177:
	; IMUL_M r3, L3[232968]
	imul r11, qword ptr [rsi+232968]
randomx_isn_178:
	; IADD_RS r5, r3, -2108568616, LSH 1
	lea r13, [r13+r11*2-2108568616]
randomx_isn_179:
	; IADD_M r3, L1[r4+1322108729]
	lea eax, [r12d+1322108729]
	and eax, 16376
	add r11, qword ptr [rsi+rax]
randomx_isn_180:
	; FADD_R f3, a1
	addpd xmm3, xmm9
randomx_isn_181:
	; FSQRT_R e3
	sqrtpd xmm7, xmm7
randomx_isn_182:
	; FMUL_R e2, a2
	mulpd xmm6, xmm10
randomx_isn_183:
	; IADD_RS r6, r2, LSH 0
	lea r14, [r14+r10*8]
randomx_isn_184:
	; FADD_R f2, a3
	addpd xmm2, xmm11
randomx_isn_185:
	; FSWAP_R f3
	shufpd xmm3, xmm3, 1
randomx_isn_186:
	; FSCAL_R f3
	xorps xmm3, xmm15
randomx_isn_187:
	; IADD_RS r6, r6, LSH 3
	lea r14, [r14+r14*4]
randomx_isn_188:
	; FSCAL_R f2
	xorps xmm2, xmm15
randomx_isn_189:
	; IMUL_M r4, L1[r5+1014226422]
	lea eax, [r13d+1014226422]
	and eax, 16376
	imul r12, qword ptr [rsi+rax]
randomx_isn_190:
	; FSUB_M f2, L1[r3-87032284]
	lea eax, [r11d-87032284]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm2, xmm12
randomx_isn_191:
	; IMUL_M r4, L2[r3+1279913094]
	lea eax, [r11d+1279913094]
	and eax, 262136
	imul r12, qword ptr [rsi+rax]
randomx_isn_192:
	; ISUB_R r7, r3
	sub r15, r11
randomx_isn_193:
	; ISTORE L1[r1+901530824], r1
	lea eax, [r9d+901530824]
	and eax, 16376
	mov qword ptr [rsi+rax], r9
randomx_isn_194:
	; ISTORE L1[r3+965438117], r4
	lea eax, [r11d+965438117]
	and eax, 16376
	mov qword ptr [rsi+rax], r12
randomx_isn_195:
	; FMUL_R e2, a2
	mulpd xmm6, xmm10
randomx_isn_196:
	; FMUL_R e2, a0
	mulpd xmm6, xmm8
randomx_isn_197:
	; FADD_M f2, L2[r0-197871122]
	lea eax, [r8d-197871122]
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm2, xmm12
randomx_isn_198:
	; FADD_R f1, a2
	addpd xmm1, xmm10
randomx_isn_199:
	; FSUB_R f3, a3
	subpd xmm3, xmm11
randomx_isn_200:
	; IADD_RS r2, r5, LSH 2
	lea r10, [r10+r13*1]
randomx_isn_201:
	; ISUB_M r6, L2[r3+376384700]
	lea eax, [r11d+376384700]
	and eax, 262136
	sub r14, qword ptr [rsi+rax]
randomx_isn_202:
	; ISWAP_R r3, r6
	xchg r11, r14
randomx_isn_203:
	; ISTORE L1[r1+330228321], r3
	lea eax, [r9d+330228321]
	and eax, 16376
	mov qword ptr [rsi+rax], r11
randomx_isn_204:
	; IMUL_R r6, r1
	imul r14, r9
randomx_isn_205:
	; IADD_M r7, L2[r5-579800039]
	lea eax, [r13d-579800039]
	and eax, 262136
	add r15, qword ptr [rsi+rax]
randomx_isn_206:
	; IADD_RS r3, r5, LSH 0
	lea r11, [r11+r13*1]
randomx_isn_207:
	; FSCAL_R f1
	xorps xmm1, xmm15
randomx_isn_208:
	; IADD_RS r6, r3, LSH 1
	lea r14, [r14+r11*1]
randomx_isn_209:
	; FSUB_M f0, L1[r4-557177119]
	lea eax, [r12d-557177119]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm0, xmm12
randomx_isn_210:
	; FSWAP_R e2
	shufpd xmm6, xmm6, 1
randomx_isn_211:
	; FMUL_R e2, a3
	mulpd xmm6, xmm11
randomx_isn_212:
	; IMUL_M r0, L1[r1-300353849]
	lea eax, [r9d-300353849]
	and eax, 16376
	imul r8, qword ptr [rsi+rax]
randomx_isn_213:
	; FSUB_M f2, L1[r5-2016234225]
	lea eax, [r13d-2016234225]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm2, xmm12
randomx_isn_214:
	; FMUL_R e1, a2
	mulpd xmm5, xmm10
randomx_isn_215:
	; FADD_R f3, a1
	addpd xmm3, xmm9
randomx_isn_216:
	; IXOR_M r4, L2[r1+926150064]
	lea eax, [r9d+926150064]
	and eax, 262136
	xor r12, qword ptr [rsi+rax]
randomx_isn_217:
	; IMUL_M r6, L1[r5-1692567271]
	lea eax, [r13d-1692567271]
	and eax, 16376
	imul r14, qword ptr [rsi+rax]
randomx_isn_218:
	; FSCAL_R f2
	xorps xmm2, xmm15
randomx_isn_219:
	; FADD_M f3, L1[r7+162195095]
	lea eax, [r15d+162195095]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm3, xmm12
randomx_isn_220:
	; FSUB_R f0, a0
	subpd xmm0, xmm8
randomx_isn_221:
	; IMUL_R r1, r0
	imul r9, r8
randomx_isn_222:
	; IADD_RS r1, r0, LSH 2
	lea r9, [r9+r8*4]
randomx_isn_223:
	; FSCAL_R f2
	xorps xmm2, xmm15
randomx_isn_224:
	; IADD_RS r5, r4, 312567979, LSH 1
	lea r13, [r13+r12*4+312567979]
randomx_isn_225:
	; ISTORE L2[r2+260885699], r1
	lea eax, [r10d+260885699]
	and eax, 262136
	mov qword ptr [rsi+rax], r9
randomx_isn_226:
	; ISUB_R r6, -791575725
	sub r14, -791575725
randomx_isn_227:
	; FDIV_M e3, L1[r0-1140408845]
	lea eax, [r8d-1140408845]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	andps xmm12, xmm13
	orps xmm12, xmm14
	divpd xmm7, xmm12
randomx_isn_228:
	; IXOR_R r7, r1
	xor r15, r9
randomx_isn_229:
	; ISWAP_R r0, r6
	xchg r8, r14
randomx_isn_230:
	; IADD_RS r2, r7, LSH 2
	lea r10, [r10+r15*1]
randomx_isn_231:
	; FMUL_R e1, a0
	mulpd xmm5, xmm8
randomx_isn_232:
	; FMUL_R e3, a3
	mulpd xmm7, xmm11
randomx_isn_233:
	; FMUL_R e0, a2
	mulpd xmm4, xmm10
randomx_isn_234:
	; IADD_M r2, L1[r7+1435646464]
	lea eax, [r15d+1435646464]
	and eax, 16376
	add r10, qword ptr [rsi+rax]
randomx_isn_235:
	; ISWAP_R r7, r6
	xchg r15, r14
randomx_isn_236:
	; FMUL_R e3, a2
	mulpd xmm7, xmm10
randomx_isn_237:
	; FSUB_R f1, a3
	subpd xmm1, xmm11
randomx_isn_238:
	; IADD_RS r4, r2, LSH 1
	lea r12, [r12+r10*4]
randomx_isn_239:
	; IMUL_RCP r7, 3065786637
	mov rax, 12921343181238534701
	imul r15, rax
randomx_isn_240:
	; IMUL_R r5, r7
	imul r13, r15
randomx_isn_241:
	; IROR_R r6, r5
	mov ecx, r13d
	ror r14, cl
randomx_isn_242:
	; IMUL_R r6, r4
	imul r14, r12
randomx_isn_243:
	; FSUB_R f0, a3
	subpd xmm0, xmm11
randomx_isn_244:
	; FADD_M f1, L1[r0-389606015]
	lea eax, [r8d-389606015]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm1, xmm12
randomx_isn_245:
	; INEG_R r3
	neg r11
randomx_isn_246:
	; ISUB_M r7, L2[r2+1938400676]
	lea eax, [r10d+1938400676]
	and eax, 262136
	sub r15, qword ptr [rsi+rax]
randomx_isn_247:
	; COND_R r2, be(r5, -8545330), LSH 2
	add r9, 4
	test r9, 508
	jz randomx_isn_223
	xor rcx, rcx
	cmp r13d, -8545330
	setbe cl
	add r10, rcx
randomx_isn_248:
	; ISTORE L1[r0+1951752498], r5
	lea eax, [r8d+1951752498]
	and eax, 16376
	mov qword ptr [rsi+rax], r13
randomx_isn_249:
	; IADD_RS r6, r5, LSH 2
	lea r14, [r14+r13*4]
randomx_isn_250:
	; FADD_R f3, a0
	addpd xmm3, xmm8
randomx_isn_251:
	; IADD_RS r0, r0, LSH 0
	lea r8, [r8+r8*4]
randomx_isn_252:
	; ISUB_R r4, r2
	sub r12, r10
randomx_isn_253:
	; IADD_RS r5, r4, 256175395, LSH 0
	lea r13, [r13+r12*4+256175395]
randomx_isn_254:
	; IADD_RS r6, r7, LSH 2
	lea r14, [r14+r15*4]
randomx_isn_255:
	; IROR_R r7, r3
	mov ecx, r11d
	ror r15, cl
