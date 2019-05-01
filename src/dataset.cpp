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

/* Original code from Argon2 reference source code package used under CC0 Licence
 * https://github.com/P-H-C/phc-winner-argon2
 * Copyright 2015
 * Daniel Dinu, Dmitry Khovratovich, Jean-Philippe Aumasson, and Samuel Neves
*/

#include <new>
#include <algorithm>
#include <stdexcept>
#include <cstring>
#include <limits>
#include <cstring>

#include "common.hpp"
#include "dataset.hpp"
#include "virtual_memory.hpp"
#include "superscalar.hpp"
#include "blake2_generator.hpp"
#include "reciprocal.h"
#include "blake2/endian.h"
#include "argon2.h"
#include "argon2_core.h"
#include "jit_compiler_x86.hpp"
#include "intrin_portable.h"

static_assert(RANDOMX_ARGON_MEMORY % (RANDOMX_ARGON_LANES * ARGON2_SYNC_POINTS) == 0, "RANDOMX_ARGON_MEMORY - invalid value");
static_assert(ARGON2_BLOCK_SIZE == randomx::ArgonBlockSize, "Unpexpected value of ARGON2_BLOCK_SIZE");

namespace randomx {

	void initCache(randomx_cache* cache, const void* key, size_t keySize) {
		uint32_t memory_blocks, segment_length;
		argon2_instance_t instance;
		argon2_context context;

		context.out = nullptr;
		context.outlen = 0;
		context.pwd = CONST_CAST(uint8_t *)key;
		context.pwdlen = (uint32_t)keySize;
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
		instance.memory = (block*)cache->memory;

		if (instance.threads > instance.lanes) {
			instance.threads = instance.lanes;
		}

		/* 3. Initialization: Hashing inputs, allocating memory, filling first
		 * blocks
		 */
		argon_initialize(&instance, &context);

		fill_memory_blocks(&instance);

		cache->reciprocalCache.clear();
		randomx::Blake2Generator gen(key, keySize);
		for (int i = 0; i < RANDOMX_CACHE_ACCESSES; ++i) {
			randomx::generateSuperscalar(cache->programs[i], gen);
			for (unsigned j = 0; j < cache->programs[i].getSize(); ++j) {
				auto& instr = cache->programs[i](j);
				if (instr.opcode == randomx::SuperscalarInstructionType::IMUL_RCP) {
					auto rcp = randomx_reciprocal(instr.getImm32());
					instr.setImm32(cache->reciprocalCache.size());
					cache->reciprocalCache.push_back(rcp);
				}
			}
		}
	}

	void initCacheCompile(randomx_cache* cache, const void* key, size_t keySize) {
		initCache(cache, key, keySize);
		cache->jit->generateSuperscalarHash(cache->programs, cache->reciprocalCache);
		cache->jit->generateDatasetInitCode();
	}

	constexpr uint64_t superscalarMul0 = 6364136223846793005ULL;
	constexpr uint64_t superscalarAdd1 = 9298411001130361340ULL;
	constexpr uint64_t superscalarAdd2 = 12065312585734608966ULL;
	constexpr uint64_t superscalarAdd3 = 9306329213124626780ULL;
	constexpr uint64_t superscalarAdd4 = 5281919268842080866ULL;
	constexpr uint64_t superscalarAdd5 = 10536153434571861004ULL;
	constexpr uint64_t superscalarAdd6 = 3398623926847679864ULL;
	constexpr uint64_t superscalarAdd7 = 9549104520008361294ULL;

	static inline uint8_t* getMixBlock(uint64_t registerValue, uint8_t *memory) {
		constexpr uint32_t mask = CacheSize / CacheLineSize - 1;
		return memory + (registerValue & mask) * CacheLineSize;
	}

	void initDatasetItem(randomx_cache* cache, uint8_t* out, uint64_t itemNumber) {
		int_reg_t rl[8];
		uint8_t* mixBlock;
		uint64_t registerValue = itemNumber;
		rl[0] = (itemNumber + 1) * superscalarMul0;
		rl[1] = rl[0] ^ superscalarAdd1;
		rl[2] = rl[0] ^ superscalarAdd2;
		rl[3] = rl[0] ^ superscalarAdd3;
		rl[4] = rl[0] ^ superscalarAdd4;
		rl[5] = rl[0] ^ superscalarAdd5;
		rl[6] = rl[0] ^ superscalarAdd6;
		rl[7] = rl[0] ^ superscalarAdd7;
		for (unsigned i = 0; i < RANDOMX_CACHE_ACCESSES; ++i) {
			mixBlock = getMixBlock(registerValue, cache->memory);
			PREFETCHNTA(mixBlock);
			SuperscalarProgram& prog = cache->programs[i];

			executeSuperscalar(rl, prog, &cache->reciprocalCache);

			for (unsigned q = 0; q < 8; ++q)
				rl[q] ^= load64_native(mixBlock + 8 * q);

			registerValue = rl[prog.getAddressRegister()];
		}

		memcpy(out, &rl, CacheLineSize);
	}

	void initDataset(randomx_cache* cache, uint8_t* dataset, uint32_t startItem, uint32_t endItem) {
		for (uint32_t itemNumber = startItem; itemNumber < endItem; ++itemNumber, dataset += CacheLineSize)
			initDatasetItem(cache, dataset, itemNumber);
	}
}
