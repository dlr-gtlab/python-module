/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_loggingmodule.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 22.07.2020
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPYLOGGINGMODULE_H
#define GTPYLOGGINGMODULE_H

#include <QString>

#include "PythonQtPythonInclude.h"

#include "gtpy_globals.h"
#include "gtpy_code.h"

namespace GtpyLoggingModule
{

enum LogLevel
{
    DEBUG = 0,
    INFO,
    ERROR,
    FATAL,
    WARNING
};

extern PyTypeObject GtpyPyLogger_Type;

typedef struct
{
    PyObject_HEAD
    PyObject* m_logLevel;
} GtpyPyLogger;

extern PyObjectAPIReturn
gtDebug_C_function(PyObject* self, PyObject* args);

extern PyObjectAPIReturn
gtInfo_C_function(PyObject* self, PyObject* args);

extern PyObjectAPIReturn
gtError_C_function(PyObject* self, PyObject* args);

extern PyObjectAPIReturn
gtFatal_C_function(PyObject* self, PyObject* args);

extern PyObjectAPIReturn
gtWarning_C_function(PyObject* self, PyObject* args);

static PyMethodDef
GtpyLoggingModule_StaticMethods[] =
{
    {
        gtpy::code::funcs::GT_DEBUG, (PyCFunction)gtDebug_C_function,
        METH_VARARGS,
        "gtDebug([message]) -> None | GtpyPyLogger\n"
        "Logs a debug-level message if a message is provided.\n"
        "Otherwise returns a GtpyPyLogger instance that allows logging using "
        "the lshift operator (e.g. gtDebug() << 'debug message').\n"
        "The recommended usage is: gtDebug('debug message')."
    },
    {
        gtpy::code::funcs::GT_INFO, (PyCFunction)gtInfo_C_function,
        METH_VARARGS,
        "gtInfo([message]) -> None | GtpyPyLogger\n"
        "Logs an info-level message if a message is provided.\n"
        "Otherwise returns a GtpyPyLogger instance that allows logging using "
        "the lshift operator (e.g. gtInfo() << 'info message').\n"
        "The recommended usage is: gtInfo('info message')."
    },
    {
        gtpy::code::funcs::GT_ERROR, (PyCFunction)gtError_C_function,
        METH_VARARGS,
        "gtError([message]) -> None | GtpyPyLogger\n"
        "Logs an error-level message if a message is provided.\n"
        "Otherwise returns a GtpyPyLogger instance that allows logging using "
        "the lshift operator (e.g. gtError() << 'error message').\n"
        "The recommended usage is: gtError('error message')."
    },
    {
        gtpy::code::funcs::GT_FATAL, (PyCFunction)gtFatal_C_function,
        METH_VARARGS,
        "gtFatal([message]) -> None | GtpyPyLogger\n"
        "Logs a fatal-level message if a message is provided.\n"
        "Otherwise returns a GtpyPyLogger instance that allows logging using "
        "the lshift operator (e.g. gtFatal() << 'fatal message').\n"
        "The recommended usage is: gtFatal('fatal message')."
    },
    {
        gtpy::code::funcs::GT_WARNING, (PyCFunction)gtWarning_C_function,
        METH_VARARGS,
        "gtWarning([message]) -> None | GtpyPyLogger\n"
        "Logs a warning-level message if a message is provided.\n"
        "Otherwise returns a GtpyPyLogger instance that allows logging using "
        "the lshift operator (e.g. gtWarning() << 'warning message').\n"
        "The recommended usage is: gtWarning('warning message')."
    },
    { NULL, NULL, 0, NULL }
};


#ifdef PY3K
static PyModuleDef
GtpyLogging_Module =
{
    PyModuleDef_HEAD_INIT,
    gtpy::code::modules::GT_LOGGING,
    NULL,
    -1,
    GtpyLoggingModule_StaticMethods,
    NULL,
    NULL,
    NULL,
    NULL
};
#endif

}

#endif // GTPYLOGGINGMODULE_H
