/*
Copyright (c) 2018-2019, tevador <tevador@gmail.com>

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	* Neither the name of the copyright holder nor the
	  names of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <cstdint>
#include <vector>
#include "superscalar_program.hpp"
#include "blake2_generator.hpp"

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
	void executeSuperscalar(uint64_t(&r)[8], SuperscalarProgram& prog, std::vector<uint64_t> *reciprocals = nullptr);
}