/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionbrowser.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QTreeView>
#include <QVBoxLayout>
#include <QCheckBox>

#include "gtpy_collectionbrowsermodel.h"
#include "gtpy_collectionitemwidget.h"

#include "gtpy_collectionbrowsersortmodel.h"

#include "gtpy_collectionbrowser.h"

GtpyCollectionBrowser::GtpyCollectionBrowser(QWidget* parent) :
    GtAbstractBrowserWidget(parent)
{
    QVBoxLayout* lay = new QVBoxLayout;
    lay->setContentsMargins(0, 0, 0, 0);

    m_view = new QTreeView;

    m_model = new GtpyCollectionBrowserModel(this);
    m_sortModel = new GtpyCollectionBrowserSortModel(this);

    m_sortModel->sort(0, Qt::AscendingOrder);
    m_sortModel->setSourceModel(m_model);

    m_view->setModel(m_sortModel);

    m_view->setColumnWidth(0, 300);
    m_view->setColumnWidth(1, 25);

    lay->addWidget(m_view);

    setLayout(lay);

    connect(m_view, SIGNAL(clicked(QModelIndex)),
            SLOT(onItemClicked(QModelIndex)));

    connect(m_model, SIGNAL(selectionChanged()), SLOT(onItemSelectionChange()));
}

void
GtpyCollectionBrowser::setItems(
    const QList<GtCollectionNetworkItem>& installedItems,
    const QList<GtCollectionNetworkItem>& availableItems,
    const QList<GtCollectionNetworkItem>& updateAvailableItems)
{
    m_model->setCollectionData(installedItems,
                               availableItems,
                               updateAvailableItems);

    m_view->expandAll();
}

QList<GtCollectionNetworkItem>
GtpyCollectionBrowser::itemsToDownload()
{
    return m_model->selectedItems();
}

QList<GtCollectionNetworkItem>
GtpyCollectionBrowser::itemsToUpdate()
{
    return m_model->itemsToUpdate();
}

void
GtpyCollectionBrowser::selectAllItems()
{
    m_model->selectAll();
    m_view->expandAll();
}

void
GtpyCollectionBrowser::unselectAllItems()
{
    m_model->unselectAll();
    m_view->expandAll();
}

QModelIndex
GtpyCollectionBrowser::mapToSource(const QModelIndex& index) const
{
    QModelIndex retval;

    if (m_sortModel)
    {
        retval = m_sortModel->mapToSource(index);
    }

    return retval;
}

void
GtpyCollectionBrowser::onItemClicked(const QModelIndex& index)
{
    if (!index.isValid())
    {
        return;
    }

    if (index.column() != 1)
    {
        return;
    }

    GtCollectionItem item = m_model->itemFromIndex(mapToSource(index));

    if (!item.isValid())
    {
        return;
    }

    GtpyCollectionItemWidget* widget = new GtpyCollectionItemWidget(item);

    widget->show();
}

void
GtpyCollectionBrowser::onItemSelectionChange()
{
    emit selectionChanged();
}
