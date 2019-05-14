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

#include <new>
#include <vector>
#include "common.hpp"
#include "virtual_machine.hpp"
#include "intrin_portable.h"
#include "allocator.hpp"

namespace randomx {

	struct InstructionByteCode {
		union {
			int_reg_t* idst;
			rx_vec_f128* fdst;
		};
		union {
			int_reg_t* isrc;
			rx_vec_f128* fsrc;
		};
		union {
			uint64_t imm;
			int64_t simm;
		};
		uint16_t type;
		union {
			int16_t target;
			uint16_t shift;
		};
		uint32_t memMask;
	};

	template<class Allocator, bool softAes>
	class InterpretedVm : public VmBase<Allocator, softAes> {
	public:
		using VmBase<Allocator, softAes>::mem;
		using VmBase<Allocator, softAes>::scratchpad;
		using VmBase<Allocator, softAes>::program;
		using VmBase<Allocator, softAes>::config;
		using VmBase<Allocator, softAes>::reg;
		using VmBase<Allocator, softAes>::datasetPtr;
		using VmBase<Allocator, softAes>::datasetOffset;
		void* operator new(size_t size) {
			void* ptr = AlignedAllocator<CacheLineSize>::allocMemory(size);
			if (ptr == nullptr)
				throw std::bad_alloc();
			return ptr;
		}
		void operator delete(void* ptr) {
			AlignedAllocator<CacheLineSize>::freeMemory(ptr, sizeof(InterpretedVm));
		}
		void run(void* seed) override;
		void setDataset(randomx_dataset* dataset) override;
	protected:
		virtual void datasetRead(uint32_t blockNumber, int_reg_t(&r)[RegistersCount]);
	private:
		void execute();
		void precompileProgram(int_reg_t(&r)[RegistersCount], rx_vec_f128(&f)[RegisterCountFlt], rx_vec_f128(&e)[RegisterCountFlt], rx_vec_f128(&a)[RegisterCountFlt]);
		void executeBytecode(int_reg_t(&r)[RegistersCount], rx_vec_f128(&f)[RegisterCountFlt], rx_vec_f128(&e)[RegisterCountFlt], rx_vec_f128(&a)[RegisterCountFlt]);
		void executeBytecode(int& i, int_reg_t(&r)[RegistersCount], rx_vec_f128(&f)[RegisterCountFlt], rx_vec_f128(&e)[RegisterCountFlt], rx_vec_f128(&a)[RegisterCountFlt]);
		void* getScratchpadAddress(InstructionByteCode& ibc);
		rx_vec_f128 maskRegisterExponentMantissa(rx_vec_f128);

		InstructionByteCode byteCode[RANDOMX_PROGRAM_SIZE];
	};

	using InterpretedVmDefault = InterpretedVm<AlignedAllocator<CacheLineSize>, true>;
	using InterpretedVmHardAes = InterpretedVm<AlignedAllocator<CacheLineSize>, false>;
	using InterpretedVmLargePage = InterpretedVm<LargePageAllocator, true>;
	using InterpretedVmLargePageHardAes = InterpretedVm<LargePageAllocator, false>;
}