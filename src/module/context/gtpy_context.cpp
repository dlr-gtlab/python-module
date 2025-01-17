/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_context.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 10.01.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include <QUuid>
#include <QMutex>

#include "gt_logging.h"

#include "gtpy_gilscope.h"
#include "gtpy_code.h"

#include "gtpypp.h"

#include "gtpy_context.h"

namespace {

const QString taskRunContextInitCode()
{
    gtDebug() << gtpy::code::import::defaultModules();
    gtDebug() << gtpy::code::import::loggingFunctions();
    gtDebug() << gtpy::code::import::calculatorModule();

    return "";
}

const QString initPyCode(GtpyContext::Type type)
{
    switch (type)
    {
    case GtpyContext::BatchContext:

        break;
    case GtpyContext::GlobalContext:

        break;
    case GtpyContext::ScriptEditorContext:

        break;
    case GtpyContext::CalculatorRunContext:

        break;
    case GtpyContext::TaskEditorContext:

        break;
    case GtpyContext::TaskRunContext:

        return taskRunContextInitCode();
    case GtpyContext::CollectionContext:

        break;
    }

    return "";
}

}

GtpyContext::GtpyContext(Type type) : GtpyModule(QUuid::createUuid().toString())
{
    GTPY_GIL_SCOPE

    // remove created module from sys.modules dict
    if (auto modulesDict = PyPPImport_GetModuleDict())
    {
        PyPPDict_DelItem(modulesDict, moduleName().toUtf8().constData());
    }
}

GtpyContext
GtpyContext::createContext(Type type)
{
    auto context = GtpyContext{type};
    context.evalScript(initPyCode(type));

    return std::move(context);
}
