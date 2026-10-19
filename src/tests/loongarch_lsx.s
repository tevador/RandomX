/* LoongArch - test if the LSX vector extension is present */

	.text
	.global	main

main:
	li.w	$a0, 0
	vxor.v	$vr0, $vr0, $vr0
	ret
