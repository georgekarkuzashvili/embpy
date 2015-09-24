//#include <QCoreApplication>

#include <unistd.h>
#include <stdio.h>

#include <Python.h>

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    char cwd[1024];
    char path[1024];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        sprintf(path, "sys.path.append(\"%s\")", cwd);
        printf("Path: %s\n", path);
    }
    else {
        perror("getcwd() error");
        return 0;
    }



    PyObject *pName, *pModule, *pDict, *pFunc, *pValue;

    if (argc < 3)
    {
        printf("Usage: exe_name python_source function_name\n");
        return 1;
    }

    // Initialize the Python Interpreter
    Py_Initialize();

    PyRun_SimpleString("import sys");
    PyRun_SimpleString(path);

    // Build the name object
    pName = PyString_FromString(argv[1]);

    // Load the module object
    pModule = PyImport_Import(pName);

    // pDict is a borrowed reference
    pDict = PyModule_GetDict(pModule);

    // pFunc is also a borrowed reference
    pFunc = PyDict_GetItemString(pDict, argv[2]);

    if (PyCallable_Check(pFunc))
    {
        pValue = PyObject_CallObject(pFunc, NULL);
    } else
    {
        PyErr_Print();
    }

    if (pValue != NULL)
    {
        printf("Return of call : %d\n", PyInt_AsLong(pValue));
        Py_DECREF(pValue);
    }
    else
    {
        PyErr_Print();
    }

    // Clean up
    Py_DECREF(pModule);
    Py_DECREF(pName);

    // Finish the Python Interpreter
    Py_Finalize();

    return 0;

    //return a.exec();
}
