//RandomX ALU + FPU test
//https://github.com/tevador/RandomX
//License: GPL v3

#include <iostream>
#include <iomanip>
#include <limits>
#include "Instructions.h"

using namespace RandomX;

typedef void(*VmOperation)(convertible_t&, convertible_t&, convertible_t&);

double rxRound(uint32_t mode, int64_t x, int64_t y, VmOperation op) {
	convertible_t a, b, c;
	a.u64 = mode;
	FROUND(a, b, c);
	a.i64 = x;
	b.i64 = y;
	op(a, b, c);
	return c.f64;
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
	REQUIRE(c.u64 == 0x100000000);

	RX_EXECUTE_U64(0x8000000000000000, 0x8000000000000000, ADD_64);
	REQUIRE(c.u64 == 0x0);
}

TEST_CASE("Integer addition (32-bit)", "[ADD_32]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0xFFFFFFFF, 0x1, ADD_32);
	REQUIRE(c.u64 == 0);

	RX_EXECUTE_U64(0xFF00000000000001, 0x0000000100000001, ADD_32);
	REQUIRE(c.u64 == 2);
}

TEST_CASE("Integer subtraction (64-bit)", "[SUB_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(1, 0xFFFFFFFF, SUB_64);
	REQUIRE(c.u64 == 0xFFFFFFFF00000002);
}

TEST_CASE("Integer subtraction (32-bit)", "[SUB_32]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(1, 0xFFFFFFFF, SUB_32);
	REQUIRE(c.u64 == 2);
}

TEST_CASE("Unsigned multiplication (64-bit, low half)", "[MUL_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0xBC550E96BA88A72B, 0xF5391FA9F18D6273, MUL_64);
	REQUIRE(c.u64 == 0x28723424A9108E51);
}

TEST_CASE("Unsigned multiplication (64-bit, high half)", "[MULH_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0xBC550E96BA88A72B, 0xF5391FA9F18D6273, MULH_64);
	REQUIRE(c.u64 == 0xB4676D31D2B34883);
}

TEST_CASE("Unsigned multiplication (32-bit x 32-bit -> 64-bit)", "[MUL_32]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0xBC550E96BA88A72B, 0xF5391FA9F18D6273, MUL_32);
	REQUIRE(c.u64 == 0xB001AA5FA9108E51);
}

TEST_CASE("Signed multiplication (32-bit x 32-bit -> 64-bit)", "[IMUL_32]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0xBC550E96BA88A72B, 0xF5391FA9F18D6273, IMUL_32);
	REQUIRE(c.u64 == 0x03EBA0C1A9108E51);
}

TEST_CASE("Signed multiplication (64-bit, high half)", "[IMULH_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0xBC550E96BA88A72B, 0xF5391FA9F18D6273, IMULH_64);
	REQUIRE(c.u64 == 0x02D93EF1269D3EE5);
}

TEST_CASE("Unsigned division (64-bit / 32-bit -> 32-bit)", "[DIV_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(8774217225983458895, 3014068202, DIV_64);
	REQUIRE(c.u64 == 2911087818);

	RX_EXECUTE_U64(8774217225983458895, 0, DIV_64);
	REQUIRE(c.u64 == 8774217225983458895);

	RX_EXECUTE_U64(3014068202, 8774217225983458895, DIV_64);
	REQUIRE(c.u64 == 2);
}

TEST_CASE("Signed division (64-bit / 32-bit -> 32-bit)", "[IDIV_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(8774217225983458895, 3014068202, IDIV_64);
	REQUIRE(c.u64 == 0xFFFFFFFE67B4994E);

	RX_EXECUTE_U64(8774217225983458895, 0, IDIV_64);
	REQUIRE(c.u64 == 8774217225983458895);

	RX_EXECUTE_U64(0x8000000000000000, 0xFFFFFFFFFFFFFFFF, IDIV_64);
	REQUIRE(c.u64 == 0x8000000000000000);

	RX_EXECUTE_U64(0xFFFFFFFFB3A707EA, 8774217225983458895, IDIV_64);
	REQUIRE(c.u64 == 0xFFFFFFFFFFFFFFFF);
}

TEST_CASE("Bitwise AND (64-bit)", "[AND_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0xCCCCCCCCCCCCCCCC, 0xAAAAAAAAAAAAAAAA, AND_64);
	REQUIRE(c.u64 == 0x8888888888888888);
}

TEST_CASE("Bitwise AND (32-bit)", "[AND_32]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0xCCCCCCCCCCCCCCCC, 0xAAAAAAAAAAAAAAAA, AND_32);
	REQUIRE(c.u64 == 0x88888888);
}

TEST_CASE("Bitwise OR (64-bit)", "[OR_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0x4444444444444444, 0xAAAAAAAAAAAAAAAA, OR_64);
	REQUIRE(c.u64 == 0xEEEEEEEEEEEEEEEE);
}

TEST_CASE("Bitwise OR (32-bit)", "[OR_32]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0x4444444444444444, 0xAAAAAAAAAAAAAAAA, OR_32);
	REQUIRE(c.u64 == 0xEEEEEEEE);
}

TEST_CASE("Bitwise XOR (64-bit)", "[XOR_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0x8888888888888888, 0xAAAAAAAAAAAAAAAA, XOR_64);
	REQUIRE(c.u64 == 0x2222222222222222);
}

TEST_CASE("Bitwise XOR (32-bit)", "[XOR_32]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0x8888888888888888, 0xAAAAAAAAAAAAAAAA, XOR_32);
	REQUIRE(c.u64 == 0x22222222);
}

TEST_CASE("Logical left shift (64-bit)", "[SHL_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0x3, 52, SHL_64);
	REQUIRE(c.u64 == 0x30000000000000);

	RX_EXECUTE_U64(953360005391419562, 4569451684712230561, SHL_64);
	REQUIRE(c.u64 == 6978065200108797952);

	RX_EXECUTE_U64(0x8000000000000000, 1, SHL_64);
	REQUIRE(c.u64 == 0);
}

TEST_CASE("Logical right shift (64-bit)", "[SHR_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0x3, 52, SHR_64);
	REQUIRE(c.u64 == 0);

	RX_EXECUTE_U64(953360005391419562, 4569451684712230561, SHR_64);
	REQUIRE(c.u64 == 110985711);

	RX_EXECUTE_U64(0x8000000000000000, 1, SHR_64);
	REQUIRE(c.u64 == 0x4000000000000000);
}

TEST_CASE("Arithmetic right shift (64-bit)", "[SAR_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_I64(-9, 2, SAR_64);
	REQUIRE(c.i64 == -3);

	RX_EXECUTE_I64(INT64_MIN, 63, SAR_64);
	REQUIRE(c.i64 == -1);

	RX_EXECUTE_I64(INT64_MAX, 163768499474606398, SAR_64);
	REQUIRE(c.i64 == 1);
}

TEST_CASE("Circular left shift (64-bit)", "[ROL_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0x3, 52, ROL_64);
	REQUIRE(c.u64 == 0x30000000000000);

	RX_EXECUTE_U64(953360005391419562, 4569451684712230561, ROL_64);
	REQUIRE(c.u64 == 6978065200552740799);

	RX_EXECUTE_U64(0x8000000000000000, 1, ROL_64);
	REQUIRE(c.u64 == 1);
}

TEST_CASE("Circular right shift (64-bit)", "[ROR_64]") {
	convertible_t a, b, c;

	RX_EXECUTE_U64(0x3, 52, ROR_64);
	REQUIRE(c.u64 == 12288);

	RX_EXECUTE_U64(953360005391419562, 4569451684712230561, ROR_64);
	REQUIRE(c.u64 == 0xD835C455069D81EF);

	RX_EXECUTE_U64(0x8000000000000000, 1, ROR_64);
	REQUIRE(c.u64 == 0x4000000000000000);
}

TEST_CASE("Denormal numbers are flushed to zero", "[FTZ]") {
	FPINIT();
	convertible_t a, c;
	a.i64 = 1;
	FDIV_64(a, std::numeric_limits<double>::max(), c);
	REQUIRE(c.f64 == 0.0);
}

TEST_CASE("IEEE-754 compliance", "[FPU]") {
	FPINIT();
	convertible_t a, c;

	a.i64 = 1;
	FDIV_64(a, 0, c);
	REQUIRE(c.f64 == std::numeric_limits<double>::infinity());

	a.i64 = -1;
	FDIV_64(a, 0, c);
	REQUIRE(c.f64 == -std::numeric_limits<double>::infinity());

	REQUIRE(rxRound(RoundToNearest, 33073499373184121, -37713516328519941, &FADD) == -4640016955335824.0);
	REQUIRE(rxRound(RoundDown, 33073499373184121, -37713516328519941, &FADD) == -4640016955335824.0);
	REQUIRE(rxRound(RoundUp, 33073499373184121, -37713516328519941, &FADD) == -4640016955335812.0);
	REQUIRE(rxRound(RoundToZero, 33073499373184121, -37713516328519941, &FADD) == -4640016955335816.0);

	REQUIRE(rxRound(RoundToNearest, -8570200862721897289, -1111111111111111119, &FSUB) == -7.4590897516107858e+18);
	REQUIRE(rxRound(RoundDown, -8570200862721897289, -1111111111111111119, &FSUB) == -7.4590897516107868e+18);
	REQUIRE(rxRound(RoundUp, -8570200862721897289, -1111111111111111119, &FSUB) == -7.4590897516107848e+18);
	REQUIRE(rxRound(RoundToZero, -8570200862721897289, -1111111111111111119, &FSUB) == -7.4590897516107848e+18);

	REQUIRE(rxRound(RoundToNearest, 1, -10, &FDIV) == -0.10000000000000001);
	REQUIRE(rxRound(RoundDown, 1, -10, &FDIV) == -0.10000000000000001);
	REQUIRE(rxRound(RoundUp, 1, -10, &FDIV) == -0.099999999999999992);
	REQUIRE(rxRound(RoundToZero, 1, -10, &FDIV) == -0.099999999999999992);

	REQUIRE(rxRound(RoundToNearest, -2, 0, &FABSQRT) == 1.4142135623730951);
	REQUIRE(rxRound(RoundDown, -2, 0, &FABSQRT) == 1.4142135623730949);
	REQUIRE(rxRound(RoundUp, -2, 0, &FABSQRT) == 1.4142135623730951);
	REQUIRE(rxRound(RoundToZero, -2, 0, &FABSQRT) == 1.4142135623730949);
}
