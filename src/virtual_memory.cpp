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

#include "virtual_memory.hpp"

#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#else
#ifdef __APPLE__
#include <mach/vm_statistics.h>
#endif
#include <sys/types.h>
#include <sys/mman.h>
#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS MAP_ANON
#endif
#endif

#ifdef _WIN32
std::string getErrorMessage(const char* function) {
	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
	std::string message(messageBuffer, size);
	LocalFree(messageBuffer);
	return std::string(function) + std::string(": ") + message;
}

void setPrivilege(const char* pszPrivilege, BOOL bEnable) {
	HANDLE           hToken;
	TOKEN_PRIVILEGES tp;
	BOOL             status;
	DWORD            error;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		throw std::runtime_error(getErrorMessage("OpenProcessToken"));

	if (!LookupPrivilegeValue(NULL, pszPrivilege, &tp.Privileges[0].Luid))
		throw std::runtime_error(getErrorMessage("LookupPrivilegeValue"));

	tp.PrivilegeCount = 1;

	if (bEnable)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	status = AdjustTokenPrivileges(hToken, FALSE, &tp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

	error = GetLastError();
	if (!status || (error != ERROR_SUCCESS))
		throw std::runtime_error(getErrorMessage("AdjustTokenPrivileges"));

	if (!CloseHandle(hToken))
		throw std::runtime_error(getErrorMessage("CloseHandle"));
}
#endif

void* allocExecutableMemory(std::size_t bytes) {
	void* mem;
#ifdef _WIN32
	mem = VirtualAlloc(nullptr, bytes, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (mem == nullptr)
		throw std::runtime_error(getErrorMessage("allocExecutableMemory - VirtualAlloc"));
#else
	mem = mmap(nullptr, bytes, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (mem == MAP_FAILED)
		throw std::runtime_error("allocExecutableMemory - mmap failed");
#endif
	return mem;
}

constexpr std::size_t align(std::size_t pos, std::size_t align) {
	return ((pos - 1) / align + 1) * align;
}

void* allocLargePagesMemory(std::size_t bytes) {
	void* mem;
#ifdef _WIN32
	setPrivilege("SeLockMemoryPrivilege", 1);
	auto pageMinimum = GetLargePageMinimum();
	if (pageMinimum > 0)
		mem = VirtualAlloc(NULL, align(bytes, pageMinimum), MEM_COMMIT | MEM_RESERVE | MEM_LARGE_PAGES, PAGE_READWRITE);
	else
		throw std::runtime_error("allocLargePagesMemory - Large pages are not supported");
	if (mem == nullptr)
		throw std::runtime_error(getErrorMessage("allocLargePagesMemory - VirtualAlloc"));
#else
#ifdef __APPLE__
	mem = mmap(nullptr, bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, VM_FLAGS_SUPERPAGE_SIZE_2MB, 0);
#else
	mem = mmap(nullptr, bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_POPULATE, -1, 0);
#endif
	if (mem == MAP_FAILED)
		throw std::runtime_error("allocLargePagesMemory - mmap failed");
#endif
	return mem;
}

void freePagedMemory(void* ptr, std::size_t bytes) {
#ifdef _WIN32
	VirtualFree(ptr, 0, MEM_RELEASE);
#else
	munmap(ptr, bytes);
#endif
}
