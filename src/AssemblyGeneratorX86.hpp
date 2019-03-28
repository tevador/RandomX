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
#include "configuration.h"
#include <sstream>

namespace RandomX {

	class Program;
	class AssemblyGeneratorX86;

	typedef void(AssemblyGeneratorX86::*InstructionGenerator)(Instruction&, int);

	class AssemblyGeneratorX86 {
	public:
		void generateProgram(Program&);
		void printCode(std::ostream& os) {
			os << asmCode.rdbuf();
		}
	private:
		static InstructionGenerator engine[256];
		std::stringstream asmCode;
		int registerUsage[8];

		void genAddressReg(Instruction&, const char*);
		void genAddressRegDst(Instruction&, int);
		int32_t genAddressImm(Instruction&);
		int getConditionRegister();
		void handleCondition(Instruction&, int);

		void generateCode(Instruction&, int);

		void traceint(Instruction&);
		void traceflt(Instruction&);
		void tracenop(Instruction&);

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