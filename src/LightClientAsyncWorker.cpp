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

#include "LightClientAsyncWorker.hpp"
#include "dataset.hpp"
#include "Cache.hpp"

namespace RandomX {

	LightClientAsyncWorker::LightClientAsyncWorker(const Cache& c) : ILightClientAsyncWorker(c), output(nullptr), hasWork(false), 
#ifdef TRACE
		sw(true),
#endif
		workerThread(&LightClientAsyncWorker::runWorker, this) {

	}

	void LightClientAsyncWorker::prepareBlock(addr_t addr) {
#ifdef TRACE
		std::cout << sw.getElapsed() << ": prepareBlock-enter " << addr / CacheLineSize << std::endl;
#endif
		{
			std::lock_guard<std::mutex> lk(mutex);
			startBlock = addr / CacheLineSize;
			blockCount = 1;
			output = currentLine.data();
			hasWork = true;
		}
#ifdef TRACE
		std::cout << sw.getElapsed() << ": prepareBlock-notify " << startBlock << "/" << blockCount << std::endl;
#endif
		notifier.notify_one();
	}

	const uint64_t* LightClientAsyncWorker::getBlock(addr_t addr) {
#ifdef TRACE
		std::cout << sw.getElapsed() << ": getBlock-enter " << addr / CacheLineSize << std::endl;
#endif
		uint32_t currentBlock = addr / CacheLineSize;
		if (currentBlock != startBlock || output != currentLine.data()) {
			initBlock(cache, (uint8_t*)currentLine.data(), currentBlock, RANDOMX_CACHE_ACCESSES / 8);
		}
		else {
			sync();
		}
#ifdef TRACE
		std::cout << sw.getElapsed() << ": getBlock-return " << addr / CacheLineSize << std::endl;
#endif
		return currentLine.data();
	}

	void LightClientAsyncWorker::prepareBlocks(void* out, uint32_t startBlock, uint32_t blockCount) {
#ifdef TRACE
		std::cout << sw.getElapsed() << ": prepareBlocks-enter " << startBlock << "/" << blockCount << std::endl;
#endif
		{
			std::lock_guard<std::mutex> lk(mutex);
			this->startBlock = startBlock;
			this->blockCount = blockCount;
			output = out;
			hasWork = true;
			notifier.notify_one();
		}
	}

	void LightClientAsyncWorker::getBlocks(void* out, uint32_t startBlock, uint32_t blockCount) {
		for (uint32_t i = 0; i < blockCount; ++i) {
			initBlock(cache, (uint8_t*)out + CacheLineSize * i, startBlock + i, RANDOMX_CACHE_ACCESSES / 8);
		}
	}

	void LightClientAsyncWorker::sync() {
		std::unique_lock<std::mutex> lk(mutex);
		notifier.wait(lk, [this] { return !hasWork; });
	}

	void LightClientAsyncWorker::runWorker() {
#ifdef TRACE
		std::cout << sw.getElapsed() << ": runWorker-enter " << std::endl;
#endif
		for (;;) {
			std::unique_lock<std::mutex> lk(mutex);
			notifier.wait(lk, [this] { return hasWork; });
#ifdef TRACE
			std::cout << sw.getElapsed() << ": runWorker-getBlocks " << startBlock << "/" << blockCount << std::endl;
#endif
			//getBlocks(output, startBlock, blockCount);
			initBlock(cache, (uint8_t*)output, startBlock, RANDOMX_CACHE_ACCESSES / 8);
			hasWork = false;
#ifdef TRACE
			std::cout << sw.getElapsed() << ": runWorker-finished " << startBlock << "/" << blockCount << std::endl;
#endif
			lk.unlock();
			notifier.notify_one();
		}
	}
}