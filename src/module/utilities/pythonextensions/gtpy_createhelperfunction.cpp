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

static GtpyCreateHelperFunctionObject* createfunction_free_list = Q_NULLPTR;

static void
meth_dealloc(GtpyCreateHelperFunctionObject* m)
{
    PyObject_GC_UnTrack(m);
    Py_XDECREF(m->m_self);
    Py_XDECREF(m->m_module);
    Py_XDECREF(m->m_helperName);
    m->m_self = (PyObject*)createfunction_free_list;

    createfunction_free_list = m;
}

static int
meth_compare(GtpyCreateHelperFunctionObject* a,
             GtpyCreateHelperFunctionObject* b)
{
    if (a->m_self != b->m_self)
    {
        return (a->m_self < b->m_self) ? -1 : 1;
    }

    return 0;
}

static PyObject*
meth_repr(GtpyCreateHelperFunctionObject* f)
{
    if (f->m_self->ob_type == &GtpyExtendedWrapper_Type)
    {
        QString repr = "<Function to create " + QString(
                            PyString_AsString(f->m_helperName)) +
                        " instances>";

        return PyString_FromFormat(repr.toStdString().c_str());
    }
    else
    {
        return PyString_FromFormat("<invalid function>");
    }
}

static long
meth_hash(GtpyCreateHelperFunctionObject* a)
{
    long x;

    if (a->m_self == Q_NULLPTR)
    {
        x = 0;
    }
    else
    {
        x = PyObject_Hash(a->m_self);
    }

    return x;
}

static PyObject*
GtpyCreateHelperFunction_Call(PyObject* func, PyObject* args,
                                    PyObject* /*kw*/)
{
    GtpyCreateHelperFunctionObject* f = (GtpyCreateHelperFunctionObject*)func;

    QString objectName;

    Py_ssize_t argc = PyTuple_Size(args);

    QString helperName = QString(PyString_AsString(f->m_helperName));

    if (argc == 1)
    {
        PyObject* firstArg = PyTuple_GET_ITEM(args, 0);

        Py_INCREF(firstArg);

        if(!PyString_Check(firstArg))
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

    return Q_NULLPTR; //PythonQtMemberFunction_Call(f->m_ml, f->m_self, args, kw);
}

// for python 3.x
static PyObject*
meth_richcompare(GtpyCreateHelperFunctionObject* a,
                 GtpyCreateHelperFunctionObject* b, int op)
{
    int x = meth_compare(a, b);
    bool r;

    if (op == Py_LT)
    {
        r = x < 0;
    }
    else if (op == Py_LE)
    {
        r = x < 1;
    }
    else if (op == Py_EQ)
    {
        r = x == 0;
    }
    else if (op == Py_NE)
    {
        r = x != 0;
    }
    else if (op == Py_GE)
    {
        r = x > -1;
    }
    else if (op == Py_GT)
    {
        r = x > 0;
    }
    if (r)
    {
        Py_RETURN_TRUE;
    }
    else
    {
        Py_RETURN_FALSE;
    }
}

PyObject*
GtpyCreateHelperFunction_New(QString helperName, PyObject* self,
                             PyObject* module)
{
    GtpyCreateHelperFunctionObject* op;
    op = createfunction_free_list;

    if (op != Q_NULLPTR)
    {
        createfunction_free_list = (GtpyCreateHelperFunctionObject*)(
                                       op->m_self);

        PyObject_INIT(op, &GtpyCreateHelperFunction_Type);
    }
    else
    {
        op = PyObject_GC_New(GtpyCreateHelperFunctionObject,
                         &GtpyCreateHelperFunction_Type);
        if (op == Q_NULLPTR)
        {
            return Q_NULLPTR;
        }
    }

    PyObject* name = PyString_FromString(helperName.toStdString().data());
    Py_XINCREF(name);
    op->m_helperName = name;

    Py_XINCREF(self);
    op->m_self = self;

    Py_XINCREF(module);
    op->m_module = module;

    PyObject_GC_Track(op);

    return (PyObject*)op;
}

PyTypeObject
GtpyCreateHelperFunction_Type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "GtpyCreateHelperFunction",
    sizeof(GtpyCreateHelperFunctionObject),
    0,
    (destructor)meth_dealloc,     /* tp_dealloc */
    0,          /* tp_print */
    0,          /* tp_getattr */
    0,          /* tp_setattr */
#ifdef PY3K
    0,
#else
    (cmpfunc)meth_compare,      /* tp_compare */
#endif
    (reprfunc)meth_repr,      /* tp_repr */
    0,          /* tp_as_number */
    0,          /* tp_as_sequence */
    0,          /* tp_as_mapping */
    (hashfunc)meth_hash,      /* tp_hash */
    GtpyCreateHelperFunction_Call,      /* tp_call */
    0,          /* tp_str */
    PyObject_GenericGetAttr,    /* tp_getattro */
    0,          /* tp_setattro */
    0,          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,/* tp_flags */
    0,          /* tp_doc */
    0,//(traverseproc)meth_traverse,    /* tp_traverse */
    0,          /* tp_clear */
    (richcmpfunc)meth_richcompare,          /* tp_richcompare */
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
    0,
};
