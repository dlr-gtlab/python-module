/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_propertysetter.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 26.02.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QString>

#include "PythonQtConversion.h"

#include "gt_object.h"

#include "gtpy_extendedwrapper.h"
#include "gtpy_decorator.h"

#include "gtpy_propertysetter.h"

static GtpyPropertySetterObject* setterfunction_free_list = Q_NULLPTR;

static void
meth_dealloc(GtpyPropertySetterObject* m)
{
    PyObject_GC_UnTrack(m);
    Py_XDECREF(m->m_self);
    Py_XDECREF(m->m_module);
    Py_XDECREF(m->m_propId);
    m->m_self = (PyObject*)setterfunction_free_list;

    setterfunction_free_list = m;
}

static int
meth_compare(GtpyPropertySetterObject* a,
             GtpyPropertySetterObject* b)
{
    if (a->m_self != b->m_self)
    {
        return (a->m_self < b->m_self) ? -1 : 1;
    }

    return 0;
}

static PyObject*
meth_repr(GtpyPropertySetterObject* f)
{
    if (f->m_self->ob_type == &GtpyExtendedWrapper_Type)
    {
        QString repr = "<Function to set " + QString(
                           PyString_AsString(f->m_propId)) +
                       " value>";

        return PyString_FromFormat(repr.toStdString().c_str());
    }
    else
    {
        return PyString_FromFormat("<invalid function>");
    }
}

static long
meth_hash(GtpyPropertySetterObject* a)
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
GtpyPropertySetter_Call(PyObject* func, PyObject* args,
                        PyObject* /*kw*/)
{
    GtpyPropertySetterObject* f = (GtpyPropertySetterObject*)func;

    Py_ssize_t argc = PyTuple_Size(args);

    QString propId = QString(PyString_AsString(f->m_propId));

    if (argc != 1)
    {
        if (!propId.isEmpty())
        {
            propId.replace(0, 1, propId.at(0).toUpper());
        }

        QString error = "set" + propId +
                        "(value) requires one argument";

        PyErr_SetString(PyExc_TypeError, error.toStdString().c_str());

        return Q_NULLPTR;
    }

    PyObject* value = PyTuple_GET_ITEM(args, 0);

    if (f->m_self->ob_type == &GtpyExtendedWrapper_Type)
    {
        GtpyExtendedWrapper* self = (GtpyExtendedWrapper*)f->m_self;

        if (self && self->_obj)
        {
            GtObject* gtObj = qobject_cast<GtObject*>(self->_obj->_obj);

            if (gtObj)
            {
                if (PyObject_TypeCheck(value, &GtpyExtendedWrapper_Type))
                {
                    GtpyExtendedWrapper* wrapper = (GtpyExtendedWrapper*)value;
                    value = (PyObject*)wrapper->_obj;
                }

                GtpyDecorator decorator;
                decorator.setPropertyValue(gtObj, propId,
                                           PythonQtConv::PyObjToQVariant(value));

                Py_INCREF(Py_True);
                return Py_True;
            }
        }
    }

    QString error = "Invalid GtpyExtendedWrapper instance";
    PyErr_SetString(PyExc_ValueError, error.toLatin1().data());

    return Q_NULLPTR;
}

// for python 3.x
static PyObject*
meth_richcompare(GtpyPropertySetterObject* a,
                 GtpyPropertySetterObject* b, int op)
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
GtpyPropertySetter_New(QString propId, PyObject* self, PyObject* module)
{
    GtpyPropertySetterObject* op;
    op = setterfunction_free_list;

    if (op != Q_NULLPTR)
    {
        setterfunction_free_list = (GtpyPropertySetterObject*)(
                                       op->m_self);

        PyObject_INIT(op, &GtpyPropertySetter_Type);
    }
    else
    {
        op = PyObject_GC_New(GtpyPropertySetterObject,
                             &GtpyPropertySetter_Type);

        if (op == Q_NULLPTR)
        {
            return Q_NULLPTR;
        }
    }

    PyObject* id = PyString_FromString(propId.toStdString().data());
    Py_XINCREF(id);
    op->m_propId = id;

    Py_XINCREF(self);
    op->m_self = self;

    Py_XINCREF(module);
    op->m_module = module;

    PyObject_GC_Track(op);

    return (PyObject*)op;
}

PyTypeObject
GtpyPropertySetter_Type =
{
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "GtpyPropertySetter",
    sizeof(GtpyPropertySetterObject),
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
    GtpyPropertySetter_Call,      /* tp_call */
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
    0,          /* tp_dict */
};
