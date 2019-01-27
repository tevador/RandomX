/*
Copyright (c) 2018 tevador

This file is part of RandomX.

RandomX is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RandomX is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RandomX.  If not, see<http://www.gnu.org/licenses/>.
*/

extern "C" {
	void randomx_program_prologue();
	void randomx_loop_begin();
	void randomx_program_load_int();
	void randomx_program_load_flt();
	void randomx_program_start();
	void randomx_program_read_dataset();
	void randomx_program_store_int();
	void randomx_program_store_flt();
	void randomx_program_loop_end();
	void randomx_program_epilogue();
	void randomx_program_end();
}