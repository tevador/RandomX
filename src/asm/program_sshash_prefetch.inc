	and rbx, 4194303
	shl rbx, 6
	add rbx, rdi
	prefetchnta byte ptr [rbx]