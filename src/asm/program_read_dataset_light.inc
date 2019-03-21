	xor rbp, rax                       ;# modify "mx"
	ror rbp, 32                        ;# swap "ma" and "mx"
	mov ecx, ebp                       ;# ecx = ma
	shr ecx, 6                         ;# ecx = Dataset block number
