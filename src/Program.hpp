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

#include <cstdint>
#include <ostream>
#include "common.hpp"

class Pcg32;

namespace RandomX {

	struct Instruction {
		uint8_t opcode;
		uint8_t loca;
		uint8_t rega;
		uint8_t locb;
		uint8_t regb;
		uint8_t locc;
		uint8_t regc;
		uint8_t imm0;
		uint32_t addr0;
		union {
			uint32_t addr1;
			int32_t imm1;
		};
	};

	static_assert(sizeof(Instruction) == 16, "Invalid alignment of struct Instruction");

	class Program {
	public:
		Instruction& operator()(uint64_t pc) {
			return programBuffer[pc];
		}
		const char* getName(Instruction& instr) {
			return instrNames[instr.opcode];
		}
		void initialize(Pcg32& gen);
		friend std::ostream& operator<<(std::ostream& os, const Program& p) {
			p.print(os);
			return os;
		}
	private:
		void print(std::ostream&) const;
		static const char* instrNames[256];
		Instruction programBuffer[ProgramLength];
	};
}
