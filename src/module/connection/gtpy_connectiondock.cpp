/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_connectiondock.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 26.02.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QPushButton>

#include "gt_logging.h"
#include "gt_project.h"
#include "gt_application.h"
#include "gt_colors.h"
#include "gt_stylesheets.h"
#include "gt_icons.h"
#include "gt_listview.h"

#include "gtpy_connection.h"
#include "gtpy_connectionmodel.h"


#include "gtpy_connectiondock.h"

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
    bool enable{false};

    m_listView->setModel(nullptr);

    delete m_datamodel;
    m_datamodel = nullptr;

    if (project)
    {
        enable = true;

        auto* connections =
                project->findDirectChild<GtpyConnectionContainer*>();

        if (!connections)
        {
            connections = new GtpyConnectionContainer;
            project->appendChild(connections);
        }

        m_datamodel = new GtpyConnectionModel(connections, this);
        m_listView->setModel(m_datamodel);
    }

    m_addConnectionBtn->setEnabled(enable);
}

void
GtpyConnectionDock::onAddConnection()
{
    if (!m_datamodel)
        return;
    auto* connection = new GtpyConnection;
    connection->setObjectName("connection");
    m_datamodel->addConnection(connection);
}

void
GtpyConnectionDock::onCurrentChanged(const QModelIndex &current,
                                     const QModelIndex &/*previous*/)
{
    if (!m_datamodel)
        return;
    auto* obj = m_datamodel->connectionFromIndex(current);
    emit selectedObjectChanged(obj);
}

void
GtpyConnectionDock::onClicked(const QModelIndex& index)
{
    onCurrentChanged(index);
}

