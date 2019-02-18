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
#include "Program.hpp"

namespace RandomX {



	class VirtualMachine {
	public:
		VirtualMachine();
		virtual ~VirtualMachine() {}
		virtual void setDataset(dataset_t ds) = 0;
		void setScratchpad(void* ptr) {
			scratchpad = (uint8_t*)ptr;
		}
		void resetRoundingMode();
		virtual void initialize();
		virtual void execute() = 0;
		template<bool softAes>
		void getResult(void* scratchpad, size_t scratchpadSize, void* outHash);
		const RegisterFile& getRegisterFile() {
			return reg;
		}
		Program* getProgramBuffer() {
			return &program;
		}
	protected:
		alignas(64) Program program;
		alignas(64) RegisterFile reg;
		MemoryRegisters mem;
		uint8_t* scratchpad;
		uint32_t readReg0, readReg1, readReg2, readReg3;
	};
}