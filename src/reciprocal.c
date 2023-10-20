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

#include <assert.h>
#include "reciprocal.h"

/*
	Calculates rcp = 2**x / divisor for highest integer x such that rcp < 2**64.
	divisor must not be 0 or a power of 2

	Equivalent x86 assembly (divisor in rcx):

	mov edx, 1
	mov r8, rcx
	xor eax, eax
	bsr rcx, rcx
	shl rdx, cl
	div r8
	ret

*/
uint64_t randomx_reciprocal(uint32_t divisor) {

	assert(divisor != 0);

	const uint64_t p2exp63 = 1ULL << 63;
	const uint64_t q = p2exp63 / divisor;
	const uint64_t r = p2exp63 % divisor;

#ifdef __GNUC__
	const uint32_t shift = 64 - __builtin_clzll(divisor);
#else
	uint32_t shift = 32;
	for (uint32_t k = 1U << 31; (k & divisor) == 0; k >>= 1)
		--shift;
#endif

	return (q << shift) + ((r << shift) / divisor);
}

#if !RANDOMX_HAVE_FAST_RECIPROCAL

uint64_t randomx_reciprocal_fast(uint32_t divisor) {
	return randomx_reciprocal(divisor);
}

#endif
