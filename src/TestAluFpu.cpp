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

#include <iostream>
#include <iomanip>
#include <limits>
#include "instructions.hpp"
#include "Pcg32.hpp"
//#define DEBUG

using namespace RandomX;

typedef void(*VmOperation)(convertible_t&, convertible_t&, convertible_t&);

uint64_t rxRound(uint32_t mode, int64_t x, int64_t y, VmOperation op) {
	convertible_t a, b, c;
	a.u64 = mode;
	FPROUND(a, b, c);
#ifdef DEBUG
	a.f64 = convertToDouble(x);
	b.f64 = convertToDouble(y);
	std::cout << std::hex << (uint64_t)x << " -> " << a.u64 << std::endl;
	std::cout << std::hex << (uint64_t)y << " -> " << b.u64 << std::endl;
	std::cout << std::dec;
#endif
	a.i64 = x;
	b.i64 = y;
	op(a, b, c);
	return c.u64;
}

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#define RX_EXECUTE_U64(va, vb, INST) do { \
	a.u64 = va; \
	b.u64 = vb; \
	INST(a, b, c); \
	} while(false)

#define RX_EXECUTE_I64(va, vb, INST) do { \
	a.i64 = va; \
	b.i64 = vb; \
	INST(a, b, c); \
	} while(false)

TEST_CASE("Integer addition (64-bit)", "[ADD_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0xFFFFFFFF, 0x1, ADD_64);
	CHECK(c.u64 == 0x100000000);

	RX_EXECUTE_U64(0x8000000000000000, 0x8000000000000000, ADD_64);
	CHECK(c.u64 == 0x0);
}

TEST_CASE("Integer addition (32-bit)", "[ADD_32]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0xFFFFFFFF, 0x1, ADD_32);
	CHECK(c.u64 == 0);

	RX_EXECUTE_U64(0xFF00000000000001, 0x0000000100000001, ADD_32);
	CHECK(c.u64 == 2);
}

TEST_CASE("Integer subtraction (64-bit)", "[SUB_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(1, 0xFFFFFFFF, SUB_64);
	CHECK(c.u64 == 0xFFFFFFFF00000002);
}

TEST_CASE("Integer subtraction (32-bit)", "[SUB_32]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(1, 0xFFFFFFFF, SUB_32);
	CHECK(c.u64 == 2);
}

TEST_CASE("Unsigned multiplication (64-bit, low half)", "[MUL_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0xBC550E96BA88A72B, 0xF5391FA9F18D6273, MUL_64);
	CHECK(c.u64 == 0x28723424A9108E51);
}

TEST_CASE("Unsigned multiplication (64-bit, high half)", "[MULH_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0xBC550E96BA88A72B, 0xF5391FA9F18D6273, MULH_64);
	CHECK(c.u64 == 0xB4676D31D2B34883);
}

TEST_CASE("Unsigned multiplication (32-bit x 32-bit -> 64-bit)", "[MUL_32]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0xBC550E96BA88A72B, 0xF5391FA9F18D6273, MUL_32);
	CHECK(c.u64 == 0xB001AA5FA9108E51);
}

TEST_CASE("Signed multiplication (32-bit x 32-bit -> 64-bit)", "[IMUL_32]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0xBC550E96BA88A72B, 0xF5391FA9F18D6273, IMUL_32);
	CHECK(c.u64 == 0x03EBA0C1A9108E51);
}

TEST_CASE("Signed multiplication (64-bit, high half)", "[IMULH_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0xBC550E96BA88A72B, 0xF5391FA9F18D6273, IMULH_64);
	CHECK(c.u64 == 0x02D93EF1269D3EE5);
}

TEST_CASE("Unsigned division (64-bit / 32-bit -> 32-bit)", "[DIV_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(8774217225983458895, 3014068202, DIV_64);
	CHECK(c.u64 == 2911087818);

	RX_EXECUTE_U64(8774217225983458895, 0, DIV_64);
	CHECK(c.u64 == 8774217225983458895);

	RX_EXECUTE_U64(3014068202, 8774217225983458895, DIV_64);
	CHECK(c.u64 == 2);
}

TEST_CASE("Signed division (64-bit / 32-bit -> 32-bit)", "[IDIV_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(8774217225983458895, 3014068202, IDIV_64);
	CHECK(c.u64 == 0xFFFFFFFE67B4994E);

	RX_EXECUTE_U64(5, 0xFFFFFFFFFFFFFFFF, IDIV_64);
	CHECK(c.u64 == 0xFFFFFFFFFFFFFFFB);

	RX_EXECUTE_U64(8774217225983458895, 0, IDIV_64);
	CHECK(c.u64 == 8774217225983458895);

	RX_EXECUTE_U64(0x8000000000000000, 0xFFFFFFFFFFFFFFFF, IDIV_64);
	CHECK(c.u64 == 0x8000000000000000);

	RX_EXECUTE_U64(0x8000000000000000, 0x93D1FFFFFFFFFFFF, IDIV_64);
	CHECK(c.u64 == 0x8000000000000000);

	RX_EXECUTE_U64(0xFFFFFFFFB3A707EA, 8774217225983458895, IDIV_64);
	CHECK(c.u64 == 0xFFFFFFFFFFFFFFFF);
}

TEST_CASE("Bitwise AND (64-bit)", "[AND_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0xCCCCCCCCCCCCCCCC, 0xAAAAAAAAAAAAAAAA, AND_64);
	CHECK(c.u64 == 0x8888888888888888);
}

TEST_CASE("Bitwise AND (32-bit)", "[AND_32]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0xCCCCCCCCCCCCCCCC, 0xAAAAAAAAAAAAAAAA, AND_32);
	CHECK(c.u64 == 0x88888888);
}

TEST_CASE("Bitwise OR (64-bit)", "[OR_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0x4444444444444444, 0xAAAAAAAAAAAAAAAA, OR_64);
	CHECK(c.u64 == 0xEEEEEEEEEEEEEEEE);
}

TEST_CASE("Bitwise OR (32-bit)", "[OR_32]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0x4444444444444444, 0xAAAAAAAAAAAAAAAA, OR_32);
	CHECK(c.u64 == 0xEEEEEEEE);
}

TEST_CASE("Bitwise XOR (64-bit)", "[XOR_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0x8888888888888888, 0xAAAAAAAAAAAAAAAA, XOR_64);
	CHECK(c.u64 == 0x2222222222222222);
}

TEST_CASE("Bitwise XOR (32-bit)", "[XOR_32]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0x8888888888888888, 0xAAAAAAAAAAAAAAAA, XOR_32);
	CHECK(c.u64 == 0x22222222);
}

TEST_CASE("Logical left shift (64-bit)", "[SHL_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0x3, 52, SHL_64);
	CHECK(c.u64 == 0x30000000000000);

	RX_EXECUTE_U64(953360005391419562, 4569451684712230561, SHL_64);
	CHECK(c.u64 == 6978065200108797952);

	RX_EXECUTE_U64(0x8000000000000000, 1, SHL_64);
	CHECK(c.u64 == 0);
}

TEST_CASE("Logical right shift (64-bit)", "[SHR_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0x3, 52, SHR_64);
	CHECK(c.u64 == 0);

	RX_EXECUTE_U64(953360005391419562, 4569451684712230561, SHR_64);
	CHECK(c.u64 == 110985711);

	RX_EXECUTE_U64(0x8000000000000000, 1, SHR_64);
	CHECK(c.u64 == 0x4000000000000000);
}

TEST_CASE("Arithmetic right shift (64-bit)", "[SAR_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_I64(-9, 2, SAR_64);
	CHECK(c.i64 == -3);

	RX_EXECUTE_I64(INT64_MIN, 63, SAR_64);
	CHECK(c.i64 == -1);

	RX_EXECUTE_I64(INT64_MAX, 163768499474606398, SAR_64);
	CHECK(c.i64 == 1);
}

TEST_CASE("Circular left shift (64-bit)", "[ROL_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0x3, 52, ROL_64);
	CHECK(c.u64 == 0x30000000000000);

	RX_EXECUTE_U64(953360005391419562, 4569451684712230561, ROL_64);
	CHECK(c.u64 == 6978065200552740799);

	RX_EXECUTE_U64(0x8000000000000000, 1, ROL_64);
	CHECK(c.u64 == 1);
}

TEST_CASE("Circular right shift (64-bit)", "[ROR_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0x3, 52, ROR_64);
	CHECK(c.u64 == 12288);

	RX_EXECUTE_U64(953360005391419562, 4569451684712230561, ROR_64);
	CHECK(c.u64 == 0xD835C455069D81EF);

	RX_EXECUTE_U64(0x8000000000000000, 1, ROR_64);
	CHECK(c.u64 == 0x4000000000000000);
}

TEST_CASE("Denormal results are not produced", "[FTZ]") {
	FPINIT();
	convertible_t a, b, c;
	a.i64 = 2048;
	FPDIV(a, DBL_MAX, c);
#ifdef DEBUG
	std::cout << a.i64 << " / " << DBL_MAX << " = " << std::hex << c.u64 << std::endl;
#endif
	REQUIRE(std::fpclassify(c.f64) != FP_SUBNORMAL);
	b.f64 = c.f64;
	a.i64 = 0;
	FPSUB_64(a, b, c);
#ifdef DEBUG
	std::cout << a.i64 << " - " << b.f64 << " = " << std::hex << c.u64 << std::endl;
#endif
	CHECK(std::fpclassify(c.f64) != FP_SUBNORMAL);
}

TEST_CASE("NaN results are not produced", "[NAN]") {
	FPINIT();
	convertible_t a, c;
	a.i64 = 0;
	FPDIV(a, 0, c);
	CHECK(std::fpclassify(c.f64) != FP_NAN);
	FPMUL(a, std::numeric_limits<double>::infinity(), c);
	CHECK(std::fpclassify(c.f64) != FP_NAN);
}

volatile int64_t fpAdda = 7379480244170225589;
volatile int64_t fpAddb = -438072579179686797;
volatile int64_t fpSuba = 2939258788088626026;
volatile int64_t fpSubb = 4786131045320678734;
volatile int64_t fpMula1 = 8399833736388895639;
volatile int64_t fpMulb1 = 5671608020317594922;
volatile int64_t fpMula2 = -7094299423744805450;
volatile int64_t fpMulb2 = 4982086006202596504;
volatile int64_t fpDiva1 = 8399833736388895639;
volatile int64_t fpDivb1 = 5671608020317594922;
volatile int64_t fpDiva2 = -7434878587645025912;
volatile int64_t fpDivb2 = 5266243837734830806;
volatile int64_t fpSqrta = -7594301562963134542;

TEST_CASE("IEEE-754 compliance", "[FPU]") {
	FPINIT();
	convertible_t a, b, c;

	a.i64 = 2048;
	FPDIV(a, 0, c);
	CHECK(c.f64 == std::numeric_limits<double>::infinity());

	a.i64 = -2048;
	FPDIV(a, 0, c);
	CHECK(c.f64 == -std::numeric_limits<double>::infinity());

#ifdef DEBUG
	std::cout << "FPROUND" << std::endl;
#endif
	CHECK(rxRound(RoundToNearest, fpAdda, 0, &FPROUND) == 0x43d99a4b8bc531dcU);
	CHECK(rxRound(RoundDown, fpAdda, 0, &FPROUND) == 0x43d99a4b8bc531dcU);
	CHECK(rxRound(RoundUp, fpAdda, 0, &FPROUND) == 0x43d99a4b8bc531dcU);
	CHECK(rxRound(RoundToZero, fpAdda, 0, &FPROUND) == 0x43d99a4b8bc531dcU);

	CHECK(rxRound(RoundToNearest, fpSuba, 0, &FPROUND) == 0x43c4652c25bf7bdcU);
	CHECK(rxRound(RoundDown, fpSuba, 0, &FPROUND) == 0x43c4652c25bf7bdcU);
	CHECK(rxRound(RoundUp, fpSuba, 0, &FPROUND) == 0x43c4652c25bf7bdcU);
	CHECK(rxRound(RoundToZero, fpSuba, 0, &FPROUND) == 0x43c4652c25bf7bdcU);

#ifdef DEBUG
	std::cout << "FPADD" << std::endl;
#endif
	CHECK(rxRound(RoundToNearest, fpAdda, fpAddb, &FPADD_64) == 0xf9eba74f6c27d473U);
	CHECK(rxRound(RoundDown, fpAdda, fpAddb, &FPADD_64) == 0xf9eba74f6c27d473U);
	CHECK(rxRound(RoundUp, fpAdda, fpAddb, &FPADD_64) == 0xf9eba74f6c27d472U);
	CHECK(rxRound(RoundToZero, fpAdda, fpAddb, &FPADD_64) == 0xf9eba74f6c27d472U);

#ifdef DEBUG
	std::cout << "FPSUB" << std::endl;
#endif
	CHECK(rxRound(RoundToNearest, fpSuba, fpSubb, &FPSUB_64) == 0x43c4652bb6bc2c49U);
	CHECK(rxRound(RoundDown, fpSuba, fpSubb, &FPSUB_64) == 0x43c4652bb6bc2c48U);
	CHECK(rxRound(RoundUp, fpSuba, fpSubb, &FPSUB_64) == 0x43c4652bb6bc2c49U);
	CHECK(rxRound(RoundToZero, fpSuba, fpSubb, &FPSUB_64) == 0x43c4652bb6bc2c48U);

#ifdef DEBUG
	std::cout << "FPMUL" << std::endl;
#endif
	CHECK(rxRound(RoundToNearest, fpMula1, fpMulb1, &FPMUL_64) == 0x52a3abbb1677f3e9U);
	CHECK(rxRound(RoundDown, fpMula1, fpMulb1, &FPMUL_64) == 0x52a3abbb1677f3e8U);
	CHECK(rxRound(RoundUp, fpMula1, fpMulb1, &FPMUL_64) == 0x52a3abbb1677f3e9U);
	CHECK(rxRound(RoundToZero, fpMula1, fpMulb1, &FPMUL_64) == 0x52a3abbb1677f3e8U);

	CHECK(rxRound(RoundToNearest, fpMula2, fpMulb2, &FPMUL_64) == 0xc90ea6c25e29c583U);
	CHECK(rxRound(RoundDown, fpMula2, fpMulb2, &FPMUL_64) == 0xc90ea6c25e29c583U);
	CHECK(rxRound(RoundUp, fpMula2, fpMulb2, &FPMUL_64) == 0xc90ea6c25e29c582U);
	CHECK(rxRound(RoundToZero, fpMula2, fpMulb2, &FPMUL_64) == 0xc90ea6c25e29c582U);

#ifdef DEBUG
	std::cout << "FPDIV" << std::endl;
#endif
	CHECK(rxRound(RoundToNearest, fpDiva1, fpDivb1, &FPDIV_64) == 0x3515967d3015e81cU);
	CHECK(rxRound(RoundDown, fpDiva1, fpDivb1, &FPDIV_64) == 0x3515967d3015e81bU);
	CHECK(rxRound(RoundUp, fpDiva1, fpDivb1, &FPDIV_64) == 0x3515967d3015e81cU);
	CHECK(rxRound(RoundToZero, fpDiva1, fpDivb1, &FPDIV_64) == 0x3515967d3015e81bU);

	CHECK(rxRound(RoundToNearest, fpDiva2, fpDivb2, &FPDIV_64) == 0xbab33c30b92b8fccU);
	CHECK(rxRound(RoundDown, fpDiva2, fpDivb2, &FPDIV_64) == 0xbab33c30b92b8fccU);
	CHECK(rxRound(RoundUp, fpDiva2, fpDivb2, &FPDIV_64) == 0xbab33c30b92b8fcbU);
	CHECK(rxRound(RoundToZero, fpDiva2, fpDivb2, &FPDIV_64) == 0xbab33c30b92b8fcbU);

#ifdef DEBUG
	std::cout << "FPSQRT" << std::endl;
#endif
	CHECK(rxRound(RoundToNearest, fpSqrta, 0, &FPSQRT) == 0x41d304e3fcc31a2dU);
	CHECK(rxRound(RoundDown, fpSqrta, 0, &FPSQRT) == 0x41d304e3fcc31a2cU);
	CHECK(rxRound(RoundUp, fpSqrta, 0, &FPSQRT) == 0x41d304e3fcc31a2dU);
	CHECK(rxRound(RoundToZero, fpSqrta, 0, &FPSQRT) == 0x41d304e3fcc31a2cU);
}
