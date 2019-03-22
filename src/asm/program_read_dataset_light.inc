	xor rbp, rax                       ;# modify "mx"
	ror rbp, 32                        ;# swap "ma" and "mx"
	mov ecx, ebp                       ;# ecx = ma
	and ecx, 2147483584                ;# align "ma" to the start of a cache line
	shr ecx, 6                         ;# ecx = Dataset block number
