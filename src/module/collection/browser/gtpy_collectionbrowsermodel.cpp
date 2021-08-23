/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionbrowsermodel.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QIcon>
#include <QDebug>

#include "gt_application.h"

#include "gtpy_collectionitem.h"
#include "gtpy_collectioncollapsibleitem.h"

#include "gtpy_collectionbrowsermodel.h"

GtpyCollectionBrowserModel::GtpyCollectionBrowserModel(QObject* parent) :
    QAbstractItemModel(parent)
{

}

GtpyCollectionBrowserModel::~GtpyCollectionBrowserModel()
{
    qDeleteAll(m_installedItems);
    qDeleteAll(m_updateAvailableItems);
    qDeleteAll(m_availableItems);
}

int
GtpyCollectionBrowserModel::rowCount(const QModelIndex& parent) const
{
    if (!parent.isValid())
    {
        return groupTypes().size();
    }

    const CollectionItemType itemType = static_cast<CollectionItemType>
                                        (parent.internalId());

    switch (itemType)
    {
        case UpdateAvailableItemGroup:
            return m_updateAvailableItems.size();

        case AvailableItemGroup:
            return m_availableItems.size();

        case InstalledItemGroup:
            return m_installedItems.size();
    }

    return 0;
}

int
GtpyCollectionBrowserModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 4;
}

QVariant
GtpyCollectionBrowserModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    const int col = index.column();

    const CollectionItemType itemType = static_cast<CollectionItemType>
                                        (index.internalId());

    switch (itemType)
    {
        case UpdateAvailableItemGroup:
        {
            if (role == Qt::DisplayRole && col == 0)
            {
                return tr("Update available!");
            }
            else if (role == Qt::DecorationRole && col == 0)
            {
                return gtApp->icon(QStringLiteral("updateIcon_16.png"));
            }
            else if (role == Qt::BackgroundRole)
            {
                return QColor(180, 229, 190);
            }

            break;
        }

        case AvailableItemGroup:
        {
            if (role == Qt::DisplayRole && col == 0)
            {
                return tr("Available");
            }
            else if (role == Qt::DecorationRole && col == 0)
            {
                return gtApp->icon(QStringLiteral("stackIcon.png"));
            }

            break;
        }

        case InstalledItemGroup:
        {
            if (role == Qt::DisplayRole && col == 0)
            {
                return tr("Installed");
            }
            else if (role == Qt::DecorationRole && col == 0)
            {
                return gtApp->icon(QStringLiteral("collectionIcon_16.png"));
            }
            else if (role == Qt::BackgroundRole)
            {
                return QColor(240, 240, 240);
            }

            break;
        }

        case UpdateAvailableItem:
        {
            if (role == Qt::DisplayRole)
            {
                const int row = index.row();

                if (row < 0 || row >= m_updateAvailableItems.size())
                {
                    return QVariant();
                }

                if (col == 0)
                {
                    return m_updateAvailableItems[row]->ident();
                }
                else if (col == 2)
                {
                    return m_updateAvailableItems[row]->version();
                }
                else if (col == 3)
                {
                    return
                        m_updateAvailableItems[row]->installedVersion();
                }
            }
            else if (role == Qt::DecorationRole)
            {
                if (col == 0)
                {
                    return gtApp->icon(QStringLiteral("pluginIcon_16.png"));
                }
                else if (col == 1)
                {
                    return gtApp->icon(QStringLiteral("infoBlueIcon_16.png"));
                }
            }
            else if (role == Qt::CheckStateRole && col == 0)
            {
                const int row = index.row();

                if (row < 0 || row >= m_updateAvailableItems.size())
                {
                    return QVariant();
                }

                if (m_updateAvailableItems[row]->isSelected())
                {
                    return Qt::Checked;
                }
                else
                {
                    return Qt::Unchecked;
                }
            }

            break;
        }

        case AvailableItem:
        {
            if (role == Qt::DisplayRole)
            {
                const int row = index.row();

                if (row < 0 || row >= m_availableItems.size())
                {
                    return QVariant();
                }

                if (col == 0)
                {
                    return m_availableItems[row]->ident();
                }
                else if (col == 2)
                {
                    return m_availableItems[row]->version();
                }
            }
            else if (role == Qt::DecorationRole)
            {
                if (col == 0)
                {
                    return gtApp->icon(QStringLiteral("pluginIcon_16.png"));
                }
                else if (col == 1)
                {
                    return gtApp->icon(QStringLiteral("infoBlueIcon_16.png"));
                }
            }
            else if (role == Qt::CheckStateRole && col == 0)
            {
                const int row = index.row();

                if (row < 0 || row >= m_availableItems.size())
                {
                    return QVariant();
                }

                if (m_availableItems[row]->isSelected())
                {
                    return Qt::Checked;
                }
                else
                {
                    return Qt::Unchecked;
                }
            }

            break;
        }

        case InstalledItem:
        {
            if (role == Qt::DisplayRole)
            {
                const int row = index.row();

                if (row < 0 || row >= m_installedItems.size())
                {
                    return QVariant();
                }

                if (col == 0)
                {
                    return m_installedItems[row]->ident();
                }
                else if (col == 2)
                {
                    return m_installedItems[row]->version();
                }
                else if (col == 3)
                {
                    return m_installedItems[row]->installedVersion();
                }
            }
            else if (role == Qt::DecorationRole)
            {
                if (col == 0)
                {
                    return gtApp->icon(QStringLiteral("pluginIcon_16.png"));
                }
                else if (col == 1)
                {
                    return gtApp->icon(QStringLiteral("infoBlueIcon_16.png"));
                }
            }
            else if (role == Qt::ForegroundRole)
            {
                return QColor(Qt::darkGray);
            }

            break;
        }
    }

    return QVariant();
}

bool
GtpyCollectionBrowserModel::setData(const QModelIndex& index,
                                    const QVariant& value, int role)
{
    if (!index.isValid())
    {
        return QAbstractItemModel::setData(index, value, role);
    }

    const int col = index.column();

    const CollectionItemType itemType = static_cast<CollectionItemType>
                                        (index.internalId());

    switch (itemType)
    {

        case UpdateAvailableItem:
        {
            if (role == Qt::CheckStateRole && col == 0)
            {
                const int row = index.row();

                if (row < 0 || row >= m_updateAvailableItems.size())
                {
                    return QAbstractItemModel::setData(index, value, role);
                }

                Qt::CheckState state =
                    static_cast<Qt::CheckState>(value.toInt());

                if (state == Qt::Checked)
                {
                    m_updateAvailableItems[row]->setSelected(true);
                }
                else
                {
                    m_updateAvailableItems[row]->setSelected(false);
                }

                emit dataChanged(index, index);
                emit selectionChanged();
            }

            break;
        }

        case AvailableItem:
        {
            if (role == Qt::CheckStateRole && col == 0)
            {
                const int row = index.row();

                if (row < 0 || row >= m_availableItems.size())
                {
                    return QAbstractItemModel::setData(index, value, role);
                }

                Qt::CheckState state =
                    static_cast<Qt::CheckState>(value.toInt());

                if (state == Qt::Checked)
                {
                    m_availableItems[row]->setSelected(true);
                }
                else
                {
                    m_availableItems[row]->setSelected(false);
                }

                emit dataChanged(index, index);
                emit selectionChanged();
            }

            break;
        }
    }

    return QAbstractItemModel::setData(index, value, role);
}

QVariant
GtpyCollectionBrowserModel::headerData(int section, Qt::Orientation orientation,
                                       int role) const
{
    if (orientation != Qt::Horizontal)
    {
        return QAbstractItemModel::headerData(section, orientation, role);
    }

    switch (role)
    {
        case Qt::DisplayRole:
            if (section == 0)
            {
                return tr("Name");
            }
            else if (section == 1)
            {
                return QString();
            }
            else if (section == 2)
            {
                return tr("Version");
            }
            else if (section == 3)
            {
                return tr("Installed Version");
            }

            break;

        default:
            break;
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

void
GtpyCollectionBrowserModel::setCollectionData(const
        QList<GtCollectionNetworkItem>& installedItems,
        const QList<GtCollectionNetworkItem>& availableItems,
        const QList<GtCollectionNetworkItem>& updataAvailableItems)
{
    beginResetModel();

    m_installedItems.clear();
    m_availableItems.clear();
    m_updateAvailableItems.clear();

    foreach (GtCollectionNetworkItem item, installedItems)
    {
        m_installedItems << new GtpyCollectionItem(item);
    }

    foreach (GtCollectionNetworkItem item, availableItems)
    {
        m_availableItems << new GtpyCollectionItem(item);
    }

    foreach (GtCollectionNetworkItem item, updataAvailableItems)
    {
        m_updateAvailableItems << new GtpyCollectionItem(item);
    }

    endResetModel();

    emit selectionChanged();
}

QModelIndex
GtpyCollectionBrowserModel::index(int row, int column,
                                  const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    if (!parent.isValid())
    {
        QList<GtpyCollectionBrowserModel::CollectionItemType> groups =
            groupTypes();

        if (row < 0 || row >= groups.size())
        {
            return QModelIndex();
        }

        return createIndex(row, column, groups[row]);
    }
    else
    {
        const CollectionItemType itemType =
            static_cast<CollectionItemType>(parent.internalId());

        if (itemType == UpdateAvailableItemGroup)
        {
            return createIndex(row, column, UpdateAvailableItem);
        }
        else if (itemType == AvailableItemGroup)
        {
            return createIndex(row, column, AvailableItem);
        }
        else if (itemType == InstalledItemGroup)
        {
            return createIndex(row, column, InstalledItem);
        }
        else
        {
            return QModelIndex();
        }
    }

    return QModelIndex();
}

QModelIndex
GtpyCollectionBrowserModel::parent(const QModelIndex& index) const
{
    const CollectionItemType itemType = static_cast<CollectionItemType>
                                        (index.internalId());

    switch (itemType)
    {
        case UpdateAvailableItemGroup:
        case AvailableItemGroup:
        case InstalledItemGroup:
        default:
            return QModelIndex();

        case UpdateAvailableItem:
            return createIndex(0, 0, UpdateAvailableItemGroup);

        case AvailableItem:
            return createIndex(1, 0, AvailableItemGroup);

        case InstalledItem:
            return createIndex(1, 0, InstalledItemGroup);
    }

    return QModelIndex();
}

Qt::ItemFlags
GtpyCollectionBrowserModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (index.isValid() && index.parent().isValid() && index.column() == 0)
    {
        const CollectionItemType itemType = static_cast<CollectionItemType>
                                            (index.internalId());

        switch (itemType)
        {
            case UpdateAvailableItem:
            case AvailableItem:
            {
                flags = flags | Qt::ItemIsUserCheckable;
            }

        }
    }

    return flags;
}

GtCollectionItem
GtpyCollectionBrowserModel::itemFromIndex(const QModelIndex& index)
{
    if (!index.isValid())
    {
        return GtCollectionItem();
    }

    if (index.model() != this)
    {
        return GtCollectionItem();
    }

    QModelIndex parent = index.parent();

    if (!parent.isValid())
    {
        return GtCollectionItem();
    }

    const int row = index.row();

    const CollectionItemType itemType = static_cast<CollectionItemType>
                                        (index.internalId());

    switch (itemType)
    {
        case UpdateAvailableItem:
        {
            if (row < 0 || row >= m_updateAvailableItems.size())
            {
                return GtCollectionItem();
            }

            return m_updateAvailableItems[row]->item();
        }

        case AvailableItem:
        {
            if (row < 0 || row >= m_availableItems.size())
            {
                return GtCollectionItem();
            }

            return m_availableItems[row]->item();
        }

        case InstalledItem:
        {
            if (row < 0 || row >= m_installedItems.size())
            {
                return GtCollectionItem();
            }

            return m_installedItems[row]->item();
        }
    }

    return GtCollectionItem();
}

QList<GtCollectionNetworkItem>
GtpyCollectionBrowserModel::selectedItems()
{
    QList<GtCollectionNetworkItem> retval;

    foreach (GtpyAbstractCollectionItem* modelItem, m_updateAvailableItems)
    {
        if (modelItem->isSelected())
        {
            retval << modelItem->item();
        }
    }

    foreach (GtpyAbstractCollectionItem* modelItem, m_availableItems)
    {
        if (modelItem->isSelected())
        {
            retval << modelItem->item();
        }
    }

    return retval;
}

QList<GtCollectionNetworkItem>
GtpyCollectionBrowserModel::itemsToUpdate()
{
    QList<GtCollectionNetworkItem> retval;

    foreach (GtpyAbstractCollectionItem* modelItem, m_updateAvailableItems)
    {
        retval << modelItem->item();
    }

    return retval;
}

void
GtpyCollectionBrowserModel::selectAll()
{
    beginResetModel();

    for (int i = 0; i < m_updateAvailableItems.size(); i++)
    {
        m_updateAvailableItems[i]->setSelected(true);
    }

    for (int i = 0; i < m_availableItems.size(); i++)
    {
        m_availableItems[i]->setSelected(true);
    }

    endResetModel();
}

void
GtpyCollectionBrowserModel::unselectAll()
{
    beginResetModel();

    for (int i = 0; i < m_updateAvailableItems.size(); i++)
    {
        m_updateAvailableItems[i]->setSelected(false);
    }

    for (int i = 0; i < m_availableItems.size(); i++)
    {
        m_availableItems[i]->setSelected(false);
    }

    endResetModel();
}

QList<GtpyCollectionBrowserModel::CollectionItemType>
GtpyCollectionBrowserModel::groupTypes() const
{
    QList<GtpyCollectionBrowserModel::CollectionItemType> retval;

    if (!m_updateAvailableItems.isEmpty())
    {
        retval << UpdateAvailableItemGroup;
    }

    if (!m_availableItems.isEmpty())
    {
        retval << AvailableItemGroup;
    }

    if (!m_installedItems.isEmpty())
    {
        retval << InstalledItemGroup;
    }

    return retval;
}

