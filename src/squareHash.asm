IFDEF RAX

PUBLIC squareHash
PUBLIC initBlock

.code

squareHash PROC
	include asm/squareHash.inc
squareHash ENDP

; rcx = cache
; rdx = out
; r8 = blockNumber
; r9 = iterations
initBlock PROC
	push rbx
	push rbp
	push rsi
	push rdi
	push r12
	push r13
	push r14
	push r15
	mov rdi, qword ptr [rcx]
	mov rbp, rdx
	; r8 = blockNumber
	mov rsi, r9
	include asm/initBlock.inc
	pop r15
	pop r14
	pop r13
	pop r12
	pop rdi
	pop rsi
	pop rbp
	pop rbx
	ret
initBlock ENDP

ENDIF

END