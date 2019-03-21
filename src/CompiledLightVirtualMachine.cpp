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

#include "CompiledLightVirtualMachine.hpp"
#include "common.hpp"
#include <stdexcept>

namespace RandomX {

	CompiledLightVirtualMachine::CompiledLightVirtualMachine() {
	}

	void CompiledLightVirtualMachine::setDataset(dataset_t ds, uint64_t size) {
		mem.ds = ds;
		datasetRange = (size - RANDOMX_DATASET_SIZE + CacheLineSize) / CacheLineSize;
		//datasetBasePtr = ds.dataset.memory;
	}

	void CompiledLightVirtualMachine::initialize() {
		VirtualMachine::initialize();
		compiler.generateProgramLight(program);
		//mem.ds.dataset.memory = datasetBasePtr + (datasetBase * CacheLineSize);
	}
}