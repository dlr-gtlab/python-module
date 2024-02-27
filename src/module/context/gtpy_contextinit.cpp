/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_context_init.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 30.01.2024
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QDebug>

#include "PythonQtObjectPtr.h"

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
    GTPY_GIL_SCOPE

    con.evalScript(code);
}

void
importDefaultModules(PythonQtObjectPtr& con)
{
    QString code = fromAimportB(
                gtpy::constants::modules::GTOBJECTWRAPPERMODULEC,
                gtpy::constants::classes::GTPYEXTENDEDWRAPPER);
    code += fromAimportB("PythonQt", gtpy::constants::modules::GTCLASSES);
    code += fromAimportB("PythonQt", "QtCore");

    evalCode(con, code);
}

void
importLoggingFuncs(PythonQtObjectPtr& con)
{
    auto fromGtLoggingImport = [](const QString& funcName){
        return fromAimportB(gtpy::constants::modules::GTLOGGING, funcName);
    };

    QString code = fromGtLoggingImport("gtDebug");
    code += fromGtLoggingImport("gtInfo");
    code += fromGtLoggingImport("gtError");
    code += fromGtLoggingImport("gtFatal");
    code += fromGtLoggingImport("gtWarning");

    evalCode(con, code);
}

void
logToAppConsole(PythonQtObjectPtr& con, bool toAppConsole = true)
{
    QString code{gtpy::constants::attr::LOGTOAPPCONSOLE};

    if (toAppConsole)
        code += " = True\n";
    else
        code += " = False\n";

    evalCode(con, code);
}

}

void
gtpy::context::init::batchContext()
{
    qDebug() << "init batchContext";
}

void
gtpy::context::init::taskRunContext()
{
    qDebug() << "init taskRunContext";
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



