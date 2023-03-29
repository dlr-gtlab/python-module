/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_sharedfunctiondef.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 29.03.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_sharedfunctiondef.h"

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)

#include <QString>

#include "PythonQtConversion.h"

#include "gt_sharedfunction.h"

PyObject*
gtpy::python::func::callSharedFunc(PyObject* /*self*/, PyObject* args)
{
    int argc = PyTuple_Size(args);

    if (argc != 3)
    {
        auto e = QString{"%1(moduleId: str, functionId: str, args: tuple) "
                         "takes 3 positional arguments but %2 were given"}
                .arg(gtpy::constants::SHAREDFUNCCALLER_FUNC_NAME,
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
        auto e = QString{"Error occurred while calling %1. Check the types and "
                         "the number of the passed arguments."}
                .arg(functionId);

        PyErr_SetString(PyExc_TypeError, e.toLatin1().constData());
    }

    return res;
}

#endif
