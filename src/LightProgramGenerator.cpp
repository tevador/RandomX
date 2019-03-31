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
#include <vector>

namespace RandomX {
                                            //                             Intel Ivy Bridge reference
	namespace LightInstructionType {        //uOPs (decode)   execution ports         latency       code size
		constexpr int IADD_R = 0;           //1               p015                    1               3
		constexpr int IADD_C = 1;           //1               p015                    1               7
		constexpr int IADD_RC = 2;          //1               p1                      3               8
		constexpr int ISUB_R = 3;           //1               p015                    1               3
		constexpr int IMUL_9C = 4;          //1               p1                      3               8
		constexpr int IMUL_R = 5;           //1               p1                      3               4
		constexpr int IMUL_C = 6;           //1               p1                      3               7
		constexpr int IMULH_R = 7;          //1+2+1           0+(p1,p5)+0             3               3+3+3
		constexpr int ISMULH_R = 8;         //1+2+1           0+(p1,p5)+0             3               3+3+3
		constexpr int IMUL_RCP = 9;         //1+1             p015+p1                 4              10+4
		constexpr int IXOR_R = 10;          //1               p015                    1               3
		constexpr int IXOR_C = 11;          //1               p015                    1               7
		constexpr int IROR_R = 12;          //1+2             0+(p0,p5)               1               3+3
		constexpr int IROR_C = 13;          //1               p05                     1               4
		constexpr int COND_R = 14;          //1+1+1+1+1+1     p015+p5+0+p015+p05+p015 3               7+13+3+7+3+3
		constexpr int COUNT = 15;
	}

	namespace LightInstructionOpcode {
		constexpr int IADD_R = 0;
		constexpr int IADD_RC = RANDOMX_FREQ_IADD_R + RANDOMX_FREQ_IADD_M;
		constexpr int ISUB_R = IADD_RC + RANDOMX_FREQ_IADD_RC;
		constexpr int IMUL_9C = ISUB_R + RANDOMX_FREQ_ISUB_R + RANDOMX_FREQ_ISUB_M;
		constexpr int IMUL_R = IMUL_9C + RANDOMX_FREQ_IMUL_9C;
		constexpr int IMULH_R = IMUL_R + RANDOMX_FREQ_IMUL_R + RANDOMX_FREQ_IMUL_M;
		constexpr int ISMULH_R = IMULH_R + RANDOMX_FREQ_IMULH_R + RANDOMX_FREQ_IMULH_M;
		constexpr int IMUL_RCP = ISMULH_R + RANDOMX_FREQ_ISMULH_R + RANDOMX_FREQ_ISMULH_M;
		constexpr int IXOR_R = IMUL_RCP + RANDOMX_FREQ_IMUL_RCP + RANDOMX_FREQ_INEG_R;
		constexpr int IROR_R = IXOR_R + RANDOMX_FREQ_IXOR_R + RANDOMX_FREQ_IXOR_M;
		constexpr int COND_R = IROR_R + RANDOMX_FREQ_IROR_R + RANDOMX_FREQ_IROL_R + RANDOMX_FREQ_ISWAP_R + RANDOMX_FREQ_FSWAP_R + RANDOMX_FREQ_FADD_R + RANDOMX_FREQ_FADD_M + RANDOMX_FREQ_FSUB_R + RANDOMX_FREQ_FSUB_M + RANDOMX_FREQ_FSCAL_R + RANDOMX_FREQ_FMUL_R + RANDOMX_FREQ_FDIV_M + RANDOMX_FREQ_FSQRT_R;
	}

	const int lightInstructionOpcode[] = {
		LightInstructionOpcode::IADD_R,
		LightInstructionOpcode::IADD_R,
		LightInstructionOpcode::IADD_RC,
		LightInstructionOpcode::ISUB_R,
		LightInstructionOpcode::IMUL_9C,
		LightInstructionOpcode::IMUL_R,
		LightInstructionOpcode::IMUL_R,
		LightInstructionOpcode::IMULH_R,
		LightInstructionOpcode::ISMULH_R,
		LightInstructionOpcode::IMUL_RCP,
		LightInstructionOpcode::IXOR_R,
		LightInstructionOpcode::IXOR_R,
		LightInstructionOpcode::IROR_R,
		LightInstructionOpcode::IROR_R,
		LightInstructionOpcode::COND_R
	};

	const int lightInstruction[] = {
		LightInstructionType::IADD_R,
		LightInstructionType::IADD_C,
		LightInstructionType::IADD_RC,
		LightInstructionType::ISUB_R,
		LightInstructionType::IMUL_9C,
		LightInstructionType::IMUL_R,
		LightInstructionType::IMUL_R,
		LightInstructionType::IMUL_C,
		LightInstructionType::IMULH_R,
		LightInstructionType::ISMULH_R,
		LightInstructionType::IMUL_RCP,
		LightInstructionType::IXOR_R,
		LightInstructionType::IXOR_C,
		LightInstructionType::IROR_R,
		LightInstructionType::IROR_C,
		LightInstructionType::COND_R
	};

	namespace ExecutionPort {
		using type = int;
		constexpr type Null = 0;
		constexpr type P0 = 1;
		constexpr type P1 = 2;
		constexpr type P5 = 4;
		constexpr type P05 = 6;
		constexpr type P015 = 7;
	}

	class Blake2Generator {
	public:
		Blake2Generator(const void* seed) : dataIndex(sizeof(data)) {
			memset(data, 0, sizeof(data));
			memcpy(data, seed, SeedSize);
			data[60] = 39;
		}

		uint8_t getByte() {
			checkData(1);
			return data[dataIndex++];
		}

		uint32_t getInt32() {
			checkData(4);
			auto ret = load32(&data[dataIndex]);
			dataIndex += 4;
			return ret;
		}

	private:
		uint8_t data[64];
		size_t dataIndex;

		void checkData(const size_t bytesNeeded) {
			if (dataIndex + bytesNeeded > sizeof(data))	{
				blake2b(data, sizeof(data), data, sizeof(data), nullptr, 0);
				dataIndex = 0;
			}
		}
	};

	class MacroOp {
	public:
		MacroOp(const char* name, int size)
			: name_(name), size_(size), latency_(0), uop1_(ExecutionPort::Null), uop2_(ExecutionPort::Null) {}
		MacroOp(const char* name, int size, int latency, ExecutionPort::type uop)
			: name_(name), size_(size), latency_(latency), uop1_(uop), uop2_(ExecutionPort::Null) {}
		MacroOp(const char* name, int size, int latency, ExecutionPort::type uop1, ExecutionPort::type uop2)
			: name_(name), size_(size), latency_(latency), uop1_(uop1), uop2_(uop2) {}
		const char* getName() const {
			return name_;
		}
		int getSize() const {
			return size_;
		}
		int getLatency() const {
			return latency_;
		}
		ExecutionPort::type getUop1() const {
			return uop1_;
		}
		ExecutionPort::type getUop2() const {
			return uop2_;
		}
		bool isSimple() const {
			return uop2_ == ExecutionPort::Null;
		}
		bool isEliminated() const {
			return uop1_ == ExecutionPort::Null;
		}
		static const MacroOp Add_rr;
		static const MacroOp Add_ri;
		static const MacroOp Lea_sib;
		static const MacroOp Sub_rr;
		static const MacroOp Imul_rr;
		static const MacroOp Imul_rri;
		static const MacroOp Imul_r;
		static const MacroOp Mul_r;
		static const MacroOp Mov_rr;
		static const MacroOp Mov_ri64;
		static const MacroOp Xor_rr;
		static const MacroOp Xor_ri;
		static const MacroOp Ror_rcl;
		static const MacroOp Ror_ri;
		static const MacroOp TestJmp_fused;
		static const MacroOp Xor_self;
		static const MacroOp Cmp_ri;
		static const MacroOp Setcc_r;
	private:
		const char* name_;
		int size_;
		int latency_;
		ExecutionPort::type uop1_;
		ExecutionPort::type uop2_;
	};

	const MacroOp MacroOp::Add_rr = MacroOp("add r,r", 3, 1, ExecutionPort::P015);
	const MacroOp MacroOp::Add_ri = MacroOp("add r,i", 7, 1, ExecutionPort::P015);
	const MacroOp MacroOp::Lea_sib = MacroOp("lea r,m", 8, 3, ExecutionPort::P1);
	const MacroOp MacroOp::Sub_rr = MacroOp("sub r,r", 3, 1, ExecutionPort::P015);
	const MacroOp MacroOp::Imul_rr = MacroOp("imul r,r", 4, 3, ExecutionPort::P1);
	const MacroOp MacroOp::Imul_rri = MacroOp("imul r,r,i", 7, 3, ExecutionPort::P1);
	const MacroOp MacroOp::Imul_r = MacroOp("imul r", 3, 3, ExecutionPort::P1, ExecutionPort::P5);
	const MacroOp MacroOp::Mul_r = MacroOp("mul r", 3, 3, ExecutionPort::P1, ExecutionPort::P5);
	const MacroOp MacroOp::Mov_rr = MacroOp("mov r,r", 3);
	const MacroOp MacroOp::Mov_ri64 = MacroOp("mov rax,i64", 10, 1, ExecutionPort::P015);
	const MacroOp MacroOp::Xor_rr = MacroOp("xor r,r", 3, 1, ExecutionPort::P015);
	const MacroOp MacroOp::Xor_ri = MacroOp("xor r,i", 7, 1, ExecutionPort::P015);
	const MacroOp MacroOp::Ror_rcl = MacroOp("ror r,cl", 3, 1, ExecutionPort::P0, ExecutionPort::P5);
	const MacroOp MacroOp::Ror_ri = MacroOp("ror r,i", 4, 1, ExecutionPort::P05);
	const MacroOp MacroOp::Xor_self = MacroOp("xor rcx,rcx", 3);
	const MacroOp MacroOp::Cmp_ri = MacroOp("cmp r,i", 7, 1, ExecutionPort::P015);
	const MacroOp MacroOp::Setcc_r = MacroOp("setcc cl", 3, 1, ExecutionPort::P05);
	const MacroOp MacroOp::TestJmp_fused = MacroOp("testjmp r,i", 13, 0, ExecutionPort::P5);

	template <typename T, size_t N>
	T* begin(T(&arr)[N]) { return &arr[0]; }
	template <typename T, size_t N>
	T* end(T(&arr)[N]) { return &arr[0] + N; }

	const MacroOp* IMULH_R_ops_array[] = { &MacroOp::Mov_rr, &MacroOp::Mul_r, &MacroOp::Mov_rr };
	const MacroOp* ISMULH_R_ops_array[] = { &MacroOp::Mov_rr, &MacroOp::Imul_r, &MacroOp::Mov_rr };
	const MacroOp* IMUL_RCP_ops_array[] = { &MacroOp::Mov_ri64, &MacroOp::Imul_rr };
	const MacroOp* IROR_R_ops_array[] = { &MacroOp::Mov_rr, &MacroOp::Ror_rcl };
	const MacroOp* COND_R_ops_array[] = { &MacroOp::Add_ri, &MacroOp::TestJmp_fused, &MacroOp::Xor_self, &MacroOp::Cmp_ri, &MacroOp::Setcc_r, &MacroOp::Add_rr };


	class LightInstructionInfo {
	public:
		LightInstructionInfo(const char* name, const MacroOp* op)
			: name_(name), op_(op), opsCount_(1), latency_(op->getLatency()) {}
		template <size_t N>
		LightInstructionInfo(const char* name, const MacroOp*(&arr)[N])
			: name_(name), ops_(arr), opsCount_(N), latency_(0) {
			for (unsigned i = 0; i < N; ++i) {
				latency_ += arr[i]->getLatency();
			}
			static_assert(N > 1, "Invalid array size");
		}
		template <size_t N>
		LightInstructionInfo(const char* name, const MacroOp*(&arr)[N], int latency)
			: name_(name), ops_(arr), opsCount_(N), latency_(latency) {
			static_assert(N > 1, "Invalid array size");
		}
		const char* getName() const {
			return name_;
		}
		int getSize() const {
			return opsCount_;
		}
		bool isSimple() const {
			return opsCount_ == 1;
		}
		int getLatency() const {
			return latency_;
		}
		const MacroOp* getOp(int index) const {
			return opsCount_ > 1 ? ops_[index] : op_;
		}
		static const LightInstructionInfo IADD_R;
		static const LightInstructionInfo IADD_C;
		static const LightInstructionInfo IADD_RC;
		static const LightInstructionInfo ISUB_R;
		static const LightInstructionInfo IMUL_9C;
		static const LightInstructionInfo IMUL_R;
		static const LightInstructionInfo IMUL_C;
		static const LightInstructionInfo IMULH_R;
		static const LightInstructionInfo ISMULH_R;
		static const LightInstructionInfo IMUL_RCP;
		static const LightInstructionInfo IXOR_R;
		static const LightInstructionInfo IXOR_C;
		static const LightInstructionInfo IROR_R;
		static const LightInstructionInfo IROR_C;
		static const LightInstructionInfo COND_R;
		static const LightInstructionInfo NOP;
	private:
		const char* name_;
		union {
			const MacroOp** ops_;
			const MacroOp* op_;
		};
		int opsCount_;
		int latency_;

		LightInstructionInfo(const char* name)
			: name_(name), opsCount_(0), latency_(0) {}
	};

	const LightInstructionInfo LightInstructionInfo::IADD_R = LightInstructionInfo("IADD_R", &MacroOp::Add_rr);
	const LightInstructionInfo LightInstructionInfo::IADD_C = LightInstructionInfo("IADD_C", &MacroOp::Add_ri);
	const LightInstructionInfo LightInstructionInfo::IADD_RC = LightInstructionInfo("IADD_RC", &MacroOp::Lea_sib);
	const LightInstructionInfo LightInstructionInfo::ISUB_R = LightInstructionInfo("ISUB_R", &MacroOp::Sub_rr);
	const LightInstructionInfo LightInstructionInfo::IMUL_9C = LightInstructionInfo("IMUL_9C", &MacroOp::Lea_sib);
	const LightInstructionInfo LightInstructionInfo::IMUL_R = LightInstructionInfo("IMUL_R", &MacroOp::Imul_rr);
	const LightInstructionInfo LightInstructionInfo::IMUL_C = LightInstructionInfo("IMUL_C", &MacroOp::Imul_rri);
	const LightInstructionInfo LightInstructionInfo::IMULH_R = LightInstructionInfo("IMULH_R", IMULH_R_ops_array);
	const LightInstructionInfo LightInstructionInfo::ISMULH_R = LightInstructionInfo("ISMULH_R", ISMULH_R_ops_array);
	const LightInstructionInfo LightInstructionInfo::IMUL_RCP = LightInstructionInfo("IMUL_RCP", IMUL_RCP_ops_array);
	const LightInstructionInfo LightInstructionInfo::IXOR_R = LightInstructionInfo("IXOR_R", &MacroOp::Xor_rr);
	const LightInstructionInfo LightInstructionInfo::IXOR_C = LightInstructionInfo("IXOR_C", &MacroOp::Xor_ri);
	const LightInstructionInfo LightInstructionInfo::IROR_R = LightInstructionInfo("IROR_R", IROR_R_ops_array);
	const LightInstructionInfo LightInstructionInfo::IROR_C = LightInstructionInfo("IROR_C", &MacroOp::Ror_ri);
	const LightInstructionInfo LightInstructionInfo::COND_R = LightInstructionInfo("COND_R", COND_R_ops_array);
	const LightInstructionInfo LightInstructionInfo::NOP = LightInstructionInfo("NOP");

	const int buffer0[] = { 3, 3, 10 };
	const int buffer1[] = { 7, 3, 3, 3 };
	const int buffer2[] = { 3, 3, 3, 7 };
	const int buffer3[] = { 4, 8, 4 };
	const int buffer4[] = { 4, 4, 4, 4 };
	const int buffer5[] = { 3, 7, 3, 3 };
	const int buffer6[] = { 3, 3, 7, 3 };
	const int buffer7[] = { 13, 3 };

	class DecoderBuffer {
	public:
		static DecoderBuffer Default;
		template <size_t N>
		DecoderBuffer(const char* name, int index, const int(&arr)[N])
			: name_(name), index_(index), counts_(arr), opsCount_(N) {}
		const int* getCounts() const {
			return counts_;
		}
		int getSize() const {
			return opsCount_;
		}
		int getIndex() const {
			return index_;
		}
		const char* getName() const {
			return name_;
		}
		const DecoderBuffer& fetchNext(int prevType, Blake2Generator& gen) {
			if (prevType == LightInstructionType::IMULH_R || prevType == LightInstructionType::ISMULH_R)
				return decodeBuffers[0];
			if (index_ == 0) {
				if ((gen.getByte() % 2) == 0)
					return decodeBuffers[3];
				else
					return decodeBuffers[4];
			}
			if (index_ == 2) {
				return decodeBuffers[7];
			}
			if (index_ == 7) {
				return decodeBuffers[1];
			}
			return fetchNextDefault(gen);
		}
	private:
		const char* name_;
		int index_;
		const int* counts_;
		int opsCount_;
		DecoderBuffer() : index_(-1) {}
		static const DecoderBuffer decodeBuffers[8];
		const DecoderBuffer& fetchNextDefault(Blake2Generator& gen) {
			int select;
			do {
				select = gen.getByte() & 7;
			} while (select == 7);
			return decodeBuffers[select];
		}
	};

	const DecoderBuffer DecoderBuffer::decodeBuffers[8] = {
			DecoderBuffer("3,3,10", 0, buffer0),
			DecoderBuffer("7,3,3,3", 1, buffer1),
			DecoderBuffer("3,3,3,7", 2, buffer2),
			DecoderBuffer("4,8,4", 3, buffer3),
			DecoderBuffer("4,4,4,4", 4, buffer4),
			DecoderBuffer("3,7,3,3", 5, buffer5),
			DecoderBuffer("3,3,7,3", 6, buffer6),
			DecoderBuffer("13,3", 7, buffer7),
	};

	DecoderBuffer DecoderBuffer::Default = DecoderBuffer();

	const int slot_3[]  = { LightInstructionType::IADD_R, LightInstructionType::ISUB_R, LightInstructionType::IXOR_R, LightInstructionType::IADD_R };
	const int slot_3L[] = { LightInstructionType::IADD_R, LightInstructionType::ISUB_R, LightInstructionType::IXOR_R, LightInstructionType::IMULH_R, LightInstructionType::ISMULH_R, LightInstructionType::IXOR_R, LightInstructionType::IMULH_R, LightInstructionType::ISMULH_R };
	const int slot_3F[] = { LightInstructionType::IADD_R, LightInstructionType::ISUB_R, LightInstructionType::IXOR_R, LightInstructionType::IROR_R };
	const int slot_4[]  = { LightInstructionType::IMUL_R, LightInstructionType::IROR_C };
	const int slot_7[]  = { LightInstructionType::IADD_C, LightInstructionType::IMUL_C, LightInstructionType::IXOR_C, LightInstructionType::IXOR_C };
	const int slot_7L   =   LightInstructionType::COND_R;
	const int slot_8[]  = { LightInstructionType::IADD_RC, LightInstructionType::IMUL_9C };
	const int slot_10   =   LightInstructionType::IMUL_RCP;

	class LightInstruction {
	public:
		Instruction toInstr() {
			Instruction instr;
			instr.opcode = lightInstructionOpcode[type_];
			instr.dst = dst_;
			instr.src = src_ >= 0 ? src_ : dst_;
			instr.mod = mod_;
			instr.setImm32(imm32_);
			return instr;
		}

		static LightInstruction createForSlot(Blake2Generator& gen, int slotSize, bool isLast = false, bool isFirst = false) {
			switch (slotSize)
			{
			case 3:
				if (isLast) {
					return create(slot_3L[gen.getByte() & 7], gen);
				}
				else if (isFirst) {
					return create(slot_3F[gen.getByte() & 3], gen);
				}
				else {
					return create(slot_3[gen.getByte() & 3], gen);
				}
			case 4:
				return create(slot_4[gen.getByte() & 1], gen);
			case 7:
				if (isLast) {
					return create(slot_7L, gen);
				}
				else {
					return create(slot_7[gen.getByte() & 3], gen);
				}
			case 8:
				return create(slot_8[gen.getByte() & 1], gen);
			case 10:
				return create(slot_10, gen);
			default:
				break;
			}
		}

		static LightInstruction create(int type, Blake2Generator& gen) {
			LightInstruction li;
			li.type_ = type;
			li.opGroup_ = type;
			switch (type)
			{
			case LightInstructionType::IADD_R: {
				li.dst_ = gen.getByte() & 7;
				do {
					li.src_ = gen.getByte() & 7;
				} while (li.dst_ == li.src_);
				li.mod_ = 0;
				li.imm32_ = 0;
				li.info_ = &LightInstructionInfo::IADD_R;
				li.opGroup_ = LightInstructionType::IADD_R;
				li.opGroupPar_ = li.src_;
			} break;

			case LightInstructionType::IADD_C: {
				li.dst_ = gen.getByte() & 7;
				li.src_ = -1;
				li.mod_ = 0;
				li.imm32_ = gen.getInt32();
				li.info_ = &LightInstructionInfo::IADD_C;
				li.opGroup_ = LightInstructionType::IADD_R;
				li.opGroupPar_ = li.src_;
			} break;

			case LightInstructionType::IADD_RC: {
				li.dst_ = gen.getByte() & 7;
				do {
					li.src_ = gen.getByte() & 7;
				} while (li.dst_ == li.src_);
				li.mod_ = 0;
				li.imm32_ = gen.getInt32();
				li.info_ = &LightInstructionInfo::IADD_RC;
				li.opGroup_ = LightInstructionType::IADD_R;
				li.opGroupPar_ = li.src_;
			} break;

			case LightInstructionType::ISUB_R: {
				li.dst_ = gen.getByte() & 7;
				do {
					li.src_ = gen.getByte() & 7;
				} while (li.dst_ == li.src_);
				li.mod_ = 0;
				li.imm32_ = 0;
				li.info_ = &LightInstructionInfo::ISUB_R;
				li.opGroup_ = LightInstructionType::IADD_R;
				li.opGroupPar_ = li.src_;
			} break;

			case LightInstructionType::IMUL_9C: {
				li.dst_ = gen.getByte() & 7;
				do {
					li.src_ = gen.getByte() & 7;
				} while (li.dst_ == li.src_);
				li.mod_ = 0;
				li.imm32_ = gen.getInt32();
				li.info_ = &LightInstructionInfo::IMUL_9C;
				li.opGroup_ = LightInstructionType::IMUL_C;
				li.opGroupPar_ = -1;
			} break;

			case LightInstructionType::IMUL_R: {
				li.dst_ = gen.getByte() & 7;
				do {
					li.src_ = gen.getByte() & 7;
				} while (li.dst_ == li.src_);
				li.mod_ = 0;
				li.imm32_ = 0;
				li.info_ = &LightInstructionInfo::IMUL_R;
				li.opGroup_ = LightInstructionType::IMUL_R;
				li.opGroupPar_ = gen.getInt32();
			} break;

			case LightInstructionType::IMUL_C: {
				li.dst_ = gen.getByte() & 7;
				li.src_ = -1;
				li.mod_ = 0;
				li.imm32_ = gen.getInt32();
				li.info_ = &LightInstructionInfo::IMUL_C;
				li.opGroup_ = LightInstructionType::IMUL_C;
				li.opGroupPar_ = li.src_;
			} break;

			case LightInstructionType::IMULH_R: {
				li.dst_ = gen.getByte() & 7;
				li.src_ = gen.getByte() & 7;
				li.mod_ = 0;
				li.imm32_ = 0;
				li.info_ = &LightInstructionInfo::IMULH_R;
				li.opGroup_ = LightInstructionType::IMULH_R;
				li.opGroupPar_ = gen.getInt32();
			} break;

			case LightInstructionType::ISMULH_R: {
				li.dst_ = gen.getByte() & 7;
				li.src_ = gen.getByte() & 7;
				li.mod_ = 0;
				li.imm32_ = 0;
				li.info_ = &LightInstructionInfo::ISMULH_R;
				li.opGroup_ = LightInstructionType::ISMULH_R;
				li.opGroupPar_ = gen.getInt32();
			} break;

			case LightInstructionType::IMUL_RCP: {
				li.dst_ = gen.getByte() & 7;
				li.src_ = -1;
				li.mod_ = 0;
				li.imm32_ = gen.getInt32();
				li.info_ = &LightInstructionInfo::IMUL_RCP;
				li.opGroup_ = LightInstructionType::IMUL_C;
				li.opGroupPar_ = -1;
			} break;

			case LightInstructionType::IXOR_R: {
				li.dst_ = gen.getByte() & 7;
				do {
					li.src_ = gen.getByte() & 7;
				} while (li.dst_ == li.src_);
				li.mod_ = 0;
				li.imm32_ = 0;
				li.info_ = &LightInstructionInfo::IXOR_R;
				li.opGroup_ = LightInstructionType::IXOR_R;
				li.opGroupPar_ = li.src_;
			} break;

			case LightInstructionType::IXOR_C: {
				li.dst_ = gen.getByte() & 7;
				li.src_ = -1;
				li.mod_ = 0;
				li.imm32_ = gen.getInt32();
				li.info_ = &LightInstructionInfo::IXOR_C;
				li.opGroup_ = LightInstructionType::IXOR_R;
				li.opGroupPar_ = li.src_;
			} break;

			case LightInstructionType::IROR_R: {
				li.dst_ = gen.getByte() & 7;
				do {
					li.src_ = gen.getByte() & 7;
				} while (li.dst_ == li.src_);
				li.mod_ = 0;
				li.imm32_ = 0;
				li.info_ = &LightInstructionInfo::IROR_R;
				li.opGroup_ = LightInstructionType::IROR_R;
				li.opGroupPar_ = -1;
			} break;

			case LightInstructionType::IROR_C: {
				li.dst_ = gen.getByte() & 7;
				li.src_ = -1;
				li.mod_ = 0;
				li.imm32_ = gen.getByte();
				li.info_ = &LightInstructionInfo::IROR_C;
				li.opGroup_ = LightInstructionType::IROR_R;
				li.opGroupPar_ = -1;
			} break;

			case LightInstructionType::COND_R: {
				li.dst_ = gen.getByte() & 7;
				li.src_ = gen.getByte() & 7;
				li.mod_ = gen.getByte();
				li.imm32_ = gen.getInt32();
				li.info_ = &LightInstructionInfo::COND_R;
				li.opGroup_ = LightInstructionType::COND_R;
				li.opGroupPar_ = li.imm32_;
			} break;

			default:
				break;
			}

			return li;
		}

		int getType() {
			return type_;
		}
		int getSource() {
			return src_;
		}
		int getDestination() {
			return dst_;
		}
		int getGroup() {
			return opGroup_;
		}
		int getGroupPar() {
			return opGroupPar_;
		}

		const LightInstructionInfo* getInfo() {
			return info_;
		}

		static const LightInstruction Null;

	private:
		int type_;
		int src_;
		int dst_;
		int mod_;
		uint32_t imm32_;

		const LightInstructionInfo* info_;
		int opGroup_;
		int opGroupPar_;

		LightInstruction() {}
		LightInstruction(int type, const LightInstructionInfo* info) : type_(type), info_(info) {}
	};

	class RegisterInfo {
	public:
		RegisterInfo() : lastOpGroup(-1), source(-1), value(0), latency(0) {}
		int lastOpGroup;
		int source;
		int value;
		int latency;
	};

	const LightInstruction LightInstruction::Null = LightInstruction(-1, &LightInstructionInfo::NOP);

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

	void generateLightProg2(LightProgram& prog, const void* seed, int indexRegister) {

		bool portBusy[RANDOMX_LPROG_LATENCY][3];
		RegisterInfo registers[8];
		bool decoderBusy[RANDOMX_LPROG_LATENCY][4];
		Blake2Generator gen(seed);
		std::vector<LightInstruction> instructions;

		DecoderBuffer& fetchLine = DecoderBuffer::Default;
		LightInstruction currentInstruction = LightInstruction::Null;
		int instrIndex = 0;
		int codeSize = 0;
		int macroOpCount = 0;
		int rxOpCount = 0;

		for (int cycle = 0; cycle < 170; ++cycle) {
			fetchLine = fetchLine.fetchNext(currentInstruction.getType(), gen);
			std::cout << "; cycle " << cycle << " buffer " << fetchLine.getName() << std::endl;

			int mopIndex = 0;
			
			while (mopIndex < fetchLine.getSize()) {
				if (instrIndex >= currentInstruction.getInfo()->getSize()) {
					currentInstruction = LightInstruction::createForSlot(gen, fetchLine.getCounts()[mopIndex], fetchLine.getSize() == mopIndex + 1, fetchLine.getIndex() == 0 && mopIndex == 0);
					instrIndex = 0;
					std::cout << "; " << currentInstruction.getInfo()->getName() << std::endl;
					rxOpCount++;
				}
				if (fetchLine.getCounts()[mopIndex] != currentInstruction.getInfo()->getOp(instrIndex)->getSize()) {
					std::cout << "ERROR instruction " << currentInstruction.getInfo()->getOp(instrIndex)->getName() << " doesn't fit into slot of size " << fetchLine.getCounts()[mopIndex] << std::endl;
					return;
				}
				std::cout << currentInstruction.getInfo()->getOp(instrIndex)->getName() << std::endl;
				codeSize += currentInstruction.getInfo()->getOp(instrIndex)->getSize();
				mopIndex++;
				instrIndex++;
				macroOpCount++;
			}
		}

		std::cout << "; code size " << codeSize << std::endl;
		std::cout << "; x86 macro-ops: " << macroOpCount << std::endl;
		std::cout << "; RandomX instructions: " << rxOpCount << std::endl;
	}

	void generateLightProgram(LightProgram& prog, const void* seed, int indexRegister) {

		// Source: https://www.agner.org/optimize/instruction_tables.pdf
		const int op_latency[LightInstructionType::COUNT] = { 1, 2, 1, 2, 3, 5, 5, 4, 1, 2, 5 };

		// Instruction latencies for theoretical ASIC implementation
		const int asic_op_latency[LightInstructionType::COUNT] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

		// Available ALUs for each instruction
		const int op_ALUs[LightInstructionType::COUNT] = { ALU_COUNT, ALU_COUNT, ALU_COUNT, ALU_COUNT, ALU_COUNT_MUL, ALU_COUNT_MUL, ALU_COUNT_MUL, ALU_COUNT_MUL, ALU_COUNT, ALU_COUNT, ALU_COUNT };

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
			bool is_rotation[LightInstructionType::COUNT];
			bool rotated[8];
			int rotate_count = 0;

			memset(latency, 0, sizeof(latency));
			memset(asic_latency, 0, sizeof(asic_latency));
			memset(alu_busy, 0, sizeof(alu_busy));
			memset(is_rotation, 0, sizeof(is_rotation));
			memset(rotated, 0, sizeof(rotated));
			is_rotation[LightInstructionType::IROR_R] = true;

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
				if (instrType != LightInstructionType::IMULH_R && instrType != LightInstructionType::ISMULH_R && ((inst_data[a] & 0xFFFF00) == (instrType << 8) + ((inst_data[b] & 255) << 16)))
				{
					continue;
				}

				if ((instrType == LightInstructionType::IADD_RC) || (instrType == LightInstructionType::IMUL_9C) || (instrType == LightInstructionType::IMUL_RCP) || (instrType == LightInstructionType::COND_R) || ((instrType != LightInstructionType::IMULH_R) && (instrType != LightInstructionType::ISMULH_R) && (a == b)))
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
							if ((instrType == LightInstructionType::IADD_RC || instrType == LightInstructionType::IMUL_9C || instrType == LightInstructionType::IMULH_R || instrType == LightInstructionType::ISMULH_R) && alu_busy[next_latency + 1][i])
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

					if (instrType == LightInstructionType::IADD_RC || instrType == LightInstructionType::IMUL_9C || instrType == LightInstructionType::IMULH_R || instrType == LightInstructionType::ISMULH_R)
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

				const int pattern[3] = { LightInstructionType::IMUL_R, LightInstructionType::IROR_R, LightInstructionType::IMUL_R };
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