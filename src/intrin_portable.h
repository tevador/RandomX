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
#include "blake2/endian.h"

constexpr int32_t unsigned32ToSigned2sCompl(uint32_t x) {
	return (-1 == ~0) ? (int32_t)x : (x > INT32_MAX ? (-(int32_t)(UINT32_MAX - x) - 1) : (int32_t)x);
}

constexpr int64_t unsigned64ToSigned2sCompl(uint64_t x) {
	return (-1 == ~0) ? (int64_t)x : (x > INT64_MAX ? (-(int64_t)(UINT64_MAX - x) - 1) : (int64_t)x);
}

constexpr uint64_t signExtend2sCompl(uint32_t x) {
	return (-1 == ~0) ? (int64_t)(int32_t)(x) : (x > INT32_MAX ? (x | 0xffffffff00000000ULL) : (uint64_t)x);
}

constexpr int RoundToNearest = 0;
constexpr int RoundDown = 1;
constexpr int RoundUp = 2;
constexpr int RoundToZero = 3;

#if defined(_MSC_VER)
#if defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP == 2)
#define __SSE2__ 1
#endif
#endif

#ifdef __SSE2__
#ifdef __GNUC__
#include <x86intrin.h>
#else
#include <intrin.h>
#endif

typedef __m128i rx_vec_i128;
typedef __m128d rx_vec_f128;

#define rx_aligned_alloc(a, b) _mm_malloc(a,b)
#define rx_aligned_free(a) _mm_free(a)
#define rx_prefetch_nta(x) _mm_prefetch((const char *)(x), _MM_HINT_NTA)

#define rx_load_vec_f128 _mm_load_pd
#define rx_store_vec_f128 _mm_store_pd
#define rx_shuffle_vec_f128 _mm_shuffle_pd
#define rx_add_vec_f128 _mm_add_pd
#define rx_sub_vec_f128 _mm_sub_pd
#define rx_mul_vec_f128 _mm_mul_pd
#define rx_div_vec_f128 _mm_div_pd
#define rx_sqrt_vec_f128 _mm_sqrt_pd
#define rx_set1_long_vec_i128 _mm_set1_epi64x
#define rx_vec_i128_vec_f128 _mm_castsi128_pd

FORCE_INLINE rx_vec_f128 rx_set_vec_f128(uint64_t x1, uint64_t x0) {
	return _mm_castsi128_pd(_mm_set_epi64x(x1, x0));
}

FORCE_INLINE rx_vec_f128 rx_set1_vec_f128(uint64_t x) {
	return _mm_castsi128_pd(_mm_set1_epi64x(x));
}

#define rx_xor_vec_f128 _mm_xor_pd
#define rx_and_vec_f128 _mm_and_pd
#define rx_or_vec_f128 _mm_or_pd
#define rx_aesenc_vec_i128 _mm_aesenc_si128
#define rx_aesdec_vec_i128 _mm_aesdec_si128

FORCE_INLINE int rx_vec_i128_x(rx_vec_i128 a) {
	return _mm_cvtsi128_si32(a);
}

FORCE_INLINE int rx_vec_i128_y(rx_vec_i128 a) {
	return _mm_cvtsi128_si32(_mm_shuffle_epi32(a, 0x55));
}

FORCE_INLINE int rx_vec_i128_z(rx_vec_i128 a) {
	return _mm_cvtsi128_si32(_mm_shuffle_epi32(a, 0xaa));
}

FORCE_INLINE int rx_vec_i128_w(rx_vec_i128 a) {
	return _mm_cvtsi128_si32(_mm_shuffle_epi32(a, 0xff));
}

#define rx_set_int_vec_i128 _mm_set_epi32
#define rx_xor_vec_i128 _mm_xor_si128
#define rx_load_vec_i128 _mm_load_si128
#define rx_store_vec_i128 _mm_store_si128

FORCE_INLINE rx_vec_f128 rx_cvt_packed_int_vec_f128(const void* addr) {
	__m128i ix = _mm_loadl_epi64((const __m128i*)addr);
	return _mm_cvtepi32_pd(ix);
}

constexpr uint32_t rx_mxcsr_default = 0x9FC0; //Flush to zero, denormals are zero, default rounding mode, all exceptions disabled

FORCE_INLINE void rx_reset_float_state() {
	_mm_setcsr(rx_mxcsr_default);
}

FORCE_INLINE void rx_set_rounding_mode(uint32_t mode) {
	_mm_setcsr(rx_mxcsr_default | (mode << 13));
}

#else
#include <cstdint>
#include <stdexcept>
#include <cstdlib>
#include <cmath>

typedef union {
	uint64_t u64[2];
	uint32_t u32[4];
	uint16_t u16[8];
	uint8_t u8[16];
} rx_vec_i128;

typedef union {
	struct {
		double lo;
		double hi;
	};
	rx_vec_i128 i;
} rx_vec_f128;

#define rx_aligned_alloc(a, b) malloc(a)
#define rx_aligned_free(a) free(a)
#define rx_prefetch_nta(x)

FORCE_INLINE rx_vec_f128 rx_load_vec_f128(const double* pd) {
	rx_vec_f128 x;
	x.i.u64[0] = load64(pd + 0);
	x.i.u64[1] = load64(pd + 1);
	return x;
}

FORCE_INLINE void rx_store_vec_f128(double* mem_addr, rx_vec_f128 a) {
	store64(mem_addr + 0, a.i.u64[0]);
	store64(mem_addr + 1, a.i.u64[1]);
}

FORCE_INLINE rx_vec_f128 rx_shuffle_vec_f128(rx_vec_f128 a, rx_vec_f128 b, int imm8) {
	rx_vec_f128 x;
	x.lo = (imm8 & 1) ? a.hi : a.lo;
	x.hi = (imm8 & 2) ? b.hi : b.lo;
	return x;
}

FORCE_INLINE rx_vec_f128 rx_add_vec_f128(rx_vec_f128 a, rx_vec_f128 b) {
	rx_vec_f128 x;
	x.lo = a.lo + b.lo;
	x.hi = a.hi + b.hi;
	return x;
}

FORCE_INLINE rx_vec_f128 rx_sub_vec_f128(rx_vec_f128 a, rx_vec_f128 b) {
	rx_vec_f128 x;
	x.lo = a.lo - b.lo;
	x.hi = a.hi - b.hi;
	return x;
}

FORCE_INLINE rx_vec_f128 rx_mul_vec_f128(rx_vec_f128 a, rx_vec_f128 b) {
	rx_vec_f128 x;
	x.lo = a.lo * b.lo;
	x.hi = a.hi * b.hi;
	return x;
}

FORCE_INLINE rx_vec_f128 rx_div_vec_f128(rx_vec_f128 a, rx_vec_f128 b) {
	rx_vec_f128 x;
	x.lo = a.lo / b.lo;
	x.hi = a.hi / b.hi;
	return x;
}

FORCE_INLINE rx_vec_f128 rx_sqrt_vec_f128(rx_vec_f128 a) {
	rx_vec_f128 x;
	x.lo = sqrt(a.lo);
	x.hi = sqrt(a.hi);
	return x;
}

FORCE_INLINE rx_vec_i128 rx_set1_long_vec_i128(uint64_t a) {
	rx_vec_i128 x;
	x.u64[0] = a;
	x.u64[1] = a;
	return x;
}

FORCE_INLINE rx_vec_f128 rx_vec_i128_vec_f128(rx_vec_i128 a) {
	rx_vec_f128 x;
	x.i = a;
	return x;
}

FORCE_INLINE rx_vec_f128 rx_set_vec_f128(uint64_t x1, uint64_t x0) {
	rx_vec_f128 v;
	v.i.u64[0] = x0;
	v.i.u64[1] = x1;
	return v;
}

FORCE_INLINE rx_vec_f128 rx_set1_vec_f128(uint64_t x) {
	rx_vec_f128 v;
	v.i.u64[0] = x;
	v.i.u64[1] = x;
	return v;
}


FORCE_INLINE rx_vec_f128 rx_xor_vec_f128(rx_vec_f128 a, rx_vec_f128 b) {
	rx_vec_f128 x;
	x.i.u64[0] = a.i.u64[0] ^ b.i.u64[0];
	x.i.u64[1] = a.i.u64[1] ^ b.i.u64[1];
	return x;
}

FORCE_INLINE rx_vec_f128 rx_and_vec_f128(rx_vec_f128 a, rx_vec_f128 b) {
	rx_vec_f128 x;
	x.i.u64[0] = a.i.u64[0] & b.i.u64[0];
	x.i.u64[1] = a.i.u64[1] & b.i.u64[1];
	return x;
}

FORCE_INLINE rx_vec_f128 rx_or_vec_f128(rx_vec_f128 a, rx_vec_f128 b) {
	rx_vec_f128 x;
	x.i.u64[0] = a.i.u64[0] | b.i.u64[0];
	x.i.u64[1] = a.i.u64[1] | b.i.u64[1];
	return x;
}

static const char* platformError = "Platform doesn't support hardware AES";

FORCE_INLINE rx_vec_i128 rx_aesenc_vec_i128(rx_vec_i128 v, rx_vec_i128 rkey) {
	throw std::runtime_error(platformError);
}

FORCE_INLINE rx_vec_i128 rx_aesdec_vec_i128(rx_vec_i128 v, rx_vec_i128 rkey) {
	throw std::runtime_error(platformError);
}

FORCE_INLINE int rx_vec_i128_x(rx_vec_i128 a) {
	return a.u32[0];
}

FORCE_INLINE int rx_vec_i128_y(rx_vec_i128 a) {
	return a.u32[1];
}

FORCE_INLINE int rx_vec_i128_z(rx_vec_i128 a) {
	return a.u32[2];
}

FORCE_INLINE int rx_vec_i128_w(rx_vec_i128 a) {
	return a.u32[3];
}

FORCE_INLINE rx_vec_i128 rx_set_int_vec_i128(int _I3, int _I2, int _I1, int _I0) {
	rx_vec_i128 v;
	v.u32[0] = _I0;
	v.u32[1] = _I1;
	v.u32[2] = _I2;
	v.u32[3] = _I3;
	return v;
};

FORCE_INLINE rx_vec_i128 rx_xor_vec_i128(rx_vec_i128 _A, rx_vec_i128 _B) {
	rx_vec_i128 c;
	c.u32[0] = _A.u32[0] ^ _B.u32[0];
	c.u32[1] = _A.u32[1] ^ _B.u32[1];
	c.u32[2] = _A.u32[2] ^ _B.u32[2];
	c.u32[3] = _A.u32[3] ^ _B.u32[3];
	return c;
}

FORCE_INLINE rx_vec_i128 rx_load_vec_i128(rx_vec_i128 const*_P) {
#if defined(NATIVE_LITTLE_ENDIAN)
	return *_P;
#else
	uint32_t* ptr = (uint32_t*)_P;
	rx_vec_i128 c;
	c.u32[0] = load32(ptr + 0);
	c.u32[1] = load32(ptr + 1);
	c.u32[2] = load32(ptr + 2);
	c.u32[3] = load32(ptr + 3);
	return c;
#endif
}

FORCE_INLINE void rx_store_vec_i128(rx_vec_i128 *_P, rx_vec_i128 _B) {
#if defined(NATIVE_LITTLE_ENDIAN)
	*_P = _B;
#else
	uint32_t* ptr = (uint32_t*)_P;
	store32(ptr + 0, _B.u32[0]);
	store32(ptr + 1, _B.u32[1]);
	store32(ptr + 2, _B.u32[2]);
	store32(ptr + 3, _B.u32[3]);
#endif
}

FORCE_INLINE rx_vec_f128 rx_cvt_packed_int_vec_f128(const void* addr) {
	rx_vec_f128 x;
	x.lo = (double)unsigned32ToSigned2sCompl(load32((uint8_t*)addr + 0));
	x.hi = (double)unsigned32ToSigned2sCompl(load32((uint8_t*)addr + 4));
	return x;
}

#define RANDOMX_DEFAULT_FENV

void rx_reset_float_state();

void rx_set_rounding_mode(uint32_t mode);

#endif

double loadDoublePortable(const void* addr);
uint64_t mulh(uint64_t, uint64_t);
int64_t smulh(int64_t, int64_t);
uint64_t rotl(uint64_t, int);
uint64_t rotr(uint64_t, int);
