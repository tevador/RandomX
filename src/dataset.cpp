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

#include "common.hpp"
#include "dataset.hpp"
#include "Pcg32.hpp"
#include "argon2_core.h"
#include <new>
#include <algorithm>
#include <stdexcept>
#include <cstring>

#if defined(_MSC_VER)
#if defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP == 2)
#define __SSE2__ 1
#endif
#endif

#if defined(__SSE2__)
#include <wmmintrin.h>
#define PREFETCH(memory) _mm_prefetch((const char *)((memory).dataset + (memory).ma), _MM_HINT_T0)
#else
#define PREFETCH(memory)
#endif

namespace RandomX {

	void initializeCache(const void* input, size_t inputLength, void* memory) {
		uint32_t memory_blocks, segment_length;
		argon2_instance_t instance;
		argon2_context context;

		context.out = nullptr;
		context.outlen = 0;
		context.pwd = CONST_CAST(uint8_t *)input;
		context.pwdlen = (uint32_t)inputLength;
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
		initialize(&instance, &context);

		fill_memory_blocks(&instance);
	}

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
	static inline void aes_genkey_sub(__m128i* xout0, __m128i* xout2) {
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
	void expandAesKeys(const __m128i* seed, __m128i* keys) {
		__m128i xout0, xout2;
		xout0 = _mm_load_si128(seed);
		xout2 = _mm_load_si128(seed + 1);
		*keys++ = xout0;
		*keys++ = xout2;
		aes_genkey_sub<0x01, soft>(&xout0, &xout2);
		*keys++ = xout0;
		*keys++ = xout2;
		aes_genkey_sub<0x02, soft>(&xout0, &xout2);
		*keys++ = xout0;
		*keys++ = xout2;
		aes_genkey_sub<0x04, soft>(&xout0, &xout2);
		*keys++ = xout0;
		*keys++ = xout2;
		aes_genkey_sub<0x08, soft>(&xout0, &xout2);
		*keys++ = xout0;
		*keys++ = xout2;
	}

	template
		void expandAesKeys<true>(const __m128i* seed, __m128i* keys);

	template
		void expandAesKeys<false>(const __m128i* seed, __m128i* keys);

	template<typename T>
	static inline void shuffle(T* buffer, size_t bytes, Pcg32& gen) {
		auto count = bytes / sizeof(T);
		for (auto i = count - 1; i >= 1; --i) {
			int j = gen.getUniform(0, i);
			std::swap(buffer[j], buffer[i]);
		}
	}

	template<bool soft, bool enc>
	void initBlock(uint8_t* in, uint8_t* out, uint32_t blockNumber, const __m128i keys[10]) {
		__m128i xin, xout;
		//Initialization vector = block number extended to 128 bits
		xout = _mm_cvtsi32_si128(blockNumber);
		//Expand + AES
		for (uint32_t i = 0; i < DatasetBlockSize / sizeof(__m128i); ++i) {
			if ((i % 32) == 0) {
				xin = _mm_set_epi64x(*(uint64_t*)(in + i / 4), 0);
				xout = _mm_xor_si128(xin, xout);
			}
			if (enc) {
				xout = aesenc<soft>(xout, keys[0]);
				xout = aesenc<soft>(xout, keys[1]);
				xout = aesenc<soft>(xout, keys[2]);
				xout = aesenc<soft>(xout, keys[3]);
				xout = aesenc<soft>(xout, keys[4]);
				xout = aesenc<soft>(xout, keys[5]);
				xout = aesenc<soft>(xout, keys[6]);
				xout = aesenc<soft>(xout, keys[7]);
				xout = aesenc<soft>(xout, keys[8]);
				xout = aesenc<soft>(xout, keys[9]);
			}
			else {
				xout = aesdec<soft>(xout, keys[0]);
				xout = aesdec<soft>(xout, keys[1]);
				xout = aesdec<soft>(xout, keys[2]);
				xout = aesdec<soft>(xout, keys[3]);
				xout = aesdec<soft>(xout, keys[4]);
				xout = aesdec<soft>(xout, keys[5]);
				xout = aesdec<soft>(xout, keys[6]);
				xout = aesdec<soft>(xout, keys[7]);
				xout = aesdec<soft>(xout, keys[8]);
				xout = aesdec<soft>(xout, keys[9]);
			}
			_mm_store_si128((__m128i*)(out + i * sizeof(__m128i)), xout);
		}
		//Shuffle
		Pcg32 gen(&xout);
		shuffle<uint32_t>((uint32_t*)out, DatasetBlockSize, gen);
	}

	template
		void initBlock<true, true>(uint8_t* in, uint8_t* out, uint32_t blockNumber, const __m128i keys[10]);

	template
		void initBlock<true, false>(uint8_t* in, uint8_t* out, uint32_t blockNumber, const __m128i keys[10]);

	template
		void initBlock<false, true>(uint8_t* in, uint8_t* out, uint32_t blockNumber, const __m128i keys[10]);

	template
		void initBlock<false, false>(uint8_t* in, uint8_t* out, uint32_t blockNumber, const __m128i keys[10]);

	convertible_t datasetRead(addr_t addr, MemoryRegisters& memory) {
		convertible_t data;
		data.u64 = *(uint64_t*)(memory.dataset + memory.ma);
		memory.ma += 8;
		memory.mx ^= addr;
		if ((memory.mx & 0xFFF8) == 0) {
			memory.ma = memory.mx & ~7;
			PREFETCH(memory);
		}
		return data;
	}

	template<bool softAes>
	void initBlock(uint8_t* cache, uint8_t* block, uint32_t blockNumber, const __m128i k[10]) {
		if (blockNumber % 2 == 1) {
			initBlock<softAes, true>(cache + blockNumber * CacheBlockSize, block, blockNumber, k);
		}
		else {
			initBlock<softAes, false>(cache + blockNumber * CacheBlockSize, block, blockNumber, k);
		}
	}

	template<bool softAes>
	convertible_t datasetReadLight(addr_t addr, MemoryRegisters& memory) {
		convertible_t data;
		auto blockNumber = memory.ma / DatasetBlockSize;
		if (memory.lcm->blockNumber != blockNumber) {
			initBlock<softAes>(memory.lcm->cache + CacheShift, (uint8_t*)memory.lcm->block, blockNumber, memory.lcm->keys);
			memory.lcm->blockNumber = blockNumber;
		}
		data.u64 = *(uint64_t*)(memory.lcm->block + (memory.ma % DatasetBlockSize));
		memory.ma += 8;
		memory.mx ^= addr;
		if ((memory.mx & 0xFFF8) == 0) {
			memory.ma = memory.mx & ~7;
		}
		return data;
	}

	template
		convertible_t datasetReadLight<false>(addr_t addr, MemoryRegisters& memory);

	template
		convertible_t datasetReadLight<true>(addr_t addr, MemoryRegisters& memory);

	template<bool softAes>
	void datasetInit(const void* seed, uint8_t*& dataset) {
		if (sizeof(size_t) <= 4)
			throw std::runtime_error("Platform doesn't support enough memory for the dataset");
		dataset = (uint8_t*)_mm_malloc(DatasetSize, sizeof(__m128i));
		if (dataset == nullptr) {
			throw std::runtime_error("Dataset memory allocation failed. >4 GiB of virtual memory is needed.");
		}
		uint8_t* cache = (uint8_t*)_mm_malloc(CacheSize + CacheShift, sizeof(__m128i));
		if (cache == nullptr) {
			throw std::bad_alloc();
		}
		initializeCache(seed, SeedSize, cache);
		memcpy(cache + CacheSize, cache, CacheShift);
		alignas(16) __m128i keys[10];
		expandAesKeys<softAes>((const __m128i*)seed, keys);
		for (uint32_t i = 0; i < DatasetBlockCount; ++i) {
			initBlock<softAes>(cache + CacheShift, dataset + i * DatasetBlockSize, i, keys);
		}
		_mm_free(cache);
	}

	template
		void datasetInit<false>(const void*, uint8_t*&);

	template
		void datasetInit<true>(const void*, uint8_t*&);

	template<bool softAes>
	void datasetInitLight(const void* seed, LightClientMemory*& lcm) {
		lcm = new LightClientMemory();
		lcm->cache = (uint8_t*)_mm_malloc(CacheSize + CacheShift, sizeof(__m128i));
		if (lcm->cache == nullptr) {
			throw std::bad_alloc();
		}
		initializeCache(seed, SeedSize, lcm->cache);
		memcpy(lcm->cache + CacheSize, lcm->cache, CacheShift);
		expandAesKeys<softAes>((__m128i*)seed, lcm->keys);
		lcm->block = (uint8_t*)_mm_malloc(DatasetBlockSize, sizeof(__m128i));
		if (lcm->block == nullptr) {
			throw std::bad_alloc();
		}
		lcm->blockNumber = -1;
	}

	template
		void datasetInitLight<false>(const void*, LightClientMemory*&);

	template
		void datasetInitLight<true>(const void*, LightClientMemory*&);
}
