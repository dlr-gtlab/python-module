/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_code.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 10.01.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gtpy_code.h"


QString
gtpy::code::import::defaultModules()
{
    static const QString code = QStringLiteral(
            "from %1 import %2\n"
            "from %3 import %4\n"
            "from %5 import %6\n")
        .arg(gtpy::code::modules::GT_OBJECT_WRAPPER_MODULE_C,
             gtpy::code::classes::GTPY_EXTENDED_WRAPPER)
        .arg(gtpy::code::modules::PYTHON_QT, gtpy::code::modules::GT_CLASSES)
        .arg(gtpy::code::modules::PYTHON_QT, gtpy::code::modules::QTCORE);

    return code;
}

QString
gtpy::code::import::calculatorModule()
{
    static const QString code = QStringLiteral("__import__.%1()\n")
        .arg(gtpy::code::funcs::IMPORT_GT_CALCULATORS);

    return code;
}

QString
gtpy::code::import::loggingFunctions()
{
    // TODO: replace hardcoded function names with constant variables
    static const QString code = QStringLiteral(
        "from %1 import gtDebug, gtInfo, gtError, gtFatal, gtWarning\n")
        .arg(gtpy::code::modules::GT_LOGGING);

    return code;
}

QString
gtpy::code::enableAppConsoleLogging(bool enable)
{
    return QStringLiteral("%1 = %2\n")
        .arg(gtpy::code::attrs::LOGGING_ENABLED)
        .arg(enable ? "True" : "False");
}

QString
gtpy::code::removeFromSysModules(const QString& moduleName)
{
    //TODO: Check if Python C API Functions can be used in constructor of
    // GtpyContext to remove the module right after creating it
    return QStringLiteral(
               "import sys\n"
               "sys.modules['%1'] = None\n"
               "del sys\n")
        .arg(moduleName);
}
