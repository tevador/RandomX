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

#include "configuration.h"

.global DECL(squareHash)
.global DECL(initBlock)

DECL(squareHash):
	mov rcx, rdi
	#include "asm/squareHash.inc"

DECL(initBlock):
	push rbx
	push rbp
	push r12
	push r13
	push r14
	push r15
	mov rdi, qword ptr [rdi]
	mov rbp, rsi
	mov r8, rdx
	mov rsi, rcx
	#include "asm/initBlock.inc"
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbp
	pop rbx
	ret