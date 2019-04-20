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
#include "intrin_portable.h"

__m128i soft_aesenc(__m128i in, __m128i key);

__m128i soft_aesdec(__m128i in, __m128i key);

template<bool soft>
inline __m128i aesenc(__m128i in, __m128i key) {
	return soft ? soft_aesenc(in, key) : _mm_aesenc_si128(in, key);
}

template<bool soft>
inline __m128i aesdec(__m128i in, __m128i key) {
	return soft ? soft_aesdec(in, key) : _mm_aesdec_si128(in, key);
}