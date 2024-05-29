/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_pythonfunctions.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 04.04.2023
 * Author: Marvin Noethen (AT-TW)
 */

#ifndef GTPY_PYTHONFUNCTIONS_H
#define GTPY_PYTHONFUNCTIONS_H

#include "gt_globals.h"

#include "PythonQtPythonInclude.h"

#include "gtpy_constants.h"
#include "gtpy_globals.h"

namespace gtpy
{

namespace extension
{

namespace func
{

PyObjectAPIReturn projectPath(PyObject* self);
PyObjectAPIReturn footprint(PyObject* self, PyObject* args, PyObject* kwargs);


static PyMethodDef PROJECT_PATH_F_DEF[] =
{
    {
        gtpy::constants::PROJECT_PATH_F_NAME,
        (PyCFunction)projectPath,
        METH_NOARGS,
        "projectPath() returns the current project path."
    },
    {
        gtpy::constants::FOOTPRINT_F_NAME,
        (PyCFunction)footprint,
        METH_VARARGS | METH_KEYWORDS,
        "footprint(only_active=True)\n\n"
        "Returns the current application footprint map.\n"
        "Optional keyword arguments:\n"
        "only_active: If 'True', on the currently active modules are included"
    },
    { nullptr, nullptr, 0, nullptr }
};

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)

PyObjectAPIReturn sharedFunc(PyObject* self, PyObject* args, PyObject* keywds);

PyObjectAPIReturn callSharedFunc(PyObject* self, PyObject* args, PyObject* keywds);

PyObjectAPIReturn sharedFuncIds(PyObject* self);

static PyMethodDef SHARED_FUNC_F_DEF[] =
{
    {
        gtpy::constants::SHARED_FUNC_F_NAME,
        (PyCFunction)(void(*)(void))sharedFunc,
        METH_VARARGS | METH_KEYWORDS,
        "shared_function(module_id: str, function_id: str) returns the shared "
        "function identified by the given IDs as callable lambda function."
    },
    {
        gtpy::constants::CALL_SHARED_FUNC_F_NAME,
        (PyCFunction)(void(*)(void))callSharedFunc,
        METH_VARARGS | METH_KEYWORDS,
        "call_shared_function(module_id: str, function_id: str, args: tuple) "
        "calls the shared function identified by the given IDs. The given args "
        "tuple is passed to the shared function."
    },
    {
        gtpy::constants::SHARED_FUNC_IDS_F_NAME,
        (PyCFunction)sharedFuncIds,
        METH_NOARGS,
        "shared_function_ids() retruns a list of the registered shared "
        "function IDs."
    },
    { nullptr, nullptr, 0, nullptr }
};


#endif

} // namespace func

} // namespace extension

} // namespace gtpy

#endif // GTPY_PYTHONFUNCTIONS_H
