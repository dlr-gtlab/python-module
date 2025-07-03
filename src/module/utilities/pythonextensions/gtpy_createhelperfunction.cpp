/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_createhelperfunction.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 26.02.2020
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "PythonQt.h"

#include "gt_object.h"
#include "gt_calculatorhelperfactory.h"

#include "gtpy_extendedwrapper.h"
#include "gtpy_decorator.h"

#include "gtpy_createhelperfunction.h"
#include "gtpypp.h"

using namespace GtpyExtendedWrapperModule;

static void
meth_dealloc(GtpyCreateHelperFunctionObject* m)
{
    if (m->m_helperName)
    {
        Py_DECREF(m->m_helperName);
        m->m_helperName = nullptr;
    }

    if (m->m_self)
    {
        Py_DECREF(m->m_self);
        m->m_self = nullptr;
    }

    Py_TYPE(m)->tp_free((PyObject*)m);
}

static PyObjectAPIReturn
GtpyCreateHelperFunction_Call(PyObject* func, PyObject* args_py,
                              PyObject* /*kw*/)
{
    GtpyCreateHelperFunctionObject* f = (GtpyCreateHelperFunctionObject*)func;

    QString objectName;

    if (!args_py) return nullptr;

    auto args = PyPPObject::Borrow(args_py);
    Py_ssize_t argc = PyPPTuple_Size(args);

    if (!f->m_helperName || !PyString_Check(f->m_helperName))
    {
        QString error = "GtpyCreateHelperFunction is invalid!";

        PyErr_SetString(PyExc_TypeError, error.toStdString().c_str());

        return nullptr;
    }

    QString helperName = QString(PyString_AsString(f->m_helperName));

    if (argc == 1)
    {
        auto firstArg = PyPPTuple_GetItem(args, 0);

        if (!PyPPUnicode_Check(firstArg))
        {
            QString error = "create" + helperName +
                            "(objName) accepts only a argument of type string";

            PyErr_SetString(PyExc_TypeError, error.toStdString().c_str());
            return nullptr;
        }
        else
        {
            objectName = PyPPString_AsQString(firstArg);
        }
    }
    else if (argc > 1 || argc < 1)
    {
        QString error = "create" + helperName +
                        "(objName) requires one argument of type string";

        PyErr_SetString(PyExc_TypeError, error.toStdString().c_str());

        return nullptr;
    }

    if (objectName.isEmpty())
    {
        objectName = helperName;
    }

    auto gtObj = GtpyDecorator::pyObjectToGtObject(f->m_self);
    if (gtObj)
    {
        GtObject* helper = gtCalculatorHelperFactory->
                           newCalculatorHelper(helperName,
                                               objectName, gtObj);

        return GtpyDecorator::wrapGtObject(helper).release();
    }


    QString error = "Invalid GtpyExtendedWrapper instance";
    PyErr_SetString(PyExc_ValueError, error.toLatin1().data());

    return nullptr;
}

PyObjectAPIReturn
GtpyCreateHelperFunction_New(PyTypeObject* type, PyObject* argsPy,
                             PyObject* /*kwds*/)
{
    auto self = PyPPObjectT<GtpyCreateHelperFunctionObject>::NewRef(
        (GtpyCreateHelperFunctionObject*)type->tp_alloc(type, 0));

    if (!argsPy)
    {
        QString error =  "Missing required positional arguments: "
                         "GtpyCreateHelperFunction_New(self, helperName)";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return nullptr;
    }

    auto args = PyPPObject::Borrow(argsPy);
    int argsCount = PyPPTuple_Size(args);

    if (argsCount < 2)
    {
        QString error =  "Missing required positional arguments: "
                         "GtpyCreateHelperFunction_New(helperName, self)";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return nullptr;
    }

    auto arg = PyPPTuple_GetItem(args, 0);

    if (!arg)
    {
        QString error =  "Missing required positional arguments: "
                         "GtpyCreateHelperFunction_New(helperName, self)";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return nullptr;
    }


    if (!PyPPUnicode_Check(arg))
    {
        QString error =  "GtpyCreateHelperFunction_New(helperName, self) --> "
                         "Helper class name has to be a string";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return nullptr;
    }

    self->m_helperName = arg.release();

    auto secArg = PyPPTuple_GetItem(args, 1);

    if (!secArg)
    {
        QString error =  "Missing required positional arguments: "
                         "GtpyCreateHelperFunction_New(helperName, self)";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return nullptr;
    }

    if (secArg->ob_type != &GtpyExtendedWrapper_Type)
    {
        auto error = QStringLiteral(
                         "GtpyCreateHelperFunction_New(helperName, self) --> "
                         "self must be of type %1")
                         .arg(gtpy::code::classes::GTPY_EXTENDED_WRAPPER);

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return nullptr;
    }

    self->m_self = secArg.release();

    return (PyObjectAPIReturn)self.release();
}

PyTypeObject
GtpyCreateHelperFunction_Type =
{
    PyVarObject_HEAD_INIT(NULL, 0)
    "GtpyCreateHelperFunction",
    sizeof(GtpyCreateHelperFunctionObject),
    0,
    (destructor)meth_dealloc,     /* tp_dealloc */
    0,          /* tp_print */
    0,          /* tp_getattr */
    0,          /* tp_setattr */
    0,
    0,      /* tp_repr */
    0,          /* tp_as_number */
    0,          /* tp_as_sequence */
    0,          /* tp_as_mapping */
    0,      /* tp_hash */
    GtpyCreateHelperFunction_Call,      /* tp_call */
    0,          /* tp_str */
    0,    /* tp_getattro */
    0,          /* tp_setattro */
    0,          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,/* tp_flags */
    0,          /* tp_doc */
    0,//(traverseproc)meth_traverse,    /* tp_traverse */
    0,          /* tp_clear */
    0,          /* tp_richcompare */
    0,          /* tp_weaklistoffset */
    0,          /* tp_iter */
    0,          /* tp_iternext */
    0, //meth_methods,          /* tp_methods */
    0, //meth_members,       /* tp_members */
    0, //meth_getsets,       /* tp_getset */
    0,          /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,                       /* tp_init */
    0,                         /* tp_alloc */
    GtpyCreateHelperFunction_New,
};

//PyTypeObject
//GtpyCreateHelperFunction_Type =
//{
//    PyVarObject_HEAD_INIT(&PyType_Type, 0)
//    "GtpyCreateHelperFunction",
//    sizeof(GtpyCreateHelperFunctionObject),
//    0,
//    (destructor)meth_dealloc,     /* tp_dealloc */
//    0,          /* tp_print */
//    0,          /* tp_getattr */
//    0,          /* tp_setattr */
//#ifdef PY3K
//    0,
//#else
//    (cmpfunc)meth_compare,      /* tp_compare */
//#endif
//    (reprfunc)meth_repr,      /* tp_repr */
//    0,          /* tp_as_number */
//    0,          /* tp_as_sequence */
//    0,          /* tp_as_mapping */
//    (hashfunc)meth_hash,      /* tp_hash */
//    GtpyCreateHelperFunction_Call,      /* tp_call */
//    0,          /* tp_str */
//    PyObject_GenericGetAttr,    /* tp_getattro */
//    0,          /* tp_setattro */
//    0,          /* tp_as_buffer */
//    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,/* tp_flags */
//    0,          /* tp_doc */
//    0,//(traverseproc)meth_traverse,    /* tp_traverse */
//    0,          /* tp_clear */
//    (richcmpfunc)meth_richcompare,          /* tp_richcompare */
//    0,          /* tp_weaklistoffset */
//    0,          /* tp_iter */
//    0,          /* tp_iternext */
//    0, //meth_methods,          /* tp_methods */
//    0, //meth_members,       /* tp_members */
//    0, //meth_getsets,       /* tp_getset */
//    0,          /* tp_base */
//    0,                         /* tp_dict */
//    0,                         /* tp_descr_get */
//    0,                         /* tp_descr_set */
//    0,                         /* tp_dictoffset */
//    0,                       /* tp_init */
//    0,                         /* tp_alloc */
//    0,
//};
