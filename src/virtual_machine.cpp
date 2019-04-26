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

#include <cstring>
#include <iomanip>
#include <stdexcept>
#include "virtual_machine.hpp"
#include "common.hpp"
#include "aes_hash.hpp"
#include "blake2/blake2.h"
#include "intrin_portable.h"
#include "allocator.hpp"

randomx_vm::~randomx_vm() {

}

void randomx_vm::resetRoundingMode() {
	initFpu();
}

constexpr int mantissaSize = 52;
constexpr int exponentSize = 11;
constexpr uint64_t mantissaMask = (1ULL << mantissaSize) - 1;
constexpr uint64_t exponentMask = (1ULL << exponentSize) - 1;
constexpr int exponentBias = 1023;

static inline uint64_t getSmallPositiveFloatBits(uint64_t entropy) {
	auto exponent = entropy >> 59; //0..31
	auto mantissa = entropy & mantissaMask;
	exponent += exponentBias;
	exponent &= exponentMask;
	exponent <<= mantissaSize;
	return exponent | mantissa;
}

void randomx_vm::initialize() {
	store64(&reg.a[0].lo, getSmallPositiveFloatBits(program.getEntropy(0)));
	store64(&reg.a[0].hi, getSmallPositiveFloatBits(program.getEntropy(1)));
	store64(&reg.a[1].lo, getSmallPositiveFloatBits(program.getEntropy(2)));
	store64(&reg.a[1].hi, getSmallPositiveFloatBits(program.getEntropy(3)));
	store64(&reg.a[2].lo, getSmallPositiveFloatBits(program.getEntropy(4)));
	store64(&reg.a[2].hi, getSmallPositiveFloatBits(program.getEntropy(5)));
	store64(&reg.a[3].lo, getSmallPositiveFloatBits(program.getEntropy(6)));
	store64(&reg.a[3].hi, getSmallPositiveFloatBits(program.getEntropy(7)));
	mem.ma = program.getEntropy(8) & randomx::CacheLineAlignMask;
	mem.mx = program.getEntropy(10);
	auto addressRegisters = program.getEntropy(12);
	config.readReg0 = 0 + (addressRegisters & 1);
	addressRegisters >>= 1;
	config.readReg1 = 2 + (addressRegisters & 1);
	addressRegisters >>= 1;
	config.readReg2 = 4 + (addressRegisters & 1);
	addressRegisters >>= 1;
	config.readReg3 = 6 + (addressRegisters & 1);
	datasetOffset = (program.getEntropy(13) & randomx::DatasetExtraItems) * randomx::CacheLineSize;
	constexpr uint64_t mask22bit = (1ULL << 22) - 1;
	constexpr uint64_t maskExp240 = ieee_get_exponent_mask<-240>();
	store64(&config.eMask[0], (program.getEntropy(14) & mask22bit) | maskExp240);
	store64(&config.eMask[1], (program.getEntropy(15) & mask22bit) | maskExp240);
}

//TODO
std::ostream& operator<<(std::ostream& os, const randomx::RegisterFile& rf) {
	for (int i = 0; i < randomx::RegistersCount; ++i)
		os << std::hex << "r" << i << " = " << rf.r[i] << std::endl << std::dec;
	for (int i = 0; i < 4; ++i)
		os << std::hex << "f" << i << " = " << *(uint64_t*)&rf.f[i].hi << " (" << rf.f[i].hi << ")" << std::endl
		<< "   = " << *(uint64_t*)&rf.f[i].lo << " (" << rf.f[i].lo << ")" << std::endl << std::dec;
	for (int i = 0; i < 4; ++i)
		os << std::hex << "e" << i << " = " << *(uint64_t*)&rf.e[i].hi << " (" << rf.e[i].hi << ")" << std::endl
		<< "   = " << *(uint64_t*)&rf.e[i].lo << " (" << rf.e[i].lo << ")" << std::endl << std::dec;
	for (int i = 0; i < 4; ++i)
		os << std::hex << "a" << i << " = " << *(uint64_t*)&rf.a[i].hi << " (" << rf.a[i].hi << ")" << std::endl
		<< "   = " << *(uint64_t*)&rf.a[i].lo << " (" << rf.a[i].lo << ")" << std::endl << std::dec;
	return os;
}

namespace randomx {

	alignas(16) volatile static __m128i aesDummy;

	template<class Allocator, bool softAes>
	VmBase<Allocator, softAes>::~VmBase() {
		Allocator::freeMemory(scratchpad, ScratchpadSize);
	}

	template<class Allocator, bool softAes>
	void VmBase<Allocator, softAes>::allocate() {
		if (mem.memory == nullptr)
			throw std::invalid_argument("Cache/Dataset not set");
		if (!softAes) { //if hardware AES is not supported, it's better to fail now than to return a ticking bomb
			__m128i tmp = _mm_load_si128((const __m128i*)&aesDummy);
			tmp = _mm_aesenc_si128(tmp, tmp);
			_mm_store_si128((__m128i*)&aesDummy, tmp);
		}
		scratchpad = (uint8_t*)Allocator::allocMemory(ScratchpadSize);
	}

	template<class Allocator, bool softAes>
	void VmBase<Allocator, softAes>::getFinalResult(void* out, size_t outSize) {
		hashAes1Rx4<softAes>(scratchpad, ScratchpadSize, &reg.a);
		blake2b(out, outSize, &reg, sizeof(RegisterFile), nullptr, 0);
	}

	template<class Allocator, bool softAes>
	void VmBase<Allocator, softAes>::initScratchpad(void* seed) {
		fillAes1Rx4<softAes>(seed, ScratchpadSize, scratchpad);
	}

	template<class Allocator, bool softAes>
	void VmBase<Allocator, softAes>::generateProgram(void* seed) {
		fillAes1Rx4<softAes>(seed, sizeof(program), &program);
	}

	template class VmBase<AlignedAllocator<CacheLineSize>, false>;
	template class VmBase<AlignedAllocator<CacheLineSize>, true>;
	template class VmBase<LargePageAllocator, false>;
	template class VmBase<LargePageAllocator, true>;
}