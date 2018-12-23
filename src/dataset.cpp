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

#include <new>
#include <algorithm>
#include <stdexcept>
#include <cstring>

#include "common.hpp"
#include "dataset.hpp"
#include "Pcg32.hpp"
#include "Cache.hpp"

#if defined(__SSE2__)
#include <wmmintrin.h>
#define PREFETCH(memory) _mm_prefetch((const char *)((memory).ds.dataset + (memory).ma), _MM_HINT_T0)
#else
#define PREFETCH(memory)
#endif

namespace RandomX {

	template<typename T>
	static inline void shuffle(T* buffer, size_t bytes, Pcg32& gen) {
		auto count = bytes / sizeof(T);
		for (auto i = count - 1; i >= 1; --i) {
			int j = gen.getUniform(0, i);
			std::swap(buffer[j], buffer[i]);
		}
	}

	template<bool soft>
	static inline __m128i aesenc(__m128i in, __m128i key) {
		return soft ? soft_aesenc(in, key) : _mm_aesenc_si128(in, key);
	}

	template<bool soft>
	static inline __m128i aesdec(__m128i in, __m128i key) {
		return soft ? soft_aesdec(in, key) : _mm_aesdec_si128(in, key);
	}

	template<bool soft, bool enc>
	void initBlock(const uint8_t* in, uint8_t* out, uint32_t blockNumber, const KeysContainer& keys) {
		__m128i xin, xout;
		//Initialization vector = block number extended to 128 bits
		xout = _mm_cvtsi32_si128(blockNumber);
		//Expand + AES
		for (uint32_t i = 0; i < DatasetBlockSize / sizeof(__m128i); ++i) {
			if ((i % 32) == 0) {
				xin = _mm_set_epi64x(*(uint64_t*)(in + i / 4), 0);
				xout = _mm_xor_si128(xin, xout);
			}
			if (enc) {
				xout = aesenc<soft>(xout, keys[0]);
				xout = aesenc<soft>(xout, keys[1]);
				xout = aesenc<soft>(xout, keys[2]);
				xout = aesenc<soft>(xout, keys[3]);
				xout = aesenc<soft>(xout, keys[4]);
				xout = aesenc<soft>(xout, keys[5]);
				xout = aesenc<soft>(xout, keys[6]);
				xout = aesenc<soft>(xout, keys[7]);
				xout = aesenc<soft>(xout, keys[8]);
				xout = aesenc<soft>(xout, keys[9]);
			}
			else {
				xout = aesdec<soft>(xout, keys[0]);
				xout = aesdec<soft>(xout, keys[1]);
				xout = aesdec<soft>(xout, keys[2]);
				xout = aesdec<soft>(xout, keys[3]);
				xout = aesdec<soft>(xout, keys[4]);
				xout = aesdec<soft>(xout, keys[5]);
				xout = aesdec<soft>(xout, keys[6]);
				xout = aesdec<soft>(xout, keys[7]);
				xout = aesdec<soft>(xout, keys[8]);
				xout = aesdec<soft>(xout, keys[9]);
			}
			_mm_store_si128((__m128i*)(out + i * sizeof(__m128i)), xout);
		}
		//Shuffle
		Pcg32 gen(&xout);
		shuffle<uint32_t>((uint32_t*)out, DatasetBlockSize, gen);
	}

	template
		void initBlock<true, true>(const uint8_t*, uint8_t*, uint32_t, const KeysContainer&);

	template
		void initBlock<true, false>(const uint8_t*, uint8_t*, uint32_t, const KeysContainer&);

	template
		void initBlock<false, true>(const uint8_t*, uint8_t*, uint32_t, const KeysContainer&);

	template
		void initBlock<false, false>(const uint8_t*, uint8_t*, uint32_t, const KeysContainer&);

	convertible_t datasetRead(addr_t addr, MemoryRegisters& memory) {
		convertible_t data;
		data.u64 = *(uint64_t*)(memory.ds.dataset + memory.ma);
		memory.ma += 8;
		memory.mx ^= addr;
		if ((memory.mx & 0xFFF8) == 0) {
			memory.ma = memory.mx & ~7;
			PREFETCH(memory);
		}
		return data;
	}

	template<bool softAes>
	void initBlock(const uint8_t* cache, uint8_t* block, uint32_t blockNumber, const KeysContainer& keys) {
		if (blockNumber % 2 == 1) {
			initBlock<softAes, true>(cache + blockNumber * CacheBlockSize, block, blockNumber, keys);
		}
		else {
			initBlock<softAes, false>(cache + blockNumber * CacheBlockSize, block, blockNumber, keys);
		}
	}

	template
		void initBlock<true>(const uint8_t*, uint8_t*, uint32_t, const KeysContainer&);

	template
		void initBlock<false>(const uint8_t*, uint8_t*, uint32_t, const KeysContainer&);

	template<bool softAes>
	convertible_t datasetReadLight(addr_t addr, MemoryRegisters& memory) {
		convertible_t data;
		LightClientDataset* lds = memory.ds.lightDataset;
		auto blockNumber = memory.ma / DatasetBlockSize;
		if (lds->blockNumber != blockNumber) {
			initBlock<softAes>(lds->cache->getCache(), (uint8_t*)lds->block, blockNumber, lds->cache->getKeys());
			lds->blockNumber = blockNumber;
		}
		data.u64 = *(uint64_t*)(lds->block + (memory.ma % DatasetBlockSize));
		memory.ma += 8;
		memory.mx ^= addr;
		if ((memory.mx & 0xFFF8) == 0) {
			memory.ma = memory.mx & ~7;
		}
		return data;
	}

	template
		convertible_t datasetReadLight<false>(addr_t addr, MemoryRegisters& memory);

	template
		convertible_t datasetReadLight<true>(addr_t addr, MemoryRegisters& memory);

	void datasetAlloc(dataset_t& ds) {
		if (sizeof(size_t) <= 4)
			throw std::runtime_error("Platform doesn't support enough memory for the dataset");
		ds.dataset = (uint8_t*)_mm_malloc(DatasetSize, /*sizeof(__m128i)*/ 64);
		if (ds.dataset == nullptr) {
			throw std::runtime_error("Dataset memory allocation failed. >4 GiB of free virtual memory is needed.");
		}
	}

	template<bool softAes>
	void datasetInit(Cache* cache, dataset_t ds, uint32_t startBlock, uint32_t blockCount) {
		for (uint32_t i = startBlock; i < startBlock + blockCount; ++i) {
			initBlock<softAes>(cache->getCache(), ds.dataset + i * DatasetBlockSize, i, cache->getKeys());
		}
	}

	template
		void datasetInit<false>(Cache*, dataset_t, uint32_t, uint32_t);

	template
		void datasetInit<true>(Cache*, dataset_t, uint32_t, uint32_t);

	template<bool softAes>
	void datasetInitCache(const void* seed, dataset_t& ds) {
		ds.cache = new Cache();
		ds.cache->initialize<softAes>(seed, SeedSize);
	}

	template
		void datasetInitCache<false>(const void*, dataset_t&);

	template
		void datasetInitCache<true>(const void*, dataset_t&);
}
