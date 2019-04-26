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

#include "vm_compiled_light.hpp"
#include "common.hpp"
#include <stdexcept>

namespace randomx {

	template<class Allocator, bool softAes>
	void CompiledLightVm<Allocator, softAes>::setCache(randomx_cache* cache) {
		mem.memory = cache->memory;
		compiler.generateSuperscalarHash(cache->programs, cache->reciprocalCache);
	}

	template<class Allocator, bool softAes>
	void CompiledLightVm<Allocator, softAes>::run(void* seed) {
		VmBase<Allocator, softAes>::generateProgram(seed);
		randomx_vm::initialize();
		compiler.generateProgramLight(program, config, datasetOffset);
		CompiledVm<Allocator, softAes>::execute();
	}

	template class CompiledLightVm<AlignedAllocator<CacheLineSize>, false>;
	template class CompiledLightVm<AlignedAllocator<CacheLineSize>, true>;
	template class CompiledLightVm<LargePageAllocator, false>;
	template class CompiledLightVm<LargePageAllocator, true>;
}