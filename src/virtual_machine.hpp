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
#include "common.hpp"
#include "program.hpp"

/* Global namespace for C binding */
class randomx_vm {
public:
	virtual ~randomx_vm() = 0;
	virtual void allocate() = 0;
	virtual void getFinalResult(void* out, size_t outSize) = 0;
	virtual void hashAndFill(void* out, size_t outSize, uint64_t *fill_state) = 0;
	virtual void setDataset(randomx_dataset* dataset) { }
	virtual void setCache(randomx_cache* cache) { }
	virtual void initScratchpad(void* seed) = 0;
	virtual void run(void* seed) = 0;
	void resetRoundingMode();
	randomx::RegisterFile *getRegisterFile() {
		return &reg;
	}
	const void* getScratchpad() {
		return scratchpad;
	}
	void setScratchpad(void* memory) {
		scratchpad = (uint8_t*)memory;
	}
	const randomx::Program& getProgram()
	{
		return program;
	}
protected:
	void initialize();
	alignas(64) randomx::Program program;
	alignas(64) randomx::RegisterFile reg;
	alignas(16) randomx::ProgramConfiguration config;
	randomx::MemoryRegisters mem;
	uint8_t* scratchpad = nullptr;
	union {
		randomx_cache* cachePtr = nullptr;
		randomx_dataset* datasetPtr;
	};
	uint64_t datasetOffset;
public:
	std::string cacheKey;
	alignas(16) uint64_t tempHash[8]; //8 64-bit values used to store intermediate data
};

namespace randomx {

	template<class Allocator, bool softAes>
	class VmBase : public randomx_vm {
	public:
		~VmBase() override;
		void allocate() override;
		void initScratchpad(void* seed) override;
		void getFinalResult(void* out, size_t outSize) override;
		void hashAndFill(void* out, size_t outSize, uint64_t *fill_state) override;
	protected:
		void generateProgram(void* seed);
	};

}
