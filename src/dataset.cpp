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
#include "softAes.h"
#include "squareHash.h"

#if defined(__SSE2__)
#include <wmmintrin.h>
#define PREFETCHNTA(x) _mm_prefetch((const char *)(x), _MM_HINT_NTA)
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
	void initBlock(const uint8_t* intermediate, uint8_t* out, uint32_t blockNumber, const KeysContainer& keys) {
		uint64_t r0, r1, r2, r3, r4, r5, r6, r7;

		r0 = 4ULL * blockNumber;
		r1 = r2 = r3 = r4 = r5 = r6 = r7 = 0;

		constexpr int mask = (CacheSize - 1) & -64;

		for (auto i = 0; i < DatasetIterations; ++i) {
			uint64_t* mix = (uint64_t*)(intermediate + (r0 & mask));
			PREFETCHNTA(mix);
			r0 = squareHash(r0);
			r0 ^= mix[0];
			r1 ^= mix[1];
			r2 ^= mix[2];
			r3 ^= mix[3];
			r4 ^= mix[4];
			r5 ^= mix[5];
			r6 ^= mix[6];
			r7 ^= mix[7];
		}

		uint64_t* out64 = (uint64_t*)out;

		out64[0] = r0;
		out64[1] = r1;
		out64[2] = r2;
		out64[3] = r3;
		out64[4] = r4;
		out64[5] = r5;
		out64[6] = r6;
		out64[7] = r7;
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
		PREFETCHNTA(memory.ds.dataset + memory.ma);
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
	void datasetInitCache(const void* seed, dataset_t& ds, bool largePages) {
		ds.cache = new(Cache::alloc(largePages)) Cache();
		ds.cache->initialize<softAes>(seed, SeedSize);
	}

	template
		void datasetInitCache<false>(const void*, dataset_t&, bool);

	template
		void datasetInitCache<true>(const void*, dataset_t&, bool);

	template<bool softAes>
	void aesBench(uint32_t blockCount) {
		alignas(16) KeysContainer keys;
		alignas(16) uint8_t buffer[CacheLineSize];
		for (uint32_t block = 0; block < blockCount; ++block) {
			initBlock<softAes>(buffer, buffer, 0, keys);
		}
	}

	template void aesBench<false>(uint32_t blockCount);
	template void aesBench<true>(uint32_t blockCount);
}
