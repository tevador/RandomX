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
#include "intrinPortable.h"
#include "argon2.h"
#include "common.hpp"
#include "softAes.h"

namespace RandomX {

	static_assert(ArgonMemorySize % (ArgonLanes * ARGON2_SYNC_POINTS) == 0, "ArgonMemorySize - invalid value");

	void initializeCache(const void* input, size_t inputLength, void* memory);

	template<bool soft>
	void expandAesKeys(const __m128i* seed, __m128i* keys);

	template<bool soft>
	inline __m128i aesenc(__m128i in, __m128i key) {
		return soft ? soft_aesenc(in, key) : _mm_aesenc_si128(in, key);
	}

	template<bool soft>
	inline __m128i aesdec(__m128i in, __m128i key) {
		return soft ? soft_aesdec(in, key) : _mm_aesdec_si128(in, key);
	}

	template<bool soft, bool enc>
	void initBlock(uint8_t* in, uint8_t* out, uint32_t blockNumber, const __m128i keys[10]);

	template<bool softAes>
	void initBlock(uint8_t* cache, uint8_t* block, uint32_t blockNumber, const __m128i keys[10]);

	template<bool softAes>
	void datasetInit(const void* seed, uint8_t*& dataset);

	convertible_t datasetRead(addr_t addr, MemoryRegisters& memory);

	template<bool softAes>
	void datasetInitLight(const void* seed, LightClientMemory*& lcm);

	template<bool softAes>
	convertible_t datasetReadLight(addr_t addr, MemoryRegisters& memory);
}

