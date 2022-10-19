/* GTlab - Gas Turbine laboratory
 * Source File: gt_python_setup.cpp
 * copyright 2009-2022 by DLR
 *
 *  Created on: 17.10.2022
 *  Author: Marvin Nöthen (AT-TW)
 *  Tel.: +49 2203 601 2907
 */

#include <QDir>

#include "gt_logging.h"
#include "gt_environment.h"
#include "gt_functional_interface.h"

#include "gtps_pythonevaluator.h"

#include "gt_python_setup.h"

namespace {

QString
pythonExe()
{
    return gtEnvironment->value("PYTHONEXE").toString();
}

QString
pythonVersion()
{
    return GtpsPythonEvaluator{pythonExe()}.pythonVersion();
}

}

GtVersionNumber
GtPythonSetupModule::version()
{
    return GtVersionNumber(1, 0, 0);
}

QString
GtPythonSetupModule::description() const
{
    return QStringLiteral("GTlab Python Setup Module");
}


QList<gt::InterfaceFunction>
GtPythonSetupModule::sharedFunctions() const
{
    auto help = "If the Python environment is valid, it returns the Python "
                "version. Otherwise it returns an empty string.";
    auto func = gt::interface::makeInterfaceFunction(
                "pythonVersion", pythonVersion, help);
    return {func};
}

void
GtPythonSetupModule::onLoad()
{
    GtpsPythonEvaluator evaluator{pythonExe()};

    if (!evaluator.isValid())
    {
        /// Suppress all modules that require a valid Python environment
        gtApp->addSuppression(*this, "Python Module (Python 3.7)");
        gtApp->addSuppression(*this, "Python Module (Python 3.9)");

        return;
    }
}
