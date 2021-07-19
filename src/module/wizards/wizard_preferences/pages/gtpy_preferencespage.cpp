/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_preferencespage.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.07.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QVBoxLayout>
#include <QLabel>
#include <QFont>
#include <QDebug>

#include "gtpy_preferencespage.h"

GtpyPreferencesPage::GtpyPreferencesPage(QWidget* parent) : QWidget(parent)
{
    m_layout = new QVBoxLayout;
    m_title = new QLabel("Preferences Page");

    QFont font = m_title->font();
    font.setBold(true);
    m_title->setFont(font);

    m_layout->addWidget(m_title);
    m_layout->addSpacing(20);

    setLayout(m_layout);
}

void
GtpyPreferencesPage::setTitle(const QString& id)
{
    m_title->setText(id);
}

void
GtpyPreferencesPage::addStretch(int val)
{
    m_layout->addStretch(val);
}
