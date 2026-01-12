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

#include "common.hpp"

namespace randomx {

	struct CodeBuffer {
		uint8_t* code;
		int32_t codePos;
		int32_t rcpCount;

		void emit(const uint8_t* src, int32_t len) {
			memcpy(&code[codePos], src, len);
			codePos += len;
		}

		template<typename T>
		void emit(T src) {
			memcpy(&code[codePos], &src, sizeof(src));
			codePos += sizeof(src);
		}

		void emitAt(int32_t codePos, const uint8_t* src, int32_t len) {
			memcpy(&code[codePos], src, len);
		}

		template<typename T>
		void emitAt(int32_t codePos, T src) {
			memcpy(&code[codePos], &src, sizeof(src));
		}
	};

	struct CompilerState : public CodeBuffer {
		int32_t instructionOffsets[RANDOMX_PROGRAM_MAX_SIZE];
		int registerUsage[RegistersCount];
	};
}

#if defined(RANDOMX_COMPILER_X86)
#include "jit_compiler_x86.hpp"
#elif defined(RANDOMX_COMPILER_A64)
#include "jit_compiler_a64.hpp"
#elif defined(RANDOMX_COMPILER_RV64)
#include "jit_compiler_rv64.hpp"
#else
#include "jit_compiler_fallback.hpp"
#endif

#if defined(__OpenBSD__) || defined(__NetBSD__) || (defined(__APPLE__) && defined(__aarch64__))
#define RANDOMX_FORCE_SECURE
#endif
