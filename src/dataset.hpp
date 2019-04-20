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
#include "common.hpp"
#include "superscalar_program.hpp"
#include "jit_compiler_x86.hpp"
#include "allocator.hpp"

/* Global scope for C binding */
struct randomx_dataset {
	virtual ~randomx_dataset() = 0;
	virtual bool allocate() = 0;
	uint8_t* memory = nullptr;
};

/* Global scope for C binding */
struct randomx_cache : public randomx_dataset {
	virtual randomx::DatasetInitFunc getInitFunc() = 0;
	virtual void initialize(const void *seed, size_t seedSize);
	randomx::SuperscalarProgram programs[RANDOMX_CACHE_ACCESSES];
	std::vector<uint64_t> reciprocalCache;
};

namespace randomx {

	template<class Allocator>
	struct Dataset : public randomx_dataset {
		~Dataset() override;
		bool allocate() override;
	};

	using DatasetDefault = Dataset<AlignedAllocator<CacheLineSize>>;
	using DatasetLargePage = Dataset<LargePageAllocator>;

	template<class Allocator>
	struct Cache : public randomx_cache {
		~Cache() override;
		bool allocate() override;
		DatasetInitFunc getInitFunc() override;
	};

	template<class Allocator>
	struct CacheWithJit : public Cache<Allocator> {
		using Cache<Allocator>::programs;
		using Cache<Allocator>::reciprocalCache;
		void initialize(const void *seed, size_t seedSize) override;
		DatasetInitFunc getInitFunc() override;
		JitCompilerX86 jit;
	};

	using CacheDefault = Cache<AlignedAllocator<CacheLineSize>>;
	using CacheWithJitDefault = CacheWithJit<AlignedAllocator<CacheLineSize>>;
	using CacheLargePage = Cache<LargePageAllocator>;
	using CacheWithJitLargePage = CacheWithJit<LargePageAllocator>;

	void initDatasetBlock(randomx_cache* cache, uint8_t* out, uint64_t blockNumber);
	void initDataset(randomx_cache* cache, uint8_t* dataset, uint32_t startBlock, uint32_t endBlock);
}
