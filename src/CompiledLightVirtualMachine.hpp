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

#pragma once
//#define TRACEVM
#include <new>
#include "CompiledVirtualMachine.hpp"
#include "JitCompilerX86.hpp"
#include "intrinPortable.h"

namespace RandomX {

	template<bool superscalar>
	class CompiledLightVirtualMachine : public CompiledVirtualMachine {
	public:
		void* operator new(size_t size) {
			void* ptr = _mm_malloc(size, 64);
			if (ptr == nullptr)
				throw std::bad_alloc();
			return ptr;
		}
		void operator delete(void* ptr) {
			_mm_free(ptr);
		}
		CompiledLightVirtualMachine() {}
		void setDataset(dataset_t ds, uint64_t size, LightProgram(&programs)[RANDOMX_CACHE_ACCESSES]) override;
		void initialize() override;
	};
}