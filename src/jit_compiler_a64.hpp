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

#pragma once

#include <cstdint>
#include <vector>
#include <stdexcept>
#include "common.hpp"

namespace randomx {

	class Program;
	class ProgramConfiguration;
	class SuperscalarProgram;

	class JitCompilerA64 {
	public:
		JitCompilerA64() {
			throw std::runtime_error("ARM64 JIT compiler is not implemented yet.");
		}
		void generateProgram(Program&, ProgramConfiguration&) {

		}
		void generateProgramLight(Program&, ProgramConfiguration&, uint32_t) {
			
		}
		template<size_t N>
		void generateSuperscalarHash(SuperscalarProgram(&programs)[N], std::vector<uint64_t> &) {

		}
		void generateDatasetInitCode() {

		}
		ProgramFunc* getProgramFunc() {
			return nullptr;
		}
		DatasetInitFunc* getDatasetInitFunc() {
			return nullptr;
		}
		uint8_t* getCode() {
			return nullptr;
		}
		size_t getCodeSize() {
			return 0;
		}
	};
}