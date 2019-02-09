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

inline __m128d _mm_abs(__m128d xd) {
	const __m128d absmask = _mm_castsi128_pd(_mm_set1_epi64x(~(1LL << 63)));
	return _mm_and_pd(xd, absmask);
}

#define PREFETCHNTA(x) _mm_prefetch((const char *)(x), _MM_HINT_NTA)

#else
#include <cstdint>
#include <stdexcept>

#define _mm_malloc(a,b) malloc(a)
#define _mm_free(a) free(a)
#define PREFETCHNTA(x)

typedef union {
	uint64_t u64[2];
	uint32_t u32[4];
	uint16_t u16[8];
	uint8_t u8[16];
} __m128i;

typedef struct {
	double lo;
	double hi;
} __m128d;

inline __m128d _mm_load_pd(const double* pd) {
	__m128d x;
	x.lo = *(pd + 0);
	x.hi = *(pd + 1);
	return x;
}

static const char* platformError = "Platform doesn't support hardware AES";

inline __m128i _mm_aeskeygenassist_si128(__m128i key, uint8_t rcon) {
	throw std::runtime_error(platformError);
}

inline __m128i _mm_aesenc_si128(__m128i v, __m128i rkey) {
	throw std::runtime_error(platformError);
}

inline __m128i _mm_aesdec_si128(__m128i v, __m128i rkey) {
	throw std::runtime_error(platformError);
}

inline int _mm_cvtsi128_si32(__m128i v) {
	return v.u32[0];
}

inline __m128i _mm_cvtsi32_si128(int si32) {
	__m128i v;
	v.u32[0] = si32;
	v.u32[1] = 0;
	v.u32[2] = 0;
	v.u32[3] = 0;
	return v;
}

inline  __m128i _mm_set_epi64x(int64_t _I1, int64_t _I0) {
	__m128i v;
	v.u64[0] = _I0;
	v.u64[1] = _I1;
	return v;
}

inline __m128i _mm_set_epi32(int _I3, int _I2, int _I1, int _I0) {
	__m128i v;
	v.u32[0] = _I0;
	v.u32[1] = _I1;
	v.u32[2] = _I2;
	v.u32[3] = _I3;
	return v;
};

inline __m128i _mm_xor_si128(__m128i _A, __m128i _B) {
	__m128i c;
	c.u32[0] = _A.u32[0] ^ _B.u32[0];
	c.u32[1] = _A.u32[1] ^ _B.u32[1];
	c.u32[2] = _A.u32[2] ^ _B.u32[2];
	c.u32[3] = _A.u32[3] ^ _B.u32[3];
	return c;
}

inline __m128i _mm_shuffle_epi32(__m128i _A, int _Imm) {
	__m128i c;
	c.u32[0] = _A.u32[_Imm & 3];
	c.u32[1] = _A.u32[(_Imm >> 2) & 3];
	c.u32[2] = _A.u32[(_Imm >> 4) & 3];
	c.u32[3] = _A.u32[(_Imm >> 6) & 3];
	return c;
}

inline __m128i _mm_load_si128(__m128i const*_P) {
	return *_P;
}

inline void _mm_store_si128(__m128i *_P, __m128i _B) {
	*_P = _B;
}

inline __m128i _mm_slli_si128(__m128i _A, int _Imm) {
	_Imm &= 255;
	if (_Imm > 15) {
		_A.u64[0] = 0;
		_A.u64[1] = 0;
	}
	else {
		for (int i = 15; i >= _Imm; --i) {
			_A.u8[i] = _A.u8[i - _Imm];
		}
		for (int i = 0; i < _Imm; ++i) {
			_A.u8[i] = 0;
		}
	}
	return _A;
}

#endif

constexpr int RoundToNearest = 0;
constexpr int RoundDown = 1;
constexpr int RoundUp = 2;
constexpr int RoundToZero = 3;

constexpr int32_t unsigned32ToSigned2sCompl(uint32_t x) {
	return (-1 == ~0) ? (int32_t)x : (x > INT32_MAX ? (-(int32_t)(UINT32_MAX - x) - 1) : (int32_t)x);
}

constexpr int64_t unsigned64ToSigned2sCompl(uint64_t x) {
	return (-1 == ~0) ? (int64_t)x : (x > INT64_MAX ? (-(int64_t)(UINT64_MAX - x) - 1) : (int64_t)x);
}

constexpr uint64_t signExtend2sCompl(uint32_t x) {
	return (-1 == ~0) ? (int64_t)(int32_t)(x) : (x > INT32_MAX ? (x | 0xffffffff00000000ULL) : (uint64_t)x);
}

inline __m128d load_cvt_i32x2(const void* addr) {
	__m128i ix = _mm_load_si128((const __m128i*)addr);
	return _mm_cvtepi32_pd(ix);
}

double loadDoublePortable(const void* addr);

uint64_t mulh(uint64_t, uint64_t);
int64_t smulh(int64_t, int64_t);
uint64_t rotl(uint64_t, int);
uint64_t rotr(uint64_t, int);
void initFpu();
void setRoundMode(uint32_t);
bool condition(uint32_t, uint32_t, uint32_t);
