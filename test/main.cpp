#include <QtCore>
#include <QCoreApplication>
#include <QTimer>

#include <unistd.h>
#include <stdio.h>

#include <Python.h>

#include "main.h"

void Task::run() {

    char cwd[1024];
    char path[1024];
    int argc;

    QStringList argv = QCoreApplication::arguments();
    argc = argv.size();

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        sprintf(path, "sys.path.append(\"%s\")", cwd);
        printf("Path: %s\n", path);
    }
    else {
        perror("getcwd() error");
        emit finished();
    }

    PyObject *pName, *pModule, *pDict, *pFunc, *pValue, *pArgs;

    if (argc < 4) {
        printf("Usage: exe_name python_source function_name\n");
        emit finished();
    }

    // Initialize the Python Interpreter
    Py_Initialize();

    PyRun_SimpleString("import sys");
    PyRun_SimpleString(path);

    // Build the name object
    pName = PyString_FromString(argv[1].toUtf8().data());

    // Load the module object
    pModule = PyImport_Import(pName);

    // pDict is a borrowed reference
    pDict = PyModule_GetDict(pModule);

    // pFunc is also a borrowed reference
    pFunc = PyDict_GetItemString(pDict, argv[2].toUtf8().data());
    if (!pFunc) {
        PyErr_Print();
        emit finished();
    }

    if (PyCallable_Check(pFunc)) {
        // Prepare the argument list for the call
        if( argc > 3 ) {
            pArgs = PyTuple_New(argc - 3);
            for (int i = 0; i < argc - 3; i++) {
                pValue = PyInt_FromLong(atoi(argv[i + 3].toUtf8().data()));
                if (!pValue) {
                    PyErr_Print();
                    emit finished();
                }
                PyTuple_SetItem(pArgs, i, pValue);
            }

            pValue = PyObject_CallObject(pFunc, pArgs);

            if (pArgs != NULL) {
               Py_DECREF(pArgs);
            }
        }
        else {
           pValue = PyObject_CallObject(pFunc, NULL);
        }

        if (pValue != NULL) {
            printf("Return of call : %d\n", PyInt_AsLong(pValue));
            Py_DECREF(pValue);
        }
        else {
           PyErr_Print();
        }

    }

    // Clean up
    Py_DECREF(pModule);
    Py_DECREF(pName);

    // Finish the Python Interpreter
    Py_Finalize();

    emit finished();
}


int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    Task *task = new Task(&a);
    QObject::connect(task, SIGNAL(finished()), &a, SLOT(quit()));
    QTimer::singleShot(0, task, SLOT(run()));

    return a.exec();
}
