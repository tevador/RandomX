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

	template<bool softAes>
	LightClientAsyncWorker<softAes>::LightClientAsyncWorker(const Cache* c) : ILightClientAsyncWorker(c), output(nullptr), hasWork(false), 
#ifdef TRACE
		sw(true),
#endif
		workerThread(&LightClientAsyncWorker::runWorker, this) {

	}

	template<bool softAes>
	void LightClientAsyncWorker<softAes>::prepareBlock(addr_t addr) {
#ifdef TRACE
		std::cout << sw.getElapsed() << ": prepareBlock-enter " << addr << std::endl;
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
		notifier.notify_all();
	}

	template<bool softAes>
	const uint64_t* LightClientAsyncWorker<softAes>::getBlock(addr_t addr) {
		uint32_t currentBlock = addr / CacheLineSize;
		if (currentBlock != startBlock || output != currentLine.data()) {
			initBlock<softAes>(cache->getCache(), (uint8_t*)currentLine.data(), currentBlock, cache->getKeys());
		}
		else {
			sync();
		}
		return currentLine.data();
	}

	template<bool softAes>
	void LightClientAsyncWorker<softAes>::prepareBlocks(void* out, uint32_t startBlock, uint32_t blockCount) {
#ifdef TRACE
		std::cout << sw.getElapsed() << ": prepareBlocks-enter " << startBlock << "/" << blockCount << std::endl;
#endif
		{
			std::lock_guard<std::mutex> lk(mutex);
			this->startBlock = startBlock;
			this->blockCount = blockCount;
			output = out;
			hasWork = true;
		}
		notifier.notify_all();
	}

	template<bool softAes>
	void LightClientAsyncWorker<softAes>::getBlocks(void* out, uint32_t startBlock, uint32_t blockCount) {
		for (uint32_t i = 0; i < blockCount; ++i) {
			initBlock<softAes>(cache->getCache(), (uint8_t*)out + CacheLineSize * i, startBlock + i, cache->getKeys());
		}
	}

	template<bool softAes>
	void LightClientAsyncWorker<softAes>::sync() {
		std::unique_lock<std::mutex> lk(mutex);
		notifier.wait(lk, [this] { return !hasWork; });
	}

	template<bool softAes>
	void LightClientAsyncWorker<softAes>::runWorker() {
#ifdef TRACE
		std::cout << sw.getElapsed() << ": runWorker-enter " << std::endl;
#endif
		for (;;) {
			std::unique_lock<std::mutex> lk(mutex);
			notifier.wait(lk, [this] { return hasWork; });
			getBlocks(output, startBlock, blockCount);
			hasWork = false;
			lk.unlock();
			notifier.notify_all();
		}
	}

	template class LightClientAsyncWorker<true>;
	template class LightClientAsyncWorker<false>;
}