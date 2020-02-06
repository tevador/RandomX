/*
Copyright (c) 2018-2019, tevador <tevador@gmail.com>

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	* Neither the name of the copyright holder nor the
	  names of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "virtual_memory.hpp"

#include <stdexcept>

#if defined(_WIN32) || defined(__CYGWIN__)
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
#define PAGE_READONLY PROT_READ
#define PAGE_READWRITE (PROT_READ | PROT_WRITE)
#define PAGE_EXECUTE_READ (PROT_READ | PROT_EXEC)
#define PAGE_EXECUTE_READWRITE (PROT_READ | PROT_WRITE | PROT_EXEC)
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
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

void* allocMemoryPages(std::size_t bytes) {
	void* mem;
#if defined(_WIN32) || defined(__CYGWIN__)
	mem = VirtualAlloc(nullptr, bytes, MEM_COMMIT, PAGE_READWRITE);
	if (mem == nullptr)
		throw std::runtime_error(getErrorMessage("allocMemoryPages - VirtualAlloc"));
#else
	mem = mmap(nullptr, bytes, PAGE_READWRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (mem == MAP_FAILED)
		throw std::runtime_error("allocMemoryPages - mmap failed");
#endif
	return mem;
}

static inline void pageProtect(void* ptr, std::size_t bytes, int rules) {
#if defined(_WIN32) || defined(__CYGWIN__)
	DWORD oldp;
	if (!VirtualProtect(ptr, bytes, (DWORD)rules, &oldp)) {
		throw std::runtime_error(getErrorMessage("VirtualProtect"));
	}
#else
	if (-1 == mprotect(ptr, bytes, rules))
		throw std::runtime_error("mprotect failed");
#endif
}

void setPagesRW(void* ptr, std::size_t bytes) {
	pageProtect(ptr, bytes, PAGE_READWRITE);
}

void setPagesRX(void* ptr, std::size_t bytes) {
	pageProtect(ptr, bytes, PAGE_EXECUTE_READ);
}

void setPagesRWX(void* ptr, std::size_t bytes) {
	pageProtect(ptr, bytes, PAGE_EXECUTE_READWRITE);
}

void* allocLargePagesMemory(std::size_t bytes) {
	void* mem;
#if defined(_WIN32) || defined(__CYGWIN__)
	setPrivilege("SeLockMemoryPrivilege", 1);
	auto pageMinimum = GetLargePageMinimum();
	if (pageMinimum > 0)
		mem = VirtualAlloc(NULL, alignSize(bytes, pageMinimum), MEM_COMMIT | MEM_RESERVE | MEM_LARGE_PAGES, PAGE_READWRITE);
	else
		throw std::runtime_error("allocLargePagesMemory - Large pages are not supported");
	if (mem == nullptr)
		throw std::runtime_error(getErrorMessage("allocLargePagesMemory - VirtualAlloc"));
#else
#ifdef __APPLE__
	mem = mmap(nullptr, bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, VM_FLAGS_SUPERPAGE_SIZE_2MB, 0);
#elif defined(__FreeBSD__)
	mem = mmap(nullptr, bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_ALIGNED_SUPER, -1, 0);
#elif defined(__OpenBSD__)
	mem = MAP_FAILED; // OpenBSD does not support huge pages
#else
	mem = mmap(nullptr, bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_POPULATE, -1, 0);
#endif
	if (mem == MAP_FAILED)
		throw std::runtime_error("allocLargePagesMemory - mmap failed");
#endif
	return mem;
}

#if !defined(MAP_HUGE_1GB) && defined(MAP_HUGE_SHIFT)
#define MAP_HUGE_1GB (30 << MAP_HUGE_SHIFT)
#endif

void* allocMonsterPagesMemory(std::size_t bytes) {
	void* mem;
#ifdef MAP_HUGE_1GB
	mem = mmap(nullptr, bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_POPULATE | MAP_HUGE_1GB, -1, 0);
	if (mem == MAP_FAILED)
		throw std::runtime_error("allocMonsterPagesMemory - mmap failed");
#else
	throw std::runtime_error("allocMonsterPagesMemory - Monster pages are not supported");
#endif
	return mem;
}

void freePagedMemory(void* ptr, std::size_t bytes) {
#if defined(_WIN32) || defined(__CYGWIN__)
	VirtualFree(ptr, 0, MEM_RELEASE);
#else
	munmap(ptr, bytes);
#endif
}
