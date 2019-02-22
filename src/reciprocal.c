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

#include "reciprocal.h"


/*
	Calculates rcp = 2**x / divisor for highest integer x such that rcp < 2**64.

	Equivalent x86 assembly (divisor in rcx):

	mov edx, 1
	mov r8, rcx
	xor eax, eax
	bsr rcx, rcx
	shl rdx, cl
	div r8
	ret

*/
uint64_t reciprocal(uint64_t divisor) {

	const uint64_t p2exp63 = 1ULL << 63;

	uint64_t quotient = p2exp63 / divisor, remainder = p2exp63 % divisor;

	unsigned bsr = 0; //highest set bit in divisor

	for (uint64_t bit = divisor; bit > 0; bit >>= 1)
		bsr++;

	for (unsigned shift = 0; shift < bsr; shift++) {
		if (remainder >= divisor - remainder) {
			quotient = quotient * 2 + 1;
			remainder = remainder * 2 - divisor;
		}
		else {
			quotient = quotient * 2;
			remainder = remainder * 2;
		}
	}

	return quotient;
}