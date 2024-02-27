/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_context.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 30.01.2024
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QUuid>

#include "PythonQt.h"

#include "gtpy_gilscope.h"
#include "gtpy_decorator.h"

#include "gtpy_context.h"

GtpyContext::GtpyContext(gtpy::context::Type type)
{
    GTPY_GIL_SCOPE

    m_moduleName = QUuid::createUuid().toString();
    m_module = PythonQt::self()->createModuleFromScript(m_moduleName);
    Py_INCREF(m_module);

    auto initRoutine = gtpy::context::init::routine(type);
    initRoutine(m_module);


}

GtpyContext::~GtpyContext()
{
    GTPY_GIL_SCOPE

    PythonQtObjectPtr dict = PyImport_GetModuleDict();
    PyDict_DelItemString(dict, m_moduleName.toStdString().c_str());
}

void
GtpyContext::addQObject(const QString& name, QObject* obj)
{
    GTPY_GIL_SCOPE

    m_module.addObject(name, obj);
}

void
GtpyContext::addGtObject(const QString& name, GtObject* obj)
{
    GTPY_GIL_SCOPE

    auto wrappedObj = GtpyDecorator::wrapGtObject(obj);

    if (PyModule_AddObject(m_module, name.toUtf8().data(), wrappedObj) < 0)
    {
        Py_DECREF(wrappedObj);
    }
}

void
GtpyContext::addVariable(const QString& name, const QVariant& value)
{
    GTPY_GIL_SCOPE

    m_module.addVariable(name, value);
}

QVariant
GtpyContext::getVariable(const QString& name)
{
    GTPY_GIL_SCOPE

    return m_module.getVariable(name);
}

void
GtpyContext::remove(const QString& name)
{
    GTPY_GIL_SCOPE

    m_module.removeVariable(name);
}
