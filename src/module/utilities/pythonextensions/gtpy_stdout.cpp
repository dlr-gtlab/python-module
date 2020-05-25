/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_stdout.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 13.01.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_gilscope.h"
#include "gtpy_globals.h"

#include "gtpy_stdout.h"

static PyObject*
GtpyStdOutRedirect_new(PyTypeObject* type, PyObject* /*args*/,
                       PyObject* /*kwds*/)
{
    GtpyStdOutRedirect* self;
    self = (GtpyStdOutRedirect*)type->tp_alloc(type, 0);

    self->softspace = 0;
    self->closed = false;
    self->callback = NULL;

    return (PyObject*)self;
}

static PyObject*
GtpyStdOutRedirect_write(PyObject* self, PyObject* args)
{
    GTPY_GIL_SCOPE

    PyObject* threadDict = PyThreadState_GetDict();

    if (!threadDict)
    {
        return Py_BuildValue("");
    }

    Py_INCREF(threadDict);

    PyObject* valItem = PyDict_GetItem(threadDict, PyString_FromString(
                                           GtpyGlobals::CONTEXT_KEY));
    QString contextName;

    if (valItem && PyString_Check(valItem))
    {
        Py_INCREF(valItem);
        const char* val = PyString_AsString(valItem);
        contextName = QString(val);
        Py_DECREF(valItem);
    }

    PyObject* outputItem = PyDict_GetItem(threadDict, PyString_FromString(
            GtpyGlobals::OUTPUT_KEY));

    bool output = false;

    if (outputItem && PyLong_Check(outputItem))
    {
        Py_INCREF(outputItem);
        output = (bool)PyLong_AsLong(outputItem);
        Py_DECREF(outputItem);
    }

    PyObject* errorItem = PyDict_GetItem(threadDict, PyString_FromString(
            GtpyGlobals::ERROR_KEY));

    bool error = false;

    if (errorItem && PyLong_Check(errorItem))
    {
        Py_INCREF(errorItem);
        error = (bool)PyLong_AsLong(errorItem);

        Py_DECREF(errorItem);
    }

    GtpyStdOutRedirect* s = (GtpyStdOutRedirect*)self;

    if (s->callback)
    {
        QString message;

        if (PyTuple_GET_SIZE(args) >= 1)
        {
            PyObject* obj = PyTuple_GET_ITEM(args, 0);

            Py_XINCREF(obj);

            if (PyUnicode_Check(obj))
            {
#ifdef PY3K
                message = QString::fromUtf8(PyUnicode_AsUTF8(obj));
#else
                PyObject* tmp = PyUnicode_AsUTF8String(obj);

                if (tmp)
                {
                    message = QString::fromUtf8(PyString_AS_STRING(tmp));
                    Py_DECREF(tmp);
                }
                else
                {
                    Py_XDECREF(obj);
                    Py_DECREF(threadDict);
                    return NULL;
                }

#endif
            }
            else
            {
                char* string;

                if (!PyArg_ParseTuple(args, "s", &string))
                {
                    Py_XDECREF(obj);
                    Py_DECREF(threadDict);
                    return NULL;
                }

                message = QString::fromLatin1(string);
            }

            Py_XDECREF(obj);
        }

        if (s->softspace > 0)
        {
            (*s->callback)(contextName, output, error, QString(""));
            s->softspace = 0;
        }

        (*s->callback)(contextName, output, error, message);
    }

    Py_DECREF(threadDict);

    return Py_BuildValue("");
}

static PyObject*
GtpyStdOutRedirect_flush(PyObject* /*self*/, PyObject* /*args*/)
{
    return Py_BuildValue("");
}

static PyObject*
GtpyStdOutRedirect_isatty(PyObject* /*self*/, PyObject* /*args*/)
{
    Py_INCREF(Py_False);
    return Py_False;
}

static PyMethodDef
GtpyStdOutRedirect_methods[] =
{
    {
        "write", (PyCFunction)GtpyStdOutRedirect_write, METH_VARARGS,
        "redirect the writing to a callback"
    },
    {
        "flush", (PyCFunction)GtpyStdOutRedirect_flush, METH_VARARGS,
        "flush the output, currently not implemented but needed for logging "
        "framework"
    },
    {
        "isatty", (PyCFunction)GtpyStdOutRedirect_isatty,   METH_NOARGS,
        "return False since this object is not atty-like device. Needed for logging"
        " framework"
    },
    {NULL,    NULL, 0 , NULL} /* sentinel */
};

static PyMemberDef
GtpyStdOutRedirect_members[] =
{
    {
        const_cast<char*>("softspace"), T_INT, offsetof(GtpyStdOutRedirect,
        softspace), 0, const_cast<char*>("soft space flag")
    },
    {
        const_cast<char*>("closed"), T_BOOL, offsetof(GtpyStdOutRedirect, closed),
        0, const_cast<char*>("soft space flag")
    },
    {NULL}  /* Sentinel */
};

PyTypeObject
GtpyStdOutRedirect_Type =
{
    PyVarObject_HEAD_INIT(NULL, 0)
    "GtpyStdOutRedirect",             /*tp_name*/
    sizeof(GtpyStdOutRedirect),             /*tp_basicsize*/
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
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "GtpyStdOutRedirect",           /* tp_doc */
    0,                   /* tp_traverse */
    0,                   /* tp_clear */
    0,                   /* tp_richcompare */
    0,                   /* tp_weaklistoffset */
    0,                   /* tp_iter */
    0,                   /* tp_iternext */
    GtpyStdOutRedirect_methods,                   /* tp_methods */
    GtpyStdOutRedirect_members,                   /* tp_members */
    0,                   /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,                         /* tp_init */
    0,                         /* tp_alloc */
    GtpyStdOutRedirect_new,                 /* tp_new */
};
