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

//#include "AssemblyGeneratorX86.hpp"
#include "stopwatch.hpp"
//#include "blake2/blake2.h"
#include "blake2/endian.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <exception>
#include <cstring>
//#include "Program.hpp"
#include <string>
#include <vector>
#include <thread>
#include <atomic>
//#include "hashAes1Rx4.hpp"
//#include "JitCompilerX86.hpp"
#include "randomx.h"

const uint8_t seed[32] = { 191, 182, 222, 175, 249, 89, 134, 104, 241, 68, 191, 62, 162, 166, 61, 64, 123, 191, 227, 193, 118, 60, 188, 53, 223, 133, 175, 24, 123, 230, 55, 74 };

const uint8_t blockTemplate_[] = {
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
	std::cout << "  --mine        mining mode: 2 GiB, x86-64 JIT compiled VM" << std::endl;
	std::cout << "  --verify      verification mode: 256 MiB" << std::endl;
	std::cout << "  --jit         x86-64 JIT compiled verification mode (default: interpreter)" << std::endl;
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
	/*alignas(16) uint64_t hash[8];
	uint8_t blockTemplate[sizeof(blockTemplate_)];
	memcpy(blockTemplate, blockTemplate_, sizeof(blockTemplate));
	store32(blockTemplate + 39, nonce);
	blake2b(hash, sizeof(hash), blockTemplate, sizeof(blockTemplate), nullptr, 0);
	uint8_t scratchpad[RANDOMX_SCRATCHPAD_L3];
	fillAes1Rx4<softAes>((void*)hash, RANDOMX_SCRATCHPAD_L3, scratchpad);
	randomx::AssemblyGeneratorX86 asmX86;
	randomx::Program p;
	fillAes1Rx4<softAes>(hash, sizeof(p), &p);
	asmX86.generateProgram(p);
	asmX86.printCode(std::cout);*/
}

template<bool softAes>
void generateNative(uint32_t nonce) {
	/*alignas(16) uint64_t hash[8];
	uint8_t blockTemplate[sizeof(blockTemplate_)];
	memcpy(blockTemplate, blockTemplate_, sizeof(blockTemplate));
	store32(blockTemplate + 39, nonce);
	blake2b(hash, sizeof(hash), blockTemplate, sizeof(blockTemplate), nullptr, 0);
	uint8_t scratchpad[RANDOMX_SCRATCHPAD_L3];
	fillAes1Rx4<softAes>((void*)hash, RANDOMX_SCRATCHPAD_L3, scratchpad);
	alignas(16) randomx::Program prog;
	fillAes1Rx4<softAes>((void*)hash, sizeof(prog), &prog);
	for (int i = 0; i < RANDOMX_PROGRAM_SIZE; ++i) {
		prog(i).dst %= 8;
		prog(i).src %= 8;
	}
	std::cout << prog << std::endl;*/
}

void mine(randomx_vm* vm, std::atomic<uint32_t>& atomicNonce, AtomicHash& result, uint32_t noncesCount, int thread) {
	uint64_t hash[RANDOMX_HASH_SIZE / 4];
	uint8_t blockTemplate[sizeof(blockTemplate_)];
	memcpy(blockTemplate, blockTemplate_, sizeof(blockTemplate));
	void* noncePtr = blockTemplate + 39;
	auto nonce = atomicNonce.fetch_add(1);

	while (nonce < noncesCount) {
		//std::cout << "Thread " << thread << " nonce " << nonce << std::endl;
		store32(noncePtr, nonce);
		
		randomx_calculate_hash(vm, blockTemplate, sizeof(blockTemplate), &hash);

		result.xorWith(hash);
		/*if (randomx::trace) {
			std::cout << "Nonce: " << nonce << " ";
			outputHex(std::cout, (char*)hash, 16);
			std::cout << std::endl;
		}*/
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

	/*if (genSuperscalar) {
		randomx::SuperscalarProgram p;
		randomx::Blake2Generator gen(seed, programCount);
		randomx::generateSuperscalar(p, gen);
		randomx::AssemblyGeneratorX86 asmX86;
		asmX86.generateAsm(p);
		//std::ofstream file("lightProg2.asm");
		asmX86.printCode(std::cout);
		return 0;
	}*/

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
	std::vector<randomx_vm*> vms;
	std::vector<std::thread> threads;
	randomx_dataset* dataset;
	randomx_cache* cache;
	randomx_flags flags = RANDOMX_FLAG_DEFAULT;

	if (miningMode) {
		flags = (randomx_flags)(flags | RANDOMX_FLAG_FULL_MEM);
		std::cout << "RandomX - full memory mode (2 GiB)" << std::endl;
	} else {
		std::cout << "RandomX - light memory mode (256 MiB)" << std::endl;
	}

	if (jit) {
		flags = (randomx_flags)(flags | RANDOMX_FLAG_JIT);
		std::cout << "RandomX - JIT compiled mode" << std::endl;
	}
	else {
		std::cout << "RandomX - interpreted mode" << std::endl;
	}

	if (softAes) {
		std::cout << "RandomX - software AES mode" << std::endl;
	}
	else {
		flags = (randomx_flags)(flags | RANDOMX_FLAG_HARD_AES);
		std::cout << "RandomX - hardware AES mode" << std::endl;
	}

	if (largePages) {
		flags = (randomx_flags)(flags | RANDOMX_FLAG_LARGE_PAGES);
		std::cout << "RandomX - large pages mode" << std::endl;
	}
	else {
		std::cout << "RandomX - small pages mode" << std::endl;
	}

	std::cout << "Initializing";
	if(miningMode)
		std::cout << " (" << initThreadCount << " thread" << (initThreadCount > 1 ? "s)" : ")");
	std::cout << " ..." << std::endl;

	try {
		Stopwatch sw(true);
		cache = randomx_alloc_cache(flags);
		randomx_init_cache(cache, seed, sizeof(seed));
		/*if (randomx::trace) {
			std::cout << "Cache: " << std::endl;
			outputHex(std::cout, (char*)dataset.cache.memory, sizeof(__m128i));
			std::cout << std::endl;
		}*/
		if (miningMode) {
			dataset = randomx_alloc_dataset(flags);
			if (initThreadCount > 1) {
				auto perThread = RANDOMX_DATASET_BLOCKS / initThreadCount;
				auto remainder = RANDOMX_DATASET_BLOCKS % initThreadCount;
				uint32_t startBlock = 0;
				for (int i = 0; i < initThreadCount; ++i) {
					auto count = perThread + (i == initThreadCount - 1 ? remainder : 0);
					threads.push_back(std::thread(&randomx_init_dataset, dataset, cache, startBlock, count));
					startBlock += count;
				}
				for (unsigned i = 0; i < threads.size(); ++i) {
					threads[i].join();
				}
			}
			else {
				randomx_init_dataset(dataset, cache, 0, RANDOMX_DATASET_BLOCKS);
			}
			//dump((const char*)dataset.dataset.memory, RANDOMX_DATASET_SIZE, "dataset.dat");
			randomx_release_cache(cache);
			threads.clear();
		}
		std::cout << "Memory initialized in " << sw.getElapsed() << " s" << std::endl;
		std::cout << "Initializing " << threadCount << " virtual machine(s) ..." << std::endl;
		for (int i = 0; i < threadCount; ++i) {
			randomx_vm *vm = randomx_create_vm(flags);
			if (miningMode)
				randomx_vm_set_dataset(vm, dataset);
			else
				randomx_vm_set_cache(vm, cache);
			vms.push_back(vm);
		}
		std::cout << "Running benchmark (" << programCount << " nonces) ..." << std::endl;
		sw.restart();
		if (threadCount > 1) {
			for (unsigned i = 0; i < vms.size(); ++i) {
				if (softAes)
					threads.push_back(std::thread(&mine, vms[i], std::ref(atomicNonce), std::ref(result), programCount, i));
				else
					threads.push_back(std::thread(&mine, vms[i], std::ref(atomicNonce), std::ref(result), programCount, i));
			}
			for (unsigned i = 0; i < threads.size(); ++i) {
				threads[i].join();
			}
		}
		else {
			mine(vms[0], std::ref(atomicNonce), std::ref(result), programCount, 0);
		}
		double elapsed = sw.getElapsed();
		std::cout << "Calculated result: ";
		result.print(std::cout);
		if(!legacy && programCount == 1000)
		std::cout << "Reference result:  630ad3bc7f44fe8386462d7b671fa2a1167d3e062bfb9a2967f64832760cfedb" << std::endl;
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