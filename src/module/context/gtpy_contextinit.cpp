/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_context_init.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 30.01.2024
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QDebug>
#include <QObject>

#include "gt_logging.h"

#include "gtpy_constants.h"
#include "gtpy_gilscope.h"

#include "gtpy_contextinit.h"

namespace {

QString
fromAimportB(QString a, QString b)
{
    return QString{"from %1 import %2\n"}.arg(a, b);
}

void
evalCode(PythonQtObjectPtr& con, const QString& code)
{
    if (con.isNull())
    {
        gtDebug() << "Python code cannot be evaluated. "
                     "The given context is a nullptr.";
        return;
    }

    GTPY_GIL_SCOPE

    con.evalScript(code);
}

void
importDefaultModules(PythonQtObjectPtr& con)
{
    QString code = fromAimportB(
                gtpy::constants::modules::GT_OBJECT_WRAPPER_MODULE_C,
                gtpy::constants::classes::GTPY_EXTENDED_WRAPPER);
    code += fromAimportB("PythonQt", gtpy::constants::modules::GT_CLASSES);
    code += fromAimportB("PythonQt", "QtCore");

    evalCode(con, code);
}

void
importLoggingFuncs(PythonQtObjectPtr& con)
{
    auto fromGtLoggingImport = [](const QString& funcName){
        return fromAimportB(gtpy::constants::modules::GT_LOGGING, funcName);
    };

    QString code = fromGtLoggingImport("gtDebug");
    code += fromGtLoggingImport("gtInfo");
    code += fromGtLoggingImport("gtError");
    code += fromGtLoggingImport("gtFatal");
    code += fromGtLoggingImport("gtWarning");

    evalCode(con, code);
}

void
importCalculatorModule(PythonQtObjectPtr& con)
{
    QString code{"__import__.%1()"};
    code.arg(gtpy::constants::funcs::IMPORT_GT_CALCS);

    GTPY_GIL_SCOPE

    con.evalScript(code);
}

void
logToAppConsole(PythonQtObjectPtr& con, bool toAppConsole = true)
{
    QString code{gtpy::constants::attr::LOG_TO_APP_CONSOLE};

    if (toAppConsole)
        code += " = True\n";
    else
        code += " = False\n";

    evalCode(con, code);
}

}

void
gtpy::context::init::batchContext(PythonQtObjectPtr&)
{
    qDebug() << "init batchContext";
}

void
gtpy::context::init::taskRunContext(PythonQtObjectPtr& con)
{
    importDefaultModules(con);
    importLoggingFuncs(con);
    logToAppConsole(con);
    importCalculatorModule(con);
}

std::map<gtpy::context::Type, gtpy::context::init::InitFunc> initRoutineMap = {
    {gtpy::context::BatchContext, gtpy::context::init::batchContext},
    {gtpy::context::TaskRunContext, gtpy::context::init::taskRunContext}
};

gtpy::context::init::InitFunc
gtpy::context::init::routine(gtpy::context::Type type)
{
    return initRoutineMap.at(type);
}



