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
#include <iostream>
#include "blake2/endian.h"
#include "configuration.h"
#include "randomx.h"

namespace randomx {

	static_assert((RANDOMX_ARGON_MEMORY & (RANDOMX_ARGON_MEMORY - 1)) == 0, "RANDOMX_ARGON_MEMORY must be a power of 2.");
	static_assert((RANDOMX_DATASET_SIZE & (RANDOMX_DATASET_SIZE - 1)) == 0, "RANDOMX_DATASET_SIZE must be a power of 2.");
	static_assert(RANDOMX_DATASET_SIZE <= 4294967296ULL, "RANDOMX_DATASET_SIZE must not exceed 4294967296.");
	static_assert(RANDOMX_PROGRAM_SIZE > 0, "RANDOMX_PROGRAM_SIZE must be greater than 0");
	static_assert(RANDOMX_PROGRAM_ITERATIONS > 0, "RANDOMX_PROGRAM_ITERATIONS must be greater than 0");
	static_assert(RANDOMX_PROGRAM_COUNT > 0, "RANDOMX_PROGRAM_COUNT must be greater than 0");
	static_assert((RANDOMX_SCRATCHPAD_L3 & (RANDOMX_SCRATCHPAD_L3 - 1)) == 0, "RANDOMX_SCRATCHPAD_L3 must be a power of 2.");
	static_assert(RANDOMX_SCRATCHPAD_L3 >= RANDOMX_SCRATCHPAD_L2, "RANDOMX_SCRATCHPAD_L3 must be greater than or equal to RANDOMX_SCRATCHPAD_L2.");
	static_assert((RANDOMX_SCRATCHPAD_L2 & (RANDOMX_SCRATCHPAD_L2 - 1)) == 0, "RANDOMX_SCRATCHPAD_L2 must be a power of 2.");
	static_assert(RANDOMX_SCRATCHPAD_L2 >= RANDOMX_SCRATCHPAD_L1, "RANDOMX_SCRATCHPAD_L2 must be greater than or equal to RANDOMX_SCRATCHPAD_L1.");
	static_assert((RANDOMX_SCRATCHPAD_L1 & (RANDOMX_SCRATCHPAD_L1 - 1)) == 0, "RANDOMX_SCRATCHPAD_L1 must be a power of 2.");
	static_assert(RANDOMX_CACHE_ACCESSES > 1, "RANDOMX_CACHE_ACCESSES must be greater than 1");

	constexpr int wtSum = RANDOMX_FREQ_IADD_RS + RANDOMX_FREQ_IADD_M + RANDOMX_FREQ_ISUB_R + \
		RANDOMX_FREQ_ISUB_M + RANDOMX_FREQ_IMUL_R + RANDOMX_FREQ_IMUL_M + RANDOMX_FREQ_IMULH_R + \
		RANDOMX_FREQ_IMULH_M + RANDOMX_FREQ_ISMULH_R + RANDOMX_FREQ_ISMULH_M + RANDOMX_FREQ_IMUL_RCP + \
		RANDOMX_FREQ_INEG_R + RANDOMX_FREQ_IXOR_R + RANDOMX_FREQ_IXOR_M + RANDOMX_FREQ_IROR_R + RANDOMX_FREQ_ISWAP_R + \
		RANDOMX_FREQ_FSWAP_R + RANDOMX_FREQ_FADD_R + RANDOMX_FREQ_FADD_M + RANDOMX_FREQ_FSUB_R + RANDOMX_FREQ_FSUB_M + \
		RANDOMX_FREQ_FSCAL_R + RANDOMX_FREQ_FMUL_R + RANDOMX_FREQ_FDIV_M + RANDOMX_FREQ_FSQRT_R + RANDOMX_FREQ_COND_R + \
		RANDOMX_FREQ_CFROUND + RANDOMX_FREQ_ISTORE + RANDOMX_FREQ_NOP;

	static_assert(wtSum == 256,	"Sum of instruction frequencies must be 256.");

	using addr_t = uint32_t;

	constexpr int ArgonBlockSize = 1024;
	constexpr int ArgonSaltSize = sizeof(RANDOMX_ARGON_SALT) - 1;
	constexpr int CacheLineSize = RANDOMX_DATASET_ITEM_SIZE;
	constexpr int ScratchpadSize = RANDOMX_SCRATCHPAD_L3;
	constexpr uint32_t CacheLineAlignMask = (RANDOMX_DATASET_SIZE - 1) & ~(CacheLineSize - 1);
	constexpr uint32_t CacheSize = RANDOMX_ARGON_MEMORY * 1024;

#ifdef TRACE
	constexpr bool trace = true;
#else
	constexpr bool trace = false;
#endif

#ifndef UNREACHABLE
#ifdef __GNUC__
#define UNREACHABLE __builtin_unreachable()
#elif _MSC_VER
#define UNREACHABLE __assume(false)
#else
#define UNREACHABLE
#endif
#endif

	using int_reg_t = uint64_t;

	struct fpu_reg_t {
		double lo;
		double hi;
	};

	constexpr uint32_t ScratchpadL1 = RANDOMX_SCRATCHPAD_L1 / sizeof(int_reg_t);
	constexpr uint32_t ScratchpadL2 = RANDOMX_SCRATCHPAD_L2 / sizeof(int_reg_t);
	constexpr uint32_t ScratchpadL3 = RANDOMX_SCRATCHPAD_L3 / sizeof(int_reg_t);
	constexpr int ScratchpadL1Mask = (ScratchpadL1 - 1) * 8;
	constexpr int ScratchpadL2Mask = (ScratchpadL2 - 1) * 8;
	constexpr int ScratchpadL1Mask16 = (ScratchpadL1 / 2 - 1) * 16;
	constexpr int ScratchpadL2Mask16 = (ScratchpadL2 / 2 - 1) * 16;
	constexpr int ScratchpadL3Mask = (ScratchpadL3 - 1) * 8;
	constexpr int ScratchpadL3Mask64 = (ScratchpadL3 / 8 - 1) * 64;
	constexpr int RegistersCount = 8;
	constexpr int RegisterNeedsDisplacement = 5; //x86 r13 register
	constexpr int RegisterNeedsSib = 4; //x86 r12 register

	struct MemoryRegisters {
		addr_t mx, ma;
		uint8_t* memory = nullptr;
	};

	struct RegisterFile {
		int_reg_t r[RegistersCount];
		fpu_reg_t f[RegistersCount / 2];
		fpu_reg_t e[RegistersCount / 2];
		fpu_reg_t a[RegistersCount / 2];
	};

	typedef void(*DatasetReadFunc)(addr_t, MemoryRegisters&, int_reg_t(&reg)[RegistersCount]);
	typedef void(*ProgramFunc)(RegisterFile&, MemoryRegisters&, uint8_t* /* scratchpad */, uint64_t);
	typedef void(*DatasetInitFunc)(randomx_cache* cache, uint8_t* dataset, uint32_t startBlock, uint32_t endBlock);
}

std::ostream& operator<<(std::ostream& os, const randomx::RegisterFile& rf);
