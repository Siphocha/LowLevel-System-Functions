// sensor.c
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

static int counter = 0;
static int max_reads = 15; // 30 seconds / 2 seconds = 15 readings

static PyObject *
sensor_get_reading(PyObject *self, PyObject *args)
{
    if (counter >= max_reads) {
        // Signal completion to Python by raising StopIteration
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }

    // Sleep 2 seconds (cross-platform)
#ifdef _WIN32
    Sleep(2000);
#else
    sleep(2);
#endif

    // Timestamp (seconds since epoch)
    double ts = (double)time(NULL) + (double)counter * 0.0; // pure epoch seconds

    // Simple simulated sensor values (some randomness + time-dependent variation)
    double t_noise = (double)(rand() % 1000) / 1000.0; // 0..1
    double temp = 22.0 + 6.0 * sin(counter * 0.5) + 2.0 * t_noise;      // ~22..30
    double hum  = 40.0 + 30.0 * ((double)(rand() % 1000) / 1000.0);     // ~40..70
    double batt = 100.0 - (double)counter * (100.0 / (double)max_reads) + ((double)(rand() % 50)/100.0); // degrade

    counter++;

    // Return tuple: (timestamp, temperature, humidity, battery)
    return Py_BuildValue("(dddd)", ts, temp, hum, batt);
}

static PyMethodDef SensorMethods[] = {
    {"get_reading",  sensor_get_reading, METH_NOARGS, "Block 2s then return (timestamp, temp, hum, batt). Raises StopIteration after 15 reads."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef sensormodule = {
    PyModuleDef_HEAD_INIT,
    "sensor",   /* name of module */
    "Lightweight IoT sensor simulator (C extension).", /* module documentation */
    -1,
    SensorMethods
};

PyMODINIT_FUNC
PyInit_sensor(void)
{
    srand((unsigned int)time(NULL) ^ (unsigned int)clock());
    counter = 0;
    return PyModule_Create(&sensormodule);
}
