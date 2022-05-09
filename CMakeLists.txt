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
if(ARCH_ID STREQUAL "x86_64" OR ARCH_ID STREQUAL "x86-64" OR ARCH_ID STREQUAL "amd64")
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
set_property(TARGET randomx-benchmark PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET randomx-benchmark PROPERTY CXX_STANDARD 11)
