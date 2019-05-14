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

#pragma STDC FENV_ACCESS ON
#include <cfenv>
#include <cmath>
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

#ifdef RANDOMX_DEFAULT_FENV

void rx_reset_float_state() {
	setRoundMode_(FE_TONEAREST);
}

void rx_set_rounding_mode(uint32_t mode) {
	switch (mode & 3) {
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

#endif

union double_ser_t {
	double f;
	uint64_t i;
};

double loadDoublePortable(const void* addr) {
	double_ser_t ds;
	ds.i = load64(addr);
	return ds.f;
}
