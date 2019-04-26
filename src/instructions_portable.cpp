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

#pragma STDC FENV_ACCESS ON
#include <cfenv>
#include <cmath>
#ifdef DEBUG
#include <iostream>
#endif
#include "common.hpp"
#include "intrin_portable.h"
#include "blake2/endian.h"

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

	static void setRoundMode_(uint32_t mode) {
		_controlfp(mode, _MCW_RC);
	}
	#define HAVE_SETROUNDMODE_IMPL
#endif

#ifndef HAVE_SETROUNDMODE_IMPL
	static void setRoundMode_(uint32_t mode) {
		fesetround(mode);
	}
#endif

#ifndef HAVE_ROTR
	uint64_t rotr(uint64_t a, int b) {
		return (a >> b) | (a << (64 - b));
	}
	#define HAVE_ROTR
#endif

#ifndef HAVE_ROTL
	uint64_t rotl(uint64_t a, int b) {
		return (a << b) | (a >> (64 - b));
	}
	#define HAVE_ROTL
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

#if __GNUC__ >= 5
#undef __has_builtin
#define __has_builtin(x) 1
#endif

#if defined(__has_builtin)
#if __has_builtin(__builtin_sub_overflow)
	static inline bool subOverflow_(uint32_t a, uint32_t b) {
		int32_t temp;
		return __builtin_sub_overflow(unsigned32ToSigned2sCompl(a), unsigned32ToSigned2sCompl(b), &temp);
	}
	#define HAVE_SUB_OVERFLOW
#endif
#endif

#ifndef HAVE_SUB_OVERFLOW
	static inline bool subOverflow_(uint32_t a, uint32_t b) {
		auto c = unsigned32ToSigned2sCompl(a - b);
		return (c < unsigned32ToSigned2sCompl(a)) != (unsigned32ToSigned2sCompl(b) > 0);
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
			setRoundMode_(FE_DOWNWARD);
			break;
		case RoundUp:
			setRoundMode_(FE_UPWARD);
			break;
		case RoundToZero:
			setRoundMode_(FE_TOWARDZERO);
			break;
		case RoundToNearest:
			setRoundMode_(FE_TONEAREST);
			break;
		default:
			UNREACHABLE;
	}
}

bool condition(uint32_t type, uint32_t value, uint32_t imm32) {
	switch (type & 7)
	{
		case 0:
			return value <= imm32;
		case 1:
			return value > imm32;
		case 2:
			return unsigned32ToSigned2sCompl(value - imm32) < 0;
		case 3:
			return unsigned32ToSigned2sCompl(value - imm32) >= 0;
		case 4:
			return subOverflow_(value, imm32);
		case 5:
			return !subOverflow_(value, imm32);
		case 6:
			return unsigned32ToSigned2sCompl(value) < unsigned32ToSigned2sCompl(imm32);
		case 7:
			return unsigned32ToSigned2sCompl(value) >= unsigned32ToSigned2sCompl(imm32);
		default:
			UNREACHABLE;
	}
}

void initFpu() {
#ifdef __SSE2__
	_mm_setcsr(0x9FC0); //Flush to zero, denormals are zero, default rounding mode, all exceptions disabled
#else
	setRoundMode(FE_TONEAREST);
#endif
}

union double_ser_t {
	double f;
	uint64_t i;
};

double loadDoublePortable(const void* addr) {
	double_ser_t ds;
	ds.i = load64(addr);
	return ds.f;
}
