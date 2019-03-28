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

#include "blake2/blake2.h"
#include "configuration.h"
#include "Program.hpp"
#include "blake2/endian.h";
#include <iostream>

namespace RandomX {

	namespace LightInstruction {
		constexpr int IADD_R = 0;
		constexpr int IADD_RC = 1;
		constexpr int ISUB_R = 2;
		constexpr int IMUL_9C = 3;
		constexpr int IMUL_R = 4;
		constexpr int IMULH_R = 5;
		constexpr int ISMULH_R = 6;
		constexpr int IMUL_RCP = 7;
		constexpr int IXOR_R = 8;
		constexpr int IROR_R = 9;
		constexpr int COND_R = 10;
		constexpr int COUNT = 11;
	}

	const int lightInstruction[] = {
		LightInstruction::IADD_RC,
		LightInstruction::IADD_RC,
		LightInstruction::ISUB_R,
		LightInstruction::ISUB_R,
		LightInstruction::IMUL_9C,
		LightInstruction::IMUL_R,
		LightInstruction::IMUL_R,
		LightInstruction::IMUL_R,
		LightInstruction::IMULH_R,
		LightInstruction::ISMULH_R,
		LightInstruction::IMUL_RCP,
		LightInstruction::IXOR_R,
		LightInstruction::IXOR_R,
		LightInstruction::IROR_R,
		LightInstruction::IROR_R,
		LightInstruction::COND_R
	};

	namespace LightInstructionOpcode {
		constexpr int IADD_R = 0;
		constexpr int IADD_RC = RANDOMX_FREQ_IADD_R + RANDOMX_FREQ_IADD_M;
		constexpr int ISUB_R = IADD_RC + RANDOMX_FREQ_IADD_RC;
		constexpr int IMUL_9C = ISUB_R + RANDOMX_FREQ_ISUB_R + RANDOMX_FREQ_ISUB_M;
		constexpr int IMUL_R = IMUL_9C + RANDOMX_FREQ_IMUL_9C;
		constexpr int IMULH_R = IMUL_R + RANDOMX_FREQ_IMUL_R + RANDOMX_FREQ_IMUL_M;
		constexpr int ISMULH_R = IMULH_R + RANDOMX_FREQ_IMULH_R + RANDOMX_FREQ_IMULH_M;
		constexpr int IMUL_RCP = ISMULH_R + RANDOMX_FREQ_ISMULH_R + RANDOMX_FREQ_ISMULH_M;;
		constexpr int IXOR_R = IMUL_RCP + RANDOMX_FREQ_IMUL_RCP + RANDOMX_FREQ_INEG_R;
		constexpr int IROR_R = IXOR_R + RANDOMX_FREQ_IXOR_R + RANDOMX_FREQ_IXOR_M;
		constexpr int COND_R = IROR_R + RANDOMX_FREQ_IROR_R + RANDOMX_FREQ_IROL_R + RANDOMX_FREQ_ISWAP_R + RANDOMX_FREQ_FSWAP_R + RANDOMX_FREQ_FADD_R + RANDOMX_FREQ_FADD_M + RANDOMX_FREQ_FSUB_R + RANDOMX_FREQ_FSUB_M + RANDOMX_FREQ_FSCAL_R + RANDOMX_FREQ_FMUL_R + RANDOMX_FREQ_FDIV_M + RANDOMX_FREQ_FSQRT_R;
	}

	const int lightInstructionOpcode[] = {
		LightInstructionOpcode::IADD_R,
		LightInstructionOpcode::IADD_RC,
		LightInstructionOpcode::ISUB_R,
		LightInstructionOpcode::IMUL_9C,
		LightInstructionOpcode::IMUL_R,
		LightInstructionOpcode::IMULH_R,
		LightInstructionOpcode::ISMULH_R,
		LightInstructionOpcode::IMUL_RCP,
		LightInstructionOpcode::IXOR_R,
		LightInstructionOpcode::IROR_R,
		LightInstructionOpcode::COND_R
	};

	constexpr int ALU_COUNT_MUL = 1;
	constexpr int ALU_COUNT = 4;
	constexpr int LIGHT_OPCODE_BITS = 4;
	constexpr int V4_SRC_INDEX_BITS = 3;
	constexpr int V4_DST_INDEX_BITS = 3;

	static int blakeCounter = 0;

	// If we don't have enough data available, generate more
	static FORCE_INLINE void check_data(size_t& data_index, const size_t bytes_needed, uint8_t* data, const size_t data_size)
	{
		if (data_index + bytes_needed > data_size)
		{
			std::cout << "Calling Blake " << (++blakeCounter) << std::endl;
			blake2b(data, data_size, data, data_size, nullptr, 0);
			data_index = 0;
		}
	}

	void generateLightProgram(LightProgram& prog, const void* seed, int indexRegister) {

		// Source: https://www.agner.org/optimize/instruction_tables.pdf
		const int op_latency[LightInstruction::COUNT] = { 1, 2, 1, 2, 3, 5, 5, 4, 1, 2, 5 };

		// Instruction latencies for theoretical ASIC implementation
		const int asic_op_latency[LightInstruction::COUNT] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

		// Available ALUs for each instruction
		const int op_ALUs[LightInstruction::COUNT] = { ALU_COUNT, ALU_COUNT, ALU_COUNT, ALU_COUNT, ALU_COUNT_MUL, ALU_COUNT_MUL, ALU_COUNT_MUL, ALU_COUNT_MUL, ALU_COUNT, ALU_COUNT, ALU_COUNT };

		uint8_t data[64];
		memset(data, 0, sizeof(data));
		memcpy(data, seed, SeedSize);

		// Set data_index past the last byte in data
		// to trigger full data update with blake hash
		// before we start using it
		size_t data_index = sizeof(data);

		int code_size;

		do {
			uint8_t opcode;
			uint8_t dst_index;
			uint8_t src_index;
			uint32_t imm32 = 0;

			int latency[8];
			int asic_latency[9];

			// Tracks previous instruction and value of the source operand for registers R0-R3 throughout code execution
			// byte 0: current value of the destination register
			// byte 1: instruction opcode
			// byte 2: current value of the source register
			//
			// Registers R4-R8 are constant and are treated as having the same value because when we do
			// the same operation twice with two constant source registers, it can be optimized into a single operation
			uint64_t inst_data[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

			bool alu_busy[RANDOMX_LPROG_LATENCY + 1][ALU_COUNT];
			bool is_rotation[LightInstruction::COUNT];
			bool rotated[8];
			int rotate_count = 0;

			memset(latency, 0, sizeof(latency));
			memset(asic_latency, 0, sizeof(asic_latency));
			memset(alu_busy, 0, sizeof(alu_busy));
			memset(is_rotation, 0, sizeof(is_rotation));
			memset(rotated, 0, sizeof(rotated));
			is_rotation[LightInstruction::IROR_R] = true;

			int num_retries = 0;
			code_size = 0;

			int total_iterations = 0;

			// Generate random code to achieve minimal required latency for our abstract CPU
			// Try to get this latency for all 4 registers
			while (((latency[0] < RANDOMX_LPROG_LATENCY) || (latency[1] < RANDOMX_LPROG_LATENCY) || (latency[2] < RANDOMX_LPROG_LATENCY) || (latency[3] < RANDOMX_LPROG_LATENCY)
				|| (latency[4] < RANDOMX_LPROG_LATENCY) || (latency[5] < RANDOMX_LPROG_LATENCY) || (latency[6] < RANDOMX_LPROG_LATENCY) || (latency[7] < RANDOMX_LPROG_LATENCY)) && (num_retries < 64))
			{
				// Fail-safe to guarantee loop termination
				++total_iterations;
				if (total_iterations > 1024) {
					std::cout << "total_iterations = " << total_iterations << std::endl;
					break;
				}

				check_data(data_index, 1, data, sizeof(data));
				const uint8_t b1 = data[data_index++];
				int instrType = lightInstruction[b1 & ((1 << LIGHT_OPCODE_BITS) - 1)];

				check_data(data_index, 1, data, sizeof(data));
				const uint8_t b2 = data[data_index++];
				dst_index = b2 & ((1 << V4_DST_INDEX_BITS) - 1);
				src_index = (b2 >> (V4_DST_INDEX_BITS)) & ((1 << V4_SRC_INDEX_BITS) - 1);

				const int a = dst_index;
				int b = src_index;

				// Don't do rotation with the same destination twice because it's equal to a single rotation
				if (is_rotation[instrType] && rotated[a])
				{
					continue;
				}

				// Don't do the same instruction (except MUL) with the same source value twice because all other cases can be optimized:
				//	2x IADD_RC(a, b, C) = IADD_RC(a, b*2, C1+C2)
				//	2x ISUB_R(a, b) = ISUB_R(a, 2*b)
				//	2x IMUL_R(a, b) = IMUL_R(a, b*b)
				//	2x IMUL_9C(a, C) = 9 * (9 * a + C1) + C2 = 81 * a + (9 * C1 + C2)
				//	2x IMUL_RCP(a, C) = a * (C * C)
				//	2x IXOR_R = NOP
				//	2x IROR_R(a, b) = IROR_R(a, 2*b)
				if (instrType != LightInstruction::IMULH_R && instrType != LightInstruction::ISMULH_R && ((inst_data[a] & 0xFFFF00) == (instrType << 8) + ((inst_data[b] & 255) << 16)))
				{
					continue;
				}

				if ((instrType == LightInstruction::IADD_RC) || (instrType == LightInstruction::IMUL_9C) || (instrType == LightInstruction::IMUL_RCP) || (instrType == LightInstruction::COND_R) || ((instrType != LightInstruction::IMULH_R) && (instrType != LightInstruction::ISMULH_R) && (a == b)))
				{
					check_data(data_index, 4, data, sizeof(data));
					imm32 = load32(&data[data_index++]);
				}

				// Find which ALU is available (and when) for this instruction
				int next_latency = (latency[a] > latency[b]) ? latency[a] : latency[b];
				int alu_index = -1;
				while (next_latency < RANDOMX_LPROG_LATENCY)
				{
					for (int i = op_ALUs[instrType] - 1; i >= 0; --i)
					{
						if (!alu_busy[next_latency][i])
						{
							// ADD is implemented as two 1-cycle instructions on a real CPU, so do an additional availability check
							if ((instrType == LightInstruction::IADD_RC || instrType == LightInstruction::IMUL_9C || instrType == LightInstruction::IMULH_R || instrType == LightInstruction::ISMULH_R) && alu_busy[next_latency + 1][i])
							{
								continue;
							}

							// Rotation can only start when previous rotation is finished, so do an additional availability check
							if (is_rotation[instrType] && (next_latency < rotate_count * op_latency[instrType]))
							{
								continue;
							}

							alu_index = i;
							break;
						}
					}
					if (alu_index >= 0)
					{
						break;
					}
					++next_latency;
				}

				// Don't generate instructions that leave some register unchanged for more than 15 cycles
				if (next_latency > latency[a] + 15)
				{
					continue;
				}

				next_latency += op_latency[instrType];

				if (next_latency <= RANDOMX_LPROG_LATENCY)
				{
					if (is_rotation[instrType])
					{
						++rotate_count;
					}

					// Mark ALU as busy only for the first cycle when it starts executing the instruction because ALUs are fully pipelined
					alu_busy[next_latency - op_latency[instrType]][alu_index] = true;
					latency[a] = next_latency;

					// ASIC is supposed to have enough ALUs to run as many independent instructions per cycle as possible, so latency calculation for ASIC is simple
					asic_latency[a] = ((asic_latency[a] > asic_latency[b]) ? asic_latency[a] : asic_latency[b]) + asic_op_latency[instrType];

					rotated[a] = is_rotation[instrType];

					inst_data[a] = code_size + (instrType << 8) + ((inst_data[b] & 255) << 16);

					prog(code_size).opcode = lightInstructionOpcode[instrType];
					prog(code_size).dst = dst_index;
					prog(code_size).src = src_index;
					prog(code_size).setImm32(imm32);

					if (instrType == LightInstruction::IADD_RC || instrType == LightInstruction::IMUL_9C || instrType == LightInstruction::IMULH_R || instrType == LightInstruction::ISMULH_R)
					{
						// ADD instruction is implemented as two 1-cycle instructions on a real CPU, so mark ALU as busy for the next cycle too
						alu_busy[next_latency - op_latency[instrType] + 1][alu_index] = true;
					}

					++code_size;
					if (code_size >= RANDOMX_LPROG_MIN_SIZE)
					{
						break;
					}
				}
				else
				{
					++num_retries;
					std::cout << "Retry " << num_retries << " with code_size = " << code_size << ", next_latency = " << next_latency << std::endl;
				}
			}

			// ASIC has more execution resources and can extract as much parallelism from the code as possible
			// We need to add a few more MUL and ROR instructions to achieve minimal required latency for ASIC
			// Get this latency for at least 1 of the 4 registers
			const int prev_code_size = code_size;
			if ((code_size < RANDOMX_LPROG_MAX_SIZE) && (asic_latency[indexRegister] < RANDOMX_LPROG_ASIC_LATENCY))
			{
				int min_idx = indexRegister;
				int max_idx = 0;
				for (int i = 1; i < 8; ++i)
				{
					//if (asic_latency[i] < asic_latency[min_idx]) min_idx = i;
					if (asic_latency[i] > asic_latency[max_idx]) max_idx = i;
				}

				const int pattern[3] = { LightInstruction::IMUL_R, LightInstruction::IROR_R, LightInstruction::IMUL_R };
				const int instrType = pattern[(code_size - prev_code_size) % 3];
				latency[min_idx] = latency[max_idx] + op_latency[instrType];
				asic_latency[min_idx] = asic_latency[max_idx] + asic_op_latency[instrType];

				prog(code_size).opcode = lightInstructionOpcode[instrType];
				prog(code_size).dst = min_idx;
				prog(code_size).src = max_idx;

				++code_size;
			}

			for (int i = 0; i < 8; ++i) {
				std::cout << "Latency " << i << " = " << latency[i] << std::endl;
			}

			std::cout << "Code size = " << code_size << std::endl;
			std::cout << "ALUs:" << std::endl;
			for (int i = 0; i < RANDOMX_LPROG_LATENCY + 1; ++i) {
				for (int j = 0; j < ALU_COUNT; ++j) {
					std::cout << (alu_busy[i][j] ? '*' : '_');
				}
				std::cout << std::endl;
			}

			// There is ~98.15% chance that loop condition is false, so this loop will execute only 1 iteration most of the time
			// It never does more than 4 iterations for all block heights < 10,000,000
		} while ((code_size < RANDOMX_LPROG_MIN_SIZE) || (code_size > RANDOMX_LPROG_MAX_SIZE));

		prog.setSize(code_size);
	}
}