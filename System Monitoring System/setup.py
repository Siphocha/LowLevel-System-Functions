from setuptools import setup, Extension
import sys

sensor_module = Extension(
    'sensor_simulator',
    sources=['sensor_simulator.c'],
    extra_compile_args=['-std=c99'] if sys.platform != 'win32' else [],
    libraries=['m'] if sys.platform != 'win32' else []  # Link math library for Unix-like systems
)

setup(
    name='iot sensor simulator',
    version='1.0.0',
    description='Lightweight IoT Sensor Data Simulator',
    ext_modules=[sensor_module],
    python_requires='>=3.6',
    author='IoT Monitoring System',
    author_email='iot@example.com',
    classifiers=[
        'Development Status: Beta',
        'Intended Audience: Developers',
        'Software Development:  Embedded Systems',
        'Programming Language : C',
    ],
)