# setup.py
from setuptools import setup, Extension

ext_modules = [
    Extension(
        'sensor',
        sources=['sensor.c'],
        extra_compile_args=[],
    )
]

setup(
    name='sensor',
    version='0.1',
    description='Lightweight IoT sensor simulator C extension',
    ext_modules=ext_modules,
)
