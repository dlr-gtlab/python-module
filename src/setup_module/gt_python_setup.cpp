/* GTlab - Gas Turbine laboratory
 * Source File: gt_python_setup.cpp
 * copyright 2009-2022 by DLR
 *
 *  Created on: 17.10.2022
 *  Author: Marvin Nöthen (AT-TW)
 *  Tel.: +49 2203 601 2907
 */

#include "gt_application.h"
#include "gt_logging.h"
#include "gt_settings.h"
#include "gt_environment.h"

#include "gtps_globals.h"
#include "gtps_pythoninterpreter.h"
#include "gtps_pythonpreferencepage.h"

#include "gt_python_setup.h"

#include <QLibrary>
#include <QMessageBox>

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
    return GtVersionNumber(1, 2, 1);
}

QString
GtPythonSetupModule::description() const
{
    return QStringLiteral("GTlab Python Setup Module");
}

void
GtPythonSetupModule::onLoad()
{
    clearPythonPaths();

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
#ifdef _WIN32
    QString pathSeparator{";"};
#else
    QString pathSeparator{":"};
#endif

    QString pySysPaths{interpreter.sysPaths().join(pathSeparator)};
    QString pyHome = interpreter.pythonHomePath();

    qputenv("PYTHONPATH", pySysPaths.toUtf8());
    qputenv("PYTHONHOME", pyHome.toUtf8());

    gtDebug().medium() << "Python library == " << interpreter.sharedLib();
    gtDebug().medium() << "PYTHONPATH == " << pySysPaths;
    gtDebug().medium() << "PYTHONHOME == " << pyHome;


    // Setting path or LD_LIBRARY_PATH does not work on UNIX systems
    // as these variables is evaluated only at the start of each process
    // The workaround is to preload the python library instead

    QLibrary pythonlib(interpreter.sharedLib());
    pythonlib.setLoadHints(QLibrary::ExportExternalSymbolsHint);
    auto loadSuccess = pythonlib.load();
    if (!loadSuccess)
    {
        gtError() << "Python library could not be loaded.";
    }
    else
    {
        gtDebug().medium() << "Sucessfully loaded python library";
    }

    QVariant var = gtEnvironment->value("PYTHONHOME");

    if (!var.isNull())
    {
        if (var.toString() != pyHome)
        {
            gtInfo() << tr("PYTHONHOME has been set to ('%1')").arg(pyHome);
            gtEnvironment->setValue("PYTHONHOME", pyHome);
            gtEnvironment->saveEnvironment();
        }
    }
}

void
GtPythonSetupModule::clearPythonPaths()
{
    qunsetenv("PYTHONPATH");
    qunsetenv("PYTHONHOME");
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


