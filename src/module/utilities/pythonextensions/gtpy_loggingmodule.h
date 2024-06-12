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

extern PyObjectAPIReturn
gtDebug_C_function();

extern PyObjectAPIReturn
gtInfo_C_function();

extern PyObjectAPIReturn
gtError_C_function();

extern PyObjectAPIReturn
gtFatal_C_function();

extern PyObjectAPIReturn
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
    "GtLogging",
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
