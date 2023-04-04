/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_pythonfunctions.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 04.04.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_PYTHONFUNCTIONS_H
#define GTPY_PYTHONFUNCTIONS_H

#include "gt_globals.h"

#include "PythonQtPythonInclude.h"

#include "gtpy_constants.h"

namespace gtpy
{

namespace extension
{

namespace func
{

PyObject* projectPath(PyObject* self, PyObject* args);

static PyMethodDef PROJECT_PATH_F_DEF[] =
{
    {
        gtpy::constants::PROJECT_PATH_F_NAME,
        (PyCFunction)projectPath,
        METH_NOARGS,
        ""
    },
    { nullptr, nullptr, 0, nullptr }
};

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)

PyObject* sharedFunc(PyObject* self, PyObject* args);

PyObject* callSharedFunc(PyObject* self, PyObject* args);

static PyMethodDef SHARED_FUNC_F_DEF[] =
{
    {
        gtpy::constants::SHARED_FUNC_F_NAME,
        (PyCFunction)sharedFunc,
        METH_VARARGS,
        ""
    },
    {
        gtpy::constants::CALL_SHARED_FUNC_F_NAME,
        (PyCFunction)callSharedFunc,
        METH_VARARGS,
        ""
    },
    { nullptr, nullptr, 0, nullptr }
};


#endif

} // namespace func

} // namespace extension

} // namespace gtpy

#endif // GTPY_PYTHONFUNCTIONS_H
