/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_stdout.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 13.01.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QDebug>

#include "gtpy_gilscope.h"

#include "gtpy_stdout.h"

static PyObject*
GtpyStdOutRedirect_new(PyTypeObject *type, PyObject * /*args*/, PyObject * /*kwds*/)
{
    GtpyStdOutRedirect *self;
    self = (GtpyStdOutRedirect *)type->tp_alloc(type, 0);

    self->softspace = 0;
    self->closed = false;
    self->_cb = NULL;

    return (PyObject *)self;
}

static PyObject*
GtpyStdOutRedirect_write(PyObject* self, PyObject* args)
{
    GTPY_GIL_SCOPE

    GtpyStdOutRedirect* s = (GtpyStdOutRedirect*)self;

    PyObject* globals = PyEval_GetGlobals();

    QString contextName;

    if (globals && PyDict_Check(globals))
    {
        PyObject* keys = PyDict_Keys(globals);

        if (keys)
        {
            int count = PyList_Size(keys);
            PyObject* key = Q_NULLPTR;
            QString keyStr;

            for (int i = 0; i < count; i++)
            {
                key = PyList_GetItem(keys, i);

                const char* keyChar = PyString_AsString(key);
                keyStr = QString::fromUtf8(keyChar);

                if (keyStr == "__name__")
                {
                    PyObject* value = PyDict_GetItem(globals, key);
                    Py_INCREF(value);

                    const char* valueChar = PyString_AsString(value);
                    contextName = QString::fromUtf8(valueChar);

                    break;
                }
            }
        }
    }

    if (s->_cb)
    {
        QString output;

        if (PyTuple_GET_SIZE(args)>=1)
        {
            PyObject* obj = PyTuple_GET_ITEM(args,0);

            if (PyUnicode_Check(obj))
            {
#ifdef PY3K
                output = QString::fromUtf8(PyUnicode_AsUTF8(obj));
#else
                PyObject *tmp = PyUnicode_AsUTF8String(obj);

                if(tmp)
                {
                    output = QString::fromUtf8(PyString_AS_STRING(tmp));
                    Py_DECREF(tmp);
                }
                else
                {
                    return NULL;
                }
#endif
            }
            else
            {
                char *string;

                if (!PyArg_ParseTuple(args, "s", &string))
                {
                    return NULL;
                }

                output = QString::fromLatin1(string);
            }
        }

        if (s->softspace > 0)
        {
            (*s->_cb)(contextName, QString(""));
            s->softspace = 0;
        }

        (*s->_cb)(contextName, output);
    }

    return Py_BuildValue("");
}

static PyObject*
GtpyStdOutRedirect_flush(PyObject * /*self*/, PyObject * /*args*/)
{
    return Py_BuildValue("");
}

static PyObject*
GtpyStdOutRedirect_module(PyObject * /*self*/, PyObject * /*args*/)
{
    return Py_None;
}

static PyObject*
GtpyStdOutRedirect_isatty(PyObject * /*self*/, PyObject * /*args*/)
{
    Py_INCREF(Py_False);
    return Py_False;
}

static PyMethodDef
GtpyStdOutRedirect_methods[] =
{
    {"write", (PyCFunction)GtpyStdOutRedirect_write, METH_VARARGS,
    "redirect the writing to a callback"},
    {"flush", (PyCFunction)GtpyStdOutRedirect_flush, METH_VARARGS,
    "flush the output, currently not implemented but needed for logging framework"
    },
    {"module", (PyCFunction)GtpyStdOutRedirect_module, METH_VARARGS,
    "modfunc"
    },
    {"isatty", (PyCFunction)GtpyStdOutRedirect_isatty,   METH_NOARGS,
    "return False since this object is not atty-like device. Needed for logging framework"
    },
    {NULL,    NULL, 0 , NULL} /* sentinel */
};

static PyMemberDef
GtpyStdOutRedirect_members[] =
{
    {const_cast<char*>("softspace"), T_INT, offsetof(GtpyStdOutRedirect, softspace), 0,
      const_cast<char*>("soft space flag")
    },
    { const_cast<char*>("closed"), T_BOOL, offsetof(GtpyStdOutRedirect, closed), 0,
    const_cast<char*>("soft space flag")
    },
    {NULL}  /* Sentinel */
};

PyTypeObject
GtpyStdOutRedirectType =
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
