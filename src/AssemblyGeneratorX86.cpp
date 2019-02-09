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
//#define TRACE
#define MAGIC_DIVISION
#include "AssemblyGeneratorX86.hpp"
#include "common.hpp"
#include "instructions.hpp"
#ifdef MAGIC_DIVISION
#include "divideByConstantCodegen.h"
#endif
#include "Program.hpp"

namespace RandomX {

	static const char* regR[8] = { "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15" };
	static const char* regR32[8] = { "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d" };
	static const char* regFE[8] = { "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7" };
	static const char* regF[4] = { "xmm0", "xmm1", "xmm2", "xmm3" };
	static const char* regE[4] = { "xmm4", "xmm5", "xmm6", "xmm7" };
	static const char* regA[4] = { "xmm8", "xmm9", "xmm10", "xmm11" };

	static const char* fsumInstr[4] = { "paddb", "paddw", "paddd", "paddq" };

	static const char* regA4 = "xmm12";
	static const char* dblMin = "xmm13";
	static const char* absMask = "xmm14";
	static const char* signMask = "xmm15";
	static const char* regMx = "rbp";
	static const char* regIc = "rbx";
	static const char* regIc32 = "ebx";
	static const char* regIc8 = "bl";
	static const char* regDatasetAddr = "rdi";
	static const char* regScratchpadAddr = "rsi";

	void AssemblyGeneratorX86::generateProgram(Program& prog) {
		asmCode.str(std::string()); //clear
		for (unsigned i = 0; i < ProgramLength; ++i) {
			Instruction& instr = prog(i);
			instr.src %= RegistersCount;
			instr.dst %= RegistersCount;
			generateCode(instr, i);
			//asmCode << std::endl;
		}
	}

	void AssemblyGeneratorX86::generateCode(Instruction& instr, int i) {
		asmCode << "\t; " << instr;
		auto generator = engine[instr.opcode];
		(this->*generator)(instr, i);
	}

	void AssemblyGeneratorX86::genAddressReg(Instruction& instr, const char* reg = "eax") {
		asmCode << "\tmov " << reg << ", " << regR32[instr.src] << std::endl;
		asmCode << "\tand " << reg << ", " << ((instr.mod % 4) ? ScratchpadL1Mask : ScratchpadL2Mask) << std::endl;
	}

	void AssemblyGeneratorX86::genAddressRegDst(Instruction& instr, int maskAlign = 8) {
		asmCode << "\tmov eax" << ", " << regR32[instr.dst] << std::endl;
		asmCode << "\tand eax" << ", " << ((instr.mod % 4) ? (ScratchpadL1Mask & (-maskAlign)) : (ScratchpadL2Mask & (-maskAlign))) << std::endl;
	}

	int32_t AssemblyGeneratorX86::genAddressImm(Instruction& instr) {
		return (int32_t)instr.imm32 & ScratchpadL3Mask;
	}

	//1 uOP
	void AssemblyGeneratorX86::h_IADD_R(Instruction& instr, int i) {
		if (instr.src != instr.dst) {
			asmCode << "\tadd " << regR[instr.dst] << ", " << regR[instr.src] << std::endl;
		}
		else {
			asmCode << "\tadd " << regR[instr.dst] << ", " << (int32_t)instr.imm32 << std::endl;
		}
	}

	//2.75 uOP
	void AssemblyGeneratorX86::h_IADD_M(Instruction& instr, int i) {
		if (instr.src != instr.dst) {
			genAddressReg(instr);
			asmCode << "\tadd " << regR[instr.dst] << ", qword ptr [rsi+rax]" << std::endl;
		}
		else {
			asmCode << "\tadd " << regR[instr.dst] << ", qword ptr [rsi+" << genAddressImm(instr) << "]" << std::endl;
		}
	}

	//1 uOP
	void AssemblyGeneratorX86::h_IADD_RC(Instruction& instr, int i) {
		asmCode << "\tlea " << regR[instr.dst] << ", [" << regR[instr.dst] << "+" << regR[instr.src] << std::showpos << (int32_t)instr.imm32 << std::noshowpos << "]" << std::endl;
	}

	//1 uOP
	void AssemblyGeneratorX86::h_ISUB_R(Instruction& instr, int i) {
		if (instr.src != instr.dst) {
			asmCode << "\tsub " << regR[instr.dst] << ", " << regR[instr.src] << std::endl;
		}
		else {
			asmCode << "\tsub " << regR[instr.dst] << ", " << (int32_t)instr.imm32 << std::endl;
		}
	}

	//2.75 uOP
	void AssemblyGeneratorX86::h_ISUB_M(Instruction& instr, int i) {
		if (instr.src != instr.dst) {
			genAddressReg(instr);
			asmCode << "\tsub " << regR[instr.dst] << ", qword ptr [rsi+rax]" << std::endl;
		}
		else {
			asmCode << "\tsub " << regR[instr.dst] << ", qword ptr [rsi+" << genAddressImm(instr) << "]" << std::endl;
		}
	}

	//1 uOP
	void AssemblyGeneratorX86::h_IMUL_9C(Instruction& instr, int i) {
		asmCode << "\tlea " << regR[instr.dst] << ", [" << regR[instr.dst] << "+" << regR[instr.dst] << "*8" << std::showpos << (int32_t)instr.imm32 << std::noshowpos << "]" << std::endl;
	}

	//1 uOP
	void AssemblyGeneratorX86::h_IMUL_R(Instruction& instr, int i) {
		if (instr.src != instr.dst) {
			asmCode << "\timul " << regR[instr.dst] << ", " << regR[instr.src] << std::endl;
		}
		else {
			asmCode << "\timul " << regR[instr.dst] << ", " << (int32_t)instr.imm32 << std::endl;
		}
	}

	//2.75 uOP
	void AssemblyGeneratorX86::h_IMUL_M(Instruction& instr, int i) {
		if (instr.src != instr.dst) {
			genAddressReg(instr);
			asmCode << "\timul " << regR[instr.dst] << ", qword ptr [rsi+rax]" << std::endl;
		}
		else {
			asmCode << "\timul " << regR[instr.dst] << ", qword ptr [rsi+" << genAddressImm(instr) << "]" << std::endl;
		}
	}

	//4 uOPs
	void AssemblyGeneratorX86::h_IMULH_R(Instruction& instr, int i) {
		asmCode << "\tmov rax, " << regR[instr.dst] << std::endl;
		asmCode << "\tmul " << regR[instr.src] << std::endl;
		asmCode << "\tmov " << regR[instr.dst] << ", rdx" << std::endl;
	}

	//5.75 uOPs
	void AssemblyGeneratorX86::h_IMULH_M(Instruction& instr, int i) {
		if (instr.src != instr.dst) {
			genAddressReg(instr, "ecx");
			asmCode << "\tmov rax, " << regR[instr.dst] << std::endl;
			asmCode << "\tmul qword ptr [rsi+rcx]" << std::endl;
		}
		else {
			asmCode << "\tmov rax, " << regR[instr.dst] << std::endl;
			asmCode << "\tmul qword ptr [rsi+" << genAddressImm(instr) << "]" << std::endl;
		}
		asmCode << "\tmov " << regR[instr.dst] << ", rdx" << std::endl;
	}

	//4 uOPs
	void AssemblyGeneratorX86::h_ISMULH_R(Instruction& instr, int i) {
		asmCode << "\tmov rax, " << regR[instr.dst] << std::endl;
		asmCode << "\timul " << regR[instr.src] << std::endl;
		asmCode << "\tmov " << regR[instr.dst] << ", rdx" << std::endl;
	}

	//5.75 uOPs
	void AssemblyGeneratorX86::h_ISMULH_M(Instruction& instr, int i) {
		if (instr.src != instr.dst) {
			genAddressReg(instr, "ecx");
			asmCode << "\tmov rax, " << regR[instr.dst] << std::endl;
			asmCode << "\timul qword ptr [rsi+rcx]" << std::endl;
		}
		else {
			asmCode << "\tmov rax, " << regR[instr.dst] << std::endl;
			asmCode << "\timul qword ptr [rsi+" << genAddressImm(instr) << "]" << std::endl;
		}
		asmCode << "\tmov " << regR[instr.dst] << ", rdx" << std::endl;
	}

	//1 uOP
	void AssemblyGeneratorX86::h_INEG_R(Instruction& instr, int i) {
		asmCode << "\tneg " << regR[instr.dst] << std::endl;
	}

	//1 uOP
	void AssemblyGeneratorX86::h_IXOR_R(Instruction& instr, int i) {
		if (instr.src != instr.dst) {
			asmCode << "\txor " << regR[instr.dst] << ", " << regR[instr.src] << std::endl;
		}
		else {
			asmCode << "\txor " << regR[instr.dst] << ", " << (int32_t)instr.imm32 << std::endl;
		}
	}

	//2.75 uOP
	void AssemblyGeneratorX86::h_IXOR_M(Instruction& instr, int i) {
		if (instr.src != instr.dst) {
			genAddressReg(instr);
			asmCode << "\txor " << regR[instr.dst] << ", qword ptr [rsi+rax]" << std::endl;
		}
		else {
			asmCode << "\txor " << regR[instr.dst] << ", qword ptr [rsi+" << genAddressImm(instr) << "]" << std::endl;
		}
	}

	//1.75 uOPs
	void AssemblyGeneratorX86::h_IROR_R(Instruction& instr, int i) {
		if (instr.src != instr.dst) {
			asmCode << "\tmov ecx, " << regR32[instr.src] << std::endl;
			asmCode << "\tror " << regR[instr.dst] << ", cl" << std::endl;
		}
		else {
			asmCode << "\tror " << regR[instr.dst] << ", " << (instr.imm32 & 63) << std::endl;
		}
	}

	//1.75 uOPs
	void AssemblyGeneratorX86::h_IROL_R(Instruction& instr, int i) {
		if (instr.src != instr.dst) {
			asmCode << "\tmov ecx, " << regR32[instr.src] << std::endl;
			asmCode << "\trol " << regR[instr.dst] << ", cl" << std::endl;
		}
		else {
			asmCode << "\trol " << regR[instr.dst] << ", " << (instr.imm32 & 63) << std::endl;
		}
	}

	//~6 uOPs
	void AssemblyGeneratorX86::h_IDIV_C(Instruction& instr, int i) {
		if (instr.imm32 != 0) {
			uint32_t divisor = instr.imm32;
			if (divisor & (divisor - 1)) {
				magicu_info mi = compute_unsigned_magic_info(divisor, sizeof(uint64_t) * 8);
				if (mi.pre_shift == 0 && !mi.increment) {
					asmCode << "\tmov rax, " << mi.multiplier << std::endl;
					asmCode << "\tmul " << regR[instr.dst] << std::endl;
				}
				else {
					asmCode << "\tmov rax, " << regR[instr.dst] << std::endl;
					if (mi.pre_shift > 0)
						asmCode << "\tshr rax, " << mi.pre_shift << std::endl;
					if (mi.increment) {
						asmCode << "\tadd rax, 1" << std::endl;
						asmCode << "\tsbb rax, 0" << std::endl;
					}
					asmCode << "\tmov rcx, " << mi.multiplier << std::endl;
					asmCode << "\tmul rcx" << std::endl;
				}
				if (mi.post_shift > 0)
					asmCode << "\tshr rdx, " << mi.post_shift << std::endl;
				asmCode << "\tadd " << regR[instr.dst] << ", rdx" << std::endl;
			}
			else { //divisor is a power of two
				int shift = 0;
				while (divisor >>= 1)
					++shift;
				if(shift > 0)
					asmCode << "\tshr " << regR[instr.dst] << ", " << shift << std::endl;
			}
		}	
	}

	//~8.5 uOPs
	void AssemblyGeneratorX86::h_ISDIV_C(Instruction& instr, int i) {
		int64_t divisor = (int32_t)instr.imm32;
		if ((divisor & -divisor) == divisor || (divisor & -divisor) == -divisor) {
			asmCode << "\tmov rax, " << regR[instr.dst] << std::endl;
			// +/- power of two
			bool negative = divisor < 0;
			if (negative)
				divisor = -divisor;
			int shift = 0;
			uint64_t unsignedDivisor = divisor;
			while (unsignedDivisor >>= 1)
				++shift;
			if (shift > 0) {
				asmCode << "\tmov rcx, rax" << std::endl;
				asmCode << "\tsar rcx, 63" << std::endl;
				uint32_t mask = (1ULL << shift) + 0xFFFFFFFF;
				asmCode << "\tand ecx, 0" << std::hex << mask << std::dec << "h" << std::endl;
				asmCode << "\tadd rax, rcx" << std::endl;
				asmCode << "\tsar rax, " << shift << std::endl;
			}
			if (negative)
				asmCode << "\tneg rax" << std::endl;
			asmCode << "\tadd " << regR[instr.dst] << ", rax" << std::endl;
		}
		else if (divisor != 0) {
			magics_info mi = compute_signed_magic_info(divisor);
			asmCode << "\tmov rax, " << mi.multiplier << std::endl;
			asmCode << "\timul " << regR[instr.dst] << std::endl;
			//asmCode << "\tmov rax, rdx" << std::endl;
			asmCode << "\txor eax, eax" << std::endl;
			bool haveSF = false;
			if (divisor > 0 && mi.multiplier < 0) {
				asmCode << "\tadd rdx, " << regR[instr.dst] << std::endl;
				haveSF = true;
			}
			if (divisor < 0 && mi.multiplier > 0) {
				asmCode << "\tsub rdx, " << regR[instr.dst] << std::endl;
				haveSF = true;
			}
			if (mi.shift > 0) {
				asmCode << "\tsar rdx, " << mi.shift << std::endl;
				haveSF = true;
			}
			if (!haveSF)
				asmCode << "\ttest rdx, rdx" << std::endl;
			asmCode << "\tsets al" << std::endl;
			asmCode << "\tadd rdx, rax" << std::endl;
			asmCode << "\tadd " << regR[instr.dst] << ", rdx" << std::endl;
		}
	}

	//2 uOPs
	void AssemblyGeneratorX86::h_ISWAP_R(Instruction& instr, int i) {
		if (instr.src != instr.dst) {
			asmCode << "\txchg " << regR[instr.dst] << ", " << regR[instr.src] << std::endl;
		}
	}

	//1 uOPs
	void AssemblyGeneratorX86::h_FSWAP_R(Instruction& instr, int i) {
		asmCode << "\tshufpd " << regFE[instr.dst] << ", " << regFE[instr.dst] << ", 1" << std::endl;
	}

	//1 uOP
	void AssemblyGeneratorX86::h_FADD_R(Instruction& instr, int i) {
		instr.dst %= 4;
		instr.src %= 4;
		asmCode << "\taddpd " << regF[instr.dst] << ", " << regA[instr.src] << std::endl;
		//asmCode << "\t" << fsumInstr[instr.mod % 4] << " " << signMask << ", " << regF[instr.dst] << std::endl;
	}

	//5 uOPs
	void AssemblyGeneratorX86::h_FADD_M(Instruction& instr, int i) {
		instr.dst %= 4;
		genAddressReg(instr);
		asmCode << "\tcvtdq2pd xmm12, qword ptr [rsi+rax]" << std::endl;
		asmCode << "\taddpd " << regF[instr.dst] << ", xmm12" << std::endl;
	}

	//1 uOP
	void AssemblyGeneratorX86::h_FSUB_R(Instruction& instr, int i) {
		instr.dst %= 4;
		instr.src %= 4;
		asmCode << "\tsubpd " << regF[instr.dst] << ", " << regA[instr.src] << std::endl;
		//asmCode << "\t" << fsumInstr[instr.mod % 4] << " " << signMask << ", " << regF[instr.dst] << std::endl;
	}

	//5 uOPs
	void AssemblyGeneratorX86::h_FSUB_M(Instruction& instr, int i) {
		instr.dst %= 4;
		genAddressReg(instr);
		asmCode << "\tcvtdq2pd xmm12, qword ptr [rsi+rax]" << std::endl;
		asmCode << "\tsubpd " << regF[instr.dst] << ", xmm12" << std::endl;
	}

	//1 uOP
	void AssemblyGeneratorX86::h_FNEG_R(Instruction& instr, int i) {
		instr.dst %= 4;
		asmCode << "\txorps " << regF[instr.dst] << ", " << signMask << std::endl;
	}

	//1 uOPs
	void AssemblyGeneratorX86::h_FMUL_R(Instruction& instr, int i) {
		instr.dst %= 4;
		instr.src %= 4;
		asmCode << "\tmulpd " << regE[instr.dst] << ", " << regA[instr.src] << std::endl;
	}

	//7 uOPs
	void AssemblyGeneratorX86::h_FMUL_M(Instruction& instr, int i) {
		instr.dst %= 4;
		genAddressReg(instr);
		asmCode << "\tcvtdq2pd xmm12, qword ptr [rsi+rax]" << std::endl;
		asmCode << "\tandps xmm12, xmm14" << std::endl;
		asmCode << "\tmulpd " << regE[instr.dst] << ", xmm12" << std::endl;
		asmCode << "\tmaxpd " << regE[instr.dst] << ", " << dblMin << std::endl;
	}

	//2 uOPs
	void AssemblyGeneratorX86::h_FDIV_R(Instruction& instr, int i) {
		instr.dst %= 4;
		instr.src %= 4;
		asmCode << "\tdivpd " << regE[instr.dst] << ", " << regA[instr.src] << std::endl;
		asmCode << "\tmaxpd " << regE[instr.dst] << ", " << dblMin << std::endl;
	}

	//7 uOPs
	void AssemblyGeneratorX86::h_FDIV_M(Instruction& instr, int i) {
		instr.dst %= 4;
		genAddressReg(instr);
		asmCode << "\tcvtdq2pd xmm12, qword ptr [rsi+rax]" << std::endl;
		asmCode << "\tandps xmm12, xmm14" << std::endl;
		asmCode << "\tdivpd " << regE[instr.dst] << ", xmm12" << std::endl;
		asmCode << "\tmaxpd " << regE[instr.dst] << ", " << dblMin << std::endl;
	}

	//1 uOP
	void AssemblyGeneratorX86::h_FSQRT_R(Instruction& instr, int i) {
		instr.dst %= 4;
		asmCode << "\tsqrtpd " << regE[instr.dst] << ", " << regE[instr.dst] << std::endl;
	}	

	//6 uOPs
	void AssemblyGeneratorX86::h_CFROUND(Instruction& instr, int i) {
		asmCode << "\tmov rax, " << regR[instr.src] << std::endl;
		int rotate = (13 - (instr.imm32 & 63)) & 63;
		if (rotate != 0)
			asmCode << "\trol rax, " << rotate << std::endl;
		asmCode << "\tand eax, 24576" << std::endl;
		asmCode << "\tor eax, 40896" << std::endl;
		asmCode << "\tmov dword ptr [rsp-8], eax" << std::endl;
		asmCode << "\tldmxcsr dword ptr [rsp-8]" << std::endl;
	}

	static inline const char* condition(Instruction& instr, bool invert = false) {
		switch (((instr.mod >> 2) & 7) ^ invert)
		{
			case 0:
				return "be";
			case 1:
				return "a";
			case 2:
				return "s";
			case 3:
				return "ns";
			case 4:
				return "o";
			case 5:
				return "no";
			case 6:
				return "l";
			case 7:
				return "ge";
		}
	}

	//4 uOPs
	void AssemblyGeneratorX86::h_COND_R(Instruction& instr, int i) {
		asmCode << "\txor ecx, ecx" << std::endl;
		asmCode << "\tcmp " << regR32[instr.src] << ", " << (int32_t)instr.imm32 << std::endl;
		asmCode << "\tset" << condition(instr) << " cl" << std::endl;
		asmCode << "\tadd " << regR[instr.dst] << ", rcx" << std::endl;
	}

	//6 uOPs
	void AssemblyGeneratorX86::h_COND_M(Instruction& instr, int i) {
		asmCode << "\txor ecx, ecx" << std::endl;
		genAddressReg(instr);
		asmCode << "\tcmp dword ptr [rsi+rax], " << (int32_t)instr.imm32 << std::endl;
		asmCode << "\tset" << condition(instr) << " cl" << std::endl;
		asmCode << "\tadd " << regR[instr.dst] << ", rcx" << std::endl;
	}

	//3 uOPs
	void AssemblyGeneratorX86::h_ISTORE(Instruction& instr, int i) {
		genAddressRegDst(instr);
		asmCode << "\tmov qword ptr [rsi+rax], " << regR[instr.src] << std::endl;
	}

	//3 uOPs
	void AssemblyGeneratorX86::h_FSTORE(Instruction& instr, int i) {
		genAddressRegDst(instr, 16);
		asmCode << "\tmovapd xmmword ptr [rsi+rax], " << regFE[instr.src] << std::endl;
	}

	void AssemblyGeneratorX86::h_NOP(Instruction& instr, int i) {
		asmCode << "\tnop" << std::endl;
	}

#include "instructionWeights.hpp"
#define INST_HANDLE(x) REPN(&AssemblyGeneratorX86::h_##x, WT(x))

	InstructionGenerator AssemblyGeneratorX86::engine[256] = {
		//Integer
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
		INST_HANDLE(IDIV_C)
		INST_HANDLE(ISDIV_C)
		INST_HANDLE(INEG_R)
		INST_HANDLE(IXOR_R)
		INST_HANDLE(IXOR_M)
		INST_HANDLE(IROR_R)
		INST_HANDLE(IROL_R)
		INST_HANDLE(ISWAP_R)

		//Common floating point
		INST_HANDLE(FSWAP_R)

		//Floating point group F
		INST_HANDLE(FADD_R)
		INST_HANDLE(FADD_M)
		INST_HANDLE(FSUB_R)
		INST_HANDLE(FSUB_M)
		INST_HANDLE(FNEG_R)

		//Floating point group E
		INST_HANDLE(FMUL_R)
		INST_HANDLE(FMUL_M)
		INST_HANDLE(FDIV_R)
		INST_HANDLE(FDIV_M)
		INST_HANDLE(FSQRT_R)

		//Control
		INST_HANDLE(COND_R)
		INST_HANDLE(COND_M)
		INST_HANDLE(CFROUND)

		INST_HANDLE(ISTORE)
		INST_HANDLE(FSTORE)

		INST_HANDLE(NOP)
	};
}