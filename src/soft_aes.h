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

rx_vec_i128 soft_aesenc(rx_vec_i128 in, rx_vec_i128 key);

rx_vec_i128 soft_aesdec(rx_vec_i128 in, rx_vec_i128 key);

template<bool soft>
inline rx_vec_i128 aesenc(rx_vec_i128 in, rx_vec_i128 key) {
	return soft ? soft_aesenc(in, key) : rx_aesenc_vec_i128(in, key);
}

template<bool soft>
inline rx_vec_i128 aesdec(rx_vec_i128 in, rx_vec_i128 key) {
	return soft ? soft_aesdec(in, key) : rx_aesdec_vec_i128(in, key);
}