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
gtpy::extension::func::projectPath(PyObject* /*self*/, PyObject* /*args*/)
{
    auto pro = gtApp->currentProject();

    if (!pro)
    {
        QString error = "No project is open.";
        PyErr_SetString(PyExc_Warning, error.toLatin1().data());

        return nullptr;
    }

    return PythonQtConv::QStringToPyObject(pro->path());
}

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)

PyObject*
gtpy::extension::func::sharedFunc(PyObject* /*self*/, PyObject* args)
{
    int argc = PyTuple_Size(args);

    if (argc != 2)
    {
        auto e = QString{"%1(module_id: str, function_id: str) "
                         "takes 2 positional arguments but %2 were given"}
                .arg(gtpy::constants::SHARED_FUNC_F_NAME,
                     QString::number(argc));

        PyErr_SetString(PyExc_ValueError, e.toLatin1().constData());

        return nullptr;
    }

    auto moduleIdObj = PyTuple_GetItem(args, 0);
    auto moduleId = PythonQtConv::PyObjGetString(moduleIdObj);

    auto functionIdObj = PyTuple_GetItem(args, 1);
    auto functionId = PythonQtConv::PyObjGetString(functionIdObj);

    /// todo: check that shared function exists. otherwise return None

    auto pyCode = QString{"lambda *args: call_shared_function("
                          "'%1', '%2', args)"}.arg(moduleId, functionId);

    return PyRun_String(pyCode.toStdString().c_str(), Py_eval_input,
                        PyEval_GetGlobals(), nullptr);
}

PyObject*
gtpy::extension::func::callSharedFunc(PyObject* /*self*/, PyObject* args)
{
    int argc = PyTuple_Size(args);

    if (argc != 3)
    {
        auto e = QString{"%1(module_id: str, function_id: str, args: tuple) "
                         "takes 3 positional arguments but %2 were given"}
                .arg(gtpy::constants::CALL_SHARED_FUNC_F_NAME,
                     QString::number(argc));

        PyErr_SetString(PyExc_ValueError, e.toLatin1().constData());

        return nullptr;
    }

    auto moduleIdObj = PyTuple_GetItem(args, 0);
    auto moduleId = PythonQtConv::PyObjGetString(moduleIdObj);

    auto functionIdObj = PyTuple_GetItem(args, 1);
    auto functionId = PythonQtConv::PyObjGetString(functionIdObj);

    auto func = gt::interface::getSharedFunction(moduleId, functionId);

    if (func.isNull())
    {
        auto e = QString{"%1 has no shared function named %2"}
                .arg(moduleId, functionId);

        PyErr_SetString(PyExc_NameError, e.toLatin1().constData());

        return nullptr;
    }

    auto argListObj = PyTuple_GetItem(args, 2);
    auto argList = PythonQtConv::PyObjToQVariant(argListObj).toList();

    PyObject* res{nullptr};

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


#endif
