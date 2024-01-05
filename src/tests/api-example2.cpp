#include "../randomx.h"
#include <iostream>
#include <iomanip>
#include <thread>

int main()
{
	const char myKey[] = "RandomX example key";
	const char myInput[] = "RandomX example input";
	char hash[RANDOMX_HASH_SIZE];

	randomx_flags flags = randomx_get_flags();
	// flags |= RANDOMX_FLAG_LARGE_PAGES;
	flags |= RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_JIT | RANDOMX_FLAG_ARGON2_SSSE3 | RANDOMX_FLAG_ARGON2_AVX2;

	randomx_cache *myCache = randomx_alloc_cache(flags);
	if (myCache == nullptr)
	{
		std::cout << "Cache allocation failed" << std::endl;
		return 1;
	}
	randomx_init_cache(myCache, myKey, sizeof myKey);

	randomx_dataset *myDataset = randomx_alloc_dataset(flags);
	if (myDataset == nullptr)
	{
		std::cout << "Dataset allocation failed" << std::endl;
		return 1;
	}

	auto datasetItemCount = randomx_dataset_item_count();
	std::thread t1(&randomx_init_dataset, myDataset, myCache, 0, datasetItemCount / 2);
	std::thread t2(&randomx_init_dataset, myDataset, myCache, datasetItemCount / 2, datasetItemCount - datasetItemCount / 2);
	t1.join();
	t2.join();
	randomx_release_cache(myCache);

	randomx_vm *myMachine = randomx_create_vm(flags, nullptr, myDataset);
	if (myMachine == nullptr)
	{
		std::cout << "Failed to create a virtual machine" << std::endl;
		return 1;
	}

	randomx_calculate_hash(myMachine, &myInput, sizeof myInput, hash);

	randomx_destroy_vm(myMachine);
	randomx_release_dataset(myDataset);

	for (unsigned i = 0; i < RANDOMX_HASH_SIZE; ++i)
		std::cout << std::hex << std::setw(2) << std::setfill('0') << ((int)hash[i] & 0xff);

	std::cout << std::endl;

	return 0;
}