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

#include <new>
#include "VirtualMachine.hpp"
#include "JitCompilerX86.hpp"

namespace randomx {

	extern "C" {
		void executeProgram(RegisterFile&, MemoryRegisters&, uint8_t* /* scratchpad */, uint64_t);
	}

	template<class Allocator, bool softAes>
	class CompiledVm : public VmBase<Allocator, softAes> {
	public:
		void* operator new(size_t size) {
			void* ptr = AlignedAllocator<CacheLineSize>::allocMemory(size);
			if (ptr == nullptr)
				throw std::bad_alloc();
			return ptr;
		}
		void operator delete(void* ptr) {
			AlignedAllocator<CacheLineSize>::freeMemory(ptr, sizeof(CompiledVm));
		}
		void setDataset(randomx_dataset* dataset) override;
		void run(void* seed) override;

		using VmBase<Allocator, softAes>::mem;
		using VmBase<Allocator, softAes>::program;
		using VmBase<Allocator, softAes>::config;
		using VmBase<Allocator, softAes>::reg;
		using VmBase<Allocator, softAes>::scratchpad;
	protected:
		void execute();

		JitCompilerX86 compiler;
		uint8_t* datasetBasePtr;
	};

	using CompiledVmDefault = CompiledVm<AlignedAllocator<CacheLineSize>, true>;
	using CompiledVmHardAes = CompiledVm<AlignedAllocator<CacheLineSize>, false>;
	using CompiledVmLargePage = CompiledVm<LargePageAllocator, false>;
	using CompiledVmLargePageHardAes = CompiledVm<LargePageAllocator, true>;
}
