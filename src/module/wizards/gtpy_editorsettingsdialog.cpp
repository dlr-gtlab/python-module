/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_editorsettingsdialog.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 15.07.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QIcon>
#include <QListWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QHBoxLayout>

#include "gt_application.h"

#include "gtpy_editorsettingsdialog.h"

GtpyEditorSettingsDialog::GtpyEditorSettingsDialog()
{
    Qt::WindowFlags flags = windowFlags();
    flags = flags & (~Qt::WindowContextHelpButtonHint);
    setWindowFlags(flags);

    m_contentsWidget = new QListWidget;
    m_contentsWidget->setViewMode(QListView::IconMode);
    m_contentsWidget->setIconSize(QSize(32, 32));
    m_contentsWidget->setMovement(QListView::Static);
    m_contentsWidget->setMaximumWidth(135);
    m_contentsWidget->setSizePolicy(QSizePolicy::MinimumExpanding,
                                    QSizePolicy::Expanding);
    m_contentsWidget->setSpacing(12);
    m_contentsWidget->setFrameStyle(QFrame::NoFrame);

    m_pagesWidget = new QStackedWidget;
    m_pagesWidget->setMinimumWidth(400);

    QPushButton* saveButton = new QPushButton(tr("Save"));
    saveButton->setIcon(gtApp->icon("saveProjectIcon_16.png"));
    QPushButton* closeButton = new QPushButton(tr("Cancel"));
    closeButton->setIcon(gtApp->icon("closeIcon_16.png"));

    createIcons();
    m_contentsWidget->setCurrentRow(0);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveChanges()));

    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->setContentsMargins(4, 4, 4, 4);
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(saveButton);
    buttonsLayout->addWidget(closeButton);

    QVBoxLayout* verticalLayout = new QVBoxLayout;
    verticalLayout->addWidget(m_pagesWidget);
    verticalLayout->addLayout(buttonsLayout);

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(m_contentsWidget);
    mainLayout->addLayout(verticalLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);

    setWindowTitle(tr("Editor settings"));
    setWindowIcon(gtApp->icon("configIcon_16.png"));
    setFixedHeight(470);
    setFixedWidth(600);
}

void
GtpyEditorSettingsDialog::createIcons()
{
    QListWidgetItem* configButton = new QListWidgetItem(m_contentsWidget);
    configButton->setIcon(gtApp->icon("configIcon.png"));
    configButton->setText(tr("Settings"));
    configButton->setTextAlignment(Qt::AlignHCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    configButton->setWhatsThis(tr("Editor Settings"));
    configButton->setSizeHint(QSize(100, 50));
}

void
GtpyEditorSettingsDialog::saveChanges()
{

}
