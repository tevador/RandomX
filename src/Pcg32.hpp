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

// Based on:
// *Really* minimal PCG32 code / (c) 2014 M.E. O'Neill / pcg-random.org
// Licensed under Apache License 2.0 (NO WARRANTY, etc. see website)

#pragma once
#include <cstdint>

#if defined(_MSC_VER)
#pragma warning (disable : 4146)
#endif

class Pcg32 {
public:
	typedef uint32_t result_type;
	static constexpr result_type min() { return 0U; }
	static constexpr result_type max() { return UINT32_MAX; }
	Pcg32(const void* seed) {
		auto* u64seed = (const uint64_t*)seed;
		state = *(u64seed + 0);
		inc = *(u64seed + 1) | 1ull;
	}
	Pcg32(uint64_t state, uint64_t inc) : state(state), inc(inc | 1ull) {
	}
	result_type operator()() {
		return next();
	}
	result_type getUniform(result_type min, result_type max) {
		const result_type range = max - min;
		const result_type erange = range + 1;
		result_type ret;

		for (;;) {
			ret = next();
			if (ret / erange < UINT32_MAX / erange || UINT32_MAX % erange == range) {
				ret %= erange;
				break;
			}
		}
		return ret + min;
	}
private:
	uint64_t state;
	uint64_t inc;
	result_type next() {
		uint64_t oldstate = state;
		// Advance internal state
		state = oldstate * 6364136223846793005ULL + inc;
		// Calculate output function (XSH RR), uses old state for max ILP
		uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
		uint32_t rot = oldstate >> 59u;
		return (xorshifted >> rot) | (xorshifted << (-rot & 31));
	}
};
