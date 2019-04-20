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
#include <cstring>

#include "common.hpp"
#include "dataset.hpp"
#include "virtualMemory.hpp"
#include "superscalarGenerator.hpp"
#include "Blake2Generator.hpp"
#include "reciprocal.h"
#include "blake2/endian.h"
#include "argon2.h"
#include "argon2_core.h"

#if defined(__SSE2__)
#include <wmmintrin.h>
#define PREFETCHNTA(x) _mm_prefetch((const char *)(x), _MM_HINT_NTA)
#else
#define PREFETCH(memory)
#endif

randomx_dataset::~randomx_dataset() {

}

static_assert(RANDOMX_ARGON_MEMORY % (RANDOMX_ARGON_LANES * ARGON2_SYNC_POINTS) == 0, "RANDOMX_ARGON_MEMORY - invalid value");

void randomx_cache::initialize(const void *seed, size_t seedSize) {
	uint32_t memory_blocks, segment_length;
	argon2_instance_t instance;
	argon2_context context;

	context.out = nullptr;
	context.outlen = 0;
	context.pwd = CONST_CAST(uint8_t *)seed;
	context.pwdlen = (uint32_t)seedSize;
	context.salt = CONST_CAST(uint8_t *)RANDOMX_ARGON_SALT;
	context.saltlen = (uint32_t)randomx::ArgonSaltSize;
	context.secret = NULL;
	context.secretlen = 0;
	context.ad = NULL;
	context.adlen = 0;
	context.t_cost = RANDOMX_ARGON_ITERATIONS;
	context.m_cost = RANDOMX_ARGON_MEMORY;
	context.lanes = RANDOMX_ARGON_LANES;
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

	reciprocalCache.clear();
	randomx::Blake2Generator gen(seed, 1000);
	for (int i = 0; i < RANDOMX_CACHE_ACCESSES; ++i) {
		randomx::generateSuperscalar(programs[i], gen);
		for (unsigned j = 0; j < programs[i].getSize(); ++j) {
			auto& instr = programs[i](j);
			if (instr.opcode == randomx::SuperscalarInstructionType::IMUL_RCP) {
				auto rcp = reciprocal(instr.getImm32());
				instr.setImm32(reciprocalCache.size());
				reciprocalCache.push_back(rcp);
			}
		}
	}
}

namespace randomx {

	template<class Allocator>
	bool Dataset<Allocator>::allocate() {
		memory = (uint8_t*)Allocator::allocMemory(RANDOMX_DATASET_SIZE);
		return true;
	}

	template<class Allocator>
	Dataset<Allocator>::~Dataset() {
		Allocator::freeMemory(memory, RANDOMX_DATASET_SIZE);
	}

	template<class Allocator>
	bool Cache<Allocator>::allocate() {
		memory = (uint8_t*)Allocator::allocMemory(RANDOMX_ARGON_MEMORY * ARGON2_BLOCK_SIZE);
		return true;
	}

	template<class Allocator>
	Cache<Allocator>::~Cache() {
		Allocator::freeMemory(memory, RANDOMX_ARGON_MEMORY * ARGON2_BLOCK_SIZE);
	}

	template<class Allocator>
	DatasetInitFunc Cache<Allocator>::getInitFunc() {
		return &initDataset;
	}

	template<class Allocator>
	DatasetInitFunc CacheWithJit<Allocator>::getInitFunc() {
		return jit.getDatasetInitFunc();
	}

	template<class Allocator>
	void CacheWithJit<Allocator>::initialize(const void *seed, size_t seedSize) {
		randomx_cache::initialize(seed, seedSize);
		jit.generateSuperscalarHash(programs, reciprocalCache);
		jit.generateDatasetInitCode();
	}

	template class Dataset<AlignedAllocator<CacheLineSize>>;
	template class Dataset<LargePageAllocator>;
	template class Cache<AlignedAllocator<CacheLineSize>>;
	template class Cache<LargePageAllocator>;
	template class CacheWithJit<AlignedAllocator<CacheLineSize>>;
	template class CacheWithJit<LargePageAllocator>;

	constexpr uint64_t superscalarMul0 = 6364136223846793005ULL;
	constexpr uint64_t superscalarAdd1 = 9298410992540426748ULL;
	constexpr uint64_t superscalarAdd2 = 12065312585734608966ULL;
	constexpr uint64_t superscalarAdd3 = 9306329213124610396ULL;
	constexpr uint64_t superscalarAdd4 = 5281919268842080866ULL;
	constexpr uint64_t superscalarAdd5 = 10536153434571861004ULL;
	constexpr uint64_t superscalarAdd6 = 3398623926847679864ULL;
	constexpr uint64_t superscalarAdd7 = 9549104520008361294ULL;

	static inline uint8_t* getMixBlock(uint64_t registerValue, uint8_t *memory) {
		constexpr uint32_t mask = (RANDOMX_ARGON_MEMORY * ArgonBlockSize / CacheLineSize - 1);
		return memory + (registerValue & mask) * CacheLineSize;
	}

	void initDatasetBlock(randomx_cache* cache, uint8_t* out, uint64_t blockNumber) {
		int_reg_t rl[8];
		uint8_t* mixBlock;
		uint64_t registerValue = blockNumber;
		rl[0] = (blockNumber + 1) * superscalarMul0;
		rl[1] = rl[0] ^ superscalarAdd1;
		rl[2] = rl[0] ^ superscalarAdd2;
		rl[3] = rl[0] ^ superscalarAdd3;
		rl[4] = rl[0] ^ superscalarAdd4;
		rl[5] = rl[0] ^ superscalarAdd5;
		rl[6] = rl[0] ^ superscalarAdd6;
		rl[7] = rl[0] ^ superscalarAdd7;
		for (unsigned i = 0; i < RANDOMX_CACHE_ACCESSES; ++i) {
			mixBlock = getMixBlock(registerValue, cache->memory);
			SuperscalarProgram& prog = cache->programs[i];

			executeSuperscalar(rl, prog, &cache->reciprocalCache);

			for (unsigned q = 0; q < 8; ++q)
				rl[q] ^= load64(mixBlock + 8 * q);

			registerValue = rl[prog.getAddressRegister()];
		}

		memcpy(out, &rl, CacheLineSize);
	}

	void initDataset(randomx_cache* cache, uint8_t* dataset, uint32_t startBlock, uint32_t endBlock) {
		for (uint32_t blockNumber = startBlock; blockNumber < endBlock; ++blockNumber, dataset += CacheLineSize)
			initDatasetBlock(cache, dataset, blockNumber);
	}
	
	/*void datasetAlloc(dataset_t& ds, bool largePages) {
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

	void datasetInitCache(const void* seed, dataset_t& ds, bool largePages) {
		ds.cache.memory = allocCache(ds.cache.size, largePages);
		argonFill(ds.cache, seed, SeedSize);
	}*/
}
