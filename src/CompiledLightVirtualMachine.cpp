/*
Copyright (c) 2019 tevador

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

#include "CompiledLightVirtualMachine.hpp"
#include "common.hpp"
#include <stdexcept>

namespace randomx {

	template<class Allocator, bool softAes>
	void CompiledLightVm<Allocator, softAes>::setCache(randomx_cache* cache) {
		this->mem.memory = cache->memory;
		//datasetRange = (size - RANDOMX_DATASET_SIZE + CacheLineSize) / CacheLineSize;
		this->compiler.generateSuperscalarHash(cache->programs, cache->reciprocalCache);
		//datasetBasePtr = ds.dataset.memory;
	}

	template<class Allocator, bool softAes>
	void CompiledLightVm<Allocator, softAes>::initialize() {
		randomx_vm::initialize();
		this->compiler.generateProgramLight(this->program, this->config);
		//mem.ds.dataset.memory = datasetBasePtr + (datasetBase * CacheLineSize);
	}

	template class CompiledLightVm<AlignedAllocator<CacheLineSize>, false>;
	template class CompiledLightVm<AlignedAllocator<CacheLineSize>, true>;
	template class CompiledLightVm<LargePageAllocator, false>;
	template class CompiledLightVm<LargePageAllocator, true>;
}