/*
Copyright (c) 2018-2019, tevador <tevador@gmail.com>

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	* Neither the name of the copyright holder nor the
	  names of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <stdint.h>
#include "intrin_portable.h"

extern "C" const uint32_t randomx_aes_lut_enc[4][256];
extern "C" const uint32_t randomx_aes_lut_dec[4][256];

extern "C" const uint8_t randomx_aes_lut_enc_index[4][32];
extern "C" const uint8_t randomx_aes_lut_dec_index[4][32];

/*
 * The soft AES rounds live in this header so the streaming loops of
 * aes_hash.cpp can inline them: the table lookups of one round then
 * overlap with the loads and stores of the neighbouring rounds.
 * Without the inlining the call overhead dominates on machines
 * without a hardware AES unit (measured on Loongson 3A6000: the
 * hash+fill pass over the 2 MiB scratchpad runs about three times
 * faster with these two functions inlined).
 */

FORCE_INLINE rx_vec_i128 soft_aesenc(rx_vec_i128 in, rx_vec_i128 key) {
	uint32_t s0, s1, s2, s3;

	s0 = rx_vec_i128_w(in);
	s1 = rx_vec_i128_z(in);
	s2 = rx_vec_i128_y(in);
	s3 = rx_vec_i128_x(in);

	rx_vec_i128 out = rx_set_int_vec_i128(
		(randomx_aes_lut_enc[0][s0 & 0xff] ^ randomx_aes_lut_enc[1][(s3 >> 8) & 0xff] ^ randomx_aes_lut_enc[2][(s2 >> 16) & 0xff] ^ randomx_aes_lut_enc[3][s1 >> 24]),
		(randomx_aes_lut_enc[0][s1 & 0xff] ^ randomx_aes_lut_enc[1][(s0 >> 8) & 0xff] ^ randomx_aes_lut_enc[2][(s3 >> 16) & 0xff] ^ randomx_aes_lut_enc[3][s2 >> 24]),
		(randomx_aes_lut_enc[0][s2 & 0xff] ^ randomx_aes_lut_enc[1][(s1 >> 8) & 0xff] ^ randomx_aes_lut_enc[2][(s0 >> 16) & 0xff] ^ randomx_aes_lut_enc[3][s3 >> 24]),
		(randomx_aes_lut_enc[0][s3 & 0xff] ^ randomx_aes_lut_enc[1][(s2 >> 8) & 0xff] ^ randomx_aes_lut_enc[2][(s1 >> 16) & 0xff] ^ randomx_aes_lut_enc[3][s0 >> 24])
	);

	return rx_xor_vec_i128(out, key);
}

FORCE_INLINE rx_vec_i128 soft_aesdec(rx_vec_i128 in, rx_vec_i128 key) {
	uint32_t s0, s1, s2, s3;

	s0 = rx_vec_i128_w(in);
	s1 = rx_vec_i128_z(in);
	s2 = rx_vec_i128_y(in);
	s3 = rx_vec_i128_x(in);

	rx_vec_i128 out = rx_set_int_vec_i128(
		(randomx_aes_lut_dec[0][s0 & 0xff] ^ randomx_aes_lut_dec[1][(s1 >> 8) & 0xff] ^ randomx_aes_lut_dec[2][(s2 >> 16) & 0xff] ^ randomx_aes_lut_dec[3][s3 >> 24]),
		(randomx_aes_lut_dec[0][s1 & 0xff] ^ randomx_aes_lut_dec[1][(s2 >> 8) & 0xff] ^ randomx_aes_lut_dec[2][(s3 >> 16) & 0xff] ^ randomx_aes_lut_dec[3][s0 >> 24]),
		(randomx_aes_lut_dec[0][s2 & 0xff] ^ randomx_aes_lut_dec[1][(s3 >> 8) & 0xff] ^ randomx_aes_lut_dec[2][(s0 >> 16) & 0xff] ^ randomx_aes_lut_dec[3][s1 >> 24]),
		(randomx_aes_lut_dec[0][s3 & 0xff] ^ randomx_aes_lut_dec[1][(s0 >> 8) & 0xff] ^ randomx_aes_lut_dec[2][(s1 >> 16) & 0xff] ^ randomx_aes_lut_dec[3][s2 >> 24])
	);

	return rx_xor_vec_i128(out, key);
}

template<bool soft>
inline rx_vec_i128 aesenc(rx_vec_i128 in, rx_vec_i128 key) {
	return soft ? soft_aesenc(in, key) : rx_aesenc_vec_i128(in, key);
}

template<bool soft>
inline rx_vec_i128 aesdec(rx_vec_i128 in, rx_vec_i128 key) {
	return soft ? soft_aesdec(in, key) : rx_aesdec_vec_i128(in, key);
}
