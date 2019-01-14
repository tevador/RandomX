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
#include "dataset.hpp"

namespace RandomX {

	class Cache {
	public:
		void* operator new(size_t size) {
			void* ptr = _mm_malloc(size, sizeof(__m128i));
			if (ptr == nullptr)
				throw std::bad_alloc();
			return ptr;
		}

		void operator delete(void* ptr) {
			_mm_free(ptr);
		}

		template<bool softAes>
		void initialize(const void* seed, size_t seedSize);

		const KeysContainer& getKeys() const {
			return keys;
		}

		const uint8_t* getCache() const {
			return memory;
		}
	private:
		alignas(16) KeysContainer keys;
		uint8_t memory[CacheSize];
		void argonFill(const void* seed, size_t seedSize);
	};
}