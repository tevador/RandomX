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

#pragma once

#include "allocator.hpp"
#include "virtualMemory.hpp"
#include "intrinPortable.h"
#include "common.hpp"

namespace randomx {

	template<size_t alignment>
	void* AlignedAllocator<alignment>::allocMemory(size_t count) {
		return _mm_malloc(count, alignment);
	}

	template<size_t alignment>
	void AlignedAllocator<alignment>::freeMemory(void* ptr, size_t count) {
		_mm_free(ptr);
	}

	template void* AlignedAllocator<CacheLineSize>::allocMemory(size_t count);
	template void AlignedAllocator<CacheLineSize>::freeMemory(void* ptr, size_t count);
	template void* AlignedAllocator<sizeof(__m128i)>::allocMemory(size_t count);
	template void AlignedAllocator<sizeof(__m128i)>::freeMemory(void* ptr, size_t count);

	void* LargePageAllocator::allocMemory(size_t count) {
		return allocLargePagesMemory(count);
	}

	void LargePageAllocator::freeMemory(void* ptr, size_t count) {
		freePagedMemory(ptr, count);
	};

}