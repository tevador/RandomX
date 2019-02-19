/*
Copyright (c) 2019 tevador

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

/*
	Based on the original idea by SChernykh:
	https://github.com/SChernykh/xmr-stak-cpu/issues/1#issuecomment-414336613
*/

#include <stdint.h>

#if !defined(_M_X64) && !defined(__x86_64__)

typedef struct {
	uint64_t lo;
	uint64_t hi;
} uint128_t;

#define LO(x) ((x)&0xffffffff)
#define HI(x) ((x)>>32)
static inline uint128_t square128(uint64_t x) {
	uint64_t xh = HI(x), xl = LO(x);
	uint64_t xll = xl * xl;
	uint64_t xlh = xl * xh;
	uint64_t xhh = xh * xh;
	uint64_t m1 = 2 * LO(xlh) + HI(xll);
	uint64_t m2 = 2 * HI(xlh) + LO(xhh) + HI(m1);
	uint64_t m3 = HI(xhh) + HI(m2);

	uint128_t x2;

	x2.lo = (m1 << 32) + LO(xll);
	x2.hi = (m3 << 32) + LO(m2);

	return x2;
}
#undef LO
#undef HI

inline uint64_t squareHash(uint64_t x) {
	x += 1613783669344650115;
	for (int i = 0; i < 42; ++i) {
		uint128_t x2 = square128(x);
		x = x2.lo - x2.hi;
	}
	return x;
}

#else

#if defined(__cplusplus)
extern "C" {
#endif

uint64_t squareHash(uint64_t);

#if defined(__cplusplus)
}
#endif

#endif