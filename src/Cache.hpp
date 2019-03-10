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

#include <cstdint>
#include <new>
#include "common.hpp"
#include "intrinPortable.h"
#include "virtualMemory.hpp"

namespace RandomX {

	void argonFill(Cache& cache, const void* seed, size_t seedSize);

	inline uint8_t* allocCache(size_t size, bool largePages) {
		if (largePages) {
			return (uint8_t*)allocLargePagesMemory(size);
		}
		else {
			void* ptr = _mm_malloc(size, sizeof(__m128i));
			if (ptr == nullptr)
				throw std::bad_alloc();
			return (uint8_t*)ptr;
		}
	}

	inline void deallocCache(Cache cache, bool largePages) {
		if (largePages) {
			freePagedMemory(cache.memory, cache.size);
		}
		else {
			_mm_free(cache.memory);
		}
	}
}