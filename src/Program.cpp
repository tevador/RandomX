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

#include "Program.hpp"
#include "Pcg32.hpp"

namespace RandomX {
	void Program::initialize(Pcg32& gen) {
		for (unsigned i = 0; i < sizeof(programBuffer) / sizeof(Pcg32::result_type); ++i) {
			*(((uint32_t*)&programBuffer) + i) = gen();
		}
	}

	void Program::print(std::ostream& os) const {
		for (int i = 0; i < RandomX::ProgramLength; ++i) {
			auto instr = programBuffer[i];
			os << std::dec << instrNames[instr.opcode] << " (" << i << "):" << std::endl;
			os << "  A: loc = " << (instr.loca & 7) << ", reg: " << (instr.rega & 7) << std::endl;
			os << "  B: loc = " << (instr.locb & 7) << ", reg: " << (instr.regb & 7) << std::endl;
			os << "  C: loc = " << (instr.locc & 7) << ", reg: " << (instr.regc & 7) << std::endl;
			os << "  imm0 = " << (int)instr.imm0 << std::endl;
			os << "  imm1 = " << std::hex << instr.imm1 << std::endl;
		}
	}

#include "instructionWeights.hpp"
#define INST_NAME(x) REPN(#x, WT(x))

	const char* Program::instrNames[256] = {
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
		INST_NAME(CALL)
		INST_NAME(RET)
	};
}
