/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_pythonfunctions.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 04.04.2023
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include <QString>

#include "PythonQtConversion.h"

#include "gt_environment.h"
#include "gt_project.h"
#include "gt_application.h"
#include "gt_versionnumber.h"

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
#include "gt_sharedfunction.h"
#endif

#include "gtpy_pythonfunctions.h"
#include "gtpypp.h"

PyObject*
gtpy::extension::func::projectPath(PyObject* /*self*/)
{
    auto pro = gtApp->currentProject();

    if (!pro)
    {
        PyErr_SetString(PyExc_Warning, "No project open.");

        return nullptr;
    }

    return PythonQtConv::QStringToPyObject(pro->path());
}

PyObjectAPIReturn
gtpy::extension::func::envVars(PyObject* self)
{
    auto envVarsMap = PyPPDict_New();

    for (const auto& id : gtEnvironment->varIds())
    {
        PyPPDict_SetItem(envVarsMap,
                         PyPPObject::fromQString(id),
                         PyPPObject::fromQVariant(gtEnvironment->value(id)));
    }

    return envVarsMap.release();
}

/**
 * @brief By default, it returns the application footprint of GTlab, which is a
 * map of loaded module names and their versions. If only_active is true,
 * it returns only the modules that are part of the data model of the current
 * project.
 * @param only_active If true it returns the project footprint, otherwise
 * it returns the application footprint. Defaults to false.
 * @return A map of module names and their versions.
 */
PyObjectAPIReturn
gtpy::extension::func::footprint(PyObject* self, PyObject* args, PyObject* kwargs)
{

    // Define the arguments and their default values
    static const char* kwlist[] = {"only_active", nullptr};

    // Parse the arguments
    int onlyActive = 0; // Default value

    // Parse the arguments
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|p:footprint",
                                     const_cast<char**>(kwlist), &onlyActive)) {
        return NULL;
    }

    auto versionMap = PyPPDict_New();
    PyPPDict_SetItem(versionMap, "Core", PyPPObject::fromQString(
                                         GtVersionNumber(GT_VERSION_MAJOR,
                                                         GT_VERSION_MINOR,
                                                         GT_VERSION_PATCH).toString()));

    QStringList modules{};

    if (onlyActive == 0)
    {
        modules = gtApp->moduleIds();
    }
    else if (auto* proj = gtApp->currentProject())
    {
        modules = proj->moduleIds();
    }

    for (const auto& mod : qAsConst(modules))
    {
        PyPPDict_SetItem(versionMap,
                         PyPPObject::fromQString(mod),
                         PyPPObject::fromQString(gtApp->moduleVersion(mod).toString()));
    }

    return versionMap.release();
}

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)

PyObjectAPIReturn
gtpy::extension::func::sharedFunc(PyObject* /*self*/, PyObject* args,
                                  PyObject* keywds)
{
    const char* moduleId;
    const char* functionId;

    static const char* kwlist[] = {"module_id", "function_id", nullptr};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ss",
                                     const_cast<char**>(kwlist),
                                     &moduleId, &functionId))
    {
        return nullptr;
    }

    auto ids = gt::interface::sharedFunctionIDs();

    using FuncId = gt::interface::SharedFunctionID;
    auto res = std::find_if(ids.begin(), ids.end(),
                            [&moduleId, &functionId](const FuncId& id) {
        return id.moduleId == moduleId && id.functionId == functionId;
    });

    if (res == ids.end())
    {
        auto e = QString{"%1 has no shared function named %2"}
                .arg(moduleId, functionId);

        PyErr_SetString(PyExc_NameError, e.toLatin1().constData());

        return nullptr;
    }

    auto pyCode = QString{"lambda *args: call_shared_function("
                          "'%1', '%2', args)"}.arg(moduleId, functionId);

    return PyRun_String(pyCode.toStdString().c_str(), Py_eval_input,
                        PyEval_GetGlobals(), nullptr);
}

PyObjectAPIReturn
gtpy::extension::func::callSharedFunc(PyObject* /*self*/, PyObject* args,
                                      PyObject* keywds)
{
    const char* moduleId;
    const char* functionId;
    PyObject* argTupleIn = nullptr;

    static const char* kwlist[] = {"module_id", "function_id", "args", nullptr};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ssO!",
                                     const_cast<char**>(kwlist),
                                     &moduleId, &functionId,
                                     &PyTuple_Type, &argTupleIn))
    {
        return nullptr;
    }

    auto func = gt::interface::getSharedFunction(moduleId, functionId);

    if (func.isNull())
    {
        auto e = QString{"%1 has no shared function named %2"}
                .arg(moduleId, functionId);

        PyErr_SetString(PyExc_NameError, e.toLatin1().constData());

        return nullptr;
    }

    const auto argTuple = PyPPObject::Borrow(argTupleIn);
    assert(PyPPTuple_Check(argTuple));

    // Convert args to variants
    QVariantList funcArgs;
    for (int i = 0; i <  PyPPTuple_Size(argTuple); i++)
    {
        funcArgs.append(PyPPObject_AsQVariant(PyPPTuple_GetItem(argTuple, i)));
    }

    try
    {
        // call the shared function
        return PythonQtConv::QVariantListToPyObject(func(funcArgs));
    }
    catch (const std::runtime_error& err)
    {
        PyErr_SetString(PyExc_TypeError, err.what());
    }
    catch (...)
    {
        auto e = QString{"Error occurred while calling %1. Check the type and "
                         "number of the passed arguments."}
                .arg(functionId);

        PyErr_SetString(PyExc_TypeError, e.toLatin1().constData());
    }

    return nullptr;
}

PyObjectAPIReturn
gtpy::extension::func::sharedFuncIds(PyObject* /*self*/)
{
    auto ids = gt::interface::sharedFunctionIDs();
    auto list = PyPPList_New(0);

    using FuncId = gt::interface::SharedFunctionID;
    for (const FuncId& id : qAsConst(ids))
    {
        /// convert the ids to PyObject pointers
        auto modId = PyPPObject::fromQString(id.moduleId);
        auto funcId = PyPPObject::fromQString(id.functionId);

        /// set ids to a new dictionary
        auto dict = PyPPDict_New();
        PyPPDict_SetItem(dict, "module_id", modId);
        PyPPDict_SetItem(dict, "function_id", funcId);

        /// append dict to list
        PyPPList_Append(list, dict);
    }

    return list.release();
}

#endif
