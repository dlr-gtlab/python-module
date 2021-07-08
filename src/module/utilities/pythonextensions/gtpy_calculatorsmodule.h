/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_calculatorsmodule.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 22.04.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYCALCULATORSMODULE_H
#define GTPYCALCULATORSMODULE_H

#include <QString>

#include "PythonQtPythonInclude.h"

#include "gt_task.h"

#include "gtpy_globals.h"

namespace GtpyCalculatorsModule
{
extern PyTypeObject GtpyCreateCalculator_Type;

typedef struct
{
    PyObject_HEAD
    PyObject* m_calcClassName;
} GtpyCreateCalculator;

extern PyObject*
findGtTask_C_function(PyObject* self, PyObject* args);

extern GtTask*
findRunningParentTask();

static PyMethodDef
GtpyCalculatorsModule_StaticMethods[] =
{
    {
        "findGtTask", (PyCFunction)findGtTask_C_function, METH_VARARGS,
        "GtTask* findGtTask(name) --> finds an existing GtTask by objectname "
        "and returns it"
    },
    { NULL, NULL, 0, NULL }
};

void createCalcConstructors();

#ifdef PY3K
static PyModuleDef
GtpyCalculators_Module =
{
    PyModuleDef_HEAD_INIT,
    "GtCalculators", //GtpyGlobals::MODULE_GtCalculators.toStdString().data(),
    NULL,
    -1,
    GtpyCalculatorsModule_StaticMethods,
    NULL,
    NULL,
    NULL,
    NULL
};
#endif

}

#endif // GTPYCALCULATORSMODULE_H
