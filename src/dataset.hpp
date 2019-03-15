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
#include "intrinPortable.h"
#include "common.hpp"

namespace RandomX {

#if defined(_M_X64)
	extern "C"
#endif
	void initBlock(const Cache& cache, uint8_t* out, uint64_t blockNumber, unsigned iterations);

	void datasetAlloc(dataset_t& ds, bool largePages);

	void datasetInit(Cache& cache, Dataset& ds, uint32_t startBlock, uint32_t blockCount);

	void datasetRead(addr_t addr, MemoryRegisters& memory, RegisterFile&);

	void datasetInitCache(const void* seed, dataset_t& dataset, bool largePages);

	void datasetReadLight(addr_t addr, MemoryRegisters& memory, int_reg_t(&reg)[RegistersCount]);

	void datasetReadLightAsync(addr_t addr, MemoryRegisters& memory, int_reg_t(&reg)[RegistersCount]);
}

