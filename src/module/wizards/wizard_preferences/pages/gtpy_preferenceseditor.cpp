/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_preferenceseditor.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.07.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QTableWidget>
#include <QFormLayout>
#include <QSpinBox>
#include <QCheckBox>

#include "gtpy_preferenceseditor.h"

GtpyPreferencesEditor::GtpyPreferencesEditor()
{
    setTitle(tr("Editor"));

    QTabWidget* tabWidget = new QTabWidget;

    tabWidget->setContentsMargins(0, 0, 0, 0);

    layout()->addWidget(tabWidget);

    QWidget* generalPage = new QWidget;
    tabWidget->addTab(generalPage, tr("General"));

    QVBoxLayout* generalLayout = new QVBoxLayout(generalPage);

    QFormLayout* formLay = new QFormLayout;

    QSpinBox* m_maxLogSpin = new QSpinBox;
    m_maxLogSpin->setMinimum(1);
    m_maxLogSpin->setMaximum(100);
    m_maxLogSpin->setValue(4);

    QCheckBox* m_replaceBySpaces =
        new QCheckBox(tr("Replace tab by spaces"));

    QHBoxLayout* tabLay = new QHBoxLayout();

    tabLay->addWidget(m_maxLogSpin);
    tabLay->addWidget(m_replaceBySpaces);

    formLay->addRow(tr("Tab size:"), tabLay);
    generalLayout->addLayout(formLay);

    generalLayout->addStretch(1);
}

void
GtpyPreferencesEditor::saveSettings()
{

}

void GtpyPreferencesEditor::loadSettings()
{

}
