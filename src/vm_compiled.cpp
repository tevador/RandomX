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

#include "vm_compiled.hpp"
#include "common.hpp"

namespace randomx {

	static_assert(sizeof(MemoryRegisters) == 2 * sizeof(addr_t) + sizeof(uintptr_t), "Invalid alignment of struct randomx::MemoryRegisters");
	static_assert(sizeof(RegisterFile) == 256, "Invalid alignment of struct randomx::RegisterFile");

	template<class Allocator, bool softAes>
	void CompiledVm<Allocator, softAes>::setDataset(randomx_dataset* dataset) {
		mem.memory = dataset->memory;
		//datasetBasePtr = dataset.memory;
	}

	template<class Allocator, bool softAes>
	void CompiledVm<Allocator, softAes>::run(void* seed) {
		VmBase<Allocator, softAes>::generateProgram(seed);
		randomx_vm::initialize();
		compiler.generateProgram(program, config);
		//mem.memory = datasetBasePtr + (datasetBase * CacheLineSize);
		execute();
	}

	template<class Allocator, bool softAes>
	void CompiledVm<Allocator, softAes>::execute() {
		compiler.getProgramFunc()(reg, mem, scratchpad, RANDOMX_PROGRAM_ITERATIONS);
	}

	template class CompiledVm<AlignedAllocator<CacheLineSize>, false>;
	template class CompiledVm<AlignedAllocator<CacheLineSize>, true>;
	template class CompiledVm<LargePageAllocator, false>;
	template class CompiledVm<LargePageAllocator, true>;
}