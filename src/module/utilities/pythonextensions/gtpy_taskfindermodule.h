/* GTlab - Gas Turbine laboratory
 * Source File: gtpytaskfindermodule.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 22.04.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYTASKFINDERMODULE_H
#define GTPYTASKFINDERMODULE_H

#include "PythonQtPythonInclude.h"

extern PyObject*
findGtTask_C_function(PyObject* self, PyObject* args);

static PyMethodDef
GtpyTaskFinder_StaticMethods[] = {
    { "findGtTask", (PyCFunction)findGtTask_C_function, METH_VARARGS, "GtTask* "
      "findGtTask(name) --> finds an existing GtTask by objectname and"
      " returns it" },
    { NULL, NULL, 0, NULL }
};

#ifdef PY3K
static PyModuleDef
GtpyTaskFinder_Module = {
  PyModuleDef_HEAD_INIT,
  GtpyGlobals::GTPYTASKFINDER_MODULE.toStdString().data(),
  NULL,
  -1,
  GtpyTaskFinder_StaticMethods,
  NULL,
  NULL,
  NULL,
  NULL
};
#endif
#endif // GTPYTASKFINDERMODULE_H
