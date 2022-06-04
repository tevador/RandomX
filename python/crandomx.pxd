cdef extern from "../src/randomx.h":

    ctypedef enum randomx_flags:
        RANDOMX_FLAG_DEFAULT
        RANDOMX_FLAG_LARGE_PAGES
        RANDOMX_FLAG_HARD_AES
        RANDOMX_FLAG_FULL_MEM
        RANDOMX_FLAG_JIT
        RANDOMX_FLAG_SECURE
        RANDOMX_FLAG_ARGON2_SSSE3
        RANDOMX_FLAG_ARGON2_AVX2
        RANDOMX_FLAG_ARGON2

    ctypedef struct randomx_dataset:
        pass
    ctypedef struct randomx_cache:
        pass
    ctypedef struct randomx_vm:
        pass

    randomx_flags randomx_get_flags() nogil

    randomx_cache* randomx_alloc_cache(randomx_flags flags) nogil

    void randomx_init_cache(randomx_cache* cache, void* key, size_t keySize) nogil

    void randomx_release_cache(randomx_cache* cache) nogil

    randomx_dataset* randomx_alloc_dataset(randomx_flags flags) nogil

    unsigned long randomx_dataset_item_count() nogil

    void randomx_init_dataset(randomx_dataset* dataset, randomx_cache* cache, unsigned long startItem, unsigned long itemCount) nogil

    void* randomx_get_dataset_memory(randomx_dataset* dataset) nogil

    void randomx_release_dataset(randomx_dataset* dataset) nogil

    randomx_vm* randomx_create_vm(randomx_flags flags, randomx_cache* cache, randomx_dataset* dataset) nogil

    void randomx_vm_set_cache(randomx_vm* machine, randomx_cache* cache) nogil

    void randomx_vm_set_dataset(randomx_vm* machine, randomx_dataset* dataset) nogil

    void randomx_destroy_vm(randomx_vm* machine) nogil

    void randomx_calculate_hash(randomx_vm* machine, void* input, size_t inputSize, void* output) nogil

    void randomx_calculate_hash_first(randomx_vm* machine, void* input, size_t inputSize) nogil

    void randomx_calculate_hash_next(randomx_vm* machine, void* nextInput, size_t nextInputSize, void* output) nogil

    void randomx_calculate_hash_last(randomx_vm* machine, void* output) nogil
