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
#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include "LightProgramGenerator.hpp"

namespace RandomX {

	namespace LightInstructionOpcode {
		constexpr int IADD_RS = 0;
		constexpr int IADD_RC = RANDOMX_FREQ_IADD_RS + RANDOMX_FREQ_IADD_M;
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

	static bool isMul(int type) {
		return type == LightInstructionType::IMUL_R || type == LightInstructionType::IMULH_R || type == LightInstructionType::ISMULH_R || type == LightInstructionType::IMUL_RCP;
	}

	const int lightInstructionOpcode[] = {
		LightInstructionOpcode::IADD_RS,
		LightInstructionOpcode::ISUB_R,    //ISUB_R
		LightInstructionOpcode::ISUB_R,    //ISUB_R
		LightInstructionOpcode::IMUL_R,    //IMUL_R
		LightInstructionOpcode::IMUL_R,    //IMUL_C
		LightInstructionOpcode::IMULH_R,
		LightInstructionOpcode::ISMULH_R,
		LightInstructionOpcode::IMUL_RCP,
		LightInstructionOpcode::IXOR_R,    //IXOR_R
		LightInstructionOpcode::IXOR_R,    //IXOR_C
		LightInstructionOpcode::IROR_R,    //IROR_R
		LightInstructionOpcode::IROR_R,    //IROR_C
		LightInstructionOpcode::COND_R
	};

	namespace ExecutionPort {
		using type = int;
		constexpr type Null = 0;
		constexpr type P0 = 1;
		constexpr type P1 = 2;
		constexpr type P5 = 3;
		constexpr type P01 = 4;
		constexpr type P05 = 5;
		constexpr type P015 = 6;
	}

	Blake2Generator::Blake2Generator(const void* seed, int nonce) : dataIndex(sizeof(data)) {
		memset(data, 0, sizeof(data));
		memcpy(data, seed, SeedSize);
		store32(&data[60], nonce);
	}

	uint8_t Blake2Generator::getByte() {
		checkData(1);
		return data[dataIndex++];
	}

	uint32_t Blake2Generator::getInt32() {
		checkData(4);
		auto ret = load32(&data[dataIndex]);
		dataIndex += 4;
		return ret;
	}

	void Blake2Generator::checkData(const size_t bytesNeeded) {
		if (dataIndex + bytesNeeded > sizeof(data))	{
			blake2b(data, sizeof(data), data, sizeof(data), nullptr, 0);
			dataIndex = 0;
		}
	}

	class RegisterInfo {
	public:
		RegisterInfo() : latency(0), lastOpGroup(-1), lastOpPar(-1), value(0) {}
		int latency;
		int lastOpGroup;
		int lastOpPar;
		int value;
	};

	class MacroOp {
	public:
		MacroOp(const char* name, int size)
			: name_(name), size_(size), latency_(0), uop1_(ExecutionPort::Null), uop2_(ExecutionPort::Null) {}
		MacroOp(const char* name, int size, int latency, ExecutionPort::type uop)
			: name_(name), size_(size), latency_(latency), uop1_(uop), uop2_(ExecutionPort::Null) {}
		MacroOp(const char* name, int size, int latency, ExecutionPort::type uop1, ExecutionPort::type uop2)
			: name_(name), size_(size), latency_(latency), uop1_(uop1), uop2_(uop2) {}
		MacroOp(const MacroOp& parent, bool dependent)
			: name_(parent.name_), size_(parent.size_), latency_(parent.latency_), uop1_(parent.uop1_), uop2_(parent.uop2_), dependent_(dependent) {}
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
		bool isDependent() const {
			return dependent_;
		}
		int getCycle() const {
			return cycle_;
		}
		void setCycle(int cycle) {
			cycle_ = cycle;
		}
		MacroOp* getSrcDep() const {
			return depSrc_;
		}
		void setSrcDep(MacroOp* src) {
			depSrc_ = src;
		}
		MacroOp* getDstDep() const {
			return depDst_;
		}
		void setDstDep(MacroOp* dst) {
			depDst_ = dst;
		}
		static const MacroOp Add_rr;
		static const MacroOp Add_ri;
		static const MacroOp Lea_sib;
		static const MacroOp Sub_rr;
		static const MacroOp Sub_ri;
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
		static const MacroOp TestJz_fused;
		static const MacroOp Xor_self;
		static const MacroOp Cmp_ri;
		static const MacroOp Setcc_r;
	private:
		const char* name_;
		int size_;
		int latency_;
		ExecutionPort::type uop1_;
		ExecutionPort::type uop2_;
		int cycle_;
		bool dependent_ = false;
		MacroOp* depDst_ = nullptr;
		MacroOp* depSrc_ = nullptr;
	};

	const MacroOp MacroOp::Add_rr = MacroOp("add r,r", 3, 1, ExecutionPort::P015);
	const MacroOp MacroOp::Add_ri = MacroOp("add r,i", 7, 1, ExecutionPort::P015);
	const MacroOp MacroOp::Lea_sib = MacroOp("lea r,r+r*s", 4, 1, ExecutionPort::P01);
	const MacroOp MacroOp::Sub_rr = MacroOp("sub r,r", 3, 1, ExecutionPort::P015);
	const MacroOp MacroOp::Sub_ri = MacroOp("sub r,i", 7, 1, ExecutionPort::P015);
	const MacroOp MacroOp::Imul_rr = MacroOp("imul r,r", 4, 3, ExecutionPort::P1);
	const MacroOp MacroOp::Imul_rri = MacroOp("imul r,r,i", 7, 3, ExecutionPort::P1);
	const MacroOp MacroOp::Imul_r = MacroOp("imul r", 3, 4, ExecutionPort::P1, ExecutionPort::P5);
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
	const MacroOp MacroOp::TestJz_fused = MacroOp("testjz r,i", 13, 0, ExecutionPort::P5);

	const MacroOp IMULH_R_ops_array[] = { MacroOp::Mov_rr, MacroOp::Mul_r, MacroOp::Mov_rr };
	const MacroOp ISMULH_R_ops_array[] = { MacroOp::Mov_rr, MacroOp::Imul_r, MacroOp::Mov_rr };
	const MacroOp IMUL_RCP_ops_array[] = { MacroOp::Mov_ri64, MacroOp(MacroOp::Imul_rr, true) };
	const MacroOp IROR_R_ops_array[] = { MacroOp::Mov_rr, MacroOp::Ror_rcl };
	const MacroOp COND_R_ops_array[] = { MacroOp::Add_ri, MacroOp(MacroOp::TestJz_fused, true), MacroOp::Xor_self, MacroOp::Cmp_ri, MacroOp(MacroOp::Setcc_r, true), MacroOp(MacroOp::Add_rr, true) };


	class LightInstructionInfo {
	public:
		LightInstructionInfo(const char* name, int type, const MacroOp& op, int srcOp)
			: name_(name), type_(type), latency_(op.getLatency()), srcOp_(srcOp) {
			ops_.push_back(MacroOp(op));
		}
		template <size_t N>
		LightInstructionInfo(const char* name, int type, const MacroOp(&arr)[N], int resultOp, int dstOp, int srcOp)
			: name_(name), type_(type), latency_(0), resultOp_(resultOp), dstOp_(dstOp), srcOp_(srcOp) {
			for (unsigned i = 0; i < N; ++i) {
				ops_.push_back(MacroOp(arr[i]));
				latency_ += ops_.back().getLatency();
			}
			static_assert(N > 1, "Invalid array size");
		}
		template <size_t N>
		LightInstructionInfo(const char* name, int type, const MacroOp*(&arr)[N], int latency, int resultOp, int dstOp, int srcOp)
			: name_(name), type_(type), latency_(latency), resultOp_(resultOp), dstOp_(dstOp), srcOp_(srcOp) {
			for (unsigned i = 0; i < N; ++i) {
				ops_.push_back(MacroOp(arr[i]));
				if (arr[i].isDependent()) {
					ops_[i].setSrcDep(&ops_[i - 1]);
				}
			}
			static_assert(N > 1, "Invalid array size");
		}
		const char* getName() const {
			return name_;
		}
		int getSize() const {
			return ops_.size();
		}
		bool isSimple() const {
			return getSize() == 1;
		}
		int getLatency() const {
			return latency_;
		}
		MacroOp& getOp(int index) {
			return ops_[index];
		}
		int getType() const {
			return type_;
		}
		int getResultOp() const {
			return resultOp_;
		}
		int getDstOp() const {
			return dstOp_;
		}
		int getSrcOp() const {
			return srcOp_;
		}
		static const LightInstructionInfo ISUB_R;
		static const LightInstructionInfo IXOR_R;
		static const LightInstructionInfo IADD_RS;
		static const LightInstructionInfo IMUL_R;
		static const LightInstructionInfo IROR_C;
		static const LightInstructionInfo IADD_C7;
		static const LightInstructionInfo IXOR_C7;
		static const LightInstructionInfo IADD_C8;
		static const LightInstructionInfo IXOR_C8;
		static const LightInstructionInfo IADD_C9;
		static const LightInstructionInfo IXOR_C9;
		static const LightInstructionInfo IMULH_R;
		static const LightInstructionInfo ISMULH_R;
		static const LightInstructionInfo IMUL_RCP;
		static const LightInstructionInfo NOP;
	private:
		const char* name_;
		int type_;
		std::vector<MacroOp> ops_;
		int latency_;
		int resultOp_ = 0;
		int dstOp_ = 0;
		int srcOp_;

		LightInstructionInfo(const char* name)
			: name_(name), type_(-1), latency_(0) {}
	};

	const LightInstructionInfo LightInstructionInfo::ISUB_R = LightInstructionInfo("ISUB_R", LightInstructionType::ISUB_R, MacroOp::Sub_rr, 0);
	const LightInstructionInfo LightInstructionInfo::IXOR_R = LightInstructionInfo("IXOR_R", LightInstructionType::IXOR_R, MacroOp::Xor_rr, 0);
	const LightInstructionInfo LightInstructionInfo::IADD_RS = LightInstructionInfo("IADD_RS", LightInstructionType::IADD_RS, MacroOp::Lea_sib, 0);
	const LightInstructionInfo LightInstructionInfo::IMUL_R = LightInstructionInfo("IMUL_R", LightInstructionType::IMUL_R, MacroOp::Imul_rr, 0);
	const LightInstructionInfo LightInstructionInfo::IROR_C = LightInstructionInfo("IROR_C", LightInstructionType::IROR_C, MacroOp::Ror_ri, -1);

	const LightInstructionInfo LightInstructionInfo::IADD_C7 = LightInstructionInfo("IADD_C7", LightInstructionType::IADD_C7, MacroOp::Add_ri, -1);
	const LightInstructionInfo LightInstructionInfo::IXOR_C7 = LightInstructionInfo("IXOR_C7", LightInstructionType::IXOR_C7, MacroOp::Xor_ri, -1);
	const LightInstructionInfo LightInstructionInfo::IADD_C8 = LightInstructionInfo("IADD_C8", LightInstructionType::IADD_C8, MacroOp::Add_ri, -1);
	const LightInstructionInfo LightInstructionInfo::IXOR_C8 = LightInstructionInfo("IXOR_C8", LightInstructionType::IXOR_C8, MacroOp::Xor_ri, -1);
	const LightInstructionInfo LightInstructionInfo::IADD_C9 = LightInstructionInfo("IADD_C9", LightInstructionType::IADD_C9, MacroOp::Add_ri, -1);
	const LightInstructionInfo LightInstructionInfo::IXOR_C9 = LightInstructionInfo("IXOR_C9", LightInstructionType::IXOR_C9, MacroOp::Xor_ri, -1);

	const LightInstructionInfo LightInstructionInfo::IMULH_R = LightInstructionInfo("IMULH_R", LightInstructionType::IMULH_R, IMULH_R_ops_array, 1, 0, 1);
	const LightInstructionInfo LightInstructionInfo::ISMULH_R = LightInstructionInfo("ISMULH_R", LightInstructionType::ISMULH_R, ISMULH_R_ops_array, 1, 0, 1);
	const LightInstructionInfo LightInstructionInfo::IMUL_RCP = LightInstructionInfo("IMUL_RCP", LightInstructionType::IMUL_RCP, IMUL_RCP_ops_array, 1, 1, -1);
	
	const LightInstructionInfo LightInstructionInfo::NOP = LightInstructionInfo("NOP");

	const int buffer0[] = { 4, 8, 4 };
	const int buffer1[] = { 7, 3, 3, 3 };
	const int buffer2[] = { 3, 7, 3, 3 };
	const int buffer3[] = { 4, 9, 3 };
	const int buffer4[] = { 4, 4, 4, 4 };
	const int buffer5[] = { 3, 3, 10 };

	class DecoderBuffer {
	public:
		static const DecoderBuffer Default;
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
		const DecoderBuffer* fetchNext(int instrType, int cycle, int mulCount, Blake2Generator& gen) const {
			if (instrType == LightInstructionType::IMULH_R || instrType == LightInstructionType::ISMULH_R)
				return &decodeBuffer3310; //2-1-1 decode
			if (mulCount < cycle + 1)
				return &decodeBuffer4444;
			if (index_ == 5) { //IMUL_RCP end
				return (gen.getByte() & 1) ? &decodeBuffer484 : &decodeBuffer493;
			}
			return fetchNextDefault(gen);
		}
	private:
		const char* name_;
		int index_;
		const int* counts_;
		int opsCount_;
		DecoderBuffer() : index_(-1) {}
		static const DecoderBuffer decodeBuffer484;
		static const DecoderBuffer decodeBuffer7333;
		static const DecoderBuffer decodeBuffer3733;
		static const DecoderBuffer decodeBuffer493;
		static const DecoderBuffer decodeBuffer4444;
		static const DecoderBuffer decodeBuffer3310;
		static const DecoderBuffer* decodeBuffers[4];
		const DecoderBuffer* fetchNextDefault(Blake2Generator& gen) const {
			return decodeBuffers[gen.getByte() & 3];
		}
	};

	const DecoderBuffer DecoderBuffer::decodeBuffer484 = DecoderBuffer("4,8,4", 0, buffer0);
	const DecoderBuffer DecoderBuffer::decodeBuffer7333 = DecoderBuffer("7,3,3,3", 1, buffer1);
	const DecoderBuffer DecoderBuffer::decodeBuffer3733 = DecoderBuffer("3,7,3,3", 2, buffer2);
	const DecoderBuffer DecoderBuffer::decodeBuffer493 = DecoderBuffer("4,9,3", 3, buffer3);
	const DecoderBuffer DecoderBuffer::decodeBuffer4444 = DecoderBuffer("4,4,4,4", 4, buffer4);
	const DecoderBuffer DecoderBuffer::decodeBuffer3310 = DecoderBuffer("3,3,10", 5, buffer5);

	const DecoderBuffer* DecoderBuffer::decodeBuffers[4] = {
			&DecoderBuffer::decodeBuffer484,
			&DecoderBuffer::decodeBuffer7333,
			&DecoderBuffer::decodeBuffer3733,
			&DecoderBuffer::decodeBuffer493,
	};

	const DecoderBuffer DecoderBuffer::Default = DecoderBuffer();

	const LightInstructionInfo* slot_3[]  = { &LightInstructionInfo::ISUB_R, &LightInstructionInfo::IXOR_R };
	const LightInstructionInfo* slot_3L[] = { &LightInstructionInfo::ISUB_R, &LightInstructionInfo::IXOR_R, &LightInstructionInfo::IMULH_R, &LightInstructionInfo::ISMULH_R };
	const LightInstructionInfo* slot_4[]  = { &LightInstructionInfo::IROR_C, &LightInstructionInfo::IADD_RS };
	const LightInstructionInfo* slot_7[]  = { &LightInstructionInfo::IXOR_C7, &LightInstructionInfo::IADD_C7 };
	const LightInstructionInfo* slot_8[] = { &LightInstructionInfo::IXOR_C8, &LightInstructionInfo::IADD_C8 };
	const LightInstructionInfo* slot_9[] = { &LightInstructionInfo::IXOR_C9, &LightInstructionInfo::IADD_C9 };
	const LightInstructionInfo* slot_10   = &LightInstructionInfo::IMUL_RCP;

	static bool selectRegister(std::vector<int>& availableRegisters, Blake2Generator& gen, int& reg) {
		int index;
		if (availableRegisters.size() == 0)
			return false;
			//throw std::runtime_error("No available registers");

		if (availableRegisters.size() > 1) {
			index = gen.getInt32() % availableRegisters.size();
		}
		else {
			index = 0;
		}
		reg = availableRegisters[index];
		return true;
	}

	class LightInstruction {
	public:
		void toInstr(Instruction& instr) {
			instr.opcode = getType();
			instr.dst = dst_;
			instr.src = src_ >= 0 ? src_ : dst_;
			instr.mod = mod_;
			instr.setImm32(imm32_);
		}

		static LightInstruction createForSlot(Blake2Generator& gen, int slotSize, int fetchType, bool isLast, bool isFirst) {
			switch (slotSize)
			{
			case 3:
				if (isLast) {
					return create(slot_3L[gen.getByte() & 3], gen);
				}
				else {
					return create(slot_3[gen.getByte() & 1], gen);
				}
			case 4:
				if (fetchType == 4 && !isLast) {
					return create(&LightInstructionInfo::IMUL_R, gen);
				}
				else {
					return create(slot_4[gen.getByte() & 1], gen);
				}
			case 7:
				return create(slot_7[gen.getByte() & 1], gen);
			case 8:
				return create(slot_8[gen.getByte() & 1], gen);
			case 9:
				return create(slot_9[gen.getByte() & 1], gen);
			case 10:
				return create(slot_10, gen);
			default:
				throw std::runtime_error("Invalid slot");
			}
		}

		static LightInstruction create(const LightInstructionInfo* info, Blake2Generator& gen) {
			LightInstruction li(info);
			switch (info->getType())
			{
			case LightInstructionType::ISUB_R: {
				li.mod_ = 0;
				li.imm32_ = 0;
				li.opGroup_ = LightInstructionType::IADD_RS;
				li.groupParIsSource_ = true;
			} break;

			case LightInstructionType::IXOR_R: {
				li.mod_ = 0;
				li.imm32_ = 0;
				li.opGroup_ = LightInstructionType::IXOR_R;
				li.groupParIsSource_ = true;
			} break;

			case LightInstructionType::IADD_RS: {
				li.mod_ = gen.getByte();
				li.imm32_ = 0;
				li.opGroup_ = LightInstructionType::IADD_RS;
				li.groupParIsSource_ = true;
			} break;

			case LightInstructionType::IMUL_R: {
				li.mod_ = 0;
				li.imm32_ = 0;
				li.opGroup_ = LightInstructionType::IMUL_R;
				li.opGroupPar_ = -1; //TODO
			} break;

			case LightInstructionType::IROR_C: {
				li.mod_ = 0;
				do {
					li.imm32_ = gen.getByte() & 63;
				} while (li.imm32_ == 0);
				li.opGroup_ = LightInstructionType::IROR_C;
				li.opGroupPar_ = -1;
			} break;

			case LightInstructionType::IADD_C7:
			case LightInstructionType::IADD_C8:
			case LightInstructionType::IADD_C9: {
				li.mod_ = 0;
				li.imm32_ = gen.getInt32();
				li.opGroup_ = LightInstructionType::IADD_C7;
				li.opGroupPar_ = -1;
			} break;

			case LightInstructionType::IXOR_C7:
			case LightInstructionType::IXOR_C8:
			case LightInstructionType::IXOR_C9: {
				li.mod_ = 0;
				li.imm32_ = gen.getInt32();
				li.opGroup_ = LightInstructionType::IXOR_C7;
				li.opGroupPar_ = -1;
			} break;

			case LightInstructionType::IMULH_R: {
				li.canReuse_ = true;
				li.mod_ = 0;
				li.imm32_ = 0;
				li.opGroup_ = LightInstructionType::IMULH_R;
				li.opGroupPar_ = gen.getInt32();
			} break;

			case LightInstructionType::ISMULH_R: {
				li.canReuse_ = true;
				li.mod_ = 0;
				li.imm32_ = 0;
				li.opGroup_ = LightInstructionType::ISMULH_R;
				li.opGroupPar_ = gen.getInt32();
			} break;

			case LightInstructionType::IMUL_RCP: {
				li.mod_ = 0;
				do {
					li.imm32_ = gen.getInt32();
				} while ((li.imm32_ & (li.imm32_ - 1)) == 0);
				li.opGroup_ = LightInstructionType::IMUL_RCP;
				li.opGroupPar_ = -1;
			} break;

			default:
				break;
			}

			return li;
		}

		bool selectDestination(int cycle, RegisterInfo (&registers)[8], Blake2Generator& gen) {
			std::vector<int> availableRegisters;
			for (unsigned i = 0; i < 8; ++i) {
				if (registers[i].latency <= cycle && (canReuse_ || i != src_) && (registers[i].lastOpGroup != opGroup_ || registers[i].lastOpPar != opGroupPar_) && (info_.getType() != LightInstructionType::IADD_RS || i != 5))
					availableRegisters.push_back(i);
			}
			return selectRegister(availableRegisters, gen, dst_);
		}

		bool selectSource(int cycle, RegisterInfo(&registers)[8], Blake2Generator& gen) {
			std::vector<int> availableRegisters;
			for (unsigned i = 0; i < 8; ++i) {
				if (registers[i].latency <= cycle)
					availableRegisters.push_back(i);
			}
			if (availableRegisters.size() == 2 && info_.getType() == LightInstructionType::IADD_RS) {
				if (availableRegisters[0] == 5 || availableRegisters[1] == 5) {
					opGroupPar_ = src_ = 5;
					return true;
				}
			}
			if (selectRegister(availableRegisters, gen, src_)) {
				if (groupParIsSource_)
					opGroupPar_ = src_;
				return true;
			}
			return false;
		}

		int getType() {
			return info_.getType();
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

		LightInstructionInfo& getInfo() {
			return info_;
		}

		static const LightInstruction Null;

	private:
		LightInstructionInfo info_;
		int src_ = -1;
		int dst_ = -1;
		int mod_;
		uint32_t imm32_;
		int opGroup_;
		int opGroupPar_;
		bool canReuse_ = false;
		bool groupParIsSource_ = false;

		LightInstruction(const LightInstructionInfo* info) : info_(*info) {
			for (unsigned i = 0; i < info_.getSize(); ++i) {
				MacroOp& mop = info_.getOp(i);
				if (mop.isDependent()) {
					mop.setSrcDep(&info_.getOp(i - 1));
				}
			}
		}
	};

	const LightInstruction LightInstruction::Null = LightInstruction(&LightInstructionInfo::NOP);

	constexpr int ALU_COUNT_MUL = 1;
	constexpr int ALU_COUNT = 3;
	constexpr int LIGHT_OPCODE_BITS = 4;
	constexpr int V4_SRC_INDEX_BITS = 3;
	constexpr int V4_DST_INDEX_BITS = 3;
	constexpr int CYCLE_MAP_SIZE = RANDOMX_LPROG_LATENCY + 3;
#ifndef _DEBUG
	constexpr bool TRACE = false;
	constexpr bool INFO = false;
#else
	constexpr bool TRACE = true;
	constexpr bool INFO = true;
#endif

	static int blakeCounter = 0;

	template<bool commit>
	static int scheduleUop(const MacroOp& mop, ExecutionPort::type(&portBusy)[CYCLE_MAP_SIZE][3], int cycle, int depCycle) {
		if (mop.isDependent()) {
			cycle = std::max(cycle, depCycle);
		}
		if (mop.isEliminated()) {
			if (commit)
				if (TRACE) std::cout << "; (eliminated)" << std::endl;
			return cycle;
		} 
		else if (mop.isSimple()) {
			if (mop.getUop1() <= ExecutionPort::P5) {
				for (; cycle < CYCLE_MAP_SIZE; ++cycle) {
					if (!portBusy[cycle][mop.getUop1() - 1]) {
						if (commit) {
							if (TRACE) std::cout << "; P" << mop.getUop1() - 1 << " at cycle " << cycle << std::endl;
							portBusy[cycle][mop.getUop1() - 1] = mop.getUop1();
						}
						return cycle;
					}
				}
			}
			else if (mop.getUop1() == ExecutionPort::P01) {
				for (; cycle < CYCLE_MAP_SIZE; ++cycle) {
					if (!portBusy[cycle][0]) {
						if (commit) {
							if (TRACE) std::cout << "; P0 at cycle " << cycle << std::endl;
							portBusy[cycle][0] = mop.getUop1();
						}
						return cycle;
					}
					if (!portBusy[cycle][1]) {
						if (commit) {
							if (TRACE) std::cout << "; P1 at cycle " << cycle << std::endl;
							portBusy[cycle][1] = mop.getUop1();
						}
						return cycle;
					}
				}
			}
			else if (mop.getUop1() == ExecutionPort::P05) {
				for (; cycle < CYCLE_MAP_SIZE; ++cycle) {
					if (!portBusy[cycle][2]) {
						if (commit) {
							if (TRACE) std::cout << "; P2 at cycle " << cycle << std::endl;
							portBusy[cycle][2] = mop.getUop1();
						}
						return cycle;
					}
					if (!portBusy[cycle][0]) {
						if (commit) {
							if (TRACE) std::cout << "; P0 at cycle " << cycle << std::endl;
							portBusy[cycle][0] = mop.getUop1();
						}
						return cycle;
					}
				}
			}
			else {
				for (; cycle < CYCLE_MAP_SIZE; ++cycle) {
					if (!portBusy[cycle][2]) {
						if (commit) {
							if (TRACE) std::cout << "; P2 at cycle " << cycle << std::endl;
							portBusy[cycle][2] = mop.getUop1();
						}
						return cycle;
					}
					if (!portBusy[cycle][0]) {
						if (commit) {
							if (TRACE) std::cout << "; P0 at cycle " << cycle << std::endl;
							portBusy[cycle][0] = mop.getUop1();
						}
						return cycle;
					}
					if (!portBusy[cycle][1]) {
						if (commit) {
							if (TRACE) std::cout << "; P1 at cycle " << cycle << std::endl;
							portBusy[cycle][1] = mop.getUop1();
						}
						return cycle;
					}
				}
			}
		}
		else {
			for (; cycle < CYCLE_MAP_SIZE; ++cycle) {
				if (!portBusy[cycle][mop.getUop1() - 1] && !portBusy[cycle][mop.getUop2() - 1]) {
					if (commit) {
						if (TRACE) std::cout << "; P" << mop.getUop1() - 1 << " P" << mop.getUop2() - 1 << " at cycle " << cycle << std::endl;
						portBusy[cycle][mop.getUop1() - 1] = mop.getUop1();
						portBusy[cycle][mop.getUop2() - 1] = mop.getUop2();
					}
					return cycle;
				}
			}
		}

		if (TRACE) std::cout << "Unable to map operation '" << mop.getName() << "' to execution port (cycle " << cycle << ")" << std::endl;
		return -1;
	}

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

	double generateLightProg2(LightProgram& prog, Blake2Generator& gen) {

		ExecutionPort::type portBusy[CYCLE_MAP_SIZE][3];
		memset(portBusy, 0, sizeof(portBusy));
		RegisterInfo registers[8];
		std::vector<LightInstruction> instructions;

		const DecoderBuffer* fetchLine = &DecoderBuffer::Default;
		LightInstruction currentInstruction = LightInstruction::Null;
		int instrIndex = 0;
		int codeSize = 0;
		int macroOpCount = 0;
		int cycle = 0;
		int fetchCycle = 0;
		int depCycle = 0;
		int retireCycle = 0;
		int mopIndex = 0;
		bool portsSaturated = false;
		int outIndex = 0;
		int attempts = 0;
		int mulCount = 0;
		constexpr int MAX_ATTEMPTS = 4;

		while(!portsSaturated) {
			fetchLine = fetchLine->fetchNext(currentInstruction.getType(), fetchCycle++, mulCount, gen);
			if (TRACE) std::cout << "; ------------- fetch cycle " << cycle << " (" << fetchLine->getName() << ")" << std::endl;

			mopIndex = 0;
			
			while (mopIndex < fetchLine->getSize()) {
				int topCycle = cycle;
				if (instrIndex >= currentInstruction.getInfo().getSize()) {
					if (portsSaturated)
						break;
					currentInstruction = LightInstruction::createForSlot(gen, fetchLine->getCounts()[mopIndex], fetchLine->getIndex(), fetchLine->getSize() == mopIndex + 1, mopIndex == 0);
					instrIndex = 0;
					if (TRACE) std::cout << "; " << currentInstruction.getInfo().getName() << std::endl;
				}
				MacroOp& mop = currentInstruction.getInfo().getOp(instrIndex);
				if (fetchLine->getCounts()[mopIndex] != mop.getSize()) {
					if (TRACE) std::cout << "ERROR instruction " << mop.getName() << " doesn't fit into slot of size " << fetchLine->getCounts()[mopIndex] << std::endl;
				}
				
				if (TRACE) std::cout << mop.getName() << " ";
				int scheduleCycle = scheduleUop<false>(mop, portBusy, cycle, depCycle);
				mop.setCycle(scheduleCycle);
				if (scheduleCycle < 0) {
					if (TRACE) std::cout << "; Failed at cycle " << cycle << std::endl;
					return DBL_MIN;
				}

				if (instrIndex == currentInstruction.getInfo().getSrcOp()) {
					for (attempts = 0; attempts < MAX_ATTEMPTS && !currentInstruction.selectSource(scheduleCycle, registers, gen); ++attempts) {
						if (TRACE) std::cout << "; src STALL at cycle " << cycle << std::endl;
						++scheduleCycle;
						++cycle;
					}
					if (attempts == MAX_ATTEMPTS) { //throw instruction away
						//cycle = topCycle;
						instrIndex = currentInstruction.getInfo().getSize();
						if (TRACE) std::cout << "; THROW away " << currentInstruction.getInfo().getName() << std::endl;
						continue;
					}
					if (TRACE) std::cout << "; src = r" << currentInstruction.getSource() << std::endl;
				}
				if (instrIndex == currentInstruction.getInfo().getDstOp()) {
					for (attempts = 0; attempts < MAX_ATTEMPTS && !currentInstruction.selectDestination(scheduleCycle, registers, gen); ++attempts) {
						if (TRACE) std::cout << "; dst STALL at cycle " << cycle << std::endl;
						++scheduleCycle;
						++cycle;
					}
					if (attempts == MAX_ATTEMPTS) { //throw instruction away
						//cycle = topCycle;
						instrIndex = currentInstruction.getInfo().getSize();
						if (TRACE) std::cout << "; THROW away " << currentInstruction.getInfo().getName() << std::endl;
						continue;
					}
					if (TRACE) std::cout << "; dst = r" << currentInstruction.getDestination() << std::endl;
				}
				scheduleCycle = scheduleUop<true>(mop, portBusy, scheduleCycle, scheduleCycle);
				depCycle = scheduleCycle + mop.getLatency();
				if (instrIndex == currentInstruction.getInfo().getResultOp()) {
					int dst = currentInstruction.getDestination();
					RegisterInfo& ri = registers[dst];
					retireCycle = depCycle;
					ri.latency = retireCycle;
					ri.lastOpGroup = currentInstruction.getGroup();
					ri.lastOpPar = currentInstruction.getGroupPar();
					if (TRACE) std::cout << "; RETIRED at cycle " << retireCycle << std::endl;
				}
				codeSize += mop.getSize();
				mopIndex++;
				instrIndex++;
				macroOpCount++;
				if (scheduleCycle >= RANDOMX_LPROG_LATENCY) {
					portsSaturated = true;
				}
				cycle = topCycle;
				if (instrIndex >= currentInstruction.getInfo().getSize()) {
					currentInstruction.toInstr(prog(outIndex++));
					mulCount += isMul(currentInstruction.getType());
				}
			}
			++cycle;
		}

		if(INFO) std::cout << "; ALU port utilization:" << std::endl;
		if (INFO) std::cout << "; (* = in use, _ = idle)" << std::endl;

		int portCycles = 0;
		/*for (int i = 0; i < CYCLE_MAP_SIZE; ++i) {
			std::cout << "; " << std::setw(3) << i << " ";
			for (int j = 0; j < 3; ++j) {
				std::cout << (portBusy[i][j] ? '*' : '_');
				portCycles += !!portBusy[i][j];
			}
			std::cout << std::endl;
		}*/

		double ipc = (macroOpCount / (double)retireCycle);

		if (INFO) std::cout << "; code size " << codeSize << " bytes" << std::endl;
		if (INFO) std::cout << "; x86 macro-ops: " << macroOpCount << std::endl;
		if (INFO) std::cout << "; RandomX instructions: " << outIndex << std::endl;
		if (INFO) std::cout << "; Execution time: " << retireCycle << " cycles" << std::endl;
		if (INFO) std::cout << "; IPC = " << ipc << std::endl;
		if (INFO) std::cout << "; Port-cycles: " << portCycles << std::endl;
		if (INFO) std::cout << "; Multiplications: " << mulCount << std::endl;

		int asicLatency[8];
		memset(asicLatency, 0, sizeof(asicLatency));

		
		for (int i = 0; i < outIndex; ++i) {
			Instruction& instr = prog(i);
			int latDst = asicLatency[instr.dst] + 1;
			int latSrc = instr.dst != instr.src ? asicLatency[instr.src] + 1 : 0;
			asicLatency[instr.dst] = std::max(latDst, latSrc);
		}

		int asicLatencyFinal = 0;
		int addressReg = 0;
		for (int i = 0; i < 8; ++i) {
			if (asicLatency[i] > asicLatencyFinal) {
				asicLatencyFinal = asicLatency[i];
				addressReg = i;
			}
		}

		if (INFO) std::cout << "; ASIC latency: " << asicLatencyFinal << std::endl;

		if (INFO) {
			std::cout << "; ASIC latency:" << std::endl;
			for (int i = 0; i < 8; ++i) {
				std::cout << ";  r" << i << " = " << asicLatency[i] << std::endl;
			}
			if (INFO) std::cout << "; CPU latency:" << std::endl;
			for (int i = 0; i < 8; ++i) {
				std::cout << ";  r" << i << " = " << registers[i].latency << std::endl;
			}
		}

		prog.setSize(outIndex);
		prog.setAddressRegister(addressReg);
		return outIndex;
	}
}