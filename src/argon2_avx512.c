/*
Copyright (c) 2026, NexusXe <nex@nexusxe.com>
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

/* Original code from Argon2 reference source code package used under CC0 Licence
 * https://github.com/P-H-C/phc-winner-argon2
 * Copyright 2015
 * Daniel Dinu, Dmitry Khovratovich, Jean-Philippe Aumasson, and Samuel Neves
*/

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "argon2.h"

void randomx_argon2_fill_segment_avx512(const argon2_instance_t* instance,
	argon2_position_t position);

randomx_argon2_impl* randomx_argon2_impl_avx512() {
#if defined(__AVX512F__)
	return &randomx_argon2_fill_segment_avx512;
#endif
	return NULL;
}

#if defined(__AVX512F__)

#include "argon2_core.h"

#include "blake2/blamka-round-avx512.h"
#include "blake2/blake2-impl.h"
#include "blake2/blake2.h"

/*
	Since the AVX-512 implementation processes v0 and v1 together, these helpers
	are used to help un-interleave these values from the state block.
*/

/// Packs the lower 256-bit halves of v0 and v1 into a single 512-bit register
static inline __m512i avx512_pack_lower(__m512i v0, __m512i v1) {
    return _mm512_inserti64x4(
        _mm512_castsi256_si512(_mm512_castsi512_si256(v0)), 
        _mm512_castsi512_si256(v1), 
        1
    );
}

/// Packs the upper 256-bit halves of v0 and v1 into a single 512-bit register
static inline __m512i avx512_pack_upper(__m512i v0, __m512i v1) {
    return _mm512_inserti64x4(
        _mm512_castsi256_si512(_mm512_extracti64x4_epi64(v0, 1)), 
        _mm512_extracti64x4_epi64(v1, 1), 
        1
    );
}

static void fill_block(__m512i* state, const block* ref_block,
    block* next_block, int with_xor) {
    
    __m512i block_XY[ARGON2_512BIT_WORDS_IN_BLOCK];
    unsigned int i;

    if (with_xor) {
        for (i = 0; i < ARGON2_512BIT_WORDS_IN_BLOCK; i++) {
            // 1. state[i] = state[i] ^ ref_block[i]
            state[i] = _mm512_xor_si512(
                state[i], 
                _mm512_loadu_si512((const __m512i*)ref_block->v + i)
            );
            
            // 2. block_XY[i] = state[i] ^ next_block[i]
            block_XY[i] = _mm512_xor_si512(
                state[i], 
                _mm512_loadu_si512((const __m512i*)next_block->v + i)
            );
        }
    }
    else {
        for (i = 0; i < ARGON2_512BIT_WORDS_IN_BLOCK; i++) {
            // block_XY[i] = state[i] = state[i] ^ ref_block[i]
            block_XY[i] = state[i] = _mm512_xor_si512(
                state[i], 
                _mm512_loadu_si512((const __m512i*)ref_block->v + i)
            );
        }
    }

	for (i = 0; i < 4; ++i) {
        __m512i A = avx512_pack_lower(state[4 * i + 0], state[4 * i + 2]);
        __m512i B = avx512_pack_upper(state[4 * i + 0], state[4 * i + 2]);
        __m512i C = avx512_pack_lower(state[4 * i + 1], state[4 * i + 3]);
        __m512i D = avx512_pack_upper(state[4 * i + 1], state[4 * i + 3]);

        BLAKE2_ROUND_1(A, B, C, D);

        state[4 * i + 0] = avx512_pack_lower(A, B);
        state[4 * i + 2] = avx512_pack_upper(A, B);
        state[4 * i + 1] = avx512_pack_lower(C, D);
        state[4 * i + 3] = avx512_pack_upper(C, D);
    }

	for (int j = 0; j < 2; ++j) {
        // j=0 handles AVX2 i=0 and i=1 (evens)
        // j=1 handles AVX2 i=2 and i=3 (odds)
        
        __m512i A_0 = avx512_pack_lower(state[j], state[j + 2]);
        __m512i B_0 = avx512_pack_lower(state[j + 4], state[j + 6]);
        __m512i C_0 = avx512_pack_lower(state[j + 8], state[j + 10]);
        __m512i D_0 = avx512_pack_lower(state[j + 12], state[j + 14]);

        __m512i A_1 = avx512_pack_upper(state[j], state[j + 2]);
        __m512i B_1 = avx512_pack_upper(state[j + 4], state[j + 6]);
        __m512i C_1 = avx512_pack_upper(state[j + 8], state[j + 10]);
        __m512i D_1 = avx512_pack_upper(state[j + 12], state[j + 14]);

        BLAKE2_ROUND_2(A_0, B_0, C_0, D_0);
        BLAKE2_ROUND_2(A_1, B_1, C_1, D_1);

        state[j]      = avx512_pack_lower(A_0, A_1); 
        state[j + 2]  = avx512_pack_upper(A_0, A_1);
        state[j + 4]  = avx512_pack_lower(B_0, B_1);
        state[j + 6]  = avx512_pack_upper(B_0, B_1);
        state[j + 8]  = avx512_pack_lower(C_0, C_1);
        state[j + 10] = avx512_pack_upper(C_0, C_1);
        state[j + 12] = avx512_pack_lower(D_0, D_1);
        state[j + 14] = avx512_pack_upper(D_0, D_1);
    }

	for (i = 0; i < ARGON2_512BIT_WORDS_IN_BLOCK; i++) {
		state[i] = _mm512_xor_si512(state[i], block_XY[i]);
		_mm512_storeu_si512((__m512i*)next_block->v + i, state[i]);
	}
}

void randomx_argon2_fill_segment_avx512(const argon2_instance_t* instance,
	argon2_position_t position) {
	block* ref_block = NULL, * curr_block = NULL;
	block address_block, input_block;
	uint64_t pseudo_rand, ref_index, ref_lane;
	uint32_t prev_offset, curr_offset;
	uint32_t starting_index, i;
	__m512i state[ARGON2_512BIT_WORDS_IN_BLOCK];
	
	if (instance == NULL) {
		return;
	}

	starting_index = 0;

	if ((0 == position.pass) && (0 == position.slice)) {
		starting_index = 2; /* We have already generated the first two blocks */
	}

	/* Offset of the current block */
	curr_offset = position.lane * instance->lane_length +
		position.slice * instance->segment_length + starting_index;

	if (0 == curr_offset % instance->lane_length) {
		/* Last block in this lane */
		prev_offset = curr_offset + instance->lane_length - 1;
	}
	else {
		/* Previous block */
		prev_offset = curr_offset - 1;
	}

	memcpy(state, ((instance->memory + prev_offset)->v), ARGON2_BLOCK_SIZE);

	for (i = starting_index; i < instance->segment_length;
		++i, ++curr_offset, ++prev_offset) {
		/* 1.1 Rotating prev_offset if needed */
		if (curr_offset % instance->lane_length == 1) {
			prev_offset = curr_offset - 1;
		}

		/* 1.2 Computing the index of the reference block */
		/* 1.2.1 Taking pseudo-random value from the previous block */
		pseudo_rand = instance->memory[prev_offset].v[0];

		/* 1.2.2 Computing the lane of the reference block */
		ref_lane = ((pseudo_rand >> 32)) % instance->lanes;

		if ((position.pass == 0) && (position.slice == 0)) {
			/* Can not reference other lanes yet */
			ref_lane = position.lane;
		}

		/* 1.2.3 Computing the number of possible reference block within the
		* lane.
		*/
		position.index = i;
		ref_index = randomx_argon2_index_alpha(instance, &position, pseudo_rand & 0xFFFFFFFF,
			ref_lane == position.lane);

		/* 2 Creating a new block */
		ref_block =
			instance->memory + instance->lane_length * ref_lane + ref_index;
	curr_block = instance->memory + curr_offset;
		if (ARGON2_VERSION_10 == instance->version) {
			/* version 1.2.1 and earlier: overwrite, not XOR */
			fill_block(state, ref_block, curr_block, 0);
		}
		else {
			if (0 == position.pass) {
				fill_block(state, ref_block, curr_block, 0);
			}
			else {
				fill_block(state, ref_block, curr_block, 1);
			}
		}
	}
}

#endif
