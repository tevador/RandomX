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
#include "hashAes1Rx4.hpp"
#include "blake2/blake2.h"
#include <cstring>
#include <iomanip>
#include "intrinPortable.h"

std::ostream& operator<<(std::ostream& os, const RandomX::RegisterFile& rf) {
	for (int i = 0; i < RandomX::RegistersCount; ++i)
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

namespace RandomX {

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

	VirtualMachine::VirtualMachine() {
		mem.ds.dataset = nullptr;
	}

	void VirtualMachine::resetRoundingMode() {
		initFpu();
	}

	void VirtualMachine::initialize() {
		store64(&reg.a[0].lo, getSmallPositiveFloatBits(program.getEntropy(0)));
		store64(&reg.a[0].hi, getSmallPositiveFloatBits(program.getEntropy(1)));
		store64(&reg.a[1].lo, getSmallPositiveFloatBits(program.getEntropy(2)));
		store64(&reg.a[1].hi, getSmallPositiveFloatBits(program.getEntropy(3)));
		store64(&reg.a[2].lo, getSmallPositiveFloatBits(program.getEntropy(4)));
		store64(&reg.a[2].hi, getSmallPositiveFloatBits(program.getEntropy(5)));
		store64(&reg.a[3].lo, getSmallPositiveFloatBits(program.getEntropy(6)));
		store64(&reg.a[3].hi, getSmallPositiveFloatBits(program.getEntropy(7)));
		mem.ma = program.getEntropy(8) & CacheLineAlignMask;
		mem.mx = program.getEntropy(10);
		auto addressRegisters = program.getEntropy(12);
		readReg0 = 0 + (addressRegisters & 1);
		addressRegisters >>= 1;
		readReg1 = 2 + (addressRegisters & 1);
		addressRegisters >>= 1;
		readReg2 = 4 + (addressRegisters & 1);
		addressRegisters >>= 1;
		readReg3 = 6 + (addressRegisters & 1);
	}

	template<bool softAes>
	void VirtualMachine::getResult(void* scratchpad, size_t scratchpadSize, void* outHash) {
		if (scratchpadSize > 0) {
			hashAes1Rx4<false>(scratchpad, scratchpadSize, &reg.a);
		}
		blake2b(outHash, ResultSize, &reg, sizeof(RegisterFile), nullptr, 0);
	}

	template void VirtualMachine::getResult<false>(void* scratchpad, size_t scratchpadSize, void* outHash);
	template void VirtualMachine::getResult<true>(void* scratchpad, size_t scratchpadSize, void* outHash);

}