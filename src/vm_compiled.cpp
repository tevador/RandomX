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

#include "vm_compiled.hpp"
#include "common.hpp"

namespace randomx {

	static_assert(sizeof(MemoryRegisters) == 2 * sizeof(addr_t) + sizeof(uintptr_t), "Invalid alignment of struct randomx::MemoryRegisters");
	static_assert(sizeof(RegisterFile) == 256, "Invalid alignment of struct randomx::RegisterFile");

	template<class Allocator, bool softAes, bool secureJit>
	CompiledVm<Allocator, softAes, secureJit>::CompiledVm() {
		if (!secureJit) {
			compiler.enableAll(); //make JIT buffer both writable and executable
		}
	}

	template<class Allocator, bool softAes, bool secureJit>
	void CompiledVm<Allocator, softAes, secureJit>::setDataset(randomx_dataset* dataset) {
		datasetPtr = dataset;
	}

	template<class Allocator, bool softAes, bool secureJit>
	void CompiledVm<Allocator, softAes, secureJit>::run(void* seed) {
		VmBase<Allocator, softAes>::generateProgram(seed);
		randomx_vm::initialize();
		if (secureJit) {
			compiler.enableWriting();
		}
		compiler.generateProgram(program, config);
		if (secureJit) {
			compiler.enableExecution();
		}
		mem.memory = datasetPtr->memory + datasetOffset;
		execute();
	}

	template<class Allocator, bool softAes, bool secureJit>
	void CompiledVm<Allocator, softAes, secureJit>::execute() {
#ifdef __aarch64__
		memcpy(reg.f, config.eMask, sizeof(config.eMask));
#endif
		compiler.getProgramFunc()(reg, mem, scratchpad, RANDOMX_PROGRAM_ITERATIONS);
	}

	template class CompiledVm<AlignedAllocator<CacheLineSize>, false, false>;
	template class CompiledVm<AlignedAllocator<CacheLineSize>, true, false>;
	template class CompiledVm<LargePageAllocator, false, false>;
	template class CompiledVm<LargePageAllocator, true, false>;
	template class CompiledVm<AlignedAllocator<CacheLineSize>, false, true>;
	template class CompiledVm<AlignedAllocator<CacheLineSize>, true, true>;
	template class CompiledVm<LargePageAllocator, false, true>;
	template class CompiledVm<LargePageAllocator, true, true>;

	template class CompiledVm<NullAllocator, false, false>;
	template class CompiledVm<NullAllocator, true, false>;
	template class CompiledVm<NullAllocator, false, true>;
	template class CompiledVm<NullAllocator, true, true>;
}