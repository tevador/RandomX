/*
Copyright (c) 2019, tevador <tevador@gmail.com>

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

#include "container.hpp"
#include "allocator.hpp"
#include "virtual_machine.hpp"
#include "virtual_memory.hpp"
#include "dataset.hpp"
#include "vm_interpreted.hpp"
#include "vm_compiled.hpp"
#include <new>
#include <memory>
#include <cstdint>
#include <cassert>
#include <limits>
#include <iostream>

namespace randomx {

	static void* allocStatic(size_t bytes, size_t align, void*& bufferHead, size_t& bufferCapacity) {
		auto ptr = std::align(align, bytes, bufferHead, bufferCapacity);
		assert(ptr != nullptr);
		void* memory = bufferHead;
		bufferHead = (uint8_t*)bufferHead + bytes;
		bufferCapacity -= bytes;
		//std::cout << "Container alloc: " << bytes << ", remaining: " << bufferCapacity << std::endl;
		return memory;
	}

	template<class T>
	static T* constructStatic(void*& bufferHead, size_t& bufferCapacity) {
		return new(allocStatic(sizeof(T), alignof(T), bufferHead, bufferCapacity)) T();
	}

	static void addAlign(uint64_t& size, size_t add, size_t align) {
		size = alignSize(size, align);
		size += add;
	}

	template<class T>
	static void addAlign(uint64_t& size) {
		addAlign(size, sizeof(T), alignof(T));
	}

	static uint64_t calculateSize(randomx_flags flags, size_t vmCount) {
		uint64_t size = 0;
		uint64_t align = randomx::CacheLineSize;
		addAlign<Container>(size);
		addAlign<randomx_container_data>(size);
		if (flags & RANDOMX_FLAG_FULL_MEM) {
			addAlign<randomx_cache>(size);
			if (flags & RANDOMX_FLAG_JIT) {
				addAlign<randomx::JitCompiler>(size);
			}
		}
		if ((flags & RANDOMX_FLAG_LARGE_PAGES) && !(flags & RANDOMX_FLAG_MONSTER_PAGES)) {
			align = 2 * 1024 * 1024;
		}
		addAlign<randomx_dataset>(size);
		addAlign(size, sizeof(uintptr_t) * vmCount, alignof(uintptr_t));
		for (size_t i = 0; i < vmCount; ++i) {
			randomx_vm* vm;
			if (flags & RANDOMX_FLAG_JIT) {
				if (flags & RANDOMX_FLAG_HARD_AES) {
					if (flags & RANDOMX_FLAG_SECURE) {
						addAlign<randomx::CompiledVmContainerHardAesSecure>(size);
					}
					else {
						addAlign<randomx::CompiledVmContainerHardAes>(size);
					}
				}
				else {
					if (flags & RANDOMX_FLAG_SECURE) {
						addAlign<randomx::CompiledVmContainerSecure>(size);
					}
					else {
						addAlign<randomx::CompiledVmContainer>(size);
					}
				}
			}
			else {
				if (flags & RANDOMX_FLAG_HARD_AES) {
					addAlign<randomx::InterpretedVmContainerHardAes>(size);
				}
				else {
					addAlign<randomx::InterpretedVmContainer>(size);
				}
			}
		}
		for (size_t i = 0; i < vmCount; ++i) {
			addAlign(size, randomx::ScratchpadSize, align);
		}
		if (flags & RANDOMX_FLAG_FULL_MEM) {
			addAlign(size, randomx::CacheSize, align);
		}
		addAlign(size, randomx::DatasetSize, align);
		return size;
	}

	template<class Allocator>
	Container* Container::create(randomx_flags flags, size_t vmCount) {
		uint64_t bufferSize = calculateSize(flags, vmCount);
		//std::cout << "Container size: " << bufferSize << std::endl;
		if (bufferSize > std::numeric_limits<size_t>::max()) {
			throw std::bad_alloc();
		}
		size_t bufferCapacity = bufferSize;
		void* buffer = Allocator::allocMemory(bufferSize);
		void* bufferHead = buffer;
		auto* container = constructStatic<Container>(bufferHead, bufferCapacity);
		auto* data = constructStatic<randomx_container_data>(bufferHead, bufferCapacity);
		data->buffer = buffer;
		data->bufferSize = bufferSize;
		data->bufferHead = bufferHead;
		data->bufferCapacity = bufferCapacity;
		data->dealloc = &Allocator::freeMemory;
		container->data = data;
		return container;
	}

	template Container* Container::create<MonsterPageAllocator>(randomx_flags flags, size_t vmCount);
	template Container* Container::create<LargePageAllocator>(randomx_flags flags, size_t vmCount);
	template Container* Container::create<DefaultAllocator>(randomx_flags flags, size_t vmCount);

	void* Container::alloc(size_t bytes, size_t align) {
		return allocStatic(bytes, align, data->bufferHead, data->bufferCapacity);
	}

	void Container::dealloc() {
		if (cache != nullptr) {
			cache->dealloc(cache);
		}
		for (size_t i = 0; i < vmCount; ++i) {
			vms[i]->~randomx_vm();
		}
		data->dealloc(data->buffer, data->bufferSize);
	}
	
}
