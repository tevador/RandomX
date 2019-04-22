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

#ifndef RANDOMX_H
#define RANDOMX_H

#include <stddef.h>

#define RANDOMX_HASH_SIZE 32
#define RANDOMX_DATASET_ITEMS 33554432UL

typedef enum {
  RANDOMX_FLAG_DEFAULT = 0,
  RANDOMX_FLAG_LARGE_PAGES = 1,
  RANDOMX_FLAG_HARD_AES = 2,
  RANDOMX_FLAG_FULL_MEM = 4,
  RANDOMX_FLAG_JIT = 8,
} randomx_flags;

typedef struct randomx_dataset randomx_dataset;
typedef struct randomx_cache randomx_cache;
typedef struct randomx_vm randomx_vm;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Creates a randomx_cache structure and allocates memory for RandomX Cache.
 *
 * @param flags is any combination of these 2 flags (each flag can be set or not set):
 *        RANDOMX_FLAG_LARGE_PAGES - allocate memory in large pages
 *        RANDOMX_FLAG_JIT - create cache structure with JIT compilation support; this makes
 *                           subsequent Dataset initialization faster
 *
 * @return Pointer to an allocated randomx_cache structure.
           NULL is returned if memory allocation fails or if the RANDOMX_FLAG_JIT
		   is set and JIT compilation is not supported on the current platform.
 */
randomx_cache *randomx_alloc_cache(randomx_flags flags);

/**
 * Initializes the cache memory and SuperscalarHash using the provided seed value.
 *
 * @param cache is a pointer to a previously allocated randomx_cache structure. Must not be NULL.
 * @param seed is a pointer to memory which contains the seed value. Must not be NULL.
 * @param seedSize is the number of bytes of the seed.
*/
void randomx_init_cache(randomx_cache *cache, const void *seed, size_t seedSize);

/**
 * Releases all memory occupied by the randomx_cache structure.
 *
 * @param cache is a pointer to a previously allocated randomx_cache structure.
*/
void randomx_release_cache(randomx_cache* cache);

/**
 * Creates a randomx_dataset structure and allocates memory for RandomX Dataset.
 *
 * @param flags is the initialization flags. Only one flag is supported (can be set or not set):
 *        RANDOMX_FLAG_LARGE_PAGES - allocate memory in large pages

 * @return Pointer to an allocated randomx_cache structure.
		   NULL is returned if memory allocation fails.
 */
randomx_dataset *randomx_alloc_dataset(randomx_flags flags);

/**
 * Initializes dataset items.
 *
 * Note: In order to use the Dataset, all items from 0 to (RANDOMX_DATASET_ITEMS - 1) must be initialized.
 * This may be done by several calls to this function using non-overlapping item sequences.
 *
 * @param dataset is a pointer to a previously allocated randomx_dataset structure. Must not be NULL.
 * @param cache is a pointer to a previously allocated and initialized randomx_cache structure. Must not be NULL.
 * @param startItem is the item number where intialization should start.
 * @param itemCount is the number of items that should be initialized.
*/
void randomx_init_dataset(randomx_dataset *dataset, randomx_cache *cache, unsigned long startItem, unsigned long itemCount);

/**
 * Releases all memory occupied by the randomx_dataset structure.
 *
 * @param dataset is a pointer to a previously allocated randomx_dataset structure.
*/
void randomx_release_dataset(randomx_dataset *dataset);

/**
 * Creates and initializes a RandomX virtual machine.
 *
 * @param flags is any combination of these 4 flags (each flag can be set or not set):
 *        RANDOMX_FLAG_LARGE_PAGES - allocate scratchpad memory in large pages
 *        RANDOMX_FLAG_HARD_AES - virtual machine will use hardware accelerated AES
 *        RANDOMX_FLAG_FULL_MEM - virtual machine will use the full dataset
 *        RANDOMX_FLAG_JIT - virtual machine will use a JIT compiler
 *        The numeric values of the flags are ordered so that a higher value will provide
 *        faster hash calculation and a lower numeric value will provide higher portability.
 *        Using RANDOMX_FLAG_DEFAULT (all flags not set) works on all platforms, but is the slowest.
 * @param cache is a pointer to an initialized randomx_cache structure. Can be
 *        NULL if RANDOMX_FLAG_FULL_MEM is set.
 * @param dataset is a pointer to a randomx_dataset structure. Can be NULL
 *        if RANDOMX_FLAG_FULL_MEM is not set.
 *
 * @return Pointer to an initialized randomx_vm structure.
 *         Returns NULL if:
 *         (1) Scratchpad memory allocation fails.
 *         (2) The requested initialization flags are not supported on the current platform.
 *         (3) cache parameter is NULL and RANDOMX_FLAG_FULL_MEM is not set
 *         (4) dataset parameter is NULL and RANDOMX_FLAG_FULL_MEM is set
*/
randomx_vm *randomx_create_vm(randomx_flags flags, randomx_cache *cache, randomx_dataset *dataset);

/**
 * Reinitializes a virtual machine with a new Cache. This function should be called anytime
 * the Cache is reinitialized with a new seed.
 *
 * @param machine is a pointer to a randomx_vm structure that was initialized
 *        without RANDOMX_FLAG_FULL_MEM. Must not be NULL.
 * @param cache is a pointer to an initialized randomx_cache structure. Must not be NULL.
*/
void randomx_vm_set_cache(randomx_vm *machine, randomx_cache* cache);

/**
 * Reinitializes a virtual machine with a new Dataset.
 *
 * @param machine is a pointer to a randomx_vm structure that was initialized
 *        with RANDOMX_FLAG_FULL_MEM. Must not be NULL.
 * @param dataset is a pointer to an initialized randomx_dataset structure. Must not be NULL.
*/
void randomx_vm_set_dataset(randomx_vm *machine, randomx_dataset *dataset);

/**
 * Releases all memory occupied by the randomx_vm structure.
 *
 * @param machine is a pointer to a previously created randomx_vm structure.
*/
void randomx_destroy_vm(randomx_vm *machine);

/**
 * Calculates a RandomX hash value.
 *
 * @param machine is a pointer to a randomx_vm structure. Must not be NULL.
 * @param input is a pointer to memory to be hashed. Must not be NULL.
 * @param inputSize is the number of bytes to be hashed.
 * @param output is a pointer to memory where the hash will be stored. Must not
 *        be NULL and at least RANDOMX_HASH_SIZE bytes must be available for writing.
*/
void randomx_calculate_hash(randomx_vm *machine, const void *input, size_t inputSize, void *output);

#if defined(__cplusplus)
}
#endif

#endif
