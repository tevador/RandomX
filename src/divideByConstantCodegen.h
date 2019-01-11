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
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

	typedef uint64_t uint;
	typedef int64_t sint;

	/* Computes "magic info" for performing signed division by a fixed integer D.
	   The type 'sint' is assumed to be defined as a signed integer type large enough
	   to hold both the dividend and the divisor.
	   Here >> is arithmetic (signed) shift, and >>> is logical shift.

	   To emit code for n/d, rounding towards zero, use the following sequence:

		 m = compute_signed_magic_info(D)
		 emit("result = (m.multiplier * n) >> SINT_BITS");
		 if d > 0 and m.multiplier < 0: emit("result += n")
		 if d < 0 and m.multiplier > 0: emit("result -= n")
		 if m.post_shift > 0: emit("result >>= m.shift")
		 emit("result += (result < 0)")

	  The shifts by SINT_BITS may be "free" if the high half of the full multiply
	  is put in a separate register.

	  The final add can of course be implemented via the sign bit, e.g.
		  result += (result >>> (SINT_BITS - 1))
	   or
		  result -= (result >> (SINT_BITS - 1))

	   This code is heavily indebted to Hacker's Delight by Henry Warren.
	   See http://www.hackersdelight.org/HDcode/magic.c.txt
	   Used with permission from http://www.hackersdelight.org/permissions.htm
	 */

	struct magics_info {
		sint multiplier; // the "magic number" multiplier
		unsigned shift; // shift for the dividend after multiplying
	};
	struct magics_info compute_signed_magic_info(sint D);


	/* Computes "magic info" for performing unsigned division by a fixed positive integer D.
	   The type 'uint' is assumed to be defined as an unsigned integer type large enough
	   to hold both the dividend and the divisor. num_bits can be set appropriately if n is
	   known to be smaller than the largest uint; if this is not known then pass
	   (sizeof(uint) * CHAR_BIT) for num_bits.

	   Assume we have a hardware register of width UINT_BITS, a known constant D which is
	   not zero and not a power of 2, and a variable n of width num_bits (which may be
	   up to UINT_BITS). To emit code for n/d, use one of the two following sequences
	   (here >>> refers to a logical bitshift):

		 m = compute_unsigned_magic_info(D, num_bits)
		 if m.pre_shift > 0: emit("n >>>= m.pre_shift")
		 if m.increment: emit("n = saturated_increment(n)")
		 emit("result = (m.multiplier * n) >>> UINT_BITS")
		 if m.post_shift > 0: emit("result >>>= m.post_shift")

	   or

		 m = compute_unsigned_magic_info(D, num_bits)
		 if m.pre_shift > 0: emit("n >>>= m.pre_shift")
		 emit("result = m.multiplier * n")
		 if m.increment: emit("result = result + m.multiplier")
		 emit("result >>>= UINT_BITS")
		 if m.post_shift > 0: emit("result >>>= m.post_shift")

	  The shifts by UINT_BITS may be "free" if the high half of the full multiply
	  is put in a separate register.

	  saturated_increment(n) means "increment n unless it would wrap to 0," i.e.
		if n == (1 << UINT_BITS)-1: result = n
		else: result = n+1
	  A common way to implement this is with the carry bit. For example, on x86:
		 add 1
		 sbb 0

	  Some invariants:
	   1: At least one of pre_shift and increment is zero
	   2: multiplier is never zero

	   This code incorporates the "round down" optimization per ridiculous_fish.
	 */

	struct magicu_info {
		uint multiplier; // the "magic number" multiplier
		unsigned pre_shift; // shift for the dividend before multiplying
		unsigned post_shift; //shift for the dividend after multiplying
		int increment; // 0 or 1; if set then increment the numerator, using one of the two strategies
	};
	struct magicu_info compute_unsigned_magic_info(uint D, unsigned num_bits);

#if defined(__cplusplus)
}
#endif