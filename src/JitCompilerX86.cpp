/*
Copyright (c) 2018 tevador

This file is part of RandomX.

RandomX is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RandomX is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RandomX.  If not, see<http://www.gnu.org/licenses/>.
*/

#include <cstring>
#include <stdexcept>
#include "JitCompilerX86.hpp"
#include "Program.hpp"
#include "reciprocal.h"
#include "virtualMemory.hpp"

namespace RandomX {

#if !defined(_M_X64) && !defined(__x86_64__)
	JitCompilerX86::JitCompilerX86() {
		throw std::runtime_error("JIT compiler only supports x86-64 CPUs");
	}

	void JitCompilerX86::generateProgram(Program& p) {

	}

	size_t JitCompilerX86::getCodeSize() {
		return 0;
	}
#else

	/*

	REGISTER ALLOCATION:

	; rax -> temporary
	; rbx -> loop counter "lc"
	; rcx -> temporary
	; rdx -> temporary
	; rsi -> scratchpad pointer
	; rdi -> dataset pointer
	; rbp -> memory registers "ma" (high 32 bits), "mx" (low 32 bits)
	; rsp -> stack pointer
	; r8  -> "r0"
	; r9  -> "r1"
	; r10 -> "r2"
	; r11 -> "r3"
	; r12 -> "r4"
	; r13 -> "r5"
	; r14 -> "r6"
	; r15 -> "r7"
	; xmm0 -> "f0"
	; xmm1 -> "f1"
	; xmm2 -> "f2"
	; xmm3 -> "f3"
	; xmm4 -> "e0"
	; xmm5 -> "e1"
	; xmm6 -> "e2"
	; xmm7 -> "e3"
	; xmm8 -> "a0"
	; xmm9 -> "a1"
	; xmm10 -> "a2"
	; xmm11 -> "a3"
	; xmm12 -> temporary
	; xmm13 -> DBL_MIN
	; xmm14 -> absolute value mask 0x7fffffffffffffff7fffffffffffffff
	; xmm15 -> sign mask           0x80000000000000008000000000000000

	*/

#include "JitCompilerX86-static.hpp"

	const uint8_t* codePrologue = (uint8_t*)&randomx_program_prologue;
	const uint8_t* codeLoopBegin = (uint8_t*)&randomx_program_loop_begin;
	const uint8_t* codeLoopLoad = (uint8_t*)&randomx_program_loop_load;
	const uint8_t* codeProgamStart = (uint8_t*)&randomx_program_start;
	const uint8_t* codeReadDataset = (uint8_t*)&randomx_program_read_dataset;
	const uint8_t* codeLoopStore = (uint8_t*)&randomx_program_loop_store;
	const uint8_t* codeLoopEnd = (uint8_t*)&randomx_program_loop_end;
	const uint8_t* codeEpilogue = (uint8_t*)&randomx_program_epilogue;
	const uint8_t* codeProgramEnd = (uint8_t*)&randomx_program_end;

	const int32_t prologueSize = codeLoopBegin - codePrologue;
	const int32_t epilogueSize = codeProgramEnd - codeEpilogue;

	const int32_t loopLoadSize = codeProgamStart - codeLoopLoad;
	const int32_t readDatasetSize = codeLoopStore - codeReadDataset;
	const int32_t loopStoreSize = codeLoopEnd - codeLoopStore;

	const int32_t epilogueOffset = CodeSize - epilogueSize;

	static const uint8_t REX_ADD_RR[] = { 0x4d, 0x03 };
	static const uint8_t REX_ADD_RM[] = { 0x4c, 0x03 };
	static const uint8_t REX_SUB_RR[] = { 0x4d, 0x2b };
	static const uint8_t REX_SUB_RM[] = { 0x4c, 0x2b };
	static const uint8_t REX_MOV_RR[] = { 0x41, 0x8b };
	static const uint8_t REX_MOV_RR64[] = { 0x49, 0x8b };
	static const uint8_t REX_MOV_R64R[] = { 0x4c, 0x8b };
	static const uint8_t REX_IMUL_RR[] = { 0x4d, 0x0f, 0xaf };
	static const uint8_t REX_IMUL_RRI[] = { 0x4d, 0x69 };
	static const uint8_t REX_IMUL_RM[] = { 0x4c, 0x0f, 0xaf };
	static const uint8_t REX_MUL_R[] = { 0x49, 0xf7 };
	static const uint8_t REX_MUL_M[] = { 0x48, 0xf7 };
	static const uint8_t REX_81[] = { 0x49, 0x81 };
	static const uint8_t AND_EAX_I = 0x25;
	static const uint8_t MOV_EAX_I = 0xb8;
	static const uint8_t MOV_RAX_I[] = { 0x48, 0xb8 };
	static const uint8_t MOV_RCX_I[] = { 0x48, 0xb9 };
	static const uint8_t REX_LEA[] = { 0x4f, 0x8d };
	static const uint8_t REX_MUL_MEM[] = { 0x48, 0xf7, 0x24, 0x0e };
	static const uint8_t REX_IMUL_MEM[] = { 0x48, 0xf7, 0x2c, 0x0e };
	static const uint8_t REX_SHR_RAX[] = { 0x48, 0xc1, 0xe8 };
	static const uint8_t RAX_ADD_SBB_1[] = { 0x48, 0x83, 0xC0, 0x01, 0x48, 0x83, 0xD8, 0x00 };
	static const uint8_t MUL_RCX[] = { 0x48, 0xf7, 0xe1 };
	static const uint8_t REX_SHR_RDX[] = { 0x48, 0xc1, 0xea };
	static const uint8_t REX_SH[] = { 0x49, 0xc1 };
	static const uint8_t MOV_RCX_RAX_SAR_RCX_63[] = { 0x48, 0x89, 0xc1, 0x48, 0xc1, 0xf9, 0x3f };
	static const uint8_t AND_ECX_I[] = { 0x81, 0xe1 };
	static const uint8_t ADD_RAX_RCX[] = { 0x48, 0x01, 0xC8 };
	static const uint8_t SAR_RAX_I8[] = { 0x48, 0xC1, 0xF8 };
	static const uint8_t NEG_RAX[] = { 0x48, 0xF7, 0xD8 };
	static const uint8_t ADD_R_RAX[] = { 0x4C, 0x03 };
	static const uint8_t XOR_EAX_EAX[] = { 0x33, 0xC0 };
	static const uint8_t ADD_RDX_R[] = { 0x4c, 0x01 };
	static const uint8_t SUB_RDX_R[] = { 0x4c, 0x29 };
	static const uint8_t SAR_RDX_I8[] = { 0x48, 0xC1, 0xFA };
	static const uint8_t TEST_RDX_RDX[] = { 0x48, 0x85, 0xD2 };
	static const uint8_t SETS_AL_ADD_RDX_RAX[] = { 0x0F, 0x98, 0xC0, 0x48, 0x03, 0xD0 };
	static const uint8_t REX_NEG[] = { 0x49, 0xF7 };
	static const uint8_t REX_XOR_RR[] = { 0x4D, 0x33 };
	static const uint8_t REX_XOR_RI[] = { 0x49, 0x81 };
	static const uint8_t REX_XOR_RM[] = { 0x4c, 0x33 };
	static const uint8_t REX_ROT_CL[] = { 0x49, 0xd3 };
	static const uint8_t REX_ROT_I8[] = { 0x49, 0xc1 };
	static const uint8_t SHUFPD[] = { 0x66, 0x0f, 0xc6 };
	static const uint8_t REX_ADDPD[] = { 0x66, 0x41, 0x0f, 0x58 };
	static const uint8_t REX_CVTDQ2PD_XMM12[] = { 0xf3, 0x44, 0x0f, 0xe6, 0x24, 0x06 };
	static const uint8_t REX_SUBPD[] = { 0x66, 0x41, 0x0f, 0x5c };
	static const uint8_t REX_XORPS[] = { 0x41, 0x0f, 0x57 };
	static const uint8_t REX_MULPD[] = { 0x66, 0x41, 0x0f, 0x59 };
	static const uint8_t REX_MAXPD[] = { 0x66, 0x41, 0x0f, 0x5f };
	static const uint8_t REX_DIVPD[] = { 0x66, 0x41, 0x0f, 0x5e };
	static const uint8_t SQRTPD[] = { 0x66, 0x0f, 0x51 };
	static const uint8_t AND_OR_MOV_LDMXCSR[] = { 0x25, 0x00, 0x60, 0x00, 0x00, 0x0D, 0xC0, 0x9F, 0x00, 0x00, 0x89, 0x44, 0x24, 0xF8, 0x0F, 0xAE, 0x54, 0x24, 0xF8 };
	static const uint8_t ROL_RAX[] = { 0x48, 0xc1, 0xc0 };
	static const uint8_t XOR_ECX_ECX[] = { 0x33, 0xC9 };
	static const uint8_t REX_CMP_R32I[] = { 0x41, 0x81 };
	static const uint8_t REX_CMP_M32I[] = { 0x81, 0x3c, 0x06 };
	static const uint8_t MOVAPD[] = { 0x66, 0x0f, 0x29 };
	static const uint8_t REX_MOV_MR[] = { 0x4c, 0x89 };
	static const uint8_t REX_XOR_EAX[] = { 0x41, 0x33 };
	static const uint8_t SUB_EBX[] = { 0x83, 0xEB, 0x01 };
	static const uint8_t JNZ[] = { 0x0f, 0x85 };
	static const uint8_t JMP = 0xe9;
	static const uint8_t REX_XOR_RAX_R64[] = { 0x49, 0x33 };
	static const uint8_t REX_XCHG[] = { 0x4d, 0x87 };
	static const uint8_t REX_ANDPS_XMM12[] = { 0x45, 0x0f, 0x54, 0xe6 };
	static const uint8_t REX_PADD[] = { 0x66, 0x44, 0x0f };
	static const uint8_t PADD_OPCODES[] = { 0xfc, 0xfd, 0xfe, 0xd4 };

	size_t JitCompilerX86::getCodeSize() {
		return codePos - prologueSize;
	}

	JitCompilerX86::JitCompilerX86() {
		code = (uint8_t*)allocExecutableMemory(CodeSize);
		memcpy(code, codePrologue, prologueSize);
		memcpy(code + CodeSize - epilogueSize, codeEpilogue, epilogueSize);
	}

	void JitCompilerX86::generateProgram(Program& prog) {
		auto addressRegisters = prog.getEntropy(12);
		uint32_t readReg0 = 0 + (addressRegisters & 1);
		addressRegisters >>= 1;
		uint32_t readReg1 = 2 + (addressRegisters & 1);
		addressRegisters >>= 1;
		uint32_t readReg2 = 4 + (addressRegisters & 1);
		addressRegisters >>= 1;
		uint32_t readReg3 = 6 + (addressRegisters & 1);
		codePos = prologueSize;
		emit(REX_XOR_RAX_R64);
		emitByte(0xc0 + readReg0);
		emit(REX_XOR_RAX_R64);
		emitByte(0xc0 + readReg1);
		memcpy(code + codePos, codeLoopLoad, loopLoadSize);
		codePos += loopLoadSize;
		for (unsigned i = 0; i < ProgramLength; ++i) {
			Instruction& instr = prog(i);
			instr.src %= RegistersCount;
			instr.dst %= RegistersCount;
			generateCode(instr);
		}
		emit(REX_MOV_RR);
		emitByte(0xc0 + readReg2);
		emit(REX_XOR_EAX);
		emitByte(0xc0 + readReg3);
		memcpy(code + codePos, codeReadDataset, readDatasetSize);
		codePos += readDatasetSize;
		memcpy(code + codePos, codeLoopStore, loopStoreSize);
		codePos += loopStoreSize;
		emit(SUB_EBX);
		emit(JNZ);
		emit32(prologueSize - codePos - 4);
		emitByte(JMP);
		emit32(epilogueOffset - codePos - 4);
		emitByte(0x90);
	}

	void JitCompilerX86::generateCode(Instruction& instr) {
		auto generator = engine[instr.opcode];
		(this->*generator)(instr);
	}

	void JitCompilerX86::genAddressReg(Instruction& instr, bool rax = true) {
		emit(REX_MOV_RR);
		emitByte((rax ? 0xc0 : 0xc8) + instr.src);
		if (rax)
			emitByte(AND_EAX_I);
		else
			emit(AND_ECX_I);
		emit32((instr.mod % 4) ? ScratchpadL1Mask : ScratchpadL2Mask);
	}

	void JitCompilerX86::genAddressRegDst(Instruction& instr, bool align16 = false) {
		emit(REX_MOV_RR);
		emitByte(0xc0 + instr.dst);
		emitByte(AND_EAX_I);
		int32_t maskL1 = align16 ? ScratchpadL1Mask16 : ScratchpadL1Mask;
		int32_t maskL2 = align16 ? ScratchpadL2Mask16 : ScratchpadL2Mask;
		emit32((instr.mod % 4) ? maskL1 : maskL2);
	}

	void JitCompilerX86::genAddressImm(Instruction& instr) {
		emit32(instr.imm32 & ScratchpadL3Mask);
	}

	void JitCompilerX86::h_IADD_R(Instruction& instr) {
		if (instr.src != instr.dst) {
			emit(REX_ADD_RR);
			emitByte(0xc0 + 8 * instr.dst + instr.src);
		}
		else {
			emit(REX_81);
			emitByte(0xc0 + instr.dst);
			emit32(instr.imm32);
		}
	}

	void JitCompilerX86::h_IADD_M(Instruction& instr) {
		if (instr.src != instr.dst) {
			genAddressReg(instr);
			emit(REX_ADD_RM);
			emitByte(0x04 + 8 * instr.dst);
			emitByte(0x06);
		}
		else {
			emit(REX_ADD_RM);
			emitByte(0x86 + 8 * instr.dst);
			genAddressImm(instr);
		}
	}

	void JitCompilerX86::genSIB(int scale, int index, int base) {
		emitByte((scale << 6) | (index << 3) | base);
	}

	void JitCompilerX86::h_IADD_RC(Instruction& instr) {
		emit(REX_LEA);
		emitByte(0x84 + 8 * instr.dst);
		genSIB(0, instr.src, instr.dst);
		emit32(instr.imm32);
	}

	void JitCompilerX86::h_ISUB_R(Instruction& instr) {
		if (instr.src != instr.dst) {
			emit(REX_SUB_RR);
			emitByte(0xc0 + 8 * instr.dst + instr.src);
		}
		else {
			emit(REX_81);
			emitByte(0xe8 + instr.dst);
			emit32(instr.imm32);
		}
	}

	void JitCompilerX86::h_ISUB_M(Instruction& instr) {
		if (instr.src != instr.dst) {
			genAddressReg(instr);
			emit(REX_SUB_RM);
			emitByte(0x04 + 8 * instr.dst);
			emitByte(0x06);
		}
		else {
			emit(REX_SUB_RM);
			emitByte(0x86 + 8 * instr.dst);
			genAddressImm(instr);
		}
	}

	void JitCompilerX86::h_IMUL_9C(Instruction& instr) {
		emit(REX_LEA);
		emitByte(0x84 + 8 * instr.dst);
		genSIB(3, instr.dst, instr.dst);
		emit32(instr.imm32);
	}

	void JitCompilerX86::h_IMUL_R(Instruction& instr) {
		if (instr.src != instr.dst) {
			emit(REX_IMUL_RR);
			emitByte(0xc0 + 8 * instr.dst + instr.src);
		}
		else {
			emit(REX_IMUL_RRI);
			emitByte(0xc0 + 9 * instr.dst);
			emit32(instr.imm32);
		}
	}

	void JitCompilerX86::h_IMUL_M(Instruction& instr) {
		if (instr.src != instr.dst) {
			genAddressReg(instr);
			emit(REX_IMUL_RM);
			emitByte(0x04 + 8 * instr.dst);
			emitByte(0x06);
		}
		else {
			emit(REX_IMUL_RM);
			emitByte(0x86 + 8 * instr.dst);
			genAddressImm(instr);
		}
	}

	void JitCompilerX86::h_IMULH_R(Instruction& instr) {
		emit(REX_MOV_RR64);
		emitByte(0xc0 + instr.dst);
		emit(REX_MUL_R);
		emitByte(0xe0 + instr.src);
		emit(REX_MOV_R64R);
		emitByte(0xc2 + 8 * instr.dst);
	}

	void JitCompilerX86::h_IMULH_M(Instruction& instr) {
		if (instr.src != instr.dst) {
			genAddressReg(instr, false);
			emit(REX_MOV_RR64);
			emitByte(0xc0 + instr.dst);
			emit(REX_MUL_MEM);
		}
		else {
			emit(REX_MOV_RR64);
			emitByte(0xc0 + instr.dst);
			emit(REX_MUL_M);
			emitByte(0xa6);
			genAddressImm(instr);
		}
		emit(REX_MOV_R64R);
		emitByte(0xc2 + 8 * instr.dst);
	}

	void JitCompilerX86::h_ISMULH_R(Instruction& instr) {
		emit(REX_MOV_RR64);
		emitByte(0xc0 + instr.dst);
		emit(REX_MUL_R);
		emitByte(0xe8 + instr.src);
		emit(REX_MOV_R64R);
		emitByte(0xc2 + 8 * instr.dst);
	}

	void JitCompilerX86::h_ISMULH_M(Instruction& instr) {
		if (instr.src != instr.dst) {
			genAddressReg(instr, false);
			emit(REX_MOV_RR64);
			emitByte(0xc0 + instr.dst);
			emit(REX_IMUL_MEM);
		}
		else {
			emit(REX_MOV_RR64);
			emitByte(0xc0 + instr.dst);
			emit(REX_MUL_M);
			emitByte(0xae);
			genAddressImm(instr);
		}
		emit(REX_MOV_R64R);
		emitByte(0xc2 + 8 * instr.dst);
	}

	void JitCompilerX86::h_IMUL_RCP(Instruction& instr) {
		if (instr.imm32 != 0) {
			emit(MOV_RAX_I);
			emit64(reciprocal(instr.imm32));
			emit(REX_IMUL_RM);
			emitByte(0xc0 + 8 * instr.dst);
		}
	}

	void JitCompilerX86::h_ISDIV_C(Instruction& instr) {

	}

	void JitCompilerX86::h_INEG_R(Instruction& instr) {
		emit(REX_NEG);
		emitByte(0xd8 + instr.dst);
	}

	void JitCompilerX86::h_IXOR_R(Instruction& instr) {
		if (instr.src != instr.dst) {
			emit(REX_XOR_RR);
			emitByte(0xc0 + 8 * instr.dst + instr.src);
		}
		else {
			emit(REX_XOR_RI);
			emitByte(0xf0 + instr.dst);
			emit32(instr.imm32);
		}
	}

	void JitCompilerX86::h_IXOR_M(Instruction& instr) {
		if (instr.src != instr.dst) {
			genAddressReg(instr);
			emit(REX_XOR_RM);
			emitByte(0x04 + 8 * instr.dst);
			emitByte(0x06);
	}
		else {
			emit(REX_XOR_RM);
			emitByte(0x86 + 8 * instr.dst);
			genAddressImm(instr);
		}
	}

	void JitCompilerX86::h_IROR_R(Instruction& instr) {
		if (instr.src != instr.dst) {
			emit(REX_MOV_RR);
			emitByte(0xc8 + instr.src);
			emit(REX_ROT_CL);
			emitByte(0xc8 + instr.dst);
		}
		else {
			emit(REX_ROT_I8);
			emitByte(0xc8 + instr.dst);
			emitByte(instr.imm32 & 63);
		}
	}

	void JitCompilerX86::h_IROL_R(Instruction& instr) {
		if (instr.src != instr.dst) {
			emit(REX_MOV_RR);
			emitByte(0xc8 + instr.src);
			emit(REX_ROT_CL);
			emitByte(0xc0 + instr.dst);
		}
		else {
			emit(REX_ROT_I8);
			emitByte(0xc0 + instr.dst);
			emitByte(instr.imm32 & 63);
		}
	}

	void JitCompilerX86::h_ISWAP_R(Instruction& instr) {
		if (instr.src != instr.dst) {
			emit(REX_XCHG);
			emitByte(0xc0 + instr.src + 8 * instr.dst);
		}
	}

	void JitCompilerX86::h_FSWAP_R(Instruction& instr) {
		emit(SHUFPD);
		emitByte(0xc0 + 9 * instr.dst);
		emitByte(1);
	}

	void JitCompilerX86::h_FADD_R(Instruction& instr) {
		instr.dst %= 4;
		instr.src %= 4;
		emit(REX_ADDPD);
		emitByte(0xc0 + instr.src + 8 * instr.dst);
		//emit(REX_PADD);
		//emitByte(PADD_OPCODES[instr.mod % 4]);
		//emitByte(0xf8 + instr.dst);
	}

	void JitCompilerX86::h_FADD_M(Instruction& instr) {
		instr.dst %= 4;
		genAddressReg(instr);
		emit(REX_CVTDQ2PD_XMM12);
		emit(REX_ADDPD);
		emitByte(0xc4 + 8 * instr.dst);
	}

	void JitCompilerX86::h_FSUB_R(Instruction& instr) {
		instr.dst %= 4;
		instr.src %= 4;
		emit(REX_SUBPD);
		emitByte(0xc0 + instr.src + 8 * instr.dst);
		//emit(REX_PADD);
		//emitByte(PADD_OPCODES[instr.mod % 4]);
		//emitByte(0xf8 + instr.dst);
	}

	void JitCompilerX86::h_FSUB_M(Instruction& instr) {
		instr.dst %= 4;
		genAddressReg(instr);
		emit(REX_CVTDQ2PD_XMM12);
		emit(REX_SUBPD);
		emitByte(0xc4 + 8 * instr.dst);
	}

	void JitCompilerX86::h_FSCAL_R(Instruction& instr) {
		instr.dst %= 4;
		emit(REX_XORPS);
		emitByte(0xc7 + 8 * instr.dst);
	}

	void JitCompilerX86::h_FMUL_R(Instruction& instr) {
		instr.dst %= 4;
		instr.src %= 4;
		emit(REX_MULPD);
		emitByte(0xe0 + instr.src + 8 * instr.dst);
	}

	void JitCompilerX86::h_FMUL_M(Instruction& instr) {
		instr.dst %= 4;
		genAddressReg(instr);
		emit(REX_CVTDQ2PD_XMM12);
		emit(REX_ANDPS_XMM12);
		emit(REX_MULPD);
		emitByte(0xe4 + 8 * instr.dst);
		emit(REX_MAXPD);
		emitByte(0xe5 + 8 * instr.dst);
	}

	void JitCompilerX86::h_FDIV_R(Instruction& instr) {
		instr.dst %= 4;
		instr.src %= 4;
		emit(REX_DIVPD);
		emitByte(0xe0 + instr.src + 8 * instr.dst);
		emit(REX_MAXPD);
		emitByte(0xe5 + 8 * instr.dst);
	}

	void JitCompilerX86::h_FDIV_M(Instruction& instr) {
		instr.dst %= 4;
		genAddressReg(instr);
		emit(REX_CVTDQ2PD_XMM12);
		emit(REX_ANDPS_XMM12);
		emit(REX_DIVPD);
		emitByte(0xe4 + 8 * instr.dst);
		emit(REX_MAXPD);
		emitByte(0xe5 + 8 * instr.dst);
	}

	void JitCompilerX86::h_FSQRT_R(Instruction& instr) {
		instr.dst %= 4;
		emit(SQRTPD);
		emitByte(0xe4 + 9 * instr.dst);
	}

	void JitCompilerX86::h_CFROUND(Instruction& instr) {
		emit(REX_MOV_RR64);
		emitByte(0xc0 + instr.src);	
		int rotate = (13 - (instr.imm32 & 63)) & 63;
		if (rotate != 0) {
			emit(ROL_RAX);
			emitByte(rotate);
		}
		emit(AND_OR_MOV_LDMXCSR);
	}

	static inline uint8_t condition(Instruction& instr) {
		switch ((instr.mod >> 2) & 7)
		{
			case 0:
				return 0x96; //setbe
			case 1:
				return 0x97; //seta
			case 2:
				return 0x98; //sets
			case 3:
				return 0x99; //setns
			case 4:
				return 0x90; //seto
			case 5:
				return 0x91; //setno
			case 6:
				return 0x9c; //setl
			case 7:
				return 0x9d; //setge
			default:
				UNREACHABLE;
		}
	}

	void JitCompilerX86::h_COND_R(Instruction& instr) {
		emit(XOR_ECX_ECX);
		emit(REX_CMP_R32I);
		emitByte(0xf8 + instr.src);
		emit32(instr.imm32);
		emitByte(0x0f);
		emitByte(condition(instr));
		emitByte(0xc1);
		emit(REX_ADD_RM);
		emitByte(0xc1 + 8 * instr.dst);
	}

	void JitCompilerX86::h_COND_M(Instruction& instr) {
		emit(XOR_ECX_ECX);
		genAddressReg(instr);
		emit(REX_CMP_M32I);
		emit32(instr.imm32);
		emitByte(0x0f);
		emitByte(condition(instr));
		emitByte(0xc1);
		emit(REX_ADD_RM);
		emitByte(0xc1 + 8 * instr.dst);
	}

	void JitCompilerX86::h_ISTORE(Instruction& instr) {
		genAddressRegDst(instr);
		emit(REX_MOV_MR);
		emitByte(0x04 + 8 * instr.src);
		emitByte(0x06);
	}

	void JitCompilerX86::h_FSTORE(Instruction& instr) {
		genAddressRegDst(instr, true);
		emit(MOVAPD);
		emitByte(0x04 + 8 * instr.src);
		emitByte(0x06);
	}

	void JitCompilerX86::h_NOP(Instruction& instr) {
		emitByte(0x90);
	}

#include "instructionWeights.hpp"
#define INST_HANDLE(x) REPN(&JitCompilerX86::h_##x, WT(x))

	InstructionGeneratorX86 JitCompilerX86::engine[256] = {
		INST_HANDLE(IADD_R)
		INST_HANDLE(IADD_M)
		INST_HANDLE(IADD_RC)
		INST_HANDLE(ISUB_R)
		INST_HANDLE(ISUB_M)
		INST_HANDLE(IMUL_9C)
		INST_HANDLE(IMUL_R)
		INST_HANDLE(IMUL_M)
		INST_HANDLE(IMULH_R)
		INST_HANDLE(IMULH_M)
		INST_HANDLE(ISMULH_R)
		INST_HANDLE(ISMULH_M)
		INST_HANDLE(IMUL_RCP)
		INST_HANDLE(ISDIV_C)
		INST_HANDLE(INEG_R)
		INST_HANDLE(IXOR_R)
		INST_HANDLE(IXOR_M)
		INST_HANDLE(IROR_R)
		INST_HANDLE(IROL_R)
		INST_HANDLE(ISWAP_R)
		INST_HANDLE(FSWAP_R)
		INST_HANDLE(FADD_R)
		INST_HANDLE(FADD_M)
		INST_HANDLE(FSUB_R)
		INST_HANDLE(FSUB_M)
		INST_HANDLE(FSCAL_R)
		INST_HANDLE(FMUL_R)
		INST_HANDLE(FMUL_M)
		INST_HANDLE(FDIV_R)
		INST_HANDLE(FDIV_M)
		INST_HANDLE(FSQRT_R)
		INST_HANDLE(COND_R)
		INST_HANDLE(COND_M)
		INST_HANDLE(CFROUND)
		INST_HANDLE(ISTORE)
		INST_HANDLE(FSTORE)
		INST_HANDLE(NOP)
	};


#endif
}