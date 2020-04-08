/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_taskdistributor.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 30.03.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QObject>
#include <QPointer>

#include "gt_application.h"
#include "gt_processdata.h"
#include "gt_project.h"

#include "gtpy_decorator.h"
#include "gtpy_extendedwrapper.h"

#include "gtpy_taskdistributor.h"

//static GtpyTaskDistributor* dis_free_list = Q_NULLPTR;

PyObject*
GtpyTaskDistributor_New(PyObject* context)
{
    GtpyTaskDistributor* op;

//    qDebug() << "NEW TASK DIST!!!!!";
    op = PyObject_GC_New(GtpyTaskDistributor,
                         &GtpyTaskDistributor_Type);

//    qDebug() << "created!";
////    if (op == Q_NULLPTR)
////    {
////        return Q_NULLPTR;
////    }

//    GtpyTaskDistributor* op;
//    op = dis_free_list;

//    if (op != Q_NULLPTR)
//    {
//        dis_free_list = (GtpyTaskDistributor*)(
//                                       op->m_self);

//        PyObject_INIT(op, &GtpyTaskDistributor_Type);
//    }
//    else
//    {
//        op = PyObject_GC_New(GtpyTaskDistributor,
//                         &GtpyTaskDistributor_Type);
//        if (op == Q_NULLPTR)
//        {
//            return Q_NULLPTR;
//        }
//    }

//    qDebug() << "doco task!!! " << task <<"   "  << /*PyObject_GetAttrString(*/PyImport_AddModule("__main__")/*, "__task")*/;
//    qDebug() << "con == " << context << "  " << PyDict_Check(context) << "  " << PyList_Size(PyObject_Dir(context));



//    GtpyDecorator decorator;

//    PyObject* wrappedTask = decorator.wrapGtObject(task);

//    qDebug() << "wrapped == " << ((GtpyExtendedWrapper*)wrappedTask);
//    Py_XINCREF(wrappedTask);
//    op->m_task = wrappedTask;

    Py_XINCREF(context);
    op->m_context = context;

    PyObject_GC_Track(op);

    return (PyObject*)op;
}

//int
//GtpyTaskDistributor_init(GtpyTaskDistributor* /*self*/, PyObject* /*args*/,
//                        PyObject* /*kwds*/)
//{
//    qDebug() << "INIT TASK DIST";
//    return 0;
//}

static void
GtpyTaskDistributor_dealloc(GtpyTaskDistributor* m)
{
//    qDebug() << "DEALOC TASK DISTRIBUTOR  "  << m->ob_refcnt;
    PyObject_GC_UnTrack(m);
    Py_XDECREF(m->m_context);
    m->m_context = Q_NULLPTR;

//    m->m_self = (PyObject*)dis_free_list;

//    dis_free_list = m;
}

static PyObject*
GtpyTaskDistributor_repr(PyObject* /*self*/)
{
    return PyString_FromString("GtpyTaskDistributor()");
}

static PyObject*
GtpyTaskDistributor_str(PyObject* /*self*/)
{
    return PyString_FromString("GtpyTaskDistributor()");
}

static PyObject*
GtpyTaskDistributor_call(PyObject* self, PyObject* args, PyObject* /*kw*/)
{
    qDebug() << "CALL TASK DIST!!";

    GtpyTaskDistributor* dis = (GtpyTaskDistributor*)self;

    if (!dis)
    {
        return Q_NULLPTR;
    }

    qDebug() << "1";


    PyObject* taskObj = Q_NULLPTR;
    PyObject* keys = PyObject_Dir(dis->m_context);

    if (keys)
    {
        int count = PyList_Size(keys);
        PyObject* value = Q_NULLPTR;
        PyObject* key = Q_NULLPTR;
        QString keystr;

        for (int i = 0; i < count; i++)
        {
            key = PyList_GetItem(keys, i);

            if (!key)
            {
                continue;
            }

            Py_INCREF(key);

            const char* keychar = PyString_AsString(key);
            keystr = QString::fromUtf8(keychar);

            if (keystr == "__task")
            {
                value = PyObject_GetAttr(dis->m_context, key);

                if (!value || value->ob_type->tp_base !=
                        &PythonQtInstanceWrapper_Type)
                {
                    Py_DECREF(key);
                    return Q_NULLPTR;
                }

                taskObj = value;
            }

            Py_DECREF(key);
        }

        Py_DECREF(keys);
    }


    if (!taskObj)
    {
        return Q_NULLPTR;
    }

    PythonQtInstanceWrapper* wrappedTask = (PythonQtInstanceWrapper*)taskObj;




//    GtpyExtendedWrapper* wrapper = (GtpyExtendedWrapper*)dis->m_task;
//    qDebug() << "dis task == " << dis->wrapper;
//    if (!wrapper)
//    {
//        return Q_NULLPTR;
//    }
//    qDebug() << "2 " << wrapper->_obj;

//    QPointer<QObject> wrappedObj = wrapper->_obj->_obj;

//    qDebug() << "wrappeObj == " << wrappedObj;
//    if (!wrappedObj)
//    {
//        return Q_NULLPTR;
//    }
//    qDebug() << "3";

//    PyObject* globals = PyEval_GetGlobals();

//    if (!globals)
//    {
//        return Q_NULLPTR;
//    }

//    PyObject* wrappedTask = PyDict_GetItemString(globals, "__task");

//    if (!wrappedTask || wrappedTask->ob_type->tp_base !=
//            &PythonQtInstanceWrapper_Type)
//    {
//        return Q_NULLPTR;
//    }

//    QObject* taskObj = ((PythonQtInstanceWrapper*)wrappedTask)->_obj;

//    if (!taskObj)
//    {
//        return Q_NULLPTR;
//    }

    GtTask* parentTask = qobject_cast<GtTask*>(wrappedTask->_obj);

    Py_DECREF(wrappedTask);
    qDebug() << "TASK ==  "  << parentTask;

    if (!parentTask)
    {
        return Q_NULLPTR;
    }
//    qDebug() << "4";

    if (!PyTuple_Check(args) || PyTuple_Size(args) == 0)
    {
        return Q_NULLPTR;
    }

//    qDebug() << "first arg";
    PyObject* firstArg = PyTuple_GetItem(args, 0);

    if (!firstArg || !PyString_Check(firstArg))
    {
        return Q_NULLPTR;
    }

    Py_INCREF(firstArg);

    QString taskName = PyString_AsString(firstArg);

    Py_DECREF(firstArg);

    GtProcessData* data = gtApp->currentProject()->processData();

    if (!data)
    {
        return Q_NULLPTR;
    }

    GtTask* task = data->findDirectChild<GtTask*>(taskName);

    if (!task)
    {
        return Q_NULLPTR;
    }

////    QList<GtTask*> children = partentTask->findDirectChildren<GtTask*>();

////    foreach (GtTask* child, children)
////    {
////        if(child->uuid() == task->uuid())
////        {
////            delete child;
////            child = Q_NULLPTR;
////            break;
////        }
////    }

//    qDebug() << "clone task";

    task = qobject_cast<GtTask*>(task->clone());

    task->moveToThread(parentTask->thread());

    task->setParent(parentTask);

    GtpyDecorator decorator;

    return decorator.wrapGtObject(task);
//    return Q_NULLPTR;
}

//static PyMethodDef GtpyTaskDistributor_methods[] = {
//    {"taskElement", (PyCFunction)GtpyTaskDistributor_taskElement, METH_VARARGS,
//     "taskElement(name) --> returns the task with the given name"},
//    {Q_NULLPTR, Q_NULLPTR, 0, Q_NULLPTR}  /* Sentinel */
//};

//PyTypeObject
//GtpyTaskDistributor_Type =
//{
//    PyVarObject_HEAD_INIT(&PyType_Type, 0)
//    "GtpyTaskDistributor",             /*tp_name*/
//    sizeof(GtpyTaskDistributor),             /*tp_basicsize*/
//    0,                         /*tp_itemsize*/
//    (destructor)GtpyTaskDistributor_dealloc, /*tp_dealloc*/
//    0,                         /*tp_print*/
//    0,                         /*tp_getattr*/
//    0,                         /*tp_setattr*/
//    0,           /*tp_compare*/
//    GtpyTaskDistributor_repr,              /*tp_repr*/
//    0,                         /*tp_as_number*/
//    0,                         /*tp_as_sequence*/
//    0,                         /*tp_as_mapping*/
//    0,                         /*tp_hash */
//    0,                         /*tp_call*/
//    GtpyTaskDistributor_str,                         /*tp_str*/
//    0, /*tp_getattro*/
//    0,             /*tp_setattro*/
//    0,                         /*tp_as_buffer*/
//    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE
//#ifndef PY3K
//    | Py_TPFLAGS_CHECKTYPES
//#endif
//    , /*tp_flags*/
//    "GtpyTaskDistributor(name) --> returns the task with the given name",           /* tp_doc */
//    0,                   /* tp_traverse */
//    0,                   /* tp_clear */
//    0,                   /* tp_richcompare */
//    0,                   /* tp_weaklistoffset */
//    0,                   /* tp_iter */
//    0,                   /* tp_iternext */
//    #ifdef PY3K
//    GtpyTaskDistributor_methods,
//    #else
//    GtpyTaskDistributor_methods,             /* tp_methods */
//    #endif
//    0,//GtpyGtObjectWrapper_members,                   /* tp_members */
//    0,                   /* tp_getset */
//    0,                         /* tp_base */
//    0,                         /* tp_dict */
//    0,                         /* tp_descr_get */
//    0,                         /* tp_descr_set */
//    0,                         /* tp_dictoffset */
//    0,                       /* tp_init */
//    0,                         /* tp_alloc */
//    0,

//};

//PyTypeObject
//GtpyTaskDistributor_Type = {
//    PyVarObject_HEAD_INIT(&PyType_Type, 0)
//    "GtpyTaskDistributor",
//    sizeof(GtpyTaskDistributor),
//    0,
//    (destructor)GtpyTaskDistributor_dealloc,     /* tp_dealloc */
//    0,          /* tp_print */
//    0,          /* tp_getattr */
//    0,          /* tp_setattr */
//    0,      /* tp_compare */
//    (reprfunc)GtpyTaskDistributor_repr,      /* tp_repr */
//    0,          /* tp_as_number */
//    0,          /* tp_as_sequence */
//    0,          /* tp_as_mapping */
//    0,      /* tp_hash */
//    GtpyTaskDistributor_call,      /* tp_call */
//    0,          /* tp_str */
//    PyObject_GenericGetAttr,    /* tp_getattro */
//    0,          /* tp_setattro */
//    0,          /* tp_as_buffer */
//    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,/* tp_flags */
//    0,          /* tp_doc */
//    0,//(traverseproc)meth_traverse,    /* tp_traverse */
//    0,          /* tp_clear */
//    0,          /* tp_richcompare */
//    0,          /* tp_weaklistoffset */
//    0,          /* tp_iter */
//    0,          /* tp_iternext */
//    0, //meth_methods,          /* tp_methods */
//    0, //meth_members,       /* tp_members */
//    0, //meth_getsets,       /* tp_getset */
//    0,          /* tp_base */
//    0,          /* tp_dict */
//    0,                         /* tp_descr_get */
//    0,                         /* tp_descr_set */
//    0,                         /* tp_dictoffset */
//    GtpyTaskDistributor_init,                       /* tp_init */
//    0,                         /* tp_alloc */
//    0,
//};

//static int
//meth_compare(GtpyTaskDistributor* a,
//             GtpyTaskDistributor* b)
//{
////    if (a != b)
////    {
////        return (a < b) ? -1 : 1;
////    }

//    return 0;
//}

static int
meth_compare(GtpyTaskDistributor* /*a*/,
             GtpyTaskDistributor* /*b*/)
{
    return 0;
}

// for python 3.x
static PyObject*
meth_richcompare(GtpyTaskDistributor* a,
                 GtpyTaskDistributor* b, int op)
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

static long
meth_hash(GtpyTaskDistributor* a)
{
    long x;

    if (a == Q_NULLPTR)
    {
        x = 0;
    }
    else
    {
        x = PyObject_Hash((PyObject*)a);
    }

    return x;
}

PyTypeObject
GtpyTaskDistributor_Type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "GtpyTaskDistributor",
    sizeof(GtpyTaskDistributor),
    0,
    (destructor)GtpyTaskDistributor_dealloc,     /* tp_dealloc */
    0,          /* tp_print */
    0,          /* tp_getattr */
    0,          /* tp_setattr */
#ifdef PY3K
    0,
#else
    (cmpfunc)meth_compare,      /* tp_compare */
#endif
    (reprfunc)GtpyTaskDistributor_repr,      /* tp_repr */
    0,          /* tp_as_number */
    0,          /* tp_as_sequence */
    0,          /* tp_as_mapping */
    (hashfunc)meth_hash,      /* tp_hash */
    GtpyTaskDistributor_call,      /* tp_call */
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
