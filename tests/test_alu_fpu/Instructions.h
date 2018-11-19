//RandomX ALU + FPU test
//https://github.com/tevador/RandomX
//License: GPL v3

#include <cstdint>

namespace RandomX {

	constexpr int RoundToNearest = 0;
	constexpr int RoundDown = 1;
	constexpr int RoundUp = 2;
	constexpr int RoundToZero = 3;

	typedef union {
		double f64;
		int64_t i64;
		uint64_t u64;
		int32_t i32;
		uint32_t u32;
	} convertible_t;

	extern "C" {
		void ADD_64(convertible_t& a, convertible_t& b, convertible_t& c);
		void ADD_32(convertible_t& a, convertible_t& b, convertible_t& c);
		void SUB_64(convertible_t& a, convertible_t& b, convertible_t& c);
		void SUB_32(convertible_t& a, convertible_t& b, convertible_t& c);
		void MUL_64(convertible_t& a, convertible_t& b, convertible_t& c);
		void MULH_64(convertible_t& a, convertible_t& b, convertible_t& c);
		void MUL_32(convertible_t& a, convertible_t& b, convertible_t& c);
		void IMUL_32(convertible_t& a, convertible_t& b, convertible_t& c);
		void IMULH_64(convertible_t& a, convertible_t& b, convertible_t& c);
		void DIV_64(convertible_t& a, convertible_t& b, convertible_t& c);
		void IDIV_64(convertible_t& a, convertible_t& b, convertible_t& c);
		void AND_64(convertible_t& a, convertible_t& b, convertible_t& c);
		void AND_32(convertible_t& a, convertible_t& b, convertible_t& c);
		void OR_64(convertible_t& a, convertible_t& b, convertible_t& c);
		void OR_32(convertible_t& a, convertible_t& b, convertible_t& c);
		void XOR_64(convertible_t& a, convertible_t& b, convertible_t& c);
		void XOR_32(convertible_t& a, convertible_t& b, convertible_t& c);
		void SHL_64(convertible_t& a, convertible_t& b, convertible_t& c);
		void SHR_64(convertible_t& a, convertible_t& b, convertible_t& c);
		void SAR_64(convertible_t& a, convertible_t& b, convertible_t& c);
		void ROL_64(convertible_t& a, convertible_t& b, convertible_t& c);
		void ROR_64(convertible_t& a, convertible_t& b, convertible_t& c);
		void FPINIT();
		void FADD_64(convertible_t& a, double b, convertible_t& c);
		void FSUB_64(convertible_t& a, double b, convertible_t& c);
		void FMUL_64(convertible_t& a, double b, convertible_t& c);
		void FDIV_64(convertible_t& a, double b, convertible_t& c);
		void FABSQRT(convertible_t& a, convertible_t& b, convertible_t& c);
		void FROUND(convertible_t& a, convertible_t& b, convertible_t& c);

		inline void FADD(convertible_t& a, convertible_t& b, convertible_t& c) {
			FADD_64(a, (double)b.i64, c);
		}

		inline void FSUB(convertible_t& a, convertible_t& b, convertible_t& c) {
			FSUB_64(a, (double)b.i64, c);
		}

		inline void FMUL(convertible_t& a, convertible_t& b, convertible_t& c) {
			FMUL_64(a, (double)b.i64, c);
		}

		inline void FDIV(convertible_t& a, convertible_t& b, convertible_t& c) {
			FDIV_64(a, (double)b.i64, c);
		}
	}
}