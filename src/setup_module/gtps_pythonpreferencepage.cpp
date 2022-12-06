#include "gtps_pythonpreferencepage.h"
#include "ui_gtps_pythonpreferencepage.h"

#include "gt_settings.h"
#include "gt_moduleinterface.h"

#include "gtps_globals.h"
#include "gtps_pythoninterpreter.h"

#include <QFileDialog>
#include <QDir>
#include <QString>

GtPythonPreferencePage::GtPythonPreferencePage() :
    GtPreferencesPage(tr("Python Environment"), nullptr),
    ui(std::make_unique<Ui::GtPythonPreferencePage>())
{
    setIcon(QIcon(":/resources/python-icon-tinted_128.png"));
    setTitleShort("Python");

    ui->setupUi(this);

    connect(ui->btnSelectPyExe, &QPushButton::clicked,
            this, &GtPythonPreferencePage::onBtnSelectPyExePressed);

    connect(ui->btnTestPyEnv, &QPushButton::clicked, this,
            &GtPythonPreferencePage::onBtnTestPyEnvPressed);
}

void GtPythonPreferencePage::saveSettings(GtSettings & settings) const
{
    settings.setSetting(moduleSettingPath(GT_MODULENAME(), "pythonexe"),
                        ui->lePythonExe->text());
}

void GtPythonPreferencePage::loadSettings(const GtSettings & settings)
{
    auto pathToPython = settings.getSetting(moduleSettingPath(GT_MODULENAME(), "pythonexe"))
                            .toString();

    ui->lePythonExe->setText(std::move(pathToPython));
}

void GtPythonPreferencePage::onBtnSelectPyExePressed()
{
    auto currentPyExe = ui->lePythonExe->text();

    QFileDialog dlg(this, tr("Select python executable"));

    if (QFile(currentPyExe).exists())
    {
        dlg.selectFile(QDir::toNativeSeparators(currentPyExe));
    }

#ifdef WIN32
    dlg.setNameFilter("Python (*.exe)");
#endif

    dlg.setFilter(QDir::Executable | QDir::Files);
    dlg.setFileMode(QFileDialog::ExistingFile);

    if (!dlg.exec())
    {
        return;
    }

    auto selectedFiles = dlg.selectedFiles();
    ui->lePythonExe->setText(QDir::toNativeSeparators(selectedFiles[0]));
}

void GtPythonPreferencePage::onBtnTestPyEnvPressed()
{
    auto currentPyExe = ui->lePythonExe->text();
    GtpsPythonInterpreter interpreter{currentPyExe};

    auto pyVer = interpreter.version();

    if (!interpreter.isValid())
    {
        ui->lbPythonStatus->setText(tr("Error: invalid python executable!"));
    }
    else if (!gtps::validation::isSupported(pyVer))
    {
        ui->lbPythonStatus->setText(tr("Error: Python %1 is not supported!")
                                        .arg(gtps::apiVersionStr(pyVer)));
    }
    else
    {
        ui->lbPythonStatus->setText(tr("Python %1 found")
                                        .arg(pyVer.toString()));
    }
}


GtPythonPreferencePage::~GtPythonPreferencePage() = default;
