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

#include "VirtualMachine.hpp"
#include "common.hpp"
#include "dataset.hpp"
#include "Cache.hpp"
#include "t1ha/t1ha.h"
#include "blake2/blake2.h"
#include <cstring>

namespace RandomX {
	VirtualMachine::VirtualMachine(bool softAes) : softAes(softAes), lightClient(false) {
		mem.ds.dataset = nullptr;
	}

	VirtualMachine::~VirtualMachine() {
		if (lightClient) {
			delete mem.ds.lightDataset->block;
			delete mem.ds.lightDataset;
		}
	}

	void VirtualMachine::setDataset(dataset_t ds, bool light) {
		if (mem.ds.dataset != nullptr) {
			throw std::runtime_error("Dataset is already initialized");
		}
		lightClient = light;
		if (light) {
			auto lds = mem.ds.lightDataset = new LightClientDataset();
			lds->cache = ds.cache;
			lds->block = (uint8_t*)_mm_malloc(DatasetBlockSize, sizeof(__m128i));
			lds->blockNumber = -1;
			if (lds->block == nullptr) {
				throw std::bad_alloc();
			}
			if (softAes) {
				readDataset = &datasetReadLight<true>;
			}
			else {
				readDataset = &datasetReadLight<false>;
			}
		}
		else {
			mem.ds = ds;
			readDataset = &datasetRead;
		}
	}

	void VirtualMachine::initializeScratchpad(uint32_t index) {
		if (lightClient) {
			auto cache = mem.ds.lightDataset->cache;
			if (softAes) {
				for (int i = 0; i < ScratchpadSize / DatasetBlockSize; ++i) {
					initBlock<true>(cache->getCache(), ((uint8_t*)scratchpad) + DatasetBlockSize * i, (ScratchpadSize / DatasetBlockSize) * index + i, cache->getKeys());
				}
			}
			else {
				for (int i = 0; i < ScratchpadSize / DatasetBlockSize; ++i) {
					initBlock<false>(cache->getCache(), ((uint8_t*)scratchpad) + DatasetBlockSize * i, (ScratchpadSize / DatasetBlockSize) * index + i, cache->getKeys());
				}
			}
		}
		else {
			memcpy(scratchpad, mem.ds.dataset + ScratchpadSize * index, ScratchpadSize);
		}
	}

	void VirtualMachine::getResult(void* out) {
		uint64_t smallState[sizeof(RegisterFile) / sizeof(uint64_t) + 2];
		memcpy(smallState, &reg, sizeof(RegisterFile));
		smallState[17] = t1ha2_atonce128(&smallState[16], scratchpad, ScratchpadSize, reg.r[0].u64);
		blake2b(out, ResultSize, smallState, sizeof(smallState), nullptr, 0);
	}
}