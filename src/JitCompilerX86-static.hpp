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
	void randomx_program_loop_begin();
	void randomx_program_loop_load();
	void randomx_program_start();
	void randomx_program_read_dataset();
	void randomx_program_read_dataset_light();
	void randomx_program_read_dataset_sshash_init();
	void randomx_program_read_dataset_sshash_fin();
	void randomx_program_loop_store();
	void randomx_program_loop_end();
	void randomx_program_read_dataset_light_sub();
	void randomx_dataset_init();
	void randomx_program_epilogue();
	void randomx_sshash_load();
	void randomx_sshash_prefetch();
	void randomx_sshash_end();
	void randomx_sshash_init();
	void randomx_program_end();
}