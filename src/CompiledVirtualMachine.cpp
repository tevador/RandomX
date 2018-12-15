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

#include "CompiledVirtualMachine.hpp"
#include "Pcg32.hpp"
#include "common.hpp"
#include "instructions.hpp"

namespace RandomX {

	void CompiledVirtualMachine::initializeProgram(const void* seed) {
		Pcg32 gen(seed);
		for (unsigned i = 0; i < sizeof(reg) / sizeof(Pcg32::result_type); ++i) {
			*(((uint32_t*)&reg) + i) = gen();
		}
		FPINIT();
		for (int i = 0; i < 8; ++i) {
			reg.f[i].f64 = (double)reg.f[i].i64;
		}
		for (unsigned i = 0; i < ProgramLength; ++i) {
			gen(); gen(); gen(); gen();
		}
		mem.ma = (gen() ^ *(((uint32_t*)seed) + 4)) & ~7;
		mem.mx = *(((uint32_t*)seed) + 5);
	}

	void CompiledVirtualMachine::execute() {
		FPINIT();
		executeProgram(reg, mem, readDataset, scratchpad);
#ifdef TRACE
		for (int32_t i = InstructionCount - 1; i >= 0; --i) {
			std::cout << std::hex << tracepad[i].u64 << std::endl;
		}
#endif

	}
}