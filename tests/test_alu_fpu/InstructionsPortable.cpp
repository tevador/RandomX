//RandomX ALU + FPU test
//https://github.com/tevador/RandomX
//License: GPL v3

#include "Instructions.h"
#include <cfenv>
#include <cmath>

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
	#include <intrin.h>
	#include <stdlib.h>
	#define ror64 _rotr64
	#define rol64 _rotl64
	#ifdef __MACHINEARM64_X64
		#define umulhi64 __umulh
	#endif
	#ifdef __MACHINEX64
		static inline uint64_t __imulhi64(int64_t a, int64_t b) {
			int64_t hi;
			_mul128(a, b, &hi);
			return hi;
		}
		#define imulhi64 __imulhi64
	#endif
	#ifdef __MACHINEX86_X64
		#define sar64 __ll_rshift
	#endif
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

	struct usesArithmeticShift : std::integral_constant<bool, builtintShr64(-1LL, 1) == -1LL> {
	};

	static inline int64_t __sar64(int64_t a, int b) {
		return usesArithmeticShift::value ? builtintShr64(a, b) : (a < 0 ? ~(~a >> b) : a >> b);
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

static double FlushDenormal(double x) {
	if (std::fpclassify(x) == FP_SUBNORMAL) {
		return 0;
	}
	return x;
}

#define FTZ(x) FlushDenormal(x)

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
			if (a.i64 == INT64_MIN && b.i64 == -1)
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

		void FPINIT() {
			fesetround(FE_TONEAREST);
		}

		void FADD_64(convertible_t& a, double b, convertible_t& c) {
			c.f64 = FTZ((double)a.i64 + b);
		}

		void FSUB_64(convertible_t& a, double b, convertible_t& c) {
			c.f64 = FTZ((double)a.i64 - b);
		}

		void FMUL_64(convertible_t& a, double b, convertible_t& c) {
			c.f64 = FTZ((double)a.i64 * b);
		}

		void FDIV_64(convertible_t& a, double b, convertible_t& c) {
			c.f64 = FTZ((double)a.i64 / b);
		}

		void FABSQRT(convertible_t& a, convertible_t& b, convertible_t& c) {
			double d = fabs((double)a.i64);
			c.f64 = FTZ(sqrt(d));
		}

		void FROUND(convertible_t& a, convertible_t& b, convertible_t& c) {
			c.f64 = (double)a.i64;
			switch (a.u64 & 3) {
				case RoundDown:
					fesetround(FE_DOWNWARD);
					break;
				case RoundUp:
					fesetround(FE_UPWARD);
					break;
				case RoundToZero:
					fesetround(FE_TOWARDZERO);
					break;
				default:
					fesetround(FE_TONEAREST);
					break;
			}
		}
	}
}