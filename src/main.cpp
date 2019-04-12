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
#include "CompiledLightVirtualMachine.hpp"
#include "AssemblyGeneratorX86.hpp"
#include "Stopwatch.hpp"
#include "blake2/blake2.h"
#include "blake2/endian.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <exception>
#include <cstring>
#include "Program.hpp"
#include <string>
#include <thread>
#include <atomic>
#include "dataset.hpp"
#include "Cache.hpp"
#include "hashAes1Rx4.hpp"
#include "LightProgramGenerator.hpp"
#include "JitCompilerX86.hpp"

const uint8_t seed[32] = { 191, 182, 222, 175, 249, 89, 134, 104, 241, 68, 191, 62, 162, 166, 61, 64, 123, 191, 227, 193, 118, 60, 188, 53, 223, 133, 175, 24, 123, 230, 55, 74 };

const uint8_t blockTemplate__[] = {
		0x07, 0x07, 0xf7, 0xa4, 0xf0, 0xd6, 0x05, 0xb3, 0x03, 0x26, 0x08, 0x16, 0xba, 0x3f, 0x10, 0x90, 0x2e, 0x1a, 0x14,
		0x5a, 0xc5, 0xfa, 0xd3, 0xaa, 0x3a, 0xf6, 0xea, 0x44, 0xc1, 0x18, 0x69, 0xdc, 0x4f, 0x85, 0x3f, 0x00, 0x2b, 0x2e,
		0xea, 0x00, 0x00, 0x00, 0x00, 0x77, 0xb2, 0x06, 0xa0, 0x2c, 0xa5, 0xb1, 0xd4, 0xce, 0x6b, 0xbf, 0xdf, 0x0a, 0xca,
		0xc3, 0x8b, 0xde, 0xd3, 0x4d, 0x2d, 0xcd, 0xee, 0xf9, 0x5c, 0xd2, 0x0c, 0xef, 0xc1, 0x2f, 0x61, 0xd5, 0x61, 0x09
};

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

void readIntOption(const char* option, int argc, char** argv, int& out, int defaultValue) {
	for (int i = 0; i < argc - 1; ++i) {
		if (strcmp(argv[i], option) == 0 && (out = atoi(argv[i + 1])) > 0) {
			return;
		}
	}
	out = defaultValue;
}

void readInt(int argc, char** argv, int& out, int defaultValue) {
	for (int i = 0; i < argc; ++i) {
		if (*argv[i] != '-' && (out = atoi(argv[i])) > 0) {
			return;
		}
	}
	out = defaultValue;
}

class AtomicHash {
public:
	AtomicHash() {
		for (int i = 0; i < 4; ++i)
			hash[i].store(0);
	}
	void xorWith(uint64_t update[4]) {
		for (int i = 0; i < 4; ++i)
			hash[i].fetch_xor(update[i]);
	}
	void print(std::ostream& os) {
		for (int i = 0; i < 4; ++i)
			print(hash[i], os);
		os << std::endl;
	}
private:
	static void print(std::atomic<uint64_t>& hash, std::ostream& os) {
		auto h = hash.load();
		outputHex(std::cout, (char*)&h, sizeof(h));
	}
	std::atomic<uint64_t> hash[4];
};

void printUsage(const char* executable) {
	std::cout << "Usage: " << executable << " [OPTIONS]" << std::endl;
	std::cout << "Supported options:" << std::endl;
	std::cout << "  --help        shows this message" << std::endl;
	std::cout << "  --mine        mining mode: 4 GiB, x86-64 compiled VM" << std::endl;
	std::cout << "  --verify      verification mode: 256 MiB, portable VM" << std::endl;
	std::cout << "  --largePages  use large pages" << std::endl;
	std::cout << "  --softAes     use software AES (default: x86 AES-NI)" << std::endl;
	std::cout << "  --threads T   use T threads (default: 1)" << std::endl;
	std::cout << "  --init Q      initialize dataset with Q threads (default: 1)" << std::endl;
	std::cout << "  --nonces N    run N nonces (default: 1000)" << std::endl;
	std::cout << "  --genAsm      generate x86-64 asm code for nonce N" << std::endl;
	std::cout << "  --genNative   generate RandomX code for nonce N" << std::endl;
}

template<bool softAes>
void generateAsm(uint32_t nonce) {
	alignas(16) uint64_t hash[8];
	uint8_t blockTemplate[sizeof(blockTemplate__)];
	memcpy(blockTemplate, blockTemplate__, sizeof(blockTemplate));
	store32(blockTemplate + 39, nonce);
	blake2b(hash, sizeof(hash), blockTemplate, sizeof(blockTemplate), nullptr, 0);
	uint8_t scratchpad[RANDOMX_SCRATCHPAD_L3];
	fillAes1Rx4<softAes>((void*)hash, RANDOMX_SCRATCHPAD_L3, scratchpad);
	RandomX::AssemblyGeneratorX86 asmX86;
	RandomX::Program p;
	fillAes1Rx4<softAes>(hash, sizeof(p), &p);
	asmX86.generateProgram(p);
	asmX86.printCode(std::cout);
}

template<bool softAes>
void generateNative(uint32_t nonce) {
	alignas(16) uint64_t hash[8];
	uint8_t blockTemplate[sizeof(blockTemplate__)];
	memcpy(blockTemplate, blockTemplate__, sizeof(blockTemplate));
	store32(blockTemplate + 39, nonce);
	blake2b(hash, sizeof(hash), blockTemplate, sizeof(blockTemplate), nullptr, 0);
	uint8_t scratchpad[RANDOMX_SCRATCHPAD_L3];
	fillAes1Rx4<softAes>((void*)hash, RANDOMX_SCRATCHPAD_L3, scratchpad);
	alignas(16) RandomX::Program prog;
	fillAes1Rx4<softAes>((void*)hash, sizeof(prog), &prog);
	for (int i = 0; i < RANDOMX_PROGRAM_SIZE; ++i) {
		prog(i).dst %= 8;
		prog(i).src %= 8;
	}
	std::cout << prog << std::endl;
}

template<bool softAes>
void mine(RandomX::VirtualMachine* vm, std::atomic<uint32_t>& atomicNonce, AtomicHash& result, uint32_t noncesCount, int thread, uint8_t* scratchpad) {
	alignas(16) uint64_t hash[8];
	uint8_t blockTemplate[sizeof(blockTemplate__)];
	memcpy(blockTemplate, blockTemplate__, sizeof(blockTemplate));
	void* noncePtr = blockTemplate + 39;
	auto nonce = atomicNonce.fetch_add(1);

	while (nonce < noncesCount) {
		//std::cout << "Thread " << thread << " nonce " << nonce << std::endl;
		store32(noncePtr, nonce);
		blake2b(hash, sizeof(hash), blockTemplate, sizeof(blockTemplate), nullptr, 0);
		fillAes1Rx4<softAes>((void*)hash, RANDOMX_SCRATCHPAD_L3, scratchpad);
		vm->resetRoundingMode();
		vm->setScratchpad(scratchpad);
		for (int chain = 0; chain < RANDOMX_PROGRAM_COUNT - 1; ++chain) {
			fillAes1Rx4<softAes>((void*)hash, sizeof(RandomX::Program), vm->getProgramBuffer());
			vm->initialize();
			vm->execute();
			vm->getResult<false>(nullptr, 0, hash);
		}
		fillAes1Rx4<softAes>((void*)hash, sizeof(RandomX::Program), vm->getProgramBuffer());
		vm->initialize();
		vm->execute();
		/*if (RandomX::trace) {
			for (int j = 0; j < RandomX::ProgramLength; ++j) {
				uint64_t res = *(uint64_t*)(scratchpad + 8 * (RandomX::ProgramLength - 1 - j));
				std::cout << std::hex << std::setw(16) << std::setfill('0') << res << std::endl;
			}
		}*/
		vm->getResult<softAes>(scratchpad, RANDOMX_SCRATCHPAD_L3, hash);
		//dump((char*)scratchpad, RANDOMX_SCRATCHPAD_L3, "spad.txt");
		result.xorWith(hash);
		if (RandomX::trace) {
			std::cout << "Nonce: " << nonce << " ";
			outputHex(std::cout, (char*)hash, 16);
			std::cout << std::endl;
		}
		nonce = atomicNonce.fetch_add(1);
	}
}



int main(int argc, char** argv) {
	bool softAes, genAsm, miningMode, verificationMode, help, largePages, async, genNative, jit, genSuperscalar, legacy;
	int programCount, threadCount, initThreadCount, epoch;

	readOption("--softAes", argc, argv, softAes);
	readOption("--genAsm", argc, argv, genAsm);
	readOption("--mine", argc, argv, miningMode);
	readOption("--verify", argc, argv, verificationMode);
	readIntOption("--threads", argc, argv, threadCount, 1);
	readIntOption("--nonces", argc, argv, programCount, 1000);
	readIntOption("--init", argc, argv, initThreadCount, 1);
	readIntOption("--epoch", argc, argv, epoch, 0);
	readOption("--largePages", argc, argv, largePages);
	readOption("--jit", argc, argv, jit);
	readOption("--genNative", argc, argv, genNative);
	readOption("--help", argc, argv, help);
	readOption("--genSuperscalar", argc, argv, genSuperscalar);
	readOption("--legacy", argc, argv, legacy);

	if (genSuperscalar) {
		RandomX::LightProgram p;
		RandomX::Blake2Generator gen(seed, programCount);
		RandomX::generateSuperscalar(p, gen);
		RandomX::AssemblyGeneratorX86 asmX86;
		asmX86.generateAsm(p);
		//std::ofstream file("lightProg2.asm");
		//asmX86.printCode(std::cout);
		return 0;
	}

	if (genAsm) {
		if (softAes)
			generateAsm<true>(programCount);
		else
			generateAsm<false>(programCount);
		return 0;
	}

	if (genNative) {
		if (softAes)
			generateNative<true>(programCount);
		else
			generateNative<false>(programCount);
		return 0;
	}

	if (help || (!miningMode && !verificationMode)) {
		printUsage(argv[0]);
		return 0;
	}

	if (softAes)
		std::cout << "Using software AES." << std::endl;

	std::atomic<uint32_t> atomicNonce(0);
	AtomicHash result;
	std::vector<RandomX::VirtualMachine*> vms;
	std::vector<std::thread> threads;
	RandomX::dataset_t dataset;
	const uint64_t cacheSize = (RANDOMX_ARGON_MEMORY + RANDOMX_ARGON_GROWTH * epoch) * RandomX::ArgonBlockSize;
	const uint64_t datasetSize = (RANDOMX_DATASET_SIZE + RANDOMX_DS_GROWTH * epoch);
	dataset.cache.size = cacheSize;
	RandomX::LightProgram programs[RANDOMX_CACHE_ACCESSES];

	std::cout << "RandomX - " << (miningMode ? "mining" : "verification") << " mode" << std::endl;

	std::cout << "Initializing";
	if(miningMode)
		std::cout << " (" << initThreadCount << " thread" << (initThreadCount > 1 ? "s)" : ")");
	std::cout << " ..." << std::endl;

	try {
		Stopwatch sw(true);
		RandomX::datasetInitCache(seed, dataset, largePages);
		if (RandomX::trace) {
			std::cout << "Cache: " << std::endl;
			outputHex(std::cout, (char*)dataset.cache.memory, sizeof(__m128i));
			std::cout << std::endl;
		}
		if (!legacy) {
			RandomX::Blake2Generator gen(seed, programCount);
			for (int i = 0; i < RANDOMX_CACHE_ACCESSES; ++i) {
				RandomX::generateSuperscalar(programs[i], gen);
			}
		}
		if (!miningMode) {
			std::cout << "Cache (" << cacheSize << " bytes) initialized in " << sw.getElapsed() << " s" << std::endl;
		}
		else {
			auto cache = dataset.cache;
			dataset.dataset.size = datasetSize;
			RandomX::datasetAlloc(dataset, largePages);
			const uint64_t datasetBlockCount = datasetSize / RandomX::CacheLineSize;
			if (!legacy) {
				RandomX::JitCompilerX86 jit86;
				jit86.generateSuperScalarHash(programs);
				jit86.getDatasetInitFunc()(cache.memory, dataset.dataset.memory, 0, datasetBlockCount);
			//dump((const char*)dataset.dataset.memory, RANDOMX_DATASET_SIZE, "dataset.dat");
			}
			else {
				if (initThreadCount > 1) {
					auto perThread = datasetBlockCount / initThreadCount;
					auto remainder = datasetBlockCount % initThreadCount;
					for (int i = 0; i < initThreadCount; ++i) {
						auto count = perThread + (i == initThreadCount - 1 ? remainder : 0);
						threads.push_back(std::thread(&RandomX::datasetInit, std::ref(cache), std::ref(dataset.dataset), i * perThread, count));
					}
					for (unsigned i = 0; i < threads.size(); ++i) {
						threads[i].join();
					}
				}
				else {
					RandomX::datasetInit(cache, dataset.dataset, 0, datasetBlockCount);
				}
			}
			RandomX::deallocCache(cache, largePages);
			threads.clear();
			std::cout << "Dataset (" << datasetSize << " bytes) initialized in " << sw.getElapsed() << " s" << std::endl;
		}
		std::cout << "Initializing " << threadCount << " virtual machine(s) ..." << std::endl;
		for (int i = 0; i < threadCount; ++i) {
			RandomX::VirtualMachine* vm;
			if (miningMode) {
				vm = new RandomX::CompiledVirtualMachine();
			}
			else {
				if (jit && !legacy)
					vm = new RandomX::CompiledLightVirtualMachine<true>();
				else if (jit)
					vm = new RandomX::CompiledLightVirtualMachine<false>();
				else if (!legacy)
					vm = new RandomX::InterpretedVirtualMachine<true>(softAes);
				else
					vm = new RandomX::InterpretedVirtualMachine<false>(softAes);
			}
			vm->setDataset(dataset, datasetSize, programs);
			vms.push_back(vm);
		}
		uint8_t* scratchpadMem;
		if (largePages) {
			scratchpadMem = (uint8_t*)allocLargePagesMemory(threadCount * RANDOMX_SCRATCHPAD_L3);
		}
		else {
			scratchpadMem = (uint8_t*)_mm_malloc(threadCount * RANDOMX_SCRATCHPAD_L3, RandomX::CacheLineSize);
		}
		std::cout << "Running benchmark (" << programCount << " nonces) ..." << std::endl;
		sw.restart();
		if (threadCount > 1) {
			for (unsigned i = 0; i < vms.size(); ++i) {
				if (softAes)
					threads.push_back(std::thread(&mine<true>, vms[i], std::ref(atomicNonce), std::ref(result), programCount, i, scratchpadMem + RANDOMX_SCRATCHPAD_L3 * i));
				else
					threads.push_back(std::thread(&mine<false>, vms[i], std::ref(atomicNonce), std::ref(result), programCount, i, scratchpadMem + RANDOMX_SCRATCHPAD_L3 * i));
			}
			for (unsigned i = 0; i < threads.size(); ++i) {
				threads[i].join();
			}
		}
		else {
			if(softAes)
				mine<true>(vms[0], std::ref(atomicNonce), std::ref(result), programCount, 0, scratchpadMem);
			else
				mine<false>(vms[0], std::ref(atomicNonce), std::ref(result), programCount, 0, scratchpadMem);
			/*if (miningMode)
				std::cout << "Average program size: " << ((RandomX::CompiledVirtualMachine*)vms[0])->getTotalSize() / programCount / RandomX::ChainLength << std::endl;*/
		}
		double elapsed = sw.getElapsed();
		std::cout << "Calculated result: ";
		result.print(std::cout);
		if(!legacy && programCount == 1000)
		std::cout << "Reference result:  4a74a376d490c8b41d42887e86d4addb5a95572e0c663d1e81aec928e4e094e1" << std::endl;
		if (!miningMode) {
			std::cout << "Performance: " << 1000 * elapsed / programCount << " ms per hash" << std::endl;
		}
		else {
			std::cout << "Performance: " << programCount / elapsed << " hashes per second" << std::endl;
		}
	}
	catch (std::exception& e) {
		std::cout << "ERROR: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}