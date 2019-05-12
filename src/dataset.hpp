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
#include <vector>
#include <type_traits>
#include "common.hpp"
#include "superscalar_program.hpp"
#include "allocator.hpp"

/* Global scope for C binding */
struct randomx_dataset {
	uint8_t* memory = nullptr;
	randomx::DatasetDeallocFunc* dealloc;
};

/* Global scope for C binding */
struct randomx_cache {
	uint8_t* memory = nullptr;
	randomx::CacheDeallocFunc* dealloc;
	randomx::JitCompiler* jit;
	randomx::CacheInitializeFunc* initialize;
	randomx::DatasetInitFunc* datasetInit;
	randomx::SuperscalarProgram programs[RANDOMX_CACHE_ACCESSES];
	std::vector<uint64_t> reciprocalCache;
};

//A pointer to a standard-layout struct object points to its initial member
static_assert(std::is_standard_layout<randomx_dataset>(), "randomx_dataset must be a standard-layout struct");
static_assert(std::is_standard_layout<randomx_cache>(), "randomx_cache must be a standard-layout struct");

namespace randomx {

	using DefaultAllocator = AlignedAllocator<CacheLineSize>;

	template<class Allocator>
	void deallocDataset(randomx_dataset* dataset) {
		if (dataset->memory != nullptr)
			Allocator::freeMemory(dataset->memory, DatasetSize);
	}

	template<class Allocator>
	void deallocCache(randomx_cache* cache) {
		if(cache->memory != nullptr)
			Allocator::freeMemory(cache->memory, CacheSize);
		if (cache->jit != nullptr)
			delete cache->jit;
	}

	void initCache(randomx_cache*, const void*, size_t);
	void initCacheCompile(randomx_cache*, const void*, size_t);
	void initDatasetItem(randomx_cache* cache, uint8_t* out, uint64_t blockNumber);
	void initDataset(randomx_cache* cache, uint8_t* dataset, uint32_t startBlock, uint32_t endBlock);
}
