/* GTlab - Gas Turbine laboratory
 * Source File: gtpytaskfindermodule.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 22.04.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYTASKFINDERMODULE_H
#define GTPYTASKFINDERMODULE_H

#include <QThread>

#include "PythonQtPythonInclude.h"
#include "PythonQtInstanceWrapper.h"

#include "gt_application.h"
#include "gt_processdata.h"
#include "gt_project.h"
#include "gt_task.h"
#include "gt_coreprocessexecutor.h"
#include "gt_abstractrunnable.h"

#include "gtpy_processdatadistributor.h"
#include "gtpy_decorator.h"
#include "gtpy_gilscope.h"


static PyObject*
findGtTask_C_function(PyObject* /*self*/, PyObject* args)
{
    GTPY_GIL_SCOPE

    GtTask* parentTask = Q_NULLPTR;

    PyObject* globals = PyEval_GetGlobals();

    if (globals)
    {
        Py_INCREF(globals);

        if (PyDict_Check(globals))
        {
            PyObject* taskVar = PyDict_GetItemString(globals, "__task");

            if (taskVar)
            {
                Py_INCREF(taskVar);

                if (taskVar->ob_type->tp_base == &PythonQtInstanceWrapper_Type)
                {
                    PythonQtInstanceWrapper* wrapper =
                            (PythonQtInstanceWrapper*)taskVar;

                    if (wrapper && wrapper->_obj)
                    {
                        parentTask = qobject_cast<GtTask*>(wrapper->_obj);
                    }
                }
                else
                {
                    GtpyDecorator decorator;
                    GtObject* taskObj = decorator.pyObjectToGtObject(taskVar);
                    parentTask = qobject_cast<GtTask*>(taskObj);
                }

                Py_DECREF(taskVar);
            }
        }

        Py_DECREF(globals);
    }

    if (!parentTask)
    {
        QThread* thread = QThread::currentThread();

        GtAbstractRunnable* runnable = thread->findChild<GtAbstractRunnable*>(
                    QString(), Qt::FindDirectChildrenOnly);

        if (runnable)
        {
            parentTask = runnable->findChild<GtTask*>(
                        QString(), Qt::FindDirectChildrenOnly);
        }
    }

    if (!parentTask)
    {
        QString error =  "findGtTask(name) --> can not find a "
                         "running parent task!";

        PyErr_SetString(PyExc_RuntimeError, error.toLatin1().data());

        return Q_NULLPTR;
    }

    if (!args)
    {
        QString error =  "findGtTask(name) missing 1 "
                         "required positional argument: name of an "
                         "existing task";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return Q_NULLPTR;
    }

    int argsCount = PyTuple_Size(args);

    if (argsCount < 1)
    {
        QString error =  "findGtTask(name) missing 1 "
                         "required positional argument: name of an "
                         "existing task";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return Q_NULLPTR;
    }
    else if (argsCount > 1)
    {
        QString error = "findGtTask(name) takes 1 positional arguments but ";
        error += QString::number(argsCount);
        error += " were given";

        PyErr_SetString(PyExc_TypeError, error.toStdString().c_str());

        return Q_NULLPTR;
    }

    PyObject* arg = PyTuple_GetItem(args, 0);

    if (!arg)
    {
        QString error =  "findGtTask(name) missing 1 "
                         "required positional argument: name of an "
                         "existing task";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return Q_NULLPTR;
    }

    Py_INCREF(arg);

    if (!PyString_Check(arg))
    {
        QString error =  "findGtTask(name) --> name has to be a string";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        Py_DECREF(arg);
        return Q_NULLPTR;
    }

    QString taskName = PyString_AsString(arg);

    Py_DECREF(arg);

    GtpyProcessDataDistributor distributor(parentTask);
    GtpyDecorator decorator;

    GtTask* task = distributor.taskElement(taskName);

    if (!task)
    {
        QString error =  "findGtTask(name) --> no task is named " + taskName;

        PyErr_SetString(PyExc_SystemError, error.toLatin1().data());

        return Q_NULLPTR;
    }


    return decorator.wrapGtObject(task);
}

static PyMethodDef
GtpyTaskFinder_StaticMethods[] = {
    { "findGtTask", findGtTask_C_function, METH_VARARGS, "GtTask* "
      "findGtTask(name) --> finds an existing GtTask by objectname and"
      " returns it" },
    { NULL, NULL, 0, NULL }
};


#ifdef PY3K
static PyModuleDef
GtpyTaskFinder_Module = {
  PyModuleDef_HEAD_INIT,
  GtpyConsts::GTPYTASKFINDER_MODULE.toStdString().data(),
  NULL,
  -1,
  GtpyTaskFinder_StaticMethods,
  NULL,
  NULL,
  NULL,
  NULL
};
#endif
#endif // GTPYTASKFINDERMODULE_H
