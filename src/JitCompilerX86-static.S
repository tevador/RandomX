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
.global DECL(randomx_program_begin)
.global DECL(randomx_program_epilogue)
.global DECL(randomx_program_read_r)
.global DECL(randomx_program_read_f)
.global DECL(randomx_program_end)

.align 64
DECL(randomx_program_prologue):
	#include "asm/program_prologue_linux.inc"

.align 64
DECL(randomx_program_begin):
	nop

.align 64
DECL(randomx_program_epilogue):
	#include "asm/program_epilogue_linux.inc"

.align 64
DECL(randomx_program_read_r):
	#include "asm/program_read_r.inc"

.align 64
DECL(randomx_program_read_f):
	#include "asm/program_read_f.inc"

.align 64
DECL(randomx_program_end):
	nop