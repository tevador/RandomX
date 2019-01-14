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

#include "common.hpp"
#include "InterpretedVirtualMachine.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>

namespace RandomX {

	class Mul9Transform : public ITransform {
	public:
		Mul9Transform(int32_t cc) : c(cc) {
			std::ostringstream oss;
			oss << "mul9_" << std::hex << (cc & 255);
			name = oss.str();
		}
		int32_t apply(int32_t x) const override {
			return 9 * x + c;
		}
		const char* getName() const override {
			return name.c_str();
		}
		std::ostream& printAsm(std::ostream& os) const override {
			os << "lea ecx, [rcx+rcx*8" << std::showpos << c << "]" << std::noshowpos << std::endl;
			return os;
		}
		std::ostream& printCxx(std::ostream& os) const override {
			os << "static const Mul9Transform " << name << "(" << c << ");" << std::endl;
			return os;
		}
	private:
		int32_t c;
		std::string name;
	};

	class AddTransform : public ITransform {
	public:
		AddTransform(int32_t cc) : c(cc) {
			std::ostringstream oss;
			oss << "add_" << std::hex << (cc & 255);
			name = oss.str();
		}
		int32_t apply(int32_t x) const override {
			return x + c;
		}
		const char* getName() const override {
			return name.c_str();
		}
		std::ostream& printAsm(std::ostream& os) const override {
			os << "db 64" << std::endl;
			os << "add ecx, " << c << std::endl;
			return os;
		}
		std::ostream& printCxx(std::ostream& os) const override {
			os << "static const AddTransform " << name << "(" << c << ");" << std::endl;
			return os;
		}
	private:
		int32_t c;
		std::string name;
	};

	class XorTransform : public ITransform {
	public:
		XorTransform(int32_t cc) : c(cc) {
			std::ostringstream oss;
			oss << "xor_" << std::hex << (cc & 255);
			name = oss.str();
		}
		int32_t apply(int32_t x) const override {
			return x ^ c;
		}
		const char* getName() const override {
			return name.c_str();
		}
		std::ostream& printAsm(std::ostream& os) const override {
			os << "db 64" << std::endl;
			os << "xor ecx, " << c << std::endl;
			return os;
		}
		std::ostream& printCxx(std::ostream& os) const override {
			os << "static const XorTransform " << name << "(" << c << ");" << std::endl;
			return os;
		}
	private:
		int32_t c;
		std::string name;
	};

	static const Mul9Transform mul9_6d(109);
	static const XorTransform xor_60(96);
	static const Mul9Transform mul9_ed(-19);
	static const AddTransform add_9e(-98);
	static const AddTransform add_eb(-21);
	static const XorTransform xor_b0(-80);
	static const Mul9Transform mul9_a4(-92);
	static const AddTransform add_71(113);
	static const Mul9Transform mul9_64(100);
	static const AddTransform add_d9(-39);
	static const XorTransform xor_78(120);
	static const Mul9Transform mul9_89(-119);
	static const AddTransform add_8f(-113);
	static const AddTransform add_6f(111);
	static const XorTransform xor_68(104);
	static const Mul9Transform mul9_ad(-83);
	static const Mul9Transform mul9_7f(127);
	static const XorTransform xor_90(-112);
	static const AddTransform add_59(89);
	static const AddTransform add_e0(-32);
	static const AddTransform add_68(104);
	static const XorTransform xor_88(-120);
	static const XorTransform xor_18(24);
	static const Mul9Transform mul9_9(9);
	static const AddTransform add_e1(-31);
	static const XorTransform xor_f0(-16);
	static const AddTransform add_44(68);
	static const Mul9Transform mul9_92(-110);
	static const XorTransform xor_40(64);
	static const XorTransform xor_d8(-40);
	static const XorTransform xor_f8(-8);
	static const AddTransform add_f6(-10);
	static const XorTransform xor_e0(-32);
	static const AddTransform add_e(14);
	static const Mul9Transform mul9_d2(-46);
	static const XorTransform xor_98(-104);
	static const Mul9Transform mul9_24(36);
	static const AddTransform add_64(100);
	static const Mul9Transform mul9_bf(-65);
	static const Mul9Transform mul9_1b(27);
	static const Mul9Transform mul9_5b(91);
	static const AddTransform add_9b(-101);
	static const AddTransform add_a2(-94);
	static const Mul9Transform mul9_f6(-10);
	static const XorTransform xor_50(80);
	static const AddTransform add_94(-108);
	static const AddTransform add_c6(-58);
	static const XorTransform xor_30(48);
	static const Mul9Transform mul9_49(73);
	static const XorTransform xor_d0(-48);
	static const XorTransform xor_20(32);
	static const XorTransform xor_a0(-96);
	static const AddTransform add_76(118);
	static const AddTransform add_5b(91);
	static const Mul9Transform mul9_12(18);
	static const AddTransform add_f5(-11);
	static const Mul9Transform mul9_3f(63);
	static const AddTransform add_72(114);
	static const Mul9Transform mul9_2d(45);
	static const AddTransform add_bd(-67);
	static const AddTransform add_35(53);
	static const Mul9Transform mul9_9b(-101);
	static const Mul9Transform mul9_ff(-1);
	static const XorTransform xor_10(16);
	static const Mul9Transform mul9_db(-37);
	static const Mul9Transform mul9_e4(-28);
	static const Mul9Transform mul9_c9(-55);
	static const XorTransform xor_a8(-88);
	static const XorTransform xor_b8(-72);
	static const AddTransform add_24(36);
	static const XorTransform xor_c8(-56);
	static const AddTransform add_74(116);
	static const XorTransform xor_58(88);
	static const XorTransform xor_80(-128);
	static const AddTransform add_32(50);
	static const AddTransform add_69(105);
	static const AddTransform add_db(-37);
	static const XorTransform xor_70(112);
	static const XorTransform xor_8(8);
	static const XorTransform xor_e8(-24);
	static const Mul9Transform mul9_76(118);
	static const XorTransform xor_48(72);
	static const XorTransform xor_c0(-64);
	static const AddTransform add_28(40);
	static const Mul9Transform mul9_b6(-74);
	static const Mul9Transform mul9_52(82);
	static const Mul9Transform mul9_36(54);
	static const XorTransform xor_38(56);
	static const XorTransform xor_28(40);
	static const AddTransform add_57(87);

	const ITransform* InterpretedVirtualMachine::addressTransformations[TransformationCount] = {
		(ITransform*)&mul9_6d,
		(ITransform*)&xor_60,
		(ITransform*)&mul9_ed,
		(ITransform*)&add_9e,
		(ITransform*)&add_eb,
		(ITransform*)&xor_b0,
		(ITransform*)&mul9_a4,
		(ITransform*)&add_71,
		(ITransform*)&mul9_64,
		(ITransform*)&add_d9,
		(ITransform*)&xor_78,
		(ITransform*)&mul9_89,
		(ITransform*)&add_8f,
		(ITransform*)&add_6f,
		(ITransform*)&xor_68,
		(ITransform*)&mul9_ad,
		(ITransform*)&mul9_7f,
		(ITransform*)&xor_90,
		(ITransform*)&add_59,
		(ITransform*)&add_e0,
		(ITransform*)&add_68,
		(ITransform*)&xor_88,
		(ITransform*)&xor_18,
		(ITransform*)&mul9_9,
		(ITransform*)&add_e1,
		(ITransform*)&xor_f0,
		(ITransform*)&add_44,
		(ITransform*)&mul9_92,
		(ITransform*)&xor_40,
		(ITransform*)&xor_d8,
		(ITransform*)&xor_f8,
		(ITransform*)&add_f6,
		(ITransform*)&xor_e0,
		(ITransform*)&add_e,
		(ITransform*)&mul9_d2,
		(ITransform*)&xor_98,
		(ITransform*)&mul9_24,
		(ITransform*)&add_64,
		(ITransform*)&mul9_bf,
		(ITransform*)&mul9_1b,
		(ITransform*)&mul9_5b,
		(ITransform*)&add_9b,
		(ITransform*)&add_a2,
		(ITransform*)&mul9_f6,
		(ITransform*)&xor_50,
		(ITransform*)&add_94,
		(ITransform*)&add_c6,
		(ITransform*)&xor_30,
		(ITransform*)&mul9_49,
		(ITransform*)&xor_d0,
		(ITransform*)&xor_20,
		(ITransform*)&xor_a0,
		(ITransform*)&add_76,
		(ITransform*)&add_5b,
		(ITransform*)&mul9_12,
		(ITransform*)&add_f5,
		(ITransform*)&mul9_3f,
		(ITransform*)&add_72,
		(ITransform*)&mul9_2d,
		(ITransform*)&add_bd,
		(ITransform*)&add_35,
		(ITransform*)&mul9_9b,
		(ITransform*)&mul9_ff,
		(ITransform*)&xor_10,
		(ITransform*)&mul9_db,
		(ITransform*)&mul9_e4,
		(ITransform*)&mul9_c9,
		(ITransform*)&xor_a8,
		(ITransform*)&xor_b8,
		(ITransform*)&add_24,
		(ITransform*)&xor_c8,
		(ITransform*)&add_74,
		(ITransform*)&xor_58,
		(ITransform*)&xor_80,
		(ITransform*)&add_32,
		(ITransform*)&add_69,
		(ITransform*)&add_db,
		(ITransform*)&xor_70,
		(ITransform*)&xor_8,
		(ITransform*)&xor_e8,
		(ITransform*)&mul9_76,
		(ITransform*)&xor_48,
		(ITransform*)&xor_c0,
		(ITransform*)&add_28,
		(ITransform*)&mul9_b6,
		(ITransform*)&mul9_52,
		(ITransform*)&mul9_36,
		(ITransform*)&xor_38,
		(ITransform*)&xor_28,
		(ITransform*)&add_57,
	};
}