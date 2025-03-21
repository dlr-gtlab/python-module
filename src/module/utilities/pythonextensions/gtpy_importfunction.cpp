/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_importfunction.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 23.03.2020
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gtpy_code.h"
#include "gtpy_globals.h"
#include "gtpy_calculatorsmodule.h"

#include "gtpy_importfunction.h"
#include "gtpypp.h"

static void
GtpyMyImport_dealloc(GtpyMyImport* self)
{
    if (self->defaultImp)
    {
        Py_DECREF(self->defaultImp);
        self->defaultImp = nullptr;
    }

    Py_TYPE(self)->tp_free((PyObject*)self);
}


static PyObjectAPIReturn
GtpyMyImport_new(PyTypeObject* type, PyObject* /*args*/,
                 PyObject* /*kwds*/)
{
    GtpyMyImport* self;
    self = (GtpyMyImport*)type->tp_alloc(type, 0);
    self->defaultImp = nullptr;

    return (PyObject*)self;
}

static bool
checkModuleName(PyObject* argsPy, const QString& moduleName)
{
    auto args = PyPPObject::Borrow(argsPy);
    if (!args || !PyPPTuple_Check(args)) return false;

    auto name = PyPPTuple_GetItem(args, 0);
    if (!name || !PyPPUnicode_Check(name)) return false;

    QString nameStr = PyPPString_AsQString(name);
    if (nameStr != moduleName) return false;

    return true;
}

static bool
isImportAllowed(PyObject* args)
{
    if (checkModuleName(args, gtpy::code::modules::GT_CALCULATORS))
    {
        //if (!GtpyCalculatorsModule::findRunningParentTask())
        //{
        //    QString error =  "The import of " +
        //                     GtpyGlobals::MODULE_GtCalculators +
        //                     " is not allowed in this context! Maybe you have "
        //                     "to use a Python Task.";
        //
        //    PyErr_SetString(PyExc_ImportError, error.toLatin1().data());
        //
        //    return false;
        //}
    }

    return true;
}

static PyObjectAPIReturn
GtpyMyImport_Call(PyObject* func, PyObject* args,
                  PyObject* kwds)
{
    GtpyMyImport* f = (GtpyMyImport*)func;

    if (f->defaultImp == nullptr)
    {
        QString error =  "Something is wrong with the import system defined "
                         "for the Python Module! (raised by "
                         "GtpyMyImport_Call)";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return nullptr;
    }
    else
    {
        if (!isImportAllowed(args))
        {
            return nullptr;
        }

        auto mod = PyPPObject_Call(PyPPObject::Borrow(f->defaultImp.object()),
                                  PyPPObject::Borrow(args),
                                  PyPPObject::Borrow(kwds));

        return mod.release();
    }
}

static PyObjectAPIReturn
meth_importGtCalculators(PyObject* self)
{
    GtpyMyImport* f = (GtpyMyImport*)self;

    if (f->defaultImp == nullptr)
    {
        QString error =  "Something is wrong with the import system defined "
                         "for the Python Module! (raised by "
                         "GtpyMyImport_Call)";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return nullptr;
    }
    else
    {
        auto argsTuple = PyPPTuple_New(1);

        PyPPTuple_SetItem(argsTuple, 0, PyPPObject::fromQString(gtpy::code::modules::GT_CALCULATORS));

        auto mod = PyPPObject_Call(PyPPObject::Borrow(f->defaultImp), argsTuple);

        if (!mod)
        {
            return nullptr;
        }

        auto globals = PyPPEval_GetGlobals();

        if (globals && PyPPDict_Check(globals))
        {
            PyPPDict_Merge(globals, PyPPModule_GetDict(mod), 0);
            PyPPDict_SetItem(globals, PyPPObject::fromQString(gtpy::code::modules::GT_CALCULATORS), mod);
        }

        return mod.release();
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
    {nullptr, nullptr, 0, nullptr}  /* Sentinel */
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
