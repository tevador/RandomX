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

#include "Instruction.hpp"

namespace RandomX {

	void Instruction::print(std::ostream& os) const {
			os << "  A: loc = " << std::dec << (loca & 7) << ", reg: " << (rega & 7) << std::endl;
			os << "  B: loc = " << (locb & 7) << ", reg: " << (regb & 7) << std::endl;
			os << "  C: loc = " << (locc & 7) << ", reg: " << (regc & 7) << std::endl;
			os << "  addra = " << std::hex << addra << std::endl;
			os << "  addrc = " << addrc << std::endl;
			os << "  imm8 = " << std::dec << (int)imm8 << std::endl;
			os << "  imm32 = " << imm32 << std::endl;
		}

#include "instructionWeights.hpp"
#define INST_NAME(x) REPN(#x, WT(x))

	const char* Instruction::names[256] = {
		INST_NAME(ADD_64)
		INST_NAME(ADD_32)
		INST_NAME(SUB_64)
		INST_NAME(SUB_32)
		INST_NAME(MUL_64)
		INST_NAME(MULH_64)
		INST_NAME(MUL_32)
		INST_NAME(IMUL_32)
		INST_NAME(IMULH_64)
		INST_NAME(DIV_64)
		INST_NAME(IDIV_64)
		INST_NAME(AND_64)
		INST_NAME(AND_32)
		INST_NAME(OR_64)
		INST_NAME(OR_32)
		INST_NAME(XOR_64)
		INST_NAME(XOR_32)
		INST_NAME(SHL_64)
		INST_NAME(SHR_64)
		INST_NAME(SAR_64)
		INST_NAME(ROL_64)
		INST_NAME(ROR_64)
		INST_NAME(FPADD)
		INST_NAME(FPSUB)
		INST_NAME(FPMUL)
		INST_NAME(FPDIV)
		INST_NAME(FPSQRT)
		INST_NAME(FPROUND)
		INST_NAME(JUMP)
		INST_NAME(CALL)
		INST_NAME(RET)
	};

}