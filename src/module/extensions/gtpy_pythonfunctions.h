/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_pythonfunctions.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 04.04.2023
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPY_PYTHONFUNCTIONS_H
#define GTPY_PYTHONFUNCTIONS_H

#include "PythonQtPythonInclude.h"

#include "gt_version.h"

#include "gtpy_code.h"
#include "gtpy_globals.h"

namespace gtpy
{

namespace extension
{

namespace func
{

PyObjectAPIReturn projectPath(PyObject* self);
PyObjectAPIReturn footprint(PyObject* self, PyObject* args, PyObject* kwargs);

PyObjectAPIReturn envVars(PyObject* self);

static PyMethodDef PROJECT_PATH_F_DEF[] =
{
    {
        gtpy::code::funcs::PROJECT_PATH_F_NAME,
        (PyCFunction)projectPath,
        METH_NOARGS,
        "projectPath() returns the current project path."
    },
    {
        gtpy::code::funcs::ENV_VARS_F_NAME,
        (PyCFunction)envVars,
        METH_NOARGS,
        "envVars returns the GTlab environment variables."
    },
    {
        gtpy::code::funcs::FOOTPRINT_F_NAME,
        (PyCFunction)footprint,
        METH_VARARGS | METH_KEYWORDS,
        "footprint(only_active=False)\n\n"
        "Returns a map of module names and their versions.\n"
        "By default, it returns the application footprint, which includes "
        "all\navailable modules in GTlab.\n\n"
        "Optional keyword arguments:\n"
        "only_active : bool, optional\n"
        "    If True, it returns only the modules that are part of the "
        "current project's\n"
        "    data model (project footprint). Defaults to False."
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
        gtpy::code::funcs::SHARED_FUNC_F_NAME,
        (PyCFunction)(void(*)(void))sharedFunc,
        METH_VARARGS | METH_KEYWORDS,
        "shared_function(module_id: str, function_id: str) returns the shared "
        "function identified by the given IDs as callable lambda function."
    },
    {
        gtpy::code::funcs::CALL_SHARED_FUNC_F_NAME,
        (PyCFunction)(void(*)(void))callSharedFunc,
        METH_VARARGS | METH_KEYWORDS,
        "call_shared_function(module_id: str, function_id: str, args: tuple) "
        "calls the shared function identified by the given IDs. The given args "
        "tuple is passed to the shared function."
    },
    {
        gtpy::code::funcs::SHARED_FUNC_IDS_F_NAME,
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
