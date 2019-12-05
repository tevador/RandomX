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

#pragma once

#include "randomx.h"
#include <type_traits>

namespace randomx {

	typedef void(DeallocFunc)(void*, size_t);

}

struct randomx_container_data {
	void* buffer;
	void* bufferHead;
	size_t bufferSize;
	size_t bufferCapacity;
	randomx::DeallocFunc* dealloc;
};

namespace randomx {

	struct Container : public randomx_container {
		template<class Allocator>
		static Container* create(randomx_flags flags, size_t vmCount);
		void* alloc(size_t bytes, size_t align = sizeof(uintptr_t));
		template<typename T>
		void* alloc() {
			return alloc(sizeof(T), alignof(T));
		}
		template<class T>
		T* construct() {
			return new(alloc<T>()) T();
		}
		template<class T>
		T* construct(size_t numElements) {
			return new(alloc(numElements * sizeof(T), alignof(T))) T[numElements];
		}
		void dealloc();
	};

	static_assert(std::is_standard_layout<Container>(), "Container must be a standard-layout struct");
}
