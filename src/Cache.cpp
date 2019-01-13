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

// Parts of this file are originally copyright (c) xmr-stak

#include <cstring>
#include "Cache.hpp"
#include "softAes.h"
#include "argon2.h"
#include "Pcg32.hpp"
#include "argon2_core.h"

namespace RandomX {

	static_assert(ArgonMemorySize % (ArgonLanes * ARGON2_SYNC_POINTS) == 0, "ArgonMemorySize - invalid value");

	// This will shift and xor tmp1 into itself as 4 32-bit vals such as
	// sl_xor(a1 a2 a3 a4) = a1 (a2^a1) (a3^a2^a1) (a4^a3^a2^a1)
	static inline __m128i sl_xor(__m128i tmp1) {
		__m128i tmp4;
		tmp4 = _mm_slli_si128(tmp1, 0x04);
		tmp1 = _mm_xor_si128(tmp1, tmp4);
		tmp4 = _mm_slli_si128(tmp4, 0x04);
		tmp1 = _mm_xor_si128(tmp1, tmp4);
		tmp4 = _mm_slli_si128(tmp4, 0x04);
		tmp1 = _mm_xor_si128(tmp1, tmp4);
		return tmp1;
	}

	template<uint8_t rcon, bool soft>
	static inline void aesGenKeys(__m128i* xout0, __m128i* xout2) {
		__m128i xout1 = soft ? soft_aeskeygenassist(*xout2, rcon) : _mm_aeskeygenassist_si128(*xout2, rcon);
		xout1 = _mm_shuffle_epi32(xout1, 0xFF);
		*xout0 = sl_xor(*xout0);
		*xout0 = _mm_xor_si128(*xout0, xout1);
		xout1 = soft ? soft_aeskeygenassist(*xout0, 0x00) : _mm_aeskeygenassist_si128(*xout0, 0x00);
		xout1 = _mm_shuffle_epi32(xout1, 0xAA);
		*xout2 = sl_xor(*xout2);
		*xout2 = _mm_xor_si128(*xout2, xout1);
	}

	template<bool soft>
	static inline void expandAesKeys(const __m128i* seed, __m128i* keys) {
		__m128i xout0, xout2;
		xout0 = _mm_load_si128(seed);
		xout2 = _mm_load_si128(seed + 1);
		*keys++ = xout0;
		*keys++ = xout2;
		aesGenKeys<0x01, soft>(&xout0, &xout2);
		*keys++ = xout0;
		*keys++ = xout2;
		aesGenKeys<0x02, soft>(&xout0, &xout2);
		*keys++ = xout0;
		*keys++ = xout2;
		aesGenKeys<0x04, soft>(&xout0, &xout2);
		*keys++ = xout0;
		*keys++ = xout2;
		aesGenKeys<0x08, soft>(&xout0, &xout2);
		*keys++ = xout0;
		*keys++ = xout2;
	}

	void Cache::argonFill(const void* seed, size_t seedSize) {
		uint32_t memory_blocks, segment_length;
		argon2_instance_t instance;
		argon2_context context;

		context.out = nullptr;
		context.outlen = 0;
		context.pwd = CONST_CAST(uint8_t *)seed;
		context.pwdlen = (uint32_t)seedSize;
		context.salt = CONST_CAST(uint8_t *)ArgonSalt;
		context.saltlen = (uint32_t)ArgonSaltSize;
		context.secret = NULL;
		context.secretlen = 0;
		context.ad = NULL;
		context.adlen = 0;
		context.t_cost = ArgonIterations;
		context.m_cost = ArgonMemorySize;
		context.lanes = ArgonLanes;
		context.threads = 1;
		context.allocate_cbk = NULL;
		context.free_cbk = NULL;
		context.flags = ARGON2_DEFAULT_FLAGS;
		context.version = ARGON2_VERSION_NUMBER;

		/* 2. Align memory size */
		/* Minimum memory_blocks = 8L blocks, where L is the number of lanes */
		memory_blocks = context.m_cost;

		segment_length = memory_blocks / (context.lanes * ARGON2_SYNC_POINTS);

		instance.version = context.version;
		instance.memory = NULL;
		instance.passes = context.t_cost;
		instance.memory_blocks = memory_blocks;
		instance.segment_length = segment_length;
		instance.lane_length = segment_length * ARGON2_SYNC_POINTS;
		instance.lanes = context.lanes;
		instance.threads = context.threads;
		instance.type = Argon2_d;
		instance.memory = (block*)memory;

		if (instance.threads > instance.lanes) {
			instance.threads = instance.lanes;
		}

		/* 3. Initialization: Hashing inputs, allocating memory, filling first
		 * blocks
		 */
		argon_initialize(&instance, &context);

		fill_memory_blocks(&instance);
	}

	template<bool softAes>
	void Cache::initialize(const void* seed, size_t seedSize) {
		//Argon2d memory fill
		argonFill(seed, seedSize);

		//AES keys
		expandAesKeys<softAes>((__m128i*)seed, keys.data());
	}

	template void Cache::initialize<true>(const void*, size_t);

	template void Cache::initialize<false>(const void*, size_t);
}