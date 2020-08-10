/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_importfunction.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 23.03.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_globals.h"
#include "gtpy_calculatorsmodule.h"

#include "gtpy_importfunction.h"

static void
GtpyMyImport_dealloc(GtpyMyImport* self)
{
    qDebug() << "Dealloc import!°!!!!!!!!!!!!!!!!!!!!!!!!!";

    if (self->defaultImp)
    {
        Py_DECREF(self->defaultImp);
        self->defaultImp = Q_NULLPTR;
    }

    Py_TYPE(self)->tp_free((PyObject*)self);
}


static PyObject*
GtpyMyImport_new(PyTypeObject* type, PyObject* /*args*/,
                 PyObject* /*kwds*/)
{
    qDebug() << "new import!°!!!!!!!!!!!!!!!!!!!!!!!!!";
    GtpyMyImport* self;
    self = (GtpyMyImport*)type->tp_alloc(type, 0);
    self->defaultImp = Q_NULLPTR;

    return (PyObject*)self;
}

static bool
checkModuleName(PyObject* args, const QString& moduleName)
{
    bool retval = false;

    if (args && PyTuple_Check(args))
    {
        Py_INCREF(args);

        PyObject* name = PyTuple_GetItem(args, 0);

        if (name && PyString_Check(name))
        {
            Py_INCREF(name);
            QString nameStr = PyString_AsString(name);

            if (nameStr == moduleName)
            {
                retval = true;
            }

            Py_DECREF(name);
        }

        Py_DECREF(args);
    }

    return retval;
}

static bool
isImportAllowed(PyObject* args)
{
    if (checkModuleName(args, GtpyGlobals::MODULE_GtCalculators))
    {
        if (!GtpyCalculatorsModule::findRunningParentTask())
        {
            QString error =  "The import of " +
                             GtpyGlobals::MODULE_GtCalculators +
                             " is not allowed in this context! Maybe you have "
                             "to use a Python Task.";

            PyErr_SetString(PyExc_ImportError, error.toLatin1().data());

            return false;
        }
    }

    return true;
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
        if (!isImportAllowed(args))
        {
            return Q_NULLPTR;
        }

        PyObject* mod = PyObject_Call(f->defaultImp, args, kwds);

        if (mod == Q_NULLPTR)
        {
            return Q_NULLPTR;
        }

        return mod;
    }
}

static PyObject*
meth_importGtCalculators(PyObject* self)
{
    GtpyMyImport* f = (GtpyMyImport*)self;

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
        PyObject* argsTuple = PyTuple_New(1);

        PyTuple_SetItem(argsTuple, 0, QSTRING_AS_PYSTRING(
                            GtpyGlobals::MODULE_GtCalculators));

        PyObject* mod = PyObject_Call(f->defaultImp, argsTuple, Q_NULLPTR);

        if (mod == Q_NULLPTR)
        {
            return Q_NULLPTR;
        }

        PyObject* globals = PyEval_GetGlobals();

        if (globals)
        {
            Py_INCREF(globals);

            if (PyDict_Check(globals))
            {
                PyDict_Merge(globals, PyModule_GetDict(mod), 0);

                PyDict_SetItem(globals, QSTRING_AS_PYSTRING(
                                   GtpyGlobals::MODULE_GtCalculators), mod);
            }

            Py_DECREF(globals);
        }

        return mod;
    }
}

static PyMethodDef
GtpyMyImport_methods[] =
{
    {
        "importGtCalculators", (PyCFunction)meth_importGtCalculators,
        METH_NOARGS, "Imports GtCalculators without checking if it is "
        "allowed."
    },
    {Q_NULLPTR, Q_NULLPTR, 0, Q_NULLPTR}  /* Sentinel */
};

PyTypeObject
GtpyMyImport_Type =
{
    PyVarObject_HEAD_INIT(NULL, 0)
    "GtpyMyImport",             /*tp_name*/
    sizeof(GtpyMyImport),             /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)GtpyMyImport_dealloc, /*tp_dealloc*/
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
    GtpyMyImport_methods,                   /* tp_methods */
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
