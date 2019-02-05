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
#include "VirtualMachine.hpp"
#include "Program.hpp"
#include <vector>

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

	struct alignas(64) InstructionByteCode {
		convertible_t* idst;
		convertible_t* isrc;
		convertible_t imm;
		fpu_reg_t* fdst;
		fpu_reg_t* fsrc;
		uint32_t condition;
		uint32_t memMask;
		uint32_t type;
	};

	constexpr int asedwfagdewsa = sizeof(InstructionByteCode);

	class InterpretedVirtualMachine : public VirtualMachine {
	public:
		InterpretedVirtualMachine(bool soft, bool async) : softAes(soft), asyncWorker(async) {}
		~InterpretedVirtualMachine();
		void setDataset(dataset_t ds) override;
		void initializeScratchpad(uint8_t* scratchpad, int32_t index) override;
		void initializeProgram(const void* seed) override;
		void execute() override;
		const Program& getProgam() {
			return p;
		}
	private:
		static InstructionHandler engine[256];
		static const ITransform* addressTransformations[TransformationCount];
		bool softAes, asyncWorker;
		Program p;
		InstructionByteCode byteCode[ProgramLength];
		std::vector<convertible_t> stack;
		uint64_t pc, ic;
		const ITransform* currentTransform;
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
		int count_CALL_taken = 0;
		int count_CALL_not_taken = 0;
		int count_RET_stack_empty = 0;
		int count_RET_taken = 0;
		int count_jump_taken[8] = { 0 };
		int count_jump_not_taken[8] = { 0 };
		int count_max_stack = 0;
		int count_retdepth = 0;
		int count_retdepth_max = 0;
		int count_endstack = 0;
		int count_instructions[ProgramLength] = { 0 };
		int count_FADD_nop = 0;
		int count_FADD_nop2 = 0;
		int count_FSUB_nop = 0;
		int count_FSUB_nop2 = 0;
		int count_FMUL_nop = 0;
		int count_FMUL_nop2 = 0;
		int datasetAccess[256] = { 0 };
#endif
		void executeInstruction(Instruction&);
		convertible_t loada(Instruction&);
		convertible_t loadbiashift(Instruction&);
		convertible_t loadbiadiv(Instruction&);
		convertible_t loadbia(Instruction&);
		convertible_t& getcr(Instruction&);
		void writecf(Instruction&, fpu_reg_t&);

		void stackPush(convertible_t& c) {
			stack.push_back(c);
		}

		void stackPush(uint64_t x) {
			convertible_t c;
			c.u64 = x;
			stack.push_back(c);
		}

		convertible_t stackPopValue() {
			convertible_t top = stack.back();
			stack.pop_back();
			return top;
		}

		uint64_t stackPopAddress() {
			convertible_t top = stack.back();
			stack.pop_back();
			return top.u64;
		}

		void h_ADD_64(Instruction&);
		void h_ADD_32(Instruction&);
		void h_SUB_64(Instruction&);
		void h_SUB_32(Instruction&);
		void h_MUL_64(Instruction&);
		void h_MULH_64(Instruction&);
		void h_MUL_32(Instruction&);
		void h_IMUL_32(Instruction&);
		void h_IMULH_64(Instruction&);
		void h_DIV_64(Instruction&);
		void h_IDIV_64(Instruction&);
		void h_AND_64(Instruction&);
		void h_AND_32(Instruction&);
		void h_OR_64(Instruction&);
		void h_OR_32(Instruction&);
		void h_XOR_64(Instruction&);
		void h_XOR_32(Instruction&);
		void h_SHL_64(Instruction&);
		void h_SHR_64(Instruction&);
		void h_SAR_64(Instruction&);
		void h_ROL_64(Instruction&);
		void h_ROR_64(Instruction&);
		void h_FADD(Instruction&);
		void h_FSUB(Instruction&);
		void h_FMUL(Instruction&);
		void h_FDIV(Instruction&);
		void h_FSQRT(Instruction&);
		void h_FPROUND(Instruction&);
		void h_JUMP(Instruction&);
		void h_CALL(Instruction&);
		void h_RET(Instruction&);
	};
}