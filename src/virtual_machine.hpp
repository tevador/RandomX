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
#include "common.hpp"
#include "program.hpp"

/* Global namespace for C binding */
class randomx_vm {
public:
	virtual ~randomx_vm() = 0;
	virtual bool allocate() = 0;
	virtual void getFinalResult(void* out, size_t outSize) = 0;
	virtual void setDataset(randomx_dataset* dataset) { }
	virtual void setCache(randomx_cache* cache) { }
	virtual void initScratchpad(void* seed) = 0;
	virtual void run(void* seed) = 0;
	void resetRoundingMode();
	randomx::RegisterFile *getRegisterFile() {
		return &reg;
	}
protected:
	void initialize();
	alignas(64) randomx::Program program;
	alignas(64) randomx::RegisterFile reg;
	alignas(16) randomx::ProgramConfiguration config;
	randomx::MemoryRegisters mem;
	uint8_t* scratchpad;
};

namespace randomx {

	template<class Allocator, bool softAes>
	class VmBase : public randomx_vm {
	public:
		~VmBase() override;
		bool allocate() override;
		void initScratchpad(void* seed) override;
		void getFinalResult(void* out, size_t outSize) override;
	protected:
		void generateProgram(void* seed);
	};

}