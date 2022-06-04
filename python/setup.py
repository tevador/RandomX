from setuptools import setup, Extension
from Cython.Build import cythonize

setup(
    name='RandomX',
    version='3.5',
    ext_modules=cythonize(
        Extension(
            "randomx", ["randomx.pyx"],
            libraries=["randomx"],
            language="c++",
        ),
        compiler_directives=dict(
            language_level="3",
            embedsignature=True
        )
    )
)
