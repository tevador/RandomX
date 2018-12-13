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
#include <new>
#include "intrinPortable.h"

namespace RandomX {

	using addr_t = uint32_t;

	constexpr int RoundToNearest = 0;
	constexpr int RoundDown = 1;
	constexpr int RoundUp = 2;
	constexpr int RoundToZero = 3;

	constexpr int SeedSize = 32;

	constexpr int CacheBlockSize = 1024;
	constexpr int BlockExpansionRatio = 64;
	constexpr uint32_t DatasetBlockSize = BlockExpansionRatio * CacheBlockSize;
	constexpr uint32_t DatasetBlockCount = 65536;
	constexpr uint32_t CacheSize = DatasetBlockCount * CacheBlockSize;
	constexpr uint64_t DatasetSize = (uint64_t)DatasetBlockCount * DatasetBlockSize;

	constexpr int ArgonIterations = 12;
	constexpr uint32_t ArgonMemorySize = 65536; //KiB
	constexpr int ArgonLanes = 1;
	const char ArgonSalt[] = "Monero\x1A$";
	constexpr int ArgonSaltSize = sizeof(ArgonSalt) - 1;

#ifdef TRACE
	constexpr bool trace = true;
#else
	constexpr bool trace = false;
#endif

	typedef union {
		double f64;
		int64_t i64;
		uint64_t u64;
		int32_t i32;
		uint32_t u32;
	} convertible_t;

	constexpr int ProgramLength = 512;
	constexpr int InstructionCount = 1024 * 1024;
	constexpr uint32_t ScratchpadSize = 256 * 1024;
	constexpr uint32_t ScratchpadLength = ScratchpadSize / sizeof(convertible_t);
	constexpr uint32_t ScratchpadL1 = ScratchpadSize / 16 / sizeof(convertible_t);
	constexpr uint32_t ScratchpadL2 = ScratchpadSize / sizeof(convertible_t);
	constexpr int RegistersCount = 8;

	struct LightClientMemory {
		uint8_t* cache;
		uint8_t* block;
		uint32_t blockNumber;
		alignas(16) __m128i keys[10];

		void* operator new(size_t size) {
			void* ptr = _mm_malloc(size, sizeof(__m128i));
			if (ptr == nullptr)
				throw std::bad_alloc();
			return ptr;
		}

		void operator delete(void* ptr) {
			_mm_free(ptr);
		}
	};

	struct MemoryRegisters {
		addr_t ma, mx;
		union {
			uint8_t* dataset;
			LightClientMemory* lcm;
		};
	};

	static_assert(sizeof(MemoryRegisters) == 2 * sizeof(addr_t) + sizeof(uintptr_t), "Invalid alignment of struct RandomX::MemoryRegisters");

	struct RegisterFile {
		convertible_t r[RegistersCount];
		convertible_t f[RegistersCount];
	};

	static_assert(sizeof(RegisterFile) == 2 * RegistersCount * sizeof(convertible_t), "Invalid alignment of struct RandomX::RegisterFile");

	typedef convertible_t(*DatasetReadFunc)(addr_t, MemoryRegisters&);

	extern "C" {
		void executeProgram(RegisterFile& registerFile, MemoryRegisters& memory, DatasetReadFunc readFunc, convertible_t* scratchpad);
	}
}