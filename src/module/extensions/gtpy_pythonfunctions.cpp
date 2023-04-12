/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_pythonfunctions.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 04.04.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QString>

#include "PythonQtConversion.h"

#include "gt_project.h"
#include "gt_application.h"

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
#include "gt_sharedfunction.h"
#endif

#include "gtpy_pythonfunctions.h"

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

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)

PyObject*
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

PyObject*
gtpy::extension::func::callSharedFunc(PyObject* /*self*/, PyObject* args,
                                      PyObject* keywds)
{
    const char* moduleId;
    const char* functionId;
    PyObject* argTuple = nullptr;

    static const char* kwlist[] = {"module_id", "function_id", "args", nullptr};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ssO!",
                                     const_cast<char**>(kwlist),
                                     &moduleId, &functionId,
                                     &PyTuple_Type, &argTuple))
    {
        return nullptr;
    }

    Py_INCREF(argTuple);

    auto func = gt::interface::getSharedFunction(moduleId, functionId);

    if (func.isNull())
    {
        auto e = QString{"%1 has no shared function named %2"}
                .arg(moduleId, functionId);

        PyErr_SetString(PyExc_NameError, e.toLatin1().constData());

        return nullptr;
    }

    auto argList = PythonQtConv::PyObjToQVariant(argTuple).toList();
    Py_DECREF(argTuple);

    PyObject* res = nullptr;

    try
    {
        res = PythonQtConv::QVariantListToPyObject(func(argList));
    }
    catch (...)
    {
        auto e = QString{"Error occurred while calling %1. Check the type and "
                         "number of the passed arguments."}
                .arg(functionId);

        PyErr_SetString(PyExc_TypeError, e.toLatin1().constData());
    }

    return res;
}

PyObject*
gtpy::extension::func::sharedFuncIds(PyObject* /*self*/)
{
    auto ids = gt::interface::sharedFunctionIDs();
    auto list = PyList_New(0);

    using FuncId = gt::interface::SharedFunctionID;
    for (const FuncId& id : qAsConst(ids))
    {
        /// convert the ids to PyObject pointers
        auto modId = PythonQtConv::QStringToPyObject(id.moduleId);
        auto funcId = PythonQtConv::QStringToPyObject(id.functionId);

        /// set ids to a new dictionary
        auto dict = PyDict_New();
        PyDict_SetItemString(dict, "module_id", modId);
        PyDict_SetItemString(dict, "function_id", funcId);

        /// append dict to list
        PyList_Append(list, dict);

        /// decrement the object counter, because PyDict_SetItemString and
        /// PyList_Append do not steal the value reference
        Py_DECREF(modId);
        Py_DECREF(funcId);
        Py_DECREF(dict);
    }

    return list;
}

#endif
