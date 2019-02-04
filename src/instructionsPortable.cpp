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
//#define DEBUG
#include "intrinPortable.h"
#pragma STDC FENV_ACCESS on
#include <cfenv>
#include <cmath>
#ifdef DEBUG
#include <iostream>
#endif

#if defined(__SIZEOF_INT128__)
	typedef unsigned __int128 uint128_t;
	typedef __int128 int128_t;
	uint64_t mulh(uint64_t a, uint64_t b) {
		return ((uint128_t)a * b) >> 64;
	}
	int64_t smulh(int64_t a, int64_t b) {
		return ((int128_t)a * b) >> 64;
	}
	#define HAVE_MULH
	#define HAVE_SMULH
#endif

#if defined(_MSC_VER)
	#define HAS_VALUE(X) X ## 0
	#define EVAL_DEFINE(X) HAS_VALUE(X)
	#include <intrin.h>
	#include <stdlib.h>

	uint64_t rotl(uint64_t x, int c) {
		return _rotl64(x, c);
	}
	uint64_t rotr(uint64_t x , int c) {
		return _rotr64(x, c);
	}
	#define HAVE_ROTL
	#define HAVE_ROTR

	#if EVAL_DEFINE(__MACHINEARM64_X64(1))
		uint64_t mulh(uint64_t a, uint64_t b) {
			return __umulh(a, b);
		}
		#define HAVE_MULH
	#endif

	#if EVAL_DEFINE(__MACHINEX64(1))
		int64_t smulh(int64_t a, int64_t b) {
			int64_t hi;
			_mul128(a, b, &hi);
			return hi;
		}
		#define HAVE_SMULH
	#endif

	static void setRoundMode__(uint32_t mode) {
		_controlfp(mode, _MCW_RC);
	}
	#define HAVE_SETROUNDMODE_IMPL
#endif

#ifndef HAVE_SETROUNDMODE_IMPL
	static void setRoundMode__(uint32_t mode) {
		fesetround(mode);
	}
#endif

#ifndef HAVE_ROTR
	uint64_t rotr(uint64_t a, int b) {
		return (a >> b) | (a << (64 - b));
	}
	#define HAS_ROTR
#endif

#ifndef HAVE_ROTL
	uint64_t rotl(uint64_t a, int b) {
		return (a << b) | (a >> (64 - b));
	}
	#define HAS_ROTL
#endif

#ifndef HAVE_MULH
	#define LO(x) ((x)&0xffffffff)
	#define HI(x) ((x)>>32)
	uint64_t mulh(uint64_t a, uint64_t b) {
		uint64_t ah = HI(a), al = LO(a);
		uint64_t bh = HI(b), bl = LO(b);
		uint64_t x00 = al * bl;
		uint64_t x01 = al * bh;
		uint64_t x10 = ah * bl;
		uint64_t x11 = ah * bh;
		uint64_t m1 = LO(x10) + LO(x01) + HI(x00);
		uint64_t m2 = HI(x10) + HI(x01) + LO(x11) + HI(m1);
		uint64_t m3 = HI(x11) + HI(m2);

		return (m3 << 32) + LO(m2);
	}
	#define HAVE_MULH
#endif

#ifndef HAVE_SMULH
	int64_t smulh(int64_t a, int64_t b) {
		int64_t hi = mulh(a, b);
		if (a < 0LL) hi -= b;
		if (b < 0LL) hi -= a;
		return hi;
	}
	#define HAVE_SMULH
#endif

// avoid undefined behavior of signed overflow
static inline int32_t safeSub(int32_t a, int32_t b) {
	return int32_t(uint32_t(a) - uint32_t(b));
}

#if __GNUC__ >= 5
#undef __has_builtin
#define __has_builtin(x) 1
#endif

#if defined(__has_builtin)
#if __has_builtin(__builtin_sub_overflow)
	static inline bool subOverflow__(int32_t a, int32_t b) {
		int32_t temp;
		return __builtin_sub_overflow(a, b, &temp);
	}
	#define HAVE_SUB_OVERFLOW
#endif
#endif

#ifndef HAVE_SUB_OVERFLOW
	static inline bool subOverflow__(int32_t a, int32_t b) {
		auto c = safeSub(a, b);
		return (c < a) != (b > 0);
	}
	#define HAVE_SUB_OVERFLOW
#endif

static inline double FlushDenormalNaN(double x) {
	int fpc = std::fpclassify(x);
	if (fpc == FP_SUBNORMAL || fpc == FP_NAN) {
		return 0.0;
	}
	return x;
}

static inline double FlushNaN(double x) {
	return x != x ? 0.0 : x;
}

void setRoundMode(uint32_t rcflag) {
	switch (rcflag & 3) {
	case RoundDown:
		setRoundMode__(FE_DOWNWARD);
		break;
	case RoundUp:
		setRoundMode__(FE_UPWARD);
		break;
	case RoundToZero:
		setRoundMode__(FE_TOWARDZERO);
		break;
	default:
		setRoundMode__(FE_TONEAREST);
		break;
	}
}

bool condition(uint32_t type, int32_t value, int32_t imm32) {
	switch (type & 7)
	{
	case 0:
		return (uint32_t)value <= (uint32_t)imm32;
	case 1:
		return (uint32_t)value > (uint32_t)imm32;
	case 2:
		return safeSub(value, imm32) < 0;
	case 3:
		return safeSub(value, imm32) >= 0;
	case 4:
		return subOverflow__(value, imm32);
	case 5:
		return !subOverflow__(value, imm32);
	case 6:
		return value < imm32;
	case 7:
		return value >= imm32;
	}
}

void initFpu() {
#ifdef __SSE2__
	_mm_setcsr(0x9FC0); //Flush to zero, denormals are zero, default rounding mode, all exceptions disabled
#else
	setRoundMode(FE_TONEAREST);
#endif
}

namespace RandomX {

	extern "C" {
		/*void DIV_64(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.u64 = a.u64 / (b.u32 != 0 ? b.u32 : 1U);
		}

		void IDIV_64(convertible_t& a, convertible_t& b, convertible_t& c) {
			if (a.i64 == INT64_MIN && b.i32 == -1)
				c.i64 = INT64_MIN;
			else
				c.i64 = a.i64 / (b.i32 != 0 ? b.i32 : 1);
		}

		void FPADD(convertible_t& a, fpu_reg_t& b, fpu_reg_t& c) {
#ifdef __SSE2__
			__m128i ai = _mm_loadl_epi64((const __m128i*)&a);
			__m128d ad = _mm_cvtepi32_pd(ai);
			__m128d bd = _mm_load_pd(&b.lo.f64);
			__m128d cd = _mm_add_pd(ad, bd);
			_mm_store_pd(&c.lo.f64, cd);
#else
			double alo = (double)a.i32lo;
			double ahi = (double)a.i32hi;
			c.lo.f64 = alo + b.lo.f64;
			c.hi.f64 = ahi + b.hi.f64;
#endif
		}

		void FPSUB(convertible_t& a, fpu_reg_t& b, fpu_reg_t& c) {
#ifdef __SSE2__
			__m128i ai = _mm_loadl_epi64((const __m128i*)&a);
			__m128d ad = _mm_cvtepi32_pd(ai);
			__m128d bd = _mm_load_pd(&b.lo.f64);
			__m128d cd = _mm_sub_pd(ad, bd);
			_mm_store_pd(&c.lo.f64, cd);
#else
			double alo = (double)a.i32lo;
			double ahi = (double)a.i32hi;
			c.lo.f64 = alo - b.lo.f64;
			c.hi.f64 = ahi - b.hi.f64;
#endif
		}

		void FPMUL(convertible_t& a, fpu_reg_t& b, fpu_reg_t& c) {
#ifdef __SSE2__
			__m128i ai = _mm_loadl_epi64((const __m128i*)&a);
			__m128d ad = _mm_cvtepi32_pd(ai);
			__m128d bd = _mm_load_pd(&b.lo.f64);
			__m128d cd = _mm_mul_pd(ad, bd);
			__m128d mask = _mm_cmpeq_pd(cd, cd);
			cd = _mm_and_pd(cd, mask);
			_mm_store_pd(&c.lo.f64, cd);
#else
			double alo = (double)a.i32lo;
			double ahi = (double)a.i32hi;
			c.lo.f64 = FlushNaN(alo * b.lo.f64);
			c.hi.f64 = FlushNaN(ahi * b.hi.f64);
#endif
		}

		void FPDIV(convertible_t& a, fpu_reg_t& b, fpu_reg_t& c) {
#ifdef __SSE2__
			__m128i ai = _mm_loadl_epi64((const __m128i*)&a);
			__m128d ad = _mm_cvtepi32_pd(ai);
			__m128d bd = _mm_load_pd(&b.lo.f64);
			__m128d cd = _mm_div_pd(ad, bd);
			__m128d mask = _mm_cmpeq_pd(cd, cd);
			cd = _mm_and_pd(cd, mask);
			_mm_store_pd(&c.lo.f64, cd);
#else
			double alo = (double)a.i32lo;
			double ahi = (double)a.i32hi;
			c.lo.f64 = FlushDenormalNaN(alo / b.lo.f64);
			c.hi.f64 = FlushDenormalNaN(ahi / b.hi.f64);
#endif
		}

		void FPSQRT(convertible_t& a, fpu_reg_t& b, fpu_reg_t& c) {
#ifdef __SSE2__
			__m128i ai = _mm_loadl_epi64((const __m128i*)&a);
			__m128d ad = _mm_cvtepi32_pd(ai);
			const __m128d absmask = _mm_castsi128_pd(_mm_set1_epi64x(~(1LL << 63)));
			ad = _mm_and_pd(ad, absmask);
			__m128d cd = _mm_sqrt_pd(ad);
			_mm_store_pd(&c.lo.f64, cd);
#else
			double alo = (double)a.i32lo;
			double ahi = (double)a.i32hi;
			c.lo.f64 = sqrt(std::abs(alo));
			c.hi.f64 = sqrt(std::abs(ahi));
#endif
		}*/


	}
}