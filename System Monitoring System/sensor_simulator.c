#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

// Sensor data structure
typedef struct {
    double temperature;
    double humidity;
    double battery_level;
    long timestamp;
} SensorData;

// Global variables to maintain state
static int simulation_running = 0;
static long start_time = 0;
static int data_count = 0;

// Initialize random number generator
void init_random() {
    static int initialized = 0;
    if (!initialized) {
        srand(time(NULL));
        initialized = 1;
    }
}

// Generate simulated sensor data
SensorData generate_sensor_data() {
    SensorData data;
    init_random();
    
    // Base values with some variation
    double base_temp = 25.0 + (rand() % 150) / 10.0; // 25.0 ± 7.5°C
    double base_humidity = 50.0 + (rand() % 400) / 10.0; // 50.0 ± 20%
    
    // Add some realistic patterns and noise
    data.temperature = base_temp + sin(data_count * 0.5) * 2.0;
    data.humidity = fmax(20.0, fmin(95.0, base_humidity + cos(data_count * 0.3) * 10.0));
    
    // Battery level decreases slowly over time
    data.battery_level = fmax(10.0, 95.0 - (data_count * 0.8));
    
    data.timestamp = time(NULL);
    data_count++;
    
    return data;
}

// Python function: get_sensor_readings()
static PyObject* py_get_sensor_readings(PyObject* self, PyObject* args) {
    if (!simulation_running) {
        simulation_running = 1;
        start_time = time(NULL);
        data_count = 0;
    }
    
    // Check if 30 seconds have passed
    if (time(NULL) - start_time > 30) {
        simulation_running = 0;
        Py_RETURN_NONE;
    }
    
    // Generate new sensor data
    SensorData data = generate_sensor_data();
    
    // Create Python dictionary to return
    PyObject* result = PyDict_New();
    PyDict_SetItemString(result, "temperature", PyFloat_FromDouble(data.temperature));
    PyDict_SetItemString(result, "humidity", PyFloat_FromDouble(data.humidity));
    PyDict_SetItemString(result, "battery_level", PyFloat_FromDouble(data.battery_level));
    PyDict_SetItemString(result, "timestamp", PyLong_FromLong(data.timestamp));
    PyDict_SetItemString(result, "count", PyLong_FromLong(data_count));
    
    return result;
}

// Python function: reset_simulation()
static PyObject* py_reset_simulation(PyObject* self, PyObject* args) {
    simulation_running = 0;
    data_count = 0;
    start_time = 0;
    Py_RETURN_NONE;
}

// Method definitions
static PyMethodDef SensorSimulatorMethods[] = {
    {"get_sensor_readings", py_get_sensor_readings, METH_NOARGS, "Get current sensor readings"},
    {"reset_simulation", py_reset_simulation, METH_NOARGS, "Reset the simulation"},
    {NULL, NULL, 0, NULL}
};

// Module definition
static struct PyModuleDef sensor_simulator_module = {
    PyModuleDef_HEAD_INIT,
    "sensor_simulator",
    "Lightweight IoT Sensor Simulator",
    -1,
    SensorSimulatorMethods
};

// Module initialization
PyMODINIT_FUNC PyInit_sensor_simulator(void) {
    return PyModule_Create(&sensor_simulator_module);
}