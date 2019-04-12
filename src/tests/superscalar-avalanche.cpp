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

#include <iostream>
#include <cstdint>
#include <vector>
#include "../superscalarGenerator.hpp"
#include "../InterpretedVirtualMachine.hpp"
#include "../intrinPortable.h"
#include "../Blake2Generator.hpp"

const uint8_t seed[32] = { 191, 182, 222, 175, 249, 89, 134, 104, 241, 68, 191, 62, 162, 166, 61, 64, 123, 191, 227, 193, 118, 60, 188, 53, 223, 133, 175, 24, 123, 230, 55, 74 };

int main() {

	int insensitiveProgCount[64] = { 0 };
	std::vector<uint64_t> dummy;
	for (int bit = 0; bit < 64; ++bit) {
		for (int i = 0; i < 10000; ++i) {
			uint64_t ra[8] = {
				6364136223846793005ULL,
				9298410992540426048ULL,
				12065312585734608966ULL,
				9306329213124610396ULL,
				5281919268842080866ULL,
				10536153434571861004ULL,
				3398623926847679864ULL,
				9549104520008361294ULL,
			};
			uint64_t rb[8];
			memcpy(rb, ra, sizeof rb);
			rb[0] ^= (1ULL << bit);
			RandomX::SuperscalarProgram p;
			RandomX::Blake2Generator gen(seed, i);
			RandomX::generateSuperscalar(p, gen);
			RandomX::InterpretedVirtualMachine<false>::executeSuperscalar(ra, p, dummy);
			RandomX::InterpretedVirtualMachine<false>::executeSuperscalar(rb, p, dummy);
			uint64_t diff = 0;
			for (int j = 0; j < 8; ++j) {
				diff += __popcnt64(ra[j] ^ rb[j]);
			}
			if (diff < 192 || diff > 320) {
				std::cout << "Seed: " << i << " diff = " << diff << std::endl;
				insensitiveProgCount[bit]++;
			}
		}
	}
	for (int bit = 0; bit < 64; ++bit) {
		std::cout << bit << " " << insensitiveProgCount[bit] << std::endl;
	}

	return 0;
}