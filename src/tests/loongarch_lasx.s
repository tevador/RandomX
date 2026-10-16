/* LoongArch - test if the LASX vector extension is present */

	.text
	.global	main

main:
	li.w	$a0, 0
	xvxor.v	$xr0, $xr0, $xr0
	ret
