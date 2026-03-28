/*
Copyright (c) 2019, tevador <tevador@gmail.com>

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

#include "cpu.hpp"
#include <cstring>

#if defined(_M_X64) || defined(__x86_64__)
	#define HAVE_CPUID
	#if defined(_MSC_VER)
		#include <intrin.h>
		#define cpuid(info, x) __cpuidex(info, x, 0)
	#else //GCC
		#include <cpuid.h>
		void cpuid(int info[4], int InfoType) {
			__cpuid_count(InfoType, 0, info[0], info[1], info[2], info[3]);
		}
	#endif
#endif

#if defined(HAVE_HWCAP)
	#include <sys/auxv.h>
	#include <asm/hwcap.h>
#endif

#if defined(__ppc64__) || defined(__PPC64__) || defined(__ppc64le__) || defined(__PPC64LE__)
	#include <sys/auxv.h>
	#include <asm/cputable.h>
#endif

#ifdef __riscv
#include <signal.h>
#include <setjmp.h>
#include <cstdint>

extern "C" uint64_t rv64_test_vector();
extern "C" uint64_t rv64_test_vector_aes();

static sigjmp_buf jump_buffer;
static void sigill_handler(int) { siglongjmp(jump_buffer, 1); }

void hashAes1Rx4_zvkned(const void *input, size_t inputSize, void *hash);
#endif

namespace randomx {

	Cpu::Cpu()
	{
#ifdef HAVE_CPUID
		int info[4];
		cpuid(info, 0);
		int nIds = info[0];
		if (nIds >= 0x00000001) {
			cpuid(info, 0x00000001);
			ssse3_ = (info[2] & (1 << 9)) != 0;
			aes_ = (info[2] & (1 << 25)) != 0;
		}
		if (nIds >= 0x00000007) {
			cpuid(info, 0x00000007);
			avx2_ = (info[1] & (1 << 5)) != 0;
		}
#elif defined(__aarch64__)
	#if defined(HWCAP_AES)
		long hwcaps = getauxval(AT_HWCAP);
		aes_ = (hwcaps & HWCAP_AES) != 0;
	#elif defined(__APPLE__)
		aes_ = true;
	#endif
#elif defined(__riscv)
		struct sigaction new_action, old_action;

		new_action.sa_handler = sigill_handler;
		sigemptyset(&new_action.sa_mask);
		new_action.sa_flags = 0;

		if (sigaction(SIGILL, &new_action, &old_action) == 0) {
			if (sigsetjmp(jump_buffer, 1) == 0) {
				rvv_length = static_cast<int>(rv64_test_vector());
				// If execution gets here, vector instructions executed successfully
				rvv_ = true;
			}

			if (sigsetjmp(jump_buffer, 1) == 0) {
				if (rv64_test_vector_aes() == 0) {
					// If execution gets here, vector AES instructions executed successfully
					// Now need to check that they actually do what they're supposed to do

					uint64_t input[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
					uint64_t hash[8] = {};

					static constexpr uint64_t ref_hash[8] = {
						0x195268637f56cab0ull, 0xdf7d7d3553e9e1d1ull, 0x3067fb6e5efcbee2ull, 0xfab778b414feaf77ull,
						0x99905a5000820817ull, 0xae359bff2379ff97ull, 0x0d87373e6505c4c3ull, 0xf3f5cffd57f2dd62ull
					};

					hashAes1Rx4_zvkned(input, sizeof(input), hash);

					aes_ = (memcmp(hash, ref_hash, sizeof(hash)) == 0);
				}
			}

			sigaction(SIGILL, &old_action, nullptr);
		}
#elif defined(__ppc64__) || defined(__PPC64__) || defined(__ppc64le__) || defined(__PPC64LE__)
		unsigned long hwcaps2 = getauxval(AT_HWCAP2);
		aes_ = (hwcaps2 & PPC_FEATURE2_VEC_CRYPTO) != 0;
#endif
	}

	const Cpu cpu;
}
