/* GTlab - Gas Turbine laboratory
 * Source File: gt_python_setup.cpp
 * copyright 2009-2022 by DLR
 *
 *  Created on: 17.10.2022
 *  Author: Marvin Nöthen (AT-TW)
 *  Tel.: +49 2203 601 2907
 */

#include <QMessageBox>

#include "gt_application.h"
#include "gt_settings.h"

#include "gtps_globals.h"
#include "gtps_pythoninterpreter.h"
#include "gtps_pythonpreferencepage.h"

#include "gt_python_setup.h"

namespace {

QString
pythonExe()
{
    return gtApp->settings()->getSetting(moduleSettingPath(GT_MODULENAME(),
                                         "pythonexe")).toString();
}

}

GtVersionNumber
GtPythonSetupModule::version()
{
    return GtVersionNumber(1, 2, 0);
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
    m_pyVersion = interpreter.version();

    if (!(m_isPythonValid = interpreter.isValid()) ||
        !gtps::validation::isSupported(m_pyVersion))
    {
        suppressPythonModules(gtps::supportedVersions());
        return;
    }

    acceptPythonModule(m_pyVersion);
    setPythonPaths(interpreter);
}

void GtPythonSetupModule::init()
{
    auto pageFactory = []() -> GtPreferencesPage* {
        return new GtPythonPreferencePage;
    };
    GtApplication::addCustomPreferencePage(pageFactory);

    if (pythonExe().isEmpty())
    {
        showNotification(tr("No Python interpreter specified.\n"
                            "Do you want to specify a Python interpreter?"));
    }
    else if (!m_isPythonValid)
    {
        showNotification(tr("The specified Python interpreter is invalid.\n"
                            "Do you want to specify another Python "
                            "interpreter?"));
    }
    else if (!gtps::validation::isSupported(m_pyVersion))
    {
        showNotification(tr("Python %1 is not supported.\n"
                            "Do you want to specify another Python iterpreter?")
                            .arg(gtps::apiVersionStr(m_pyVersion)));
    }
}

void
GtPythonSetupModule::setPythonPaths(const GtpsPythonInterpreter& interpreter)
{
    QString pathVar{qEnvironmentVariable("PATH")};
    pathVar.prepend(interpreter.sharedLibPath() + ";");
    QString pySysPaths{interpreter.sysPaths().join(";")};

    qputenv("PATH", pathVar.toUtf8());
    qputenv("PYTHONPATH", pySysPaths.toUtf8());
    qputenv("PYTHONHOME", interpreter.pythonHomePath().toUtf8());
}

void
GtPythonSetupModule::showNotification(const QString& msg)
{
    auto reply = QMessageBox::question(nullptr, tr("Python setup"), msg,
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        gtApp->showPreferences("Python Environment");
    }
}

void
GtPythonSetupModule::acceptPythonModule(const GtVersionNumber& version)
{
    auto versions = gtps::supportedVersions();
    versions.removeOne(GtVersionNumber{version.major(), version.minor()});
    suppressPythonModules(versions);
}

void
GtPythonSetupModule::suppressPythonModules(
        const QVector<GtVersionNumber>& pyVersions)
{
    auto suppress = [this](const GtVersionNumber& version) {
        gtApp->addSuppression(*this, gtps::pythonModuleId(version));
    };
    std::for_each(pyVersions.begin(), pyVersions.end(), suppress);
}


