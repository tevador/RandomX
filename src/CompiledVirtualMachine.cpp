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
#include "common.hpp"
#include <stdexcept>

namespace RandomX {

	CompiledVirtualMachine::CompiledVirtualMachine() {
		totalSize = 0;
	}

	void CompiledVirtualMachine::setDataset(dataset_t ds) {
		mem.ds = ds;
	}

	void CompiledVirtualMachine::initialize() {
		VirtualMachine::initialize();
		compiler.generateProgram(program);
	}

	void CompiledVirtualMachine::execute() {
		//executeProgram(reg, mem, scratchpad, InstructionCount);
		totalSize += compiler.getCodeSize();
		compiler.getProgramFunc()(reg, mem, scratchpad, InstructionCount);
#ifdef TRACEVM
		for (int32_t i = InstructionCount - 1; i >= 0; --i) {
			std::cout << std::hex << tracepad[i].u64 << std::endl;
		}
#endif

	}
}