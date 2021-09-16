/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionlocalwidget.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 16.09.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QVBoxLayout>
#include <QMenu>

#include "gt_application.h"
#include "gt_logging.h"

#include "gtpy_collectionlocalmodel.h"
#include "gtpy_collectionitemwidget.h"

#include "gtpy_collectionlocalwidget.h"

GtpyCollectionLocalWidget::GtpyCollectionLocalWidget(QWidget* parent) :
    QWidget(parent), m_contextMenuEnabled(true)
{
    QVBoxLayout* lay = new QVBoxLayout;
    lay->setContentsMargins(0, 0, 0, 0);

    m_view = new QTreeView;

    m_model = new GtpyCollectionLocalModel(this);
    m_view->setModel(m_model);

    m_view->setColumnWidth(0, 300);
    m_view->setColumnWidth(1, 25);

    m_view->setContextMenuPolicy(Qt::CustomContextMenu);

    lay->addWidget(m_view);

    setLayout(lay);

    connect(m_view, SIGNAL(clicked(QModelIndex)),
            SLOT(onItemClicked(QModelIndex)));
    connect(m_view, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(onCustomContextMenuRequested(QPoint)));
    connect(m_view->selectionModel(),
            SIGNAL(currentChanged(QModelIndex, QModelIndex)),
            SLOT(onCurrentChanged(QModelIndex, QModelIndex)),
            Qt::UniqueConnection);
    connect(m_view, SIGNAL(doubleClicked(QModelIndex)),
            SIGNAL(itemDoubleClicked(QModelIndex)));
}

void
GtpyCollectionLocalWidget::setItems(const QList<GtCollectionItem>& items)
{
    m_model->setItems(items);

    m_view->expandAll();
}

GtCollectionItem
GtpyCollectionLocalWidget::getItem(const QModelIndex& index)
{
    return m_model->itemFromIndex(index);
}

void
GtpyCollectionLocalWidget::setCustomContextMenuEnabled(bool val)
{
    m_contextMenuEnabled = val;
}

void
GtpyCollectionLocalWidget::setShowItemInfo(bool val)
{
    if (m_model == Q_NULLPTR)
    {
        return;
    }

    m_model->setShowInfoColumns(val);

    if (val)
    {
        m_view->setColumnWidth(0, 300);
        m_view->setColumnWidth(1, 25);
    }
    else
    {
        m_view->resizeColumnToContents(0);
    }
}

void
GtpyCollectionLocalWidget::onItemClicked(const QModelIndex& index)
{
    if (!index.isValid())
    {
        return;
    }

    if (index.column() != 1)
    {
        emit itemSelected(index);
        return;
    }

    GtCollectionItem item = m_model->itemFromIndex(index);

    if (!item.isValid())
    {
        return;
    }

    GtpyCollectionItemWidget* widget = new GtpyCollectionItemWidget(item);

    widget->show();
}

void
GtpyCollectionLocalWidget::onCustomContextMenuRequested(const QPoint& /*pos*/)
{
    if (!m_contextMenuEnabled)
    {
        return;
    }

    QModelIndex index = m_view->currentIndex();

    if (!index.isValid())
    {
        return;
    }

    GtCollectionItem item = m_model->itemFromIndex(index);

    if (!item.isValid())
    {
        return;
    }

    QMenu menu;

    QAction* deleteAction = menu.addAction(
                                gtApp->icon(QStringLiteral("closeIcon_16.png")),
                                tr("Uninstall"));

    QAction* a = menu.exec(QCursor::pos());

    if (a == deleteAction)
    {
        gtDebug() << "deleting item...";
        gtDebug() << "  |-> " << item.localPath();
        gtDebug() << "  |-> " << index;

        if (!m_model->uninstallItem(index))
        {
            gtError() << tr("Could not uninstall collection item!");
        }
    }
}

void
GtpyCollectionLocalWidget::onCurrentChanged(const QModelIndex& current,
        const QModelIndex& /*previous*/)
{
    emit itemSelected(current);
}
