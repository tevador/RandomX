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

#if defined(_WIN32) || defined(__CYGWIN__)
#include <windows.h>
#else
#define _GNU_SOURCE	1	/* needed for MAP_ANONYMOUS on older platforms */
#ifdef __APPLE__
#include <mach/vm_statistics.h>
#include <TargetConditionals.h>
#include <AvailabilityMacros.h>
# if TARGET_OS_OSX
#  define USE_PTHREAD_JIT_WP	1
#  include <pthread.h>
#  include <sys/utsname.h>
#  include <stdio.h>
# endif
#endif
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>
#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS MAP_ANON
#endif
#define PAGE_READONLY PROT_READ
#define PAGE_READWRITE (PROT_READ | PROT_WRITE)
#define PAGE_EXECUTE_READ (PROT_READ | PROT_EXEC)
#define PAGE_EXECUTE_READWRITE (PROT_READ | PROT_WRITE | PROT_EXEC)
#endif

#include "virtual_memory.h"

#if defined(USE_PTHREAD_JIT_WP) && defined(MAC_OS_VERSION_11_0) \
	&& MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_VERSION_11_0
static int MacOSchecked, MacOSver;
/* This function is used implicitly by clang's __builtin_available() checker.
 * When cross-compiling, the library containing this function doesn't exist,
 * and linking will fail because the symbol is unresolved. The function here
 * is a quick and dirty hack to get close enough to identify MacOSX 11.0.
 */
static int32_t __isOSVersionAtLeast(int32_t major, int32_t minor, int32_t subminor) {
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
#endif


#if defined(_WIN32) || defined(__CYGWIN__)
#define Fail(func)	do  {*errfunc = func; return GetLastError();} while(0)
int setPrivilege(const char* pszPrivilege, BOOL bEnable, char **errfunc) {
	HANDLE           hToken;
	TOKEN_PRIVILEGES tp;
	BOOL             status;
	DWORD            error = 0;

	*errfunc = NULL;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		Fail("OpenProcessToken");

	if (!LookupPrivilegeValue(NULL, pszPrivilege, &tp.Privileges[0].Luid)) {
		*errfunc = "LookupPrivilegeValue";
		error = GetLastError();
		goto out;
	}

	tp.PrivilegeCount = 1;

	if (bEnable)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	status = AdjustTokenPrivileges(hToken, FALSE, &tp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

	error = GetLastError();
	if (!status || (error != ERROR_SUCCESS)) {
		*errfunc = "AdjustTokenPrivileges";
		goto out;
	}

out:
	if (!CloseHandle(hToken)) {
		if (*errfunc == NULL) {
			*errfunc = "CloseHandle";
			error = GetLastError();
		}
	}
	return error;
}
#else
#define Fail(func)	do  {*errfunc = func; return errno;} while(0)
#endif

void* allocMemoryPages(size_t bytes) {
	void* mem;
#if defined(_WIN32) || defined(__CYGWIN__)
	mem = VirtualAlloc(NULL, bytes, MEM_COMMIT, PAGE_READWRITE);
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
	mem = mmap(NULL, bytes, PAGE_READWRITE | RESERVED_FLAGS | PEXTRA, MAP_ANONYMOUS | MAP_PRIVATE | MEXTRA, -1, 0);
	if (mem == MAP_FAILED)
		mem = NULL;
#if defined(USE_PTHREAD_JIT_WP) && defined(MAC_OS_VERSION_11_0) \
	&& MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_VERSION_11_0
	if (__builtin_available(macOS 11.0, *)) {
		pthread_jit_write_protect_np(0);
	}
#endif
#endif
	return mem;
}

static inline int pageProtect(void* ptr, size_t bytes, int rules, char **errfunc) {
#if defined(_WIN32) || defined(__CYGWIN__)
	DWORD oldp;
	if (!VirtualProtect(ptr, bytes, (DWORD)rules, &oldp)) {
		Fail("VirtualProtect");
	}
#else
	if (-1 == mprotect(ptr, bytes, rules))
		Fail("mprotect");
#endif
	return 0;
}

void setPagesRW(void* ptr, size_t bytes) {
	char *errfunc;
#if defined(USE_PTHREAD_JIT_WP) && defined(MAC_OS_VERSION_11_0) \
	&& MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_VERSION_11_0
	if (__builtin_available(macOS 11.0, *)) {
		pthread_jit_write_protect_np(0);
	} else {
		pageProtect(ptr, bytes, PAGE_READWRITE, &errfunc);
	}
#else
	pageProtect(ptr, bytes, PAGE_READWRITE, &errfunc);
#endif
}

void setPagesRX(void* ptr, size_t bytes) {
	char *errfunc;
#if defined(USE_PTHREAD_JIT_WP) && defined(MAC_OS_VERSION_11_0) \
	&& MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_VERSION_11_0
	if (__builtin_available(macOS 11.0, *)) {
		pthread_jit_write_protect_np(1);
		__builtin___clear_cache((char*)ptr, ((char*)ptr) + bytes);
	} else {
		pageProtect(ptr, bytes, PAGE_EXECUTE_READ, &errfunc);
	}
#else
	pageProtect(ptr, bytes, PAGE_EXECUTE_READ, &errfunc);
#endif
}

void setPagesRWX(void* ptr, size_t bytes) {
	char *errfunc;
	pageProtect(ptr, bytes, PAGE_EXECUTE_READWRITE, &errfunc);
}

void* allocLargePagesMemory(size_t bytes) {
	void* mem;
	char *errfunc;
#if defined(_WIN32) || defined(__CYGWIN__)
	if (setPrivilege("SeLockMemoryPrivilege", 1, &errfunc))
		return NULL;
	size_t pageMinimum = GetLargePageMinimum();
	if (!pageMinimum) {
		errfunc = "No large pages";
		return NULL;
	}
	mem = VirtualAlloc(NULL, alignSize(bytes, pageMinimum), MEM_COMMIT | MEM_RESERVE | MEM_LARGE_PAGES, PAGE_READWRITE);
#else
#ifdef __APPLE__
	mem = mmap(NULL, bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, VM_FLAGS_SUPERPAGE_SIZE_2MB, 0);
#elif defined(__FreeBSD__)
	mem = mmap(NULL, bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_ALIGNED_SUPER, -1, 0);
#elif defined(__OpenBSD__) || defined(__NetBSD__)
	mem = MAP_FAILED; // OpenBSD does not support huge pages
#else
	mem = mmap(NULL, bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_POPULATE, -1, 0);
#endif
	if (mem == MAP_FAILED)
		mem = NULL;
#endif
	return mem;
}

void freePagedMemory(void* ptr, size_t bytes) {
#if defined(_WIN32) || defined(__CYGWIN__)
	VirtualFree(ptr, 0, MEM_RELEASE);
#else
	munmap(ptr, bytes);
#endif
}
