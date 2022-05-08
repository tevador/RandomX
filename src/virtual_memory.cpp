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
#include <TargetConditionals.h>
#include <AvailabilityMacros.h>
# if TARGET_OS_OSX
#  define USE_PTHREAD_JIT_WP	1
#  include <pthread.h>
#  include <sys/utsname.h>
# endif
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

#if defined(USE_PTHREAD_JIT_WP) && defined(MAC_OS_VERSION_11_0) \
	&& MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_VERSION_11_0
extern "C" {
static int MacOSchecked, MacOSver;
/* This function is used implicitly by clang's __builtin_available() checker.
 * When cross-compiling, the library containing this function doesn't exist,
 * and linking will fail because the symbol is unresolved. The function here
 * is a quick and dirty hack to get close enough to identify MacOSX 11.0.
 *
 * It must be global otherwise clang refuses to emit its code. We change it
 * to a static later using nmedit, to prevent interfering with anyone else's
 * code that might rely on the real function. See the custom command in
 * CMakeLists.txt for the nmedit invocation.
 */
int32_t __isOSVersionAtLeast(int32_t major, int32_t minor, int32_t subminor) {
	if (!MacOSchecked) {
	    struct utsname ut;
		int mmaj, mmin;
		uname(&ut);
		sscanf(ut.release, "%d.%d", &mmaj, &mmin);
		// The utsname release version is 9 greater than the canonical OS version
		mmaj -= 9;
		MacOSver = (mmaj << 8) | mmin;
		MacOSchecked = 1;
	}
	return MacOSver >= ((major << 8) | minor);
}
}
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
	#if defined(__NetBSD__)
		#define RESERVED_FLAGS PROT_MPROTECT(PROT_EXEC)
	#else
		#define RESERVED_FLAGS 0
	#endif
	#ifdef USE_PTHREAD_JIT_WP
		#define MEXTRA MAP_JIT
		#define PEXTRA	PROT_EXEC
	#else
		#define MEXTRA 0
		#define PEXTRA	0
	#endif
	mem = mmap(nullptr, bytes, PAGE_READWRITE | RESERVED_FLAGS | PEXTRA, MAP_ANONYMOUS | MAP_PRIVATE | MEXTRA, -1, 0);
	if (mem == MAP_FAILED)
		throw std::runtime_error("allocMemoryPages - mmap failed");
#if defined(USE_PTHREAD_JIT_WP) && defined(MAC_OS_VERSION_11_0) \
	&& MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_VERSION_11_0
	if (__builtin_available(macOS 11.0, *)) {
		pthread_jit_write_protect_np(false);
	}
#endif
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
#if defined(USE_PTHREAD_JIT_WP) && defined(MAC_OS_VERSION_11_0) \
	&& MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_VERSION_11_0
	if (__builtin_available(macOS 11.0, *)) {
		pthread_jit_write_protect_np(false);
	} else {
		pageProtect(ptr, bytes, PAGE_READWRITE);
	}
#else
	pageProtect(ptr, bytes, PAGE_READWRITE);
#endif
}

void setPagesRX(void* ptr, std::size_t bytes) {
#if defined(USE_PTHREAD_JIT_WP) && defined(MAC_OS_VERSION_11_0) \
	&& MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_VERSION_11_0
	if (__builtin_available(macOS 11.0, *)) {
		pthread_jit_write_protect_np(true);
	} else {
		pageProtect(ptr, bytes, PAGE_EXECUTE_READ);
	}
#else
	pageProtect(ptr, bytes, PAGE_EXECUTE_READ);
#endif
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
#elif defined(__OpenBSD__) || defined(__NetBSD__)
	mem = MAP_FAILED; // OpenBSD does not support huge pages
#else
	mem = mmap(nullptr, bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_POPULATE, -1, 0);
#endif
	if (mem == MAP_FAILED)
		throw std::runtime_error("allocLargePagesMemory - mmap failed");
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
