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
		for (int i = 0; i < 8; ++i) {
			reg.f[i].f64 = (double)reg.f[i].i64;
		}
		p.initialize(gen);
		mem.ma = (gen() ^ *(((uint32_t*)seed) + 4)) & ~7;
		mem.mx = *(((uint32_t*)seed) + 5);
		pc = 0;
		ic = InstructionCount;
		stack.clear();
	}

	void InterpretedVirtualMachine::execute() {
		while (ic > 0) {
			auto& inst = p(pc);
			if(trace) std::cout << p.getName(inst) << " (" << std::dec << pc << ")" << std::endl;
			pc = (pc + 1) % ProgramLength;
			auto handler = engine[inst.opcode];
			(this->*handler)(inst);
			ic--;
		}
	}

	convertible_t InterpretedVirtualMachine::loada(Instruction& inst) {
		convertible_t& rega = reg.r[inst.rega % RegistersCount];
		rega.u64 ^= inst.addr0;
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
		switch (inst.loca & 7)
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
			temp.i64 = inst.imm1;
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
		case 4:
		case 5:
			return reg.r[inst.regb % RegistersCount];
		case 6:
		case 7:
			convertible_t temp;
			temp.u64 = inst.imm0;
			return temp;
		}
	}

	double InterpretedVirtualMachine::loadbf(Instruction& inst) {
		switch (inst.locb & 7)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			return reg.f[inst.regb % RegistersCount].f64;
		case 6:
		case 7:
			return (double)inst.imm1;
		}
	}

	convertible_t& InterpretedVirtualMachine::getcr(Instruction& inst) {
		addr_t addr;
		switch (inst.locc & 7)
		{
		case 0:
			addr = reg.r[inst.regc % RegistersCount].u32 ^ inst.addr1;
			return scratchpad[addr % ScratchpadL2];

		case 1:
		case 2:
		case 3:
			addr = reg.r[inst.regc % RegistersCount].u32 ^ inst.addr1;
			return scratchpad[addr % ScratchpadL1];

		case 4:
		case 5:
		case 6:
		case 7:
			return reg.r[inst.regc % RegistersCount];
		}
	}

	convertible_t& InterpretedVirtualMachine::getcf(Instruction& inst) {
		addr_t addr;
		switch (inst.locc & 7)
		{
		case 0:
			addr = reg.r[inst.regc % RegistersCount].u32 ^ inst.addr1;
			return scratchpad[addr % ScratchpadL2];

		case 1:
		case 2:
		case 3:
			addr = reg.r[inst.regc % RegistersCount].u32 ^ inst.addr1;
			return scratchpad[addr % ScratchpadL1];

		case 4:
		case 5:
		case 6:
		case 7:
			return reg.f[inst.regc % RegistersCount];
		}
	}

#define ALU_RETIRE(x) x(a, b, c); \
	if(trace) std::cout << std::hex << a.u64 << " " << b.u64 << " " << c.u64 << std::endl;

#define FPU_RETIRE(x) x(a, b, c); \
	if(trace) { \
		convertible_t bc; \
		bc.f64 = b; \
		std::cout << std::hex << a.u64 << " " << bc.u64 << " " << c.u64 << std::endl; \
	} \
	if(fpuCheck) { \
		convertible_t bc; \
		if(c.f64 != c.f64)  { \
			std::stringstream ss; \
			bc.f64 = b; \
			ss << "NaN result of " << #x << "(" << std::hex << a.u64 << ", " << bc.u64 << ") = " << c.u64; \
			throw std::runtime_error(ss.str()); \
		} else if (std::fpclassify(c.f64) == FP_SUBNORMAL) {\
			std::stringstream ss; \
			bc.f64 = b; \
			ss << "Denormal result of " << #x << "(" << std::hex << a.u64 << ", " << bc.u64 << ") = " << c.u64; \
			throw std::runtime_error(ss.str()); \
		} \
	}

#define FPU_RETIRE_NB(x) x(a, b, c); \
	if(trace) std::cout << std::hex << a.u64 << " " << c.u64 << std::endl;

#define ALU_INST(x) void InterpretedVirtualMachine::h_##x(Instruction& inst) { \
	convertible_t a = loada(inst); \
	convertible_t b = loadbr1(inst); \
	convertible_t& c = getcr(inst); \
	ALU_RETIRE(x) \
	}

#define ALU_INST_SR(x) void InterpretedVirtualMachine::h_##x(Instruction& inst) { \
	convertible_t a = loada(inst); \
	convertible_t b = loadbr0(inst); \
	convertible_t& c = getcr(inst); \
	ALU_RETIRE(x) \
	}

#define FPU_INST(x) void InterpretedVirtualMachine::h_##x(Instruction& inst) { \
	convertible_t a = loada(inst); \
	double b = loadbf(inst); \
	convertible_t& c = getcf(inst); \
	FPU_RETIRE(x) \
	}

#define FPU_INST_NB(x) void InterpretedVirtualMachine::h_##x(Instruction& inst) { \
	convertible_t a = loada(inst); \
	convertible_t b; \
	convertible_t& c = getcf(inst); \
	FPU_RETIRE_NB(x) \
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
		convertible_t b = loadbr1(inst);
		convertible_t& c = getcr(inst);
		if (b.u32 <= (uint32_t)inst.imm1) {
			stackPush(a);
			stackPush(pc);
			pc += (inst.imm0 & 127) + 1;
			pc = pc % ProgramLength;
		}
		else {
			c.u64 = a.u64;
		}
	}

	void InterpretedVirtualMachine::h_RET(Instruction& inst) {
		convertible_t a = loada(inst);
		convertible_t b = loadbr1(inst);
		convertible_t& c = getcr(inst);
		if (stack.size() > 0 && b.u32 <= (uint32_t)inst.imm1) {
			auto raddr = stackPopAddress();
			auto retval = stackPopValue();
			c.u64 = a.u64 ^ retval.u64;
			pc = raddr;
		}
		else {
			c.u64 = a.u64;
		}
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