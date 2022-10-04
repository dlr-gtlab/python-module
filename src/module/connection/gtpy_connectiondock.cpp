/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_connectiondock.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 26.02.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */
#include "gtpy_connectiondock.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QPushButton>

#include "gt_logging.h"
#include "gt_application.h"
#include "gt_colors.h"
#include "gt_stylesheets.h"
#include "gt_icons.h"
#include "gt_listview.h"

GtpyConnectionDock::GtpyConnectionDock()
{
    setAcceptDrops(false);

    setObjectName(tr("Python Connections"));

    auto* widget = new QWidget(this);
    setWidget(widget);

    auto* widLayout = new QVBoxLayout;
    widLayout->setContentsMargins(0, 0, 0, 0);
    widLayout->setSpacing(0);
    widget->setLayout(widLayout);

    auto* btnLayout = new QHBoxLayout;
    btnLayout->setContentsMargins(0, 0, 0, 0);
    btnLayout->setSpacing(1);

    m_addConnectionBtn = new QPushButton(tr("Add..."));
    m_addConnectionBtn->setIcon(GtGUI::Icon::add16());
    m_addConnectionBtn->setToolTip(tr("Add New Connection"));
    m_addConnectionBtn->setEnabled(false);
    m_addConnectionBtn->setStyleSheet(GtStyleSheets::buttonStyleSheet());
    btnLayout->addWidget(m_addConnectionBtn);

    m_listView = new GtListView(this);
    m_listView->setFrameStyle(QListView::NoFrame);
    m_listView->setAlternatingRowColors(true);
    m_listView->installEventFilter(this);

    //m_datamodel = new GtpyConnectionModel(this);

    //m_listView->setModel(m_datamodel);

    auto* frame = new QFrame;
    frame->setAutoFillBackground(true);
    frame->setFrameShape(m_listView->frameShape());
    frame->setFrameStyle(m_listView->frameStyle());
    frame->setFrameShadow(m_listView->frameShadow());

    auto* frameLayout = new QVBoxLayout;
    frameLayout->setContentsMargins(0, 0, 0, 0);

    frameLayout->addWidget(m_listView);
    frame->setLayout(frameLayout);
    widLayout->addLayout(btnLayout);
    widLayout->addWidget(frame);

    connect(m_addConnectionBtn, SIGNAL(clicked(bool)), this,
            SLOT(onAddConnection()));
    connect(m_listView, SIGNAL(clicked(QModelIndex)), this,
            SLOT(onClicked(QModelIndex)));
    connect(this, SIGNAL(selectedObjectChanged(GtObject*)),
            gtApp, SIGNAL(objectSelected(GtObject*)));
}

Qt::DockWidgetArea
GtpyConnectionDock::getDockWidgetArea()
{
    return Qt::RightDockWidgetArea;
}

void
GtpyConnectionDock::projectChangedEvent(GtProject* project)
{
    gtDebug() << "++++++++++++++++++++++++++++++";
    gtDebug() << project;
    m_listView->setModel(nullptr);
    //delete m_datamodel;

    bool enable{false};

    if (project)
    {
        enable = true;
//        m_datamodel = new GtpyConnectionModel(this);
//        m_listView->setModel(m_datamodel);
    }

    m_addConnectionBtn->setEnabled(enable);
}
