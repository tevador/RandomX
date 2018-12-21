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
//#define FTZ
#include "instructions.hpp"
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
	static inline uint64_t __umulhi64(uint64_t a, uint64_t b) {
		return ((uint128_t)a * b) >> 64;
	}
	static inline uint64_t __imulhi64(int64_t a, int64_t b) {
		return ((int128_t)a * b) >> 64;
	}
	#define umulhi64 __umulhi64
	#define imulhi64 __imulhi64
#endif

#if defined(_MSC_VER)
	#define HAS_VALUE(X) X ## 0
	#define EVAL_DEFINE(X) HAS_VALUE(X)
	#include <intrin.h>
	#include <stdlib.h>
	#define ror64 _rotr64
	#define rol64 _rotl64
	#if EVAL_DEFINE(__MACHINEARM64_X64(1))
		#define umulhi64 __umulh
	#endif
	#if EVAL_DEFINE(__MACHINEX64(1))
		static inline uint64_t __imulhi64(int64_t a, int64_t b) {
			int64_t hi;
			_mul128(a, b, &hi);
			return hi;
		}
		#define imulhi64 __imulhi64
	#endif
	static inline uint32_t _setRoundMode(uint32_t mode) {
		return _controlfp(mode, _MCW_RC);
	}
	#define setRoundMode _setRoundMode
#endif

#ifndef setRoundMode
	#define setRoundMode fesetround
#endif

#ifndef ror64
	static inline uint64_t __ror64(uint64_t a, int b) {
		return (a >> b) | (a << (64 - b));
	}
	#define ror64 __ror64
#endif

#ifndef rol64
	static inline uint64_t __rol64(uint64_t a, int b) {
		return (a << b) | (a >> (64 - b));
	}
	#define rol64 __rol64
#endif

#ifndef sar64
	#include <type_traits>
	constexpr int64_t builtintShr64(int64_t value, int shift) noexcept {
		return value >> shift;
	}

	struct UsesArithmeticShift : std::integral_constant<bool, builtintShr64(-1LL, 1) == -1LL> {
	};

	static inline int64_t __sar64(int64_t a, int b) {
		return UsesArithmeticShift::value ? builtintShr64(a, b) : (a < 0 ? ~(~a >> b) : a >> b);
	}
	#define sar64 __sar64
#endif

#ifndef umulhi64
	#define LO(x) ((x)&0xffffffff)
	#define HI(x) ((x)>>32)
	static inline uint64_t __umulhi64(uint64_t a, uint64_t b) {
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
	#define umulhi64 __umulhi64
#endif

#ifndef imulhi64
	static inline int64_t __imulhi64(int64_t a, int64_t b) {
		int64_t hi = umulhi64(a, b);
		if (a < 0LL) hi -= b;
		if (b < 0LL) hi -= a;
		return hi;
	}
	#define imulhi64 __imulhi64
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
	static inline bool __subOverflow(int32_t a, int32_t b) {
		int32_t temp;
		return __builtin_sub_overflow(a, b, &temp);
	}
	#define subOverflow __subOverflow
#endif
#endif

#ifndef subOverflow
	static inline bool __subOverflow(int32_t a, int32_t b) {
		auto c = safeSub(a, b);
		return (c < a) != (b > 0);
	}
	#define subOverflow __subOverflow
#endif

static double FlushDenormal(double x) {
	if (std::fpclassify(x) == FP_SUBNORMAL) {
		return 0;
	}
	return x;
}

#ifdef FTZ
#undef FTZ
#define FTZ(x) FlushDenormal(x)
#else
#define FTZ(x) x
#endif

namespace RandomX {

	extern "C" {

		void ADD_64(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.u64 = a.u64 + b.u64;
		}

		void ADD_32(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.u64 = a.u32 + b.u32;
		}

		void SUB_64(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.u64 = a.u64 - b.u64;
		}

		void SUB_32(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.u64 = a.u32 - b.u32;
		}

		void MUL_64(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.u64 = a.u64 * b.u64;
		}

		void MULH_64(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.u64 = umulhi64(a.u64, b.u64);
		}

		void MUL_32(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.u64 = (uint64_t)a.u32 * b.u32;
		}

		void IMUL_32(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.i64 = (int64_t)a.i32 * b.i32;
		}

		void IMULH_64(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.i64 = imulhi64(a.i64, b.i64);
		}

		void DIV_64(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.u64 = a.u64 / (b.u32 != 0 ? b.u32 : 1U);
		}

		void IDIV_64(convertible_t& a, convertible_t& b, convertible_t& c) {
			if (a.i64 == INT64_MIN && b.i32 == -1)
				c.i64 = INT64_MIN;
			else
				c.i64 = a.i64 / (b.i32 != 0 ? b.i32 : 1);
		}

		void AND_64(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.u64 = a.u64 & b.u64;
		}

		void AND_32(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.u64 = a.u32 & b.u32;
		}

		void OR_64(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.u64 = a.u64 | b.u64;
		}

		void OR_32(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.u64 = a.u32 | b.u32;
		}

		void XOR_64(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.u64 = a.u64 ^ b.u64;
		}

		void XOR_32(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.u64 = a.u32 ^ b.u32;
		}

		void SHL_64(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.u64 = a.u64 << (b.u64 & 63);
		}

		void SHR_64(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.u64 = a.u64 >> (b.u64 & 63);
		}

		void SAR_64(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.u64 = sar64(a.i64, b.u64 & 63);
		}

		void ROL_64(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.u64 = rol64(a.u64, (b.u64 & 63));
		}

		void ROR_64(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.u64 = ror64(a.u64, (b.u64 & 63));
		}

		bool JMP_COND(uint8_t type, convertible_t& regb, int32_t imm32) {
			switch (type & 7)
			{
				case 0:
					return regb.u32 <= (uint32_t)imm32;
				case 1:
					return regb.u32 > (uint32_t)imm32;
				case 2:
					return safeSub(regb.i32, imm32) < 0;
				case 3:
					return safeSub(regb.i32, imm32) >= 0;
				case 4:
					return subOverflow(regb.i32, imm32);
				case 5:
					return !subOverflow(regb.i32, imm32);
				case 6:
					return regb.i32 < imm32;
				case 7:
					return regb.i32 >= imm32;
			}
		}

		void FPINIT() {
			setRoundMode(FE_TONEAREST);
		}

		void FPADD(convertible_t& a, double b, convertible_t& c) {
			c.f64 = FTZ(convertToDouble(a.i64) + b);
		}

		void FPSUB(convertible_t& a, double b, convertible_t& c) {
			c.f64 = FTZ(convertToDouble(a.i64) - b);
		}

		void FPMUL(convertible_t& a, double b, convertible_t& c) {
			c.f64 = FTZ(convertToDoubleNonZero(a.i64) * b);
		}

		void FPDIV(convertible_t& a, double b, convertible_t& c) {
			c.f64 = FTZ(convertToDoubleNonZero(a.i64) / b);
		}

		void FPSQRT(convertible_t& a, convertible_t& b, convertible_t& c) {
#ifdef __SSE2__
			double d = convertToDoubleNonNegative(a.i64);
			c.f64 = _mm_cvtsd_f64(_mm_sqrt_sd(_mm_setzero_pd(), _mm_load_pd(&d)));
#else
			c.f64 = FTZ(sqrt(convertToDoubleNonNegative(a.i64)));
#endif

		}

		void FPROUND(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.f64 = convertToDouble(a.i64);
			switch (a.u64 & 3) {
				case RoundDown:
#ifdef DEBUG
					std::cout << "Round FE_DOWNWARD (" << FE_DOWNWARD << ") = " <<
#endif
					setRoundMode(FE_DOWNWARD);
#ifdef DEBUG
					std::cout << std::endl;
#endif
					break;
				case RoundUp:
#ifdef DEBUG
					std::cout << "Round FE_UPWARD (" << FE_UPWARD << ") = " <<
#endif
					setRoundMode(FE_UPWARD);
#ifdef DEBUG
					std::cout << std::endl;
#endif
					break;
				case RoundToZero:
#ifdef DEBUG
					std::cout << "Round FE_TOWARDZERO (" << FE_TOWARDZERO << ") = " <<
#endif
					setRoundMode(FE_TOWARDZERO);
#ifdef DEBUG
					std::cout << std::endl;
#endif
					break;
				default:
#ifdef DEBUG
					std::cout << "Round FE_TONEAREST (" << FE_TONEAREST << ") = " <<
#endif
					setRoundMode(FE_TONEAREST);
#ifdef DEBUG
					std::cout << std::endl;
#endif
					break;
			}
		}
	}
}