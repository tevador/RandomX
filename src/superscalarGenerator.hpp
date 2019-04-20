/*
Copyright (c) 2019 tevador

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
#include "superscalar_program.hpp"
#include "Blake2Generator.hpp"
#include <vector>

namespace randomx {
	                                              //                  Intel Ivy Bridge reference
	namespace SuperscalarInstructionType {        //uOPs (decode)   execution ports         latency       code size
		constexpr int ISUB_R = 0;                 //1               p015                    1               3 (sub)
		constexpr int IXOR_R = 1;                 //1               p015                    1               3 (xor)
		constexpr int IADD_RS = 2;                //1               p01                     1               4 (lea)
		constexpr int IMUL_R = 3;                 //1               p1                      3               4 (imul)
		constexpr int IROR_C = 4;                 //1               p05                     1               4 (ror)
		constexpr int IADD_C7 = 5;                //1               p015                    1               7 (add)
		constexpr int IXOR_C7 = 6;                //1               p015                    1               7 (xor)
		constexpr int IADD_C8 = 7;                //1+0             p015                    1               7+1 (add+nop)
		constexpr int IXOR_C8 = 8;                //1+0             p015                    1               7+1 (xor+nop)
		constexpr int IADD_C9 = 9;                //1+0             p015                    1               7+2 (add+nop)
		constexpr int IXOR_C9 = 10;               //1+0             p015                    1               7+2 (xor+nop)
		constexpr int IMULH_R = 11;               //1+2+1           0+(p1,p5)+0             3               3+3+3 (mov+mul+mov)
		constexpr int ISMULH_R = 12;              //1+2+1           0+(p1,p5)+0             3               3+3+3 (mov+imul+mov)
		constexpr int IMUL_RCP = 13;              //1+1             p015+p1                 4              10+4   (mov+imul)

		constexpr int COUNT = 14;
		constexpr int INVALID = -1;
	}

	void generateSuperscalar(SuperscalarProgram& prog, Blake2Generator& gen);
	void executeSuperscalar(int_reg_t(&r)[8], SuperscalarProgram& prog, std::vector<uint64_t> *reciprocals = nullptr);
}