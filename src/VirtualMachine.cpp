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
#include <cstring>

namespace RandomX {
	VirtualMachine::VirtualMachine(bool softAes) : softAes(softAes), lightClient(false) {
		mem.dataset = nullptr;
	}

	void VirtualMachine::initializeDataset(const void* seed, bool light) {
		if (lightClient) {
			_mm_free(mem.lcm->cache);
			_mm_free(mem.lcm->block);
		}
		_mm_free(mem.dataset);
		lightClient = light;
		if (light) {
			if (softAes) {
				datasetInitLight<true>(seed, mem.lcm);
				readDataset = &datasetReadLight<true>;
			}
			else {
				datasetInitLight<false>(seed, mem.lcm);
				readDataset = &datasetReadLight<false>;
			}
		}
		else {
			readDataset = &datasetRead;
			if (softAes) {
				datasetInit<true>(seed, mem.dataset);
			}
			else {
				datasetInit<false>(seed, mem.dataset);
			}
		}
	}

	void VirtualMachine::initializeScratchpad(uint32_t index) {
		if (lightClient) {
			if (softAes) {
				for (int i = 0; i < ScratchpadSize / DatasetBlockSize; ++i) {
					initBlock<true>(mem.lcm->cache + CacheShift, ((uint8_t*)scratchpad) + DatasetBlockSize * i, (ScratchpadSize / DatasetBlockSize) * index + i, mem.lcm->keys);
				}
			}
			else {
				for (int i = 0; i < ScratchpadSize / DatasetBlockSize; ++i) {
					initBlock<false>(mem.lcm->cache + CacheShift, ((uint8_t*)scratchpad) + DatasetBlockSize * i, (ScratchpadSize / DatasetBlockSize) * index + i, mem.lcm->keys);
				}
			}
		}
		else {
			memcpy(scratchpad, mem.dataset + ScratchpadSize * index, ScratchpadSize);
		}
	}
}