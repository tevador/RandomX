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

#define WT_ADD_64 11
#define WT_ADD_32 2
#define WT_SUB_64 11
#define WT_SUB_32 2
#define WT_MUL_64 23
#define WT_MULH_64 10
#define WT_MUL_32 15
#define WT_IMUL_32 15
#define WT_IMULH_64 6
#define WT_DIV_64 1
#define WT_IDIV_64 1
#define WT_AND_64 4
#define WT_AND_32 2
#define WT_OR_64 4
#define WT_OR_32 2
#define WT_XOR_64 4
#define WT_XOR_32 2
#define WT_SHL_64 3
#define WT_SHR_64 3
#define WT_SAR_64 3
#define WT_ROL_64 6
#define WT_ROR_64 6
#define WT_FPADD 20
#define WT_FPSUB 20
#define WT_FPMUL 22
#define WT_FPDIV 8
#define WT_FPSQRT 6
#define WT_FPROUND 2
#define WT_CALL 20
#define WT_RET 22


constexpr int wtSum = WT_ADD_64 + WT_ADD_32 + WT_SUB_64 + WT_SUB_32 + \
WT_MUL_64 + WT_MULH_64 + WT_MUL_32 + WT_IMUL_32 + WT_IMULH_64 + \
WT_DIV_64 + WT_IDIV_64 + WT_AND_64 + WT_AND_32 + WT_OR_64 + \
WT_OR_32 + WT_XOR_64 + WT_XOR_32 + WT_SHL_64 + WT_SHR_64 + \
WT_SAR_64 + WT_ROL_64 + WT_ROR_64 + WT_FPADD + WT_FPSUB + WT_FPMUL \
+ WT_FPDIV + WT_FPSQRT + WT_FPROUND + WT_CALL + WT_RET;

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
