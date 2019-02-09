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

#include "Program.hpp"
#include "hashAes1Rx4.hpp"

namespace RandomX {
	void Program::print(std::ostream& os) const {
		for (int i = 0; i < RandomX::ProgramLength; ++i) {
			auto instr = programBuffer[i];
			os << instr;
		}
	}
}
