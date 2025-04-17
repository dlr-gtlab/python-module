/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_loggingmodule.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 22.07.2020
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gt_logging.h"

#include "gtpy_loggingmodule.h"

#include "gt_logging.h"

#include "gtpypp.h"

using namespace GtpyLoggingModule;

namespace
{

bool isLoggingToAppConsolEnabled()
{
    auto globals = PyPPEval_GetGlobals();
    if (!globals || !PyPPDict_Check(globals)) return false;

    auto loggingEnabledObj = PyPPDict_GetItem(
        globals, gtpy::code::attrs::LOGGING_ENABLED);

    if (!loggingEnabledObj || !PyPPBool_Check(loggingEnabledObj)) return false;

    return loggingEnabledObj.get() == Py_True;
}

void printToPyConsol(LogLevel type, const QString& msg)
{
    static const QMap<LogLevel, QString> prefixes = {
        { DEBUG,   "[DEBUG]   " },
        { INFO,    "[INFO]    " },
        { ERROR,   "[ERROR]   " },
        { FATAL,   "[FATAL]   " },
        { WARNING, "[WARNING] " }
    };

    QString pyMsg{msg};
    if (prefixes.contains(type)) pyMsg.prepend(prefixes[type]);

    auto globals = PyPPEval_GetGlobals();
    if (!globals || !PyPPDict_Check(globals)) return;

    auto builtinsDict = PyPPDict_GetItem(globals, "__builtins__");
    if (!builtinsDict || !PyPPDict_Check(builtinsDict)) return;

    auto print = PyPPDict_GetItem(builtinsDict, "print");
    if (!print || !PyPPCallable_Check(print)) return;

    auto argsTuple = PyPPTuple_New(1);
    PyPPTuple_SetItem(argsTuple, 0, PyPPObject::fromQString(pyMsg));

    PyPPObject_Call(print, argsTuple);
}

void printToAppConsol(LogLevel type, const QString& msg)
{
    if (!isLoggingToAppConsolEnabled()) return;

    switch (type)
    {
    case DEBUG:
        gtDebug() << msg;
        break;

    case INFO:
        gtInfo() << msg;
        break;

    case ERROR:
        gtError() << msg;
        break;

    case FATAL:
        gtFatal() << msg;
        break;

    case WARNING:
        gtWarning() << msg;
        break;

    default:
        break;
    }
}

void printToConsols(LogLevel type, const QString& msg)
{
    printToPyConsol(type, msg);
    printToAppConsol(type, msg);
}

PyObjectAPIReturn
createLogger(LogLevel outputType)
{
    auto argsTuple = PyPPTuple_New(1);
    PyPPTuple_SetItem(argsTuple, 0, PyPPObject::fromLong(outputType));

    auto logger = PyPPObject::NewRef(
        GtpyPyLogger_Type.tp_new(&GtpyPyLogger_Type, argsTuple.get(), NULL));

    return logger.release();
}

PyObjectAPIReturn
printLogMsg(LogLevel level, PyObject* args)
{
    if (!args) Py_RETURN_NONE;

    PyObject* pyArg = nullptr;

    // check if no argument or one argument is passed
    if (!PyArg_ParseTuple(args, "|O", &pyArg)) return nullptr;

    // if no argument is passed, return a logger instance to support
    // the use of the lshift "<<" operator for logging
    if (!pyArg) return createLogger(level);

    auto arg = PyPPObject::Borrow(pyArg);

    // convert the argument to a Python unicode object
    // (equivalent to calling str(arg))
    auto pyMsg = PyPPObject_Str(arg);
    if (!pyMsg) return nullptr;

    // convert the Python unicode object to a char pointer
    const auto* msg = PyPPUnicode_AsUTF8(pyMsg);
    if (!msg) return nullptr;

    printToConsols(level, msg);

    Py_RETURN_NONE;
}

}

static void
GtpyPyLogger_dealloc(GtpyPyLogger* self)
{
    if (self->m_logLevel)
    {
        Py_DECREF(self->m_logLevel);
        self->m_logLevel = nullptr;
    }

    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObjectAPIReturn
GtpyPyLogger_new(PyTypeObject* type, PyObject* argsPy,
                 PyObject* /*kwds*/)
{
    GtpyPyLogger* self;
    self = (GtpyPyLogger*)type->tp_alloc(type, 0);

    if (!argsPy)
    {
        QString error =  "__init__(self, LogLevel) missing 1 "
                        "required positional argument: int";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return nullptr;
    }


    const auto args = PyPPObject::Borrow(argsPy);
    const auto argsCount = PyPPTuple_Size(args);

    if (argsCount < 1)
    {
        QString error =  "__init__(self, LogLevel) missing 1 "
                        "required positional argument: int";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return nullptr;
    }
    else if (argsCount > 1)
    {
        QString error = "__init__(self, LogLevel) takes 2 positional "
                        "arguments but ";
        error += QString::number(argsCount);
        error += " were given";

        PyErr_SetString(PyExc_TypeError, error.toStdString().c_str());

        return nullptr;
    }

    auto outputType = PyPPTuple_GetItem(args, 0);

    if (outputType)
    {
        if (!PyPPLong_Check(outputType))
        {
            QString error = "__init__(self, LogLevel) given LogLevel is no "
                            "int value!";

            PyErr_SetString(PyExc_TypeError, error.toStdString().c_str());

            // @TODO potential bug. shouldnt this return nullptr?
        }

        self->m_logLevel = outputType.release();
    }

    return (PyObjectAPIReturn)self;
}

PyObjectAPIReturn
GtpyLoggingModule::gtDebug_C_function(PyObject* /*self*/, PyObject* args)
{
    return printLogMsg(DEBUG, args);
}

PyObjectAPIReturn
GtpyLoggingModule::gtInfo_C_function(PyObject* /*self*/, PyObject* args)
{
    return printLogMsg(INFO, args);
}

PyObjectAPIReturn
GtpyLoggingModule::gtError_C_function(PyObject* /*self*/, PyObject* args)
{
    return printLogMsg(ERROR, args);
}

PyObjectAPIReturn
GtpyLoggingModule::gtFatal_C_function(PyObject* /*self*/, PyObject* args)
{
    return printLogMsg(FATAL, args);
}

PyObjectAPIReturn
GtpyLoggingModule::gtWarning_C_function(PyObject* /*self*/, PyObject* args)
{
    return printLogMsg(WARNING, args);
}

static PyObjectAPIReturn
GtpyPyLogger_lshift(PyObject* self, PyObject* arg)
{
    if (!self || !arg) Py_RETURN_NONE;

    auto* logger = (GtpyPyLogger*)self;
    auto pyppArg = PyPPObject::Borrow(arg);

    // convert the argument to a Python unicode object
    // (equivalent to calling str(arg))
    auto pyMsg = PyPPObject_Str(pyppArg);
    if (!pyMsg) return nullptr;

    // convert the Python unicode object to a char pointer
    const auto* msg = PyPPUnicode_AsUTF8(pyMsg);
    if (!msg) return nullptr;

    auto logLevel = static_cast<LogLevel>(PyInt_AsLong(logger->m_logLevel));

    printToConsols(logLevel, msg);

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
    "GtLogging.GtpyPyLogger",         /*tp_name*/
    sizeof(GtpyPyLogger),             /*tp_basicsize*/
    0,                                /*tp_itemsize*/
    (destructor)GtpyPyLogger_dealloc, /*tp_dealloc*/
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
    "GtpyPyLogger doc",/* tp_doc */
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
