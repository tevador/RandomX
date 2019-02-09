/*
Copyright (c) 2018 tevador

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

#include <chrono>
#include <cstdint>

class Stopwatch {
public:
	Stopwatch(bool startNow = false) {
		reset();
		if (startNow) {
			start();
		}
	}
	void reset() {
		isRunning = false;
		elapsed = 0;
	}
	void start() {
		if (!isRunning) {
			startMark = std::chrono::high_resolution_clock::now();
			isRunning = true;
		}
	}
	void restart() {
		startMark = std::chrono::high_resolution_clock::now();
		isRunning = true;
		elapsed = 0;
	}
	void stop() {
		if (isRunning) {
			chrono_t endMark = std::chrono::high_resolution_clock::now();
			uint64_t ns = std::chrono::duration_cast<sw_unit>(endMark - startMark).count();
			elapsed += ns;
			isRunning = false;
		}
	}
	double getElapsed() const {
		return getElapsedNanosec() / 1e+9;
	}
private:
	using chrono_t = std::chrono::high_resolution_clock::time_point;
	using sw_unit = std::chrono::nanoseconds;
	chrono_t startMark;
	uint64_t elapsed;
	bool isRunning;

	uint64_t getElapsedNanosec() const {
		uint64_t elns = elapsed;
		if (isRunning) {
			chrono_t endMark = std::chrono::high_resolution_clock::now();
			uint64_t ns = std::chrono::duration_cast<sw_unit>(endMark - startMark).count();
			elns += ns;
		}
		return elns;
	}
};