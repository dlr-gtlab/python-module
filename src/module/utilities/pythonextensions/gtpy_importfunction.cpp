/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_importfunction.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 23.03.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_importfunction.h"

static PyObject*
GtpyMyImport_new(PyTypeObject* type, PyObject* /*args*/,
                       PyObject* /*kwds*/)
{
    GtpyMyImport* self;
    self = (GtpyMyImport*)type->tp_alloc(type, 0);
    self->defaultImp = Q_NULLPTR;

    return (PyObject*)self;
}

static void
deleteGtCalculatorsModule(PyObject* args)
{
    if (args && PyTuple_Check(args))
    {
        Py_INCREF(args);

        PyObject* name = PyTuple_GetItem(args, 0);

        if (name && PyString_Check(name))
        {
            Py_INCREF(name);
            QString nameStr = PyString_AsString(name);

            if (nameStr == "GtCalculators")
            {
                PythonQtObjectPtr sys;
                sys.setNewRef(PyImport_ImportModule("sys"));

                PyObject* dict = PyModule_GetDict(sys);

                if (dict)
                {
                    Py_INCREF(dict);

                    PyObject* modules = PyDict_GetItemString(dict,
                                                             "modules");

                    if (modules)
                    {
                        Py_INCREF(modules);

                        if (PyDict_Contains(modules, name))
                        {
                            PyDict_DelItemString(modules, "GtCalculators");
                        }

                        Py_DECREF(modules);
                    }

                    Py_DECREF(dict);
                }
            }

            Py_DECREF(name);
        }

        Py_DECREF(args);
    }
}

static PyObject*
GtpyMyImport_Call(PyObject* func, PyObject* args,
                                    PyObject* kwds)
{
    GtpyMyImport* f = (GtpyMyImport*)func;

    if (f->defaultImp == Q_NULLPTR)
    {
        QString error =  "Something is wrong with the import system defined "
                         "for the Python Module! (raised by "
                         "GtpyMyImport_Call)";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return Q_NULLPTR;
    }
    else
    {
        PyObject* mod = PyObject_Call(f->defaultImp, args, kwds);

        if (mod == Q_NULLPTR)
        {
            return Q_NULLPTR;
        }

        return mod;
    }
}

PyTypeObject
GtpyMyImport_Type =
{
    PyVarObject_HEAD_INIT(NULL, 0)
    "GtpyMyImport",             /*tp_name*/
    sizeof(GtpyMyImport),             /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    0, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,           /*tp_compare*/
    0,              /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    GtpyMyImport_Call,        /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "GtpyMyImport doc",           /* tp_doc */
    0,                   /* tp_traverse */
    0,                   /* tp_clear */
    0,                   /* tp_richcompare */
    0,                   /* tp_weaklistoffset */
    0,                   /* tp_iter */
    0,                   /* tp_iternext */
    0,                   /* tp_methods */
    0,                   /* tp_members */
    0,                   /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,                         /* tp_init */
    0,                         /* tp_alloc */
    GtpyMyImport_new,                 /* tp_new */
};
