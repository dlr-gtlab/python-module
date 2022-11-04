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
    return GtpsPythonEvaluator{pythonExe()}.pythonVersion().toString();
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
    QString pyModuleId{"Python Module (Python %1.%2)"};

    if (!evaluator.isValid() || evaluator.pythonVersion().major() != 3)
    {
        /// Suppress all modules that require a valid Python environment
        gtApp->addSuppression(*this, pyModuleId.arg(3).arg(7));
        gtApp->addSuppression(*this, pyModuleId.arg(3).arg(9));
        return;
    }

    if (evaluator.pythonVersion().minor() != 7)    
        gtApp->addSuppression(*this, pyModuleId.arg(3).arg(7));

    if (evaluator.pythonVersion().minor() != 9)
        gtApp->addSuppression(*this, pyModuleId.arg(3).arg(9));

    setPythonPaths(evaluator);
}

void
GtPythonSetupModule::setPythonPaths(const GtpsPythonEvaluator& evaluator)
{
    QString pyCode = "import sys;print(', '.join([x for x in sys.path if x]), "
                      "end='')";

    bool ok{false};

    auto pyPaths = evaluator.eval(pyCode, &ok);

    if (!ok)
        return;

    QStringList pyPathsList{pyPaths.split(", ")};
    QString pathVar = qEnvironmentVariable("PATH");

    auto prependPath = [&pathVar](QString pyPath){
                      pyPath.replace("\\\\", "\\");
                      pathVar.prepend(QDir::toNativeSeparators(pyPath) + ";");};

    std::for_each(pyPathsList.begin(), pyPathsList.end(), prependPath);

    auto pyHome = QFileInfo(evaluator.pythonExe()).dir().path();

    qputenv("PATH", pathVar.toUtf8());
    qputenv("PYTHONHOME", QDir::toNativeSeparators(pyHome).toUtf8());
}
