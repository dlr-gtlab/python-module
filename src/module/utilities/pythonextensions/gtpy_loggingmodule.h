/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_loggingmodule.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 22.07.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYLOGGINGMODULE_H
#define GTPYLOGGINGMODULE_H

#include <QString>

#include "PythonQtPythonInclude.h"
#include "structmember.h"

#include "gt_task.h"

#include "gtpy_globals.h"

namespace GtpyLoggingModule
{

enum OutputType
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
    PyObject* m_outputType;
} GtpyPyLogger;

extern PyObject*
gtDebug_C_function();

extern PyObject*
gtInfo_C_function();

extern PyObject*
gtError_C_function();

extern PyObject*
gtFatal_C_function();

extern PyObject*
gtWarning_C_function();

static PyMethodDef
GtpyLoggingModule_StaticMethods[] =
{
    {
        "gtDebug", (PyCFunction)gtDebug_C_function, METH_NOARGS,
        "GtpyPyLogger gtDebug() --> Returns a GtpyPyLogger instance that "
        "enables debug outputs"
    },
    {
        "gtInfo", (PyCFunction)gtInfo_C_function, METH_NOARGS,
        "GtpyPyLogger gtInfo() --> Returns a GtpyPyLogger instance that "
        "enables info outputs"
    },
    {
        "gtError", (PyCFunction)gtError_C_function, METH_NOARGS,
        "GtpyPyLogger gtError() --> Returns a GtpyPyLogger instance that "
        "enables error outputs"
    },
    {
        "gtFatal", (PyCFunction)gtFatal_C_function, METH_NOARGS,
        "GtpyPyLogger gtFatal() --> Returns a GtpyPyLogger instance that "
        "enables fatal outputs"
    },
    {
        "gtWarning", (PyCFunction)gtWarning_C_function, METH_NOARGS,
        "GtpyPyLogger gtWarning() --> Returns a GtpyPyLogger instance that "
        "enables warning outputs"
    },
    { NULL, NULL, 0, NULL }
};


#ifdef PY3K
static PyModuleDef
GtpyLogging_Module =
{
    PyModuleDef_HEAD_INIT,
    QSTRING_TO_CHAR_PTR(GtpyGlobals::MODULE_GtLogging_C),
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
