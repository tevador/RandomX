/*
Copyright (c) 2019 tevador

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

#include "randomx.h"
#include "dataset.hpp"
#include "vm_interpreted.hpp"
#include "vm_interpreted_light.hpp"
#include "vm_compiled.hpp"
#include "vm_compiled_light.hpp"
#include "blake2/blake2.h"

extern "C" {

	randomx_cache *randomx_alloc_cache(randomx_flags flags) {
		randomx_cache *cache = nullptr;

		try {
			switch (flags & (RANDOMX_FLAG_JIT | RANDOMX_FLAG_LARGE_PAGES)) {
				case RANDOMX_FLAG_DEFAULT:
					cache = new randomx::CacheDefault();
					break;

				case RANDOMX_FLAG_JIT:
					cache = new randomx::CacheWithJitDefault();
					break;

				case RANDOMX_FLAG_LARGE_PAGES:
					cache = new randomx::CacheLargePage();
					break;

				case RANDOMX_FLAG_JIT | RANDOMX_FLAG_LARGE_PAGES:
					cache = new randomx::CacheWithJitLargePage();
					break;

				default:
					UNREACHABLE;
			}

			cache->allocate();
		}
		catch (std::exception &ex) {
			delete cache;
			cache = nullptr;
		}

		return cache;
	}

	void randomx_init_cache(randomx_cache *cache, const void *seed, size_t seedSize) {
		cache->initialize(seed, seedSize);
	}

	void randomx_release_cache(randomx_cache* cache) {
		delete cache;
	}

	randomx_dataset *randomx_alloc_dataset(randomx_flags flags) {
		randomx_dataset *dataset = nullptr;

		try {
			if (flags & RANDOMX_FLAG_LARGE_PAGES) {
				dataset = new randomx::DatasetLargePage();
			}
			else {
				dataset = new randomx::DatasetDefault();
			}
			dataset->allocate();
		}
		catch (std::exception &ex) {
			delete dataset;
			dataset = nullptr;
		}

		return dataset;
	}

	unsigned long randomx_dataset_item_count() {
		return RANDOMX_DATASET_SIZE / RANDOMX_DATASET_ITEM_SIZE;
	}

	void randomx_init_dataset(randomx_dataset *dataset, randomx_cache *cache, unsigned long startItem, unsigned long itemCount) {
		randomx::DatasetInitFunc dsfunc = cache->getInitFunc();
		dsfunc(cache, dataset->memory + startItem * randomx::CacheLineSize, startItem, startItem + itemCount);
	}

	void *randomx_get_dataset_memory(randomx_dataset *dataset) {
		return dataset->memory;
	}

	void randomx_release_dataset(randomx_dataset *dataset) {
		delete dataset;
	}

	randomx_vm *randomx_create_vm(randomx_flags flags, randomx_cache *cache, randomx_dataset *dataset) {
		randomx_vm *vm = nullptr;

		try {
			switch (flags & (RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_JIT | RANDOMX_FLAG_HARD_AES | RANDOMX_FLAG_LARGE_PAGES)) {
				case RANDOMX_FLAG_DEFAULT:
					vm = new randomx::InterpretedLightVmDefault();
					break;

				case RANDOMX_FLAG_FULL_MEM:
					vm = new randomx::InterpretedVmDefault();
					break;

				case RANDOMX_FLAG_JIT:
					vm = new randomx::CompiledLightVmDefault();
					break;

				case RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_JIT:
					vm = new randomx::CompiledVmDefault();
					break;

				case RANDOMX_FLAG_HARD_AES:
					vm = new randomx::InterpretedLightVmHardAes();
					break;

				case RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_HARD_AES:
					vm = new randomx::InterpretedVmHardAes();
					break;

				case RANDOMX_FLAG_JIT | RANDOMX_FLAG_HARD_AES:
					vm = new randomx::CompiledLightVmHardAes();
					break;

				case RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_JIT | RANDOMX_FLAG_HARD_AES:
					vm = new randomx::CompiledVmHardAes();
					break;

				case RANDOMX_FLAG_LARGE_PAGES:
					vm = new randomx::InterpretedLightVmLargePage();
					break;

				case RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_LARGE_PAGES:
					vm = new randomx::InterpretedVmLargePage();
					break;

				case RANDOMX_FLAG_JIT | RANDOMX_FLAG_LARGE_PAGES:
					vm = new randomx::CompiledLightVmLargePage();
					break;

				case RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_JIT | RANDOMX_FLAG_LARGE_PAGES:
					vm = new randomx::CompiledVmLargePage();
					break;

				case RANDOMX_FLAG_HARD_AES | RANDOMX_FLAG_LARGE_PAGES:
					vm = new randomx::InterpretedLightVmLargePageHardAes();
					break;

				case RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_HARD_AES | RANDOMX_FLAG_LARGE_PAGES:
					vm = new randomx::InterpretedVmLargePageHardAes();
					break;

				case RANDOMX_FLAG_JIT | RANDOMX_FLAG_HARD_AES | RANDOMX_FLAG_LARGE_PAGES:
					vm = new randomx::CompiledLightVmLargePageHardAes();
					break;

				case RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_JIT | RANDOMX_FLAG_HARD_AES | RANDOMX_FLAG_LARGE_PAGES:
					vm = new randomx::CompiledVmLargePageHardAes();
					break;

				default:
					UNREACHABLE;
			}

			if(cache != nullptr)
				vm->setCache(cache);

			if(dataset != nullptr)
				vm->setDataset(dataset);

			vm->allocate();
		}
		catch (std::exception &ex) {
			delete vm;
			vm = nullptr;
		}

		return vm;
	}

	void randomx_vm_set_cache(randomx_vm *machine, randomx_cache* cache) {
		machine->setCache(cache);
	}

	void randomx_vm_set_dataset(randomx_vm *machine, randomx_dataset *dataset) {
		machine->setDataset(dataset);
	}

	void randomx_destroy_vm(randomx_vm *machine) {
		delete machine;
	}

	void randomx_calculate_hash(randomx_vm *machine, const void *input, size_t inputSize, void *output) {
		alignas(16) uint64_t tempHash[8];
		blake2b(tempHash, sizeof(tempHash), input, inputSize, nullptr, 0);
		machine->initScratchpad(&tempHash);
		machine->resetRoundingMode();
		for (int chain = 0; chain < RANDOMX_PROGRAM_COUNT - 1; ++chain) {
			machine->run(&tempHash);
			blake2b(tempHash, sizeof(tempHash), machine->getRegisterFile(), sizeof(randomx::RegisterFile), nullptr, 0);
		}
		machine->run(&tempHash);
		machine->getFinalResult(output, RANDOMX_HASH_SIZE);
	}

}
