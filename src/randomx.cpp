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
		randomx_cache *cache;
		switch (flags & (RANDOMX_FLAG_JIT | RANDOMX_FLAG_LARGE_PAGES))
		{
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

		try {
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
		randomx_dataset *dataset;
		if (flags & RANDOMX_FLAG_LARGE_PAGES) {
			dataset = new randomx::DatasetLargePage();
		}
		else {
			dataset = new randomx::DatasetDefault();
		}

		try {
			dataset->allocate();
		}
		catch (std::exception &ex) {
			delete dataset;
			dataset = nullptr;
		}

		return dataset;
	}

	void randomx_init_dataset(randomx_dataset *dataset, randomx_cache *cache, unsigned long startBlock, unsigned long blockCount) {
		randomx::DatasetInitFunc dsfunc = cache->getInitFunc();
		dsfunc(cache, dataset->memory + startBlock * randomx::CacheLineSize, startBlock, startBlock + blockCount);
	}

	void randomx_release_dataset(randomx_dataset *dataset) {
		delete dataset;
	}

	randomx_vm *randomx_create_vm(randomx_flags flags) {
		randomx_vm *vm;
		switch (flags & (RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_JIT | RANDOMX_FLAG_HARD_AES | RANDOMX_FLAG_LARGE_PAGES)) {
		case RANDOMX_FLAG_DEFAULT: //0
			vm = new randomx::InterpretedLightVmDefault();
			break;

		case RANDOMX_FLAG_FULL_MEM: //1
			vm = new randomx::InterpretedVmDefault();
			break;

		case RANDOMX_FLAG_JIT: //2
			vm = new randomx::CompiledLightVmDefault();
			break;

		case RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_JIT: //3
			vm = new randomx::CompiledVmDefault();
			break;

		case RANDOMX_FLAG_HARD_AES: //4
			vm = new randomx::InterpretedLightVmHardAes();
			break;

		case RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_HARD_AES: //5
			vm = new randomx::InterpretedVmHardAes();
			break;

		case RANDOMX_FLAG_JIT | RANDOMX_FLAG_HARD_AES: //6
			vm = new randomx::CompiledLightVmHardAes();
			break;

		case RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_JIT | RANDOMX_FLAG_HARD_AES: //7
			vm = new randomx::CompiledVmHardAes();
			break;

		case RANDOMX_FLAG_LARGE_PAGES: //8
			vm = new randomx::InterpretedLightVmLargePage();
			break;

		case RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_LARGE_PAGES: //9
			vm = new randomx::InterpretedVmLargePage();
			break;

		case RANDOMX_FLAG_JIT | RANDOMX_FLAG_LARGE_PAGES: //10
			vm = new randomx::CompiledLightVmLargePage();
			break;

		case RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_JIT | RANDOMX_FLAG_LARGE_PAGES: //11
			vm = new randomx::CompiledVmLargePage();
			break;

		case RANDOMX_FLAG_HARD_AES | RANDOMX_FLAG_LARGE_PAGES: //12
			vm = new randomx::InterpretedLightVmLargePageHardAes();
			break;

		case RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_HARD_AES | RANDOMX_FLAG_LARGE_PAGES: //13
			vm = new randomx::InterpretedVmLargePageHardAes();
			break;

		case RANDOMX_FLAG_JIT | RANDOMX_FLAG_HARD_AES | RANDOMX_FLAG_LARGE_PAGES: //14
			vm = new randomx::CompiledLightVmLargePageHardAes();
			break;

		case RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_JIT | RANDOMX_FLAG_HARD_AES | RANDOMX_FLAG_LARGE_PAGES: //15
			vm = new randomx::CompiledVmLargePageHardAes();
			break;

		default:
			UNREACHABLE;
		}

		if (!vm->allocate()) {
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
		alignas(16) uint64_t hash[8];
		blake2b(hash, sizeof(hash), input, inputSize, nullptr, 0);
		machine->initScratchpad(&hash);
		//dump((char*)scratchpad, RANDOMX_SCRATCHPAD_L3, "spad-before.txt");
		machine->resetRoundingMode();
		for (int chain = 0; chain < RANDOMX_PROGRAM_COUNT - 1; ++chain) {
			machine->run(&hash);
			blake2b(hash, sizeof(hash), machine->getRegisterFile(), sizeof(randomx::RegisterFile), nullptr, 0);
		}
		machine->run(&hash);
		machine->getFinalResult(output, RANDOMX_HASH_SIZE);
	}

}
