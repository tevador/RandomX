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

namespace RandomX {

	using addr_t = uint32_t;

	constexpr int SeedSize = 32;
	constexpr int ResultSize = 64;

	constexpr int ArgonIterations = 3;
	constexpr uint32_t ArgonMemorySize = 262144; //KiB
	constexpr int ArgonLanes = 1;
	const char ArgonSalt[] = "Monero\x1A$";
	constexpr int ArgonSaltSize = sizeof(ArgonSalt) - 1;

	constexpr int CacheLineSize = 64;

	constexpr uint64_t DatasetSize = 4ULL * 1024 * 1024 * 1024; //4 GiB
	constexpr uint32_t CacheLineAlignMask = (DatasetSize - 1) & ~(CacheLineSize - 1);
	constexpr uint32_t CacheSize = ArgonMemorySize * 1024;
	constexpr int CacheBlockCount = CacheSize / CacheLineSize;
	constexpr int DatasetExpansionRatio = DatasetSize / CacheSize;
	constexpr int DatasetBlockCount = DatasetExpansionRatio * CacheBlockCount;
	constexpr int DatasetIterations = DatasetExpansionRatio;


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

	constexpr int ProgramLength = 256;
	constexpr uint32_t InstructionCount = 2048;
	constexpr int ChainLength = 8;
	constexpr uint32_t ScratchpadSize = 2 * 1024 * 1024;
	constexpr uint32_t ScratchpadLength = ScratchpadSize / sizeof(int_reg_t);
	constexpr uint32_t ScratchpadL1 = ScratchpadSize / 128 / sizeof(int_reg_t);
	constexpr uint32_t ScratchpadL2 = ScratchpadSize / 8 / sizeof(int_reg_t);
	constexpr uint32_t ScratchpadL3 = ScratchpadSize / sizeof(int_reg_t);
	constexpr int ScratchpadL1Mask = (ScratchpadL1 - 1) * 8;
	constexpr int ScratchpadL2Mask = (ScratchpadL2 - 1) * 8;
	constexpr int ScratchpadL1Mask16 = (ScratchpadL1 / 2 - 1) * 16;
	constexpr int ScratchpadL2Mask16 = (ScratchpadL2 / 2 - 1) * 16;
	constexpr int ScratchpadL3Mask = (ScratchpadLength - 1) * 8;
	constexpr int ScratchpadL3Mask64 = (ScratchpadLength / 8 - 1) * 64;
	constexpr uint32_t TransformationCount = 90;
	constexpr int RegistersCount = 8;

	class Cache;

	inline int wrapInstr(int i) {
		return i % RandomX::ProgramLength;
	}

	class ILightClientAsyncWorker {
	public:
		virtual ~ILightClientAsyncWorker() {}
		virtual void prepareBlock(addr_t) = 0;
		virtual void prepareBlocks(void* out, uint32_t startBlock, uint32_t blockCount) = 0;
		virtual const uint64_t* getBlock(addr_t) = 0;
		virtual void getBlocks(void* out, uint32_t startBlock, uint32_t blockCount) = 0;
		virtual void sync() = 0;
		const Cache* getCache() {
			return cache;
		}
	protected:
		ILightClientAsyncWorker(const Cache* c) : cache(c) {}
		const Cache* cache;
	};

	union dataset_t {
		uint8_t* dataset;
		Cache* cache;
		ILightClientAsyncWorker* asyncWorker;
	};

	struct MemoryRegisters {
		addr_t mx, ma;
		dataset_t ds;
	};

	static_assert(sizeof(MemoryRegisters) == 2 * sizeof(addr_t) + sizeof(uintptr_t), "Invalid alignment of struct RandomX::MemoryRegisters");

	struct RegisterFile {
		int_reg_t r[RegistersCount];
		fpu_reg_t f[RegistersCount / 2];
		fpu_reg_t e[RegistersCount / 2];
		fpu_reg_t a[RegistersCount / 2];
	};

	static_assert(sizeof(RegisterFile) == 256, "Invalid alignment of struct RandomX::RegisterFile");

	typedef void(*DatasetReadFunc)(addr_t, MemoryRegisters&, int_reg_t(&reg)[RegistersCount]);

	typedef void(*ProgramFunc)(RegisterFile&, MemoryRegisters&, uint8_t* /* scratchpad */, uint64_t);

	extern "C" {
		void executeProgram(RegisterFile&, MemoryRegisters&, uint8_t* /* scratchpad */, uint64_t);
	}
}

std::ostream& operator<<(std::ostream& os, const RandomX::RegisterFile& rf);
