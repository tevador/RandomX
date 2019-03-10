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

//#define TRACE
#include "common.hpp"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <array>
#ifdef TRACE
#include "Stopwatch.hpp"
#include <iostream>
#endif

namespace RandomX {

	using DatasetLine = std::array<uint64_t, CacheLineSize / sizeof(uint64_t)>;

	class LightClientAsyncWorker : public ILightClientAsyncWorker {
	public:
		LightClientAsyncWorker(const Cache&);
		void prepareBlock(addr_t) final;
		void prepareBlocks(void* out, uint32_t startBlock, uint32_t blockCount) final;
		const uint64_t* getBlock(addr_t) final;
		void getBlocks(void* out, uint32_t startBlock, uint32_t blockCount) final;
		void sync() final;
	private:
		void runWorker();
		std::condition_variable notifier;
		std::mutex mutex;
		alignas(16) DatasetLine currentLine;
		void* output;
		uint32_t startBlock, blockCount;
		bool hasWork;
#ifdef TRACE
		Stopwatch sw;
#endif
		std::thread workerThread;
	};
}