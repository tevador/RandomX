//RandomX performance test for x86
//https://github.com/tevador/RandomX
//License: GPL v3

#include <cstdint> 
#include <random>
#include <iostream>
#include <chrono>
#include <sstream>
#include <cmath>
#include <cstring>

#if defined(_WIN32) || defined(__MINGW32__) || defined(__CYGWIN__) || defined(__CYGWIN32__)
  #define WINDOWS
  #include <io.h>
  #include <fcntl.h>
#endif

#if defined(__GNUC__) && defined(__x86_64__)
  #include <x86intrin.h>
  typedef unsigned __int128 uint128_t;
  typedef __int128 int128_t;
  static inline uint64_t umulhi64(uint64_t a, uint64_t b) {
     return ((uint128_t)a * b) >> 64;
  }
  static inline uint64_t imulhi64(int64_t a, int64_t b) {
     return ((int128_t)a * b) >> 64;
  }
  #define ror64 __rorq
  #define rol64 __rolq
  #define forceinline inline
  #ifdef __clang__
  static inline uint64_t __rolq(uint64_t a, int b) {
    return (a << b) | (a >> (64 - b));
  }
  static inline uint64_t __rorq(uint64_t a, int b) {
    return (a >> b) | (a << (64 - b));
  }
  #endif
#elif defined(_MSC_VER) && defined(_M_X64)
  #include <intrin.h>
  #include <stdlib.h>
  #define umulhi64 __umulh
  static inline uint64_t imulhi64(int64_t a, int64_t b) {
     int64_t hi;
     _mul128(a, b, &hi);
     return hi;
  }
  #define ror64 _rotr64
  #define rol64 _rotl64
  #define forceinline __forceinline
#else
	#error "Unsupported platform"
#endif

typedef union {
	double f64;
	int64_t i64;
	uint64_t u64;
	int32_t i32;
	uint32_t u32;
} convertible_t;

forceinline void NOOP(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.u64 = a.u64;
}

forceinline void FNOOP(convertible_t& a, convertible_t& b, convertible_t& c) {
	c.f64 = (double)a.i64;
}

forceinline void ADD_64(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.u64 = a.u64 + b.u64;
}

forceinline void ADD_32(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.u64 = a.u32 + b.u32;
}

forceinline void SUB_64(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.u64 = a.u64 - b.u64;
}

forceinline void SUB_32(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.u64 = a.u32 - b.u32;
}

forceinline void MUL_64(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.u64 = a.u64 * b.u64;
}

forceinline void MULH_64(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.u64 = umulhi64(a.u64, b.u64);
}

forceinline void MUL_32(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.u64 = (uint64_t)a.u32 * b.u32;
}

forceinline void IMUL_32(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.i64 = (int64_t)a.i32 * b.i32;
}

forceinline void IMULH_64(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.i64 = imulhi64(a.i64, b.i64);
}

forceinline void DIV_64(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.u64 = a.u64 / (b.u32 != 0 ? b.u32 : 1U);
}

forceinline void IDIV_64(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.i64 = a.i64 / (b.i32 != 0 ? b.i32 : 1);
}

forceinline void AND_64(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.u64 = a.u64 & b.u64;
}

forceinline void AND_32(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.u64 = a.u32 & b.u32;
}

forceinline void OR_64(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.u64 = a.u64 | b.u64;
}

forceinline void OR_32(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.u64 = a.u32 | b.u32;
}

forceinline void XOR_64(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.u64 = a.u64 ^ b.u64;
}

forceinline void XOR_32(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.u64 = a.u32 ^ b.u32;
}

forceinline void SHL_64(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.u64 = a.u64 << (b.u64 & 63);
}

forceinline void SHR_64(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.u64 = a.u64 >> (b.u64 & 63);
}

forceinline void SAR_64(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.i64 = a.i64 >> (b.u64 & 63);
}

forceinline void ROL_64(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.u64 = rol64(a.u64, (b.u64 & 63));
}

forceinline void ROR_64(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.u64 = ror64(a.u64, (b.u64 & 63));
}

forceinline void FADD(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.f64 = (double)a.i64 + (double)b.i64;
}

forceinline void FSUB(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.f64 = (double)a.i64 - (double)b.i64;
}

forceinline void FMUL(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.f64 = (double)a.i64 * (double)b.i64;
}

forceinline void FDIV(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.f64 = (double)a.i64 / (double)b.i64;
}

forceinline void FSQRT(convertible_t& a, convertible_t& b, convertible_t& c) {
  double d = fabs((double)a.i64);
  c.f64 = _mm_cvtsd_f64(_mm_sqrt_sd(_mm_setzero_pd(), _mm_load_pd(&d)));
}

static uint32_t mxcsr;

forceinline void FROUND(convertible_t& a, convertible_t& b, convertible_t& c) {
  c.f64 = (double)a.i64;
  _mm_setcsr(mxcsr | ((uint32_t)(a.u64 << 13) & _MM_ROUND_MASK));
}

inline void init_FPU() {
  mxcsr = (_mm_getcsr() | _MM_FLUSH_ZERO_ON) & ~_MM_ROUND_MASK;
  _mm_setcsr(mxcsr);
}

template<typename T>
bool tryParse(char* buffer, T& out) {
	std::istringstream ss(buffer);
	if (!(ss >> out)) {
		std::cout << "Invalid value '" << buffer << "'" << std::endl;
		return false;
	}
	return true;
}

//#define ITERATIONS 10000000
#define SCRATCHPAD_SIZE (16 * 1024)
#define SCRATCHPAD_LENGTH (SCRATCHPAD_SIZE / sizeof(convertible_t))
#define SCRATCHPAD_MASK (SCRATCHPAD_SIZE / sizeof(convertible_t) - 1)
#define SCRATCHPAD_16K(x) scratchpad[(x) & SCRATCHPAD_MASK]

#define BENCHMARK(FUNC,TYPE) do { \
	memcpy((void*)scratchpad, input, SCRATCHPAD_SIZE); \
	tstart = std::chrono::high_resolution_clock::now(); \
	for (uint64_t i = 0; i < iterations; ++i) { \
		FUNC(SCRATCHPAD_16K(i + 8 + 0), r0, SCRATCHPAD_16K(i + 0)); \
		SCRATCHPAD_16K(i + 0).u64 ^= r7.u64;\
		FUNC(SCRATCHPAD_16K(i + 8 + 1), r1, SCRATCHPAD_16K(i + 1)); \
		SCRATCHPAD_16K(i + 1).u64 ^= r6.u64;\
		FUNC(SCRATCHPAD_16K(i + 8 + 2), r2, SCRATCHPAD_16K(i + 2)); \
		SCRATCHPAD_16K(i + 2).u64 ^= r5.u64;\
		FUNC(SCRATCHPAD_16K(i + 8 + 3), r3, SCRATCHPAD_16K(i + 3)); \
		SCRATCHPAD_16K(i + 3).u64 ^= r4.u64;\
		FUNC(SCRATCHPAD_16K(i + 8 + 4), r4, SCRATCHPAD_16K(i + 4)); \
		SCRATCHPAD_16K(i + 4).u64 ^= r3.u64;\
		FUNC(SCRATCHPAD_16K(i + 8 + 5), r5, SCRATCHPAD_16K(i + 5)); \
		SCRATCHPAD_16K(i + 5).u64 ^= r2.u64;\
		FUNC(SCRATCHPAD_16K(i + 8 + 6), r6, SCRATCHPAD_16K(i + 6)); \
		SCRATCHPAD_16K(i + 6).u64 ^= r1.u64;\
		FUNC(SCRATCHPAD_16K(i + 8 + 7), r7, SCRATCHPAD_16K(i + 7)); \
		SCRATCHPAD_16K(i + 7).u64 ^= r0.u64;\
	} \
	tend = std::chrono::high_resolution_clock::now(); \
	uint64_t acum = 0; \
	for (int i = 0; i < SCRATCHPAD_LENGTH; ++i) \
		acum += scratchpad[i].u64; \
	std::cout << "| " << #FUNC << " | " << std::chrono::duration<double>(tend - tstart).count() << " | " << acum << " |" << std::endl; \
  } while(false)


int main(int argc, char** argv) {
	uint64_t iterations;
	if (argc > 1) {
		if (!tryParse(argv[1], iterations))
			return 1;
	}
	else {
		iterations = 100000000;
	}
#ifdef WINDOWS
  _setmode(_fileno(stdin), O_BINARY);
#endif
  convertible_t input[SCRATCHPAD_LENGTH];

  std::cout << "Reading " << sizeof(input) << " bytes from STDIN..." << std::endl;
  std::cin.read((char*)input, sizeof(input));

  if (!std::cin) {
	  std::cerr << "Insufficient input" << std::endl;
	  return 1;
  }

  convertible_t scratchpad[SCRATCHPAD_LENGTH];
  convertible_t r0, r1, r2, r3, r4, r5, r6, r7;
  
  r0.u64 = input[0].u64;
  r1.u64 = input[1].u64;
  r2.u64 = input[2].u64;
  r3.u64 = input[3].u64;
  r4.u64 = input[4].u64;
  r5.u64 = input[5].u64;
  r6.u64 = input[6].u64;
  r7.u64 = input[7].u64;

  std::chrono::high_resolution_clock::time_point tstart, tend;

  std::cout << iterations << " iterations:" << std::endl << std::endl;

  std::cout << "| operation | time [s] | (result) |" << std::endl;
  std::cout << "|-----------|----------|----------|" << std::endl;

  BENCHMARK(NOOP, u64);
  BENCHMARK(ADD_64, u64);
  BENCHMARK(ADD_32, u64);
  BENCHMARK(SUB_64, u64);
  BENCHMARK(SUB_32, u64);
  BENCHMARK(MUL_64, u64);
  BENCHMARK(MULH_64, u64);
  BENCHMARK(MUL_32, u64);
  BENCHMARK(IMUL_32, u64);
  BENCHMARK(IMULH_64, u64);
  BENCHMARK(DIV_64, u64);
  BENCHMARK(IDIV_64, u64);
  BENCHMARK(AND_64, u64);
  BENCHMARK(AND_32, u64);
  BENCHMARK(OR_64, u64);
  BENCHMARK(OR_32, u64);
  BENCHMARK(XOR_64, u64);
  BENCHMARK(XOR_32, u64);
  BENCHMARK(SHL_64, u64);
  BENCHMARK(SHR_64, u64);
  BENCHMARK(SAR_64, u64);
  BENCHMARK(ROR_64, u64);
  BENCHMARK(ROL_64, u64);
  
  init_FPU();

  BENCHMARK(FNOOP, f64);
  BENCHMARK(FADD, f64);
  BENCHMARK(FSUB, f64);
  BENCHMARK(FMUL, f64);
  BENCHMARK(FDIV, f64);
  BENCHMARK(FSQRT, f64);
  BENCHMARK(FROUND, f64);

  return 0;
}
