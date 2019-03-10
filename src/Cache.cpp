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

#include <cstring>
#include "Cache.hpp"
#include "argon2.h"
#include "argon2_core.h"

namespace RandomX {

	static_assert(RANDOMX_ARGON_MEMORY % (RANDOMX_ARGON_LANES * ARGON2_SYNC_POINTS) == 0, "RANDOMX_ARGON_MEMORY - invalid value");
	static_assert(RANDOMX_ARGON_GROWTH % (RANDOMX_ARGON_LANES * ARGON2_SYNC_POINTS) == 0, "RANDOMX_ARGON_GROWTH - invalid value");

	void argonFill(Cache& cache, const void* seed, size_t seedSize) {
		uint32_t memory_blocks, segment_length;
		argon2_instance_t instance;
		argon2_context context;

		context.out = nullptr;
		context.outlen = 0;
		context.pwd = CONST_CAST(uint8_t *)seed;
		context.pwdlen = (uint32_t)seedSize;
		context.salt = CONST_CAST(uint8_t *)RANDOMX_ARGON_SALT;
		context.saltlen = (uint32_t)ArgonSaltSize;
		context.secret = NULL;
		context.secretlen = 0;
		context.ad = NULL;
		context.adlen = 0;
		context.t_cost = RANDOMX_ARGON_ITERATIONS;
		context.m_cost = cache.size / ArgonBlockSize;
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
		instance.memory = (block*)cache.memory;

		if (instance.threads > instance.lanes) {
			instance.threads = instance.lanes;
		}

		/* 3. Initialization: Hashing inputs, allocating memory, filling first
		 * blocks
		 */
		argon_initialize(&instance, &context);

		fill_memory_blocks(&instance);
	}
}