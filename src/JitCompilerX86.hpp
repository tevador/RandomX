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

class Pcg32;

namespace RandomX {

	class JitCompilerX86;

	typedef void(JitCompilerX86::*InstructionGeneratorX86)(Instruction&);

	constexpr uint32_t CodeSize = 64 * 1024;

	class JitCompilerX86 {
	public:
		JitCompilerX86();
		void generateProgram(Pcg32&);
		ProgramFunc getProgramFunc() {
			return (ProgramFunc)code;
		}
		uint8_t* getCode() {
			return code;
		}
		size_t getCodeSize();
	private:
		static InstructionGeneratorX86 engine[256];
		uint8_t* code;
		int32_t codePos;

		void genAddressReg(Instruction&, bool);
		void genAddressRegDst(Instruction&, bool);
		void genAddressImm(Instruction&);
		void genSIB(int scale, int index, int base);

		void generateCode(Instruction&);

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
			for (int i = 0; i < N; ++i) {
				code[codePos + i] = src[i];
			}
			codePos += N;
		}

		void  h_IADD_R(Instruction&);
		void  h_IADD_M(Instruction&);
		void  h_IADD_RC(Instruction&);
		void  h_ISUB_R(Instruction&);
		void  h_ISUB_M(Instruction&);
		void  h_IMUL_9C(Instruction&);
		void  h_IMUL_R(Instruction&);
		void  h_IMUL_M(Instruction&);
		void  h_IMULH_R(Instruction&);
		void  h_IMULH_M(Instruction&);
		void  h_ISMULH_R(Instruction&);
		void  h_ISMULH_M(Instruction&);
		void  h_IDIV_C(Instruction&);
		void  h_ISDIV_C(Instruction&);
		void  h_INEG_R(Instruction&);
		void  h_IXOR_R(Instruction&);
		void  h_IXOR_M(Instruction&);
		void  h_IROR_R(Instruction&);
		void  h_IROL_R(Instruction&);
		void  h_FPSWAP_R(Instruction&);
		void  h_FPADD_R(Instruction&);
		void  h_FPADD_M(Instruction&);
		void  h_FPSUB_R(Instruction&);
		void  h_FPSUB_M(Instruction&);
		void  h_FPNEG_R(Instruction&);
		void  h_FPMUL_R(Instruction&);
		void  h_FPMUL_M(Instruction&);
		void  h_FPDIV_R(Instruction&);
		void  h_FPDIV_M(Instruction&);
		void  h_FPSQRT_R(Instruction&);
		void  h_COND_R(Instruction&);
		void  h_COND_M(Instruction&);
		void  h_CFROUND(Instruction&);
		void  h_ISTORE(Instruction&);
		void  h_FSTORE(Instruction&);
		void  h_NOP(Instruction&);
	};

}