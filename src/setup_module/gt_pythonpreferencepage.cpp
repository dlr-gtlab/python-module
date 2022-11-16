#include "gt_pythonpreferencepage.h"
#include "ui_gt_pythonpreferencepage.h"

#include "gt_icons.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>



GtPythonPreferencePage::GtPythonPreferencePage() :
    GtPreferencesPage(tr("Python Environment"), nullptr),
    ui(std::make_unique<Ui::GtPythonPreferencePage>())
{
    setIcon(QIcon(":/resources/python-icon-tinted_128.png"));
    setTitleShort("Python");

    ui->setupUi(this);
}

void GtPythonPreferencePage::saveSettings(GtSettings &) const
{

}

void GtPythonPreferencePage::loadSettings(const GtSettings &)
{

}


GtPythonPreferencePage::~GtPythonPreferencePage() = default;
