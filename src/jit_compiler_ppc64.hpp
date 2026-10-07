/*
Copyright (c) 2023 tevador <tevador@gmail.com>
Copyright (c) 2026, Forest Crossman <cyrozap@gmail.com>

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

#include <vector>

#include "common.hpp"
#include "jit_compiler.hpp"

#include "jit_compiler_ppc64_static.hpp"

#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	#define PPC_BIG_ENDIAN 1
#else
	#define PPC_BIG_ENDIAN 0
#endif

#if (defined(_CALL_ELF) && _CALL_ELF == 2) || (!defined(_CALL_ELF) && !PPC_BIG_ENDIAN)
	#define PPC_ABI_V2 1
#else
	#define PPC_ABI_V2 0
#endif

namespace randomx {

	class Program;
	struct ProgramConfiguration;
	class SuperscalarProgram;
	class Instruction;

	class JitCompilerPPC64 {
	public:
		JitCompilerPPC64();
		~JitCompilerPPC64();

		void generateProgram(Program&, ProgramConfiguration&);
		void generateProgramLight(Program&, ProgramConfiguration&, uint32_t);

		void generateSuperscalarHash(SuperscalarProgramList& programs, std::vector<uint64_t> &);

		void generateDatasetInitCode() {}

		ProgramFunc* getProgramFunc() {
#if PPC_ABI_V2
			return reinterpret_cast<ProgramFunc*>(entryProgram);
#else
			return reinterpret_cast<ProgramFunc*>(descriptorProgram);
#endif
		}
		DatasetInitFunc* getDatasetInitFunc() {
#if PPC_ABI_V2
			return reinterpret_cast<DatasetInitFunc*>(entryDataInit);
#else
			return reinterpret_cast<DatasetInitFunc*>(descriptorDataInit);
#endif
		}
		uint8_t* getCode() { return state.code; }
		size_t getCodeSize();

		void enableWriting();
		void enableExecution();
		void enableAll();

		void setFlags(randomx_flags f) { flags = f; }

		uint32_t getTempGpr();
		uint32_t getTempVr();

		static uint8_t instMap[256];

	private:
		void emitProgramPrefix(CompilerState& state, Program& prog, ProgramConfiguration& pcfg, randomx_flags flags);
		void emitProgramSuffix(CompilerState& state, ProgramConfiguration& pcfg, randomx_flags flags);

		CompilerState state;
		randomx_flags flags;

		void* entryDataInit = nullptr;
		void* entryProgram = nullptr;
#if !PPC_ABI_V2
		uint64_t descriptorProgram[3];
		uint64_t descriptorDataInit[3];
#endif

		int32_t RandomXCodePos;
		int32_t SshashSingleItemPos;
		int32_t LoopBeginPos;

		uint32_t tempGprIndex = 0;
		uint32_t tempVrIndex = 0;
	};

}
