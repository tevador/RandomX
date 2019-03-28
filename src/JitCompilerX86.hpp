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

#pragma once

#include "common.hpp"
#include "Instruction.hpp"
#include <cstring>
#include <vector>

namespace RandomX {

	class Program;
	class JitCompilerX86;

	typedef void(JitCompilerX86::*InstructionGeneratorX86)(Instruction&, int);

	constexpr uint32_t CodeSize = 64 * 1024;

	class JitCompilerX86 {
	public:
		JitCompilerX86();
		void generateProgram(Program&);
		void generateProgramLight(Program&);
		ProgramFunc getProgramFunc() {
			return (ProgramFunc)code;
		}
		uint8_t* getCode() {
			return code;
		}
		size_t getCodeSize();
	private:
		static InstructionGeneratorX86 engine[256];
		std::vector<int32_t> instructionOffsets;
		int registerUsage[8];
		uint8_t* code;
		int32_t codePos;

		template<class P>
		void generateCode(P& prog) {
			for (unsigned i = 0; i < prog.getSize(); ++i) {
				Instruction& instr = prog(i);
				instr.src %= RegistersCount;
				instr.dst %= RegistersCount;
				generateCode(instr, i);
			}
		}

		void generateProgramPrologue(Program&);
		void generateProgramEpilogue(Program&);
		int getConditionRegister();
		void genAddressReg(Instruction&, bool);
		void genAddressRegDst(Instruction&, bool);
		void genAddressImm(Instruction&);
		void genSIB(int scale, int index, int base);

		void handleCondition(Instruction&, int);
		void generateCode(Instruction&, int);

		void emitByte(uint8_t val) {
			code[codePos] = val;
			codePos++;
		}

		void emit32(uint32_t val) {
			code[codePos + 0] = val;
			code[codePos + 1] = val >> 8;
			code[codePos + 2] = val >> 16;
			code[codePos + 3] = val >> 24;
			codePos += 4;
		}

		void emit64(uint64_t val) {
			code[codePos + 0] = val;
			code[codePos + 1] = val >> 8;
			code[codePos + 2] = val >> 16;
			code[codePos + 3] = val >> 24;
			code[codePos + 4] = val >> 32;
			code[codePos + 5] = val >> 40;
			code[codePos + 6] = val >> 48;
			code[codePos + 7] = val >> 56;
			codePos += 8;
		}

		template<size_t N>
		void emit(const uint8_t (&src)[N]) {
			for (unsigned i = 0; i < N; ++i) {
				code[codePos + i] = src[i];
			}
			codePos += N;
		}

		void  h_IADD_R(Instruction&, int);
		void  h_IADD_M(Instruction&, int);
		void  h_IADD_RC(Instruction&, int);
		void  h_ISUB_R(Instruction&, int);
		void  h_ISUB_M(Instruction&, int);
		void  h_IMUL_9C(Instruction&, int);
		void  h_IMUL_R(Instruction&, int);
		void  h_IMUL_M(Instruction&, int);
		void  h_IMULH_R(Instruction&, int);
		void  h_IMULH_M(Instruction&, int);
		void  h_ISMULH_R(Instruction&, int);
		void  h_ISMULH_M(Instruction&, int);
		void  h_IMUL_RCP(Instruction&, int);
		void  h_ISDIV_C(Instruction&, int);
		void  h_INEG_R(Instruction&, int);
		void  h_IXOR_R(Instruction&, int);
		void  h_IXOR_M(Instruction&, int);
		void  h_IROR_R(Instruction&, int);
		void  h_IROL_R(Instruction&, int);
		void  h_ISWAP_R(Instruction&, int);
		void  h_FSWAP_R(Instruction&, int);
		void  h_FADD_R(Instruction&, int);
		void  h_FADD_M(Instruction&, int);
		void  h_FSUB_R(Instruction&, int);
		void  h_FSUB_M(Instruction&, int);
		void  h_FSCAL_R(Instruction&, int);
		void  h_FMUL_R(Instruction&, int);
		void  h_FMUL_M(Instruction&, int);
		void  h_FDIV_R(Instruction&, int);
		void  h_FDIV_M(Instruction&, int);
		void  h_FSQRT_R(Instruction&, int);
		void  h_COND_R(Instruction&, int);
		void  h_COND_M(Instruction&, int);
		void  h_CFROUND(Instruction&, int);
		void  h_ISTORE(Instruction&, int);
		void  h_FSTORE(Instruction&, int);
		void  h_NOP(Instruction&, int);
	};

}