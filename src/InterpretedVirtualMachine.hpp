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

	class InterpretedVirtualMachine;

	typedef void(InterpretedVirtualMachine::*InstructionHandler)(Instruction&);

	class InterpretedVirtualMachine : public VirtualMachine {
	public:
		InterpretedVirtualMachine(bool softAes) : VirtualMachine(softAes) {}
		virtual void initializeProgram(const void* seed) override;
		virtual void execute() override;
		const Program& getProgam() {
			return p;
		}
	private:
		static InstructionHandler engine[256];
		Program p;
		std::vector<convertible_t> stack;
		uint64_t pc, ic;
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
		int count_FPADD = 0;
		int count_FPSUB = 0;
		int count_FPMUL = 0;
		int count_FPDIV = 0;
		int count_FPSQRT = 0;
		int count_FPROUND = 0;
		int count_CALL_taken = 0;
		int count_CALL_not_taken = 0;
		int count_RET_stack_empty = 0;
		int count_RET_taken = 0;
		int count_RET_not_taken = 0;
		int count_jump_taken[8] = { 0 };
		int count_jump_not_taken[8] = { 0 };
		int count_max_stack = 0;
		int count_retdepth = 0;
		int count_retdepth_max = 0;
		int count_endstack = 0;
		int count_instructions[ProgramLength] = { 0 };
#endif

		convertible_t loada(Instruction&);
		convertible_t loadbr0(Instruction&);
		convertible_t loadbr1(Instruction&);
		convertible_t& getcr(Instruction&);
		void writecf(Instruction&, fpu_reg_t&);
		void writecflo(Instruction&, fpu_reg_t&);

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
		void h_FPADD(Instruction&);
		void h_FPSUB(Instruction&);
		void h_FPMUL(Instruction&);
		void h_FPDIV(Instruction&);
		void h_FPSQRT(Instruction&);
		void h_FPROUND(Instruction&);
		void h_CALL(Instruction&);
		void h_RET(Instruction&);
	};
}