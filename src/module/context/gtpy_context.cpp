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
#include "gt_version.h"
#include "gt_coreapplication.h"

#include "gtpy_gilscope.h"
#include "gtpy_code.h"
#include "gtpy_pythonfunctions.h"
#include "gtpypp.h"

#include "gtpy_context.h"

namespace {

void initBatchContext(GtpyContext& context)
{
    context.evalScript(gtpy::code::import::defaultModules());
    context.evalScript(gtpy::code::import::loggingFunctions());

    context.evalScript(gtpy::code::enableAppConsoleLogging(true));

    if (!gtApp) return;

    const QString gtlabIdent{"GTlab"};

    context.addObject(gtlabIdent, gtApp);
    context.evalScript(gtpy::code::enableGTlabControl(gtlabIdent));
}

void initGlobalContext(GtpyContext& context)
{
    initBatchContext(context);

    // TODO: Figure out why we need to run this Python code
    context.evalScript("import sys\nsys.argv.append('')\ndel sys\n");
}

void initScriptEditorContext(GtpyContext& context)
{
    context.evalScript(gtpy::code::import::defaultModules());
    context.evalScript(gtpy::code::import::loggingFunctions());
}

void initCalculatorRunContext(GtpyContext& context)
{
    initScriptEditorContext(context);

    context.evalScript(gtpy::code::enableAppConsoleLogging(true));
}

void initTaskEditorContext(GtpyContext& context)
{
    context.evalScript(gtpy::code::import::defaultModules());
    context.evalScript(gtpy::code::import::loggingFunctions());
    context.evalScript(gtpy::code::import::calculatorModule());
}

void initTaskRunContext(GtpyContext& context)
{
    initTaskEditorContext(context);

    context.evalScript(gtpy::code::enableAppConsoleLogging(true));
}

void initContext(GtpyContext::Type type, GtpyContext& context)
{
    switch (type)
    {
    case GtpyContext::BatchContext:
        initBatchContext(context);
        break;

    case GtpyContext::GlobalContext:
        initGlobalContext(context);
        break;

    case GtpyContext::ScriptEditorContext:
        initScriptEditorContext(context);
        break;

    case GtpyContext::CalculatorRunContext:
        initCalculatorRunContext(context);
        break;

    case GtpyContext::TaskEditorContext:
        initTaskEditorContext(context);
        break;

    case GtpyContext::TaskRunContext:
        initTaskRunContext(context);
        break;

    case GtpyContext::CollectionContext:
        break;
    }
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

    // add functions via Python C API
    addFunctions(gtpy::extension::func::PROJECT_PATH_F_DEF);
#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    addFunctions(gtpy::extension::func::SHARED_FUNC_F_DEF);
#endif

    initContext(type, *this);
}

GtpyContext
GtpyContext::createContext(Type type)
{
    return std::move(GtpyContext{type});
}
