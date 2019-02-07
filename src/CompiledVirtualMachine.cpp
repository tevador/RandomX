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

#include "CompiledVirtualMachine.hpp"
#include "Pcg32.hpp"
#include "common.hpp"
#include "instructions.hpp"
#include <stdexcept>

namespace RandomX {

	constexpr int mantissaSize = 52;
	constexpr int exponentSize = 11;
	constexpr uint64_t mantissaMask = (1ULL << mantissaSize) - 1;
	constexpr uint64_t exponentMask = (1ULL << exponentSize) - 1;
	constexpr int exponentBias = 1023;

	CompiledVirtualMachine::CompiledVirtualMachine() {
		totalSize = 0;
	}

	void CompiledVirtualMachine::setDataset(dataset_t ds) {
		mem.ds = ds;
	}

	void CompiledVirtualMachine::initializeScratchpad(uint8_t* scratchpad, int32_t index) {
		memcpy(scratchpad, mem.ds.dataset + ScratchpadSize * index, ScratchpadSize);
	}

	static uint64_t getSmallPositiveFloatBits(uint64_t entropy) {
		auto exponent = entropy >> 59; //0..31
		auto mantissa = entropy & mantissaMask;
		exponent += exponentBias;
		exponent &= exponentMask;
		exponent <<= mantissaSize;
		return exponent | mantissa;
	}

	void CompiledVirtualMachine::initializeProgram(const void* seed) {
		Pcg32 gen(seed);
		for (unsigned i = 0; i < sizeof(reg) / sizeof(Pcg32::result_type); ++i) {
			*(((uint32_t*)&reg) + i) = gen();
		}
		initFpu();
		/*for (int i = 0; i < RegistersCount / 2; ++i) {
			reg.f[i].lo.f64 = (double)reg.f[i].lo.i64;
			reg.f[i].hi.f64 = (double)reg.f[i].hi.i64;
		}
		for (int i = 0; i < RegistersCount / 2; ++i) {
			reg.g[i].lo.f64 = std::abs((double)reg.g[i].lo.i64);
			reg.g[i].hi.f64 = std::abs((double)reg.g[i].hi.i64);
		}*/
		for (int i = 0; i < RegistersCount / 2; ++i) {
			reg.a[i].lo.u64 = getSmallPositiveFloatBits(reg.f[i].lo.u64);
			reg.a[i].hi.u64 = getSmallPositiveFloatBits(reg.f[i].hi.u64);
		}
		compiler.generateProgram(gen);
		mem.ma = (gen() ^ *(((uint32_t*)seed) + 4)) & -64;
		mem.mx = *(((uint32_t*)seed) + 5);
	}

	void CompiledVirtualMachine::execute() {
		//executeProgram(reg, mem, scratchpad, InstructionCount);
		totalSize += compiler.getCodeSize();
		compiler.getProgramFunc()(reg, mem, scratchpad, InstructionCount);
#ifdef TRACEVM
		for (int32_t i = InstructionCount - 1; i >= 0; --i) {
			std::cout << std::hex << tracepad[i].u64 << std::endl;
		}
#endif

	}
}