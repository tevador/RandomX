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

#include "soft_aes.h"

/*
	Calculate a 512-bit hash of 'input' using 4 lanes of AES.
	The input is treated as a set of round keys for the encryption
	of the initial state.

	'inputSize' must be a multiple of 64.

	For a 2 MiB input, this has the same security as 32768-round
	AES encryption.

	Hashing throughput: >20 GiB/s per CPU core with hardware AES
*/
template<bool softAes>
void hashAes1Rx4(const void *input, size_t inputSize, void *hash) {
	const uint8_t* inptr = (uint8_t*)input;
	const uint8_t* inputEnd = inptr + inputSize;

	rx_vec_i128 state0, state1, state2, state3;
	rx_vec_i128 in0, in1, in2, in3;

	//intial state
	state0 = rx_set_int_vec_i128(0x8d3126fd, 0x1146d167, 0x887af5ab, 0xc4778e00);
	state1 = rx_set_int_vec_i128(0x19fe9fa1, 0x58da632b, 0x1b95af89, 0xb834ef4b);
	state2 = rx_set_int_vec_i128(0x1bb2cd74, 0xc35ad744, 0xab283a00, 0x7742dd3a);
	state3 = rx_set_int_vec_i128(0xbb30a58a, 0x49593c57, 0xdc5d97cc, 0xe18b449a);

	//process 64 bytes at a time in 4 lanes
	while (inptr < inputEnd) {
		in0 = rx_load_vec_i128((rx_vec_i128*)inptr + 0);
		in1 = rx_load_vec_i128((rx_vec_i128*)inptr + 1);
		in2 = rx_load_vec_i128((rx_vec_i128*)inptr + 2);
		in3 = rx_load_vec_i128((rx_vec_i128*)inptr + 3);

		state0 = aesenc<softAes>(state0, in0);
		state1 = aesdec<softAes>(state1, in1);
		state2 = aesenc<softAes>(state2, in2);
		state3 = aesdec<softAes>(state3, in3);

		inptr += 64;
	}

	//two extra rounds to achieve full diffusion
	rx_vec_i128 xkey0 = rx_set_int_vec_i128(0x83951283, 0xe4c5593d, 0x2a5a929c, 0x11cbf247);
	rx_vec_i128 xkey1 = rx_set_int_vec_i128(0xff215bb2, 0xabbc2523, 0x477bef0b, 0xce816c95);

	state0 = aesenc<softAes>(state0, xkey0);
	state1 = aesdec<softAes>(state1, xkey0);
	state2 = aesenc<softAes>(state2, xkey0);
	state3 = aesdec<softAes>(state3, xkey0);

	state0 = aesenc<softAes>(state0, xkey1);
	state1 = aesdec<softAes>(state1, xkey1);
	state2 = aesenc<softAes>(state2, xkey1);
	state3 = aesdec<softAes>(state3, xkey1);

	//output hash
	rx_store_vec_i128((rx_vec_i128*)hash + 0, state0);
	rx_store_vec_i128((rx_vec_i128*)hash + 1, state1);
	rx_store_vec_i128((rx_vec_i128*)hash + 2, state2);
	rx_store_vec_i128((rx_vec_i128*)hash + 3, state3);
}

template void hashAes1Rx4<false>(const void *input, size_t inputSize, void *hash);
template void hashAes1Rx4<true>(const void *input, size_t inputSize, void *hash);

/*
	Fill 'buffer' with pseudorandom data based on 512-bit 'state'.
	The state is encrypted using a single AES round per 16 bytes of output
	in 4 lanes.

	'outputSize' must be a multiple of 64.

	The modified state is written back to 'state' to allow multiple
	calls to this function.
*/
template<bool softAes>
void fillAes1Rx4(void *state, size_t outputSize, void *buffer) {
	const uint8_t* outptr = (uint8_t*)buffer;
	const uint8_t* outputEnd = outptr + outputSize;

	rx_vec_i128 state0, state1, state2, state3;
	rx_vec_i128 key0, key1, key2, key3;

	key0 = rx_set_int_vec_i128(0xdf20a2e3, 0xca329132, 0x454ff6d5, 0x84eeec2d);
	key1 = rx_set_int_vec_i128(0x1deb5971, 0xfed0387f, 0xf10fc578, 0x017b63d0);
	key2 = rx_set_int_vec_i128(0xdfc926b3, 0xa517ceb4, 0x2f2c70a1, 0x327d7a52);
	key3 = rx_set_int_vec_i128(0x341cf31c, 0xa0ece0a9, 0x3d17da5e, 0x5c8d77d3);

	state0 = rx_load_vec_i128((rx_vec_i128*)state + 0);
	state1 = rx_load_vec_i128((rx_vec_i128*)state + 1);
	state2 = rx_load_vec_i128((rx_vec_i128*)state + 2);
	state3 = rx_load_vec_i128((rx_vec_i128*)state + 3);

	while (outptr < outputEnd) {
		state0 = aesdec<softAes>(state0, key0);
		state1 = aesenc<softAes>(state1, key1);
		state2 = aesdec<softAes>(state2, key2);
		state3 = aesenc<softAes>(state3, key3);

		rx_store_vec_i128((rx_vec_i128*)outptr + 0, state0);
		rx_store_vec_i128((rx_vec_i128*)outptr + 1, state1);
		rx_store_vec_i128((rx_vec_i128*)outptr + 2, state2);
		rx_store_vec_i128((rx_vec_i128*)outptr + 3, state3);

		outptr += 64;
	}

	rx_store_vec_i128((rx_vec_i128*)state + 0, state0);
	rx_store_vec_i128((rx_vec_i128*)state + 1, state1);
	rx_store_vec_i128((rx_vec_i128*)state + 2, state2);
	rx_store_vec_i128((rx_vec_i128*)state + 3, state3);
}

template void fillAes1Rx4<true>(void *state, size_t outputSize, void *buffer);
template void fillAes1Rx4<false>(void *state, size_t outputSize, void *buffer);
