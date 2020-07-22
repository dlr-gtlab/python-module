/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_calculatorsmodule.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 22.04.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QThread>

#include "PythonQtInstanceWrapper.h"

#include "gt_application.h"
#include "gt_processdata.h"
#include "gt_project.h"
#include "gt_calculator.h"
#include "gt_calculatordata.h"
#include "gt_calculatorfactory.h"
#include "gt_coreprocessexecutor.h"
#include "gt_abstractrunnable.h"

#include "gtpy_processdatadistributor.h"
#include "gtpy_calculatorfactory.h"
#include "gtpy_decorator.h"
#include "gtpy_gilscope.h"
#include "gtpy_extendedwrapper.h"

#include "gtpy_calculatorsmodule.h"

using namespace GtpyCalculatorsModule;

static GtTask*
findValue__task()
{
    GtTask* parentTask = Q_NULLPTR;

    PyObject* globals = PyEval_GetGlobals();

    if (globals)
    {
        Py_INCREF(globals);

        if (PyDict_Check(globals))
        {
            PyObject* taskVar = PyDict_GetItemString(globals,
                                GtpyGlobals::ATTR_task.
                                toStdString().data());

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
                    GtObject* taskObj = GtpyDecorator::pyObjectToGtObject(
                                            taskVar);
                    parentTask = qobject_cast<GtTask*>(taskObj);
                }

                Py_DECREF(taskVar);
            }
        }

        Py_DECREF(globals);
    }

    return parentTask;
}

static GtTask*
findTaskFromHigherFrame()
{
    GtTask* parentTask = Q_NULLPTR;

    PythonQtObjectPtr inspect;
    inspect.setNewRef(PyImport_ImportModule("inspect"));

    PyObject* dict = PyModule_GetDict(inspect);

    if (dict)
    {
        Py_INCREF(dict);

        PyObject* func = PyDict_GetItemString(dict, "currentframe");

        if (func)
        {
            Py_INCREF(func);

            if (PyCallable_Check(func))
            {
                PyObject* fram = PyObject_Call(func, PyTuple_New(0), NULL);
                PyObject* backFram = Q_NULLPTR;

                while (fram != Py_None)
                {
                    PyObject* frameGlobals = PyObject_GetAttrString(
                                                 fram, "f_globals");

                    if (frameGlobals)
                    {
                        if (PyDict_Check(frameGlobals))
                        {
                            PyObject* taskVar =
                                PyDict_GetItemString(frameGlobals,
                                                     GtpyGlobals::ATTR_task.
                                                     toStdString().data());

                            if (taskVar)
                            {
                                Py_INCREF(taskVar);

                                if (taskVar->ob_type->tp_base ==
                                        &PythonQtInstanceWrapper_Type)
                                {
                                    PythonQtInstanceWrapper* wrapper =
                                        (PythonQtInstanceWrapper*)taskVar;

                                    if (wrapper && wrapper->_obj)
                                    {
                                        parentTask = qobject_cast<GtTask*>(
                                                         wrapper->_obj);
                                    }
                                }
                                else
                                {
                                    GtObject* taskObj =
                                        GtpyDecorator::pyObjectToGtObject(
                                            taskVar);
                                    parentTask = qobject_cast<GtTask*>(taskObj);
                                }

                                Py_DECREF(taskVar);
                            }
                        }

                        Py_DECREF(frameGlobals);
                    }

                    backFram = PyObject_GetAttrString(fram,
                                                      "f_back");

                    Py_DECREF(fram);

                    fram = backFram;
                }
            }

            Py_DECREF(func);
        }

        Py_DECREF(dict);
    }

    return parentTask;
}

static GtTask*
findTaskByRunnable()
{
    GtTask* parentTask = Q_NULLPTR;

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

    return parentTask;
}

GtTask*
GtpyCalculatorsModule::findRunningParentTask()
{
    GtTask* parentTask = Q_NULLPTR;

    parentTask = findValue__task();

    if (!parentTask)
    {
        parentTask = findTaskFromHigherFrame();
    }

    if (!parentTask)
    {
        parentTask = findTaskByRunnable();
    }

    return parentTask;
}

QString
calcClassName(GtpyCreateCalculator* func)
{
    QString className;

    if (func->m_calcClassName)
    {
        if (PyString_Check(func->m_calcClassName))
        {
            className = PyString_AsString(func->m_calcClassName);
        }
    }

    return className;
}

static PyObject*
GtpyCreateCalculator_new(PyTypeObject* type, PyObject* args,
                         PyObject* /*kwds*/)
{
    GtpyCreateCalculator* self;
    self = (GtpyCreateCalculator*)type->tp_alloc(type, 0);

    if (!args)
    {
        QString error =  "Missing 1 required positional argument: name of "
                         "calculator class";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return Q_NULLPTR;
    }

    int argsCount = PyTuple_Size(args);

    if (argsCount < 1)
    {
        QString error =  "Missing 1 required positional argument: name of "
                         "calculator class";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return Q_NULLPTR;
    }
    else if (argsCount > 1)
    {
        QString error = "Takes 1 positional argument but ";
        error += QString::number(argsCount);
        error += " were given";

        PyErr_SetString(PyExc_TypeError, error.toStdString().c_str());

        return Q_NULLPTR;
    }

    PyObject* arg = PyTuple_GetItem(args, 0);

    if (!arg)
    {
        QString error =  "Missing 1 required positional argument: name of "
                         "calculator class";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return Q_NULLPTR;
    }

    Py_INCREF(arg);

    if (!PyString_Check(arg))
    {
        QString error =  "Calculator class name has to be a string";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        Py_DECREF(arg);
        return Q_NULLPTR;
    }

    self->m_calcClassName = arg;

    return (PyObject*)self;
}

static void
GtpyCreateCalculator_dealloc(GtpyCreateCalculator* self)
{
    if (self->m_calcClassName)
    {
        Py_DECREF(self->m_calcClassName);
        self->m_calcClassName = Q_NULLPTR;
    }

    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject*
GtpyCreateCalculator_Call(PyObject* func, PyObject* args,
                          PyObject* /*kwds*/)
{
    GtpyCreateCalculator* f = (GtpyCreateCalculator*)func;

    GtTask* parentTask = findRunningParentTask();

    if (!parentTask)
    {
        QString error = calcClassName(f) + "(name) --> can not find a " +
                        "running parent task!";

        PyErr_SetString(PyExc_RuntimeError, error.toLatin1().data());

        return Q_NULLPTR;
    }

    QString objName;

    if (args)
    {
        int argsCount = PyTuple_Size(args);

        if (argsCount > 1)
        {
            QString error = calcClassName(f) + "(name) --> takes 1" +
                            " positional argument but ";
            error += QString::number(argsCount);
            error += " were given";

            PyErr_SetString(PyExc_TypeError, error.toStdString().c_str());

            return Q_NULLPTR;
        }

        if (argsCount == 1)
        {
            PyObject* arg = PyTuple_GetItem(args, 0);

            if (arg)
            {
                Py_INCREF(arg);

                if (!PyString_Check(arg))
                {
                    QString error = calcClassName(f) + "(name) --> "
                                    "object name has to be a string";

                    PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

                    Py_DECREF(arg);
                    return Q_NULLPTR;
                }

                objName = PyString_AsString(arg);

                Py_DECREF(arg);
            }
        }
    }

    GtpyCalculatorFactory fac;

    GtCalculator* calc =
        fac.createCalculator(calcClassName(f), objName, parentTask);

    PyObject* obj = GtpyDecorator::wrapGtObject(calc);

    return obj;
}

PyTypeObject
GtpyCalculatorsModule::GtpyCreateCalculator_Type =
{
    PyVarObject_HEAD_INIT(NULL, 0)
    "GtpyCreateCalculator",             /*tp_name*/
    sizeof(GtpyCreateCalculator),             /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)GtpyCreateCalculator_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,           /*tp_compare*/
    0,              /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    GtpyCreateCalculator_Call,        /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "GtpyCreateCalculator doc",           /* tp_doc */
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
    GtpyCreateCalculator_new,                 /* tp_new */
};

PyObject*
GtpyCalculatorsModule::findGtTask_C_function(PyObject* /*self*/,
        PyObject* args)
{
    GTPY_GIL_SCOPE

    GtTask* parentTask = findRunningParentTask();

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
        QString error = "findGtTask(name) takes 1 positional argument but ";
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

    GtTask* task = distributor.taskElement(taskName);

    if (!task)
    {
        QString error =  "findGtTask(name) --> no task is named " + taskName;

        PyErr_SetString(PyExc_SystemError, error.toLatin1().data());

        return Q_NULLPTR;
    }

    return GtpyDecorator::wrapGtObject(task);
}

void
GtpyCalculatorsModule::createCalcConstructors()
{
    PythonQtObjectPtr mod =
        PyImport_ImportModule(
            QSTRING_TO_CHAR_PTR(GtpyGlobals::MODULE_GtCalculators));

    if (!mod)
    {
        return;
    }

    foreach (GtCalculatorData calcData,
             gtCalculatorFactory->calculatorDataList())
    {
        if (!gtApp->devMode() &&
                calcData->status != GtCalculatorDataImpl::RELEASE)
        {
            continue;
        }

        QString className = QString::fromUtf8(
                                calcData->metaData().className());

        PyObject* argsTuple = PyTuple_New(1);

        PyTuple_SetItem(argsTuple, 0, PyString_FromString(
                            className.toStdString().data()));

        PythonQtObjectPtr calcConstructor = GtpyCreateCalculator_Type.tp_new(
                                                &GtpyCreateCalculator_Type, argsTuple, NULL);

        Py_DECREF(argsTuple);

        PyModule_AddObject(mod, className.toStdString().data(), calcConstructor);
    }
}
