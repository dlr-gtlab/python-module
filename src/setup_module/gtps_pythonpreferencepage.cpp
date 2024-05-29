#include "gtps_pythonpreferencepage.h"
#include "ui_gtps_pythonpreferencepage.h"

#include "gt_settings.h"
#include "gt_colors.h"
#include "gt_icons.h"

#include "gtps_constants.h"
#include "gtps_globals.h"
#include "gtps_pythoninterpreter.h"

#include <QFileDialog>
#include <QDir>
#include <QString>

GtPythonPreferencePage::GtPythonPreferencePage() :
    GtPreferencesPage(tr("Python Environment"), nullptr),
    ui(std::make_unique<Ui::GtPythonPreferencePage>())
{
    setIcon(gt::gui::icon::python());
    setTitleShort("Python");

    ui->setupUi(this);

    ui->iconLabel->setPixmap(gt::gui::icon::python().pixmap(32,32));

    connect(ui->btnSelectPyExe, &QPushButton::clicked,
            this, &GtPythonPreferencePage::onBtnSelectPyExePressed);

    connect(ui->btnTestPyEnv, &QPushButton::clicked, this,
            &GtPythonPreferencePage::onBtnTestPyEnvPressed);
}

void
GtPythonPreferencePage::saveSettings(GtSettings& settings) const
{  
    settings.setSetting(gtps::settings::path(gtps::constants::PYEXE_S_ID),
                        ui->lePythonExe->text());

}

void
GtPythonPreferencePage::loadSettings(const GtSettings& settings)
{
    auto pathToPython = settings.getSetting(
                gtps::settings::path(gtps::constants::PYEXE_S_ID)).toString();

    ui->lePythonExe->setText(std::move(pathToPython));
}

void
GtPythonPreferencePage::onBtnSelectPyExePressed()
{
    auto currentPyExe = ui->lePythonExe->text();

    QFileDialog dlg(this, tr("Select python executable"));

    if (QFile(currentPyExe).exists())
    {
        dlg.selectFile(QDir::toNativeSeparators(currentPyExe));
    }

#ifdef WIN32
    dlg.setNameFilter("Python (*.exe)");
#else
    dlg.setNameFilter("Python Executable (python*)");
#endif

    dlg.setFilter(QDir::Files);
    dlg.setFileMode(QFileDialog::ExistingFile);

    if (!dlg.exec())
    {
        return;
    }

    auto selectedFiles = dlg.selectedFiles();
    ui->lePythonExe->setText(QDir::toNativeSeparators(selectedFiles[0]));
    onBtnTestPyEnvPressed();
}

void
GtPythonPreferencePage::onBtnTestPyEnvPressed()
{
    auto currentPyExe = ui->lePythonExe->text();
    ui->lbPythonStatus->clear();

    using Status = GtpsPythonInterpreter::Status;
    GtpsPythonInterpreter interpreter{currentPyExe};

    switch (interpreter.status())
    {
    case Status::Valid:
        setStatusTextColor(gt::gui::color::text());
        ui->lbPythonStatus->setText(tr("Python %1 found")
                                    .arg(interpreter.version().toString()));
        break;
    case Status::Invalid:
        setStatusTextColor(gt::gui::color::errorText());
        ui->lbPythonStatus->setText(tr("Error: invalid python executable!"));
        break;
    case Status::NotSupported:
        setStatusTextColor(gt::gui::color::errorText());
        ui->lbPythonStatus->setText(
                    tr("Error: Python %1 is not supported!")
                    .arg(gtps::apiVersionStr(interpreter.version())));
        break;
    default:
        break;
    }
}

void
GtPythonPreferencePage::setStatusTextColor(const QColor& color)
{
    auto palette = ui->lbPythonStatus->palette();
    palette.setColor(QPalette::Foreground, color);
    ui->lbPythonStatus->setPalette(palette);
}

GtPythonPreferencePage::~GtPythonPreferencePage() = default;
