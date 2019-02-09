/*
  Reference implementations of computing and using the "magic number" approach to dividing
  by constants, including codegen instructions. The unsigned division incorporates the
  "round down" optimization per ridiculous_fish.

  This is free and unencumbered software. Any copyright is dedicated to the Public Domain.
*/

#include <limits.h> //for CHAR_BIT
#include <assert.h>

#include "divideByConstantCodegen.h"

struct magicu_info compute_unsigned_magic_info(unsigned_type D, unsigned num_bits) {

	//The numerator must fit in a unsigned_type
	assert(num_bits > 0 && num_bits <= sizeof(unsigned_type) * CHAR_BIT);

	// D must be larger than zero and not a power of 2
	assert(D & (D - 1));

	// The eventual result
	struct magicu_info result;

	// Bits in a unsigned_type
	const unsigned UINT_BITS = sizeof(unsigned_type) * CHAR_BIT;

	// The extra shift implicit in the difference between UINT_BITS and num_bits
	const unsigned extra_shift = UINT_BITS - num_bits;

	// The initial power of 2 is one less than the first one that can possibly work
	const unsigned_type initial_power_of_2 = (unsigned_type)1 << (UINT_BITS - 1);

	// The remainder and quotient of our power of 2 divided by d
	unsigned_type quotient = initial_power_of_2 / D, remainder = initial_power_of_2 % D;

	// ceil(log_2 D)
	unsigned ceil_log_2_D;

	// The magic info for the variant "round down" algorithm
	unsigned_type down_multiplier = 0;
	unsigned down_exponent = 0;
	int has_magic_down = 0;

	// Compute ceil(log_2 D)
	ceil_log_2_D = 0;
	unsigned_type tmp;
	for (tmp = D; tmp > 0; tmp >>= 1)
		ceil_log_2_D += 1;


	// Begin a loop that increments the exponent, until we find a power of 2 that works.
	unsigned exponent;
	for (exponent = 0; ; exponent++) {
		// Quotient and remainder is from previous exponent; compute it for this exponent.
		if (remainder >= D - remainder) {
			// Doubling remainder will wrap around D
			quotient = quotient * 2 + 1;
			remainder = remainder * 2 - D;
		}
		else {
			// Remainder will not wrap
			quotient = quotient * 2;
			remainder = remainder * 2;
		}

		// We're done if this exponent works for the round_up algorithm.
		// Note that exponent may be larger than the maximum shift supported,
		// so the check for >= ceil_log_2_D is critical.
		if ((exponent + extra_shift >= ceil_log_2_D) || (D - remainder) <= ((unsigned_type)1 << (exponent + extra_shift)))
			break;

		// Set magic_down if we have not set it yet and this exponent works for the round_down algorithm
		if (!has_magic_down && remainder <= ((unsigned_type)1 << (exponent + extra_shift))) {
			has_magic_down = 1;
			down_multiplier = quotient;
			down_exponent = exponent;
		}
	}

	if (exponent < ceil_log_2_D) {
		// magic_up is efficient
		result.multiplier = quotient + 1;
		result.pre_shift = 0;
		result.post_shift = exponent;
		result.increment = 0;
	}
	else if (D & 1) {
		// Odd divisor, so use magic_down, which must have been set
		assert(has_magic_down);
		result.multiplier = down_multiplier;
		result.pre_shift = 0;
		result.post_shift = down_exponent;
		result.increment = 1;
	}
	else {
		// Even divisor, so use a prefix-shifted dividend
		unsigned pre_shift = 0;
		unsigned_type shifted_D = D;
		while ((shifted_D & 1) == 0) {
			shifted_D >>= 1;
			pre_shift += 1;
		}
		result = compute_unsigned_magic_info(shifted_D, num_bits - pre_shift);
		assert(result.increment == 0 && result.pre_shift == 0); //expect no increment or pre_shift in this path
		result.pre_shift = pre_shift;
	}
	return result;
}

struct magics_info compute_signed_magic_info(signed_type D) {
	// D must not be zero and must not be a power of 2 (or its negative)
	assert(D != 0 && (D & -D) != D && (D & -D) != -D);

	// Our result
	struct magics_info result;

	// Bits in an signed_type
	const unsigned SINT_BITS = sizeof(signed_type) * CHAR_BIT;

	// Absolute value of D (we know D is not the most negative value since that's a power of 2)
	const unsigned_type abs_d = (D < 0 ? -D : D);

	// The initial power of 2 is one less than the first one that can possibly work
	// "two31" in Warren
	unsigned exponent = SINT_BITS - 1;
	const unsigned_type initial_power_of_2 = (unsigned_type)1 << exponent;

	// Compute the absolute value of our "test numerator,"
	// which is the largest dividend whose remainder with d is d-1.
	// This is called anc in Warren.
	const unsigned_type tmp = initial_power_of_2 + (D < 0);
	const unsigned_type abs_test_numer = tmp - 1 - tmp % abs_d;

	// Initialize our quotients and remainders (q1, r1, q2, r2 in Warren)
	unsigned_type quotient1 = initial_power_of_2 / abs_test_numer, remainder1 = initial_power_of_2 % abs_test_numer;
	unsigned_type quotient2 = initial_power_of_2 / abs_d, remainder2 = initial_power_of_2 % abs_d;
	unsigned_type delta;

	// Begin our loop
	do {
		// Update the exponent
		exponent++;

		// Update quotient1 and remainder1
		quotient1 *= 2;
		remainder1 *= 2;
		if (remainder1 >= abs_test_numer) {
			quotient1 += 1;
			remainder1 -= abs_test_numer;
		}

		// Update quotient2 and remainder2
		quotient2 *= 2;
		remainder2 *= 2;
		if (remainder2 >= abs_d) {
			quotient2 += 1;
			remainder2 -= abs_d;
		}

		// Keep going as long as (2**exponent) / abs_d <= delta
		delta = abs_d - remainder2;
	} while (quotient1 < delta || (quotient1 == delta && remainder1 == 0));

	result.multiplier = quotient2 + 1;
	if (D < 0) result.multiplier = -result.multiplier;
	result.shift = exponent - SINT_BITS;
	return result;
}
