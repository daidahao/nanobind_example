#define NPY_NO_DEPRECATED_API NPY_2_0_API_VERSION
#include <Python.h>
#include <numpy/arrayobject.h>
#include <iostream>

static PyObject* add_arrays(PyObject* self, PyObject* args);

static PyMethodDef module_methods[] = {
    {"add_arrays", add_arrays, METH_VARARGS, "Add two 1D NumPy arrays element-wise."},
    {NULL, NULL, 0, NULL} // Sentinel
};

static struct PyModuleDef module_definition = {
    PyModuleDef_HEAD_INIT,
    "a1", // name of module
    "A module that adds two 1D NumPy arrays element-wise.",
    -1,
    module_methods
};

PyMODINIT_FUNC PyInit_a1(void) {
    import_array(); // Initialize NumPy C API
    return PyModule_Create(&module_definition);
}

static PyObject* add_arrays(PyObject* self, PyObject* args) {
    PyArrayObject *array1, *array2;

    // Parse the input tuple
    if (!PyArg_ParseTuple(args, "O!O!", &PyArray_Type, &array1, &PyArray_Type, &array2)) {
        return NULL;
    }

    // Check if the input arrays are 1D and of the same size
    if (PyArray_NDIM(array1) != 1 || PyArray_NDIM(array2) != 1 ||
        PyArray_SIZE(array1) != PyArray_SIZE(array2)) {
        PyErr_SetString(PyExc_ValueError, "Input arrays must be 1D and of the same size");
        return NULL;
    }

    // Get the size of the arrays
    npy_intp size = PyArray_SIZE(array1);

    // Create a new array to hold the result
    PyArrayObject* result = (PyArrayObject*)PyArray_SimpleNew(1, &size, NPY_DOUBLE);
    if (!result) {
        return NULL;
    }

    // Get pointers to the data of the input arrays and the result array
    double* data1 = (double*)PyArray_DATA(array1);
    double* data2 = (double*)PyArray_DATA(array2);
    double* result_data = (double*)PyArray_DATA(result);

    // Perform element-wise addition
    for (npy_intp i = 0; i < size; ++i) {
        result_data[i] = data1[i] + data2[i];
    }

    return (PyObject*)result;
}