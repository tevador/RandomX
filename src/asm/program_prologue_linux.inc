	;# callee-saved registers - System V AMD64 ABI
	push rbx
	push rbp
	push r12
	push r13
	push r14
	push r15

	;# function arguments
	push rdi        ;# RegisterFile& registerFile
	mov rbx, rsi    ;# MemoryRegisters& memory
	mov rsi, rdx    ;# convertible_t* scratchpad
	mov rcx, rdi

	#include "program_prologue_load.inc"

	jmp randomx_program_begin