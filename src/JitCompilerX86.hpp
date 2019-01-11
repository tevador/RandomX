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

	typedef void(JitCompilerX86::*InstructionGeneratorX86)(Instruction&, int);

	constexpr uint32_t CodeSize = 64 * 1024;
	constexpr uint32_t CacheLineSize = 64;

	struct CallOffset {
		CallOffset(int32_t p, int32_t i) : pos(p), index(i) {}
		int32_t pos;
		int32_t index;
	};

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
	private:
		static InstructionGeneratorX86 engine[256];
		uint8_t* code;
		int32_t codePos;
		std::vector<int32_t> instructionOffsets;
		std::vector<CallOffset> callOffsets;

		void genar(Instruction&);
		void genaf(Instruction&);
		void genbr0(Instruction&, uint16_t, uint16_t);
		void genbr1(Instruction&, uint16_t, uint16_t);
		void genbr132(Instruction&, uint16_t, uint8_t);
		void genbf(Instruction&, uint8_t);
		void scratchpadStoreR(Instruction&, uint32_t, bool);
		void scratchpadStoreF(Instruction&, int, uint32_t, bool);
		void gencr(Instruction&, bool);
		void gencf(Instruction&);
		void generateCode(Instruction&, int);
		void fixCallOffsets();

		void emitByte(uint8_t val) {
			code[codePos] = val;
			codePos++;
		}

		template<typename T>
		void emit(T val) {
			*reinterpret_cast<T*>(code + codePos) = val;
			codePos += sizeof(T);
		}

		void h_ADD_64(Instruction&, int);
		void h_ADD_32(Instruction&, int);
		void h_SUB_64(Instruction&, int);
		void h_SUB_32(Instruction&, int);
		void h_MUL_64(Instruction&, int);
		void h_MULH_64(Instruction&, int);
		void h_MUL_32(Instruction&, int);
		void h_IMUL_32(Instruction&, int);
		void h_IMULH_64(Instruction&, int);
		void h_DIV_64(Instruction&, int);
		void h_IDIV_64(Instruction&, int);
		void h_AND_64(Instruction&, int);
		void h_AND_32(Instruction&, int);
		void h_OR_64(Instruction&, int);
		void h_OR_32(Instruction&, int);
		void h_XOR_64(Instruction&, int);
		void h_XOR_32(Instruction&, int);
		void h_SHL_64(Instruction&, int);
		void h_SHR_64(Instruction&, int);
		void h_SAR_64(Instruction&, int);
		void h_ROL_64(Instruction&, int);
		void h_ROR_64(Instruction&, int);
		void h_FPADD(Instruction&, int);
		void h_FPSUB(Instruction&, int);
		void h_FPMUL(Instruction&, int);
		void h_FPDIV(Instruction&, int);
		void h_FPSQRT(Instruction&, int);
		void h_FPROUND(Instruction&, int);
		void h_JUMP(Instruction&, int);
		void h_CALL(Instruction&, int);
		void h_RET(Instruction&, int);
	};

}