/*
Copyright (c) 2023 tevador <tevador@gmail.com>

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
#include <cstring>
#include <vector>
#include "jit_compiler.hpp"

namespace randomx {

	class Program;
	struct ProgramConfiguration;
	class SuperscalarProgram;
	class Instruction;

	class JitCompilerRV64 {
	public:
		JitCompilerRV64();
		~JitCompilerRV64();
		void generateProgram(Program&, ProgramConfiguration&);
		void generateProgramLight(Program&, ProgramConfiguration&, uint32_t);
		void generateSuperscalarHash(SuperscalarProgramList &programs, std::vector<uint64_t>&);
		void generateDatasetInitCode() {}
		ProgramFunc* getProgramFunc() {
			return (ProgramFunc*)(vectorCode ? entryProgramVector : entryProgram);
		}
		DatasetInitFunc* getDatasetInitFunc() {
			return (DatasetInitFunc*)((vectorCode && (vectorRegisterLength >= 256)) ? entryDataInitVector : entryDataInit);
		}
		uint8_t* getCode() {
			return vectorCode ? vectorCode : state.code;
		}
		size_t getCodeSize();
		void enableWriting();
		void enableExecution();
		void enableAll();

		void setFlags(randomx_flags f) { flags = f; }

		static uint8_t instMap[256];
	private:
		CompilerState state;

		randomx_flags flags;

		uint8_t* vectorCode = nullptr;
		size_t vectorCodeSize = 0;
		int vectorRegisterLength = 0;

		void* entryDataInit = nullptr;
		void* entryDataInitVector = nullptr;
		void* entryProgram = nullptr;
		void* entryProgramVector = nullptr;
	};
}
