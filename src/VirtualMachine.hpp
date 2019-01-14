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

namespace RandomX {

	class VirtualMachine {
	public:
		VirtualMachine();
		virtual ~VirtualMachine() {}
		virtual void setDataset(dataset_t ds) = 0;
		virtual void initializeScratchpad(uint32_t index) = 0;
		virtual void initializeProgram(const void* seed) = 0;
		virtual void execute() = 0;
		void getResult(void*);
		const RegisterFile& getRegisterFile() {
			return reg;
		}
	protected:
		DatasetReadFunc readDataset;
		alignas(16) RegisterFile reg;
		MemoryRegisters mem;
		alignas(64) convertible_t scratchpad[ScratchpadLength];
	};
}