#include "gt_pythonpreferencepage.h"
#include "ui_gt_pythonpreferencepage.h"

#include "gt_settings.h"
#include "gt_moduleinterface.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QDir>

GtPythonPreferencePage::GtPythonPreferencePage() :
    GtPreferencesPage(tr("Python Environment"), nullptr),
    ui(std::make_unique<Ui::GtPythonPreferencePage>())
{
    setIcon(QIcon(":/resources/python-icon-tinted_128.png"));
    setTitleShort("Python");

    ui->setupUi(this);

    connect(ui->btnSelectPyExe, &QPushButton::clicked,
            this, &GtPythonPreferencePage::onBtnSelectPyExePressed);
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


GtPythonPreferencePage::~GtPythonPreferencePage() = default;
