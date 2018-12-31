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

_RANDOMX_JITX86_STATIC SEGMENT PAGE READ EXECUTE

PUBLIC randomx_program_prologue
PUBLIC randomx_program_begin
PUBLIC randomx_program_epilogue
PUBLIC randomx_program_read_r
PUBLIC randomx_program_read_f
PUBLIC randomx_program_end

ALIGN 64
randomx_program_prologue PROC
	include asm/program_prologue_win64.inc
randomx_program_prologue ENDP

ALIGN 64
randomx_program_begin PROC
	nop
randomx_program_begin ENDP

ALIGN 64
randomx_program_epilogue PROC
	include asm/program_epilogue_win64.inc
randomx_program_epilogue ENDP

ALIGN 64
randomx_program_read_r PROC
	include asm/program_read_r.inc
randomx_program_read_r ENDP

ALIGN 64
randomx_program_read_f PROC
	include asm/program_read_f.inc
randomx_program_read_f ENDP

ALIGN 64
randomx_program_end PROC
	nop
randomx_program_end ENDP

_RANDOMX_JITX86_STATIC ENDS

END