randomx_isn_0:
	; ISWAP_R r3, r3
randomx_isn_1:
	; FSCAL_R f1
	xorps xmm1, xmm15
randomx_isn_2:
	; IXOR_M r3, L3[457080]
	xor r11, qword ptr [rsi+457080]
randomx_isn_3:
	; IROR_R r5, r7
	mov ecx, r15d
	ror r13, cl
randomx_isn_4:
	; IXOR_R r3, r4
	xor r11, r12
randomx_isn_5:
	; ISWAP_R r4, r0
	xchg r12, r8
randomx_isn_6:
	; CBRANCH -188214077, COND 5
	add r9, -188209981
	test r9, 2088960
	jz randomx_isn_0
randomx_isn_7:
	; ISTORE L3[r0-784322734], r3
	lea eax, [r8d-784322734]
	and eax, 2097144
	mov qword ptr [rsi+rax], r11
randomx_isn_8:
	; FMUL_R e1, a1
	mulpd xmm5, xmm9
randomx_isn_9:
	; FDIV_M e0, L1[r5+1118107460]
	lea eax, [r13d+1118107460]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	andps xmm12, xmm13
	orps xmm12, xmm14
	divpd xmm4, xmm12
randomx_isn_10:
	; ISMULH_R r2, r1
	mov rax, r10
	imul r9
	mov r10, rdx
randomx_isn_11:
	; ISTORE L1[r3-2088207007], r1
	lea eax, [r11d-2088207007]
	and eax, 16376
	mov qword ptr [rsi+rax], r9
randomx_isn_12:
	; IMUL_R r7, r4
	imul r15, r12
randomx_isn_13:
	; CBRANCH 179989705, COND 3
	add r8, 179988681
	test r8, 522240
	jz randomx_isn_7
randomx_isn_14:
	; CBRANCH 1801296358, COND 3
	add r10, 1801296358
	test r10, 522240
	jz randomx_isn_14
randomx_isn_15:
	; IADD_RS r6, r2, SHFT 3
	lea r14, [r14+r10*8]
randomx_isn_16:
	; FSCAL_R f2
	xorps xmm2, xmm15
randomx_isn_17:
	; IROR_R r4, r3
	mov ecx, r11d
	ror r12, cl
randomx_isn_18:
	; ISTORE L1[r4+1297827817], r4
	lea eax, [r12d+1297827817]
	and eax, 16376
	mov qword ptr [rsi+rax], r12
randomx_isn_19:
	; FMUL_R e1, a2
	mulpd xmm5, xmm10
randomx_isn_20:
	; CBRANCH 1593588996, COND 3
	add r11, 1593587972
	test r11, 522240
	jz randomx_isn_15
randomx_isn_21:
	; IROR_R r7, r2
	mov ecx, r10d
	ror r15, cl
randomx_isn_22:
	; IROR_R r1, r0
	mov ecx, r8d
	ror r9, cl
randomx_isn_23:
	; FDIV_M e2, L1[r0+1284530872]
	lea eax, [r8d+1284530872]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	andps xmm12, xmm13
	orps xmm12, xmm14
	divpd xmm6, xmm12
randomx_isn_24:
	; CBRANCH 149087159, COND 13
	add r12, 149087159
	test r12, 534773760
	jz randomx_isn_21
randomx_isn_25:
	; FADD_R f3, a0
	addpd xmm3, xmm8
randomx_isn_26:
	; IADD_RS r7, r0, SHFT 2
	lea r15, [r15+r8*4]
randomx_isn_27:
	; IMUL_M r2, L1[r3-487007545]
	lea eax, [r11d-487007545]
	and eax, 16376
	imul r10, qword ptr [rsi+rax]
randomx_isn_28:
	; IADD_RS r5, r7, 1345488645, SHFT 3
	lea r13, [r13+r15*8+1345488645]
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
	; FSUB_M f1, L1[r2-531715197]
	lea eax, [r10d-531715197]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm1, xmm12
randomx_isn_32:
	; IMULH_R r3, r5
	mov rax, r11
	mul r13
	mov r11, rdx
randomx_isn_33:
	; ISWAP_R r1, r1
randomx_isn_34:
	; FMUL_R e1, a1
	mulpd xmm5, xmm9
randomx_isn_35:
	; IMULH_R r6, r6
	mov rax, r14
	mul r14
	mov r14, rdx
randomx_isn_36:
	; IADD_RS r3, r4, SHFT 0
	lea r11, [r11+r12*1]
randomx_isn_37:
	; IMUL_R r6, r4
	imul r14, r12
randomx_isn_38:
	; FMUL_R e2, a2
	mulpd xmm6, xmm10
randomx_isn_39:
	; ISUB_M r5, L1[r0+1743542437]
	lea eax, [r8d+1743542437]
	and eax, 16376
	sub r13, qword ptr [rsi+rax]
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
	; FADD_R f0, a2
	addpd xmm0, xmm10
randomx_isn_44:
	; FADD_R f1, a2
	addpd xmm1, xmm10
randomx_isn_45:
	; ISTORE L1[r0+1805562386], r5
	lea eax, [r8d+1805562386]
	and eax, 16376
	mov qword ptr [rsi+rax], r13
randomx_isn_46:
	; IADD_RS r0, r7, SHFT 0
	lea r8, [r8+r15*1]
randomx_isn_47:
	; IXOR_R r5, r2
	xor r13, r10
randomx_isn_48:
	; FSUB_M f3, L1[r3+670987345]
	lea eax, [r11d+670987345]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm3, xmm12
randomx_isn_49:
	; FDIV_M e3, L1[r3+306555937]
	lea eax, [r11d+306555937]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	andps xmm12, xmm13
	orps xmm12, xmm14
	divpd xmm7, xmm12
randomx_isn_50:
	; FSUB_R f3, a0
	subpd xmm3, xmm8
randomx_isn_51:
	; CBRANCH -1975981803, COND 14
	add r9, -1973884651
	test r9, 1069547520
	jz randomx_isn_25
randomx_isn_52:
	; IADD_M r1, L3[1622792]
	add r9, qword ptr [rsi+1622792]
randomx_isn_53:
	; FSUB_R f2, a0
	subpd xmm2, xmm8
randomx_isn_54:
	; CBRANCH 1917049931, COND 12
	add r13, 1918098507
	test r13, 267386880
	jz randomx_isn_52
randomx_isn_55:
	; IXOR_R r2, r3
	xor r10, r11
randomx_isn_56:
	; FMUL_R e0, a2
	mulpd xmm4, xmm10
randomx_isn_57:
	; IMUL_R r5, r1
	imul r13, r9
randomx_isn_58:
	; IADD_RS r5, r1, -999103579, SHFT 0
	lea r13, [r13+r9*1-999103579]
randomx_isn_59:
	; FSQRT_R e2
	sqrtpd xmm6, xmm6
randomx_isn_60:
	; IROR_R r2, r6
	mov ecx, r14d
	ror r10, cl
randomx_isn_61:
	; IADD_M r0, L1[r3+553576025]
	lea eax, [r11d+553576025]
	and eax, 16376
	add r8, qword ptr [rsi+rax]
randomx_isn_62:
	; CBRANCH 1111898647, COND 1
	add r14, 1111898647
	test r14, 130560
	jz randomx_isn_55
randomx_isn_63:
	; IMUL_R r6, r5
	imul r14, r13
randomx_isn_64:
	; IMUL_R r5, r7
	imul r13, r15
randomx_isn_65:
	; INEG_R r5
	neg r13
randomx_isn_66:
	; ISUB_R r4, 841292629
	sub r12, 841292629
randomx_isn_67:
	; IADD_RS r4, r6, SHFT 3
	lea r12, [r12+r14*8]
randomx_isn_68:
	; FSCAL_R f3
	xorps xmm3, xmm15
randomx_isn_69:
	; IADD_M r6, L2[r4-1863144764]
	lea eax, [r12d-1863144764]
	and eax, 262136
	add r14, qword ptr [rsi+rax]
randomx_isn_70:
	; FMUL_R e1, a1
	mulpd xmm5, xmm9
randomx_isn_71:
	; FADD_R f1, a3
	addpd xmm1, xmm11
randomx_isn_72:
	; FADD_R f2, a0
	addpd xmm2, xmm8
randomx_isn_73:
	; FMUL_R e0, a0
	mulpd xmm4, xmm8
randomx_isn_74:
	; CBRANCH -1200328848, COND 4
	add r15, -1200326800
	test r15, 1044480
	jz randomx_isn_63
randomx_isn_75:
	; FSQRT_R e0
	sqrtpd xmm4, xmm4
randomx_isn_76:
	; FSQRT_R e3
	sqrtpd xmm7, xmm7
randomx_isn_77:
	; FADD_R f1, a2
	addpd xmm1, xmm10
randomx_isn_78:
	; FMUL_R e2, a1
	mulpd xmm6, xmm9
randomx_isn_79:
	; IADD_M r3, L1[r1-919815727]
	lea eax, [r9d-919815727]
	and eax, 16376
	add r11, qword ptr [rsi+rax]
randomx_isn_80:
	; ISTORE L1[r2+1885666804], r4
	lea eax, [r10d+1885666804]
	and eax, 16376
	mov qword ptr [rsi+rax], r12
randomx_isn_81:
	; IMUL_RCP r3, 4074670449
	mov rax, 9722033168806130558
	imul r11, rax
randomx_isn_82:
	; IXOR_R r2, r6
	xor r10, r14
randomx_isn_83:
	; FSUB_M f0, L1[r2-528310755]
	lea eax, [r10d-528310755]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm0, xmm12
randomx_isn_84:
	; IMUL_RCP r1, 3960817810
	mov rax, 10001490388453936182
	imul r9, rax
randomx_isn_85:
	; FMUL_R e0, a2
	mulpd xmm4, xmm10
randomx_isn_86:
	; ISUB_R r7, r3
	sub r15, r11
randomx_isn_87:
	; IXOR_R r4, r2
	xor r12, r10
randomx_isn_88:
	; IMUL_M r1, L1[r3+1489284408]
	lea eax, [r11d+1489284408]
	and eax, 16376
	imul r9, qword ptr [rsi+rax]
randomx_isn_89:
	; CBRANCH -122257389, COND 13
	add r8, -123305965
	test r8, 534773760
	jz randomx_isn_75
randomx_isn_90:
	; ISTORE L1[r5+228116180], r7
	lea eax, [r13d+228116180]
	and eax, 16376
	mov qword ptr [rsi+rax], r15
randomx_isn_91:
	; ISTORE L3[r6+650356254], r5
	lea eax, [r14d+650356254]
	and eax, 2097144
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
	; ISUB_M r0, L3[910032]
	sub r8, qword ptr [rsi+910032]
randomx_isn_96:
	; FADD_R f3, a2
	addpd xmm3, xmm10
randomx_isn_97:
	; IMULH_M r4, L1[r2-30542523]
	lea ecx, [r10d-30542523]
	and ecx, 16376
	mov rax, r12
	mul qword ptr [rsi+rcx]
	mov r12, rdx
randomx_isn_98:
	; IMUL_R r0, r2
	imul r8, r10
randomx_isn_99:
	; ISMULH_R r1, r5
	mov rax, r9
	imul r13
	mov r9, rdx
randomx_isn_100:
	; ISTORE L1[r7+818959056], r3
	lea eax, [r15d+818959056]
	and eax, 16376
	mov qword ptr [rsi+rax], r11
randomx_isn_101:
	; FSWAP_R f0
	shufpd xmm0, xmm0, 1
randomx_isn_102:
	; IMULH_R r2, r7
	mov rax, r10
	mul r15
	mov r10, rdx
randomx_isn_103:
	; ISUB_R r2, -1777504751
	sub r10, -1777504751
randomx_isn_104:
	; ISTORE L2[r6-2059767784], r7
	lea eax, [r14d-2059767784]
	and eax, 262136
	mov qword ptr [rsi+rax], r15
randomx_isn_105:
	; FSUB_R f3, a1
	subpd xmm3, xmm9
randomx_isn_106:
	; FSUB_M f2, L2[r6+1023745039]
	lea eax, [r14d+1023745039]
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm2, xmm12
randomx_isn_107:
	; IMUL_RCP r6, 1839867980
	mov rax, 10765468416144773821
	imul r14, rax
randomx_isn_108:
	; IADD_RS r7, r0, SHFT 2
	lea r15, [r15+r8*4]
randomx_isn_109:
	; IMULH_R r6, r5
	mov rax, r14
	mul r13
	mov r14, rdx
randomx_isn_110:
	; IMUL_R r5, r1
	imul r13, r9
randomx_isn_111:
	; FSUB_R f2, a0
	subpd xmm2, xmm8
randomx_isn_112:
	; IADD_RS r0, r3, SHFT 1
	lea r8, [r8+r11*2]
randomx_isn_113:
	; IADD_M r3, L1[r4-1138304368]
	lea eax, [r12d-1138304368]
	and eax, 16376
	add r11, qword ptr [rsi+rax]
randomx_isn_114:
	; IADD_RS r2, r4, SHFT 1
	lea r10, [r10+r12*2]
randomx_isn_115:
	; IMULH_M r7, L1[r2-106928748]
	lea ecx, [r10d-106928748]
	and ecx, 16376
	mov rax, r15
	mul qword ptr [rsi+rcx]
	mov r15, rdx
randomx_isn_116:
	; FADD_M f1, L1[r3+1652235318]
	lea eax, [r11d+1652235318]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm1, xmm12
randomx_isn_117:
	; FSUB_R f2, a2
	subpd xmm2, xmm10
randomx_isn_118:
	; IADD_RS r2, r2, SHFT 3
	lea r10, [r10+r10*8]
randomx_isn_119:
	; ISUB_M r7, L3[1780152]
	sub r15, qword ptr [rsi+1780152]
randomx_isn_120:
	; IADD_RS r4, r1, SHFT 0
	lea r12, [r12+r9*1]
randomx_isn_121:
	; IADD_RS r4, r7, SHFT 3
	lea r12, [r12+r15*8]
randomx_isn_122:
	; FSUB_R f0, a1
	subpd xmm0, xmm9
randomx_isn_123:
	; CBRANCH 269211216, COND 3
	add r9, 269212240
	test r9, 522240
	jz randomx_isn_100
randomx_isn_124:
	; FSUB_M f2, L1[r6-1615966581]
	lea eax, [r14d-1615966581]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm2, xmm12
randomx_isn_125:
	; IMUL_RCP r2, 1421890385
	mov rax, 13930075649654304680
	imul r10, rax
randomx_isn_126:
	; FSCAL_R f2
	xorps xmm2, xmm15
randomx_isn_127:
	; IMUL_M r2, L1[r4+48032324]
	lea eax, [r12d+48032324]
	and eax, 16376
	imul r10, qword ptr [rsi+rax]
randomx_isn_128:
	; FSUB_R f3, a1
	subpd xmm3, xmm9
randomx_isn_129:
	; IADD_M r1, L1[r2+697183462]
	lea eax, [r10d+697183462]
	and eax, 16376
	add r9, qword ptr [rsi+rax]
randomx_isn_130:
	; FSUB_R f1, a1
	subpd xmm1, xmm9
randomx_isn_131:
	; ISMULH_R r2, r3
	mov rax, r10
	imul r11
	mov r10, rdx
randomx_isn_132:
	; IROR_R r5, 59
	ror r13, 59
randomx_isn_133:
	; FDIV_M e3, L1[r3-1456190436]
	lea eax, [r11d-1456190436]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	andps xmm12, xmm13
	orps xmm12, xmm14
	divpd xmm7, xmm12
randomx_isn_134:
	; FSWAP_R e1
	shufpd xmm5, xmm5, 1
randomx_isn_135:
	; FMUL_R e1, a2
	mulpd xmm5, xmm10
randomx_isn_136:
	; ISUB_M r3, L2[r6+561570278]
	lea eax, [r14d+561570278]
	and eax, 262136
	sub r11, qword ptr [rsi+rax]
randomx_isn_137:
	; IADD_RS r4, r1, SHFT 1
	lea r12, [r12+r9*2]
randomx_isn_138:
	; ISTORE L1[r0+56684410], r0
	lea eax, [r8d+56684410]
	and eax, 16376
	mov qword ptr [rsi+rax], r8
randomx_isn_139:
	; FSUB_R f0, a1
	subpd xmm0, xmm9
randomx_isn_140:
	; ISUB_M r7, L1[r3-1548418722]
	lea eax, [r11d-1548418722]
	and eax, 16376
	sub r15, qword ptr [rsi+rax]
randomx_isn_141:
	; FSWAP_R f3
	shufpd xmm3, xmm3, 1
randomx_isn_142:
	; FADD_R f1, a0
	addpd xmm1, xmm8
randomx_isn_143:
	; CBRANCH 880467599, COND 5
	add r14, 880471695
	test r14, 2088960
	jz randomx_isn_124
randomx_isn_144:
	; FMUL_R e1, a1
	mulpd xmm5, xmm9
randomx_isn_145:
	; ISUB_R r5, r3
	sub r13, r11
randomx_isn_146:
	; IADD_M r0, L1[r3+1228198394]
	lea eax, [r11d+1228198394]
	and eax, 16376
	add r8, qword ptr [rsi+rax]
randomx_isn_147:
	; IADD_M r1, L1[r3+1747766580]
	lea eax, [r11d+1747766580]
	and eax, 16376
	add r9, qword ptr [rsi+rax]
randomx_isn_148:
	; CBRANCH -1843326985, COND 14
	add r10, -1841229833
	test r10, 1069547520
	jz randomx_isn_144
randomx_isn_149:
	; IADD_RS r4, r3, SHFT 2
	lea r12, [r12+r11*4]
randomx_isn_150:
	; FSUB_R f1, a0
	subpd xmm1, xmm8
randomx_isn_151:
	; IADD_RS r1, r0, SHFT 3
	lea r9, [r9+r8*8]
randomx_isn_152:
	; FSUB_M f1, L2[r4+969560460]
	lea eax, [r12d+969560460]
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm1, xmm12
randomx_isn_153:
	; FSUB_R f0, a1
	subpd xmm0, xmm9
randomx_isn_154:
	; IMULH_R r2, r6
	mov rax, r10
	mul r14
	mov r10, rdx
randomx_isn_155:
	; CFROUND r3, 46
	mov rax, r11
	rol rax, 31
	and eax, 24576
	or eax, 40896
	push rax
	ldmxcsr dword ptr [rsp]
	pop rax
randomx_isn_156:
	; FSUB_M f3, L1[r6+1704814547]
	lea eax, [r14d+1704814547]
	and eax, 16376
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	subpd xmm3, xmm12
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
	; IADD_RS r7, r2, SHFT 2
	lea r15, [r15+r10*4]
randomx_isn_160:
	; IXOR_R r7, r1
	xor r15, r9
randomx_isn_161:
	; FADD_R f3, a3
	addpd xmm3, xmm11
randomx_isn_162:
	; IADD_M r6, L1[r4-783948693]
	lea eax, [r12d-783948693]
	and eax, 16376
	add r14, qword ptr [rsi+rax]
randomx_isn_163:
	; FSWAP_R f3
	shufpd xmm3, xmm3, 1
randomx_isn_164:
	; CBRANCH -2107581963, COND 4
	add r11, -2107584011
	test r11, 1044480
	jz randomx_isn_149
randomx_isn_165:
	; FSUB_R f1, a2
	subpd xmm1, xmm10
randomx_isn_166:
	; FSWAP_R e1
	shufpd xmm5, xmm5, 1
randomx_isn_167:
	; IMUL_R r2, r0
	imul r10, r8
randomx_isn_168:
	; FSQRT_R e0
	sqrtpd xmm4, xmm4
randomx_isn_169:
	; IMUL_R r3, r4
	imul r11, r12
randomx_isn_170:
	; FSUB_R f0, a3
	subpd xmm0, xmm11
randomx_isn_171:
	; FADD_M f1, L2[r4+102970518]
	lea eax, [r12d+102970518]
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm1, xmm12
randomx_isn_172:
	; ISTORE L1[r7-933780249], r6
	lea eax, [r15d-933780249]
	and eax, 16376
	mov qword ptr [rsi+rax], r14
randomx_isn_173:
	; FSCAL_R f0
	xorps xmm0, xmm15
randomx_isn_174:
	; FSWAP_R f3
	shufpd xmm3, xmm3, 1
randomx_isn_175:
	; IMUL_RCP r0, 770576157
	mov rax, 12852098036408674138
	imul r8, rax
randomx_isn_176:
	; ISMULH_R r2, r2
	mov rax, r10
	imul r10
	mov r10, rdx
randomx_isn_177:
	; IMULH_M r3, L3[232968]
	mov rax, r11
	mul qword ptr [rsi+232968]
	mov r11, rdx
randomx_isn_178:
	; IADD_M r5, L1[r3-2108568616]
	lea eax, [r11d-2108568616]
	and eax, 16376
	add r13, qword ptr [rsi+rax]
randomx_isn_179:
	; ISUB_R r3, r4
	sub r11, r12
randomx_isn_180:
	; FSUB_R f3, a1
	subpd xmm3, xmm9
randomx_isn_181:
	; CBRANCH 556152230, COND 12
	add r12, 557200806
	test r12, 267386880
	jz randomx_isn_165
randomx_isn_182:
	; FSQRT_R e2
	sqrtpd xmm6, xmm6
randomx_isn_183:
	; IADD_RS r6, r2, SHFT 1
	lea r14, [r14+r10*2]
randomx_isn_184:
	; FADD_M f2, L2[r3+1545561007]
	lea eax, [r11d+1545561007]
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm2, xmm12
randomx_isn_185:
	; FADD_R f3, a3
	addpd xmm3, xmm11
randomx_isn_186:
	; FMUL_R e3, a3
	mulpd xmm7, xmm11
randomx_isn_187:
	; IADD_M r6, L3[1664992]
	add r14, qword ptr [rsi+1664992]
randomx_isn_188:
	; FMUL_R e2, a1
	mulpd xmm6, xmm9
randomx_isn_189:
	; ISMULH_R r4, r5
	mov rax, r12
	imul r13
	mov r12, rdx
randomx_isn_190:
	; FSCAL_R f2
	xorps xmm2, xmm15
randomx_isn_191:
	; ISMULH_R r4, r3
	mov rax, r12
	imul r11
	mov r12, rdx
randomx_isn_192:
	; ISUB_M r7, L1[r3+1266042411]
	lea eax, [r11d+1266042411]
	and eax, 16376
	sub r15, qword ptr [rsi+rax]
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
	; FSQRT_R e2
	sqrtpd xmm6, xmm6
randomx_isn_196:
	; FMUL_R e2, a0
	mulpd xmm6, xmm8
randomx_isn_197:
	; FSUB_R f2, a0
	subpd xmm2, xmm8
randomx_isn_198:
	; FADD_R f1, a2
	addpd xmm1, xmm10
randomx_isn_199:
	; FSUB_R f3, a3
	subpd xmm3, xmm11
randomx_isn_200:
	; IADD_M r2, L1[r5+248917123]
	lea eax, [r13d+248917123]
	and eax, 16376
	add r10, qword ptr [rsi+rax]
randomx_isn_201:
	; IMUL_R r6, r3
	imul r14, r11
randomx_isn_202:
	; FSWAP_R f3
	shufpd xmm3, xmm3, 1
randomx_isn_203:
	; ISTORE L1[r1+330228321], r3
	lea eax, [r9d+330228321]
	and eax, 16376
	mov qword ptr [rsi+rax], r11
randomx_isn_204:
	; IMUL_M r6, L2[r1+329350850]
	lea eax, [r9d+329350850]
	and eax, 262136
	imul r14, qword ptr [rsi+rax]
randomx_isn_205:
	; ISUB_R r7, r5
	sub r15, r13
randomx_isn_206:
	; IADD_RS r3, r5, SHFT 2
	lea r11, [r11+r13*4]
randomx_isn_207:
	; FMUL_R e1, a2
	mulpd xmm5, xmm10
randomx_isn_208:
	; IADD_RS r6, r3, SHFT 0
	lea r14, [r14+r11*1]
randomx_isn_209:
	; FSCAL_R f0
	xorps xmm0, xmm15
randomx_isn_210:
	; FADD_R f2, a3
	addpd xmm2, xmm11
randomx_isn_211:
	; FMUL_R e2, a3
	mulpd xmm6, xmm11
randomx_isn_212:
	; IMULH_M r0, L1[r1-300353849]
	lea ecx, [r9d-300353849]
	and ecx, 16376
	mov rax, r8
	mul qword ptr [rsi+rcx]
	mov r8, rdx
randomx_isn_213:
	; FMUL_R e2, a1
	mulpd xmm6, xmm9
randomx_isn_214:
	; FSQRT_R e1
	sqrtpd xmm5, xmm5
randomx_isn_215:
	; FSUB_R f3, a1
	subpd xmm3, xmm9
randomx_isn_216:
	; IROR_R r4, r1
	mov ecx, r9d
	ror r12, cl
randomx_isn_217:
	; IMULH_M r6, L1[r5-1692567271]
	lea ecx, [r13d-1692567271]
	and ecx, 16376
	mov rax, r14
	mul qword ptr [rsi+rcx]
	mov r14, rdx
randomx_isn_218:
	; FMUL_R e2, a1
	mulpd xmm6, xmm9
randomx_isn_219:
	; FSUB_R f3, a3
	subpd xmm3, xmm11
randomx_isn_220:
	; FSUB_R f0, a0
	subpd xmm0, xmm8
randomx_isn_221:
	; IMUL_M r1, L1[r0-1347065440]
	lea eax, [r8d-1347065440]
	and eax, 16376
	imul r9, qword ptr [rsi+rax]
randomx_isn_222:
	; IADD_RS r1, r0, SHFT 2
	lea r9, [r9+r8*4]
randomx_isn_223:
	; FMUL_R e2, a0
	mulpd xmm6, xmm8
randomx_isn_224:
	; IADD_RS r5, r4, 312567979, SHFT 3
	lea r13, [r13+r12*8+312567979]
randomx_isn_225:
	; ISTORE L3[r2+260885699], r1
	lea eax, [r10d+260885699]
	and eax, 2097144
	mov qword ptr [rsi+rax], r9
randomx_isn_226:
	; ISUB_R r6, -791575725
	sub r14, -791575725
randomx_isn_227:
	; FSQRT_R e3
	sqrtpd xmm7, xmm7
randomx_isn_228:
	; IXOR_R r7, r1
	xor r15, r9
randomx_isn_229:
	; FSWAP_R f0
	shufpd xmm0, xmm0, 1
randomx_isn_230:
	; IADD_RS r2, r7, SHFT 3
	lea r10, [r10+r15*8]
randomx_isn_231:
	; FSQRT_R e1
	sqrtpd xmm5, xmm5
randomx_isn_232:
	; FMUL_R e3, a3
	mulpd xmm7, xmm11
randomx_isn_233:
	; FMUL_R e0, a2
	mulpd xmm4, xmm10
randomx_isn_234:
	; ISUB_R r2, r7
	sub r10, r15
randomx_isn_235:
	; FSWAP_R e3
	shufpd xmm7, xmm7, 1
randomx_isn_236:
	; FMUL_R e3, a2
	mulpd xmm7, xmm10
randomx_isn_237:
	; FSUB_R f1, a3
	subpd xmm1, xmm11
randomx_isn_238:
	; IADD_RS r4, r2, SHFT 0
	lea r12, [r12+r10*1]
randomx_isn_239:
	; IXOR_R r7, r0
	xor r15, r8
randomx_isn_240:
	; IMULH_R r5, r7
	mov rax, r13
	mul r15
	mov r13, rdx
randomx_isn_241:
	; FSWAP_R e2
	shufpd xmm6, xmm6, 1
randomx_isn_242:
	; IMUL_M r6, L1[r4-162027475]
	lea eax, [r12d-162027475]
	and eax, 16376
	imul r14, qword ptr [rsi+rax]
randomx_isn_243:
	; FSCAL_R f0
	xorps xmm0, xmm15
randomx_isn_244:
	; FSUB_R f1, a0
	subpd xmm1, xmm8
randomx_isn_245:
	; IXOR_R r3, r2
	xor r11, r10
randomx_isn_246:
	; IMUL_R r7, r2
	imul r15, r10
randomx_isn_247:
	; CBRANCH -8545330, COND 4
	add r8, -8547378
	test r8, 1044480
	jz randomx_isn_213
randomx_isn_248:
	; ISTORE L1[r0+1951752498], r5
	lea eax, [r8d+1951752498]
	and eax, 16376
	mov qword ptr [rsi+rax], r13
randomx_isn_249:
	; IADD_RS r6, r5, SHFT 3
	lea r14, [r14+r13*8]
randomx_isn_250:
	; FADD_M f3, L2[r0-1921753477]
	lea eax, [r8d-1921753477]
	and eax, 262136
	cvtdq2pd xmm12, qword ptr [rsi+rax]
	addpd xmm3, xmm12
randomx_isn_251:
	; IADD_RS r0, r0, SHFT 0
	lea r8, [r8+r8*1]
randomx_isn_252:
	; ISUB_R r4, r2
	sub r12, r10
randomx_isn_253:
	; IADD_M r5, L2[r4+256175395]
	lea eax, [r12d+256175395]
	and eax, 262136
	add r13, qword ptr [rsi+rax]
randomx_isn_254:
	; IADD_RS r6, r7, SHFT 2
	lea r14, [r14+r15*4]
randomx_isn_255:
	; FSWAP_R e3
	shufpd xmm7, xmm7, 1
