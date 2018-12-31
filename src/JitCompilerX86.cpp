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

#include "JitCompilerX86.hpp"
#include "Pcg32.hpp"
#include <cstring>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/mman.h>
#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS MAP_ANON
#endif
#endif

namespace RandomX {

#if !defined(_M_X64) && !defined(__x86_64__)
	JitCompilerX86::JitCompilerX86() {
		throw std::runtime_error("JIT compiler only supports x86-64 CPUs");
	}

	void JitCompilerX86::generateProgram(Pcg32& gen) {

	}
#else

	/*
	 REGISTER ALLOCATION:

	 rax -> temporary
	 rbx -> MemoryRegisters& memory
	 rcx -> temporary
	 rdx -> temporary
	 rsi -> convertible_t* scratchpad
	 rdi -> "ic" (instruction counter)
	 rbp -> beginning of VM stack
	 rsp -> end of VM stack
	 r8  -> "r0"
	 r9  -> "r1"
	 r10 -> "r2"
	 r11 -> "r3"
	 r12 -> "r4"
	 r13 -> "r5"
	 r14 -> "r6"
	 r15 -> "r7"
	 xmm0 -> temporary
	 xmm1 -> temporary
	 xmm2 -> "f2"
	 xmm3 -> "f3"
	 xmm4 -> "f4"
	 xmm5 -> "f5"
	 xmm6 -> "f6"
	 xmm7 -> "f7"
	 xmm8 -> "f0"
	 xmm9 -> "f1"
	 xmm10 -> absolute value mask 0x7fffffffffffffff7fffffffffffffff

	 STACK STRUCTURE:

	   |
	   |
	   | saved registers
	   |
	   v
	 [rbp] RegisterFile& registerFile
	   |
	   |
	   | VM stack
	   |
	   v
	 [rsp] last element of VM stack

	*/

#include "JitCompilerX86-static.hpp"

	const uint8_t* codePrologue = (uint8_t*)&randomx_program_prologue;
	const uint8_t* codeProgramBegin = (uint8_t*)&randomx_program_begin;
	const uint8_t* codeEpilogue = (uint8_t*)&randomx_program_epilogue;
	const uint8_t* codeReadDatasetR = (uint8_t*)&randomx_program_read_r;
	const uint8_t* codeReadDatasetF = (uint8_t*)&randomx_program_read_f;
	const uint8_t* codeProgramEnd = (uint8_t*)&randomx_program_end;

	const int32_t prologueSize = codeProgramBegin - codePrologue;
	const int32_t epilogueSize = codeReadDatasetR - codeEpilogue;
	const int32_t readDatasetRSize = codeReadDatasetF - codeReadDatasetR;
	const int32_t readDatasetFSize = codeProgramEnd - codeReadDatasetF;

	const int32_t readDatasetFOffset = CodeSize - readDatasetFSize;
	const int32_t readDatasetROffset = readDatasetFOffset - readDatasetRSize;
	const int32_t epilogueOffset = readDatasetROffset - epilogueSize;

	JitCompilerX86::JitCompilerX86() {
#ifdef _WIN32
		code = (uint8_t*)VirtualAlloc(nullptr, CodeSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (code == nullptr)
			throw std::runtime_error("VirtualAlloc failed");
#else
		code = (uint8_t*)mmap(nullptr, CodeSize, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		if (code == (uint8_t*)-1)
			throw std::runtime_error("mmap failed");
#endif
		memcpy(code, codePrologue, prologueSize);
		memcpy(code + CodeSize - readDatasetRSize - readDatasetFSize - epilogueSize, codeEpilogue, epilogueSize);
		memcpy(code + CodeSize - readDatasetRSize - readDatasetFSize, codeReadDatasetR, readDatasetRSize);
		memcpy(code + CodeSize - readDatasetFSize, codeReadDatasetF, readDatasetFSize);
	}

	void JitCompilerX86::generateProgram(Pcg32& gen) {
		instructionOffsets.clear();
		callOffsets.clear();
		codePos = prologueSize;
		Instruction instr;
		for (unsigned i = 0; i < ProgramLength; ++i) {
			for (unsigned j = 0; j < sizeof(instr) / sizeof(Pcg32::result_type); ++j) {
				*(((uint32_t*)&instr) + j) = gen();
			}
			generateCode(instr, i);
		}
		emitByte(0xe9);
		emit(instructionOffsets[0] - (codePos + 4));
		fixCallOffsets();
	}

	void JitCompilerX86::generateCode(Instruction& instr, int i) {
		instructionOffsets.push_back(codePos);
		emit(0x840fcfff); //dec edx; jz <epilogue>
		emit(epilogueOffset - (codePos + 4)); //jump offset (RIP-relative)
		auto generator = engine[instr.opcode];
		(this->*generator)(instr, i);
	}

	void JitCompilerX86::fixCallOffsets() {
		for (CallOffset& co : callOffsets) {
			*reinterpret_cast<int32_t*>(code + co.pos) = instructionOffsets[co.index] - (co.pos + 4);
		}
	}

	void JitCompilerX86::genar(Instruction& instr) {
		emit(uint16_t(0x8149)); //xor
		emitByte(0xf0 + (instr.rega % RegistersCount));
		emit(instr.addra);
		switch (instr.loca & 7)
		{
			case 0:
			case 1:
			case 2:
			case 3:
				emit(uint16_t(0x8b41)); //mov
				emitByte(0xc8 + (instr.rega % RegistersCount)); //ecx, rega
				emitByte(0xe8); //call
				emit(readDatasetROffset - (codePos + 4));
				return;

			case 4:
				emit(uint16_t(0x8b41)); //mov
				emitByte(0xc0 + (instr.rega % RegistersCount)); //eax, rega
				emitByte(0x25); //and
				emit(ScratchpadL2 - 1); //whole scratchpad
				emit(0xc6048b48); // mov rax,QWORD PTR [rsi+rax*8]
				return;

			default:
				emit(uint16_t(0x8b41)); //mov
				emitByte(0xc0 + (instr.rega % RegistersCount)); //eax, rega
				emitByte(0x25); //and
				emit(ScratchpadL1 - 1); //first 16 KiB of scratchpad
				emit(0xc6048b48); // mov rax,QWORD PTR [rsi+rax*8]
				return;
		}
	}

	void JitCompilerX86::genaf(Instruction& instr) {
		emit(uint16_t(0x8149)); //xor
		emitByte(0xf0 + (instr.rega % RegistersCount));
		emit(instr.addra);
		switch (instr.loca & 7)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			emit(uint16_t(0x8b41)); //mov
			emitByte(0xc8 + (instr.rega % RegistersCount)); //ecx, rega
			emitByte(0xe8); //call
			emit(readDatasetFOffset - (codePos + 4));
			return;

		case 4:
			emit(uint16_t(0x8b41)); //mov
			emitByte(0xc0 + (instr.rega % RegistersCount)); //eax, rega
			emitByte(0x25); //and
			emit(ScratchpadL2 - 1); //whole scratchpad
			emitByte(0xf3);
			emit(0xc604e60f); //cvtdq2pd xmm0,QWORD PTR [rsi+rax*8]
			return;

		default:
			emit(uint16_t(0x8b41)); //mov
			emitByte(0xc0 + (instr.rega % RegistersCount)); //eax, rega
			emitByte(0x25); //and
			emit(ScratchpadL1 - 1); //first 16 KiB of scratchpad
			emitByte(0xf3);
			emit(0xc604e60f); //cvtdq2pd xmm0,QWORD PTR [rsi+rax*8]
			return;
		}
	}

	void JitCompilerX86::genbr0(Instruction& instr, uint16_t opcodeReg, uint16_t opcodeImm) {
		if ((instr.locb & 7) <= 3) {
			emit(uint16_t(0x8b49)); //mov
			emitByte(0xc8 + (instr.regb % RegistersCount)); //rcx, regb
			emitByte(0x48); //REX.W
			emit(opcodeReg); //xxx rax, cl
		}
		else {
			emitByte(0x48); //REX.W
			emit(opcodeImm); //xxx rax, imm8
			emitByte((instr.imm8 & 63));
		}
	}

	void JitCompilerX86::genbr1(Instruction& instr, uint16_t opcodeReg, uint16_t opcodeImm) {
		if ((instr.locb & 7) <= 5) {
			emit(opcodeReg); // xxx rax, r64
			emitByte(0xc0 + (instr.regb % RegistersCount));
		}
		else {
			emit(opcodeImm); // xxx rax, imm32
			emit(instr.imm32);
		}
	}

	void JitCompilerX86::genbr132(Instruction& instr, uint16_t opcodeReg, uint8_t opcodeImm) {
		if ((instr.locb & 7) <= 5) {
			emit(opcodeReg); // xxx eax, r32
			emitByte(0xc0 + (instr.regb % RegistersCount));
		}
		else {
			emitByte(opcodeImm); // xxx eax, imm32
			emit(instr.imm32);
		}
	}

	void JitCompilerX86::genbf(Instruction& instr, uint8_t opcode) {
		int regb = (instr.regb % RegistersCount);
		emitByte(0x66); //xxxpd  xmm0,regb
		if (regb <= 1) {
			emitByte(0x41); //REX
		}
		emitByte(0x0f);
		emitByte(opcode);
		emitByte(0xc0 + regb);
	}


	void JitCompilerX86::scratchpadStoreR(Instruction& instr, uint32_t scratchpadSize) {
		emit(0x41c88b48); //mov rcx, rax; REX
		emitByte(0x8b); // mov
		emitByte(0xc0 + (instr.regc % RegistersCount)); //eax, regc
		emitByte(0x35); // xor eax
		emit(instr.addrc);
		emitByte(0x25); //and
		emit(scratchpadSize - 1);
		emit(0xc60c8948); // mov    QWORD PTR [rsi+rax*8],rcx
	}

	void JitCompilerX86::gencr(Instruction& instr) {
		switch (instr.locc & 7)
		{
			case 0:
				scratchpadStoreR(instr, ScratchpadL2);
				break;

			case 1:
			case 2:
			case 3:
				scratchpadStoreR(instr, ScratchpadL1);
				break;

			default:
				emit(uint16_t(0x8b4c)); //mov
				emitByte(0xc0 + 8 * (instr.regc % RegistersCount)); //regc, rax
				break;
		}
	}

	void JitCompilerX86::scratchpadStoreF(Instruction& instr, int regc, uint32_t scratchpadSize, bool storeHigh) {
		emit(uint16_t(0x8b41)); //mov
		emitByte(0xc0 + regc); //eax, regc
		emitByte(0x35); // xor eax
		emit(instr.addrc);
		emitByte(0x25); //and
		emit(scratchpadSize - 1);
		emitByte(0x66); //movhpd/movlpd QWORD PTR [rsi+rax*8], regc
		if (regc <= 1) {
			emitByte(0x44); //REX
		}
		emitByte(0x0f);
		emitByte(storeHigh ? 0x17 : 0x13);
		emitByte(4 + 8 * regc);
		emitByte(0xc6);
	}

	void JitCompilerX86::gencf(Instruction& instr, bool alwaysLow = false) {
		int regc = (instr.regc % RegistersCount);
		if (!alwaysLow) {
			if (regc <= 1) {
				emitByte(0x44); //REX
			}
			emit(uint16_t(0x280f)); //movaps
			emitByte(0xc0 + 8 * regc); // regc, xmm0
		}
		switch (instr.locc & 7)
		{
			case 4:
				scratchpadStoreF(instr, regc, ScratchpadL2, !alwaysLow && (instr.locc & 8));
				break;

			case 5:
			case 6:
			case 7:
				scratchpadStoreF(instr, regc, ScratchpadL1, !alwaysLow && (instr.locc & 8));
				break;

			default:
				break;
		}
	}

	void JitCompilerX86::h_ADD_64(Instruction& instr, int i) {
		genar(instr);
		genbr1(instr, 0x0349, 0x0548);
		gencr(instr);
	}

	void JitCompilerX86::h_ADD_32(Instruction& instr, int i) {
		genar(instr);
		genbr132(instr, 0x0341, 0x05);
		gencr(instr);
	}

	void JitCompilerX86::h_SUB_64(Instruction& instr, int i) {
		genar(instr);
		genbr1(instr, 0x2b49, 0x2d48);
		gencr(instr);
	}

	void JitCompilerX86::h_SUB_32(Instruction& instr, int i) {
		genar(instr);
		genbr132(instr, 0x2b41, 0x2d);
		gencr(instr);
	}

	void JitCompilerX86::h_MUL_64(Instruction& instr, int i) {
		genar(instr);
		if ((instr.locb & 7) <= 5) {
			emitByte(0x49); //REX
			emit(uint16_t(0xaf0f)); // imul rax, r64
			emitByte(0xc0 + (instr.regb % RegistersCount));
		}
		else {
			emitByte(0x48); //REX
			emit(uint16_t(0xc069)); // imul rax, rax, imm32
			emit(instr.imm32);
		}
		gencr(instr);
	}

	void JitCompilerX86::h_MULH_64(Instruction& instr, int i) {
		genar(instr);
		if ((instr.locb & 7) <= 5) {
			emit(uint16_t(0x8b49)); //mov rcx, r64
			emitByte(0xc8 + (instr.regb % RegistersCount));
		}
		else {
			emitByte(0x48);
			emit(uint16_t(0xc1c7)); // mov rcx, imm32
			emit(instr.imm32);
		}
		emitByte(0x48);
		emit(uint16_t(0xe1f7)); // mul rcx
		emitByte(0x48);
		emit(uint16_t(0xc28b)); //	mov rax,rdx
		gencr(instr);
	}

	void JitCompilerX86::h_MUL_32(Instruction& instr, int i) {
		genar(instr);
		emit(uint16_t(0xc88b)); //mov ecx, eax
		if ((instr.locb & 7) <= 5) {
			emit(uint16_t(0x8b41)); // mov eax, r32
			emitByte(0xc0 + (instr.regb % RegistersCount));
		}
		else {
			emitByte(0xb8); // mov eax, imm32
			emit(instr.imm32);
		}
		emit(0xc1af0f48); //imul rax,rcx
		gencr(instr);
	}

	void JitCompilerX86::h_IMUL_32(Instruction& instr, int i) {
		genar(instr);
		emitByte(0x48);
		emit(uint16_t(0xc863)); //movsxd rcx,eax
		if ((instr.locb & 7) <= 5) {
			emit(uint16_t(0x6349)); //movsxd rax,r32
			emitByte(0xc0 + (instr.regb % RegistersCount));
		}
		else {
			emitByte(0x48);
			emit(uint16_t(0xc0c7)); // mov rax, imm32
			emit(instr.imm32);
		}
		emit(0xc1af0f48); //imul rax,rcx
		gencr(instr);
	}

	void JitCompilerX86::h_IMULH_64(Instruction& instr, int i) {
		genar(instr);
		if ((instr.locb & 7) <= 5) {
			emit(uint16_t(0x8b49)); //mov rcx, r64
			emitByte(0xc8 + (instr.regb % RegistersCount));
		}
		else {
			emitByte(0x48);
			emit(uint16_t(0xc1c7)); // mov rcx, imm32
			emit(instr.imm32);
		}
		emitByte(0x48);
		emit(uint16_t(0xe9f7)); // imul rcx
		emitByte(0x48);
		emit(uint16_t(0xc28b)); //	mov rax,rdx
		gencr(instr);
	}

	void JitCompilerX86::h_DIV_64(Instruction& instr, int i) {
		genar(instr);
		if ((instr.locb & 7) <= 5) {
			emitByte(0xb9); //mov ecx, 1
			emit(1);
			emit(uint16_t(0x8b41)); //mov edx, r32
			emitByte(0xd0 + (instr.regb % RegistersCount));
			emit(0x450fd285); //test edx, edx; cmovne ecx,edx
			emitByte(0xca);
		}
		else {
			emitByte(0xb9); //mov ecx, imm32
			emit(instr.imm32 != 0 ? instr.imm32 : 1);
		}
		emit(0xf748d233); //xor edx,edx; div rcx
		emitByte(0xf1);
		gencr(instr);
	}

	void JitCompilerX86::h_IDIV_64(Instruction& instr, int i) {
		genar(instr);
		if ((instr.locb & 7) <= 5) {
			emit(uint16_t(0x8b41)); //mov edx, r32
			emitByte(0xd0 + (instr.regb % RegistersCount));
		}
		else {
			emitByte(0xba); // xxx edx, imm32
			emit(instr.imm32);
		}
		emit(0xc88b480b75fffa83);
		emit(0x1274c9ff48c1d148);
		emit(0x0fd28500000001b9);
		emit(0x489948c96348ca45);
		emit(uint16_t(0xf9f7)); //idiv rcx
		gencr(instr);
	}

	void JitCompilerX86::h_AND_64(Instruction& instr, int i) {
		genar(instr);
		genbr1(instr, 0x2349, 0x2548);
		gencr(instr);
	}

	void JitCompilerX86::h_AND_32(Instruction& instr, int i) {
		genar(instr);
		genbr132(instr, 0x2341, 0x25);
		gencr(instr);
	}

	void JitCompilerX86::h_OR_64(Instruction& instr, int i) {
		genar(instr);
		genbr1(instr, 0x0b49, 0x0d48);
		gencr(instr);
	}

	void JitCompilerX86::h_OR_32(Instruction& instr, int i) {
		genar(instr);
		genbr132(instr, 0x0b41, 0x0d);
		gencr(instr);
	}

	void JitCompilerX86::h_XOR_64(Instruction& instr, int i) {
		genar(instr);
		genbr1(instr, 0x3349, 0x3548);
		gencr(instr);
	}

	void JitCompilerX86::h_XOR_32(Instruction& instr, int i) {
		genar(instr);
		genbr132(instr, 0x3341, 0x35);
		gencr(instr);
	}

	void JitCompilerX86::h_SHL_64(Instruction& instr, int i) {
		genar(instr);
		genbr0(instr, 0xe0d3, 0xe0c1);
		gencr(instr);
	}

	void JitCompilerX86::h_SHR_64(Instruction& instr, int i) {
		genar(instr);
		genbr0(instr, 0xe8d3, 0xe8c1);
		gencr(instr);
	}

	void JitCompilerX86::h_SAR_64(Instruction& instr, int i) {
		genar(instr);
		genbr0(instr, 0xf8d3, 0xf8c1);
		gencr(instr);
	}

	void JitCompilerX86::h_ROL_64(Instruction& instr, int i) {
		genar(instr);
		genbr0(instr, 0xc0d3, 0xc0c1);
		gencr(instr);
	}

	void JitCompilerX86::h_ROR_64(Instruction& instr, int i) {
		genar(instr);
		genbr0(instr, 0xc8d3, 0xc8c1);
		gencr(instr);
	}

	void JitCompilerX86::h_FPADD(Instruction& instr, int i) {
		genaf(instr);
		genbf(instr, 0x58);
		gencf(instr);
	}

	void JitCompilerX86::h_FPSUB(Instruction& instr, int i) {
		genaf(instr);
		genbf(instr, 0x5c);
		gencf(instr);
	}

	void JitCompilerX86::h_FPMUL(Instruction& instr, int i) {
		genaf(instr);
		genbf(instr, 0x59);
		emit(0x00c9c20f66c8280f); //movaps xmm1,xmm0; cmpeqpd xmm1,xmm1
		emit(uint16_t(0x540f)); //andps  xmm0,xmm1
		emitByte(0xc1);
		gencf(instr);
	}

	void JitCompilerX86::h_FPDIV(Instruction& instr, int i) {
		genaf(instr);
		genbf(instr, 0x5e);
		emit(0x00c9c20f66c8280f); //movaps xmm1,xmm0; cmpeqpd xmm1,xmm1
		emit(uint16_t(0x540f)); //andps  xmm0,xmm1
		emitByte(0xc1);
		gencf(instr);
	}

	void JitCompilerX86::h_FPSQRT(Instruction& instr, int i) {
		genaf(instr);
		emit(0xc0510f66c2540f41); //andps  xmm0,xmm10; sqrtpd xmm0,xmm0
		gencf(instr);
	}

	void JitCompilerX86::h_FPROUND(Instruction& instr, int i) {
		genar(instr);
		emit(0x81480de0c1c88b48);
		emit(0x600025fffff800e1);
		emit(uint16_t(0x0000));
		emitByte(0xf2);
		int regc = (instr.regc % RegistersCount);
		if (regc <= 1) {
			emitByte(0x4c); //REX
		}
		else {
			emitByte(0x48); //REX
		}
		emit(uint16_t(0x2a0f));
		emitByte(0xc1 + 8 * regc);
		emitByte(0x0d);
		emit(0xf824448900009fc0);
		emit(0x2454ae0f); //ldmxcsr DWORD PTR [rsp-0x8]
		emitByte(0xf8);
		gencf(instr, true);
	}

	static inline uint8_t jumpCondition(Instruction& instr, bool invert = false) {
		switch ((instr.locb & 7) ^ invert)
		{
			case 0:
				return 0x76; //jbe
			case 1:
				return 0x77; //ja
			case 2:
				return 0x78; //js
			case 3:
				return 0x79; //jns
			case 4:
				return 0x70; //jo
			case 5:
				return 0x71; //jno
			case 6:
				return 0x7c; //jl
			case 7:
				return 0x7d; //jge
		}
	}

	void JitCompilerX86::h_CALL(Instruction& instr, int i) {
		genar(instr);
		emit(uint16_t(0x8141)); //cmp regb, imm32
		emitByte(0xf8 + (instr.regb % RegistersCount));
		emit(instr.imm32);
		emitByte(jumpCondition(instr));
		if ((instr.locc & 7) <= 3) {
			emitByte(0x16);
		}
		else {
			emitByte(0x05);
		}
		gencr(instr);
		emit(uint16_t(0x06eb)); //jmp to next
		emitByte(0x50); //push rax
		emitByte(0xe8); //call
		i = wrapInstr(i + (instr.imm8 & 127) + 2);
		if (i < instructionOffsets.size()) {
			emit(instructionOffsets[i] - (codePos + 4));
		}
		else {
			callOffsets.push_back(CallOffset(codePos, i));
			codePos += 4;
		}
	}

	void JitCompilerX86::h_RET(Instruction& instr, int i) {
		genar(instr);
		int crlen = 0;
		if ((instr.locc & 7) <= 3) {
			crlen = 17;
		}
		emit(0x74e53b48); //cmp rsp, rbp; je
		emitByte(11 + crlen);
		emitByte(0x48);
		emit(0x08244433); //xor rax,QWORD PTR [rsp+0x8]
		gencr(instr);
		emitByte(0xc2); //ret 8
		emit(uint16_t(0x0008));
		gencr(instr);
	}

#include "instructionWeights.hpp"
#define INST_HANDLE(x) REPN(&JitCompilerX86::h_##x, WT(x))

	InstructionGeneratorX86 JitCompilerX86::engine[256] = {
		INST_HANDLE(ADD_64)
		INST_HANDLE(ADD_32)
		INST_HANDLE(SUB_64)
		INST_HANDLE(SUB_32)
		INST_HANDLE(MUL_64)
		INST_HANDLE(MULH_64)
		INST_HANDLE(MUL_32)
		INST_HANDLE(IMUL_32)
		INST_HANDLE(IMULH_64)
		INST_HANDLE(DIV_64)
		INST_HANDLE(IDIV_64)
		INST_HANDLE(AND_64)
		INST_HANDLE(AND_32)
		INST_HANDLE(OR_64)
		INST_HANDLE(OR_32)
		INST_HANDLE(XOR_64)
		INST_HANDLE(XOR_32)
		INST_HANDLE(SHL_64)
		INST_HANDLE(SHR_64)
		INST_HANDLE(SAR_64)
		INST_HANDLE(ROL_64)
		INST_HANDLE(ROR_64)
		INST_HANDLE(FPADD)
		INST_HANDLE(FPSUB)
		INST_HANDLE(FPMUL)
		INST_HANDLE(FPDIV)
		INST_HANDLE(FPSQRT)
		INST_HANDLE(FPROUND)
		INST_HANDLE(CALL)
		INST_HANDLE(RET)
	};

#endif
}