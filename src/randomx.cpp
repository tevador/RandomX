/*
Copyright (c) 2018-2019, tevador <tevador@gmail.com>

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	* Neither the name of the copyright holder nor the
	  names of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "randomx.h"
#include "dataset.hpp"
#include "vm_interpreted.hpp"
#include "vm_interpreted_light.hpp"
#include "vm_compiled.hpp"
#include "vm_compiled_light.hpp"
#include "blake2/blake2.h"
#include "cpu.hpp"
#include <cassert>
#include <limits>

#if defined(__SSE__) || defined(__SSE2__) || (defined(_M_IX86_FP) && (_M_IX86_FP > 0))
#define USE_CSR_INTRINSICS
#include <xmmintrin.h>
#else
#include <cfenv>
#endif

extern "C" {

	randomx_flags randomx_get_flags() {
		randomx_flags flags = RANDOMX_HAVE_COMPILER ? RANDOMX_FLAG_JIT : RANDOMX_FLAG_DEFAULT;
		randomx::Cpu cpu;
#ifdef RANDOMX_FORCE_SECURE
		if (flags == RANDOMX_FLAG_JIT) {
			flags |= RANDOMX_FLAG_SECURE;
		}
#endif
		if (HAVE_AES && cpu.hasAes()) {
			flags |= RANDOMX_FLAG_HARD_AES;
		}
		if (randomx_argon2_impl_avx2() != nullptr && cpu.hasAvx2()) {
			flags |= RANDOMX_FLAG_ARGON2_AVX2;
		}
		if (randomx_argon2_impl_ssse3() != nullptr && cpu.hasSsse3()) {
			flags |= RANDOMX_FLAG_ARGON2_SSSE3;
		}
		return flags;
	}

	randomx_cache *randomx_alloc_cache(randomx_flags flags) {
		randomx_cache *cache = nullptr;
		auto impl = randomx::selectArgonImpl(flags);
		if (impl == nullptr) {
			return cache;
		}

		try {
			cache = new randomx_cache();
			cache->argonImpl = impl;
			switch ((int)(flags & (RANDOMX_FLAG_JIT | RANDOMX_FLAG_LARGE_PAGES))) {
				case RANDOMX_FLAG_DEFAULT:
					cache->dealloc = &randomx::deallocCache<randomx::DefaultAllocator>;
					cache->jit = nullptr;
					cache->initialize = &randomx::initCache;
					cache->datasetInit = &randomx::initDataset;
					cache->memory = (uint8_t*)randomx::DefaultAllocator::allocMemory(randomx::CacheSize);
					break;

				case RANDOMX_FLAG_JIT:
					cache->dealloc = &randomx::deallocCache<randomx::DefaultAllocator>;
					cache->jit = new randomx::JitCompiler();
					cache->initialize = &randomx::initCacheCompile;
					cache->datasetInit = cache->jit->getDatasetInitFunc();
					cache->memory = (uint8_t*)randomx::DefaultAllocator::allocMemory(randomx::CacheSize);
					break;

				case RANDOMX_FLAG_LARGE_PAGES:
					cache->dealloc = &randomx::deallocCache<randomx::LargePageAllocator>;
					cache->jit = nullptr;
					cache->initialize = &randomx::initCache;
					cache->datasetInit = &randomx::initDataset;
					cache->memory = (uint8_t*)randomx::LargePageAllocator::allocMemory(randomx::CacheSize);
					break;

				case RANDOMX_FLAG_JIT | RANDOMX_FLAG_LARGE_PAGES:
					cache->dealloc = &randomx::deallocCache<randomx::LargePageAllocator>;
					cache->jit = new randomx::JitCompiler();
					cache->initialize = &randomx::initCacheCompile;
					cache->datasetInit = cache->jit->getDatasetInitFunc();
					cache->memory = (uint8_t*)randomx::LargePageAllocator::allocMemory(randomx::CacheSize);
					break;

				default:
					UNREACHABLE;
			}
		}
		catch (std::exception &ex) {
			if (cache != nullptr) {
				randomx_release_cache(cache);
				cache = nullptr;
			}
		}
		if (cache && cache->memory == nullptr) {
			randomx_release_cache(cache);
			cache = nullptr;
		}

		return cache;
	}

	void randomx_init_cache(randomx_cache* cache, const void* key, size_t keyLength) {
		assert(cache != nullptr);
		assert(keyLength == 0 || key != nullptr);

		std::string newCacheKey(static_cast<const char*>(key), keyLength);

		if (cache->cacheKey != newCacheKey || !cache->isInitialized()) {
			cache->initialize(cache, key, keyLength);
			cache->cacheKey = std::move(newCacheKey);
		}
	}

	void randomx_release_cache(randomx_cache* cache) {
		assert(cache != nullptr);
		cache->dealloc(cache);
		delete cache;
	}

	randomx_dataset *randomx_alloc_dataset(randomx_flags flags) {

		//fail on 32-bit systems if DatasetSize is >= 4 GiB
		if (randomx::DatasetSize > std::numeric_limits<size_t>::max()) {
			return nullptr;
		}

		randomx_dataset *dataset = nullptr;

		try {
			dataset = new randomx_dataset();
			if (flags & RANDOMX_FLAG_LARGE_PAGES) {
				dataset->dealloc = &randomx::deallocDataset<randomx::LargePageAllocator>;
				dataset->memory = (uint8_t*)randomx::LargePageAllocator::allocMemory(randomx::DatasetSize);
			}
			else {
				dataset->dealloc = &randomx::deallocDataset<randomx::DefaultAllocator>;
				dataset->memory = (uint8_t*)randomx::DefaultAllocator::allocMemory(randomx::DatasetSize);
			}
		}
		catch (std::exception &ex) {
			if (dataset != nullptr) {
				randomx_release_dataset(dataset);
				dataset = nullptr;
			}
		}
		if (dataset && dataset->memory == nullptr) {
			randomx_release_dataset(dataset);
			dataset = nullptr;
		}

		return dataset;
	}

	constexpr unsigned long DatasetItemCount = randomx::DatasetSize / RANDOMX_DATASET_ITEM_SIZE;

	unsigned long randomx_dataset_item_count() {
		return DatasetItemCount;
	}

	void randomx_init_dataset(randomx_dataset *dataset, randomx_cache *cache, unsigned long startItem, unsigned long itemCount) {
		assert(dataset != nullptr);
		assert(cache != nullptr);
		assert(startItem < DatasetItemCount && itemCount <= DatasetItemCount);
		assert(startItem + itemCount <= DatasetItemCount);
		cache->datasetInit(cache, dataset->memory + startItem * randomx::CacheLineSize, startItem, startItem + itemCount);
	}

	void *randomx_get_dataset_memory(randomx_dataset *dataset) {
		assert(dataset != nullptr);
		return dataset->memory;
	}

	void randomx_release_dataset(randomx_dataset *dataset) {
		assert(dataset != nullptr);
		dataset->dealloc(dataset);
		delete dataset;
	}

	randomx_vm *randomx_create_vm(randomx_flags flags, randomx_cache *cache, randomx_dataset *dataset) {
		assert(cache != nullptr || (flags & RANDOMX_FLAG_FULL_MEM));
		assert(cache == nullptr || cache->isInitialized());
		assert(dataset != nullptr || !(flags & RANDOMX_FLAG_FULL_MEM));

		randomx_vm *vm = nullptr;

		try {
			switch ((int)(flags & (RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_JIT | RANDOMX_FLAG_HARD_AES | RANDOMX_FLAG_LARGE_PAGES))) {
				case RANDOMX_FLAG_DEFAULT:
					vm = new randomx::InterpretedLightVmDefault();
					break;

				case RANDOMX_FLAG_FULL_MEM:
					vm = new randomx::InterpretedVmDefault();
					break;

				case RANDOMX_FLAG_JIT:
					if (flags & RANDOMX_FLAG_SECURE) {
						vm = new randomx::CompiledLightVmDefaultSecure();
					}
					else {
						vm = new randomx::CompiledLightVmDefault();
					}
					break;

				case RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_JIT:
					if (flags & RANDOMX_FLAG_SECURE) {
						vm = new randomx::CompiledVmDefaultSecure();
					}
					else {
						vm = new randomx::CompiledVmDefault();
					}
					break;

				case RANDOMX_FLAG_HARD_AES:
					vm = new randomx::InterpretedLightVmHardAes();
					break;

				case RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_HARD_AES:
					vm = new randomx::InterpretedVmHardAes();
					break;

				case RANDOMX_FLAG_JIT | RANDOMX_FLAG_HARD_AES:
					if (flags & RANDOMX_FLAG_SECURE) {
						vm = new randomx::CompiledLightVmHardAesSecure();
					}
					else {
						vm = new randomx::CompiledLightVmHardAes();
					}
					break;

				case RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_JIT | RANDOMX_FLAG_HARD_AES:
					if (flags & RANDOMX_FLAG_SECURE) {
						vm = new randomx::CompiledVmHardAesSecure();
					}
					else {
						vm = new randomx::CompiledVmHardAes();
					}
					break;

				case RANDOMX_FLAG_LARGE_PAGES:
					vm = new randomx::InterpretedLightVmLargePage();
					break;

				case RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_LARGE_PAGES:
					vm = new randomx::InterpretedVmLargePage();
					break;

				case RANDOMX_FLAG_JIT | RANDOMX_FLAG_LARGE_PAGES:
					if (flags & RANDOMX_FLAG_SECURE) {
						vm = new randomx::CompiledLightVmLargePageSecure();
					}
					else {
						vm = new randomx::CompiledLightVmLargePage();
					}
					break;

				case RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_JIT | RANDOMX_FLAG_LARGE_PAGES:
					if (flags & RANDOMX_FLAG_SECURE) {
						vm = new randomx::CompiledVmLargePageSecure();
					}
					else {
						vm = new randomx::CompiledVmLargePage();
					}
					break;

				case RANDOMX_FLAG_HARD_AES | RANDOMX_FLAG_LARGE_PAGES:
					vm = new randomx::InterpretedLightVmLargePageHardAes();
					break;

				case RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_HARD_AES | RANDOMX_FLAG_LARGE_PAGES:
					vm = new randomx::InterpretedVmLargePageHardAes();
					break;

				case RANDOMX_FLAG_JIT | RANDOMX_FLAG_HARD_AES | RANDOMX_FLAG_LARGE_PAGES:
					if (flags & RANDOMX_FLAG_SECURE) {
						vm = new randomx::CompiledLightVmLargePageHardAesSecure();
					}
					else {
						vm = new randomx::CompiledLightVmLargePageHardAes();
					}
					break;

				case RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_JIT | RANDOMX_FLAG_HARD_AES | RANDOMX_FLAG_LARGE_PAGES:
					if (flags & RANDOMX_FLAG_SECURE) {
						vm = new randomx::CompiledVmLargePageHardAesSecure();
					}
					else {
						vm = new randomx::CompiledVmLargePageHardAes();
					}
					break;

				default:
					UNREACHABLE;
			}

			if(cache != nullptr) {
				vm->setCache(cache);
				vm->cacheKey = cache->cacheKey;
			}

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
		assert(machine != nullptr);
		assert(cache != nullptr && cache->isInitialized());
		if (machine->cacheKey != cache->cacheKey || machine->getMemory() != cache->memory) {
			machine->setCache(cache);
			machine->cacheKey = cache->cacheKey;
		}
	}

	void randomx_vm_set_dataset(randomx_vm *machine, randomx_dataset *dataset) {
		assert(machine != nullptr);
		assert(dataset != nullptr);
		machine->setDataset(dataset);
	}

	void randomx_destroy_vm(randomx_vm *machine) {
		assert(machine != nullptr);
		delete machine;
	}

	void randomx_calculate_hash(randomx_vm *machine, const void *input, size_t inputSize, void *output) {
		assert(machine != nullptr);
		assert(inputSize == 0 || input != nullptr);
		assert(output != nullptr);

#ifdef USE_CSR_INTRINSICS
		const unsigned int fpstate = _mm_getcsr();
#else
		fenv_t fpstate;
		fegetenv(&fpstate);
#endif

		alignas(16) uint64_t tempHash[8];
		int blakeResult = blake2b(tempHash, sizeof(tempHash), input, inputSize, nullptr, 0);
		assert(blakeResult == 0);
		machine->initScratchpad(&tempHash);
		machine->resetRoundingMode();
		for (int chain = 0; chain < RANDOMX_PROGRAM_COUNT - 1; ++chain) {
			machine->run(&tempHash);
			blakeResult = blake2b(tempHash, sizeof(tempHash), machine->getRegisterFile(), sizeof(randomx::RegisterFile), nullptr, 0);
			assert(blakeResult == 0);
		}
		machine->run(&tempHash);
		machine->getFinalResult(output, RANDOMX_HASH_SIZE);

#ifdef USE_CSR_INTRINSICS
		_mm_setcsr(fpstate);
#else
		fesetenv(&fpstate);
#endif
	}

	void randomx_calculate_hash_first(randomx_vm* machine, const void* input, size_t inputSize) {
		blake2b(machine->tempHash, sizeof(machine->tempHash), input, inputSize, nullptr, 0);
		machine->initScratchpad(machine->tempHash);
	}

	void randomx_calculate_hash_next(randomx_vm* machine, const void* nextInput, size_t nextInputSize, void* output) {
		machine->resetRoundingMode();
		for (uint32_t chain = 0; chain < RANDOMX_PROGRAM_COUNT - 1; ++chain) {
			machine->run(machine->tempHash);
			blake2b(machine->tempHash, sizeof(machine->tempHash), machine->getRegisterFile(), sizeof(randomx::RegisterFile), nullptr, 0);
		}
		machine->run(machine->tempHash);

		// Finish current hash and fill the scratchpad for the next hash at the same time
		blake2b(machine->tempHash, sizeof(machine->tempHash), nextInput, nextInputSize, nullptr, 0);
		machine->hashAndFill(output, RANDOMX_HASH_SIZE, machine->tempHash);
	}

	void randomx_calculate_hash_last(randomx_vm* machine, void* output) {
		machine->resetRoundingMode();
		for (int chain = 0; chain < RANDOMX_PROGRAM_COUNT - 1; ++chain) {
			machine->run(machine->tempHash);
			blake2b(machine->tempHash, sizeof(machine->tempHash), machine->getRegisterFile(), sizeof(randomx::RegisterFile), nullptr, 0);
		}
		machine->run(machine->tempHash);
		machine->getFinalResult(output, RANDOMX_HASH_SIZE);
	}

	void randomx_calculate_commitment(const void* input, size_t inputSize, const void* hash_in, void* com_out) {
		assert(inputSize == 0 || input != nullptr);
		assert(hash_in != nullptr);
		assert(com_out != nullptr);
		blake2b_state state;
		blake2b_init(&state, RANDOMX_HASH_SIZE);
		blake2b_update(&state, input, inputSize);
		blake2b_update(&state, hash_in, RANDOMX_HASH_SIZE);
		blake2b_final(&state, com_out, RANDOMX_HASH_SIZE);
	}
}
