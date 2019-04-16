/*
Copyright (c) 2019 tevador

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

#pragma once

//Cache size in KiB. Must be a power of 2.
#define RANDOMX_ARGON_MEMORY       (256 * 1024)

//Cache growth per epoch in KiB.
#define RANDOMX_ARGON_GROWTH       0

//Number of Argon2d iterations for Cache initialization
#define RANDOMX_ARGON_ITERATIONS   3

//Number of parallel lanes for Cache initialization
#define RANDOMX_ARGON_LANES        1

//Argon2d salt
#define RANDOMX_ARGON_SALT         "RandomX\x03"

//Number of random Cache accesses per Dataset block. Minimum is 2.
#define RANDOMX_CACHE_ACCESSES     8

#define RANDOMX_SUPERSCALAR_LATENCY   170
#define RANDOMX_SUPERSCALAR_MAX_SIZE  512

//Dataset size in bytes. Must be a power of 2.
#define RANDOMX_DATASET_SIZE       (2ULL * 1024 * 1024 * 1024)

//Dataset growth per epoch in bytes. Must be divisible by 64.
#define RANDOMX_DS_GROWTH          0

//Number of blocks per epoch
#define RANDOMX_EPOCH_BLOCKS       2048

//Number of blocks between the seed block and the start of new epoch
#define RANDOMX_EPOCH_LAG          64

//Number of instructions in a RandomX program
#define RANDOMX_PROGRAM_SIZE       256

//Number of iterations during VM execution
#define RANDOMX_PROGRAM_ITERATIONS 2048

//Number of chained VM executions per hash
#define RANDOMX_PROGRAM_COUNT      8

//Scratchpad L3 size in bytes. Must be a power of 2.
#define RANDOMX_SCRATCHPAD_L3      (2 * 1024 * 1024)

//Scratchpad L2 size in bytes. Must be a power of two and less than or equal to RANDOMX_SCRATCHPAD_L3.
#define RANDOMX_SCRATCHPAD_L2      (256 * 1024)

//Scratchpad L1 size in bytes. Must be a power of two and less than or equal to RANDOMX_SCRATCHPAD_L2.
#define RANDOMX_SCRATCHPAD_L1      (16 * 1024)

//How many register bits must be zero for a jump condition to be triggered
#define RANDOMX_CONDITION_BITS     7

/*
Instruction frequencies (per 256 opcodes)
Total sum of frequencies must be 256
*/

#define RANDOMX_FREQ_IADD_RS       32
#define RANDOMX_FREQ_IADD_M         7
#define RANDOMX_FREQ_IADD_RC        0
#define RANDOMX_FREQ_ISUB_R        17
#define RANDOMX_FREQ_ISUB_M         7
#define RANDOMX_FREQ_IMUL_9C        0
#define RANDOMX_FREQ_IMUL_R        16
#define RANDOMX_FREQ_IMUL_M         4
#define RANDOMX_FREQ_IMULH_R        4
#define RANDOMX_FREQ_IMULH_M        1
#define RANDOMX_FREQ_ISMULH_R       4
#define RANDOMX_FREQ_ISMULH_M       1
#define RANDOMX_FREQ_IMUL_RCP       8
#define RANDOMX_FREQ_INEG_R         2
#define RANDOMX_FREQ_IXOR_R        15
#define RANDOMX_FREQ_IXOR_M         5
#define RANDOMX_FREQ_IROR_R        10
#define RANDOMX_FREQ_IROL_R         0
#define RANDOMX_FREQ_ISWAP_R        4

#define RANDOMX_FREQ_FSWAP_R        8
#define RANDOMX_FREQ_FADD_R        20
#define RANDOMX_FREQ_FADD_M         5
#define RANDOMX_FREQ_FSUB_R        20
#define RANDOMX_FREQ_FSUB_M         5
#define RANDOMX_FREQ_FSCAL_R        6
#define RANDOMX_FREQ_FMUL_R        20
#define RANDOMX_FREQ_FDIV_M         4
#define RANDOMX_FREQ_FSQRT_R        6

#define RANDOMX_FREQ_COND_R         8
#define RANDOMX_FREQ_COND_M         0
#define RANDOMX_FREQ_CFROUND        1
#define RANDOMX_FREQ_ISTORE        16

#define RANDOMX_FREQ_NOP            0
/*                               ------
                                  256
*/
