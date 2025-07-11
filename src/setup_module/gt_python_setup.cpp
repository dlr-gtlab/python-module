/* GTlab - Gas Turbine laboratory
 * Source File: gt_python_setup.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 17.10.2022
 * Author: Marvin Nöthen (DLR AT-TWK)
 */

#include "gt_python_setup.h"

#include "gt_application.h"
#include "gt_logging.h"
#include "gt_settings.h"
#include "gt_icons.h"

#include "gtps_constants.h"
#include "gtps_globals.h"
#include "gtps_systemsettings.h"
#include "gtps_pythoninterpreter.h"
#include "gtps_pythonpreferencepage.h"

#include <QLibrary>
#include <QMessageBox>
#include <QCheckBox>
#include <QCoreApplication>
#include <QDir>
#include <QFile>

GtVersionNumber
GtPythonSetupModule::version()
{
    return GtVersionNumber(1, 8, 0);
}

QString
GtPythonSetupModule::description() const
{
    return QStringLiteral("GTlab Python Setup Module");
}

void
GtPythonSetupModule::onLoad()
{
    gtps::system::clearPythonVars();

    // register current python environment path to settings
    gtApp->settings()->registerSettingRestart(
                gtps::settings::path(gtps::constants::PYEXE_S_ID), "");

    // register show python path warning to settings
    gtApp->settings()->registerSetting(
                gtps::settings::path(gtps::constants::SHOWHINT_S_ID), true);

    // register show python path warning to settings
    gtApp->settings()->registerSettingRestart(
        gtps::settings::path(gtps::constants::USE_EMBEDDED_S_ID), true);

    auto errorOccurred = [this](const ErrorMsg& msg){
        suppressPythonModules(gtps::python::version::supportedVersions());
        m_errorMsg = msg;
    };

    auto python = gtps::pythonPath();

    if (python.isEmpty())
    {
        errorOccurred({tr("No Python interpreter specified."),
                     tr("Do you want to specify a Python interpreter?")});
        return;
    }

    // Validate selected Python interpreter
    using Status = GtpsPythonInterpreter::Status;
    GtpsPythonInterpreter interpreter{python};

    switch (interpreter.status())
    {
    case Status::Valid:
        acceptPythonModule(interpreter.version());
        setPythonPaths(interpreter);
        // Next time the Python executable is invalid, the hint message box is
        // shown again.
        gtps::settings::setSetting(gtps::constants::SHOWHINT_S_ID, true);
        break;
    case Status::Invalid:
        errorOccurred({tr("The specified Python interpreter is invalid."),
                       tr("Do you want to specify another Python "
                       "interpreter?")});
        break;
    case Status::NotSupported:
        errorOccurred({tr("Python %1 is not supported.")
                       .arg(gtps::apiVersionStr(interpreter.version())),
                       tr("Do you want to specify another Python "
                       "iterpreter?")});
        // Set the GTlab environment variable PYTHONHOME to avoid problems
        // with other modules using this variable.
        gtps::system::setGtlabPythonHome(interpreter.pythonHomePath().toUtf8());
        break;
    default:
        break;
    }
}

void
GtPythonSetupModule::init()
{
    auto pageFactory = []() -> GtPreferencesPage* {
        return new GtPythonPreferencePage;
    };
    GtApplication::addCustomPreferencePage(pageFactory);

    if (!m_errorMsg.error.isEmpty())
    {
        showPythonErrorNotification(m_errorMsg);
    }
}

void
GtPythonSetupModule::setPythonPaths(const GtpsPythonInterpreter& interpreter)
{
    QString pySysPaths{
        interpreter.sysPaths().join(gtps::system::pathSeperator())};
    QString pyHome = interpreter.pythonHomePath();

    gtps::system::setPythonPath(pySysPaths.toUtf8());
    gtps::system::setPythonHome(pyHome.toUtf8());

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
}

void
GtPythonSetupModule::showPythonErrorNotification(const ErrorMsg& msg)
{
    if (gtApp->batchMode())
    {
        gtError() << msg.error;
        return;
    }

    if (gtps::settings::getSetting(gtps::constants::SHOWHINT_S_ID).toBool())
    {
        QMessageBox mb;
        mb.setIcon(QMessageBox::Question);
        mb.setWindowTitle(tr("Python setup"));
        mb.setWindowIcon(gt::gui::icon::python());
        mb.setText(QString{"%1\n%2"}.arg(msg.error, msg.task));
        mb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        mb.setDefaultButton(QMessageBox::Yes);

        QCheckBox* cb = new QCheckBox(tr("Don't ask me again."));
        mb.setCheckBox(cb);

        auto ret = mb.exec();

        gtps::settings::setSetting(gtps::constants::SHOWHINT_S_ID, !cb->isChecked());

        if (ret == QMessageBox::Yes)
        {
            gtApp->showPreferences("Python Environment");
        }
    }
    else
    {
        gtInfo() << msg.error <<
                    tr("To set up Python go to Edit > Preferences > Python.");
    }
}

void
GtPythonSetupModule::acceptPythonModule(const GtVersionNumber& version)
{
    auto versions = gtps::python::version::supportedVersions();
    versions.removeOne(GtVersionNumber{version.major(), version.minor()});
    suppressPythonModules(versions);
}

void
GtPythonSetupModule::suppressPythonModules(
        const QVector<GtVersionNumber>& pyVersions)
{
    auto suppress = [this](const GtVersionNumber& version) {
        gtApp->addSuppression(*this,
                              gtps::python::module::pythonModuleId(version));
    };
    std::for_each(pyVersions.begin(), pyVersions.end(), suppress);
}
