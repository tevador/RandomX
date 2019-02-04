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
//#define FPUCHECK
#include "InterpretedVirtualMachine.hpp"
#include "Pcg32.hpp"
#include "instructions.hpp"
#include "dataset.hpp"
#include "Cache.hpp"
#include "LightClientAsyncWorker.hpp"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <thread>
#include "intrinPortable.h"
#ifdef STATS
#include <algorithm>
#endif

#ifdef FPUCHECK
constexpr bool fpuCheck = true;
#else
constexpr bool fpuCheck = false;
#endif

namespace RandomX {

	InterpretedVirtualMachine::~InterpretedVirtualMachine() {
		if (asyncWorker) {
			delete mem.ds.asyncWorker;
		}
	}

	void InterpretedVirtualMachine::setDataset(dataset_t ds) {
		if (asyncWorker) {
			if (softAes) {
				mem.ds.asyncWorker = new LightClientAsyncWorker<true>(ds.cache);
			}
			else {
				mem.ds.asyncWorker = new LightClientAsyncWorker<false>(ds.cache);
			}
			readDataset = &datasetReadLightAsync;
		}
		else {
			mem.ds = ds;
			if (softAes) {
				readDataset = &datasetReadLight<true>;
			}
			else {
				readDataset = &datasetReadLight<false>;
			}
		}
	}

	void InterpretedVirtualMachine::initializeScratchpad(uint8_t* scratchpad, int32_t index) {
		uint32_t startingBlock = (ScratchpadSize / CacheLineSize) * index;
		if (asyncWorker) {
			ILightClientAsyncWorker* worker = mem.ds.asyncWorker;
			const uint32_t blocksPerThread = (ScratchpadSize / CacheLineSize) / 2;
			worker->prepareBlocks(scratchpad, startingBlock, blocksPerThread);                                       //async first half
			worker->getBlocks(scratchpad + ScratchpadLength / 2, startingBlock + blocksPerThread, blocksPerThread);  //sync second half
			worker->sync();
		}
		else {
			auto cache = mem.ds.cache;
			if (softAes) {
				for (int i = 0; i < ScratchpadSize / CacheLineSize; ++i) {
					initBlock<true>(cache->getCache(), ((uint8_t*)scratchpad) + CacheLineSize * i, (ScratchpadSize / CacheLineSize) * index + i, cache->getKeys());
				}
			}
			else {
				for (int i = 0; i < ScratchpadSize / CacheLineSize; ++i) {
					initBlock<false>(cache->getCache(), ((uint8_t*)scratchpad) + CacheLineSize * i, (ScratchpadSize / CacheLineSize) * index + i, cache->getKeys());
				}
			}
		}
	}

	void InterpretedVirtualMachine::initializeProgram(const void* seed) {
		Pcg32 gen(seed);
		for (unsigned i = 0; i < sizeof(reg) / sizeof(Pcg32::result_type); ++i) {
			*(((uint32_t*)&reg) + i) = gen();
		}
		initFpu();
		for (int i = 0; i < RegistersCount; ++i) {
			reg.f[i].lo.f64 = (double)reg.f[i].lo.i64;
			reg.f[i].hi.f64 = (double)reg.f[i].hi.i64;
		}
		//std::cout << reg;
		p.initialize(gen);
		currentTransform = addressTransformations[gen.getUniform(0, TransformationCount - 1)];
		mem.ma = (gen() ^ *(((uint32_t*)seed) + 4)) & ~7;
		mem.mx = *(((uint32_t*)seed) + 5);
		pc = 0;
		ic = InstructionCount;
		stack.clear();
	}

	void InterpretedVirtualMachine::execute() {
		for(int i = 0; i < InstructionCount; ++i) {
			for (int j = 0; j < ProgramLength; ++j) {
				auto& ibc = byteCode[j];
				switch (ibc.type)
				{
					case InstructionType::CFROUND: {
						uint64_t rcFlag = rotr(ibc.isrc->u64, ibc.imm.i32);
						setRoundMode(rcFlag);
						}
						break;
				}
			}
		}

	}

#include "instructionWeights.hpp"

	void InterpretedVirtualMachine::executeInstruction(Instruction& instr) {
		switch (instr.opcode)
		{
			CASE_REP(IADD_R)

				break;
		}
	}

	InstructionHandler InterpretedVirtualMachine::engine[256] = {

	};
}