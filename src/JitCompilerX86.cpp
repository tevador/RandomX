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

	/*
	 REGISTER ALLOCATION:

	 rax -> temporary
	 rbx -> MemoryRegisters& memory
	 rcx -> temporary
	 rdx -> temporary
	 rsi -> convertible_t& scratchpad
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

	constexpr uint8_t ic3 = (InstructionCount >> 24);
	constexpr uint8_t ic2 = (InstructionCount >> 16);
	constexpr uint8_t ic1 = (InstructionCount >> 8);
	constexpr uint8_t ic0 = (InstructionCount >> 0);

	const uint8_t prologue[] = {
	   0x53,                                        //push   rbx
	   0x55,                                        //push   rbp
#ifdef _WIN32
	   0x57,                                        //push   rdi
	   0x56,                                        //push   rsi
#endif
	   0x41, 0x54,                                  //push   r12
	   0x41, 0x55,                                  //push   r13
	   0x41, 0x56,                                  //push   r14
	   0x41, 0x57,                                  //push   r15
#ifdef _WIN32
	   0x48, 0x83, 0xec, 0x48,                      //sub    rsp,0x48
	   0xf3, 0x0f, 0x7f, 0x74, 0x24, 0x30,          //movdqu XMMWORD PTR[rsp + 0x30],xmm6
	   0xf3, 0x0f, 0x7f, 0x7c, 0x24, 0x20,          //movdqu XMMWORD PTR[rsp + 0x20],xmm7
	   0xf3, 0x44, 0x0f, 0x7f, 0x44, 0x24, 0x10,    //movdqu XMMWORD PTR[rsp + 0x10],xmm8
	   0xf3, 0x44, 0x0f, 0x7f, 0x0c, 0x24,          //movdqu XMMWORD PTR[rsp],xmm9
	   0x51,                                        //push   rcx
	   0x48, 0x8b, 0xda,                            //mov    rbx,rdx
	   0x49, 0x8b, 0xf0,                            //mov    rsi,r8
#else
	   0x57,                                        //push   rdi
	   0x48, 0x8b, 0xde,                            //mov    rbx, rsi
	   0x48, 0x8b, 0xf2,                            //mov    rsi, rdx
	   0x48, 0x8b, 0xcf,                            //mov    rcx, rdi
#endif
	   0x48, 0x8b, 0xec,                            //mov    rbp,rsp
	   0x48, 0xc7, 0xc7, ic0, ic1, ic2, ic3,        //mov    rdi, "InstructionCount"
	   0x4c, 0x8b, 0x01,                            //mov    r8,QWORD PTR[rcx]
	   0x4c, 0x8b, 0x49, 0x08,                      //mov    r9,QWORD PTR[rcx+0x8]
	   0x4c, 0x8b, 0x51, 0x10,                      //mov    r10,QWORD PTR[rcx+0x10]
	   0x4c, 0x8b, 0x59, 0x18,                      //mov    r11,QWORD PTR[rcx+0x18]
	   0x4c, 0x8b, 0x61, 0x20,                      //mov    r12,QWORD PTR[rcx+0x20]
	   0x4c, 0x8b, 0x69, 0x28,                      //mov    r13,QWORD PTR[rcx+0x28]
	   0x4c, 0x8b, 0x71, 0x30,                      //mov    r14,QWORD PTR[rcx+0x30]
	   0x4c, 0x8b, 0x79, 0x38,                      //mov    r15,QWORD PTR[rcx+0x38]
	   0xc7, 0x44, 0x24, 0xf8, 0xc0, 0x9f, 0x00,    //mov    DWORD PTR[rsp-0x8],0x9fc0
	   0x00,
	   0x0f, 0xae, 0x54, 0x24, 0xf8,                //ldmxcsr DWORD PTR[rsp-0x8]
	   0xf2, 0x4c, 0x0f, 0x2a, 0x41, 0x40,          //cvtsi2sd xmm8,QWORD PTR[rcx+0x40]
	   0xf2, 0x4c, 0x0f, 0x2a, 0x49, 0x48,          //cvtsi2sd xmm9,QWORD PTR[rcx+0x48]
	   0xf2, 0x48, 0x0f, 0x2a, 0x51, 0x50,          //cvtsi2sd xmm2,QWORD PTR[rcx+0x50]
	   0xf2, 0x48, 0x0f, 0x2a, 0x59, 0x58,          //cvtsi2sd xmm3,QWORD PTR[rcx+0x58]
	   0xf2, 0x48, 0x0f, 0x2a, 0x61, 0x60,          //cvtsi2sd xmm4,QWORD PTR[rcx+0x60]
	   0xf2, 0x48, 0x0f, 0x2a, 0x69, 0x68,          //cvtsi2sd xmm5,QWORD PTR[rcx+0x68]
	   0xf2, 0x48, 0x0f, 0x2a, 0x71, 0x70,          //cvtsi2sd xmm6,QWORD PTR[rcx+0x70]
	   0xf2, 0x48, 0x0f, 0x2a, 0x79, 0x78,          //cvtsi2sd xmm7,QWORD PTR[rcx+0x78]
	};

	const uint8_t epilogue[] = {
	   0x48, 0x8b, 0xe5,                         //mov    rsp,rbp
	   0x59,                                     //pop    rcx
	   0x4c, 0x89, 0x01,                         //mov    QWORD PTR [rcx],r8
	   0x4c, 0x89, 0x49, 0x08,                   //mov    QWORD PTR [rcx+0x8],r9
	   0x4c, 0x89, 0x51, 0x10,                   //mov    QWORD PTR [rcx+0x10],r10
	   0x4c, 0x89, 0x59, 0x18,                   //mov    QWORD PTR [rcx+0x18],r11
	   0x4c, 0x89, 0x61, 0x20,                   //mov    QWORD PTR [rcx+0x20],r12
	   0x4c, 0x89, 0x69, 0x28,                   //mov    QWORD PTR [rcx+0x28],r13
	   0x4c, 0x89, 0x71, 0x30,                   //mov    QWORD PTR [rcx+0x30],r14
	   0x4c, 0x89, 0x79, 0x38,                   //mov    QWORD PTR [rcx+0x38],r15
	   0x66, 0x4c, 0x0f, 0x7e, 0x41, 0x40,       //movq   QWORD PTR [rcx+0x40],xmm8
	   0x66, 0x4c, 0x0f, 0x7e, 0x49, 0x48,       //movq   QWORD PTR [rcx+0x48],xmm9
	   0x66, 0x48, 0x0f, 0x7e, 0x51, 0x50,       //movq   QWORD PTR [rcx+0x50],xmm2
	   0x66, 0x48, 0x0f, 0x7e, 0x59, 0x58,       //movq   QWORD PTR [rcx+0x58],xmm3
	   0x66, 0x48, 0x0f, 0x7e, 0x61, 0x60,       //movq   QWORD PTR [rcx+0x60],xmm4
	   0x66, 0x48, 0x0f, 0x7e, 0x69, 0x68,       //movq   QWORD PTR [rcx+0x68],xmm5
	   0x66, 0x48, 0x0f, 0x7e, 0x71, 0x70,       //movq   QWORD PTR [rcx+0x70],xmm6
	   0x66, 0x48, 0x0f, 0x7e, 0x79, 0x78,       //movq   QWORD PTR [rcx+0x78],xmm7
#ifdef _WIN32
	   0xf3, 0x44, 0x0f, 0x6f, 0x0c, 0x24,       //movdqu xmm9,XMMWORD PTR [rsp]
	   0xf3, 0x44, 0x0f, 0x6f, 0x44, 0x24, 0x10, //movdqu xmm8,XMMWORD PTR [rsp+0x10]
	   0xf3, 0x0f, 0x6f, 0x7c, 0x24, 0x20,       //movdqu xmm7,XMMWORD PTR [rsp+0x20]
	   0xf3, 0x0f, 0x6f, 0x74, 0x24, 0x30,       //movdqu xmm6,XMMWORD PTR [rsp+0x30]
	   0x48, 0x83, 0xc4, 0x48,                   //add    rsp,0x48
#endif
	   0x41, 0x5f,                               //pop    r15
	   0x41, 0x5e,                               //pop    r14
	   0x41, 0x5d,                               //pop    r13
	   0x41, 0x5c,                               //pop    r12
#ifdef _WIN32
	   0x5e,                                     //pop    rsi
	   0x5f,                                     //pop    rdi
#endif
	   0x5d,                                     //pop    rbp
	   0x5b,                                     //pop    rbx
	   0xc3,                                     //ret
	};

	//41 bytes -> 1 cache line
	const uint8_t readDatasetSub[] = {
	   0x8b, 0x13,                                  //mov    edx,DWORD PTR [rbx]
	   0x48, 0x8b, 0x43, 0x08,                      //mov    rax,QWORD PTR [rbx+0x8]
	   0x48, 0x8b, 0x04, 0x10,                      //mov    rax,QWORD PTR [rax+rdx*1]
	   0x83, 0x03, 0x08,                            //add    DWORD PTR [rbx],0x8
	   0x33, 0x4b, 0x04,                            //xor    ecx,DWORD PTR [rbx+0x4]
	   0x89, 0x4b, 0x04,                            //mov    DWORD PTR [rbx+0x4],ecx
	   0xf7, 0xc1, 0xf8, 0xff, 0x00, 0x00,          //test   ecx,0xfff8
	   0x75, 0x0d,                                  //jne
	   0x83, 0xe1, 0xf8,                            //and    ecx,0xfffffff8
	   0x89, 0x0b,                                  //mov    DWORD PTR [rbx],ecx
	   0x48, 0x8b, 0x53, 0x08,                      //mov    rdx,QWORD PTR [rbx+0x8]
	   0x0f, 0x18, 0x0c, 0x0a,                      //prefetcht0 BYTE PTR [rdx+rcx*1]
	   0xc3,                                        //ret
	};

	constexpr int getNumCacheLines(size_t size) {
		return (size + (CacheLineSize - 1)) / CacheLineSize;
	}

	constexpr int32_t align(int32_t pos, int32_t align) {
		return ((pos - 1) / align + 1) * align;
	}

	constexpr int32_t readDatasetSubOffset = CodeSize - CacheLineSize * getNumCacheLines(sizeof(readDatasetSub));
	constexpr int32_t epilogueOffset = readDatasetSubOffset - CacheLineSize * getNumCacheLines(sizeof(epilogue));
	constexpr int32_t startOffsetAligned = align(sizeof(prologue), CacheLineSize);

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
		memcpy(code, prologue, sizeof(prologue));
		codePos = sizeof(prologue);
		if (startOffsetAligned - codePos > 4) {
			emitByte(0xeb);
			emitByte(startOffsetAligned - (codePos + 1));
		}
		else {
			while (codePos < startOffsetAligned)
				emitByte(0x90); //nop
		}
		memcpy(code + readDatasetSubOffset, readDatasetSub, sizeof(readDatasetSub));
		memcpy(code + epilogueOffset, epilogue, sizeof(epilogue));
	}

	void JitCompilerX86::generateProgram(Pcg32& gen) {
		instructionOffsets.clear();
		callOffsets.clear();
		codePos = startOffsetAligned;
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
		emit(0x880fcfff); //dec edx; js <epilogue>
		emit(epilogueOffset - (codePos + 4)); //jump offset (RIP-relative)
		gena(instr);
		auto generator = engine[instr.opcode];
		(this->*generator)(instr, i);
	}

	void JitCompilerX86::fixCallOffsets() {
		for (CallOffset& co : callOffsets) {
			*reinterpret_cast<int32_t*>(code + co.pos) = instructionOffsets[co.index] - (co.pos + 4);
		}
	}

	void JitCompilerX86::gena(Instruction& instr) {
		emit(uint16_t(0x8149)); //xor
		emitByte(0xf0 + (instr.rega % RegistersCount));
		emit(instr.addra);
		int32_t pc;
		switch (instr.loca & 7)
		{
			case 0:
			case 1:
			case 2:
			case 3:
				emit(uint16_t(0x8b41)); //mov
				emitByte(0xc8 + (instr.rega % RegistersCount)); //ecx, rega
				emitByte(0xe8); //call
				emit(readDatasetSubOffset - (codePos + 4));
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

	void JitCompilerX86::genbr0(Instruction& instr, uint16_t opcodeReg, uint16_t opcodeImm) {
		if ((instr.locb & 7) <= 5) {
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
		emit(0x48f2fffff8002548); //and rax,0xfffffffffffff800; cvtsi2sd xmm0,rax
		emit(uint16_t(0x2a0f));
		emitByte(0xc0);
		if ((instr.locb & 7) <= 5) {
			int regb = (instr.regb % RegistersCount);
			emitByte(0xf2); //xxxsd  xmm0,regb
			if (regb <= 1) {
				emitByte(0x41); //REX
			}
			emitByte(0x0f);
			emitByte(opcode);
			emitByte(0xc0 + regb);
		}
		else {
			convertible_t bimm;
			bimm.f64 = (double)instr.imm32;
			emit(uint16_t(0xb848)); //movabs rax,imm64
			emit(bimm.i64);
			emitByte(0x66); //movq xmm1,rax
			emit(0xc86e0f48);
			emit(uint16_t(0x0ff2)); //xxxsd xmm0,xmm1
			emitByte(opcode);
			emitByte(0xc1);
		}
	}

	void JitCompilerX86::gencr(Instruction& instr) {
		switch (instr.locc & 7)
		{
		case 0:
			emit(0x41c88b48); //mov rcx, rax; REX
			emitByte(0x8b); // mov
			emitByte(0xc0 + (instr.regc % RegistersCount)); //eax, regc
			emitByte(0x35); // xor eax
			emit(instr.addrc);
			emitByte(0x25); //and
			emit(ScratchpadL2 - 1); //whole scratchpad
			emit(0xc60c8948); // mov    QWORD PTR [rsi+rax*8],rcx
			break;

		case 1:
		case 2:
		case 3:
			emit(0x41c88b48); //mov rcx, rax; REX
			emitByte(0x8b); // mov
			emitByte(0xc0 + (instr.regc % RegistersCount)); //eax, regc
			emitByte(0x35); // xor eax
			emit(instr.addrc);
			emitByte(0x25); //and
			emit(ScratchpadL1 - 1); //first 16 KiB of scratchpad
			emit(0xc60c8948); // mov    QWORD PTR [rsi+rax*8],rcx
			break;

		default:
			emit(uint16_t(0x8b4c)); //mov
			emitByte(0xc0 + 8 * (instr.regc % RegistersCount)); //regc, rax
			break;
		}
	}

	void JitCompilerX86::gencf(Instruction& instr) {
		int regc = (instr.regc % RegistersCount);
		switch (instr.locc & 7)
		{
		case 0:
			emit(uint16_t(0x8b41)); //mov
			emitByte(0xc0 + regc); //eax, regc
			emitByte(0x35); // xor eax
			emit(instr.addrc);
			emitByte(0x25); //and
			emit(ScratchpadL2 - 1); //whole scratchpad
			emit(uint16_t(0x4866)); //prefix
			emit(0xc6047e0f); // movq   QWORD PTR [rsi+rax*8],xmm0
			break;

		case 1:
		case 2:
		case 3:
			emit(uint16_t(0x8b41)); //mov
			emitByte(0xc0 + regc); //eax, regc
			emitByte(0x35); // xor eax
			emit(instr.addrc);
			emitByte(0x25); //and
			emit(ScratchpadL1 - 1); //first 16 KiB of scratchpad
			emit(uint16_t(0x4866)); //prefix
			emit(0xc6047e0f); // movq   QWORD PTR [rsi+rax*8],xmm0
			break;

		default:
			emitByte(0xf2);
			if (regc <= 1) {
				emitByte(0x44); //REX
			}
			emit(uint16_t(0x100f)); //movsd
			emitByte(0xc0 + 8 * regc); // regc, xmm0
			break;
		}
	}

	void JitCompilerX86::h_ADD_64(Instruction& instr, int i) {
		genbr1(instr, 0x0349, 0x0548);
		gencr(instr);
	}

	void JitCompilerX86::h_ADD_32(Instruction& instr, int i) {
		genbr132(instr, 0x0341, 0x05);
		gencr(instr);
	}

	void JitCompilerX86::h_SUB_64(Instruction& instr, int i) {
		genbr1(instr, 0x2b49, 0x2d48);
		gencr(instr);
	}

	void JitCompilerX86::h_SUB_32(Instruction& instr, int i) {
		genbr132(instr, 0x2b41, 0x2d);
		gencr(instr);
	}

	void JitCompilerX86::h_MUL_64(Instruction& instr, int i) {
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
		genbr1(instr, 0x2349, 0x2548);
		gencr(instr);
	}

	void JitCompilerX86::h_AND_32(Instruction& instr, int i) {
		genbr132(instr, 0x2341, 0x25);
		gencr(instr);
	}

	void JitCompilerX86::h_OR_64(Instruction& instr, int i) {
		genbr1(instr, 0x0b49, 0x0d48);
		gencr(instr);
	}

	void JitCompilerX86::h_OR_32(Instruction& instr, int i) {
		genbr132(instr, 0x0b41, 0x0d);
		gencr(instr);
	}

	void JitCompilerX86::h_XOR_64(Instruction& instr, int i) {
		genbr1(instr, 0x3349, 0x3548);
		gencr(instr);
	}

	void JitCompilerX86::h_XOR_32(Instruction& instr, int i) {
		genbr132(instr, 0x3341, 0x35);
		gencr(instr);
	}

	void JitCompilerX86::h_SHL_64(Instruction& instr, int i) {
		genbr0(instr, 0xe0d3, 0xe0c1);
		gencr(instr);
	}

	void JitCompilerX86::h_SHR_64(Instruction& instr, int i) {
		genbr0(instr, 0xe8d3, 0xe8c1);
		gencr(instr);
	}

	void JitCompilerX86::h_SAR_64(Instruction& instr, int i) {
		genbr0(instr, 0xf8d3, 0xf8c1);
		gencr(instr);
	}

	void JitCompilerX86::h_ROL_64(Instruction& instr, int i) {
		genbr0(instr, 0xc0d3, 0xc0c1);
		gencr(instr);
	}

	void JitCompilerX86::h_ROR_64(Instruction& instr, int i) {
		genbr0(instr, 0xc8d3, 0xc8c1);
		gencr(instr);
	}

	void JitCompilerX86::h_FPADD(Instruction& instr, int i) {
		genbf(instr, 0x58);
		gencf(instr);
	}

	void JitCompilerX86::h_FPSUB(Instruction& instr, int i) {
		genbf(instr, 0x5c);
		gencf(instr);
	}

	void JitCompilerX86::h_FPMUL(Instruction& instr, int i) {
		emit(uint16_t(0x0d48)); //or rax,0x800
		emit(0x00000800);
		genbf(instr, 0x59);
		gencf(instr);
	}

	void JitCompilerX86::h_FPDIV(Instruction& instr, int i) {
		emit(uint16_t(0x0d48)); //or rax,0x800
		emit(0x00000800);
		genbf(instr, 0x5e);
		gencf(instr);
	}

	void JitCompilerX86::h_FPSQRT(Instruction& instr, int i) {
		emit(uint16_t(0xb948)); //or movabs rcx, imm64
		emit(0x7ffffffffffff800);
		emit(0xc02a0f48f2c12348); //and rax,rcx; cvtsi2sd xmm0,rax
		emit(0xc0510ff2); //sqrtsd xmm0,xmm0
		gencf(instr);
	}

	void JitCompilerX86::h_FPROUND(Instruction& instr, int i) {
		emit(0x81480de0c1c88b48);
		emit(0x600025fffff800e1);
		emit(0x0dc12a0f48f20000);
		emit(0xf824448900009fc0);
		emit(0x2454ae0f); //ldmxcsr DWORD PTR [rsp-0x8]
		emitByte(0xf8);
		gencf(instr);
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
		int crlen = 0;
		if ((instr.locc & 7) <= 3) {
			crlen = 17;
		}
		emit(0x74e53b48); //cmp rsp, rbp; je
		emitByte(20 + crlen);
		emit(uint16_t(0x8141)); //cmp regb, imm32
		emitByte(0xf8 + (instr.regb % RegistersCount));
		emit(instr.imm32);
		emitByte(jumpCondition(instr, true));
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
}