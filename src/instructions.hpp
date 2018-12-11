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

#include <cstdint>
#include "common.hpp"

namespace RandomX {

	inline double convertToDouble(int64_t x) {
		return (double)(x &-2048L);
	}

	inline double convertToDoubleNonZero(int64_t x) {
		return (double)((x & -2048L) | 2048);
	}

	inline double convertToDoubleNonNegative(int64_t x) {
		return (double)(x & 9223372036854773760L);
	}

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
		void FPADD(convertible_t& a, double b, convertible_t& c);
		void FPSUB(convertible_t& a, double b, convertible_t& c);
		void FPMUL(convertible_t& a, double b, convertible_t& c);
		void FPDIV(convertible_t& a, double b, convertible_t& c);
		void FPSQRT(convertible_t& a, convertible_t& b, convertible_t& c);
		void FPROUND(convertible_t& a, convertible_t& b, convertible_t& c);

		inline void FPADD_64(convertible_t& a, convertible_t& b, convertible_t& c) {
			FPADD(a, b.f64, c);
		}

		inline void FPSUB_64(convertible_t& a, convertible_t& b, convertible_t& c) {
			FPSUB(a, b.f64, c);
		}

		inline void FPMUL_64(convertible_t& a, convertible_t& b, convertible_t& c) {
			FPMUL(a, b.f64, c);
		}

		inline void FPDIV_64(convertible_t& a, convertible_t& b, convertible_t& c) {
			FPDIV(a, b.f64, c);
		}
	}
}