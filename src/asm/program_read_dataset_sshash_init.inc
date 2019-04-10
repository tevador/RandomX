	sub rsp, 72
	mov qword ptr [rsp+64], rbx
	mov qword ptr [rsp+56], r8
	mov qword ptr [rsp+48], r9
	mov qword ptr [rsp+40], r10
	mov qword ptr [rsp+32], r11
	mov qword ptr [rsp+24], r12
	mov qword ptr [rsp+16], r13
	mov qword ptr [rsp+8], r14
	mov qword ptr [rsp+0], r15
	xor rbp, rax                       ;# modify "mx"
	ror rbp, 32                        ;# swap "ma" and "mx"
	mov ebx, ebp                       ;# ecx = ma
	and ebx, 2147483584                ;# align "ma" to the start of a cache line
	shr ebx, 6                         ;# ebx = Dataset block number
	;# call 32768