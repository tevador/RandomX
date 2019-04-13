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
#include "../superscalarGenerator.hpp"
#include "../Blake2Generator.hpp"

const uint8_t seed[32] = { 191, 182, 222, 175, 249, 89, 134, 104, 241, 68, 191, 62, 162, 166, 61, 64, 123, 191, 227, 193, 118, 60, 188, 53, 223, 133, 175, 24, 123, 230, 55, 74 };

int main() {

	constexpr int count = 100000;
	int64_t asicLatency = 0;
	int64_t codesize = 0;
	int64_t cpuLatency = 0;
	int64_t macroOps = 0;
	int64_t mulCount = 0;
	int64_t size = 0;
	for (int i = 0; i < count; ++i) {
		RandomX::SuperscalarProgram prog;
		RandomX::Blake2Generator gen(seed, i);
		RandomX::generateSuperscalar(prog, gen);
		asicLatency += prog.asicLatency;
		codesize += prog.codeSize;
		cpuLatency += prog.cpuLatency;
		macroOps += prog.macroOps;
		mulCount += prog.mulCount;
		size += prog.getSize();

		if ((i + 1) % (count / 100) == 0) {
			std::cout << "Completed " << ((i + 1) / (count / 100)) << "% ..." << std::endl;
		}
	}

	std::cout << "Avg. IPC: " << (macroOps / (double)cpuLatency) << std::endl;
	std::cout << "Avg. ASIC latency: " << (asicLatency / (double)count) << std::endl;
	std::cout << "Avg. CPU latency: " << (cpuLatency / (double)count) << std::endl;
	std::cout << "Avg. code size: " << (codesize / (double)count) << std::endl;
	std::cout << "Avg. x86 ops: " << (macroOps / (double)count) << std::endl;
	std::cout << "Avg. mul. count: " << (mulCount / (double)count) << std::endl;
	std::cout << "Avg. RandomX ops: " << (size / (double)count) << std::endl;

	return 0;
}