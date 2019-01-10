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
//#define TRACE
//#define FPUCHECK
#include "InterpretedVirtualMachine.hpp"
#include "Pcg32.hpp"
#include "instructions.hpp"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <cmath>
#ifdef STATS
#include <algorithm>
#endif

#ifdef FPUCHECK
constexpr bool fpuCheck = true;
#else
constexpr bool fpuCheck = false;
#endif

namespace RandomX {

	void InterpretedVirtualMachine::initializeProgram(const void* seed) {
		Pcg32 gen(seed);
		for (unsigned i = 0; i < sizeof(reg) / sizeof(Pcg32::result_type); ++i) {
			*(((uint32_t*)&reg) + i) = gen();
		}
		FPINIT();
		for (int i = 0; i < RegistersCount; ++i) {
			reg.f[i].lo.f64 = (double)reg.f[i].lo.i64;
			reg.f[i].hi.f64 = (double)reg.f[i].hi.i64;
		}
		//std::cout << reg;
		p.initialize(gen);
		mem.ma = (gen() ^ *(((uint32_t*)seed) + 4)) & ~7;
		mem.mx = *(((uint32_t*)seed) + 5);
		pc = 0;
		ic = InstructionCount;
		stack.clear();
	}

	void InterpretedVirtualMachine::execute() {
		while (ic > 0) {
#ifdef STATS
			count_instructions[pc]++;
#endif
			auto& inst = p(pc);
			if(trace) std::cout << inst.getName() << " (" << std::dec << pc << ")" << std::endl;
			pc = (pc + 1) % ProgramLength;
			auto handler = engine[inst.opcode];
			(this->*handler)(inst);
			ic--;
		}
#ifdef STATS
		count_endstack += stack.size();
#endif
	}

	convertible_t InterpretedVirtualMachine::loada(Instruction& inst) {
		convertible_t& rega = reg.r[inst.rega % RegistersCount];
		rega.i64 ^= inst.addra; //sign-extend addra
		addr_t addr = rega.u32;
		switch (inst.loca & 7)
		{
			case 0:
			case 1:
			case 2:
			case 3:
				return readDataset(addr, mem);

			case 4:
				return scratchpad[addr % ScratchpadL2];

			case 5:
			case 6:
			case 7:
				return scratchpad[addr % ScratchpadL1];
		}
	}

	convertible_t InterpretedVirtualMachine::loadbr1(Instruction& inst) {
		switch (inst.locb & 7)
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				return reg.r[inst.regb % RegistersCount];
			case 6:
			case 7:
				convertible_t temp;
				temp.i64 = inst.imm32; //sign-extend imm32
				return temp;
		}
	}

	convertible_t InterpretedVirtualMachine::loadbr0(Instruction& inst) {
		switch (inst.locb & 7)
		{
			case 0:
			case 1:
			case 2:
			case 3:
				return reg.r[inst.regb % RegistersCount];
			case 4:
			case 5:
			case 6:
			case 7:
				convertible_t temp;
				temp.u64 = inst.imm8;
				return temp;
		}
	}

	convertible_t& InterpretedVirtualMachine::getcr(Instruction& inst) {
		addr_t addr;
		switch (inst.locc & 7)
		{
			case 0:
				addr = reg.r[inst.regc % RegistersCount].u32 ^ inst.addrc;
				return scratchpad[addr % ScratchpadL2];

			case 1:
			case 2:
			case 3:
				addr = reg.r[inst.regc % RegistersCount].u32 ^ inst.addrc;
				return scratchpad[addr % ScratchpadL1];

			case 4:
			case 5:
			case 6:
			case 7:
				return reg.r[inst.regc % RegistersCount];
		}
	}

	void InterpretedVirtualMachine::writecf(Instruction& inst, fpu_reg_t& regc) {
		addr_t addr;
		switch (inst.locc & 7)
		{
			case 4:
				addr = reg.r[inst.regc % RegistersCount].u32 ^ inst.addrc;
				scratchpad[addr % ScratchpadL2] = (inst.locc & 8) ? regc.hi : regc.lo;
				break;

			case 5:
			case 6:
			case 7:
				addr = reg.r[inst.regc % RegistersCount].u32 ^ inst.addrc;
				scratchpad[addr % ScratchpadL1] = (inst.locc & 8) ? regc.hi : regc.lo;

			default:
				break;
		}
	}

	void InterpretedVirtualMachine::writecflo(Instruction& inst, fpu_reg_t& regc) {
		addr_t addr;
		switch (inst.locc & 7)
		{
			case 4:
				addr = reg.r[inst.regc % RegistersCount].u32 ^ inst.addrc;
				scratchpad[addr % ScratchpadL2] = regc.lo;
				break;

			case 5:
			case 6:
			case 7:
				addr = reg.r[inst.regc % RegistersCount].u32 ^ inst.addrc;
				scratchpad[addr % ScratchpadL1] = regc.lo;

			default:
				break;
		}
	}

#define ALU_RETIRE(x) x(a, b, c); \
	if(trace) std::cout << std::hex << /*a.u64 << " " << b.u64 << " " <<*/ c.u64 << std::endl;

#define CHECK_NOP_FPDIV(b, c)
#ifndef STATS
#define CHECK_NOP_FPADD(b, c)
#define CHECK_NOP_FPSUB(b, c)
#define CHECK_NOP_FPMUL(b, c)
#else
#define CHECK_NOP_FPADD(b, c) bool loeq = (b.lo.u64 == c.lo.u64); bool hieq = (b.hi.u64 == c.hi.u64); count_FPADD_nop += loeq + hieq; if(loeq && hieq) count_FPADD_nop2++;
#define CHECK_NOP_FPSUB(b, c) bool loeq = ((b.lo.u64 & INT64_MAX) == (c.lo.u64 & INT64_MAX)); bool hieq = ((b.hi.u64 & INT64_MAX) == (c.hi.u64 & INT64_MAX)); count_FPSUB_nop += loeq + hieq; if(loeq && hieq) count_FPSUB_nop2++; 
#define CHECK_NOP_FPMUL(b, c) bool loeq = (b.lo.u64 == c.lo.u64); bool hieq = (b.hi.u64 == c.hi.u64); count_FPMUL_nop += loeq + hieq; if(loeq && hieq) count_FPMUL_nop2++;
#endif

#define FPU_RETIRE(x) x(a, b, c); \
	writecf(inst, c); \
	if(trace) { \
		std::cout << std::hex << ((inst.locc & 8) ? c.hi.u64 : c.lo.u64) << std::endl; \
	} \
	if(fpuCheck) { \
		if(c.hi.f64 != c.hi.f64 || c.lo.f64 != c.lo.f64)  { \
			std::stringstream ss; \
			ss << "NaN result of " << #x << "(" << std::hex << a.u64 << ", " << b.hi.u64 << " " << b.lo.u64 << ") = " << c.hi.u64 << " " << c.lo.u64 << std::endl; \
			throw std::runtime_error(ss.str()); \
		} else if (std::fpclassify(c.hi.f64) == FP_SUBNORMAL || std::fpclassify(c.lo.f64) == FP_SUBNORMAL) {\
			std::stringstream ss; \
			ss << "Denormal result of " << #x << "(" << std::hex << a.u64 << ", " << b.hi.u64 << " " << b.lo.u64 << ") = " << c.hi.u64 << " " << c.lo.u64 << std::endl; \
			throw std::runtime_error(ss.str()); \
		} \
	}

#ifdef STATS
#define INC_COUNT(x) count_##x++;
#else
#define INC_COUNT(x)
#endif

#define FPU_RETIRE_FPSQRT(x) FPSQRT(a, b, c); \
	writecf(inst, c); \
	if(trace) std::cout << std::hex << ((inst.locc & 8) ? c.hi.u64 : c.lo.u64) << std::endl;

#define FPU_RETIRE_FPROUND(x) FPROUND(a, b, c); \
	writecflo(inst, c); \
	if(trace) std::cout << std::hex << c.lo.u64 << std::endl;

#define ALU_INST(x) void InterpretedVirtualMachine::h_##x(Instruction& inst) { \
	INC_COUNT(x) \
	convertible_t a = loada(inst); \
	convertible_t b = loadbr1(inst); \
	convertible_t& c = getcr(inst); \
	ALU_RETIRE(x) \
	}

#define ALU_INST_SR(x) void InterpretedVirtualMachine::h_##x(Instruction& inst) { \
	INC_COUNT(x) \
	convertible_t a = loada(inst); \
	convertible_t b = loadbr0(inst); \
	convertible_t& c = getcr(inst); \
	ALU_RETIRE(x) \
	}

#define FPU_INST(x) void InterpretedVirtualMachine::h_##x(Instruction& inst) { \
	INC_COUNT(x) \
	convertible_t a = loada(inst); \
	fpu_reg_t& b = reg.f[inst.regb % RegistersCount]; \
	fpu_reg_t btemp = b; \
	fpu_reg_t& c = reg.f[inst.regc % RegistersCount]; \
	FPU_RETIRE(x) \
	CHECK_NOP_##x(btemp, c) \
	}

#define FPU_INST_NB(x) void InterpretedVirtualMachine::h_##x(Instruction& inst) { \
	INC_COUNT(x) \
	convertible_t a = loada(inst); \
	fpu_reg_t b; \
	fpu_reg_t& c = reg.f[inst.regc % RegistersCount]; \
	FPU_RETIRE_##x(x) \
	}

	ALU_INST(ADD_64)
	ALU_INST(ADD_32)
	ALU_INST(SUB_64)
	ALU_INST(SUB_32)
	ALU_INST(MUL_64)
	ALU_INST(MULH_64)
	ALU_INST(MUL_32)
	ALU_INST(IMUL_32)
	ALU_INST(IMULH_64)
	ALU_INST(DIV_64)
	ALU_INST(IDIV_64)
	ALU_INST(AND_64)
	ALU_INST(AND_32)
	ALU_INST(OR_64)
	ALU_INST(OR_32)
	ALU_INST(XOR_64)
	ALU_INST(XOR_32)

	ALU_INST_SR(SHL_64)
	ALU_INST_SR(SHR_64)
	ALU_INST_SR(SAR_64)
	ALU_INST_SR(ROL_64)
	ALU_INST_SR(ROR_64)

	FPU_INST(FPADD)
	FPU_INST(FPSUB)
	FPU_INST(FPMUL)
	FPU_INST(FPDIV)

	FPU_INST_NB(FPSQRT)
	FPU_INST_NB(FPROUND)

	void InterpretedVirtualMachine::h_CALL(Instruction& inst) {
		convertible_t a = loada(inst);
		if (JMP_COND(inst.locb, reg.r[inst.regb % RegistersCount], inst.imm32)) {
#ifdef STATS
			count_CALL_taken++;
			count_jump_taken[inst.locb & 7]++;
			count_retdepth = std::max(0, count_retdepth - 1);
#endif
			stackPush(a);
			stackPush(pc);
#ifdef STATS
			count_max_stack = std::max(count_max_stack, (int)stack.size());
#endif
			pc += (inst.imm8 & 127) + 1;
			pc = pc % ProgramLength;
			if (trace) std::cout << std::hex << a.u64 << std::endl;
		}
		else {
			convertible_t& c = getcr(inst);
#ifdef STATS
			count_CALL_not_taken++;
			count_jump_not_taken[inst.locb & 7]++;
#endif
			c.u64 = a.u64;
			if (trace) std::cout << std::hex << /*a.u64 << " " <<*/ c.u64 << std::endl;
		}
	}

	void InterpretedVirtualMachine::h_RET(Instruction& inst) {
		convertible_t a = loada(inst);
		convertible_t b = loadbr1(inst);
		convertible_t& c = getcr(inst);
		if (stack.size() > 0) {
#ifdef STATS
			count_RET_taken++;
			count_retdepth++;
			count_retdepth_max = std::max(count_retdepth_max, count_retdepth);
#endif
			auto raddr = stackPopAddress();
			auto retval = stackPopValue();
			c.u64 = a.u64 ^ retval.u64;
			pc = raddr;
		}
		else {
#ifdef STATS
			if (stack.size() == 0)
				count_RET_stack_empty++;
			else {
				count_RET_not_taken++;
				count_jump_not_taken[inst.locb & 7]++;
			}
#endif
			c.u64 = a.u64;
		}
		if (trace) std::cout << std::hex << /*a.u64 << " " <<*/ c.u64 << std::endl;
	}

#include "instructionWeights.hpp"
#define INST_HANDLE(x) REPN(&InterpretedVirtualMachine::h_##x, WT(x))

	InstructionHandler InterpretedVirtualMachine::engine[256] = {
		INST_HANDLE(ADD_64)
		INST_HANDLE(ADD_32)
		INST_HANDLE(SUB_64)
		INST_HANDLE(SUB_32)
		INST_HANDLE(MUL_64)
		INST_HANDLE(MULH_64)
		INST_HANDLE(MUL_32)
		INST_HANDLE(IMUL_32)
		INST_HANDLE(IMULH_64)
		INST_HANDLE(DIV_64)
		INST_HANDLE(IDIV_64)
		INST_HANDLE(AND_64)
		INST_HANDLE(AND_32)
		INST_HANDLE(OR_64)
		INST_HANDLE(OR_32)
		INST_HANDLE(XOR_64)
		INST_HANDLE(XOR_32)
		INST_HANDLE(SHL_64)
		INST_HANDLE(SHR_64)
		INST_HANDLE(SAR_64)
		INST_HANDLE(ROL_64)
		INST_HANDLE(ROR_64)
		INST_HANDLE(FPADD)
		INST_HANDLE(FPSUB)
		INST_HANDLE(FPMUL)
		INST_HANDLE(FPDIV)
		INST_HANDLE(FPSQRT)
		INST_HANDLE(FPROUND)
		INST_HANDLE(CALL)
		INST_HANDLE(RET)
	};
}