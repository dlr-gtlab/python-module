/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_generalpage.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 19.07.2021
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include <QTableWidget>
#include <QFormLayout>
#include <QSpinBox>
#include <QCheckBox>

#include "gtpy_generalpage.h"

GtpyGeneralPage::GtpyGeneralPage(QWidget* parent) :
    GtpyAbstractSettingsPage(parent)
{
    setTitle(tr("Editor"));

    QTabWidget* tabWidget = new QTabWidget;

    tabWidget->setContentsMargins(0, 0, 0, 0);

    layout()->addWidget(tabWidget);

    QWidget* generalPage = new QWidget;
    tabWidget->addTab(generalPage, tr("General"));

    QVBoxLayout* generalLayout = new QVBoxLayout(generalPage);

    QFormLayout* formLay = new QFormLayout;

    m_tabSize = new QSpinBox;
    m_tabSize->setMinimum(1);
    m_tabSize->setMaximum(100);

    m_replaceBySpaces =
        new QCheckBox(tr("Replace tab by spaces"));

    QHBoxLayout* tabLay = new QHBoxLayout();

    tabLay->addWidget(m_tabSize);
    tabLay->addWidget(m_replaceBySpaces);

    formLay->addRow(tr("Tab size:"), tabLay);
    generalLayout->addLayout(formLay);

    generalLayout->addStretch(1);
}

void
GtpyGeneralPage::saveSettings(GtpyEditorSettings* pref)
{
    if (pref != nullptr)
    {
        pref->setTabSize(m_tabSize->value());
        pref->setReplaceTabBySpace(m_replaceBySpaces->isChecked());
    }
}

void
GtpyGeneralPage::loadSettings(GtpyEditorSettings* pref)
{
    if (pref != nullptr)
    {
        m_tabSize->setValue(pref->tabSize());
        m_replaceBySpaces->setChecked(pref->replaceTabBySpace());
    }
    else
    {
        m_tabSize->setValue(4);
        m_replaceBySpaces->setChecked(false);
    }
}
