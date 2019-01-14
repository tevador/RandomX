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

#include <new>
#include <algorithm>
#include <stdexcept>
#include <cstring>

#include "common.hpp"
#include "dataset.hpp"
#include "Pcg32.hpp"
#include "Cache.hpp"
#include "virtualMemory.hpp"

#if defined(__SSE2__)
#include <wmmintrin.h>
#define PREFETCH(memory) _mm_prefetch((const char *)((memory).ds.dataset + (memory).ma), _MM_HINT_NTA)
#else
#define PREFETCH(memory)
#endif

namespace RandomX {

	template<typename T>
	static inline void shuffle(T* buffer, size_t bytes, Pcg32& gen) {
		auto count = bytes / sizeof(T);
		for (auto i = count - 1; i >= 1; --i) {
			int j = gen.getUniform(0, i);
			std::swap(buffer[j], buffer[i]);
		}
	}

	template<bool soft>
	static inline __m128i aesenc(__m128i in, __m128i key) {
		return soft ? soft_aesenc(in, key) : _mm_aesenc_si128(in, key);
	}

	template<bool soft>
	static inline __m128i aesdec(__m128i in, __m128i key) {
		return soft ? soft_aesdec(in, key) : _mm_aesdec_si128(in, key);
	}

#define AES_ROUND(i) x0 = aesdec<soft>(x0, keys[i]); \
	x1 = aesenc<soft>(x1, keys[i]); \
	x2 = aesdec<soft>(x2, keys[i]); \
	x3 = aesenc<soft>(x3, keys[i])

	template<bool soft>
	void initBlock(const uint8_t* in, uint8_t* out, uint32_t blockNumber, const KeysContainer& keys) {
		__m128i x0, x1, x2, x3, iv;
		//block number 0..67108863
		//Initialization vector = block number extended to 128 bits
		iv = _mm_cvtsi32_si128(blockNumber);
		uint32_t cacheBlockNumber = blockNumber / BlockExpansionRatio; //0..1048575
		__m128i* cacheCacheLine = (__m128i*)(in + cacheBlockNumber * CacheLineSize);
		__m128i* datasetCacheLine = (__m128i*)out;

		x0 = _mm_load_si128(cacheCacheLine + 0);
		x1 = _mm_load_si128(cacheCacheLine + 1);
		x2 = _mm_load_si128(cacheCacheLine + 2);
		x3 = _mm_load_si128(cacheCacheLine + 3);

		x0 = _mm_xor_si128(x0, iv);
		x1 = _mm_xor_si128(x1, iv);
		x2 = _mm_xor_si128(x2, iv);
		x3 = _mm_xor_si128(x3, iv);

		for (auto i = 0; i < DatasetIterations; ++i) {
			AES_ROUND(0);
			AES_ROUND(1);
			AES_ROUND(2);
			AES_ROUND(3);
			AES_ROUND(4);
			AES_ROUND(5);
			AES_ROUND(6);
			AES_ROUND(7);
			AES_ROUND(8);
			AES_ROUND(9);
		}

		_mm_store_si128(datasetCacheLine + 0, x0);
		_mm_store_si128(datasetCacheLine + 1, x1);
		_mm_store_si128(datasetCacheLine + 2, x2);
		_mm_store_si128(datasetCacheLine + 3, x3);
	}

	template
		void initBlock<true>(const uint8_t*, uint8_t*, uint32_t, const KeysContainer&);

	template
		void initBlock<false>(const uint8_t*, uint8_t*, uint32_t, const KeysContainer&);

	void datasetRead(addr_t addr, MemoryRegisters& memory, RegisterFile& reg) {
		uint64_t* datasetLine = (uint64_t*)(memory.ds.dataset + memory.ma);
		memory.mx ^= addr;
		memory.mx &= -64; //align to cache line
		std::swap(memory.mx, memory.ma);
		PREFETCH(memory);
		for (int i = 0; i < RegistersCount; ++i)
			reg.r[i].u64 ^= datasetLine[i];
	}

	template<bool softAes>
	void datasetReadLight(addr_t addr, MemoryRegisters& memory, RegisterFile& reg) {
		Cache* cache = memory.ds.cache;
		uint64_t datasetLine[CacheLineSize / sizeof(uint64_t)];
		initBlock<softAes>(cache->getCache(), (uint8_t*)datasetLine, memory.ma / CacheLineSize, cache->getKeys());
		for (int i = 0; i < RegistersCount; ++i)
			reg.r[i].u64 ^= datasetLine[i];
		memory.mx ^= addr;
		memory.mx &= -64; //align to cache line
		std::swap(memory.mx, memory.ma);
	}

	template
		void datasetReadLight<false>(addr_t addr, MemoryRegisters& memory, RegisterFile& reg);

	template
		void datasetReadLight<true>(addr_t addr, MemoryRegisters& memory, RegisterFile& reg);

	void datasetReadLightAsync(addr_t addr, MemoryRegisters& memory, RegisterFile& reg) {
		ILightClientAsyncWorker* aw = memory.ds.asyncWorker;
		const uint64_t* datasetLine = aw->getBlock(memory.ma);
		for (int i = 0; i < RegistersCount; ++i)
			reg.r[i].u64 ^= datasetLine[i];
		memory.mx ^= addr;
		memory.mx &= -64; //align to cache line
		std::swap(memory.mx, memory.ma);
		aw->prepareBlock(memory.ma);
	}

	void datasetAlloc(dataset_t& ds, bool largePages) {
		if (sizeof(size_t) <= 4)
			throw std::runtime_error("Platform doesn't support enough memory for the dataset");
		if (largePages) {
			ds.dataset = (uint8_t*)allocLargePagesMemory(DatasetSize);
		}
		else {
			ds.dataset = (uint8_t*)_mm_malloc(DatasetSize, 64);
			if (ds.dataset == nullptr) {
				throw std::runtime_error("Dataset memory allocation failed. >4 GiB of free virtual memory is needed.");
			}
		}
	}

	template<bool softAes>
	void datasetInit(Cache* cache, dataset_t ds, uint32_t startBlock, uint32_t blockCount) {
		for (uint32_t i = startBlock; i < startBlock + blockCount; ++i) {
			initBlock<softAes>(cache->getCache(), ds.dataset + i * CacheLineSize, i, cache->getKeys());
		}
	}

	template
		void datasetInit<false>(Cache*, dataset_t, uint32_t, uint32_t);

	template
		void datasetInit<true>(Cache*, dataset_t, uint32_t, uint32_t);

	template<bool softAes>
	void datasetInitCache(const void* seed, dataset_t& ds) {
		ds.cache = new Cache();
		ds.cache->initialize<softAes>(seed, SeedSize);
	}

	template
		void datasetInitCache<false>(const void*, dataset_t&);

	template
		void datasetInitCache<true>(const void*, dataset_t&);
}
