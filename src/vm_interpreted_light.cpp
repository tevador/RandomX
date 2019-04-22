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

#include "vm_interpreted_light.hpp"
#include "dataset.hpp"

namespace randomx {

	template<class Allocator, bool softAes>
	void InterpretedLightVm<Allocator, softAes>::setCache(randomx_cache* cache) {
		mem.memory = cache->memory;
		//datasetRange = (size - RANDOMX_DATASET_SIZE + CacheLineSize) / CacheLineSize;
		cachePtr = cache;
	}

	template<class Allocator, bool softAes>
	void InterpretedLightVm<Allocator, softAes>::datasetRead(uint32_t address, int_reg_t(&r)[8]) {
		uint32_t itemNumber = address / CacheLineSize;
		int_reg_t rl[8];
		
		initDatasetItem(cachePtr, (uint8_t*)rl, itemNumber);

		for (unsigned q = 0; q < 8; ++q)
			r[q] ^= rl[q];
	}

	template class InterpretedLightVm<AlignedAllocator<CacheLineSize>, false>;
	template class InterpretedLightVm<AlignedAllocator<CacheLineSize>, true>;
	template class InterpretedLightVm<LargePageAllocator, false>;
	template class InterpretedLightVm<LargePageAllocator, true>;
}
