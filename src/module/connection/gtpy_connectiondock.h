/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_connectiondock.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 26.02.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */
#ifndef GTPYCONNECTIONDOCK_H
#define GTPYCONNECTIONDOCK_H

#include <QPointer>
#include <QModelIndex>

#include "gt_dockwidget.h"

#include "gtpy_connectioncontainer.h"

class QPushButton;
class GtListView;
class GtObject;
class GtpyConnectionModel;

class GtpyConnectionDock: public GtDockWidget
{
    Q_OBJECT

public:
    /**
     * @brief GtpyConnectionDock
     */
    Q_INVOKABLE GtpyConnectionDock();

    /** Returns the recommended dock widget area in main windows.
        @return dock widget area */
    virtual Qt::DockWidgetArea getDockWidgetArea();

protected:
    /** Event called after current project has changed.
        @param new current project */
    void projectChangedEvent(GtProject* project) override;

private:
    QPushButton* m_addConnectionBtn{};

    GtListView* m_listView{};

    GtpyConnectionModel* m_datamodel{};

    QPointer<GtpyConnectionContainer> m_connections{};

private slots:
    void onAddConnection();

    void onCurrentChanged(const QModelIndex& current,
                          const QModelIndex& previous = QModelIndex());

    void onClicked(const QModelIndex& index);

signals:
    void selectedObjectChanged(GtObject*);
};

#endif // GTPYCONNECTIONDOCK_H
