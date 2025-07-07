/*
Copyright (c) 2024, tevador <tevador@gmail.com>

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	* Neither the name of the copyright holder nor the
	  names of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "common.hpp"
#include "intrin_portable.h"
#include "instruction.hpp"
#include "program.hpp"

namespace randomx {

	//register file in machine byte order
	struct NativeRegisterFile {
		int_reg_t r[RegistersCount] = { 0 };
		rx_vec_f128 f[RegisterCountFlt];
		rx_vec_f128 e[RegisterCountFlt];
		rx_vec_f128 a[RegisterCountFlt];
	};

	struct InstructionByteCode {
		union {
			int_reg_t* idst;
			rx_vec_f128* fdst;
		};
		union {
			const int_reg_t* isrc;
			const rx_vec_f128* fsrc;
		};
		union {
			uint64_t imm;
			int64_t simm;
		};
		InstructionType type;
		union {
			int16_t target;
			uint16_t shift;
		};
		uint32_t memMask;
	};

#define OPCODE_CEIL_DECLARE(curr, prev) constexpr int ceil_ ## curr = ceil_ ## prev + RANDOMX_FREQ_ ## curr;
	constexpr int ceil_NULL = 0;
	OPCODE_CEIL_DECLARE(IADD_RS, NULL);
	OPCODE_CEIL_DECLARE(IADD_M, IADD_RS);
	OPCODE_CEIL_DECLARE(ISUB_R, IADD_M);
	OPCODE_CEIL_DECLARE(ISUB_M, ISUB_R);
	OPCODE_CEIL_DECLARE(IMUL_R, ISUB_M);
	OPCODE_CEIL_DECLARE(IMUL_M, IMUL_R);
	OPCODE_CEIL_DECLARE(IMULH_R, IMUL_M);
	OPCODE_CEIL_DECLARE(IMULH_M, IMULH_R);
	OPCODE_CEIL_DECLARE(ISMULH_R, IMULH_M);
	OPCODE_CEIL_DECLARE(ISMULH_M, ISMULH_R);
	OPCODE_CEIL_DECLARE(IMUL_RCP, ISMULH_M);
	OPCODE_CEIL_DECLARE(INEG_R, IMUL_RCP);
	OPCODE_CEIL_DECLARE(IXOR_R, INEG_R);
	OPCODE_CEIL_DECLARE(IXOR_M, IXOR_R);
	OPCODE_CEIL_DECLARE(IROR_R, IXOR_M);
	OPCODE_CEIL_DECLARE(IROL_R, IROR_R);
	OPCODE_CEIL_DECLARE(ISWAP_R, IROL_R);
	OPCODE_CEIL_DECLARE(FSWAP_R, ISWAP_R);
	OPCODE_CEIL_DECLARE(FADD_R, FSWAP_R);
	OPCODE_CEIL_DECLARE(FADD_M, FADD_R);
	OPCODE_CEIL_DECLARE(FSUB_R, FADD_M);
	OPCODE_CEIL_DECLARE(FSUB_M, FSUB_R);
	OPCODE_CEIL_DECLARE(FSCAL_R, FSUB_M);
	OPCODE_CEIL_DECLARE(FMUL_R, FSCAL_R);
	OPCODE_CEIL_DECLARE(FDIV_M, FMUL_R);
	OPCODE_CEIL_DECLARE(FSQRT_R, FDIV_M);
	OPCODE_CEIL_DECLARE(CBRANCH, FSQRT_R);
	OPCODE_CEIL_DECLARE(CFROUND, CBRANCH);
	OPCODE_CEIL_DECLARE(ISTORE, CFROUND);
	OPCODE_CEIL_DECLARE(NOP, ISTORE);
#undef OPCODE_CEIL_DECLARE

#define RANDOMX_EXE_ARGS InstructionByteCode& ibc, int& pc, uint8_t* scratchpad, ProgramConfiguration& config
#define RANDOMX_GEN_ARGS Instruction& instr, int i, InstructionByteCode& ibc

	class BytecodeMachine;

	typedef void(BytecodeMachine::*InstructionGenBytecode)(RANDOMX_GEN_ARGS);

	class BytecodeMachine {
	public:
		void beginCompilation(NativeRegisterFile& regFile) {
			for (unsigned i = 0; i < RegistersCount; ++i) {
				registerUsage[i] = -1;
			}
			nreg = &regFile;
		}

		void compileProgram(Program& program, InstructionByteCode bytecode[RANDOMX_PROGRAM_SIZE], NativeRegisterFile& regFile) {
			beginCompilation(regFile);
			for (unsigned i = 0; i < RANDOMX_PROGRAM_SIZE; ++i) {
				auto& instr = program(i);
				auto& ibc = bytecode[i];
				compileInstruction(instr, i, ibc);
			}
		}

		static void executeBytecode(InstructionByteCode bytecode[RANDOMX_PROGRAM_SIZE], uint8_t* scratchpad, ProgramConfiguration& config) {
			for (int pc = 0; pc < RANDOMX_PROGRAM_SIZE; ++pc) {
				auto& ibc = bytecode[pc];
				executeInstruction(ibc, pc, scratchpad, config);
			}
		}

		void compileInstruction(RANDOMX_GEN_ARGS)
#ifdef RANDOMX_GEN_TABLE
		{
			auto generator = genTable[instr.opcode];
			(this->*generator)(instr, i, ibc);
		}
#else
		;
#endif

		static void executeInstruction(RANDOMX_EXE_ARGS);

		static void exe_IADD_RS(RANDOMX_EXE_ARGS) {
			*ibc.idst += (*ibc.isrc << ibc.shift) + ibc.imm;
		}

		static void exe_IADD_M(RANDOMX_EXE_ARGS) {
			*ibc.idst += load64(getScratchpadAddress(ibc, scratchpad));
		}

		static void exe_ISUB_R(RANDOMX_EXE_ARGS) {
			*ibc.idst -= *ibc.isrc;
		}

		static void exe_ISUB_M(RANDOMX_EXE_ARGS) {
			*ibc.idst -= load64(getScratchpadAddress(ibc, scratchpad));
		}

		static void exe_IMUL_R(RANDOMX_EXE_ARGS) {
			*ibc.idst *= *ibc.isrc;
		}

		static void exe_IMUL_M(RANDOMX_EXE_ARGS) {
			*ibc.idst *= load64(getScratchpadAddress(ibc, scratchpad));
		}

		static void exe_IMULH_R(RANDOMX_EXE_ARGS) {
			*ibc.idst = mulh(*ibc.idst, *ibc.isrc);
		}

		static void exe_IMULH_M(RANDOMX_EXE_ARGS) {
			*ibc.idst = mulh(*ibc.idst, load64(getScratchpadAddress(ibc, scratchpad)));
		}

		static void exe_ISMULH_R(RANDOMX_EXE_ARGS) {
			*ibc.idst = smulh(unsigned64ToSigned2sCompl(*ibc.idst), unsigned64ToSigned2sCompl(*ibc.isrc));
		}

		static void exe_ISMULH_M(RANDOMX_EXE_ARGS) {
			*ibc.idst = smulh(unsigned64ToSigned2sCompl(*ibc.idst), unsigned64ToSigned2sCompl(load64(getScratchpadAddress(ibc, scratchpad))));
		}

		static void exe_INEG_R(RANDOMX_EXE_ARGS) {
			*ibc.idst = ~(*ibc.idst) + 1; //two's complement negative
		}

		static void exe_IXOR_R(RANDOMX_EXE_ARGS) {
			*ibc.idst ^= *ibc.isrc;
		}

		static void exe_IXOR_M(RANDOMX_EXE_ARGS) {
			*ibc.idst ^= load64(getScratchpadAddress(ibc, scratchpad));
		}

		static void exe_IROR_R(RANDOMX_EXE_ARGS) {
			*ibc.idst = rotr(*ibc.idst, *ibc.isrc & 63);
		}

		static void exe_IROL_R(RANDOMX_EXE_ARGS) {
			*ibc.idst = rotl(*ibc.idst, *ibc.isrc & 63);
		}

		static void exe_ISWAP_R(RANDOMX_EXE_ARGS) {
			int_reg_t temp = *ibc.isrc;
			*(int_reg_t*)ibc.isrc = *ibc.idst;
			*ibc.idst = temp;
		}

		static void exe_FSWAP_R(RANDOMX_EXE_ARGS) {
			*ibc.fdst = rx_swap_vec_f128(*ibc.fdst);
		}

		static void exe_FADD_R(RANDOMX_EXE_ARGS) {
			*ibc.fdst = rx_add_vec_f128(*ibc.fdst, *ibc.fsrc);
		}

		static void exe_FADD_M(RANDOMX_EXE_ARGS) {
			rx_vec_f128 fsrc = rx_cvt_packed_int_vec_f128(getScratchpadAddress(ibc, scratchpad));
			*ibc.fdst = rx_add_vec_f128(*ibc.fdst, fsrc);
		}

		static void exe_FSUB_R(RANDOMX_EXE_ARGS) {
			*ibc.fdst = rx_sub_vec_f128(*ibc.fdst, *ibc.fsrc);
		}

		static void exe_FSUB_M(RANDOMX_EXE_ARGS) {
			rx_vec_f128 fsrc = rx_cvt_packed_int_vec_f128(getScratchpadAddress(ibc, scratchpad));
			*ibc.fdst = rx_sub_vec_f128(*ibc.fdst, fsrc);
		}

		static void exe_FSCAL_R(RANDOMX_EXE_ARGS) {
			const rx_vec_f128 mask = rx_set1_vec_f128(0x80F0000000000000);
			*ibc.fdst = rx_xor_vec_f128(*ibc.fdst, mask);
		}

		static void exe_FMUL_R(RANDOMX_EXE_ARGS) {
			*ibc.fdst = rx_mul_vec_f128(*ibc.fdst, *ibc.fsrc);
		}

		static void exe_FDIV_M(RANDOMX_EXE_ARGS) {
			rx_vec_f128 fsrc = maskRegisterExponentMantissa(
				config,
				rx_cvt_packed_int_vec_f128(getScratchpadAddress(ibc, scratchpad))
			);
			*ibc.fdst = rx_div_vec_f128(*ibc.fdst, fsrc);
		}

		static void exe_FSQRT_R(RANDOMX_EXE_ARGS) {
			*ibc.fdst = rx_sqrt_vec_f128(*ibc.fdst);
		}

		static void exe_CBRANCH(RANDOMX_EXE_ARGS) {
			*ibc.idst += ibc.imm;
			if ((*ibc.idst & ibc.memMask) == 0) {
				pc = ibc.target;
			}
		}

		static void exe_CFROUND(RANDOMX_EXE_ARGS) {
			rx_set_rounding_mode(rotr(*ibc.isrc, ibc.imm) % 4);
		}

		static void exe_ISTORE(RANDOMX_EXE_ARGS) {
			store64(scratchpad + ((*ibc.idst + ibc.imm) & ibc.memMask), *ibc.isrc);
		}
	protected:
		static rx_vec_f128 maskRegisterExponentMantissa(ProgramConfiguration& config, rx_vec_f128 x) {
			const rx_vec_f128 xmantissaMask = rx_set_vec_f128(dynamicMantissaMask, dynamicMantissaMask);
			const rx_vec_f128 xexponentMask = rx_load_vec_f128((const double*)&config.eMask);
			x = rx_and_vec_f128(x, xmantissaMask);
			x = rx_or_vec_f128(x, xexponentMask);
			return x;
		}

	private:
		static const int_reg_t zero;
		int registerUsage[RegistersCount];
		NativeRegisterFile* nreg;

		static void* getScratchpadAddress(InstructionByteCode& ibc, uint8_t* scratchpad) {
			uint32_t addr = (*ibc.isrc + ibc.imm) & ibc.memMask;
			return scratchpad + addr;
		}

#ifdef RANDOMX_GEN_TABLE
		static InstructionGenBytecode genTable[256];

		void gen_IADD_RS(RANDOMX_GEN_ARGS);
		void gen_IADD_M(RANDOMX_GEN_ARGS);
		void gen_ISUB_R(RANDOMX_GEN_ARGS);
		void gen_ISUB_M(RANDOMX_GEN_ARGS);
		void gen_IMUL_R(RANDOMX_GEN_ARGS);
		void gen_IMUL_M(RANDOMX_GEN_ARGS);
		void gen_IMULH_R(RANDOMX_GEN_ARGS);
		void gen_IMULH_M(RANDOMX_GEN_ARGS);
		void gen_ISMULH_R(RANDOMX_GEN_ARGS);
		void gen_ISMULH_M(RANDOMX_GEN_ARGS);
		void gen_IMUL_RCP(RANDOMX_GEN_ARGS);
		void gen_INEG_R(RANDOMX_GEN_ARGS);
		void gen_IXOR_R(RANDOMX_GEN_ARGS);
		void gen_IXOR_M(RANDOMX_GEN_ARGS);
		void gen_IROR_R(RANDOMX_GEN_ARGS);
		void gen_IROL_R(RANDOMX_GEN_ARGS);
		void gen_ISWAP_R(RANDOMX_GEN_ARGS);
		void gen_FSWAP_R(RANDOMX_GEN_ARGS);
		void gen_FADD_R(RANDOMX_GEN_ARGS);
		void gen_FADD_M(RANDOMX_GEN_ARGS);
		void gen_FSUB_R(RANDOMX_GEN_ARGS);
		void gen_FSUB_M(RANDOMX_GEN_ARGS);
		void gen_FSCAL_R(RANDOMX_GEN_ARGS);
		void gen_FMUL_R(RANDOMX_GEN_ARGS);
		void gen_FDIV_M(RANDOMX_GEN_ARGS);
		void gen_FSQRT_R(RANDOMX_GEN_ARGS);
		void gen_CBRANCH(RANDOMX_GEN_ARGS);
		void gen_CFROUND(RANDOMX_GEN_ARGS);
		void gen_ISTORE(RANDOMX_GEN_ARGS);
		void gen_NOP(RANDOMX_GEN_ARGS);
#endif
	};
}
