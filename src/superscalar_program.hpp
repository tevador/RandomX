/*
Copyright (c) 2018-2024, tevador <tevador@gmail.com>

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
#include "instruction.hpp"
#include "common.hpp"

namespace randomx {

	class SuperscalarProgram {
	public:
		Instruction& operator()(int pc) {
			return programBuffer[pc];
		}
		friend std::ostream& operator<<(std::ostream& os, const SuperscalarProgram& p) {
			p.print(os);
			return os;
		}
		uint32_t getSize() {
			return size;
		}
		void setSize(uint32_t val) {
			size = val;
		}
		int getAddressRegister() {
			return addrReg;
		}
		void setAddressRegister(int val) {
			addrReg = val;
		}

		Instruction programBuffer[SuperscalarMaxSize];
		uint32_t size
#ifndef NDEBUG
			= 0
#endif
			;
		int addrReg;
		double ipc;
		int codeSize;
		int macroOps;
		int decodeCycles;
		int cpuLatency;
		int asicLatency;
		int mulCount;
		int cpuLatencies[8];
		int asicLatencies[8];
	private:
		void print(std::ostream& os) const {
			for (unsigned i = 0; i < size; ++i) {
				auto instr = programBuffer[i];
				os << instr;
			}
		}
	};

}