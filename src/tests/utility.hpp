/*
Copyright (c) 2018-2024, tevador <tevador@gmail.com>

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

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstdint>

constexpr char hexmap[] = "0123456789abcdef";
inline void outputHex(std::ostream& os, const char* data, int length) {
	for (int i = 0; i < length; ++i) {
		os << hexmap[(data[i] & 0xF0) >> 4];
		os << hexmap[data[i] & 0x0F];
	}
}

char parseNibble(char hex) {
	hex &= ~0x20;
	if (hex & 0x40) {
		hex -= 'A' - 10;
	}
	else {
		hex &= 0xf;
	}
	return hex;
}

void hex2bin(const char *in, int length, char *out) {
	for (int i = 0; i < length; i += 2) {
		char nibble1 = parseNibble(*in++);
		char nibble2 = parseNibble(*in++);
		*out++ = nibble1 << 4 | nibble2;
	}
}

constexpr bool stringsEqual(char const * a, char const * b) {
	return *a == *b && (*a == '\0' || stringsEqual(a + 1, b + 1));
}

template<size_t N>
bool equalsHex(const void* hash, const char (&hex)[N]) {
	char reference[N / 2];
	hex2bin(hex, N - 1, reference);
	return memcmp(hash, reference, sizeof(reference)) == 0;
}

inline void dump(const char* buffer, uint64_t count, const char* name) {
	std::ofstream fout(name, std::ios::out | std::ios::binary);
	fout.write(buffer, count);
	fout.close();
}

inline void readOption(const char* option, int argc, char** argv, bool& out) {
	for (int i = 0; i < argc; ++i) {
		if (strcmp(argv[i], option) == 0) {
			out = true;
			return;
		}
	}
	out = false;
}

inline void readIntOption(const char* option, int argc, char** argv, int& out, int defaultValue) {
	for (int i = 0; i < argc - 1; ++i) {
		if (strcmp(argv[i], option) == 0 && (out = atoi(argv[i + 1])) > 0) {
			return;
		}
	}
	out = defaultValue;
}

inline void readUInt64Option(const char* option, int argc, char** argv, uint64_t& out, uint64_t defaultValue) {
	for (int i = 0; i < argc - 1; ++i) {
		if (strcmp(argv[i], option) == 0 && (out = std::strtoull(argv[i + 1], NULL, 0)) > 0) {
			return;
		}
	}
	out = defaultValue;
}

inline void readFloatOption(const char* option, int argc, char** argv, double& out, double defaultValue) {
	for (int i = 0; i < argc - 1; ++i) {
		if (strcmp(argv[i], option) == 0 && (out = atof(argv[i + 1])) > 0) {
			return;
		}
	}
	out = defaultValue;
}

inline void readInt(int argc, char** argv, int& out, int defaultValue) {
	for (int i = 0; i < argc; ++i) {
		if (*argv[i] != '-' && (out = atoi(argv[i])) > 0) {
			return;
		}
	}
	out = defaultValue;
}
