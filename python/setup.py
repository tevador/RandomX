from setuptools import setup, Extension
from Cython.Build import cythonize

import os, platform

machine = platform.machine()

sources = '''
aes_hash.cpp
argon2_ref.c
argon2_ssse3.c
argon2_avx2.c
bytecode_machine.cpp
cpu.cpp
dataset.cpp
soft_aes.cpp
virtual_memory.c
vm_interpreted.cpp
allocator.cpp
assembly_generator_x86.cpp
instruction.cpp
randomx.cpp
superscalar.cpp
vm_compiled.cpp
vm_interpreted_light.cpp
argon2_core.c
blake2_generator.cpp
instructions_portable.cpp
reciprocal.c
virtual_machine.cpp
vm_compiled_light.cpp
blake2/blake2b.c
'''.strip().split('\n')
compile_flags = ['-march=native','-std=c++11','-fpic']

if machine in ['i386', 'i686', 'x86_64']:
    sources.extend(['jit_compiler_x86.cpp', 'jit_compiler_x86_static.S'])
elif machine in ['aarch64_be', 'aarch64', 'armv8b', 'armv8l']:
    sources.extend(['jit_compiler_a64_static.s', 'jit_compiler_a64.cpp'])
    compile_flags.append('-DHAVE_HWCAP')

sources = [os.path.join('..','src',source) for source in sources]

setup(
    name='RandomX',
    version='3.5',
    ext_modules=cythonize(
        Extension(
            'randomx', [
                'randomx.pyx',
                *[source for source in sources if source.endswith('.cpp') or source.endswith('.c')]
            ],
            extra_objects=[source for source in sources if source.endswith('.S')],
            extra_compile_args=compile_flags,
            #libraries=['randomx'],
            language='c++',
        ),
        compiler_directives=dict(
            language_level='3',
            embedsignature=True
        )
    )
)
