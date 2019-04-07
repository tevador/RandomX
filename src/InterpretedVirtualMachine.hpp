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
//#define STATS
#include <new>
#include "VirtualMachine.hpp"
#include "Program.hpp"
#include "intrinPortable.h"

namespace RandomX {

	class ITransform {
	public:
		virtual int32_t apply(int32_t) const = 0;
		virtual const char* getName() const = 0;
		virtual std::ostream& printAsm(std::ostream&) const = 0;
		virtual std::ostream& printCxx(std::ostream&) const = 0;
	};

	struct InstructionByteCode;
	class InterpretedVirtualMachine;

	typedef void(InterpretedVirtualMachine::*InstructionHandler)(Instruction&);

	struct alignas(8) InstructionByteCode {
		union {
			int_reg_t* idst;
			__m128d* fdst;
		};
		union {
			int_reg_t* isrc;
			__m128d* fsrc;
		};
		union {
			uint64_t imm;
			int64_t simm;
		};
		int_reg_t* creg;
		uint16_t condition;
		int16_t target;
		uint32_t memMask;
		uint16_t type;
		uint16_t shift;
	};

	constexpr int asedwfagdewsa = sizeof(InstructionByteCode);

	class InterpretedVirtualMachine : public VirtualMachine {
	public:
		void* operator new(size_t size) {
			void* ptr = _mm_malloc(size, 64);
			if (ptr == nullptr)
				throw std::bad_alloc();
			return ptr;
		}
		void operator delete(void* ptr) {
			_mm_free(ptr);
		}
		InterpretedVirtualMachine(bool soft) : softAes(soft) {}
		~InterpretedVirtualMachine();
		void setDataset(dataset_t ds, uint64_t size, LightProgram(&programs)[RANDOMX_CACHE_ACCESSES]) override;
		void initialize() override;
		void execute() override;
	private:
		static InstructionHandler engine[256];
		DatasetReadFunc readDataset;
		bool softAes;
		InstructionByteCode byteCode[RANDOMX_PROGRAM_SIZE];
		
#ifdef STATS
		int count_ADD_64 = 0;
		int count_ADD_32 = 0;
		int count_SUB_64 = 0;
		int count_SUB_32 = 0;
		int count_MUL_64 = 0;
		int count_MULH_64 = 0;
		int count_MUL_32 = 0;
		int count_IMUL_32 = 0;
		int count_IMULH_64 = 0;
		int count_DIV_64 = 0;
		int count_IDIV_64 = 0;
		int count_AND_64 = 0;
		int count_AND_32 = 0;
		int count_OR_64 = 0;
		int count_OR_32 = 0;
		int count_XOR_64 = 0;
		int count_XOR_32 = 0;
		int count_SHL_64 = 0;
		int count_SHR_64 = 0;
		int count_SAR_64 = 0;
		int count_ROL_64 = 0;
		int count_ROR_64 = 0;
		int count_FADD = 0;
		int count_FSUB = 0;
		int count_FMUL = 0;
		int count_FDIV = 0;
		int count_FSQRT = 0;
		int count_FPROUND = 0;
		int count_JUMP_taken = 0;
		int count_JUMP_not_taken = 0;
		int count_jump_taken[8] = { 0 };
		int count_jump_not_taken[8] = { 0 };
		int count_max_stack = 0;
		int count_retdepth = 0;
		int count_retdepth_max = 0;
		int count_endstack = 0;
		int count_instructions[RANDOMX_PROGRAM_SIZE] = { 0 };
		int count_FADD_nop = 0;
		int count_FADD_nop2 = 0;
		int count_FSUB_nop = 0;
		int count_FSUB_nop2 = 0;
		int count_FMUL_nop = 0;
		int count_FMUL_nop2 = 0;
		int datasetAccess[256] = { 0 };
#endif
		void precompileProgram(int_reg_t(&r)[8], __m128d (&f)[4], __m128d (&e)[4], __m128d (&a)[4]);
		void executeBytecode(int_reg_t(&r)[8], __m128d (&f)[4], __m128d (&e)[4], __m128d (&a)[4]);
		void executeBytecode(int& i, int_reg_t(&r)[8], __m128d (&f)[4], __m128d (&e)[4], __m128d (&a)[4]);
	};
}