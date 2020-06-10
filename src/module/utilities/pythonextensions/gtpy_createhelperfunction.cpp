/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_createhelperfunction.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 26.02.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "PythonQt.h"

#include "gt_object.h"
#include "gt_calculatorhelperfactory.h"

#include "gtpy_extendedwrapper.h"
#include "gtpy_decorator.h"

#include "gtpy_createhelperfunction.h"

static void
meth_dealloc(GtpyCreateHelperFunctionObject* m)
{
    if (m->m_helperName)
    {
        Py_DECREF(m->m_helperName);
        m->m_helperName = Q_NULLPTR;
    }

    if (m->m_self)
    {
        Py_DECREF(m->m_self);
        m->m_self = Q_NULLPTR;
    }

    Py_TYPE(m)->tp_free((PyObject*)m);
}

static PyObject*
GtpyCreateHelperFunction_Call(PyObject* func, PyObject* args,
                              PyObject* /*kw*/)
{
    GtpyCreateHelperFunctionObject* f = (GtpyCreateHelperFunctionObject*)func;

    QString objectName;

    Py_ssize_t argc = PyTuple_Size(args);

    if (!f->m_helperName || !PyString_Check(f->m_helperName))
    {
        QString error = "GtpyCreateHelperFunction is invalid!";

        PyErr_SetString(PyExc_TypeError, error.toStdString().c_str());

        return Q_NULLPTR;
    }

    QString helperName = QString(PyString_AsString(f->m_helperName));

    if (argc == 1)
    {
        PyObject* firstArg = PyTuple_GetItem(args, 0);

        Py_INCREF(firstArg);

        if (!PyString_Check(firstArg))
        {
            QString error = "create" + helperName +
                            "(objName) accepts only a argument of type string";

            PyErr_SetString(PyExc_TypeError, error.toStdString().c_str());

            Py_DECREF(firstArg);
            return Q_NULLPTR;
        }
        else
        {
            objectName = PyString_AsString(firstArg);
        }

        Py_DECREF(firstArg);
    }
    else if (argc > 1 || argc < 1)
    {
        QString error = "create" + helperName +
                        "(objName) requires one argument of type string";

        PyErr_SetString(PyExc_TypeError, error.toStdString().c_str());

        return Q_NULLPTR;
    }

    if (objectName.isEmpty())
    {
        objectName = helperName;
    }

    if (f->m_self->ob_type == &GtpyExtendedWrapper_Type)
    {
        GtpyExtendedWrapper* self = (GtpyExtendedWrapper*)f->m_self;

        if (self && self->_obj)
        {
            Py_INCREF(self);

            GtObject* gtObj = qobject_cast<GtObject*>(self->_obj->_obj);

            if (gtObj)
            {
                GtObject* helper = gtCalculatorHelperFactory->
                                   newCalculatorHelper(helperName,
                                                       objectName, gtObj);
                Py_DECREF(self);

                PyObject* test = GtpyDecorator::wrapGtObject(helper);

                return test;
            }

            Py_DECREF(self);
        }
    }

    QString error = "Invalid GtpyExtendedWrapper instance";
    PyErr_SetString(PyExc_ValueError, error.toLatin1().data());

    return Q_NULLPTR;
}

PyObject*
GtpyCreateHelperFunction_New(PyTypeObject* type, PyObject* args,
                             PyObject* /*kwds*/)
{
    GtpyCreateHelperFunctionObject* self;
    self = (GtpyCreateHelperFunctionObject*)type->tp_alloc(type, 0);

    if (!args)
    {
        QString error =  "Missing required positional arguments: "
                         "GtpyCreateHelperFunction_New(self, helperName)";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return Q_NULLPTR;
    }

    int argsCount = PyTuple_Size(args);

    if (argsCount < 2)
    {
        QString error =  "Missing required positional arguments: "
                         "GtpyCreateHelperFunction_New(helperName, self)";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return Q_NULLPTR;
    }

    PyObject* arg = PyTuple_GetItem(args, 0);

    if (!arg)
    {
        QString error =  "Missing required positional arguments: "
                         "GtpyCreateHelperFunction_New(helperName, self)";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return Q_NULLPTR;
    }

    Py_INCREF(arg);

    if (!PyString_Check(arg))
    {
        QString error =  "GtpyCreateHelperFunction_New(helperName, self) --> "
                         "Helper class name has to be a string";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        Py_DECREF(arg);
        return Q_NULLPTR;
    }

    self->m_helperName = arg;

    PyObject* secArg = PyTuple_GetItem(args, 1);

    if (!secArg)
    {
        QString error =  "Missing required positional arguments: "
                         "GtpyCreateHelperFunction_New(helperName, self)";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return Q_NULLPTR;
    }

    Py_INCREF(secArg);

    if (secArg->ob_type != &GtpyExtendedWrapper_Type)
    {
        QString error =  "GtpyCreateHelperFunction_New(helperName, self) --> "
                         "self must be of type int " +
                         GtpyGlobals::GTOBJECT_WRAPPER;

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        Py_DECREF(secArg);
        return Q_NULLPTR;
    }

    self->m_self = secArg;

    return (PyObject*)self;
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
