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

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <cfloat>
#include <climits>
#include "vm_interpreted.hpp"
#include "dataset.hpp"
#include "intrin_portable.h"
#include "reciprocal.h"

namespace randomx {

	static int_reg_t Zero = 0;

	template<class Allocator, bool softAes>
	void InterpretedVm<Allocator, softAes>::setDataset(randomx_dataset* dataset) {
		datasetPtr = dataset;
		mem.memory = dataset->memory;
	}

	template<class Allocator, bool softAes>
	void InterpretedVm<Allocator, softAes>::run(void* seed) {
		VmBase<Allocator, softAes>::generateProgram(seed);
		randomx_vm::initialize();
		execute();
	}

	template<class Allocator, bool softAes>
	void InterpretedVm<Allocator, softAes>::executeBytecode(int_reg_t(&r)[RegistersCount], __m128d (&f)[RegisterCountFlt], __m128d (&e)[RegisterCountFlt], __m128d (&a)[RegisterCountFlt]) {
		for (int pc = 0; pc < RANDOMX_PROGRAM_SIZE; ++pc) {
			executeBytecode(pc, r, f, e, a);
		}
	}

	template<class Allocator, bool softAes>
	FORCE_INLINE void* InterpretedVm<Allocator, softAes>::getScratchpadAddress(InstructionByteCode& ibc) {
		uint32_t addr = (*ibc.isrc + ibc.imm) & ibc.memMask;
		return scratchpad + addr;
	}

	template<class Allocator, bool softAes>
	FORCE_INLINE __m128d InterpretedVm<Allocator, softAes>::maskRegisterExponentMantissa(__m128d x) {
		constexpr uint64_t mantissaMask64 = (1ULL << 52) - 1;
		const __m128d mantissaMask = _mm_castsi128_pd(_mm_set_epi64x(mantissaMask64, mantissaMask64));
		const __m128d exponentMask = _mm_load_pd((const double*)&config.eMask);
		x = _mm_and_pd(x, mantissaMask);
		x = _mm_or_pd(x, exponentMask);
		return x;
	}

	template<class Allocator, bool softAes>
	void InterpretedVm<Allocator, softAes>::executeBytecode(int& pc, int_reg_t(&r)[RegistersCount], __m128d (&f)[RegisterCountFlt], __m128d (&e)[RegisterCountFlt], __m128d (&a)[RegisterCountFlt]) {
		auto& ibc = byteCode[pc];
		switch (ibc.type)
		{
			case InstructionType::IADD_RS: {
				*ibc.idst += (*ibc.isrc << ibc.shift) + ibc.imm;
			} break;

			case InstructionType::IADD_M: {
				*ibc.idst += load64(getScratchpadAddress(ibc));
			} break;

			case InstructionType::ISUB_R: {
				*ibc.idst -= *ibc.isrc;
			} break;

			case InstructionType::ISUB_M: {
				*ibc.idst -= load64(getScratchpadAddress(ibc));
			} break;

			case InstructionType::IMUL_R: { //also handles IMUL_RCP
				*ibc.idst *= *ibc.isrc;
			} break;

			case InstructionType::IMUL_M: {
				*ibc.idst *= load64(getScratchpadAddress(ibc));
			} break;

			case InstructionType::IMULH_R: {
				*ibc.idst = mulh(*ibc.idst, *ibc.isrc);
			} break;

			case InstructionType::IMULH_M: {
				*ibc.idst = mulh(*ibc.idst, load64(getScratchpadAddress(ibc)));
			} break;

			case InstructionType::ISMULH_R: {
				*ibc.idst = smulh(unsigned64ToSigned2sCompl(*ibc.idst), unsigned64ToSigned2sCompl(*ibc.isrc));
			} break;

			case InstructionType::ISMULH_M: {
				*ibc.idst = smulh(unsigned64ToSigned2sCompl(*ibc.idst), unsigned64ToSigned2sCompl(load64(getScratchpadAddress(ibc))));
			} break;

			case InstructionType::INEG_R: {
				*ibc.idst = ~(*ibc.idst) + 1; //two's complement negative
			} break;

			case InstructionType::IXOR_R: {
				*ibc.idst ^= *ibc.isrc;
			} break;

			case InstructionType::IXOR_M: {
				*ibc.idst ^= load64(getScratchpadAddress(ibc));
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
				__m128d fsrc = load_cvt_i32x2(getScratchpadAddress(ibc));
				*ibc.fdst = _mm_add_pd(*ibc.fdst, fsrc);
			} break;

			case InstructionType::FSUB_R: {
				*ibc.fdst = _mm_sub_pd(*ibc.fdst, *ibc.fsrc);
			} break;

			case InstructionType::FSUB_M: {
				__m128d fsrc = load_cvt_i32x2(getScratchpadAddress(ibc));
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
				__m128d fsrc = maskRegisterExponentMantissa(load_cvt_i32x2(getScratchpadAddress(ibc)));
				*ibc.fdst = _mm_div_pd(*ibc.fdst, fsrc);
			} break;

			case InstructionType::FSQRT_R: {
				*ibc.fdst = _mm_sqrt_pd(*ibc.fdst);
			} break;

			case InstructionType::CBRANCH: {
				*ibc.isrc += ibc.imm;
				if ((*ibc.isrc & ibc.memMask) == 0) {
					pc = ibc.target;
				}
			} break;

			case InstructionType::CFROUND: {
				setRoundMode(rotr(*ibc.isrc, ibc.imm) % 4);
			} break;

			case InstructionType::ISTORE: {
				store64(scratchpad + ((*ibc.idst + ibc.imm) & ibc.memMask), *ibc.isrc);
			} break;

			case InstructionType::NOP: {
				//nothing
			} break;

			default:
				UNREACHABLE;
		}
	}

	template<class Allocator, bool softAes>
	void InterpretedVm<Allocator, softAes>::execute() {
		int_reg_t r[RegistersCount] = { 0 };
		__m128d f[RegisterCountFlt];
		__m128d e[RegisterCountFlt];
		__m128d a[RegisterCountFlt];

		for(unsigned i = 0; i < RegisterCountFlt; ++i)
			a[i] = _mm_load_pd(&reg.a[i].lo);

		precompileProgram(r, f, e, a);

		uint32_t spAddr0 = mem.mx;
		uint32_t spAddr1 = mem.ma;

		for(unsigned ic = 0; ic < RANDOMX_PROGRAM_ITERATIONS; ++ic) {
			uint64_t spMix = r[config.readReg0] ^ r[config.readReg1];
			spAddr0 ^= spMix;
			spAddr0 &= ScratchpadL3Mask64;
			spAddr1 ^= spMix >> 32;
			spAddr1 &= ScratchpadL3Mask64;
			
			for (unsigned i = 0; i < RegistersCount; ++i)
				r[i] ^= load64(scratchpad + spAddr0 + 8 * i);

			for (unsigned i = 0; i < RegisterCountFlt; ++i)
				f[i] = load_cvt_i32x2(scratchpad + spAddr1 + 8 * i);

			for (unsigned i = 0; i < RegisterCountFlt; ++i)
				e[i] = maskRegisterExponentMantissa(load_cvt_i32x2(scratchpad + spAddr1 + 8 * (RegisterCountFlt + i)));

			executeBytecode(r, f, e, a);

			mem.mx ^= r[config.readReg2] ^ r[config.readReg3];
			mem.mx &= CacheLineAlignMask;
			datasetRead(datasetOffset + mem.ma, r);
			std::swap(mem.mx, mem.ma);

			for (unsigned i = 0; i < RegistersCount; ++i)
				store64(scratchpad + spAddr1 + 8 * i, r[i]);

			for (unsigned i = 0; i < RegisterCountFlt; ++i)
				f[i] = _mm_xor_pd(f[i], e[i]);

			for (unsigned i = 0; i < RegisterCountFlt; ++i)
				_mm_store_pd((double*)(scratchpad + spAddr0 + 16 * i), f[i]);

			spAddr0 = 0;
			spAddr1 = 0;
		}

		for (unsigned i = 0; i < RegistersCount; ++i)
			store64(&reg.r[i], r[i]);

		for (unsigned i = 0; i < RegisterCountFlt; ++i)
			_mm_store_pd(&reg.f[i].lo, f[i]);

		for (unsigned i = 0; i < RegisterCountFlt; ++i)
			_mm_store_pd(&reg.e[i].lo, e[i]);
	}

	template<class Allocator, bool softAes>
	void InterpretedVm<Allocator, softAes>::datasetRead(uint32_t address, int_reg_t(&r)[RegistersCount]) {
		uint64_t* datasetLine = (uint64_t*)(mem.memory + address);
		for (int i = 0; i < RegistersCount; ++i)
			r[i] ^= datasetLine[i];
	}

#include "instruction_weights.hpp"

	template<class Allocator, bool softAes>
	void InterpretedVm<Allocator, softAes>::precompileProgram(int_reg_t(&r)[RegistersCount], __m128d (&f)[RegisterCountFlt], __m128d (&e)[RegisterCountFlt], __m128d (&a)[RegisterCountFlt]) {
		RegisterUsage registerUsage[RegistersCount];
		for (unsigned i = 0; i < RegistersCount; ++i) {
			registerUsage[i].lastUsed = -1;
			registerUsage[i].count = 0;
		}
		for (unsigned i = 0; i < RANDOMX_PROGRAM_SIZE; ++i) {
			auto& instr = program(i);
			auto& ibc = byteCode[i];
			switch (instr.opcode) {
				CASE_REP(IADD_RS) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::IADD_RS;
					ibc.idst = &r[dst];
					if (dst != RegisterNeedsDisplacement) {
						ibc.isrc = &r[src];
						ibc.shift = instr.getModShift();
						ibc.imm = 0;
					}
					else {
						ibc.isrc = &r[src];
						ibc.shift = instr.getModShift();
						ibc.imm = signExtend2sCompl(instr.getImm32());
					}
					registerUsage[dst].lastUsed = i;
				} break;

				CASE_REP(IADD_M) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::IADD_M;
					ibc.idst = &r[dst];
					ibc.imm = signExtend2sCompl(instr.getImm32());
					if (src != dst) {
						ibc.isrc = &r[src];
						ibc.memMask = (instr.getModMem() ? ScratchpadL1Mask : ScratchpadL2Mask);
					}
					else {
						ibc.isrc = &Zero;
						ibc.memMask = ScratchpadL3Mask;
					}
					registerUsage[dst].lastUsed = i;
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
					registerUsage[dst].lastUsed = i;
				} break;

				CASE_REP(ISUB_M) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::ISUB_M;
					ibc.idst = &r[dst];
					ibc.imm = signExtend2sCompl(instr.getImm32());
					if (src != dst) {
						ibc.isrc = &r[src];
						ibc.memMask = (instr.getModMem() ? ScratchpadL1Mask : ScratchpadL2Mask);
					}
					else {
						ibc.isrc = &Zero;
						ibc.memMask = ScratchpadL3Mask;
					}
					registerUsage[dst].lastUsed = i;
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
					registerUsage[dst].lastUsed = i;
				} break;

				CASE_REP(IMUL_M) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::IMUL_M;
					ibc.idst = &r[dst];
					ibc.imm = signExtend2sCompl(instr.getImm32());
					if (src != dst) {
						ibc.isrc = &r[src];
						ibc.memMask = (instr.getModMem() ? ScratchpadL1Mask : ScratchpadL2Mask);
					}
					else {
						ibc.isrc = &Zero;
						ibc.memMask = ScratchpadL3Mask;
					}
					registerUsage[dst].lastUsed = i;
				} break;

				CASE_REP(IMULH_R) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::IMULH_R;
					ibc.idst = &r[dst];
					ibc.isrc = &r[src];
					registerUsage[dst].lastUsed = i;
				} break;

				CASE_REP(IMULH_M) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::IMULH_M;
					ibc.idst = &r[dst];
					ibc.imm = signExtend2sCompl(instr.getImm32());
					if (src != dst) {
						ibc.isrc = &r[src];
						ibc.memMask = (instr.getModMem() ? ScratchpadL1Mask : ScratchpadL2Mask);
					}
					else {
						ibc.isrc = &Zero;
						ibc.memMask = ScratchpadL3Mask;
					}
					registerUsage[dst].lastUsed = i;
				} break;

				CASE_REP(ISMULH_R) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::ISMULH_R;
					ibc.idst = &r[dst];
					ibc.isrc = &r[src];
					registerUsage[dst].lastUsed = i;
				} break;

				CASE_REP(ISMULH_M) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::ISMULH_M;
					ibc.idst = &r[dst];
					ibc.imm = signExtend2sCompl(instr.getImm32());
					if (src != dst) {
						ibc.isrc = &r[src];
						ibc.memMask = (instr.getModMem() ? ScratchpadL1Mask : ScratchpadL2Mask);
					}
					else {
						ibc.isrc = &Zero;
						ibc.memMask = ScratchpadL3Mask;
					}
					registerUsage[dst].lastUsed = i;
				} break;

				CASE_REP(IMUL_RCP) {
					uint32_t divisor = instr.getImm32();
					if (divisor != 0) {
						auto dst = instr.dst % RegistersCount;
						ibc.type = InstructionType::IMUL_R;
						ibc.idst = &r[dst];
						ibc.imm = randomx_reciprocal(divisor);
						ibc.isrc = &ibc.imm;
						registerUsage[dst].lastUsed = i;
					}
					else {
						ibc.type = InstructionType::NOP;
					}
				} break;

				CASE_REP(INEG_R) {
					auto dst = instr.dst % RegistersCount;
					ibc.type = InstructionType::INEG_R;
					ibc.idst = &r[dst];
					registerUsage[dst].lastUsed = i;
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
					registerUsage[dst].lastUsed = i;
				} break;

				CASE_REP(IXOR_M) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::IXOR_M;
					ibc.idst = &r[dst];
					ibc.imm = signExtend2sCompl(instr.getImm32());
					if (src != dst) {
						ibc.isrc = &r[src];
						ibc.memMask = (instr.getModMem() ? ScratchpadL1Mask : ScratchpadL2Mask);
					}
					else {
						ibc.isrc = &Zero;
						ibc.memMask = ScratchpadL3Mask;
					}
					registerUsage[dst].lastUsed = i;
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
					registerUsage[dst].lastUsed = i;
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
					registerUsage[dst].lastUsed = i;
				} break;

				CASE_REP(ISWAP_R) {
					auto dst = instr.dst % RegistersCount;
					auto src = instr.src % RegistersCount;
					if (src != dst) {
						ibc.idst = &r[dst];
						ibc.isrc = &r[src];
						ibc.type = InstructionType::ISWAP_R;
						registerUsage[dst].lastUsed = i;
						registerUsage[src].lastUsed = i;
					}
					else {
						ibc.type = InstructionType::NOP;
					}
				} break;

				CASE_REP(FSWAP_R) {
					auto dst = instr.dst % RegistersCount;
					ibc.type = InstructionType::FSWAP_R;
					if (dst < RegisterCountFlt)
						ibc.fdst = &f[dst];
					else
						ibc.fdst = &e[dst - RegisterCountFlt];
				} break;

				CASE_REP(FADD_R) {
					auto dst = instr.dst % RegisterCountFlt;
					auto src = instr.src % RegisterCountFlt;
					ibc.type = InstructionType::FADD_R;
					ibc.fdst = &f[dst];
					ibc.fsrc = &a[src];
				} break;

				CASE_REP(FADD_M) {
					auto dst = instr.dst % RegisterCountFlt;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::FADD_M;
					ibc.fdst = &f[dst];
					ibc.isrc = &r[src];
					ibc.memMask = (instr.getModMem() ? ScratchpadL1Mask : ScratchpadL2Mask);
					ibc.imm = signExtend2sCompl(instr.getImm32());
				} break;

				CASE_REP(FSUB_R) {
					auto dst = instr.dst % RegisterCountFlt;
					auto src = instr.src % RegisterCountFlt;
					ibc.type = InstructionType::FSUB_R;
					ibc.fdst = &f[dst];
					ibc.fsrc = &a[src];
				} break;

				CASE_REP(FSUB_M) {
					auto dst = instr.dst % RegisterCountFlt;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::FSUB_M;
					ibc.fdst = &f[dst];
					ibc.isrc = &r[src];
					ibc.memMask = (instr.getModMem() ? ScratchpadL1Mask : ScratchpadL2Mask);
					ibc.imm = signExtend2sCompl(instr.getImm32());
				} break;

				CASE_REP(FSCAL_R) {
					auto dst = instr.dst % RegisterCountFlt;
					ibc.fdst = &f[dst];
					ibc.type = InstructionType::FSCAL_R;
				} break;

				CASE_REP(FMUL_R) {
					auto dst = instr.dst % RegisterCountFlt;
					auto src = instr.src % RegisterCountFlt;
					ibc.type = InstructionType::FMUL_R;
					ibc.fdst = &e[dst];
					ibc.fsrc = &a[src];
				} break;

				CASE_REP(FDIV_M) {
					auto dst = instr.dst % RegisterCountFlt;
					auto src = instr.src % RegistersCount;
					ibc.type = InstructionType::FDIV_M;
					ibc.fdst = &e[dst];
					ibc.isrc = &r[src];
					ibc.memMask = (instr.getModMem() ? ScratchpadL1Mask : ScratchpadL2Mask);
					ibc.imm = signExtend2sCompl(instr.getImm32());
				} break;

				CASE_REP(FSQRT_R) {
					auto dst = instr.dst % RegisterCountFlt;
					ibc.type = InstructionType::FSQRT_R;
					ibc.fdst = &e[dst];
				} break;

				CASE_REP(CBRANCH) {
					ibc.type = InstructionType::CBRANCH;
					//jump condition
					int reg = getConditionRegister(registerUsage);
					ibc.isrc = &r[reg];
					ibc.target = registerUsage[reg].lastUsed;
					registerUsage[reg].count++;
					int shift = instr.getModCond();
					const uint64_t conditionMask = ConditionMask << instr.getModCond();
					ibc.imm = signExtend2sCompl(instr.getImm32()) | (1ULL << shift);
					ibc.memMask = ConditionMask << shift;
					//mark all registers as used
					for (unsigned j = 0; j < RegistersCount; ++j) {
						registerUsage[j].lastUsed = i;
					}
				} break;

				CASE_REP(CFROUND) {
					auto src = instr.src % RegistersCount;
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
					ibc.imm = signExtend2sCompl(instr.getImm32());
					if (instr.getModCond() < StoreL3Condition)
						ibc.memMask = (instr.getModMem() ? ScratchpadL1Mask : ScratchpadL2Mask);
					else
						ibc.memMask = ScratchpadL3Mask;
				} break;

				CASE_REP(NOP) {
					ibc.type = InstructionType::NOP;
				} break;

				default:
					UNREACHABLE;
			}
		}
	}

	template class InterpretedVm<AlignedAllocator<CacheLineSize>, false>;
	template class InterpretedVm<AlignedAllocator<CacheLineSize>, true>;
	template class InterpretedVm<LargePageAllocator, false>;
	template class InterpretedVm<LargePageAllocator, true>;
}