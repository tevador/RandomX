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

#include "VirtualMachine.hpp"
#include "common.hpp"
#include "t1ha/t1ha.h"
#include "blake2/blake2.h"
#include <cstring>
#include <iomanip>

std::ostream& operator<<(std::ostream& os, const RandomX::RegisterFile& rf) {
	for (int i = 0; i < RandomX::RegistersCount; ++i)
		os << std::hex << "r" << i << " = " << rf.r[i].u64 << std::endl << std::dec;
	for (int i = 0; i < RandomX::RegistersCount; ++i)
		os << std::hex << "f" << i << " = " << rf.f[i].hi.u64 << " (" << rf.f[i].hi.f64 << ")" << std::endl
		<< "   = " << rf.f[i].lo.u64 << " (" << rf.f[i].lo.f64 << ")" << std::endl << std::dec;
	return os;
}

namespace RandomX {

	VirtualMachine::VirtualMachine() {
		mem.ds.dataset = nullptr;
	}

	void VirtualMachine::getResult(void* out) {
		constexpr size_t smallStateLength = sizeof(RegisterFile) / sizeof(uint64_t) + 2;
		uint64_t smallState[smallStateLength];
		memcpy(smallState, &reg, sizeof(RegisterFile));
		smallState[smallStateLength - 1] = t1ha2_atonce128(&smallState[smallStateLength - 2], scratchpad, ScratchpadSize, reg.r[0].u64);
		blake2b(out, ResultSize, smallState, sizeof(smallState), nullptr, 0);
	}
}