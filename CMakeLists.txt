# Copyright (c) 2019, The Monero Project
# 
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without modification, are
# permitted provided that the following conditions are met:
# 
# 1. Redistributions of source code must retain the above copyright notice, this list of
#    conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright notice, this list
#    of conditions and the following disclaimer in the documentation and/or other
#    materials provided with the distribution.
# 
# 3. Neither the name of the copyright holder nor the names of its contributors may be
#    used to endorse or promote products derived from this software without specific
#    prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
# THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
# THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 3.5)

project(RandomX)

set(randomx_sources
src/aes_hash.cpp
src/argon2_ref.c
src/argon2_ssse3.c
src/argon2_avx2.c
src/bytecode_machine.cpp
src/cpu.cpp
src/dataset.cpp
src/soft_aes.cpp
src/virtual_memory.c
src/vm_interpreted.cpp
src/allocator.cpp
src/assembly_generator_x86.cpp
src/instruction.cpp
src/randomx.cpp
src/superscalar.cpp
src/vm_compiled.cpp
src/vm_interpreted_light.cpp
src/argon2_core.c
src/blake2_generator.cpp
src/instructions_portable.cpp
src/reciprocal.c
src/virtual_machine.cpp
src/vm_compiled_light.cpp
src/blake2/blake2b.c)

if(NOT ARCH_ID)
  # allow cross compiling
  if(CMAKE_SYSTEM_PROCESSOR STREQUAL "")
    set(CMAKE_SYSTEM_PROCESSOR ${CMAKE_HOST_SYSTEM_PROCESSOR})
  endif()
  string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" ARCH_ID)
endif()

if(NOT ARM_ID)
  set(ARM_ID "${ARCH_ID}")
endif()

if(NOT ARCH)
  set(ARCH "default")
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
  message(STATUS "Setting default build type: ${CMAKE_BUILD_TYPE}")
endif()

include(CheckCXXCompilerFlag)
include(CheckCCompilerFlag)

function(add_flag flag)
  string(REPLACE "-" "_" supported_cxx ${flag}_cxx)
  check_cxx_compiler_flag(${flag} ${supported_cxx})
  if(${${supported_cxx}})
    message(STATUS "Setting CXX flag ${flag}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
  endif()
  string(REPLACE "-" "_" supported_c ${flag}_c)
  check_c_compiler_flag(${flag} ${supported_c})
  if(${${supported_c}})
    message(STATUS "Setting C flag ${flag}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flag}" PARENT_SCOPE)
  endif()
endfunction()

# x86-64
if ((CMAKE_SIZEOF_VOID_P EQUAL 8) AND (ARCH_ID STREQUAL "x86_64" OR ARCH_ID STREQUAL "x86-64" OR ARCH_ID STREQUAL "amd64"))
  list(APPEND randomx_sources
    src/jit_compiler_x86.cpp)

  if(MSVC)
    enable_language(ASM_MASM)
    list(APPEND randomx_sources src/jit_compiler_x86_static.asm)

    set_property(SOURCE src/jit_compiler_x86_static.asm PROPERTY LANGUAGE ASM_MASM)

    set_source_files_properties(src/argon2_avx2.c COMPILE_FLAGS /arch:AVX2)

    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /DRELWITHDEBINFO")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /DRELWITHDEBINFO")

    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/asm/configuration.asm
      COMMAND powershell -ExecutionPolicy Bypass -File h2inc.ps1 ..\\src\\configuration.h > ..\\src\\asm\\configuration.asm SET ERRORLEVEL = 0
      COMMENT "Generating configuration.asm at ${CMAKE_CURRENT_SOURCE_DIR}"
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vcxproj)
    add_custom_target(generate-asm
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/asm/configuration.asm)
  else()
    list(APPEND randomx_sources src/jit_compiler_x86_static.S)

    # cheat because cmake and ccache hate each other
    set_property(SOURCE src/jit_compiler_x86_static.S PROPERTY LANGUAGE C)
    set_property(SOURCE src/jit_compiler_x86_static.S PROPERTY XCODE_EXPLICIT_FILE_TYPE sourcecode.asm)

    if(ARCH STREQUAL "native")
      add_flag("-march=native")
    else()
      # default build has hardware AES enabled (software AES can be selected at runtime)
      add_flag("-maes")
      check_c_compiler_flag(-mssse3 HAVE_SSSE3)
      if(HAVE_SSSE3)
        set_source_files_properties(src/argon2_ssse3.c COMPILE_FLAGS -mssse3)
      endif()
      check_c_compiler_flag(-mavx2 HAVE_AVX2)
      if(HAVE_AVX2)
        set_source_files_properties(src/argon2_avx2.c COMPILE_FLAGS -mavx2)
      endif()
    endif()
  endif()
endif()

# PowerPC
if(ARCH_ID STREQUAL "ppc64" OR ARCH_ID STREQUAL "ppc64le")
  if(ARCH STREQUAL "native")
    add_flag("-mcpu=native")
  endif()
  # PowerPC AES requires ALTIVEC (POWER7+), so it cannot be enabled in the default build
endif()

# ARMv8
if(ARM_ID STREQUAL "aarch64" OR ARM_ID STREQUAL "arm64" OR ARM_ID STREQUAL "armv8-a")
  list(APPEND randomx_sources
    src/jit_compiler_a64_static.S
    src/jit_compiler_a64.cpp)
  # cheat because cmake and ccache hate each other
  set_property(SOURCE src/jit_compiler_a64_static.S PROPERTY LANGUAGE C)
  set_property(SOURCE src/jit_compiler_a64_static.S PROPERTY XCODE_EXPLICIT_FILE_TYPE sourcecode.asm)

  # not sure if this check is needed
  include(CheckIncludeFile)
  check_include_file(asm/hwcap.h HAVE_HWCAP)
  if(HAVE_HWCAP)
    add_definitions(-DHAVE_HWCAP)
  endif()

  if(ARCH STREQUAL "native")
    add_flag("-march=native")
  else()
    # default build has hardware AES enabled (software AES can be selected at runtime)
    add_flag("-march=armv8-a+crypto")
  endif()
endif()

# RISC-V
if(ARCH_ID STREQUAL "riscv64")
  list(APPEND randomx_sources
    src/aes_hash_rv64_vector.cpp
    src/aes_hash_rv64_zvkned.cpp
    src/jit_compiler_rv64_static.S
    src/jit_compiler_rv64.cpp
    src/jit_compiler_rv64_vector.cpp
    src/jit_compiler_rv64_vector_static.S
    src/cpu_rv64.S)

  # cheat because cmake and ccache hate each other
  set_property(SOURCE src/jit_compiler_rv64_static.S PROPERTY LANGUAGE C)
  set_property(SOURCE src/jit_compiler_rv64_vector_static.S PROPERTY LANGUAGE C)
  set_property(SOURCE src/cpu_rv64.S PROPERTY LANGUAGE C)

  # default build uses the RV64GC baseline
  set(RVARCH "rv64gc")

  # for native builds, enable Zba and Zbb if supported by the CPU
  if(ARCH STREQUAL "native")
    enable_language(ASM)

    try_run(RANDOMX_VECTOR_RUN_FAIL
        RANDOMX_VECTOR_COMPILE_OK
        ${CMAKE_CURRENT_BINARY_DIR}/
        ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/riscv64_vector.s
        COMPILE_DEFINITIONS "-march=rv64gcv")

    if (RANDOMX_VECTOR_COMPILE_OK AND NOT RANDOMX_VECTOR_RUN_FAIL)
        set(RVARCH_V ON)
        message(STATUS "RISC-V vector extension detected")
    else()
        set(RVARCH_V OFF)
    endif()

    try_run(RANDOMX_ZICBOP_RUN_FAIL
        RANDOMX_ZICBOP_COMPILE_OK
        ${CMAKE_CURRENT_BINARY_DIR}/
        ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/riscv64_zicbop.s
        COMPILE_DEFINITIONS "-march=rv64gc_zicbop")

    if (RANDOMX_ZICBOP_COMPILE_OK AND NOT RANDOMX_ZICBOP_RUN_FAIL)
        set(RVARCH_ZICBOP ON)
        message(STATUS "RISC-V zicbop extension detected")
    else()
        set(RVARCH_ZICBOP OFF)
    endif()

    try_run(RANDOMX_ZBA_RUN_FAIL
        RANDOMX_ZBA_COMPILE_OK
        ${CMAKE_CURRENT_BINARY_DIR}/
        ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/riscv64_zba.s
        COMPILE_DEFINITIONS "-march=rv64gc_zba")

    if (RANDOMX_ZBA_COMPILE_OK AND NOT RANDOMX_ZBA_RUN_FAIL)
        set(RVARCH_ZBA ON)
        message(STATUS "RISC-V zba extension detected")
    else()
        set(RVARCH_ZBA OFF)
    endif()

    try_run(RANDOMX_ZBB_RUN_FAIL
        RANDOMX_ZBB_COMPILE_OK
        ${CMAKE_CURRENT_BINARY_DIR}/
        ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/riscv64_zbb.s
        COMPILE_DEFINITIONS "-march=rv64gc_zbb")

    if (RANDOMX_ZBB_COMPILE_OK AND NOT RANDOMX_ZBB_RUN_FAIL)
        set(RVARCH_ZBB ON)
        message(STATUS "RISC-V zbb extension detected")
    else()
        set(RVARCH_ZBB OFF)
    endif()

    try_run(RANDOMX_ZVKB_RUN_FAIL
        RANDOMX_ZVKB_COMPILE_OK
        ${CMAKE_CURRENT_BINARY_DIR}/
        ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/riscv64_zvkb.s
        COMPILE_DEFINITIONS "-march=rv64gcv_zvkb")

    if (RANDOMX_ZVKB_COMPILE_OK AND NOT RANDOMX_ZVKB_RUN_FAIL)
        set(RVARCH_ZVKB ON)
        message(STATUS "RISC-V zvkb extension detected")
    else()
        set(RVARCH_ZVKB OFF)
    endif()

    try_run(RANDOMX_ZVKNED_RUN_FAIL
        RANDOMX_ZVKNED_COMPILE_OK
        ${CMAKE_CURRENT_BINARY_DIR}/
        ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/riscv64_zvkned.s
        COMPILE_DEFINITIONS "-march=rv64gcv_zvkned")

    if (RANDOMX_ZVKNED_COMPILE_OK AND NOT RANDOMX_ZVKNED_RUN_FAIL)
        set(RVARCH_ZVKNED ON)
        message(STATUS "RISC-V zvkned extension detected")
    else()
        set(RVARCH_ZVKNED OFF)
    endif()

    # for native builds, enable Zba and Zbb if supported by the CPU
    if (ARCH STREQUAL "native")
        if (RVARCH_V)
            set(RVARCH "${RVARCH}v")
        endif()
        if (RVARCH_ZICBOP)
            set(RVARCH "${RVARCH}_zicbop")
        endif()
        if (RVARCH_ZBA)
            set(RVARCH "${RVARCH}_zba")
        endif()
        if (RVARCH_ZBB)
            set(RVARCH "${RVARCH}_zbb")
        endif()
        if (RVARCH_ZVKB)
            set(RVARCH "${RVARCH}_zvkb")
        endif()
        if (RVARCH_ZVKNED)
            set(RVARCH "${RVARCH}_zvkned")
        endif()
    endif()
  endif()

  add_flag("-march=${RVARCH}")

  set(RV64_VECTOR_FILE_ARCH "rv64gcv")

  if (ARCH STREQUAL "native")
    if (RVARCH_ZICBOP)
      set(RV64_VECTOR_FILE_ARCH "${RV64_VECTOR_FILE_ARCH}_zicbop")
    endif()
    if (RVARCH_ZBA)
      set(RV64_VECTOR_FILE_ARCH "${RV64_VECTOR_FILE_ARCH}_zba")
    endif()
    if (RVARCH_ZBB)
      set(RV64_VECTOR_FILE_ARCH "${RV64_VECTOR_FILE_ARCH}_zbb")
    endif()
    if (RVARCH_ZVKB)
      set(RV64_VECTOR_FILE_ARCH "${RV64_VECTOR_FILE_ARCH}_zvkb")
    endif()
  endif()

  set_source_files_properties(src/jit_compiler_rv64_vector_static.S PROPERTIES COMPILE_FLAGS "-march=${RV64_VECTOR_FILE_ARCH}_zvkned")
  set_source_files_properties(src/aes_hash_rv64_vector.cpp PROPERTIES COMPILE_FLAGS "-O3 -march=${RV64_VECTOR_FILE_ARCH}")
  set_source_files_properties(src/aes_hash_rv64_zvkned.cpp PROPERTIES COMPILE_FLAGS "-O3 -march=${RV64_VECTOR_FILE_ARCH}_zvkned")
endif()

set(RANDOMX_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/src" CACHE STRING "RandomX Include path")

add_library(randomx ${randomx_sources})

if(TARGET generate-asm)
  add_dependencies(randomx generate-asm)
endif()

set_property(TARGET randomx PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET randomx PROPERTY CXX_STANDARD 11)
set_property(TARGET randomx PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET randomx PROPERTY PUBLIC_HEADER src/randomx.h)

include(GNUInstallDirs)
install(TARGETS randomx
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_executable(randomx-tests
  src/tests/tests.cpp)
target_link_libraries(randomx-tests
  PRIVATE randomx)
set_property(TARGET randomx-tests PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET randomx-tests PROPERTY CXX_STANDARD 11)

add_executable(randomx-codegen
  src/tests/code-generator.cpp)
target_link_libraries(randomx-codegen
  PRIVATE randomx)

set_property(TARGET randomx-codegen PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET randomx-codegen PROPERTY CXX_STANDARD 11)

if(NOT Threads_FOUND AND UNIX AND NOT APPLE)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads)
endif()

add_executable(randomx-benchmark
  src/tests/benchmark.cpp
  src/tests/affinity.cpp)
target_link_libraries(randomx-benchmark
  PRIVATE randomx
  PRIVATE ${CMAKE_THREAD_LIBS_INIT})

include(CheckCXXSourceCompiles)
check_cxx_source_compiles("
#include <cstdint>
#include <atomic>
int main() {
  std::atomic<uint64_t> a;
  a.is_lock_free();
}" HAVE_CXX_ATOMICS)

if(NOT HAVE_CXX_ATOMICS)
  target_link_libraries(randomx-benchmark
    PRIVATE "atomic")
endif()

check_cxx_source_compiles("
#include <stdlib.h>
int main() {
  void* p;
  return posix_memalign(&p, 64, 64);
}" HAVE_POSIX_MEMALIGN)

set_property(TARGET randomx-benchmark PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET randomx-benchmark PROPERTY CXX_STANDARD 11)
