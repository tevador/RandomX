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
#include "dataset.hpp"
#include "Cache.hpp"
#include "LightClientAsyncWorker.hpp"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <cfloat>
#include <thread>
#include <climits>
#include "intrinPortable.h"
#include "reciprocal.h"
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

	void InterpretedVirtualMachine::setDataset(dataset_t ds, uint64_t size) {
		if (asyncWorker) {
			if (softAes) {
				mem.ds.asyncWorker = new LightClientAsyncWorker(ds.cache);
			}
			else {
				mem.ds.asyncWorker = new LightClientAsyncWorker(ds.cache);
			}
			readDataset = &datasetReadLightAsync;
		}
		else {
			mem.ds = ds;
			readDataset = &datasetReadLight;
		}
		datasetRange = (size - RANDOMX_DATASET_SIZE + CacheLineSize) / CacheLineSize;
	}

	void InterpretedVirtualMachine::initialize() {
		VirtualMachine::initialize();
		for (unsigned i = 0; i < RANDOMX_PROGRAM_SIZE; ++i) {
			program(i).src %= RegistersCount;
			program(i).dst %= RegistersCount;
		}
	}

	template<int N>
	void InterpretedVirtualMachine::executeBytecode(int_reg_t(&r)[8], __m128d (&f)[4], __m128d (&e)[4], __m128d (&a)[4]) {
		executeBytecode(N, r, f, e, a);
		executeBytecode<N + 1>(r, f, e, a);
	}

	template<>
	void InterpretedVirtualMachine::executeBytecode<RANDOMX_PROGRAM_SIZE>(int_reg_t(&r)[8], __m128d (&f)[4], __m128d (&e)[4], __m128d (&a)[4]) {
	}

	static void print(int_reg_t r) {
		std::cout << std::hex << std::setw(16) << std::setfill('0') << r << std::endl;
	}

	static void print(__m128d f) {
		uint64_t lo = *(((uint64_t*)&f) + 0);
		uint64_t hi = *(((uint64_t*)&f) + 1);
		std::cout << std::hex << std::setw(16) << std::setfill('0') << hi << '-' << std::hex << std::setw(16) << std::setfill('0') << lo << std::endl;
	}

	static void printState(int_reg_t(&r)[8], __m128d (&f)[4], __m128d (&e)[4], __m128d (&a)[4]) {
		for (int i = 0; i < 8; ++i) {
			std::cout << "r" << i << " = "; print(r[i]);
		}
		for (int i = 0; i < 4; ++i) {
			std::cout << "f" << i << " = "; print(f[i]);
		}
		for (int i = 0; i < 4; ++i) {
			std::cout << "e" << i << " = "; print(e[i]);
		}
		for (int i = 0; i < 4; ++i) {
			std::cout << "a" << i << " = "; print(a[i]);
		}
	}

	static bool isDenormal(double x) {
		return std::fpclassify(x) == FP_SUBNORMAL;
	}

	FORCE_INLINE void InterpretedVirtualMachine::executeBytecode(int i, int_reg_t(&r)[8], __m128d (&f)[4], __m128d (&e)[4], __m128d (&a)[4]) {
		auto& ibc = byteCode[i];
		//if(trace) printState(r, f, e, a);
		switch (ibc.type)
		{
			case InstructionType::IADD_R: {
				*ibc.idst += *ibc.isrc;
			} break;

			case InstructionType::IADD_M: {
				*ibc.idst += load64(scratchpad + (*ibc.isrc & ibc.memMask));
			} break;

			case InstructionType::IADD_RC: {
				*ibc.idst += *ibc.isrc + ibc.imm;
			} break;

			case InstructionType::ISUB_R: {
				*ibc.idst -= *ibc.isrc;
			} break;

			case InstructionType::ISUB_M: {
				*ibc.idst -= load64(scratchpad + (*ibc.isrc & ibc.memMask));
			} break;

			case InstructionType::IMUL_9C: {
				*ibc.idst += 8 * *ibc.idst + ibc.imm;
			} break;

			case InstructionType::IMUL_R: { //also handles IMUL_RCP
				*ibc.idst *= *ibc.isrc;
			} break;

			case InstructionType::IMUL_M: {
				*ibc.idst *= load64(scratchpad + (*ibc.isrc & ibc.memMask));
			} break;

			case InstructionType::IMULH_R: {
				*ibc.idst = mulh(*ibc.idst, *ibc.isrc);
			} break;

			case InstructionType::IMULH_M: {
				*ibc.idst = mulh(*ibc.idst, load64(scratchpad + (*ibc.isrc & ibc.memMask)));
			} break;

			case InstructionType::ISMULH_R: {
				*ibc.idst = smulh(unsigned64ToSigned2sCompl(*ibc.idst), unsigned64ToSigned2sCompl(*ibc.isrc));
			} break;

			case InstructionType::ISMULH_M: {
				*ibc.idst = smulh(unsigned64ToSigned2sCompl(*ibc.idst), unsigned64ToSigned2sCompl(load64(scratchpad + (*ibc.isrc & ibc.memMask))));
			} break;

			case InstructionType::INEG_R: {
				*ibc.idst = ~(*ibc.idst) + 1; //two's complement negative
			} break;

			case InstructionType::IXOR_R: {
				*ibc.idst ^= *ibc.isrc;
			} break;

			case InstructionType::IXOR_M: {
				*ibc.idst ^= load64(scratchpad + (*ibc.isrc & ibc.memMask));
			} break;

			case InstructionType::IROR_R: {
				*ibc.idst = rotr(*ibc.idst, *ibc.isrc & 63);
			} break;

			case InstructionType::IROL_R: {
				*ibc.idst = rotl(*ibc.idst, *ibc.isrc & 63);
			} break;

			case InstructionType::ISWAP_R: {
				int_reg_t temp = *ibc.isrc;
				*ibc.isrc = *ibc.idst;
				*ibc.idst = temp;
			} break;

			case InstructionType::FSWAP_R: {
				*ibc.fdst = _mm_shuffle_pd(*ibc.fdst, *ibc.fdst, 1);
			} break;

			case InstructionType::FADD_R: {
				*ibc.fdst = _mm_add_pd(*ibc.fdst, *ibc.fsrc);
			} break;

			case InstructionType::FADD_M: {
				__m128d fsrc = load_cvt_i32x2(scratchpad + (*ibc.isrc & ibc.memMask));
				*ibc.fdst = _mm_add_pd(*ibc.fdst, fsrc);
			} break;

			case InstructionType::FSUB_R: {
				*ibc.fdst = _mm_sub_pd(*ibc.fdst, *ibc.fsrc);
			} break;

			case InstructionType::FSUB_M: {
				__m128d fsrc = load_cvt_i32x2(scratchpad + (*ibc.isrc & ibc.memMask));
				*ibc.fdst = _mm_sub_pd(*ibc.fdst, fsrc);
			} break;

			case InstructionType::FSCAL_R: {
				const __m128d mask = _mm_castsi128_pd(_mm_set1_epi64x(0x81F0000000000000));
				*ibc.fdst = _mm_xor_pd(*ibc.fdst, mask);
			} break;

			case InstructionType::FMUL_R: {
				*ibc.fdst = _mm_mul_pd(*ibc.fdst, *ibc.fsrc);
			} break;

			case InstructionType::FDIV_M: {
				__m128d fsrc = ieee_set_exponent<-240>(load_cvt_i32x2(scratchpad + (*ibc.isrc & ibc.memMask)));
				*ibc.fdst = _mm_div_pd(*ibc.fdst, fsrc);
			} break;

			case InstructionType::FSQRT_R: {
				*ibc.fdst = _mm_sqrt_pd(*ibc.fdst);
			} break;

			case InstructionType::COND_R: {
				*ibc.idst += condition(ibc.condition, *ibc.isrc, ibc.imm) ? 1 : 0;
			} break;

			case InstructionType::COND_M: {
				*ibc.idst += condition(ibc.condition, load64(scratchpad + (*ibc.isrc & ibc.memMask)), ibc.imm) ? 1 : 0;
			} break;

			case InstructionType::CFROUND: {
				setRoundMode(rotr(*ibc.isrc, ibc.imm) % 4);
			} break;

			case InstructionType::ISTORE: {
				store64(scratchpad + (*ibc.idst & ibc.memMask), *ibc.isrc);
			} break;

			case InstructionType::NOP: {
				//nothing
			} break;

			default:
				UNREACHABLE;
		}
		if (trace) {
			std::cout << program(i);
			if(ibc.type < 20 || ibc.type == 31 || ibc.type == 32)
				print(*ibc.idst);
			else //if(ibc.type >= 20 && ibc.type <= 30)
				print(0);
		}
#ifdef FPUCHECK
		if (ibc.type >= 26 && ibc.type <= 30) {
			double lo = *(((double*)ibc.fdst) + 0);
			double hi = *(((double*)ibc.fdst) + 1);
			if (lo <= 0 || hi <= 0) {
				std::stringstream ss;
				ss << "Underflow in operation " << ibc.type;
				printState(r, f, e, a);
				throw std::runtime_error(ss.str());
			}
		}
#endif
	}

	void InterpretedVirtualMachine::execute() {
		int_reg_t r[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
		__m128d f[4];
		__m128d e[4];
		__m128d a[4];

		a[0] = _mm_load_pd(&reg.a[0].lo);
		a[1] = _mm_load_pd(&reg.a[1].lo);
		a[2] = _mm_load_pd(&reg.a[2].lo);
		a[3] = _mm_load_pd(&reg.a[3].lo);

		precompileProgram(r, f, e, a);

		uint32_t spAddr0 = mem.mx;
		uint32_t spAddr1 = mem.ma;

		if (trace) {
			std::cout << "execute (reg: r" << readReg0 << ", r" << readReg1 << ", r" << readReg2 << ", r" << readReg3 << ")" << std::endl;
			std::cout << "spAddr " << std::hex << std::setw(8) << std::setfill('0') << spAddr1 << " / " << std::setw(8) << std::setfill('0') << spAddr0 << std::endl;
			std::cout << "ma/mx " << std::hex << std::setw(8) << std::setfill('0') << mem.ma << std::setw(8) << std::setfill('0') << mem.mx << std::endl;
			printState(r, f, e, a);
		}

		for(unsigned ic = 0; ic < RANDOMX_PROGRAM_ITERATIONS; ++ic) {
			//std::cout << "Iteration " << iter << std::endl;
			uint64_t spMix = r[readReg0] ^ r[readReg1];
			spAddr0 ^= spMix;
			spAddr0 &= ScratchpadL3Mask64;
			spAddr1 ^= spMix >> 32;
			spAddr1 &= ScratchpadL3Mask64;
			
			r[0] ^= load64(scratchpad + spAddr0 + 0);
			r[1] ^= load64(scratchpad + spAddr0 + 8);
			r[2] ^= load64(scratchpad + spAddr0 + 16);
			r[3] ^= load64(scratchpad + spAddr0 + 24);
			r[4] ^= load64(scratchpad + spAddr0 + 32);
			r[5] ^= load64(scratchpad + spAddr0 + 40);
			r[6] ^= load64(scratchpad + spAddr0 + 48);
			r[7] ^= load64(scratchpad + spAddr0 + 56);

			f[0] = load_cvt_i32x2(scratchpad + spAddr1 + 0);
			f[1] = load_cvt_i32x2(scratchpad + spAddr1 + 8);
			f[2] = load_cvt_i32x2(scratchpad + spAddr1 + 16);
			f[3] = load_cvt_i32x2(scratchpad + spAddr1 + 24);
			e[0] = ieee_set_exponent<-240>(load_cvt_i32x2(scratchpad + spAddr1 + 32));
			e[1] = ieee_set_exponent<-240>(load_cvt_i32x2(scratchpad + spAddr1 + 40));
			e[2] = ieee_set_exponent<-240>(load_cvt_i32x2(scratchpad + spAddr1 + 48));
			e[3] = ieee_set_exponent<-240>(load_cvt_i32x2(scratchpad + spAddr1 + 56));

			if (trace) {
				std::cout << "iteration " << std::dec << ic << std::endl;
				std::cout << "spAddr " << std::hex << std::setw(8) << std::setfill('0') << spAddr1 << " / " << std::setw(8) << std::setfill('0') << spAddr0 << std::endl;
				std::cout << "ma/mx " << std::hex << std::setw(8) << std::setfill('0') << mem.ma << std::setw(8) << std::setfill('0') << mem.mx << std::endl;
				printState(r, f, e, a);
				std::cout << "-----------------------------------" << std::endl;
			}

			executeBytecode<0>(r, f, e, a);

			if (asyncWorker) {
				ILightClientAsyncWorker* aw = mem.ds.asyncWorker;
				const uint64_t* datasetLine = aw->getBlock(datasetBase + mem.ma);
				for (int i = 0; i < RegistersCount; ++i)
					r[i] ^= datasetLine[i];
				mem.mx ^= r[readReg2] ^ r[readReg3];
				mem.mx &= CacheLineAlignMask; //align to cache line
				std::swap(mem.mx, mem.ma);
				aw->prepareBlock(datasetBase + mem.ma);
			}
			else {
				mem.mx ^= r[readReg2] ^ r[readReg3];
				//mem.mx &= CacheLineAlignMask;
				Cache& cache = mem.ds.cache;
				uint64_t datasetLine[CacheLineSize / sizeof(uint64_t)];
				initBlock(cache, (uint8_t*)datasetLine, datasetBase + mem.ma / CacheLineSize, RANDOMX_CACHE_ACCESSES / 8);
				for (int i = 0; i < RegistersCount; ++i)
					r[i] ^= datasetLine[i];
				std::swap(mem.mx, mem.ma);
			}

			if (trace) {
				std::cout << "iteration " << std::dec << ic << std::endl;
				std::cout << "spAddr " << std::hex << std::setw(8) << std::setfill('0') << spAddr1 << " / " << std::setw(8) << std::setfill('0') << spAddr0 << std::endl;
				std::cout << "ma/mx " << std::hex << std::setw(8) << std::setfill('0') << mem.ma << std::setw(8) << std::setfill('0') << mem.mx << std::endl;
				printState(r, f, e, a);
				std::cout << "===================================" << std::endl;
			}

			store64(scratchpad + spAddr1 + 0, r[0]);
			store64(scratchpad + spAddr1 + 8, r[1]);
			store64(scratchpad + spAddr1 + 16, r[2]);
			store64(scratchpad + spAddr1 + 24, r[3]);
			store64(scratchpad + spAddr1 + 32, r[4]);
			store64(scratchpad + spAddr1 + 40, r[5]);
			store64(scratchpad + spAddr1 + 48, r[6]);
			store64(scratchpad + spAddr1 + 56, r[7]);

			f[0] = _mm_xor_pd(f[0], e[0]);
			f[1] = _mm_xor_pd(f[1], e[1]);
			f[2] = _mm_xor_pd(f[2], e[2]);
			f[3] = _mm_xor_pd(f[3], e[3]);

#ifdef FPUCHECK
			for(int i = 0; i < 4; ++i) {
				double lo = *(((double*)&f[i]) + 0);
				double hi = *(((double*)&f[i]) + 1);
				if (isDenormal(lo) || isDenormal(hi)) {
					std::stringstream ss;
					ss << "Denormal f" << i;
					throw std::runtime_error(ss.str());
				}
			}
#endif

			_mm_store_pd((double*)(scratchpad + spAddr0 + 0), f[0]);
			_mm_store_pd((double*)(scratchpad + spAddr0 + 16), f[1]);
			_mm_store_pd((double*)(scratchpad + spAddr0 + 32), f[2]);
			_mm_store_pd((double*)(scratchpad + spAddr0 + 48), f[3]);

			spAddr0 = 0;
			spAddr1 = 0;
		}

		store64(&reg.r[0], r[0]);
		store64(&reg.r[1], r[1]);
		store64(&reg.r[2], r[2]);
		store64(&reg.r[3], r[3]);
		store64(&reg.r[4], r[4]);
		store64(&reg.r[5], r[5]);
		store64(&reg.r[6], r[6]);
		store64(&reg.r[7], r[7]);

		_mm_store_pd(&reg.f[0].lo, f[0]);
		_mm_store_pd(&reg.f[1].lo, f[1]);
		_mm_store_pd(&reg.f[2].lo, f[2]);
		_mm_store_pd(&reg.f[3].lo, f[3]);
		_mm_store_pd(&reg.e[0].lo, e[0]);
		_mm_store_pd(&reg.e[1].lo, e[1]);
		_mm_store_pd(&reg.e[2].lo, e[2]);
		_mm_store_pd(&reg.e[3].lo, e[3]);
	}

#include "instructionWeights.hpp"

	void InterpretedVirtualMachine::precompileProgram(int_reg_t(&r)[8], __m128d (&f)[4], __m128d (&e)[4], __m128d (&a)[4]) {
		for (unsigned i = 0; i < RANDOMX_PROGRAM_SIZE; ++i) {
			auto& instr = program(i);
			auto& ibc = byteCode[i];
			switch (instr.opcode) {
				CASE_REP(IADD_R) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::IADD_R;
					ibc.idst = &r[dst];
					if (src != dst) {
						ibc.isrc = &r[src];
					}
					else {
						ibc.imm = signExtend2sCompl(instr.getImm32());
						ibc.isrc = &ibc.imm;
					}
				} break;

				CASE_REP(IADD_M) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::IADD_M;
					ibc.idst = &r[dst];
					if (instr.src != instr.dst) {
						ibc.isrc = &r[src];
						ibc.memMask = ((instr.mod % 4) ? ScratchpadL1Mask : ScratchpadL2Mask);
					}
					else {
						ibc.imm = instr.getImm32();
						ibc.isrc = &ibc.imm;
						ibc.memMask = ScratchpadL3Mask;
					}
				} break;

				CASE_REP(IADD_RC) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::IADD_RC;
					ibc.idst = &r[dst];
					ibc.isrc = &r[src];
					ibc.imm = signExtend2sCompl(instr.getImm32());
				} break;

				CASE_REP(ISUB_R) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::ISUB_R;
					ibc.idst = &r[dst];
					if (src != dst) {
						ibc.isrc = &r[src];
					}
					else {
						ibc.imm = signExtend2sCompl(instr.getImm32());
						ibc.isrc = &ibc.imm;
					}
				} break;

				CASE_REP(ISUB_M) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::ISUB_M;
					ibc.idst = &r[dst];
					if (instr.src != instr.dst) {
						ibc.isrc = &r[src];
						ibc.memMask = ((instr.mod % 4) ? ScratchpadL1Mask : ScratchpadL2Mask);
					}
					else {
						ibc.imm = instr.getImm32();
						ibc.isrc = &ibc.imm;
						ibc.memMask = ScratchpadL3Mask;
					}
				} break;

				CASE_REP(IMUL_9C) {
					auto dst = instr.dst % RegistersCount;
					ibc.type = InstructionType::IMUL_9C;
					ibc.idst = &r[dst];
					ibc.imm = signExtend2sCompl(instr.getImm32());
				} break;

				CASE_REP(IMUL_R) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::IMUL_R;
					ibc.idst = &r[dst];
					if (src != dst) {
						ibc.isrc = &r[src];
					}
					else {
						ibc.imm = signExtend2sCompl(instr.getImm32());
						ibc.isrc = &ibc.imm;
					}
				} break;

				CASE_REP(IMUL_M) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::IMUL_M;
					ibc.idst = &r[dst];
					if (instr.src != instr.dst) {
						ibc.isrc = &r[src];
						ibc.memMask = ((instr.mod % 4) ? ScratchpadL1Mask : ScratchpadL2Mask);
					}
					else {
						ibc.imm = instr.getImm32();
						ibc.isrc = &ibc.imm;
						ibc.memMask = ScratchpadL3Mask;
					}
				} break;

				CASE_REP(IMULH_R) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::IMULH_R;
					ibc.idst = &r[dst];
					ibc.isrc = &r[src];
				} break;

				CASE_REP(IMULH_M) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::IMULH_M;
					ibc.idst = &r[dst];
					if (instr.src != instr.dst) {
						ibc.isrc = &r[src];
						ibc.memMask = ((instr.mod % 4) ? ScratchpadL1Mask : ScratchpadL2Mask);
					}
					else {
						ibc.imm = instr.getImm32();
						ibc.isrc = &ibc.imm;
						ibc.memMask = ScratchpadL3Mask;
					}
				} break;

				CASE_REP(ISMULH_R) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::ISMULH_R;
					ibc.idst = &r[dst];
					ibc.isrc = &r[src];
				} break;

				CASE_REP(ISMULH_M) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::ISMULH_M;
					ibc.idst = &r[dst];
					if (instr.src != instr.dst) {
						ibc.isrc = &r[src];
						ibc.memMask = ((instr.mod % 4) ? ScratchpadL1Mask : ScratchpadL2Mask);
					}
					else {
						ibc.imm = instr.getImm32();
						ibc.isrc = &ibc.imm;
						ibc.memMask = ScratchpadL3Mask;
					}
				} break;

				CASE_REP(IMUL_RCP) {
					uint32_t divisor = instr.getImm32();
					if (divisor != 0) {
						auto dst = instr.dst % RegistersCount;
						ibc.type = InstructionType::IMUL_R;
						ibc.idst = &r[dst];
						ibc.imm = reciprocal(divisor);
						ibc.isrc = &ibc.imm;
					}
					else {
						ibc.type = InstructionType::NOP;
					}
				} break;

				CASE_REP(INEG_R) {
					auto dst = instr.dst % RegistersCount;
					ibc.type = InstructionType::INEG_R;
					ibc.idst = &r[dst];
				} break;

				CASE_REP(IXOR_R) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::IXOR_R;
					ibc.idst = &r[dst];
					if (src != dst) {
						ibc.isrc = &r[src];
					}
					else {
						ibc.imm = signExtend2sCompl(instr.getImm32());
						ibc.isrc = &ibc.imm;
					}
				} break;

				CASE_REP(IXOR_M) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::IXOR_M;
					ibc.idst = &r[dst];
					if (instr.src != instr.dst) {
						ibc.isrc = &r[src];
						ibc.memMask = ((instr.mod % 4) ? ScratchpadL1Mask : ScratchpadL2Mask);
					}
					else {
						ibc.imm = instr.getImm32();
						ibc.isrc = &ibc.imm;
						ibc.memMask = ScratchpadL3Mask;
					}
				} break;

				CASE_REP(IROR_R) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::IROR_R;
					ibc.idst = &r[dst];
					if (src != dst) {
						ibc.isrc = &r[src];
					}
					else {
						ibc.imm = instr.getImm32();
						ibc.isrc = &ibc.imm;
					}
				} break;

				CASE_REP(IROL_R) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::IROL_R;
					ibc.idst = &r[dst];
					if (src != dst) {
						ibc.isrc = &r[src];
					}
					else {
						ibc.imm = instr.getImm32();
						ibc.isrc = &ibc.imm;
					}
				} break;

				CASE_REP(ISWAP_R) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					if (src != dst) {
						ibc.idst = &r[dst];
						ibc.isrc = &r[src];
						ibc.type = InstructionType::ISWAP_R;
					}
					else {
						ibc.type = InstructionType::NOP;
					}
				} break;

				CASE_REP(FSWAP_R) {
					auto dst = instr.dst % RegistersCount;
					ibc.type = InstructionType::FSWAP_R;
					if (dst < 4)
						ibc.fdst = &f[dst];
					else
						ibc.fdst = &e[dst - 4];
				} break;

				CASE_REP(FADD_R) {
					auto dst = instr.dst % 4;
					auto src = instr.src % 4;
					ibc.type = InstructionType::FADD_R;
					ibc.fdst = &f[dst];
					ibc.fsrc = &a[src];
				} break;

				CASE_REP(FADD_M) {
					auto dst = instr.dst % 4;
					auto src = instr.src % 8;
					ibc.type = InstructionType::FADD_M;
					ibc.fdst = &f[dst];
					ibc.isrc = &r[src];
					ibc.memMask = ((instr.mod % 4) ? ScratchpadL1Mask : ScratchpadL2Mask);
				} break;

				CASE_REP(FSUB_R) {
					auto dst = instr.dst % 4;
					auto src = instr.src % 4;
					ibc.type = InstructionType::FSUB_R;
					ibc.fdst = &f[dst];
					ibc.fsrc = &a[src];
				} break;

				CASE_REP(FSUB_M) {
					auto dst = instr.dst % 4;
					auto src = instr.src % 8;
					ibc.type = InstructionType::FSUB_M;
					ibc.fdst = &f[dst];
					ibc.isrc = &r[src];
					ibc.memMask = ((instr.mod % 4) ? ScratchpadL1Mask : ScratchpadL2Mask);
				} break;

				CASE_REP(FSCAL_R) {
					auto dst = instr.dst % 4;
					ibc.fdst = &f[dst];
					ibc.type = InstructionType::FSCAL_R;
				} break;

				CASE_REP(FMUL_R) {
					auto dst = instr.dst % 4;
					auto src = instr.src % 4;
					ibc.type = InstructionType::FMUL_R;
					ibc.fdst = &e[dst];
					ibc.fsrc = &a[src];
				} break;

				CASE_REP(FDIV_M) {
					auto dst = instr.dst % 4;
					auto src = instr.src % 8;
					ibc.type = InstructionType::FDIV_M;
					ibc.fdst = &e[dst];
					ibc.isrc = &r[src];
					ibc.memMask = ((instr.mod % 4) ? ScratchpadL1Mask : ScratchpadL2Mask);
				} break;

				CASE_REP(FSQRT_R) {
					auto dst = instr.dst % 4;
					ibc.type = InstructionType::FSQRT_R;
					ibc.fdst = &e[dst];
				} break;

				CASE_REP(COND_R) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::COND_R;
					ibc.idst = &r[dst];
					ibc.isrc = &r[src];
					ibc.condition = (instr.mod >> 2) & 7;
					ibc.imm = instr.getImm32();
				} break;

				CASE_REP(COND_M) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::COND_M;
					ibc.idst = &r[dst];
					ibc.isrc = &r[src];
					ibc.condition = (instr.mod >> 2) & 7;
					ibc.imm = instr.getImm32();
					ibc.memMask = ((instr.mod % 4) ? ScratchpadL1Mask : ScratchpadL2Mask);
				} break;

				CASE_REP(CFROUND) {
					auto src = instr.src % 8;
					ibc.isrc = &r[src];
					ibc.type = InstructionType::CFROUND;
					ibc.imm = instr.getImm32() & 63;
				} break;

				CASE_REP(ISTORE) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::ISTORE;
					ibc.idst = &r[dst];
					ibc.isrc = &r[src];
					ibc.memMask = ((instr.mod % 4) ? ScratchpadL1Mask : ScratchpadL2Mask);
				} break;

				CASE_REP(NOP) {
					ibc.type = InstructionType::NOP;
				} break;

				default:
					UNREACHABLE;
			}
		}
	}
}