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

#define REP0(x)
#define REP1(x) x,
#define REP2(x) REP1(x) x,
#define REP3(x) REP2(x) x,
#define REP4(x) REP3(x) x,
#define REP5(x) REP4(x) x,
#define REP6(x) REP5(x) x,
#define REP7(x) REP6(x) x,
#define REP8(x) REP7(x) x,
#define REP9(x) REP8(x) x,
#define REP10(x) REP9(x) x,
#define REP11(x) REP10(x) x,
#define REP12(x) REP11(x) x,
#define REP13(x) REP12(x) x,
#define REP14(x) REP13(x) x,
#define REP15(x) REP14(x) x,
#define REP16(x) REP15(x) x,
#define REP17(x) REP16(x) x,
#define REP18(x) REP17(x) x,
#define REP19(x) REP18(x) x,
#define REP20(x) REP19(x) x,
#define REP21(x) REP20(x) x,
#define REP22(x) REP21(x) x,
#define REP23(x) REP22(x) x,
#define REP24(x) REP23(x) x,
#define REP25(x) REP24(x) x,
#define REP26(x) REP25(x) x,
#define REP27(x) REP26(x) x,
#define REP28(x) REP27(x) x,
#define REP29(x) REP28(x) x,
#define REP30(x) REP29(x) x,
#define REP31(x) REP30(x) x,
#define REP32(x) REP31(x) x,
#define REP33(x) REP32(x) x,
#define REP40(x) REP32(x) REP8(x)
#define REP64(x) REP32(x) REP32(x)
#define REP128(x) REP32(x) REP32(x) REP32(x) REP32(x)
#define REP232(x) REP128(x) REP40(x) REP40(x) REP24(x)
#define REP256(x) REP128(x) REP128(x)
#define REPNX(x,N) REP##N(x)
#define REPN(x,N) REPNX(x,N)
#define NUM(x) x
#define WT(x) NUM(RANDOMX_FREQ_##x)
