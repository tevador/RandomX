#include "../randomx.h"
#include <stdio.h>
#include <iostream>
#include "stopwatch.hpp"
#include <string.h>

class RxWork
{
private:
    randomx_cache *mCache;

public:
    RxWork()
    {
        randomx_flags flags = RANDOMX_FLAG_JIT | RANDOMX_FLAG_ARGON2_SSSE3 | RANDOMX_FLAG_ARGON2_AVX2;
        mCache = randomx_alloc_cache(flags);
        if (mCache == nullptr)
        {
            std::cout << "Cache allocation failed" << std::endl;
            return;
        }
    }
    ~RxWork()
    {
        randomx_release_cache(mCache);
    }
    void PowHash(const char *key, int keyLen, const char *myInput, int inputLen, unsigned char *hash)
    {
        randomx_init_cache(mCache, key, keyLen);
        randomx_vm *myMachine = randomx_create_vm(randomx_get_flags(), mCache, nullptr);
        randomx_calculate_hash(myMachine, myInput, inputLen, hash);
        randomx_destroy_vm(myMachine);
    }
};

int main()
{
    Stopwatch sw(true);
    int i = 0;
    char hash[RANDOMX_HASH_SIZE];
    RxWork work;
    for (i = 0; i < 100; i++)
    {
        char myKey[80] = {0};
        sprintf(myKey, "RandomX example key:%d", i);
        char myInput[80] = {0};
        sprintf(myInput, "RandomX input :%d", i);
        work.PowHash(myKey, sizeof(myKey), myInput, sizeof(myInput), (unsigned char *)hash);
    }

    std::cout << "Memory initialized in " << sw.getElapsed() / 100 << " s" << std::endl;
    for (unsigned i = 0; i < RANDOMX_HASH_SIZE; ++i)
        printf("%02x", hash[i] & 0xff);

    printf("\n");

    return 0;
}
