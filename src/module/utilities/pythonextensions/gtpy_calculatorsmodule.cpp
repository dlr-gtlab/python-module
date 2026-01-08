/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_calculatorsmodule.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 22.04.2020
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include <QThread>

#include "PythonQtInstanceWrapper.h"

#include "gt_application.h"
#include "gt_calculator.h"
#include "gt_calculatordata.h"
#include "gt_calculatorfactory.h"
#include "gt_abstractrunnable.h"

#include "gtpy_code.h"
#include "gtpy_processdatadistributor.h"
#include "gtpy_calculatorfactory.h"
#include "gtpy_decorator.h"
#include "gtpy_gilscope.h"

#include "gtpy_calculatorsmodule.h"
#include "gtpypp.h"

using namespace GtpyCalculatorsModule;

static GtTask*
findValue__task()
{

    auto globals = PyPPEval_GetGlobals();
    if (!globals || !PyPPDict_Check(globals)) return nullptr;

    auto taskVar = PyPPDict_GetItem(globals, gtpy::code::attrs::TASK);

    if(!taskVar) return nullptr;

    if (taskVar->ob_type->tp_base == &PythonQtInstanceWrapper_Type)
    {
        PythonQtInstanceWrapper* wrapper =
            (PythonQtInstanceWrapper*)taskVar.get();

        if (wrapper && wrapper->_obj)
        {
            return qobject_cast<GtTask*>(wrapper->_obj);
        }
    }
    else
    {
        GtObject* taskObj = GtpyDecorator::pyObjectToGtObject(
            taskVar.get());
        return qobject_cast<GtTask*>(taskObj);
    }

    return nullptr;
}

static GtTask*
findTaskFromHigherFrame()
{

    auto inspect = PyPPImport_ImportModule("inspect");

    auto dict = PyPPModule_GetDict(inspect);
    if (!dict) return nullptr;


    auto func = PyPPDict_GetItem(dict, "currentframe");
    if (!func || !PyPPCallable_Check(func)) return nullptr;


    GtTask* parentTask = nullptr;

    auto fram = PyPPObject_Call(func, PyPPTuple_New(0));
    for (;fram; fram = PyPPObject_GetAttr(fram, "f_back"))
    {
        auto frameGlobals = PyPPObject_GetAttr(fram, "f_globals");
        if (!frameGlobals || !PyPPDict_Check(frameGlobals)) continue;

        auto taskVar = PyPPDict_GetItem(frameGlobals, gtpy::code::attrs::TASK);
        if (!taskVar) continue;

        if (taskVar->ob_type->tp_base ==
                &PythonQtInstanceWrapper_Type)
        {
            PythonQtInstanceWrapper* wrapper =
                (PythonQtInstanceWrapper*)taskVar.get();

            if (wrapper && wrapper->_obj)
            {
                parentTask = qobject_cast<GtTask*>(
                                 wrapper->_obj);
            }
        }
        else
        {
            GtObject* taskObj = GtpyDecorator::pyObjectToGtObject(taskVar.get());
            parentTask = qobject_cast<GtTask*>(taskObj);
        }
    }

    return parentTask;
}

static GtTask*
findTaskByRunnable()
{
    auto thread = QThread::currentThread();

    auto runnable = thread->findChild<GtAbstractRunnable*>(
                {}, Qt::FindDirectChildrenOnly);

    return runnable ? runnable->findChild<GtTask*>(
                          {}, Qt::FindDirectChildrenOnly) : nullptr;
}

GtTask*
GtpyCalculatorsModule::findRunningParentTask()
{
    GtTask* parentTask = nullptr;

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
        if (PyUnicode_Check(func->m_calcClassName))
        {
            className = PyUnicode_AsUTF8(func->m_calcClassName);
        }
    }

    return className;
}

static PyObjectAPIReturn
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

        return nullptr;
    }

    int argsCount = PyTuple_Size(args);

    if (argsCount < 1)
    {
        QString error =  "Missing 1 required positional argument: name of "
                         "calculator class";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return nullptr;
    }
    else if (argsCount > 1)
    {
        QString error = "Takes 1 positional argument but ";
        error += QString::number(argsCount);
        error += " were given";

        PyErr_SetString(PyExc_TypeError, error.toStdString().c_str());

        return nullptr;
    }

    auto arg = PyPPTuple_GetItem(PyPPObject::Borrow(args), 0);

    if (!arg)
    {
        QString error =  "Missing 1 required positional argument: name of "
                         "calculator class";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return nullptr;
    }

    if (!PyPPUnicode_Check(arg))
    {
        QString error =  "Calculator class name has to be a string";
        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return nullptr;
    }

    self->m_calcClassName = arg.release();

    return (PyObjectAPIReturn)self;
}

static void
GtpyCreateCalculator_dealloc(GtpyCreateCalculator* self)
{
    if (self->m_calcClassName)
    {
        Py_DECREF(self->m_calcClassName);
        self->m_calcClassName = nullptr;
    }

    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObjectAPIReturn
GtpyCreateCalculator_Call(PyObject* func, PyObject* args_py,
                          PyObject* /*kwds*/)
{
    GtpyCreateCalculator* f = (GtpyCreateCalculator*)func;

    GtTask* parentTask = findRunningParentTask();

    if (!parentTask)
    {
        QString error = calcClassName(f) + "(name) --> can not find a " +
                        "running parent task!";

        PyErr_SetString(PyExc_RuntimeError, error.toLatin1().data());

        return nullptr;
    }

    auto args = PyPPObject::Borrow(args_py);

    QString objName;

    //@TODO : Potential bug, what if objName is stays empty?, i.e. if args is NULL?
    if (args)
    {
        int argsCount = PyPPTuple_Size(args);

        if (argsCount > 1)
        {
            QString error = calcClassName(f) + "(name) --> takes 1" +
                            " positional argument but ";
            error += QString::number(argsCount);
            error += " were given";

            PyErr_SetString(PyExc_TypeError, error.toStdString().c_str());

            return nullptr;
        }

        if (argsCount == 1)
        {
            auto arg = PyPPTuple_GetItem(args, 0);

            if (arg)
            {

                if (!PyPPUnicode_Check(arg))
                {
                    QString error = calcClassName(f) + "(name) --> "
                                    "object name has to be a string";

                    PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

                    return nullptr;
                }

                objName = PyPPString_AsQString(arg);
            }
        }
    }

    GtpyCalculatorFactory fac{};

    auto* calc = fac.createCalculator(calcClassName(f), objName, parentTask);
    if (!calc) Py_RETURN_NONE;

    return GtpyDecorator::wrapGtObject(calc).release();
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

PyObjectAPIReturn
GtpyCalculatorsModule::findGtTask_C_function(PyObject* /*self*/,
        PyObject* args_py)
{
    GTPY_GIL_SCOPE

    GtTask* parentTask = findRunningParentTask();

    if (!parentTask)
    {
        QString error =  "findGtTask(name) --> can not find a "
                         "running parent task!";

        PyErr_SetString(PyExc_RuntimeError, error.toLatin1().data());

        return nullptr;
    }

    if (!args_py)
    {
        QString error =  "findGtTask(name) missing 1 "
                         "required positional argument: name of an "
                         "existing task";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return nullptr;
    }


    auto args = PyPPObject::Borrow(args_py);
    int argsCount = PyPPTuple_Size(args);

    if (argsCount < 1)
    {
        QString error =  "findGtTask(name) missing 1 "
                         "required positional argument: name of an "
                         "existing task";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());

        return nullptr;
    }
    else if (argsCount > 1)
    {
        QString error = "findGtTask(name) takes 1 positional argument but ";
        error += QString::number(argsCount);
        error += " were given";

        PyErr_SetString(PyExc_TypeError, error.toStdString().c_str());

        return nullptr;
    }

    auto arg = PyPPTuple_GetItem(args, 0);

    if (!arg)
    {
        QString error =  "findGtTask(name) missing 1 "
                         "required positional argument: name of an "
                         "existing task";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());
        return nullptr;
    }

    if (!PyPPUnicode_Check(arg))
    {
        QString error =  "findGtTask(name) --> name has to be a string";

        PyErr_SetString(PyExc_TypeError, error.toLatin1().data());
        return nullptr;
    }

    QString taskName = PyPPString_AsQString(arg);

    GtpyProcessDataDistributor distributor(parentTask);

    GtTask* task = distributor.taskElement(taskName);

    if (!task)
    {
        QString error =  "findGtTask(name) --> no task is named " + taskName;

        PyErr_SetString(PyExc_SystemError, error.toLatin1().data());

        return nullptr;
    }

    return GtpyDecorator::wrapGtObject(task, GtpyDecorator::ForcePython).release();
}

void
GtpyCalculatorsModule::createCalcConstructors()
{
    auto mod = PyPPImport_ImportModule(gtpy::code::modules::GT_CALCULATORS);

    if (!mod)
    {
        return;
    }

    foreach (GtCalculatorData calcData,
             gtCalculatorFactory->calculatorDataList())
    {
        if (!gtDevMode() &&
                calcData->status != GtCalculatorDataImpl::RELEASE)
        {
            continue;
        }

        QString className = QString::fromUtf8(
                                calcData->metaData().className());

        auto argsTuple = PyPPTuple_New(1);

        PyPPTuple_SetItem(argsTuple, 0, PyPPObject::fromQString(className));

        auto calcConstructor = PyPPObject::NewRef(
            GtpyCreateCalculator_Type.tp_new(&GtpyCreateCalculator_Type, argsTuple.get(), NULL));

        PyPPModule_AddObject(mod, className.toStdString().data(),
                             std::move(calcConstructor));
    }
}
