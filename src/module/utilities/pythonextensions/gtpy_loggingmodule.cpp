/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_loggingmodule.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 22.07.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_loggingmodule.h"

using namespace GtpyLoggingModule;

static void
GtpyPyLogger_dealloc(GtpyPyLogger* self)
{
    if (self->m_outputType)
    {
        Py_DECREF(self->m_outputType);
        self->m_outputType = Q_NULLPTR;
    }

    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject*
GtpyPyLogger_new(PyTypeObject* type, PyObject* args,
                 PyObject* /*kwds*/)
{
    GtpyPyLogger* self;
    self = (GtpyPyLogger*)type->tp_alloc(type, 0);

    if (!args)
    {
        QString error =  "__init__(self, OutputType) missing 1 "
                         "required positional argument: int";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return Q_NULLPTR;
    }

    int argsCount = PyTuple_Size(args);

    if (argsCount < 1)
    {
        QString error =  "__init__(self, OutputType) missing 1 "
                         "required positional argument: int";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return Q_NULLPTR;
    }
    else if (argsCount > 1)
    {
        QString error = "__init__(self, OutputType) takes 2 positional "
                        "arguments but ";
        error += QString::number(argsCount);
        error += " were given";

        PyErr_SetString(PyExc_TypeError, error.toStdString().c_str());

        return Q_NULLPTR;
    }

    PyObject* outputType = PyTuple_GetItem(args, 0);

    if (outputType)
    {
        Py_INCREF(outputType);

        if (!PyInt_Check(outputType))
        {
            Py_DECREF(outputType);

            QString error = "__init__(self, OutputType) given OutputType is no "
                            "int value!";

            PyErr_SetString(PyExc_TypeError, error.toStdString().c_str());
        }

        self->m_outputType = outputType;
    }

    return (PyObject*)self;
}

PyObject*
newLoggerInstance(OutputType outputType)
{
    if (PyType_Ready(&GtpyPyLogger_Type) < 0)
    {
        gtError() << "could not initialize GtpyPyLogger_Type";
    }

    PyObject* argsTuple = PyTuple_New(1);
    PyTuple_SetItem(argsTuple, 0, PyInt_FromLong(outputType));

    PyObject* logger = GtpyPyLogger_Type.tp_new(&GtpyPyLogger_Type, argsTuple,
                       NULL);

    Py_DECREF(argsTuple);

    return logger;
}

PyObject*
GtpyLoggingModule::gtDebug_C_function()
{
    return newLoggerInstance(DEBUG);
}

PyObject*
GtpyLoggingModule::gtInfo_C_function()
{
    return newLoggerInstance(INFO);
}

PyObject*
GtpyLoggingModule::gtError_C_function()
{
    return newLoggerInstance(ERROR);
}

PyObject*
GtpyLoggingModule::gtFatal_C_function()
{
    return newLoggerInstance(FATAL);
}

PyObject*
GtpyLoggingModule::gtWarning_C_function()
{
    return newLoggerInstance(WARNING);
}

static void
printOutput(QString message)
{
    PyObject* globals = PyEval_GetGlobals();

    if (globals)
    {
        Py_INCREF(globals);

        if (PyDict_Check(globals))
        {
            PyObject* builtinsDict = PyDict_GetItemString(globals,
                                     "__builtins__");

            if (builtinsDict)
            {
                Py_INCREF(builtinsDict);

                if (PyDict_Check(builtinsDict))
                {
                    PyObject* print = PyDict_GetItemString(builtinsDict,
                                                           "print");

                    if (print)
                    {
                        Py_INCREF(print);

                        if (PyCallable_Check(print))
                        {
                            PyObject* argsTuple = PyTuple_New(1);
                            PyTuple_SetItem(argsTuple, 0,
                                            QSTRING_AS_PYSTRING(message));

                            PyObject_Call(print, argsTuple, NULL);

                            Py_DECREF(argsTuple);
                        }

                        Py_DECREF(print);
                    }
                }

                Py_DECREF(builtinsDict);
            }
        }

        Py_DECREF(globals);
    }



    //    PyObject* builtins = PyImport_ImportModule("builtins");

    //    if (builtins)
    //    {
    //        if (PyModule_Check(builtins))
    //        {
    //            qDebug() << "builtins valid";
    //            PyObject* dict = PyModule_GetDict(builtins);

    //            if (dict)
    //            {
    //                qDebug() << "dict valid";
    //                Py_INCREF(dict);

    //                PyObject* print = PyDict_GetItemString(dict, "print");

    //                if (print)
    //                {
    //                    qDebug() << "print valid";
    //                    Py_INCREF(print);

    //                    if (PyCallable_Check(print))
    //                    {
    //                        qDebug() << "print is callback";
    //                        PyObject* argsTuple = PyTuple_New(1);
    //                        PyTuple_SetItem(argsTuple, 0,
    //                                        QSTRING_AS_PYSTRING(message));

    //                        PyObject_Call(print, argsTuple, NULL);

    //                        qDebug() << "call ready";
    //                        Py_DECREF(argsTuple);
    //                    }

    //                    Py_DECREF(print);
    //                }

    //                Py_DECREF(dict);
    //            }
    //        }

    //        Py_DECREF(builtins);
    //    }
}

static PyObject*
GtpyPyLogger_lshift(PyObject* self, PyObject* arg)
{
    if (self == Q_NULLPTR || arg == Q_NULLPTR)
    {
        Py_RETURN_NONE;
    }

    GtpyPyLogger* logger = (GtpyPyLogger*)self;

    PyObject* output = Q_NULLPTR;

    if (!PyString_Check(arg))
    {
        output = PyObject_Repr(arg);
    }
    else
    {
        output = arg;
        Py_INCREF(output);
    }

    QString message(PyString_AsString(output));

    int outputType = PyInt_AsLong(logger->m_outputType);

    bool outputToAppConsol = false;

    PyObject* globals = PyEval_GetGlobals();

    if (globals)
    {
        Py_INCREF(globals);

        if (PyDict_Check(globals))
        {
            PyObject* appOutputObj = PyDict_GetItemString(globals,
                                     QSTRING_TO_CHAR_PTR(
                                         GtpyGlobals::ATTR_outputToApp));

            if (appOutputObj)
            {
                Py_INCREF(appOutputObj);

                if (PyBool_Check(appOutputObj))
                {
                    outputToAppConsol = (bool)PyLong_AsLong(appOutputObj);
                }

                Py_DECREF(appOutputObj);
            }
        }

        Py_DECREF(globals);
    }

    switch (outputType)
    {
        case DEBUG:

            if (outputToAppConsol)
            {
                gtDebug() << message;
            }

            printOutput(message.prepend("[DEBUG]   "));
            break;

        case INFO:

            if (outputToAppConsol)
            {
                gtInfo() << message;
            }

            printOutput(message.prepend("[INFO]    "));
            break;

        case ERROR:

            if (outputToAppConsol)
            {
                gtError() << message;
            }

            printOutput(message.prepend("[ERROR]   "));
            break;

        case FATAL:

            if (outputToAppConsol)
            {
                gtFatal() << message;
            }

            printOutput(message.prepend("[FATAL]   "));
            break;

        case WARNING:

            if (outputToAppConsol)
            {
                gtWarning() << message;
            }

            printOutput(message.prepend("[WARNING] "));
            break;

        default:

            if (outputToAppConsol)
            {
                gtDebug() << message;
            }

            printOutput(message.prepend("[DEBUG] "));
            break;
    }

    Py_DECREF(output);
    output = Q_NULLPTR;

    Py_RETURN_NONE;
}

static PyNumberMethods
GtpyPyLogger_as_number =
{
    0,      /* nb_add */
    0,      /* nb_subtract */
    0,      /* nb_multiply */
#ifndef PY3K
    0,      /* nb_divide */
#endif
    0,      /* nb_remainder */
    0,      /* nb_divmod */
    0,      /* nb_power */
    0,      /* nb_negative */
    0,      /* nb_positive */
    0,      /* nb_absolute */
    0,      /* nb_nonzero / nb_bool in Py3K */
    0,      /* nb_invert */
    GtpyPyLogger_lshift,      /* nb_lshift */
    0,      /* nb_rshift */
    0,    /* nb_and */
    0,    /* nb_xor */
    0,    /* nb_or */
#ifndef PY3K
    0,      /* nb_coerce */
#endif
    0,      /* nb_int */
    0,      /* nb_long  / nb_reserved in Py3K */
    0,      /* nb_float */
#ifndef PY3K
    0,      /* nb_oct */
    0,      /* nb_hex */
#endif
    0,      /* nb_inplace_add */
    0,      /* nb_inplace_subtract */
    0,      /* nb_inplace_multiply */
#ifndef PY3K
    0,      /* nb_inplace_divide */
#endif
    0,      /* nb_inplace_remainder */
    0,      /* nb_inplace_power */
    0,      /* nb_inplace_lshift */
    0,      /* nb_inplace_rshift */
    0,      /* nb_inplace_and */
    0,      /* nb_inplace_xor */
    0,      /* nb_inplace_or */
    0,      /* nb_floor_divide */
    0,      /* nb_true_divide */
    0,      /* nb_inplace_floor_divide */
    0,      /* nb_inplace_true_divide */
#ifdef PY3K
    0,      /* nb_index in Py3K */
#endif
};

PyTypeObject
GtpyLoggingModule::GtpyPyLogger_Type =
{
    PyVarObject_HEAD_INIT(NULL, 0)
    "GtLoggingC.GtpyPyLogger",          /*tp_name*/
    sizeof(GtpyPyLogger),               /*tp_basicsize*/
    0,                                  /*tp_itemsize*/
    (destructor)GtpyPyLogger_dealloc,   /*tp_dealloc*/
    0,  /*tp_print*/
    0,  /*tp_getattr*/
    0,  /*tp_setattr*/
    0,  /*tp_compare*/
    0,  /*tp_repr*/
    &GtpyPyLogger_as_number,  /*tp_as_number*/
    0,  /*tp_as_sequence*/
    0,  /*tp_as_mapping*/
    0,  /*tp_hash */
    0,  /*tp_call*/
    0,  /*tp_str*/
    0,  /*tp_getattro*/
    0,  /*tp_setattro*/
    0,  /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE /*tp_flags*/
#ifndef PY3K
    | Py_TPFLAGS_CHECKTYPES,
#else
    ,
#endif
    QSTRING_TO_CHAR_PTR(GtpyGlobals::CLASS_GtpyPyLogger + "doc"), /* tp_doc */
    0,  /* tp_traverse */
    0,  /* tp_clear */
    0,  /* tp_richcompare */
    0,  /* tp_weaklistoffset */
    0,  /* tp_iter */
    0,  /* tp_iternext */
    0,  /* tp_methods */
    0,  /* tp_members */
    0,  /* tp_getset */
    0,  /* tp_base */
    0,  /* tp_dict */
    0,  /* tp_descr_get */
    0,  /* tp_descr_set */
    0,  /* tp_dictoffset */
    0,  /* tp_init */
    0,  /* tp_alloc */
    GtpyPyLogger_new,  /* tp_new */
};
