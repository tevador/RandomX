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
#include <thread>
#include <atomic>
#include "dataset.hpp"
#include "Cache.hpp"

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
	void print(std::atomic<uint64_t>& hash, std::ostream& os) {
		auto h = hash.load();
		outputHex(std::cout, (char*)&h, sizeof(h));
	}
	std::atomic<uint64_t> hash[4];
};

void printUsage(const char* executable) {
	std::cout << "Usage: " << executable << " [OPTIONS]" << std::endl;
	std::cout << "Supported options:" << std::endl;
	std::cout << "\t--help\t\t\tshows this message" << std::endl;
	std::cout << "\t--compiled\t\tuse x86-64 JIT-compiled VM (default: interpreted VM)" << std::endl;
	std::cout << "\t--lightClient\t\tuse 'light-client' mode (default: full dataset mode)" << std::endl;
	std::cout << "\t--softAes\t\tuse software AES (default: x86 AES-NI)" << std::endl;
	std::cout << "\t--threads T\t\tuse T threads (default: 1)" << std::endl;
	std::cout << "\t--nonces N\t\trun N nonces (default: 1000)" << std::endl;
	std::cout << "\t--genAsm\t\tgenerate x86 asm code for nonce N" << std::endl;
}

void generateAsm(int nonce) {
	uint64_t hash[4];
	unsigned char blockTemplate[] = {
		0x07, 0x07, 0xf7, 0xa4, 0xf0, 0xd6, 0x05, 0xb3, 0x03, 0x26, 0x08, 0x16, 0xba, 0x3f, 0x10, 0x90, 0x2e, 0x1a, 0x14,
		0x5a, 0xc5, 0xfa, 0xd3, 0xaa, 0x3a, 0xf6, 0xea, 0x44, 0xc1, 0x18, 0x69, 0xdc, 0x4f, 0x85, 0x3f, 0x00, 0x2b, 0x2e,
		0xea, 0x00, 0x00, 0x00, 0x00, 0x77, 0xb2, 0x06, 0xa0, 0x2c, 0xa5, 0xb1, 0xd4, 0xce, 0x6b, 0xbf, 0xdf, 0x0a, 0xca,
		0xc3, 0x8b, 0xde, 0xd3, 0x4d, 0x2d, 0xcd, 0xee, 0xf9, 0x5c, 0xd2, 0x0c, 0xef, 0xc1, 0x2f, 0x61, 0xd5, 0x61, 0x09
	};
	int* noncePtr = (int*)(blockTemplate + 39);
	*noncePtr = nonce;
	blake2b(hash, sizeof(hash), blockTemplate, sizeof(blockTemplate), nullptr, 0);
	RandomX::AssemblyGeneratorX86 asmX86;
	asmX86.generateProgram(hash);
	asmX86.printCode(std::cout);
}

void mine(RandomX::VirtualMachine* vm, std::atomic<int>& atomicNonce, AtomicHash& result, int noncesCount, int thread) {
	uint64_t hash[4];
	unsigned char blockTemplate[] = {
		0x07, 0x07, 0xf7, 0xa4, 0xf0, 0xd6, 0x05, 0xb3, 0x03, 0x26, 0x08, 0x16, 0xba, 0x3f, 0x10, 0x90, 0x2e, 0x1a, 0x14,
		0x5a, 0xc5, 0xfa, 0xd3, 0xaa, 0x3a, 0xf6, 0xea, 0x44, 0xc1, 0x18, 0x69, 0xdc, 0x4f, 0x85, 0x3f, 0x00, 0x2b, 0x2e,
		0xea, 0x00, 0x00, 0x00, 0x00, 0x77, 0xb2, 0x06, 0xa0, 0x2c, 0xa5, 0xb1, 0xd4, 0xce, 0x6b, 0xbf, 0xdf, 0x0a, 0xca,
		0xc3, 0x8b, 0xde, 0xd3, 0x4d, 0x2d, 0xcd, 0xee, 0xf9, 0x5c, 0xd2, 0x0c, 0xef, 0xc1, 0x2f, 0x61, 0xd5, 0x61, 0x09
	};
	int* noncePtr = (int*)(blockTemplate + 39);
	int nonce = atomicNonce.fetch_add(1);

	while (nonce < noncesCount) {
		//std::cout << "Thread " << thread << " nonce " << nonce << std::endl;
		*noncePtr = nonce;
		blake2b(hash, sizeof(hash), blockTemplate, sizeof(blockTemplate), nullptr, 0);
		int spIndex = ((uint8_t*)hash)[24] | ((((uint8_t*)hash)[25] & 63) << 8);
		vm->initializeScratchpad(spIndex);
		vm->initializeProgram(hash);
		//dump((char*)((RandomX::CompiledVirtualMachine*)vm)->getProgram(), RandomX::CodeSize, "code-1337-jmp.txt");
		vm->execute();
		vm->getResult(hash);
		result.xorWith(hash);
		if (RandomX::trace) {
			std::cout << "Nonce: " << nonce << " ";
			outputHex(std::cout, (char*)hash, sizeof(hash));
			std::cout << std::endl;
		}
		nonce = atomicNonce.fetch_add(1);
	}
}

int main(int argc, char** argv) {
	bool softAes, lightClient, genAsm, compiled, help;
	int programCount, threadCount;
	readOption("--help", argc, argv, help);

	if (help) {
		printUsage(argv[0]);
		return 0;
	}

	readOption("--softAes", argc, argv, softAes);
	readOption("--lightClient", argc, argv, lightClient);
	readOption("--genAsm", argc, argv, genAsm);
	readOption("--compiled", argc, argv, compiled);
	readIntOption("--threads", argc, argv, threadCount, 1);
	readIntOption("--nonces", argc, argv, programCount, 1000);

	if (genAsm) {
		generateAsm(programCount);
		return 0;
	}

	std::atomic<int> atomicNonce(0);
	AtomicHash result;
	std::vector<RandomX::VirtualMachine*> vms;
	std::vector<std::thread> threads;
	RandomX::dataset_t dataset;

	if (softAes)
		std::cout << "Using software AES." << std::endl;
	std::cout << "Initializing..." << std::endl;

	try {
		Stopwatch sw(true);
		if (softAes) {
			RandomX::datasetInitCache<true>(seed, dataset);
		}
		else {
			RandomX::datasetInitCache<false>(seed, dataset);
		}
		if (RandomX::trace) {
			std::cout << "Keys: " << std::endl;
			for (int i = 0; i < dataset.cache->getKeys().size(); ++i) {
				outputHex(std::cout, (char*)&dataset.cache->getKeys()[i], sizeof(__m128i));
			}
			std::cout << std::endl;
			std::cout << "Cache: " << std::endl;
			outputHex(std::cout, (char*)dataset.cache->getCache(), sizeof(__m128i));
			std::cout << std::endl;
		}
		if (lightClient) {
			std::cout << "Cache (64 MiB) initialized in " << sw.getElapsed() << " s" << std::endl;
		}
		else {
			RandomX::Cache* cache = dataset.cache;
			RandomX::datasetAlloc(dataset);
			if (threadCount > 1) {
				auto perThread = RandomX::DatasetBlockCount / threadCount;
				auto remainder = RandomX::DatasetBlockCount % threadCount;
				for (int i = 0; i < threadCount; ++i) {
					auto count = perThread + (i == threadCount - 1 ? remainder : 0);
					if (softAes) {
						threads.push_back(std::thread(&RandomX::datasetInit<true>, cache, dataset, i * perThread, count));
					}
					else {
						threads.push_back(std::thread(&RandomX::datasetInit<false>, cache, dataset, i * perThread, count));
					}
				}
				for (int i = 0; i < threads.size(); ++i) {
					threads[i].join();
				}
			}
			else {
				if (softAes) {
					RandomX::datasetInit<true>(cache, dataset, 0, RandomX::DatasetBlockCount);
				}
				else {
					RandomX::datasetInit<false>(cache, dataset, 0, RandomX::DatasetBlockCount);
				}
			}
			delete cache;
			threads.clear();
			std::cout << "Dataset (4 GiB) initialized in " << sw.getElapsed() << " s" << std::endl;
		}
		std::cout << "Initializing " << threadCount << " virtual machine(s)..." << std::endl;
		for (int i = 0; i < threadCount; ++i) {
			RandomX::VirtualMachine* vm;
			if (compiled) {
				vm = new RandomX::CompiledVirtualMachine(softAes);
			}
			else {
				vm = new RandomX::InterpretedVirtualMachine(softAes);
			}
			vm->setDataset(dataset, lightClient);
			vms.push_back(vm);
		}
		std::cout << "Running benchmark (" << programCount << " programs) ..." << std::endl;
		sw.restart();
		if (threadCount > 1) {
			for (int i = 0; i < vms.size(); ++i) {
				threads.push_back(std::thread(&mine, vms[i], std::ref(atomicNonce), std::ref(result), programCount, i));
			}
			for (int i = 0; i < threads.size(); ++i) {
				threads[i].join();
			}
		}
		else {
			mine(vms[0], std::ref(atomicNonce), std::ref(result), programCount, 0);
		}
		double elapsed = sw.getElapsed();
		std::cout << "Calculated result: ";
		result.print(std::cout);
		if(programCount == 1000)
		std::cout << "Reference result:  3e1c5f9b9d0bf8ffa250f860bf5f7ab76ac823b206ddee6a592660119a3640c6" << std::endl;
		std::cout << "Performance: " << programCount / elapsed << " programs per second" << std::endl;
		/*if (threadCount == 1 && !compiled) {
			auto ivm = (RandomX::InterpretedVirtualMachine*)vms[0];
			std::cout << ivm->getProgam();
		}*/
	}
	catch (std::exception& e) {
		std::cout << "ERROR: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}