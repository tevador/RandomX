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
#define WT_IADD_R 10
#define WT_IADD_M 3
#define WT_IADD_RC 12
#define WT_ISUB_R 10
#define WT_ISUB_M 3
#define WT_IMUL_9C 12
#define WT_IMUL_R 24
#define WT_IMUL_M 8
#define WT_IMULH_R 6
#define WT_IMULH_M 2
#define WT_ISMULH_R 6
#define WT_ISMULH_M 2
#define WT_IDIV_C 4
#define WT_ISDIV_C 2
#define WT_INEG_R 4
#define WT_IXOR_R 15
#define WT_IXOR_M 5
#define WT_IROR_R 10
#define WT_IROL_R 10

//Common floating point
#define WT_FPSWAP_R 6

//Floating point group F
#define WT_FPADD_R 18
#define WT_FPADD_M 3
#define WT_FPSUB_R 18
#define WT_FPSUB_M 3
#define WT_FPNEG_R 5

//Floating point group E
#define WT_FPMUL_R 18
#define WT_FPMUL_M 3
#define WT_FPDIV_R 6
#define WT_FPDIV_M 1
#define WT_FPSQRT_R 6

//Control
#define WT_COND_R 15
#define WT_COND_M 5
#define WT_CFROUND 1

#define WT_NOP 0

constexpr int wtSum = WT_IADD_R + WT_IADD_M + WT_IADD_RC + WT_ISUB_R + \
WT_ISUB_M + WT_IMUL_9C + WT_IMUL_R + WT_IMUL_M + WT_IMULH_R + \
WT_IMULH_M + WT_ISMULH_R + WT_ISMULH_M + WT_IDIV_C + WT_ISDIV_C + \
WT_INEG_R + WT_IXOR_R + WT_IXOR_M + WT_IROR_R + WT_IROL_R + \
WT_FPSWAP_R + WT_FPADD_R + WT_FPADD_M + WT_FPSUB_R + WT_FPSUB_M + \
WT_FPNEG_R + WT_FPMUL_R + WT_FPMUL_M + WT_FPDIV_R + WT_FPDIV_M + \
WT_FPSQRT_R + WT_COND_R + WT_COND_M + WT_CFROUND + WT_NOP;

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
#define REP256(x) REP128(x) REP128(x)
#define REPNX(x,N) REP##N(x)
#define REPN(x,N) REPNX(x,N)
#define NUM(x) x
#define WT(x) NUM(WT_##x)
