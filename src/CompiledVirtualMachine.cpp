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

	//static_assert(sizeof(MemoryRegisters) == 2 * sizeof(addr_t) + sizeof(uintptr_t), "Invalid alignment of struct RandomX::MemoryRegisters");
	static_assert(sizeof(RegisterFile) == 256, "Invalid alignment of struct RandomX::RegisterFile");

	CompiledVirtualMachine::CompiledVirtualMachine() {
	}

	void CompiledVirtualMachine::setDataset(dataset_t ds, uint64_t size, SuperscalarProgram(&programs)[RANDOMX_CACHE_ACCESSES]) {
		mem.ds = ds;
		datasetRange = (size - RANDOMX_DATASET_SIZE + CacheLineSize) / CacheLineSize;
		datasetBasePtr = ds.dataset.memory;
	}

	void CompiledVirtualMachine::initialize() {
		VirtualMachine::initialize();
		compiler.generateProgram(program);
		mem.ds.dataset.memory = datasetBasePtr + (datasetBase * CacheLineSize);
	}

	void CompiledVirtualMachine::execute() {
		//executeProgram(reg, mem, scratchpad, InstructionCount);
		compiler.getProgramFunc()(reg, mem, scratchpad, RANDOMX_PROGRAM_ITERATIONS);
#ifdef TRACEVM
		for (int32_t i = InstructionCount - 1; i >= 0; --i) {
			std::cout << std::hex << tracepad[i].u64 << std::endl;
		}
#endif

	}
}