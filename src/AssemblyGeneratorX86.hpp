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

#include "Instruction.hpp"
#include <sstream>

namespace RandomX {

	class AssemblyGeneratorX86;

	typedef void(AssemblyGeneratorX86::*InstructionGenerator)(Instruction&, int);

	class AssemblyGeneratorX86 {
	public:
		void generateProgram(const void* seed);
		void printCode(std::ostream& os) {
			os << asmCode.rdbuf();
		}
	private:
		static InstructionGenerator engine[256];
		std::stringstream asmCode;

		void gena(Instruction&, int);
		void genar(Instruction&, int);
		void genaf(Instruction&, int);
		void genbiashift(Instruction&, const char*);
		void genbia(Instruction&);
		void genbia32(Instruction&);
		void genbf(Instruction&, const char*);
		void gencr(Instruction&, bool);
		void gencf(Instruction&, bool);

		void generateCode(Instruction&, int);

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