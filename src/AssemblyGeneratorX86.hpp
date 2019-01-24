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
		void genAddressReg(Instruction&, const char*);
		int32_t genAddressImm(Instruction&);

		void generateCode(Instruction&, int);

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
		void  h_IDIV_C(Instruction&, int);
		void  h_ISDIV_C(Instruction&, int);
		void  h_INEG_R(Instruction&, int);
		void  h_IXOR_R(Instruction&, int);
		void  h_IXOR_M(Instruction&, int);
		void  h_IROR_R(Instruction&, int);
		void  h_IROL_R(Instruction&, int);
		void  h_FPSWAP_R(Instruction&, int);
		void  h_FPADD_R(Instruction&, int);
		void  h_FPADD_M(Instruction&, int);
		void  h_FPSUB_R(Instruction&, int);
		void  h_FPSUB_M(Instruction&, int);
		void  h_FPNEG_R(Instruction&, int);
		void  h_FPMUL_R(Instruction&, int);
		void  h_FPMUL_M(Instruction&, int);
		void  h_FPDIV_R(Instruction&, int);
		void  h_FPDIV_M(Instruction&, int);
		void  h_FPSQRT_R(Instruction&, int);
		void  h_COND_R(Instruction&, int);
		void  h_COND_M(Instruction&, int);
		void  h_CFROUND(Instruction&, int);
	};
}