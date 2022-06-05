cimport crandomx

import multiprocessing, threading

cpdef enum:
    HASH_SIZE = 32
    DATASET_ITEM_SIZE = 64

FLAG_DEFAULT      = crandomx.RANDOMX_FLAG_DEFAULT
FLAG_LARGE_PAGES  = crandomx.RANDOMX_FLAG_LARGE_PAGES
FLAG_HARD_AES     = crandomx.RANDOMX_FLAG_HARD_AES
FLAG_FULL_MEM     = crandomx.RANDOMX_FLAG_FULL_MEM
FLAG_JIT          = crandomx.RANDOMX_FLAG_JIT
FLAG_SECURE       = crandomx.RANDOMX_FLAG_SECURE
FLAG_ARGON2_SSSE3 = crandomx.RANDOMX_FLAG_ARGON2_SSSE3
FLAG_ARGON2_AVX2  = crandomx.RANDOMX_FLAG_ARGON2_AVX2
FLAG_ARGON2       = crandomx.RANDOMX_FLAG_ARGON2

cdef const void * voidptr(const unsigned char[:] data):
    if data.size == 0:
        return NULL
    else:
        return <const void*>&data[0]

def get_flags():
    '''
    /**
     * @return The recommended flags to be used on the current machine.
     *         Does not include:
     *            RANDOMX_FLAG_LARGE_PAGES
     *            RANDOMX_FLAG_FULL_MEM
     *            RANDOMX_FLAG_SECURE
     *         These flags must be added manually if desired.
     *         On OpenBSD RANDOMX_FLAG_SECURE is enabled by default in JIT mode as W^X is enforced by the OS.
     */
    '''
    return crandomx.randomx_get_flags()

cdef class Cache:
    cdef crandomx.randomx_cache * _c_cache
    cdef int _flags
    cdef int _initialized

    def __cinit__(self, crandomx.randomx_flags flags, const unsigned char [:] key = None):
        self._c_cache = crandomx.randomx_alloc_cache(flags)
        if self._c_cache is NULL:
            self._c_cache = crandomx.randomx_alloc_cache(
                <crandomx.randomx_flags>(
                    flags & ~crandomx.RANDOMX_FLAG_JIT
                )
            )
        if self._c_cache is NULL:
            self._c_cache = crandomx.randomx_alloc_cache(
                <crandomx.randomx_flags>(
                    flags &
                    ~crandomx.RANDOMX_FLAG_ARGON2_SSSE3 &
                    ~crandomx.RANDOMX_FLAG_ARGON2_AVX2
                )
            )
        if self._c_cache is NULL:
            self._c_cache = crandomx.randomx_alloc_cache(
                <crandomx.randomx_flags>(
                    flags &
                    ~crandomx.RANDOMX_FLAG_JIT &
                    ~crandomx.RANDOMX_FLAG_ARGON2_SSSE3 &
                    ~crandomx.RANDOMX_FLAG_ARGON2_AVX2
                )
            )
        if self._c_cache is NULL:
            raise MemoryError()
        self._flags = flags
        self._initialized = False

    def __init__(self, crandomx.randomx_flags flags, const unsigned char [:] key = None):
        '''
        /**
         * Creates a randomx_cache structure and allocates memory for RandomX Cache.
         *
         * @param flags is any combination of these 2 flags (each flag can be set or not set):
         *        RANDOMX_FLAG_LARGE_PAGES - allocate memory in large pages
         *        RANDOMX_FLAG_JIT - create cache structure with JIT compilation support; this makes
         *                           subsequent Dataset initialization faster
         *        Optionally, one of these two flags may be selected:
         *        RANDOMX_FLAG_ARGON2_SSSE3 - optimized Argon2 for CPUs with the SSSE3 instruction set
         *                                   makes subsequent cache initialization faster
         *        RANDOMX_FLAG_ARGON2_AVX2 - optimized Argon2 for CPUs with the AVX2 instruction set
         *                                   makes subsequent cache initialization faster
         */
        '''
        if key is not None:
            self.init(key)

    def init(self, const unsigned char [:] key not None):
        '''
        /**
         * Initializes the cache memory and SuperscalarHash using the provided key value.
         * Does nothing if called again with the same key value.
         *
         * @param key is a pointer to memory which contains the key value. Must not be NULL.
        */
        '''
        cdef const void * voidptrkey = voidptr(key)
        cdef int keylen = len(key)
        with nogil:
            crandomx.randomx_init_cache(self._c_cache, voidptrkey, keylen)
        self._initialized = True

    @property
    def initialized(self):
        return self._initialized

    @property
    def flags(self):
        return self._flags

    def __dealloc__(self):
        if self._c_cache is not NULL:
            crandomx.randomx_release_cache(self._c_cache)

cdef class Dataset:
    cdef crandomx.randomx_dataset * _c_dataset

    ITEM_SIZE = DATASET_ITEM_SIZE

    def __cinit__(self, flags):
        self._c_dataset = crandomx.randomx_alloc_dataset(flags)
        if self._c_dataset is NULL:
            raise MemoryError()

    def __init__(self, flags):
        '''
        /**
         * Creates a randomx_dataset structure and allocates memory for RandomX Dataset.
         *
         * @param flags is the initialization flags. Only one flag is supported (can be set or not set):
         *        RANDOMX_FLAG_LARGE_PAGES - allocate memory in large pages
         *
         * @return Pointer to an allocated randomx_dataset structure.
         *         NULL is returned if memory allocation fails.
         */
        '''

    @staticmethod
    def count():
        '''
        /**
         * Gets the number of items contained in the dataset.
         *
         * @return the number of items contained in the dataset.
        */
        '''
        return crandomx.randomx_dataset_item_count()

    def __len__(self):
        return self.count()

    def init(self, Cache cache, int start_item = 0, int item_count = 0):
        '''
        /**
         * Initializes dataset items.
         *
         * Note: In order to use the Dataset, all items from 0 to (randomx_dataset_item_count() - 1) must be initialized.
         * This may be done by several calls to this function using non-overlapping item sequences.
         *
         * @param cache is a pointer to a previously allocated and initialized randomx_cache structure. Must not be NULL.
         *
         * Optional:
         * @param startItem is the item number where intialization should start.
         * @param itemCount is the number of items that should be initialized.
        */
        '''
        if item_count == 0:
            item_count = Dataset.count() - start_item
        with nogil:
            crandomx.randomx_init_dataset(self._c_dataset, cache._c_cache, start_item, item_count)

    def init_parallel(self, Cache cache, int start_item = 0, int item_count = 0, int threads = 0):
        '''
        /**
         * Initializes dataset items, spawning multiple threads for speed.
         *
         * Note: In order to use the Dataset, all items from 0 to (randomx_dataset_item_count() - 1) must be initialized.
         *
         * @param cache is a pointer to a previously allocated and initialized randomx_cache structure. Must not be NULL.
         *
         * Optional:
         * @param startItem is the item number where intialization should start.
         * @param itemCount is the number of items that should be initialized.
         * @param threads is the number of threads to spawn. Defaults to twice the cores.
        */
        '''
        if threads <= 0:
            threads = multiprocessing.cpu_count() * 2
        if item_count == 0:
            item_count = Dataset.count() - start_item
        thread_list = []
        last_offset = 0 
        for thread_idx in range(threads):
            next_offset = (thread_idx + 1) * item_count // threads
            thread = threading.Thread(target = self.init, args = (cache, last_offset + start_item, next_offset - last_offset))
            thread.start()
            thread_list.append(thread)
            last_offset = next_offset
        for thread in thread_list:
            thread.join()

    def get_memory(self):
        '''
        /**
         * Returns the internal memory buffer of the dataset structure. The size
         * of the internal memory buffer is randomx_dataset_item_count() * RANDOMX_DATASET_ITEM_SIZE.
         *
         * @return Pointer to the internal memory buffer of the dataset structure.
        */
        '''
        return <unsigned char [:Dataset.count() * DATASET_ITEM_SIZE]>crandomx.randomx_get_dataset_memory(self._c_dataset)
        
    def __dealloc__(self):
        if self._c_dataset is not NULL:
            crandomx.randomx_release_dataset(self._c_dataset)

cdef class VM:
    cdef crandomx.randomx_vm * _c_vm
    cdef crandomx.randomx_flags _flags
    cdef _cache
    cdef _dataset

    def __cinit__(self, flags = FLAG_DEFAULT, cache : Cache = None, dataset : Dataset = None):
        cdef crandomx.randomx_cache * c_cache
        cdef crandomx.randomx_dataset * c_dataset
        if cache is None:
            if flags & FLAG_FULL_MEM:
                c_cache = NULL
            else:
                raise RuntimeError('cache parameter is NULL and RANDOMX_FLAG_FULL_MEM is not set')
        else:
            if not cache.initialized:
                raise AssertionError('cache not initialized')
            c_cache = cache._c_cache
        if dataset is None:
            if flags & FLAG_FULL_MEM:
                raise RuntimeError('dataset parameter is NULL and RANDOMX_FLAG_FULL_MEM is set')
            else:
                c_dataset = NULL
        else:
            c_dataset = dataset._c_dataset
        self._c_vm = crandomx.randomx_create_vm(flags, c_cache, c_dataset)
        if self._c_vm is NULL:
            raise MemoryError('out of memory or unsupported flags')

    def __init__(self, flags = FLAG_DEFAULT, cache : Cache = None, dataset : Dataset = None):
        '''
        /**
         * Creates and initializes a RandomX virtual machine.
         *
         * @param flags is any combination of these 5 flags (each flag can be set or not set):
         *        RANDOMX_FLAG_LARGE_PAGES - allocate scratchpad memory in large pages
         *        RANDOMX_FLAG_HARD_AES - virtual machine will use hardware accelerated AES
         *        RANDOMX_FLAG_FULL_MEM - virtual machine will use the full dataset
         *        RANDOMX_FLAG_JIT - virtual machine will use a JIT compiler
         *        RANDOMX_FLAG_SECURE - when combined with RANDOMX_FLAG_JIT, the JIT pages are never
         *                              writable and executable at the same time (W^X policy)
         *        The numeric values of the first 4 flags are ordered so that a higher value will provide
         *        faster hash calculation and a lower numeric value will provide higher portability.
         *        Using RANDOMX_FLAG_DEFAULT (all flags not set) works on all platforms, but is the slowest.
         * @param cache is a pointer to an initialized randomx_cache structure. Can be
         *        NULL if RANDOMX_FLAG_FULL_MEM is set.
         * @param dataset is a pointer to a randomx_dataset structure. Can be NULL
         *        if RANDOMX_FLAG_FULL_MEM is not set.
        */
        '''
        self._flags = flags
        self._cache = cache
        self._dataset = dataset

    @property
    def cache(self):
        '''
        /**
         * Reinitializes a virtual machine with a new Cache. This should be set anytime
         * the Cache is reinitialized with a new key. Does nothing if called with a Cache containing
         * the same key value as already set.
         *
         * @param cache is a pointer to an initialized randomx_cache structure. Must not be NULL.
        */
        '''
        return self._cache

    @cache.setter
    def cache(self, cache : Cache):
        crandomx.randomx_vm_set_cache(self._c_vm, cache._c_cache)

    @property
    def dataset(self):
        '''
        /**
         * Reinitializes a virtual machine with a new Dataset.
         *
         * @param dataset is a pointer to an initialized randomx_dataset structure. Must not be NULL.
        */
        '''
        return self._dataset

    @dataset.setter
    def dataset(self,  dataset : Dataset):
        crandomx.randomx_vm_set_dataset(self._c_vm, dataset._c_dataset)

    @property
    def flags(self):
        return self._flags

    def __dealloc__(self):
        if self._c_vm is not NULL:
            crandomx.randomx_destroy_vm(self._c_vm)

    cpdef calculate_hash(self, const unsigned char[:] input, unsigned char output[HASH_SIZE] = NULL):
        '''
        /**
         * Calculates a RandomX hash value.
         *
         * @param input is a pointer to memory to be hashed. Must not be NULL.
         * @param output is a pointer to memory where the hash will be stored. Must be
         *        at least RANDOMX_HASH_SIZE bytes must be available for writing.
         *        New storage will be allocated if this is NULL or left out.
        */
        '''
        cdef const void * input_ptr = voidptr(input)
        cdef unsigned char local_output[HASH_SIZE]
        if output is NULL:
            output = local_output
        with nogil:
            crandomx.randomx_calculate_hash(self._c_vm, input_ptr, len(input), <void*>output)
        return output[:HASH_SIZE]

    def __call__(self, const unsigned char[:] input not None, unsigned char output[HASH_SIZE] = NULL):
        '''An alias for calculate_hash'''
        cdef unsigned char local_output[HASH_SIZE]
        if output is NULL:
            output = local_output
        return self.calculate_hash(input, output)

    def calculate_hashes(self, inputs not None):
        '''
        /**
         * Set of functions used to calculate multiple RandomX hashes more efficiently.
         * calculate_hashes     is a convenience function that takes an iterable.
         * calculate_hash_first will begin a hash calculation.
         * calculate_hash_next  will output the hash value of the previous input
         *                      and begin the calculation of the next hash.
         * calculate_hash_last  will output the hash value of the previous input.
         *
         * WARNING: These functions may alter the floating point rounding mode of the calling thread.
         *
         * @param input is a pointer to memory to be hashed. Must not be NULL.
         * @param nextInput is a pointer to memory to be hashed for the next hash. Must not be NULL.
         * @param nextInputSize is the number of bytes to be hashed for the next hash.
         * @param output is a pointer to memory where the hash will be stored. Must not
         *        be NULL and at least RANDOMX_HASH_SIZE bytes must be available for writing.
        */
        '''
        cdef unsigned char output[HASH_SIZE]
        cdef int first = True
        for input in inputs:
            if first:
                self.calculate_hash_first(input)
                first = False
            else:
                yield self.calculate_hash_next(input, output)
        if not first:
            yield self.calculate_hash_last(output)

    cpdef calculate_hash_first(self, const unsigned char [:] input):
        crandomx.randomx_calculate_hash_first(self._c_vm, voidptr(input), len(input))

    cpdef calculate_hash_next(self, const unsigned char [:] input, unsigned char output[HASH_SIZE]):
        crandomx.randomx_calculate_hash_next(self._c_vm, voidptr(input), len(input), <void*>output)
        return output

    cpdef calculate_hash_last(self, unsigned char output[HASH_SIZE]):
        crandomx.randomx_calculate_hash_last(self._c_vm, <void*>output)
        return output

def RandomX(const unsigned char [:] key, int full_mem = False, int secure = False, large_pages = False, threads = 0):
    '''
    A convenience function for initializing and returning a RandomX Virtual Machine.

     * @param key is a pointer to memory which contains the key value. Must not be NULL.

    Optional:
     * @param full_mem set FLAG_FULL_MEM to use fast mode requiring 2080 MB of memory.
     * @param secure set FLAG_SECURE to separate writable and executable pages.
     * @param large_pages set FLAG_LARGE_PAGES to allocate memory in large page.
     * @param threads is the number of threads to initialize the dataset with.
    '''
    flags = get_flags()

    if full_mem:
        flags |= FLAG_FULL_MEM

    if secure:
        flags |= FLAG_SECURE

    if large_pages:
        flags |= FLAG_LARGE_PAGES

    cache = Cache(flags, key)

    if flags & FLAG_FULL_MEM:
        dataset = Dataset(flags | FLAG_FULL_MEM)
        dataset.init_parallel(cache, threads = threads)
    else:
        dataset = None

    return VM(flags, cache, dataset)
