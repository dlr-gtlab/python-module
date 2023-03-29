/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_sharedfunctiondef.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 29.03.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYSHAREDFUNCTIONDEF_H
#define GTPYSHAREDFUNCTIONDEF_H

#include "gt_globals.h"

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)

#include "PythonQtPythonInclude.h"

#include "gtpy_constants.h"

namespace gtpy
{

namespace python
{

namespace func
{

PyObject* callSharedFunc(PyObject* self, PyObject* args);

static PyMethodDef sharedFuncDef[] =
{
    {
        gtpy::constants::SHAREDFUNCCALLER_FUNC_NAME,
        (PyCFunction)callSharedFunc,
        METH_VARARGS,
        ""
    },
    { nullptr, nullptr, 0, nullptr }
};

} // namespace func

} // namespace python

} // namespace gtpy

#endif

#endif // GTPYSHAREDFUNCTIONDEF_H
