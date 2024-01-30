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

#include "gtpy_context.h"

GtpyContext::GtpyContext(gtpy::context::Type type)
{
    GTPY_GIL_SCOPE

    m_moduleName = QUuid::createUuid().toString();
    m_module = PythonQt::self()->createModuleFromScript(m_moduleName);
    Py_INCREF(m_module);

    auto initRoutine = gtpy::context::init::routine(type);
    initRoutine();
}

GtpyContext::~GtpyContext()
{
    GTPY_GIL_SCOPE

    PythonQtObjectPtr dict = PyImport_GetModuleDict();
    PyDict_DelItemString(dict, m_moduleName.toStdString().c_str());
}
