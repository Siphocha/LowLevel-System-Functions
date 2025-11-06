#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#define _USE_MATH_DEFINES

static PyObject* sensor_data = NULL;
static int simulation_time = 0;
static time_t start_time;

typedef struct {
    double temperature;
    double humidity;
    double battery_level;
} SensorReadings;

void init_sensor_data() {
    if (sensor_data == NULL) {
        sensor_data = PyList_New(0);
        start_time = time(NULL);
    }
}

SensorReadings generate_sensor_data() {
    SensorReadings readings;
    double time_factor = simulation_time * 0.1;
    
    readings.temperature = 25.0 + 5.0 * sin(time_factor * 2 * M_PI / 10.0);
    
    readings.humidity = 60.0 + 20.0 * sin(time_factor * 2 * M_PI / 15.0);
    
    readings.battery_level = 100.0 - (simulation_time * (100.0 / 15.0));
    if (readings.battery_level < 0) readings.battery_level = 0;
    
    return readings;
}

static PyObject* get_sensor_readings(PyObject* self, PyObject* args) {
    init_sensor_data();
    
    SensorReadings readings = generate_sensor_data();
    
    PyObject* reading_dict = PyDict_New();
    PyDict_SetItemString(reading_dict, "temperature", PyFloat_FromDouble(readings.temperature));
    PyDict_SetItemString(reading_dict, "humidity", PyFloat_FromDouble(readings.humidity));
    PyDict_SetItemString(reading_dict, "battery_level", PyFloat_FromDouble(readings.battery_level));
    PyDict_SetItemString(reading_dict, "timestamp", PyFloat_FromDouble(simulation_time * 2.0));
    
    PyList_Append(sensor_data, reading_dict);
    Py_INCREF(reading_dict);  
    
    simulation_time++;
    
    return reading_dict;
}

static PyObject* get_all_sensor_data(PyObject* self, PyObject* args) {
    init_sensor_data();
    Py_INCREF(sensor_data);
    return sensor_data;
}

static PyObject* reset_simulation(PyObject* self, PyObject* args) {
    if (sensor_data != NULL) {
        Py_DECREF(sensor_data);
        sensor_data = NULL;
    }
    simulation_time = 0;
    start_time = time(NULL);
    init_sensor_data();
    Py_RETURN_NONE;
}

static PyMethodDef SensorSimulatorMethods[] = {
    {"get sensor data", get_sensor_readings, METH_NOARGS, "Get current sensor readings"},
    {"getting all sensor data", get_all_sensor_data, METH_NOARGS, "All historical sensor data"},
    {"resetting simulation", reset_simulation, METH_NOARGS, "Reset the simulation"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef sensorsimulator = {
    PyModuleDef_HEAD_INIT,
    "sensor_simulator",
    "IoT Sensor Data Simulator",
    -1,
    SensorSimulatorMethods
};

PyMODINIT_FUNC PyInit_sensor_simulator(void) {
    PyObject* module = PyModule_Create(&sensorsimulator);
    if (module == NULL) {
        return NULL;
    }
    srand((unsigned int)time(NULL));
    
    return module;
}