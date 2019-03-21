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

IFDEF RAX

_RANDOMX_JITX86_STATIC SEGMENT PAGE READ EXECUTE

PUBLIC randomx_program_prologue
PUBLIC randomx_program_loop_begin
PUBLIC randomx_program_loop_load
PUBLIC randomx_program_start
PUBLIC randomx_program_read_dataset
PUBLIC randomx_program_read_dataset_light
PUBLIC randomx_program_read_dataset_light_sub
PUBLIC randomx_program_loop_store
PUBLIC randomx_program_loop_end
PUBLIC randomx_program_epilogue
PUBLIC randomx_program_end

ALIGN 64
randomx_program_prologue PROC
	include asm/program_prologue_win64.inc
randomx_program_prologue ENDP

ALIGN 64
	include asm/program_xmm_constants.inc

ALIGN 64
randomx_program_loop_begin PROC
	nop
randomx_program_loop_begin ENDP

randomx_program_loop_load PROC
	include asm/program_loop_load.inc
randomx_program_loop_load ENDP

randomx_program_start PROC
	nop
randomx_program_start ENDP

randomx_program_read_dataset PROC
	include asm/program_read_dataset.inc
randomx_program_read_dataset ENDP

randomx_program_read_dataset_light PROC
	include asm/program_read_dataset_light.inc
randomx_program_read_dataset_light ENDP

randomx_program_loop_store PROC
	include asm/program_loop_store.inc
randomx_program_loop_store ENDP

randomx_program_loop_end PROC
	nop
randomx_program_loop_end ENDP

ALIGN 64
randomx_program_read_dataset_light_sub PROC
	include asm/program_read_dataset_light_sub.inc
	squareHashSub:
		include asm/squareHash.inc
randomx_program_read_dataset_light_sub ENDP

ALIGN 64
randomx_program_epilogue PROC
	include asm/program_epilogue_win64.inc
randomx_program_epilogue ENDP

ALIGN 64
randomx_program_end PROC
	nop
randomx_program_end ENDP

_RANDOMX_JITX86_STATIC ENDS

ENDIF

END