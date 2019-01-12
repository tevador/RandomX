	push rax                        ;# preserve eax
	db 0, 0, 0, 0                   ;# TransformAddress placeholder
	mov rcx, qword ptr [rdi]        ;# load the dataset address
	xor rbp, rax                    ;# modify "mx"
	;# prefetch cacheline "mx"
	and rbp, -64                    ;# align "mx" to the start of a cache line
	mov edx, ebp                    ;# edx = mx
	prefetchnta byte ptr [rcx+rdx]
	;# read cacheline "ma"
	ror rbp, 32                     ;# swap "ma" and "mx"
	mov edx, ebp                    ;# edx = ma
	scratchpad_mask                 ;# limit address to the specified scratchpad size aligned to multiple of 8
	lea rax, [rsi+rax*8]            ;# scratchpad cache line
	lea rcx, [rcx+rdx]              ;# dataset cache line
	mov rdx, qword ptr [rcx+0]      ;# load first dataset quadword (prefetched into the cache by now)
	xor qword ptr [rax+0], rdx      ;# XOR the dataset item with a scratchpad item, repeat for the rest of the cacheline
	mov rdx, qword ptr [rcx+8]
	xor qword ptr [rax+8], rdx
	mov rdx, qword ptr [rcx+16]
	xor qword ptr [rax+16], rdx
	mov rdx, qword ptr [rcx+24]
	xor qword ptr [rax+24], rdx
	mov rdx, qword ptr [rcx+32]
	xor qword ptr [rax+32], rdx
	mov rdx, qword ptr [rcx+40]
	xor qword ptr [rax+40], rdx
	mov rdx, qword ptr [rcx+48]
	xor qword ptr [rax+48], rdx
	mov rdx, qword ptr [rcx+56]
	xor qword ptr [rax+56], rdx
	pop rax                         ;# restore eax
	ret