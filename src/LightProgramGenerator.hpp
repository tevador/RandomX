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

#include "Program.hpp"

namespace RandomX {

	class Blake2Generator {
	public:
		Blake2Generator(const void* seed, int nonce);
		uint8_t getByte();
		uint32_t getInt32();
	private:
		uint8_t data[64];
		size_t dataIndex;

		void checkData(const size_t);
	};

	double generateLightProg2(LightProgram& prog, Blake2Generator& gen);
}