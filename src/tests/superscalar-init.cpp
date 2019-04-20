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
#include <unordered_set>
#include "../superscalar.hpp"
#include "../common.hpp"

const uint8_t seed[32] = { 191, 182, 222, 175, 249, 89, 134, 104, 241, 68, 191, 62, 162, 166, 61, 64, 123, 191, 227, 193, 118, 60, 188, 53, 223, 133, 175, 24, 123, 230, 55, 74 };

int main() {
	std::cout << "THIS PROGRAM REQUIRES MORE THAN 10 GB OF RAM TO COMPLETE" << std::endl;
	std::vector<uint64_t> dummy;
	constexpr uint64_t superscalarMul0 = 6364136223846793005ULL;
	constexpr uint64_t superscalarAdd1 = 9298410992540426748ULL; //9298410992540426048ULL
	constexpr uint64_t superscalarAdd2 = 12065312585734608966ULL;
	constexpr uint64_t superscalarAdd3 = 9306329213124610396ULL;
	constexpr uint64_t superscalarAdd4 = 5281919268842080866ULL;
	constexpr uint64_t superscalarAdd5 = 10536153434571861004ULL;
	constexpr uint64_t superscalarAdd6 = 3398623926847679864ULL;
	constexpr uint64_t superscalarAdd7 = 9549104520008361294ULL;
	constexpr uint32_t totalBlocks = RANDOMX_DATASET_SIZE / randomx::CacheLineSize;
	std::unordered_set<uint64_t> registerValues;
	registerValues.reserve(totalBlocks);
	registerValues.rehash(totalBlocks);
	int collisionCount[9] = { 0 };
	for (uint32_t blockNumber = 0; blockNumber < totalBlocks; ++blockNumber) {
		uint64_t rl[8];
		rl[0] = (blockNumber + 1) * superscalarMul0;
		rl[1] = rl[0] ^ superscalarAdd1;
		rl[2] = rl[0] ^ superscalarAdd2;
		rl[3] = rl[0] ^ superscalarAdd3;
		rl[4] = rl[0] ^ superscalarAdd4;
		rl[5] = rl[0] ^ superscalarAdd5;
		rl[6] = rl[0] ^ superscalarAdd6;
		rl[7] = rl[0] ^ superscalarAdd7;
		int blockCollisions = 0;
		for (int i = 0; i < 8; ++i) {
			uint64_t reducedValue = rl[i] & 0x3FFFFFFFFFFFF8; //bits 3-53 only
			if (registerValues.find(reducedValue) != registerValues.end()) {
				blockCollisions++;
				std::cout << "Block " << blockNumber << ": collision of register r" << i << std::endl;
			}
			else {
				registerValues.insert(reducedValue);
			}
		}
		collisionCount[blockCollisions]++;
		if ((blockNumber % (320 * 1024)) == 0)
			std::cout << "Block " << blockNumber << " processed" << std::endl;
	}

	for (int i = 0; i < 9; ++i) {
		std::cout << i << " register(s) collide in " << collisionCount[i] << " blocks" << std::endl;
	}

	return 0;
}