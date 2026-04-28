/*
Copyright (c) 2026, Forest Crossman <cyrozap@gmail.com>

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	* Neither the name of the copyright holder nor the
	  names of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

extern "C" {
	void randomx_ppc64_constants();
	void randomx_ppc64_constant_lut_fprc_to_fpscr();
	void randomx_ppc64_constants_end();

	void randomx_ppc64_dataset_init();
	void randomx_ppc64_dataset_init_fix_call();
	void randomx_ppc64_dataset_init_end();

	void randomx_ppc64_sshash_single_item_prologue();
	void randomx_ppc64_sshash_single_item_prologue_end();
	void randomx_ppc64_sshash_single_item_epilogue();
	void randomx_ppc64_sshash_single_item_epilogue_end();
	void randomx_ppc64_sshash_cache_prefetch();
	void randomx_ppc64_sshash_cache_prefetch_end();
	void randomx_ppc64_sshash_xor();
	void randomx_ppc64_sshash_xor_end();

	void randomx_ppc64_vm_prologue();
	void randomx_ppc64_vm_prologue_end();
	void randomx_ppc64_vm_epilogue();
	void randomx_ppc64_vm_fix_loop();
	void randomx_ppc64_vm_epilogue_end();
	void randomx_ppc64_vm_loop_prologue();
	void randomx_ppc64_vm_loop_prologue_end();
	void randomx_ppc64_vm_data_read();
	void randomx_ppc64_vm_data_read_end();
	void randomx_ppc64_vm_data_read_light();
	void randomx_ppc64_vm_data_read_light_fix_call();
	void randomx_ppc64_vm_data_read_light_end();
	void randomx_ppc64_vm_spad_store_group_r();
	void randomx_ppc64_vm_spad_store_group_r_end();
	void randomx_ppc64_vm_spad_store_mix_v1();
	void randomx_ppc64_vm_spad_store_mix_v1_end();
	void randomx_ppc64_vm_spad_store_mix_v2_hard_aes();
	void randomx_ppc64_vm_spad_store_mix_v2_hard_aes_end();
	void randomx_ppc64_vm_spad_store_mix_v2_soft_aes();
	void randomx_ppc64_vm_spad_store_mix_v2_soft_aes_end();
}
