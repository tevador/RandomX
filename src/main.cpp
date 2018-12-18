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
//#define TRACE
#include "InterpretedVirtualMachine.hpp"
#include "CompiledVirtualMachine.hpp"
#include "AssemblyGeneratorX86.hpp"
#include "Stopwatch.hpp"
#include "blake2/blake2.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <exception>
#include <cstring>
#include "Program.hpp"
#include <string>
#include "instructions.hpp"

const uint8_t seed[32] = { 191, 182, 222, 175, 249, 89, 134, 104, 241, 68, 191, 62, 162, 166, 61, 64, 123, 191, 227, 193, 118, 60, 188, 53, 223, 133, 175, 24, 123, 230, 55, 74 };

void dump(const char* buffer, uint64_t count, const char* name) {
	std::ofstream fout(name, std::ios::out | std::ios::binary);
	fout.write(buffer, count);
	fout.close();
}

constexpr char hexmap[] = "0123456789abcdef";
void outputHex(std::ostream& os, const char* data, int length) {
	for (int i = 0; i < length; ++i) {
		os << hexmap[(data[i] & 0xF0) >> 4];
		os << hexmap[data[i] & 0x0F];
	}
	os << std::endl;
}

void readOption(const char* option, int argc, char** argv, bool& out) {
	for (int i = 0; i < argc; ++i) {
		if (strcmp(argv[i], option) == 0) {
			out = true;
			return;
		}
	}
	out = false;
}

void readInt(int argc, char** argv, int& out, int defaultValue) {
	for (int i = 0; i < argc; ++i) {
		if (*argv[i] != '-' && (out = atoi(argv[i])) > 0) {
			return;
		}
	}
	out = defaultValue;
}

std::ostream& operator<<(std::ostream& os, const RandomX::RegisterFile& rf) {
	for (int i = 0; i < RandomX::RegistersCount; ++i)
		os << std::hex << "r" << i << " = " << rf.r[i].u64 << std::endl << std::dec;
	for (int i = 0; i < RandomX::RegistersCount; ++i)
		os << std::hex << "f" << i << " = " << rf.f[i].u64 << " (" << rf.f[i].f64 << ")" << std::endl << std::dec;
	return os;
}

int main(int argc, char** argv) {
	bool softAes, lightClient, genAsm, compiled;
	int programCount;
	readOption("--softAes", argc, argv, softAes);
	readOption("--lightClient", argc, argv, lightClient);
	readOption("--genAsm", argc, argv, genAsm);
	readOption("--compiled", argc, argv, compiled);
	readInt(argc, argv, programCount, 1000);

	unsigned char blockTemplate[] = {
		0x07, 0x07, 0xf7, 0xa4, 0xf0, 0xd6, 0x05, 0xb3, 0x03, 0x26, 0x08, 0x16, 0xba, 0x3f, 0x10, 0x90, 0x2e, 0x1a, 0x14,
		0x5a, 0xc5, 0xfa, 0xd3, 0xaa, 0x3a, 0xf6, 0xea, 0x44, 0xc1, 0x18, 0x69, 0xdc, 0x4f, 0x85, 0x3f, 0x00, 0x2b, 0x2e,
		0xea, 0x00, 0x00, 0x00, 0x00, 0x77, 0xb2, 0x06, 0xa0, 0x2c, 0xa5, 0xb1, 0xd4, 0xce, 0x6b, 0xbf, 0xdf, 0x0a, 0xca,
		0xc3, 0x8b, 0xde, 0xd3, 0x4d, 0x2d, 0xcd, 0xee, 0xf9, 0x5c, 0xd2, 0x0c, 0xef, 0xc1, 0x2f, 0x61, 0xd5, 0x61, 0x09
	};
	int* nonce = (int*)(blockTemplate + 39);
	uint8_t hash[32];

	if (genAsm) {
		*nonce = programCount;
		blake2b(hash, sizeof(hash), blockTemplate, sizeof(blockTemplate), nullptr, 0);
		RandomX::AssemblyGeneratorX86 asmX86;
		asmX86.generateProgram(hash);
		asmX86.printCode(std::cout);
		return 0;
	}

	if (softAes)
		std::cout << "Using software AES." << std::endl;

	char cumulative[32] = { 0 };

	RandomX::VirtualMachine* vm;

	try {
		if (compiled) {
			vm = new RandomX::CompiledVirtualMachine(softAes);
		}
		else {
			vm = new RandomX::InterpretedVirtualMachine(softAes);
		}
		std::cout << "Initializing..." << std::endl;
		Stopwatch sw(true);
		vm->initializeDataset(seed, lightClient);
		if(lightClient)
			std::cout << "Cache (64 MiB) initialized in " << sw.getElapsed() << " s" << std::endl;
		else
			std::cout << "Dataset (4 GiB) initialized in " << sw.getElapsed() << " s" << std::endl;
		std::cout << "Running benchmark (" << programCount << " programs) ..." << std::endl;
		sw.restart();
		for (int i = 0; i < programCount; ++i) {
			*nonce = i;
			if (RandomX::trace) std::cout << "Nonce: " << i << " ";
			blake2b(hash, sizeof(hash), blockTemplate, sizeof(blockTemplate), nullptr, 0);
			int spIndex = hash[24] | ((hash[25] & 63) << 8);
			vm->initializeScratchpad(spIndex);
			//dump((const char *)vm.getScratchpad(), RandomX::ScratchpadSize, "scratchpad-before.txt");
			//return 0;
			vm->initializeProgram(hash);
			vm->execute();
			/*std::string fileName("scratchpad-after-");
			fileName = fileName + std::to_string(i) + ".txt";
			dump((const char *)vm.getScratchpad(), RandomX::ScratchpadSize, fileName.c_str());*/
			blake2b((void*)hash, sizeof(hash), &vm->getRegisterFile(), sizeof(RandomX::RegisterFile), nullptr, 0);
			if (RandomX::trace) {
				outputHex(std::cout, (char*)hash, sizeof(hash));
			}
			((uint64_t*)cumulative)[0] ^= ((uint64_t*)hash)[0];
			((uint64_t*)cumulative)[1] ^= ((uint64_t*)hash)[1];
			((uint64_t*)cumulative)[2] ^= ((uint64_t*)hash)[2];
			((uint64_t*)cumulative)[3] ^= ((uint64_t*)hash)[3];
		}
		double elapsed = sw.getElapsed();
		std::cout << "Cumulative output hash: ";
		outputHex(std::cout, cumulative, sizeof(cumulative));
		std::cout << "Performance: " << programCount / elapsed << " programs per second" << std::endl;
	}
	catch (std::exception& e) {
		std::cout << "ERROR: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}