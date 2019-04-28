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
#include "vm_interpreted.hpp"

namespace randomx {

	template<class Allocator, bool softAes>
	class InterpretedLightVm : public InterpretedVm<Allocator, softAes> {
	public:
		using VmBase<Allocator, softAes>::mem;
		using VmBase<Allocator, softAes>::cachePtr;
		void* operator new(size_t size) {
			void* ptr = AlignedAllocator<CacheLineSize>::allocMemory(size);
			if (ptr == nullptr)
				throw std::bad_alloc();
			return ptr;
		}
		void operator delete(void* ptr) {
			AlignedAllocator<CacheLineSize>::freeMemory(ptr, sizeof(InterpretedLightVm));
		}
		void setDataset(randomx_dataset* dataset) override { }
		void setCache(randomx_cache* cache) override;
	protected:
		void datasetRead(uint32_t address, int_reg_t(&r)[8]) override;
	};

	using InterpretedLightVmDefault = InterpretedLightVm<AlignedAllocator<CacheLineSize>, true>;
	using InterpretedLightVmHardAes = InterpretedLightVm<AlignedAllocator<CacheLineSize>, false>;
	using InterpretedLightVmLargePage = InterpretedLightVm<LargePageAllocator, true>;
	using InterpretedLightVmLargePageHardAes = InterpretedLightVm<LargePageAllocator, false>;
}
