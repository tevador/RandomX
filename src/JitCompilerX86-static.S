;# Copyright (c) 2018 tevador
;#
;# This file is part of RandomX.
;#
;# RandomX is free software: you can redistribute it and/or modify
;# it under the terms of the GNU General Public License as published by
;# the Free Software Foundation, either version 3 of the License, or
;# (at your option) any later version.
;#
;# RandomX is distributed in the hope that it will be useful,
;# but WITHOUT ANY WARRANTY; without even the implied warranty of
;# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;# GNU General Public License for more details.
;#
;# You should have received a copy of the GNU General Public License
;# along with RandomX.  If not, see<http://www.gnu.org/licenses/>.

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
.global DECL(randomx_program_prologue)
.global DECL(randomx_loop_begin)
.global DECL(randomx_program_load_int)
.global DECL(randomx_program_load_flt)
.global DECL(randomx_program_start)
.global DECL(randomx_program_read_dataset)
.global DECL(randomx_program_store_int)
.global DECL(randomx_program_store_flt)
.global DECL(randomx_program_loop_end)
.global DECL(randomx_program_epilogue)
.global DECL(randomx_program_end)

#define db .byte

.align 64
DECL(randomx_program_prologue):
	#include "asm/program_prologue_linux.inc"

.align 64
	#include "asm/program_xmm_constants.inc"

.align 64
DECL(randomx_loop_begin):
	nop

DECL(randomx_program_load_int):
	#include "asm/program_load_int.inc"

DECL(randomx_program_load_flt):
	#include "asm/program_load_flt.inc"

DECL(randomx_program_start):
	nop

DECL(randomx_program_read_dataset):
	#include "asm/program_read_dataset.inc"

DECL(randomx_program_store_int):
	#include "asm/program_store_int.inc"

DECL(randomx_program_store_flt):
	#include "asm/program_store_flt.inc"

DECL(randomx_program_loop_end):
	nop

.align 64
DECL(randomx_program_epilogue):
	#include "asm/program_epilogue_linux.inc"

.align 64
DECL(randomx_program_end):
	nop
