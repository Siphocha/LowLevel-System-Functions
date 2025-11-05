from distutils.core import setup, Extension

module = Extension('sensor_simulator',
                  sources=['sensor_simulator.c'],
                  libraries=['m'])  # Link math library

setup(name='sensor_simulator',
      version='1.0',
      description='Lightweight IoT Sensor Simulator',
      ext_modules=[module])