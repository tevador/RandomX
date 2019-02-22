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

#pragma once

//Integer
#define WT_IADD_R 12
#define WT_IADD_M 7
#define WT_IADD_RC 16
#define WT_ISUB_R 12
#define WT_ISUB_M 7
#define WT_IMUL_9C 9
#define WT_IMUL_R 16
#define WT_IMUL_M 4
#define WT_IMULH_R 4
#define WT_IMULH_M 1
#define WT_ISMULH_R 4
#define WT_ISMULH_M 1
#define WT_IMUL_RCP 8
#define WT_ISDIV_C 0
#define WT_INEG_R 2
#define WT_IXOR_R 16
#define WT_IXOR_M 4
#define WT_IROR_R 10
#define WT_IROL_R 0
#define WT_ISWAP_R 4

//Common floating point
#define WT_FSWAP_R 8

//Floating point group F
#define WT_FADD_R 20
#define WT_FADD_M 5
#define WT_FSUB_R 20
#define WT_FSUB_M 5
#define WT_FSCAL_R 6

//Floating point group E
#define WT_FMUL_R 20
#define WT_FMUL_M 0
#define WT_FDIV_R 0
#define WT_FDIV_M 4
#define WT_FSQRT_R 6

//Control
#define WT_COND_R 7
#define WT_COND_M 1
#define WT_CFROUND 1

//Store
#define WT_ISTORE 16
#define WT_FSTORE 0

#define WT_NOP 0

constexpr int wtSum = WT_IADD_R + WT_IADD_M + WT_IADD_RC + WT_ISUB_R + \
WT_ISUB_M + WT_IMUL_9C + WT_IMUL_R + WT_IMUL_M + WT_IMULH_R + \
WT_IMULH_M + WT_ISMULH_R + WT_ISMULH_M + WT_IMUL_RCP + WT_ISDIV_C + \
WT_INEG_R + WT_IXOR_R + WT_IXOR_M + WT_IROR_R + WT_IROL_R + \
WT_ISWAP_R + WT_FSWAP_R + WT_FADD_R + WT_FADD_M + WT_FSUB_R + WT_FSUB_M + \
WT_FSCAL_R + WT_FMUL_R + WT_FMUL_M + WT_FDIV_R + WT_FDIV_M + \
WT_FSQRT_R + WT_COND_R + WT_COND_M + WT_CFROUND + WT_ISTORE + WT_FSTORE + WT_NOP;

static_assert(wtSum == 256,
	"Sum of instruction weights must be 256");

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
#define REP128(x) REP32(x) REP32(x) REP32(x) REP32(x)
#define REP232(x) REP128(x) REP40(x) REP40(x) REP24(x)
#define REP256(x) REP128(x) REP128(x)
#define REPNX(x,N) REP##N(x)
#define REPN(x,N) REPNX(x,N)
#define NUM(x) x
#define WT(x) NUM(WT_##x)

#define REPCASE0(x)
#define REPCASE1(x) case __COUNTER__:
#define REPCASE2(x) REPCASE1(x) case __COUNTER__:
#define REPCASE3(x) REPCASE2(x) case __COUNTER__:
#define REPCASE4(x) REPCASE3(x) case __COUNTER__:
#define REPCASE5(x) REPCASE4(x) case __COUNTER__:
#define REPCASE6(x) REPCASE5(x) case __COUNTER__:
#define REPCASE7(x) REPCASE6(x) case __COUNTER__:
#define REPCASE8(x) REPCASE7(x) case __COUNTER__:
#define REPCASE9(x) REPCASE8(x) case __COUNTER__:
#define REPCASE10(x) REPCASE9(x) case __COUNTER__:
#define REPCASE11(x) REPCASE10(x) case __COUNTER__:
#define REPCASE12(x) REPCASE11(x) case __COUNTER__:
#define REPCASE13(x) REPCASE12(x) case __COUNTER__:
#define REPCASE14(x) REPCASE13(x) case __COUNTER__:
#define REPCASE15(x) REPCASE14(x) case __COUNTER__:
#define REPCASE16(x) REPCASE15(x) case __COUNTER__:
#define REPCASE17(x) REPCASE16(x) case __COUNTER__:
#define REPCASE18(x) REPCASE17(x) case __COUNTER__:
#define REPCASE19(x) REPCASE18(x) case __COUNTER__:
#define REPCASE20(x) REPCASE19(x) case __COUNTER__:
#define REPCASE21(x) REPCASE20(x) case __COUNTER__:
#define REPCASE22(x) REPCASE21(x) case __COUNTER__:
#define REPCASE23(x) REPCASE22(x) case __COUNTER__:
#define REPCASE24(x) REPCASE23(x) case __COUNTER__:
#define REPCASE25(x) REPCASE24(x) case __COUNTER__:
#define REPCASE26(x) REPCASE25(x) case __COUNTER__:
#define REPCASE27(x) REPCASE26(x) case __COUNTER__:
#define REPCASE28(x) REPCASE27(x) case __COUNTER__:
#define REPCASE29(x) REPCASE28(x) case __COUNTER__:
#define REPCASE30(x) REPCASE29(x) case __COUNTER__:
#define REPCASE31(x) REPCASE30(x) case __COUNTER__:
#define REPCASE32(x) REPCASE31(x) case __COUNTER__:
#define REPCASENX(x,N) REPCASE##N(x)
#define REPCASEN(x,N) REPCASENX(x,N)
#define CASE_REP(x) REPCASEN(x, WT(x))