/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_abstractsettingspage.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 19.07.2021
 * Author: Marvin Noethen (AT-TW)
 */

#include <QVBoxLayout>
#include <QLabel>
#include <QFont>

#include "gtpy_abstractsettingspage.h"

GtpyAbstractSettingsPage::GtpyAbstractSettingsPage(QWidget* parent) : QWidget(
        parent)
{
    m_layout = new QVBoxLayout;
    m_title = new QLabel("Settings Page");

    QFont font = m_title->font();
    font.setBold(true);
    m_title->setFont(font);

    m_layout->addWidget(m_title);
    m_layout->addSpacing(20);

    setLayout(m_layout);
}

GtpyAbstractSettingsPage::~GtpyAbstractSettingsPage()
{

}

void
GtpyAbstractSettingsPage::setTitle(const QString& id)
{
    m_title->setText(id);
}

void
GtpyAbstractSettingsPage::addStretch(int val)
{
    m_layout->addStretch(val);
}
