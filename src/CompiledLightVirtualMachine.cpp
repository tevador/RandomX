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

	template<bool superscalar>
	void CompiledLightVirtualMachine<superscalar>::setDataset(dataset_t ds, uint64_t size, SuperscalarProgram(&programs)[RANDOMX_CACHE_ACCESSES]) {
		mem.ds = ds;
		datasetRange = (size - RANDOMX_DATASET_SIZE + CacheLineSize) / CacheLineSize;
		if(superscalar)
			compiler.generateSuperScalarHash(programs);
		//datasetBasePtr = ds.dataset.memory;
	}

	template void CompiledLightVirtualMachine<true>::setDataset(dataset_t ds, uint64_t size, SuperscalarProgram(&programs)[RANDOMX_CACHE_ACCESSES]);
	template void CompiledLightVirtualMachine<false>::setDataset(dataset_t ds, uint64_t size, SuperscalarProgram(&programs)[RANDOMX_CACHE_ACCESSES]);

	template<bool superscalar>
	void CompiledLightVirtualMachine<superscalar>::initialize() {
		VirtualMachine::initialize();
		compiler.generateProgramLight<superscalar>(program);
		//mem.ds.dataset.memory = datasetBasePtr + (datasetBase * CacheLineSize);
	}

	template void CompiledLightVirtualMachine<true>::initialize();
	template void CompiledLightVirtualMachine<false>::initialize();
}