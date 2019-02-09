.intel_syntax noprefix
#if defined(__APPLE__)
.text
#else
.section .text
#endif
#if defined(__WIN32__) || defined(__APPLE__)
#define DECL(x) _##x
#else
#define DECL(x) x
#endif

.global DECL(squareHash)

DECL(squareHash):
	mov rcx, rsi
	#include "asm/squareHash.inc"
