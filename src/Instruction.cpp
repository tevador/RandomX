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

#include "Instruction.hpp"
#include "common.hpp"

namespace RandomX {

	void Instruction::print(std::ostream& os) const {
		os << names[opcode] << " ";
		auto handler = engine[opcode];
		(this->*handler)(os);
	}

	void Instruction::genAddressReg(std::ostream& os) const {
		os << ((mod % 4) ? "L1" : "L2") << "[r" << (int)src << std::showpos << (int32_t)getImm32() << std::noshowpos << "]";
	}

	void Instruction::genAddressRegDst(std::ostream& os) const {
		os << ((mod % 4) ? "L1" : "L2") << "[r" << (int)dst << std::showpos << (int32_t)getImm32() << std::noshowpos << "]";
	}

	void Instruction::genAddressImm(std::ostream& os) const {
		os << "L3" << "[" << (getImm32() & ScratchpadL3Mask) << "]";
	}

	void Instruction::h_IADD_RS(std::ostream& os) const {
		os << "r" << (int)dst << ", r" << (int)src;
		if(dst == RegisterNeedsDisplacement) {
			os << ", " << (int32_t)getImm32();
		}
		os << ", LSH " << (int)(mod % 4) << std::endl;
	}

	void Instruction::h_IADD_M(std::ostream& os) const {
		if (src != dst) {
			os << "r" << (int)dst << ", ";
			genAddressReg(os);
			os << std::endl;
		}
		else {
			os << "r" << (int)dst << ", ";
			genAddressImm(os);
			os << std::endl;
		}
	}

	void Instruction::h_IADD_RC(std::ostream& os) const {
		os << "r" << (int)dst << ", r" << (int)src << ", " << (int32_t)getImm32() << std::endl;
	}

	//1 uOP
	void Instruction::h_ISUB_R(std::ostream& os) const {
		if (src != dst) {
			os << "r" << (int)dst << ", r" << (int)src << std::endl;
		}
		else {
			os << "r" << (int)dst << ", " << (int32_t)getImm32() << std::endl;
		}
	}

	void Instruction::h_ISUB_M(std::ostream& os) const {
		if (src != dst) {
			os << "r" << (int)dst << ", ";
			genAddressReg(os);
			os << std::endl;
		}
		else {
			os << "r" << (int)dst << ", ";
			genAddressImm(os);
			os << std::endl;
		}
	}

	void Instruction::h_IMUL_9C(std::ostream& os) const {
		os << "r" << (int)dst << ", " << (int32_t)getImm32() << std::endl;
	}

	void Instruction::h_IMUL_R(std::ostream& os) const {
		if (src != dst) {
			os << "r" << (int)dst << ", r" << (int)src << std::endl;
		}
		else {
			os << "r" << (int)dst << ", " << (int32_t)getImm32() << std::endl;
		}
	}

	void Instruction::h_IMUL_M(std::ostream& os) const {
		if (src != dst) {
			os << "r" << (int)dst << ", ";
			genAddressReg(os);
			os << std::endl;
		}
		else {
			os << "r" << (int)dst << ", ";
			genAddressImm(os);
			os << std::endl;
		}
	}

	void Instruction::h_IMULH_R(std::ostream& os) const {
		os << "r" << (int)dst << ", r" << (int)src << std::endl;
	}

	void Instruction::h_IMULH_M(std::ostream& os) const {
		if (src != dst) {
			os << "r" << (int)dst << ", ";
			genAddressReg(os);
			os << std::endl;
		}
		else {
			os << "r" << (int)dst << ", ";
			genAddressImm(os);
			os << std::endl;
		}
	}

	void Instruction::h_ISMULH_R(std::ostream& os) const {
		os << "r" << (int)dst << ", r" << (int)src << std::endl;
	}

	void Instruction::h_ISMULH_M(std::ostream& os) const {
		if (src != dst) {
			os << "r" << (int)dst << ", ";
			genAddressReg(os);
			os << std::endl;
		}
		else {
			os << "r" << (int)dst << ", ";
			genAddressImm(os);
			os << std::endl;
		}
	}

	void Instruction::h_INEG_R(std::ostream& os) const {
		os << "r" << (int)dst << std::endl;
	}

	void Instruction::h_IXOR_R(std::ostream& os) const {
		if (src != dst) {
			os << "r" << (int)dst << ", r" << (int)src << std::endl;
		}
		else {
			os << "r" << (int)dst << ", " << (int32_t)getImm32() << std::endl;
		}
	}

	void Instruction::h_IXOR_M(std::ostream& os) const {
		if (src != dst) {
			os << "r" << (int)dst << ", ";
			genAddressReg(os);
			os << std::endl;
		}
		else {
			os << "r" << (int)dst << ", ";
			genAddressImm(os);
			os << std::endl;
		}
	}

	void Instruction::h_IROR_R(std::ostream& os) const {
		if (src != dst) {
			os << "r" << (int)dst << ", r" << (int)src << std::endl;
		}
		else {
			os << "r" << (int)dst << ", " << (getImm32() & 63) << std::endl;
		}
	}

	void Instruction::h_IROL_R(std::ostream& os) const {
		if (src != dst) {
			os << "r" << (int)dst << ", r" << (int)src << std::endl;
		}
		else {
			os << "r" << (int)dst << ", " << (getImm32() & 63) << std::endl;
		}
	}

	void Instruction::h_IMUL_RCP(std::ostream& os) const {
		os << "r" << (int)dst << ", " << getImm32() << std::endl;
	}

	void Instruction::h_ISDIV_C(std::ostream& os) const {
		os << "r" << (int)dst << ", " << (int32_t)getImm32() << std::endl;
	}

	void Instruction::h_ISWAP_R(std::ostream& os) const {
		os << "r" << (int)dst << ", r" << (int)src << std::endl;
	}

	void Instruction::h_FSWAP_R(std::ostream& os) const {
		const char reg = (dst >= 4) ? 'e' : 'f';
		auto dstIndex = dst % 4;
		os << reg << dstIndex << std::endl;
	}

	void Instruction::h_FADD_R(std::ostream& os) const {
		auto dstIndex = dst % 4;
		auto srcIndex = src % 4;
		os << "f" << dstIndex << ", a" << srcIndex << std::endl;
	}

	void Instruction::h_FADD_M(std::ostream& os) const {
		auto dstIndex = dst % 4;
		os << "f" << dstIndex << ", ";
		genAddressReg(os);
		os << std::endl;
	}

	void Instruction::h_FSUB_R(std::ostream& os) const {
		auto dstIndex = dst % 4;
		auto srcIndex = src % 4;
		os << "f" << dstIndex << ", a" << srcIndex << std::endl;
	}

	void Instruction::h_FSUB_M(std::ostream& os) const {
		auto dstIndex = dst % 4;
		os << "f" << dstIndex << ", ";
		genAddressReg(os);
		os << std::endl;
	}

	void Instruction::h_FSCAL_R(std::ostream& os) const {
		auto dstIndex = dst % 4;
		os << "f" << dstIndex << std::endl;
	}

	void Instruction::h_FMUL_R(std::ostream& os) const {
		auto dstIndex = dst % 4;
		auto srcIndex = src % 4;
		os << "e" << dstIndex << ", a" << srcIndex << std::endl;
	}

	void Instruction::h_FMUL_M(std::ostream& os) const {
		auto dstIndex = dst % 4;
		os << "e" << dstIndex << ", ";
		genAddressReg(os);
		os << std::endl;
	}

	void Instruction::h_FDIV_R(std::ostream& os) const {
		auto dstIndex = dst % 4;
		auto srcIndex = src % 4;
		os << "e" << dstIndex << ", a" << srcIndex << std::endl;
	}

	void Instruction::h_FDIV_M(std::ostream& os) const {
		auto dstIndex = dst % 4;
		os << "e" << dstIndex << ", ";
		genAddressReg(os);
		os << std::endl;
	}

	void Instruction::h_FSQRT_R(std::ostream& os) const {
		auto dstIndex = dst % 4;
		os << "e" << dstIndex << std::endl;
	}

	void Instruction::h_CFROUND(std::ostream& os) const {
		os << "r" << (int)src << ", " << (getImm32() & 63) << std::endl;
	}

	static inline const char* condition(int index) {
		switch (index)
		{
		case 0:
			return "be";
		case 1:
			return "ab";
		case 2:
			return "sg";
		case 3:
			return "ns";
		case 4:
			return "of";
		case 5:
			return "no";
		case 6:
			return "lt";
		case 7:
			return "ge";
		default:
			UNREACHABLE;
		}
	}

	void Instruction::h_COND_R(std::ostream& os) const {
		os << "r" << (int)dst << ", " << condition((mod >> 2) & 7) << "(r" << (int)src << ", " << (int32_t)getImm32() << "), LSH " << (int)(mod >> 5) << std::endl;
	}

	void Instruction::h_COND_M(std::ostream& os) const {
		os << "r" << (int)dst << ", " << condition((mod >> 2) & 7) << "(";
		genAddressReg(os);
		os << ", " << (int32_t)getImm32() << "), LSH " << (int)(mod >> 5) << std::endl;
	}

	void  Instruction::h_ISTORE(std::ostream& os) const {
		genAddressRegDst(os);
		os << ", r" << (int)src << std::endl;
	}

	void  Instruction::h_FSTORE(std::ostream& os) const {
		const char reg = (src >= 4) ? 'e' : 'f';
		genAddressRegDst(os);
		auto srcIndex = src % 4;
		os << ", " << reg << srcIndex << std::endl;
	}

	void  Instruction::h_NOP(std::ostream& os) const {
		os << std::endl;
	}

#include "instructionWeights.hpp"
#define INST_NAME(x) REPN(#x, WT(x))
#define INST_HANDLE(x) REPN(&Instruction::h_##x, WT(x))

	const char* Instruction::names[256] = {
		//Integer
		INST_NAME(IADD_RS)
		INST_NAME(IADD_M)
		INST_NAME(IADD_RC)
		INST_NAME(ISUB_R)
		INST_NAME(ISUB_M)
		INST_NAME(IMUL_9C)
		INST_NAME(IMUL_R)
		INST_NAME(IMUL_M)
		INST_NAME(IMULH_R)
		INST_NAME(IMULH_M)
		INST_NAME(ISMULH_R)
		INST_NAME(ISMULH_M)
		INST_NAME(IMUL_RCP)
		INST_NAME(INEG_R)
		INST_NAME(IXOR_R)
		INST_NAME(IXOR_M)
		INST_NAME(IROR_R)
		INST_NAME(ISWAP_R)

		//Common floating point
		INST_NAME(FSWAP_R)

		//Floating point group F
		INST_NAME(FADD_R)
		INST_NAME(FADD_M)
		INST_NAME(FSUB_R)
		INST_NAME(FSUB_M)
		INST_NAME(FSCAL_R)

		//Floating point group E
		INST_NAME(FMUL_R)
		INST_NAME(FDIV_M)
		INST_NAME(FSQRT_R)

		//Control
		INST_NAME(COND_R)
		INST_NAME(COND_M)
		INST_NAME(CFROUND)

		INST_NAME(ISTORE)

		INST_NAME(NOP)
	};

	InstructionVisualizer Instruction::engine[256] = {
		//Integer
		INST_HANDLE(IADD_RS)
		INST_HANDLE(IADD_M)
		INST_HANDLE(IADD_RC)
		INST_HANDLE(ISUB_R)
		INST_HANDLE(ISUB_M)
		INST_HANDLE(IMUL_9C)
		INST_HANDLE(IMUL_R)
		INST_HANDLE(IMUL_M)
		INST_HANDLE(IMULH_R)
		INST_HANDLE(IMULH_M)
		INST_HANDLE(ISMULH_R)
		INST_HANDLE(ISMULH_M)
		INST_HANDLE(IMUL_RCP)
		INST_HANDLE(INEG_R)
		INST_HANDLE(IXOR_R)
		INST_HANDLE(IXOR_M)
		INST_HANDLE(IROR_R)
		INST_HANDLE(IROL_R)
		INST_HANDLE(ISWAP_R)

		//Common floating point
		INST_HANDLE(FSWAP_R)

		//Floating point group F
		INST_HANDLE(FADD_R)
		INST_HANDLE(FADD_M)
		INST_HANDLE(FSUB_R)
		INST_HANDLE(FSUB_M)
		INST_HANDLE(FSCAL_R)

		//Floating point group E
		INST_HANDLE(FMUL_R)
		INST_HANDLE(FDIV_M)
		INST_HANDLE(FSQRT_R)

		//Control
		INST_HANDLE(COND_R)
		INST_HANDLE(COND_M)
		INST_HANDLE(CFROUND)

		INST_HANDLE(ISTORE)

		INST_HANDLE(NOP)
	};

}