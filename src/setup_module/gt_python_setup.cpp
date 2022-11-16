/* GTlab - Gas Turbine laboratory
 * Source File: gt_python_setup.cpp
 * copyright 2009-2022 by DLR
 *
 *  Created on: 17.10.2022
 *  Author: Marvin Nöthen (AT-TW)
 *  Tel.: +49 2203 601 2907
 */


#include "gt_python_setup.h"
#include "gt_pythonpreferencepage.h"

#include "gt_environment.h"
#include "gt_settings.h"
#include "gt_logging.h"

#include "gtps_pythoninterpreter.h"
#include "gt_python_setup.h"

#include <QDir>

namespace {

QString
pythonExe()
{
    return gtApp->settings()->getSetting(moduleSettingPath(GT_MODULENAME(), "pythonexe")).toString();
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

void
GtPythonSetupModule::onLoad()
{
    // register current python environment path to settings
    gtApp->settings()->registerSettingRestart(
        moduleSettingPath(GT_MODULENAME(), "pythonexe"), "");

    GtpsPythonInterpreter interpreter{pythonExe()};

    QString pyModuleId{"Python Module (Python %1.%2)"};

    if (!interpreter.isValid() || interpreter.pythonVersion().major() != 3)
    {
        /// Suppress all modules that require a valid Python environment
        gtApp->addSuppression(*this, pyModuleId.arg(3).arg(7));
        gtApp->addSuppression(*this, pyModuleId.arg(3).arg(9));
        return;
    }

    if (interpreter.pythonVersion().minor() != 7)
        gtApp->addSuppression(*this, pyModuleId.arg(3).arg(7));

    if (interpreter.pythonVersion().minor() != 9)
        gtApp->addSuppression(*this, pyModuleId.arg(3).arg(9));

    setPythonPaths(interpreter);
}

void GtPythonSetupModule::init()
{
    auto pageFactory = []() -> GtPreferencesPage* {
        return new GtPythonPreferencePage;
    };
    GtApplication::addCustomPreferencePage(pageFactory);
}

void
GtPythonSetupModule::setPythonPaths(const GtpsPythonInterpreter& interpreter)
{
    QString pathVar{qEnvironmentVariable("PATH")};
    pathVar.prepend(interpreter.sharedLibPath() + ";");

    qDebug() << interpreter.pythonHomePath();

    QString pySysPaths{interpreter.sysPaths().join(";")};
    qputenv("PATH", pathVar.toUtf8());
    qputenv("PYTHONPATH", pySysPaths.toUtf8());
    qputenv("PYTHONHOME", interpreter.pythonHomePath().toUtf8());
}
