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
#include <limits>

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

	void initBlock(const Cache& cache, uint8_t* out, uint64_t blockNumber) {
		uint64_t c0, c1, c2, c3, c4, c5, c6, c7;

		c0 = 4ULL * blockNumber;
		c1 = c2 = c3 = c4 = c5 = c6 = c7 = 0;

		constexpr uint32_t mask = (CacheSize - 1) & CacheLineAlignMask;

		for (auto i = 0; i < RANDOMX_CACHE_ACCESSES; ++i) {
			const uint8_t* mixBlock;
			if (RANDOMX_ARGON_GROWTH == 0) {
				constexpr uint32_t mask = (RANDOMX_ARGON_MEMORY * ArgonBlockSize / CacheLineSize - 1);
				mixBlock = cache.memory + (c0 & mask) * CacheLineSize;
			}
			else {
				const uint32_t modulus = cache.size / CacheLineSize;
				mixBlock = cache.memory + (c0 % modulus) * CacheLineSize;
			}
			PREFETCHNTA(mixBlock);
			c0 = squareHash(c0);
			c0 ^= load64(mixBlock + 0);
			c1 ^= load64(mixBlock + 8);
			c2 ^= load64(mixBlock + 16);
			c3 ^= load64(mixBlock + 24);
			c4 ^= load64(mixBlock + 32);
			c5 ^= load64(mixBlock + 40);
			c6 ^= load64(mixBlock + 48);
			c7 ^= load64(mixBlock + 56);
		}

		store64(out + 0, c0);
		store64(out + 8, c1);
		store64(out + 16, c2);
		store64(out + 24, c3);
		store64(out + 32, c4);
		store64(out + 40, c5);
		store64(out + 48, c6);
		store64(out + 56, c7);
	}

	void datasetRead(addr_t addr, MemoryRegisters& memory, RegisterFile& reg) {
		uint64_t* datasetLine = (uint64_t*)(memory.ds.dataset.memory + memory.ma);
		memory.mx ^= addr;
		memory.mx &= -64; //align to cache line
		std::swap(memory.mx, memory.ma);
		PREFETCHNTA(memory.ds.dataset.memory + memory.ma);
		for (int i = 0; i < RegistersCount; ++i)
			reg.r[i] ^= datasetLine[i];
	}

	void datasetReadLight(addr_t addr, MemoryRegisters& memory, int_reg_t (&reg)[RegistersCount]) {
		memory.mx ^= addr;
		memory.mx &= CacheLineAlignMask; //align to cache line
		Cache& cache = memory.ds.cache;
		uint64_t datasetLine[CacheLineSize / sizeof(uint64_t)];
		initBlock(cache, (uint8_t*)datasetLine, memory.ma / CacheLineSize);
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
		if (std::numeric_limits<size_t>::max() < RANDOMX_DATASET_SIZE)
			throw std::runtime_error("Platform doesn't support enough memory for the dataset");
		if (largePages) {
			ds.dataset.memory = (uint8_t*)allocLargePagesMemory(ds.dataset.size);
		}
		else {
			ds.dataset.memory = (uint8_t*)_mm_malloc(ds.dataset.size, 64);
			if (ds.dataset.memory == nullptr) {
				throw std::runtime_error("Dataset memory allocation failed. >4 GiB of free virtual memory is needed.");
			}
		}
	}

	void datasetInit(Cache& cache, Dataset& ds, uint32_t startBlock, uint32_t blockCount) {
		for (uint64_t i = startBlock; i < startBlock + blockCount; ++i) {
			initBlock(cache, ds.memory + i * CacheLineSize, i);
		}
	}

	void datasetInitCache(const void* seed, dataset_t& ds, bool largePages) {
		ds.cache.memory = allocCache(ds.cache.size, largePages);
		argonFill(ds.cache, seed, SeedSize);
	}
}
