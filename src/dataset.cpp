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
#include "Cache.hpp"
#include "virtualMemory.hpp"
#include "softAes.h"
#include "squareHash.h"
#include "blake2/endian.h"

#if defined(__SSE2__)
#include <wmmintrin.h>
#define PREFETCHNTA(x) _mm_prefetch((const char *)(x), _MM_HINT_NTA)
#else
#define PREFETCH(memory)
#endif

namespace RandomX {

	void initBlock(const uint8_t* cache, uint8_t* out, uint32_t blockNumber, const KeysContainer& keys) {
		uint64_t r0, r1, r2, r3, r4, r5, r6, r7;

		r0 = 4ULL * blockNumber;
		r1 = r2 = r3 = r4 = r5 = r6 = r7 = 0;

		constexpr uint32_t mask = (CacheSize - 1) & CacheLineAlignMask;

		for (auto i = 0; i < DatasetIterations; ++i) {
			const uint8_t* mixBlock = cache + (r0 & mask);
			PREFETCHNTA(mixBlock);
			r0 = squareHash(r0);
			r0 ^= load64(mixBlock + 0);
			r1 ^= load64(mixBlock + 8);
			r2 ^= load64(mixBlock + 16);
			r3 ^= load64(mixBlock + 24);
			r4 ^= load64(mixBlock + 32);
			r5 ^= load64(mixBlock + 40);
			r6 ^= load64(mixBlock + 48);
			r7 ^= load64(mixBlock + 56);
		}

		store64(out + 0, r0);
		store64(out + 8, r1);
		store64(out + 16, r2);
		store64(out + 24, r3);
		store64(out + 32, r4);
		store64(out + 40, r5);
		store64(out + 48, r6);
		store64(out + 56, r7);
	}

	void datasetRead(addr_t addr, MemoryRegisters& memory, RegisterFile& reg) {
		uint64_t* datasetLine = (uint64_t*)(memory.ds.dataset + memory.ma);
		memory.mx ^= addr;
		memory.mx &= -64; //align to cache line
		std::swap(memory.mx, memory.ma);
		PREFETCHNTA(memory.ds.dataset + memory.ma);
		for (int i = 0; i < RegistersCount; ++i)
			reg.r[i] ^= datasetLine[i];
	}

	void datasetReadLight(addr_t addr, MemoryRegisters& memory, int_reg_t (&reg)[RegistersCount]) {
		memory.mx ^= addr;
		memory.mx &= CacheLineAlignMask; //align to cache line
		Cache* cache = memory.ds.cache;
		uint64_t datasetLine[CacheLineSize / sizeof(uint64_t)];
		initBlock(cache->getCache(), (uint8_t*)datasetLine, memory.ma / CacheLineSize, cache->getKeys());
		for (int i = 0; i < RegistersCount; ++i)
			reg[i] ^= datasetLine[i];
		std::swap(memory.mx, memory.ma);
	}

	void datasetReadLightAsync(addr_t addr, MemoryRegisters& memory, int_reg_t(&reg)[RegistersCount]) {
		ILightClientAsyncWorker* aw = memory.ds.asyncWorker;
		const uint64_t* datasetLine = aw->getBlock(memory.ma);
		for (int i = 0; i < RegistersCount; ++i)
			reg[i] ^= datasetLine[i];
		memory.mx ^= addr;
		memory.mx &= CacheLineAlignMask; //align to cache line
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
			initBlock(cache->getCache(), ds.dataset + i * CacheLineSize, i, cache->getKeys());
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
			initBlock(buffer, buffer, 0, keys);
		}
	}

	template void aesBench<false>(uint32_t blockCount);
	template void aesBench<true>(uint32_t blockCount);
}
