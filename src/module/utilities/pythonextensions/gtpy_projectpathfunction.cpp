/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_projectpathfunction.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 03.06.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gt_application.h"
#include "gt_project.h"

#include "gtpy_globals.h"

#include "gtpy_projectpathfunction.h"

using namespace GtpyProjectPathFunction;

static void
meth_dealloc(GtpyProjectPathFunctionObject* m)
{
    Py_TYPE(m)->tp_free((PyObject*)m);
}

static PyObject*
GtpyProjectPathFunction_Call(PyObject* /*func*/, PyObject* /*args*/,
                             PyObject* /*kw*/)
{
    GtProject* pro = gtApp->currentProject();

    if (!pro)
    {
        QString error = "No project is open.";
        PyErr_SetString(PyExc_Warning, error.toLatin1().data());

        return Q_NULLPTR;
    }

    return QSTRING_AS_PYSTRING(pro->path());
}

PyObject*
GtpyProjectPathFunction_New(PyTypeObject* type, PyObject* /*args*/,
                            PyObject* /*kwds*/)
{
    GtpyProjectPathFunctionObject* self;
    self = (GtpyProjectPathFunctionObject*)type->tp_alloc(type, 0);

    return (PyObject*)self;
}

PyTypeObject
GtpyProjectPathFunction::GtpyProjectPathFunction_Type =
{
    PyVarObject_HEAD_INIT(NULL, 0)
    "GtpyProjectPathFunction",
    sizeof(GtpyProjectPathFunctionObject),
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
    GtpyProjectPathFunction_Call,      /* tp_call */
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
    GtpyProjectPathFunction_New,
};
