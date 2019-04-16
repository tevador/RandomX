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

#include <climits>
#include "AssemblyGeneratorX86.hpp"
#include "common.hpp"
#include "reciprocal.h"
#include "Program.hpp"
#include "superscalarGenerator.hpp"

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
		for (unsigned i = 0; i < 8; ++i) {
			registerUsage[i] = -1;
		}
		asmCode.str(std::string()); //clear
		for (unsigned i = 0; i < prog.getSize(); ++i) {
			asmCode << "randomx_isn_" << i << ":" << std::endl;
			Instruction& instr = prog(i);
			instr.src %= RegistersCount;
			instr.dst %= RegistersCount;
			generateCode(instr, i);
			//asmCode << std::endl;
		}
	}

	void AssemblyGeneratorX86::generateAsm(SuperscalarProgram& prog) {
		asmCode.str(std::string()); //clear
		asmCode << "ALIGN 16" << std::endl;
		for (unsigned i = 0; i < prog.getSize(); ++i) {
			Instruction& instr = prog(i);
			switch (instr.opcode)
			{
			case RandomX::SuperscalarInstructionType::ISUB_R:
				asmCode << "sub " << regR[instr.dst] << ", " << regR[instr.src] << std::endl;
				break;
			case RandomX::SuperscalarInstructionType::IXOR_R:
				asmCode << "xor " << regR[instr.dst] << ", " << regR[instr.src] << std::endl;
				break;
			case RandomX::SuperscalarInstructionType::IADD_RS:
				asmCode << "lea " << regR[instr.dst] << ", [" << regR[instr.dst] << "+" << regR[instr.src] << "*" << (1 << (instr.getModShift2())) << "]" << std::endl;
				break;
			case RandomX::SuperscalarInstructionType::IMUL_R:
				asmCode << "imul " << regR[instr.dst] << ", " << regR[instr.src] << std::endl;
				break;
			case RandomX::SuperscalarInstructionType::IROR_C:
				asmCode << "ror " << regR[instr.dst] << ", " << instr.getImm32() << std::endl;
				break;
			case RandomX::SuperscalarInstructionType::IADD_C7:
				asmCode << "add " << regR[instr.dst] << ", " << (int32_t)instr.getImm32() << std::endl;
				break;
			case RandomX::SuperscalarInstructionType::IXOR_C7:
				asmCode << "xor " << regR[instr.dst] << ", " << (int32_t)instr.getImm32() << std::endl;
				break;
			case RandomX::SuperscalarInstructionType::IADD_C8:
				asmCode << "add " << regR[instr.dst] << ", " << (int32_t)instr.getImm32() << std::endl;
				asmCode << "nop" << std::endl;
				break;
			case RandomX::SuperscalarInstructionType::IXOR_C8:
				asmCode << "xor " << regR[instr.dst] << ", " << (int32_t)instr.getImm32() << std::endl;
				asmCode << "nop" << std::endl;
				break;
			case RandomX::SuperscalarInstructionType::IADD_C9:
				asmCode << "add " << regR[instr.dst] << ", " << (int32_t)instr.getImm32() << std::endl;
				asmCode << "xchg ax, ax ;nop" << std::endl;
				break;
			case RandomX::SuperscalarInstructionType::IXOR_C9:
				asmCode << "xor " << regR[instr.dst] << ", " << (int32_t)instr.getImm32() << std::endl;
				asmCode << "xchg ax, ax ;nop" << std::endl;
				break;
			case RandomX::SuperscalarInstructionType::IMULH_R:
				asmCode << "mov rax, " << regR[instr.dst] << std::endl;
				asmCode << "mul " << regR[instr.src] << std::endl;
				asmCode << "mov " << regR[instr.dst] << ", rdx" << std::endl;
				break;
			case RandomX::SuperscalarInstructionType::ISMULH_R:
				asmCode << "mov rax, " << regR[instr.dst] << std::endl;
				asmCode << "imul " << regR[instr.src] << std::endl;
				asmCode << "mov " << regR[instr.dst] << ", rdx" << std::endl;
				break;
			case RandomX::SuperscalarInstructionType::IMUL_RCP:
				asmCode << "mov rax, " << (int64_t)reciprocal(instr.getImm32()) << std::endl;
				asmCode << "imul " << regR[instr.dst] << ", rax" << std::endl;
				break;
			default:
				UNREACHABLE;
			}
		}
	}

	void AssemblyGeneratorX86::generateC(SuperscalarProgram& prog) {
		asmCode.str(std::string()); //clear
		asmCode << "#include <stdint.h>" << std::endl;
		asmCode << "#if defined(__SIZEOF_INT128__)" << std::endl;
		asmCode << "	static inline uint64_t mulh(uint64_t a, uint64_t b) {" << std::endl;
		asmCode << "		return ((unsigned __int128)a * b) >> 64;" << std::endl;
		asmCode << "	}" << std::endl;
		asmCode << "	static inline int64_t smulh(int64_t a, int64_t b) {" << std::endl;
		asmCode << "		return ((__int128)a * b) >> 64;" << std::endl;
		asmCode << "	}" << std::endl;
		asmCode << "	#define HAVE_MULH" << std::endl;
		asmCode << "	#define HAVE_SMULH" << std::endl;
		asmCode << "#endif" << std::endl;
		asmCode << "#if defined(_MSC_VER)" << std::endl;
		asmCode << "	#define HAS_VALUE(X) X ## 0" << std::endl;
		asmCode << "	#define EVAL_DEFINE(X) HAS_VALUE(X)" << std::endl;
		asmCode << "	#include <intrin.h>" << std::endl;
		asmCode << "	#include <stdlib.h>" << std::endl;
		asmCode << "	static __inline uint64_t rotr(uint64_t x , int c) {" << std::endl;
		asmCode << "		return _rotr64(x, c);" << std::endl;
		asmCode << "	}" << std::endl;
		asmCode << "	#define HAVE_ROTR" << std::endl;
		asmCode << "	#if EVAL_DEFINE(__MACHINEARM64_X64(1))" << std::endl;
		asmCode << "		static __inline uint64_t mulh(uint64_t a, uint64_t b) {" << std::endl;
		asmCode << "			return __umulh(a, b);" << std::endl;
		asmCode << "		}" << std::endl;
		asmCode << "		#define HAVE_MULH" << std::endl;
		asmCode << "	#endif" << std::endl;
		asmCode << "	#if EVAL_DEFINE(__MACHINEX64(1))" << std::endl;
		asmCode << "		static __inline int64_t smulh(int64_t a, int64_t b) {" << std::endl;
		asmCode << "			int64_t hi;" << std::endl;
		asmCode << "			_mul128(a, b, &hi);" << std::endl;
		asmCode << "			return hi;" << std::endl;
		asmCode << "		}" << std::endl;
		asmCode << "		#define HAVE_SMULH" << std::endl;
		asmCode << "	#endif" << std::endl;
		asmCode << "#endif" << std::endl;
		asmCode << "#ifndef HAVE_ROTR" << std::endl;
		asmCode << "	static inline uint64_t rotr(uint64_t a, int b) {" << std::endl;
		asmCode << "		return (a >> b) | (a << (64 - b));" << std::endl;
		asmCode << "	}" << std::endl;
		asmCode << "	#define HAVE_ROTR" << std::endl;
		asmCode << "#endif" << std::endl;
		asmCode << "#if !defined(HAVE_MULH) || !defined(HAVE_SMULH) || !defined(HAVE_ROTR)" << std::endl;
		asmCode << "	#error \"Required functions are not defined\"" << std::endl;
		asmCode << "#endif" << std::endl;
		asmCode << "void superScalar(uint64_t r[8]) {" << std::endl;
		asmCode << "uint64_t r8 = r[0], r9 = r[1], r10 = r[2], r11 = r[3], r12 = r[4], r13 = r[5], r14 = r[6], r15 = r[7];" << std::endl;
		for (unsigned i = 0; i < prog.getSize(); ++i) {
			Instruction& instr = prog(i);
			switch (instr.opcode)
			{
			case RandomX::SuperscalarInstructionType::ISUB_R:
				asmCode << regR[instr.dst] << " -= " << regR[instr.src] << ";" << std::endl;
				break;
			case RandomX::SuperscalarInstructionType::IXOR_R:
				asmCode << regR[instr.dst] << " ^= " << regR[instr.src] << ";" << std::endl;
				break;
			case RandomX::SuperscalarInstructionType::IADD_RS:
				asmCode << regR[instr.dst] << " += " << regR[instr.src] << "*" << (1 << (instr.getModShift2())) << ";" << std::endl;
				break;
			case RandomX::SuperscalarInstructionType::IMUL_R:
				asmCode << regR[instr.dst] << " *= " << regR[instr.src] << ";" << std::endl;
				break;
			case RandomX::SuperscalarInstructionType::IROR_C:
				asmCode << regR[instr.dst] << " = rotr(" << regR[instr.dst] << ", " << instr.getImm32() << ");" << std::endl;
				break;
			case RandomX::SuperscalarInstructionType::IADD_C7:
			case RandomX::SuperscalarInstructionType::IADD_C8:
			case RandomX::SuperscalarInstructionType::IADD_C9:
				asmCode << regR[instr.dst] << " += " << (int32_t)instr.getImm32() << ";" << std::endl;
				break;
			case RandomX::SuperscalarInstructionType::IXOR_C7:
			case RandomX::SuperscalarInstructionType::IXOR_C8:
			case RandomX::SuperscalarInstructionType::IXOR_C9:
				asmCode << regR[instr.dst] << " ^= " << (int32_t)instr.getImm32() << ";" << std::endl;
				break;
			case RandomX::SuperscalarInstructionType::IMULH_R:
				asmCode << regR[instr.dst] << " = mulh(" << regR[instr.dst] << ", " << regR[instr.src] << ");" << std::endl;
				break;
			case RandomX::SuperscalarInstructionType::ISMULH_R:
				asmCode << regR[instr.dst] << " = smulh(" << regR[instr.dst] << ", " << regR[instr.src] << ");" << std::endl;
				break;
			case RandomX::SuperscalarInstructionType::IMUL_RCP:
				asmCode << regR[instr.dst] << " *= " << (int64_t)reciprocal(instr.getImm32()) << ";" << std::endl;
				break;
			default:
				UNREACHABLE;
			}
		}
		asmCode << "r[0] = r8; r[1] = r9; r[2] = r10; r[3] = r11; r[4] = r12; r[5] = r13; r[6] = r14; r[7] = r15;" << std::endl;
		asmCode << "}" << std::endl;
	}

	int AssemblyGeneratorX86::getConditionRegister() {
		int min = INT_MAX;
		int minIndex;
		for (unsigned i = 0; i < 8; ++i) {
			if (registerUsage[i] < min) {
				min = registerUsage[i];
				minIndex = i;
			}
		}
		return minIndex;
	}

	void AssemblyGeneratorX86::traceint(Instruction& instr) {
		if (trace) {
			asmCode << "\tpush " << regR[instr.dst] << std::endl;
		}
	}

	void AssemblyGeneratorX86::traceflt(Instruction& instr) {
		if (trace) {
			asmCode << "\tpush 0" << std::endl;
		}
	}

	void AssemblyGeneratorX86::tracenop(Instruction& instr) {
		if (trace) {
			asmCode << "\tpush 0" << std::endl;
		}
	}

	void AssemblyGeneratorX86::generateCode(Instruction& instr, int i) {
		asmCode << "\t; " << instr;
		auto generator = engine[instr.opcode];
		(this->*generator)(instr, i);
	}

	void AssemblyGeneratorX86::genAddressReg(Instruction& instr, const char* reg = "eax") {
		asmCode << "\tlea " << reg << ", [" << regR32[instr.src] << std::showpos << (int32_t)instr.getImm32() << std::noshowpos << "]" << std::endl;
		asmCode << "\tand " << reg << ", " << ((instr.getModMem()) ? ScratchpadL1Mask : ScratchpadL2Mask) << std::endl;
	}

	void AssemblyGeneratorX86::genAddressRegDst(Instruction& instr, int maskAlign = 8) {
		asmCode << "\tlea eax, [" << regR32[instr.dst] << std::showpos << (int32_t)instr.getImm32() << std::noshowpos << "]" << std::endl;
		int mask;
		if (instr.getModCond()) {
			mask = instr.getModMem() ? ScratchpadL1Mask : ScratchpadL2Mask;
		}
		else {
			mask = ScratchpadL3Mask;
		}
		asmCode << "\tand eax" << ", " << (mask & (-maskAlign)) << std::endl;
	}

	int32_t AssemblyGeneratorX86::genAddressImm(Instruction& instr) {
		return (int32_t)instr.getImm32() & ScratchpadL3Mask;
	}

	//1 uOP
	void AssemblyGeneratorX86::h_IADD_RS(Instruction& instr, int i) {
		registerUsage[instr.dst] = i;
		if(instr.dst == RegisterNeedsDisplacement)
			asmCode << "\tlea " << regR[instr.dst] << ", [" << regR[instr.dst] << "+" << regR[instr.src] << "*" << (1 << (instr.getModShift2())) << std::showpos << (int32_t)instr.getImm32() << std::noshowpos << "]" << std::endl;
		else
			asmCode << "\tlea " << regR[instr.dst] << ", [" << regR[instr.dst] << "+" << regR[instr.src] << "*" << (1 << (instr.getModShift2())) << "]" << std::endl;
		traceint(instr);
	}

	//2.75 uOP
	void AssemblyGeneratorX86::h_IADD_M(Instruction& instr, int i) {
		registerUsage[instr.dst] = i;
		if (instr.src != instr.dst) {
			genAddressReg(instr);
			asmCode << "\tadd " << regR[instr.dst] << ", qword ptr [rsi+rax]" << std::endl;
		}
		else {
			asmCode << "\tadd " << regR[instr.dst] << ", qword ptr [rsi+" << genAddressImm(instr) << "]" << std::endl;
		}
		traceint(instr);
	}

	//1 uOP
	void AssemblyGeneratorX86::h_IADD_RC(Instruction& instr, int i) {
		registerUsage[instr.dst] = i;
		asmCode << "\tlea " << regR[instr.dst] << ", [" << regR[instr.dst] << "+" << regR[instr.src] << std::showpos << (int32_t)instr.getImm32() << std::noshowpos << "]" << std::endl;
		traceint(instr);
	}

	//1 uOP
	void AssemblyGeneratorX86::h_ISUB_R(Instruction& instr, int i) {
		registerUsage[instr.dst] = i;
		if (instr.src != instr.dst) {
			asmCode << "\tsub " << regR[instr.dst] << ", " << regR[instr.src] << std::endl;
		}
		else {
			asmCode << "\tsub " << regR[instr.dst] << ", " << (int32_t)instr.getImm32() << std::endl;
		}
		traceint(instr);
	}

	//2.75 uOP
	void AssemblyGeneratorX86::h_ISUB_M(Instruction& instr, int i) {
		registerUsage[instr.dst] = i;
		if (instr.src != instr.dst) {
			genAddressReg(instr);
			asmCode << "\tsub " << regR[instr.dst] << ", qword ptr [rsi+rax]" << std::endl;
		}
		else {
			asmCode << "\tsub " << regR[instr.dst] << ", qword ptr [rsi+" << genAddressImm(instr) << "]" << std::endl;
		}
		traceint(instr);
	}

	//1 uOP
	void AssemblyGeneratorX86::h_IMUL_9C(Instruction& instr, int i) {
		registerUsage[instr.dst] = i;
		asmCode << "\tlea " << regR[instr.dst] << ", [" << regR[instr.dst] << "+" << regR[instr.dst] << "*8" << std::showpos << (int32_t)instr.getImm32() << std::noshowpos << "]" << std::endl;
		traceint(instr);
	}

	//1 uOP
	void AssemblyGeneratorX86::h_IMUL_R(Instruction& instr, int i) {
		registerUsage[instr.dst] = i;
		if (instr.src != instr.dst) {
			asmCode << "\timul " << regR[instr.dst] << ", " << regR[instr.src] << std::endl;
		}
		else {
			asmCode << "\timul " << regR[instr.dst] << ", " << (int32_t)instr.getImm32() << std::endl;
		}
		traceint(instr);
	}

	//2.75 uOP
	void AssemblyGeneratorX86::h_IMUL_M(Instruction& instr, int i) {
		registerUsage[instr.dst] = i;
		if (instr.src != instr.dst) {
			genAddressReg(instr);
			asmCode << "\timul " << regR[instr.dst] << ", qword ptr [rsi+rax]" << std::endl;
		}
		else {
			asmCode << "\timul " << regR[instr.dst] << ", qword ptr [rsi+" << genAddressImm(instr) << "]" << std::endl;
		}
		traceint(instr);
	}

	//4 uOPs
	void AssemblyGeneratorX86::h_IMULH_R(Instruction& instr, int i) {
		registerUsage[instr.dst] = i;
		asmCode << "\tmov rax, " << regR[instr.dst] << std::endl;
		asmCode << "\tmul " << regR[instr.src] << std::endl;
		asmCode << "\tmov " << regR[instr.dst] << ", rdx" << std::endl;
		traceint(instr);
	}

	//5.75 uOPs
	void AssemblyGeneratorX86::h_IMULH_M(Instruction& instr, int i) {
		registerUsage[instr.dst] = i;
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
		traceint(instr);
	}

	//4 uOPs
	void AssemblyGeneratorX86::h_ISMULH_R(Instruction& instr, int i) {
		registerUsage[instr.dst] = i;
		asmCode << "\tmov rax, " << regR[instr.dst] << std::endl;
		asmCode << "\timul " << regR[instr.src] << std::endl;
		asmCode << "\tmov " << regR[instr.dst] << ", rdx" << std::endl;
		traceint(instr);
	}

	//5.75 uOPs
	void AssemblyGeneratorX86::h_ISMULH_M(Instruction& instr, int i) {
		registerUsage[instr.dst] = i;
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
		traceint(instr);
	}

	//1 uOP
	void AssemblyGeneratorX86::h_INEG_R(Instruction& instr, int i) {
		registerUsage[instr.dst] = i;
		asmCode << "\tneg " << regR[instr.dst] << std::endl;
		traceint(instr);
	}

	//1 uOP
	void AssemblyGeneratorX86::h_IXOR_R(Instruction& instr, int i) {
		registerUsage[instr.dst] = i;
		if (instr.src != instr.dst) {
			asmCode << "\txor " << regR[instr.dst] << ", " << regR[instr.src] << std::endl;
		}
		else {
			asmCode << "\txor " << regR[instr.dst] << ", " << (int32_t)instr.getImm32() << std::endl;
		}
		traceint(instr);
	}

	//2.75 uOP
	void AssemblyGeneratorX86::h_IXOR_M(Instruction& instr, int i) {
		registerUsage[instr.dst] = i;
		if (instr.src != instr.dst) {
			genAddressReg(instr);
			asmCode << "\txor " << regR[instr.dst] << ", qword ptr [rsi+rax]" << std::endl;
		}
		else {
			asmCode << "\txor " << regR[instr.dst] << ", qword ptr [rsi+" << genAddressImm(instr) << "]" << std::endl;
		}
		traceint(instr);
	}

	//1.75 uOPs
	void AssemblyGeneratorX86::h_IROR_R(Instruction& instr, int i) {
		registerUsage[instr.dst] = i;
		if (instr.src != instr.dst) {
			asmCode << "\tmov ecx, " << regR32[instr.src] << std::endl;
			asmCode << "\tror " << regR[instr.dst] << ", cl" << std::endl;
		}
		else {
			asmCode << "\tror " << regR[instr.dst] << ", " << (instr.getImm32() & 63) << std::endl;
		}
		traceint(instr);
	}

	//1.75 uOPs
	void AssemblyGeneratorX86::h_IROL_R(Instruction& instr, int i) {
		registerUsage[instr.dst] = i;
		if (instr.src != instr.dst) {
			asmCode << "\tmov ecx, " << regR32[instr.src] << std::endl;
			asmCode << "\trol " << regR[instr.dst] << ", cl" << std::endl;
		}
		else {
			asmCode << "\trol " << regR[instr.dst] << ", " << (instr.getImm32() & 63) << std::endl;
		}
		traceint(instr);
	}

	//2 uOPs
	void AssemblyGeneratorX86::h_IMUL_RCP(Instruction& instr, int i) {
		if (instr.getImm32() != 0) {
			registerUsage[instr.dst] = i;
			uint32_t divisor = instr.getImm32();
			asmCode << "\tmov rax, " << reciprocal(instr.getImm32()) << std::endl;
			asmCode << "\timul " << regR[instr.dst] << ", rax" << std::endl;
			traceint(instr);
		}
		else {
			tracenop(instr);
		}
	}

	//~8.5 uOPs
	void AssemblyGeneratorX86::h_ISDIV_C(Instruction& instr, int i) {
		tracenop(instr);
	}

	//2 uOPs
	void AssemblyGeneratorX86::h_ISWAP_R(Instruction& instr, int i) {
		if (instr.src != instr.dst) {
			//std::swap(registerUsage[instr.dst], registerUsage[instr.src]);
			registerUsage[instr.dst] = i;
			registerUsage[instr.src] = i;
			asmCode << "\txchg " << regR[instr.dst] << ", " << regR[instr.src] << std::endl;
			traceint(instr);
		}
		else {
			tracenop(instr);
		}
	}

	//1 uOPs
	void AssemblyGeneratorX86::h_FSWAP_R(Instruction& instr, int i) {
		asmCode << "\tshufpd " << regFE[instr.dst] << ", " << regFE[instr.dst] << ", 1" << std::endl;
		traceflt(instr);
	}

	//1 uOP
	void AssemblyGeneratorX86::h_FADD_R(Instruction& instr, int i) {
		instr.dst %= 4;
		instr.src %= 4;
		asmCode << "\taddpd " << regF[instr.dst] << ", " << regA[instr.src] << std::endl;
		//asmCode << "\t" << fsumInstr[instr.mod % 4] << " " << signMask << ", " << regF[instr.dst] << std::endl;
		traceflt(instr);
	}

	//5 uOPs
	void AssemblyGeneratorX86::h_FADD_M(Instruction& instr, int i) {
		instr.dst %= 4;
		genAddressReg(instr);
		asmCode << "\tcvtdq2pd xmm12, qword ptr [rsi+rax]" << std::endl;
		asmCode << "\taddpd " << regF[instr.dst] << ", xmm12" << std::endl;
		traceflt(instr);
	}

	//1 uOP
	void AssemblyGeneratorX86::h_FSUB_R(Instruction& instr, int i) {
		instr.dst %= 4;
		instr.src %= 4;
		asmCode << "\tsubpd " << regF[instr.dst] << ", " << regA[instr.src] << std::endl;
		//asmCode << "\t" << fsumInstr[instr.mod % 4] << " " << signMask << ", " << regF[instr.dst] << std::endl;
		traceflt(instr);
	}

	//5 uOPs
	void AssemblyGeneratorX86::h_FSUB_M(Instruction& instr, int i) {
		instr.dst %= 4;
		genAddressReg(instr);
		asmCode << "\tcvtdq2pd xmm12, qword ptr [rsi+rax]" << std::endl;
		asmCode << "\tsubpd " << regF[instr.dst] << ", xmm12" << std::endl;
		traceflt(instr);
	}

	//1 uOP
	void AssemblyGeneratorX86::h_FSCAL_R(Instruction& instr, int i) {
		instr.dst %= 4;
		asmCode << "\txorps " << regF[instr.dst] << ", " << signMask << std::endl;
		traceflt(instr);
	}

	//1 uOPs
	void AssemblyGeneratorX86::h_FMUL_R(Instruction& instr, int i) {
		instr.dst %= 4;
		instr.src %= 4;
		asmCode << "\tmulpd " << regE[instr.dst] << ", " << regA[instr.src] << std::endl;
		traceflt(instr);
	}

	//7 uOPs
	void AssemblyGeneratorX86::h_FMUL_M(Instruction& instr, int i) {
		instr.dst %= 4;
		genAddressReg(instr);
		asmCode << "\tcvtdq2pd xmm12, qword ptr [rsi+rax]" << std::endl;
		asmCode << "\tandps xmm12, xmm14" << std::endl;
		asmCode << "\tmulpd " << regE[instr.dst] << ", xmm12" << std::endl;
		asmCode << "\tmaxpd " << regE[instr.dst] << ", " << dblMin << std::endl;
		traceflt(instr);
	}

	//2 uOPs
	void AssemblyGeneratorX86::h_FDIV_R(Instruction& instr, int i) {
		instr.dst %= 4;
		instr.src %= 4;
		asmCode << "\tdivpd " << regE[instr.dst] << ", " << regA[instr.src] << std::endl;
		asmCode << "\tmaxpd " << regE[instr.dst] << ", " << dblMin << std::endl;
		traceflt(instr);
	}

	//7 uOPs
	void AssemblyGeneratorX86::h_FDIV_M(Instruction& instr, int i) {
		instr.dst %= 4;
		genAddressReg(instr);
		asmCode << "\tcvtdq2pd xmm12, qword ptr [rsi+rax]" << std::endl;
		asmCode << "\tandps xmm12, xmm13" << std::endl;
		asmCode << "\torps xmm12, xmm14" << std::endl;
		asmCode << "\tdivpd " << regE[instr.dst] << ", xmm12" << std::endl;
		traceflt(instr);
	}

	//1 uOP
	void AssemblyGeneratorX86::h_FSQRT_R(Instruction& instr, int i) {
		instr.dst %= 4;
		asmCode << "\tsqrtpd " << regE[instr.dst] << ", " << regE[instr.dst] << std::endl;
		traceflt(instr);
	}	

	//6 uOPs
	void AssemblyGeneratorX86::h_CFROUND(Instruction& instr, int i) {
		asmCode << "\tmov rax, " << regR[instr.src] << std::endl;
		int rotate = (13 - (instr.getImm32() & 63)) & 63;
		if (rotate != 0)
			asmCode << "\trol rax, " << rotate << std::endl;
		asmCode << "\tand eax, 24576" << std::endl;
		asmCode << "\tor eax, 40896" << std::endl;
		asmCode << "\tmov dword ptr [rsp-8], eax" << std::endl;
		asmCode << "\tldmxcsr dword ptr [rsp-8]" << std::endl;
		tracenop(instr);
	}

	static inline const char* condition(Instruction& instr) {
		switch (instr.getModCond())
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
			default:
				UNREACHABLE;
		}
	}

	void AssemblyGeneratorX86::handleCondition(Instruction& instr, int i) {
		const int shift = instr.getModShift3();
		const int conditionMask = ((1 << RANDOMX_CONDITION_BITS) - 1) << shift;
		int reg = getConditionRegister();
		int target = registerUsage[reg] + 1;
		registerUsage[reg] = i;
		asmCode << "\tadd " << regR[reg] << ", " << (1 << shift) << std::endl;
		asmCode << "\ttest " << regR[reg] << ", " << conditionMask << std::endl;
		asmCode << "\tjz randomx_isn_" << target << std::endl;
		for (unsigned j = 0; j < 8; ++j) { //mark all registers as used
			registerUsage[j] = i;
		}
	}

	//4 uOPs
	void AssemblyGeneratorX86::h_COND_R(Instruction& instr, int i) {
		handleCondition(instr, i);
		asmCode << "\txor ecx, ecx" << std::endl;
		asmCode << "\tcmp " << regR32[instr.src] << ", " << (int32_t)instr.getImm32() << std::endl;
		asmCode << "\tset" << condition(instr) << " cl" << std::endl;
		asmCode << "\tadd " << regR[instr.dst] << ", rcx" << std::endl;
		traceint(instr);
	}

	//6 uOPs
	void AssemblyGeneratorX86::h_COND_M(Instruction& instr, int i) {
		handleCondition(instr, i);
		asmCode << "\txor ecx, ecx" << std::endl;
		genAddressReg(instr);
		asmCode << "\tcmp dword ptr [rsi+rax], " << (int32_t)instr.getImm32() << std::endl;
		asmCode << "\tset" << condition(instr) << " cl" << std::endl;
		asmCode << "\tadd " << regR[instr.dst] << ", rcx" << std::endl;
		traceint(instr);
	}

	//3 uOPs
	void AssemblyGeneratorX86::h_ISTORE(Instruction& instr, int i) {
		genAddressRegDst(instr);
		asmCode << "\tmov qword ptr [rsi+rax], " << regR[instr.src] << std::endl;
		tracenop(instr);
	}

	//3 uOPs
	void AssemblyGeneratorX86::h_FSTORE(Instruction& instr, int i) {
		genAddressRegDst(instr, 16);
		asmCode << "\tmovapd xmmword ptr [rsi+rax], " << regFE[instr.src] << std::endl;
		tracenop(instr);
	}

	void AssemblyGeneratorX86::h_NOP(Instruction& instr, int i) {
		asmCode << "\tnop" << std::endl;
		tracenop(instr);
	}

#include "instructionWeights.hpp"
#define INST_HANDLE(x) REPN(&AssemblyGeneratorX86::h_##x, WT(x))

	InstructionGenerator AssemblyGeneratorX86::engine[256] = {
		//Integer
		INST_HANDLE(IADD_RS)
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
		INST_HANDLE(FSCAL_R)

		//Floating point group E
		INST_HANDLE(FMUL_R)
		INST_HANDLE(FDIV_M)
		INST_HANDLE(FSQRT_R)

		//Control
		INST_HANDLE(COND_R)
		INST_HANDLE(COND_M)
		INST_HANDLE(CFROUND)
		INST_HANDLE(ISTORE)

		INST_HANDLE(NOP)
	};
}