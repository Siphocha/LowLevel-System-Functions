from setuptools import setup, Extension
import platform

# Platform-specific compilation settings
if platform.system() == "Windows":
    extra_compile_args = []
    extra_link_args = []
else:
    extra_compile_args = ["-std=c99"]
    extra_link_args = []

module = Extension(
    'sensor_simulator',
    sources=['sensor_simulator.c'],
    extra_compile_args=extra_compile_args,
    extra_link_args=extra_link_args,
)

setup(
    name='sensor_simulator',
    version='1.0.0',
    description='Lightweight IoT Sensor Simulator - C Extension',
    ext_modules=[module],
    zip_safe=False,
    python_requires='>=3.6',
    author='IoT Monitoring System',
    author_email='example@example.com',
    url='https://github.com/example/iot-monitoring',
    classifiers=[
        'Development Status :: 4 - Beta',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 3',
        'Programming Language :: C',
        'Topic :: System :: Monitoring',
    ],
)