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

#include "Program.hpp"

namespace RandomX {

	//                             Intel Ivy Bridge reference
	namespace SuperscalarInstructionType {        //uOPs (decode)   execution ports         latency       code size
		constexpr int ISUB_R = 0;           //1               p015                    1               3
		constexpr int IXOR_R = 1;           //1               p015                    1               3
		constexpr int IADD_RS = 2;          //1               p01                     1               4
		constexpr int IMUL_R = 3;           //1               p1                      3               4
		constexpr int IROR_C = 4;           //1               p05                     1               4
		constexpr int IADD_C7 = 5;          //1               p015                    1               7
		constexpr int IXOR_C7 = 6;          //1               p015                    1               7
		constexpr int IADD_C8 = 7;          //1+0             p015                    1               8
		constexpr int IXOR_C8 = 8;          //1+0             p015                    1               8
		constexpr int IADD_C9 = 9;          //1+0             p015                    1               9
		constexpr int IXOR_C9 = 10;         //1+0             p015                    1               9
		constexpr int IMULH_R = 11;         //1+2+1           0+(p1,p5)+0             3               3+3+3
		constexpr int ISMULH_R = 12;        //1+2+1           0+(p1,p5)+0             3               3+3+3
		constexpr int IMUL_RCP = 13;        //1+1             p015+p1                 4              10+4

		constexpr int COUNT = 14;
		constexpr int INVALID = -1;
	}

	class Blake2Generator {
	public:
		Blake2Generator(const void* seed, int nonce);
		uint8_t getByte();
		uint32_t getInt32();
	private:
		uint8_t data[64];
		size_t dataIndex;

		void checkData(const size_t);
	};

	double generateSuperscalar(LightProgram& prog, Blake2Generator& gen);
}