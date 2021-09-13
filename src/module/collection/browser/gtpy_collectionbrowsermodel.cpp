/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionbrowsermodel.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QIcon>

#include "gt_application.h"

#include "gtpy_globals.h"
#include "gtpy_collectionitem.h"
#include "gtpy_collectioncollapsibleitem.h"
#include "gtpy_collectionrootitem.h"

#include "gtpy_collectionbrowsermodel.h"

GtpyCollectionBrowserModel::GtpyCollectionBrowserModel(QObject* parent) :
    QAbstractItemModel(parent)
{
    m_rootItem = new GtpyCollectionRootItem();
}

GtpyCollectionBrowserModel::~GtpyCollectionBrowserModel()
{
    if (m_rootItem)
    {
        delete m_rootItem;
    }
}

int
GtpyCollectionBrowserModel::rowCount(const QModelIndex& parent) const
{
    if (m_rootItem == Q_NULLPTR)
    {
        return 0;
    }

    GtpyAbstractCollectionItem* parentItem;

    if (parent.column() > 0)
    {
        return 0;
    }

    if (!parent.isValid())
    {
        parentItem = m_rootItem;
    }
    else
    {
        parentItem =
            static_cast<GtpyAbstractCollectionItem*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

int
GtpyCollectionBrowserModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 4;
}

QVariant
GtpyCollectionBrowserModel::data(const QModelIndex& index, int role) const
{
    if (m_rootItem == Q_NULLPTR)
    {
        return QVariant();
    }

    if (!index.isValid())
    {
        return QVariant();
    }

    const int col = index.column();

    GtpyAbstractCollectionItem* item = static_cast<GtpyAbstractCollectionItem*>
                                       (index.internalPointer());

    if (!item)
    {
        return QVariant();
    }

    if (item->isCollapsible())
    {
        if (role == Qt::DisplayRole && col == 0)
        {
            return item->ident();
        }
        else if (item == m_rootItem->child(MyUpdateAvailableItem))
        {
            if (role == Qt::DecorationRole && col == 0)
            {
                return gtApp->icon(QStringLiteral("updateIcon_16.png"));
            }
            else if (role == Qt::BackgroundRole)
            {
                return QColor(180, 229, 190);
            }
        }
        else if (item == m_rootItem->child(MyAvailableItem))
        {
            if (role == Qt::DecorationRole && col == 0)
            {
                return gtApp->icon(QStringLiteral("stackIcon.png"));
            }
        }
        else if (item == m_rootItem->child(MyInstalledItem))
        {
            if (role == Qt::DecorationRole && col == 0)
            {
                return gtApp->icon(QStringLiteral("collectionIcon_16.png"));
            }
            else if (role == Qt::BackgroundRole)
            {
                return QColor(240, 240, 240);
            }
        }
    }
    else
    {
        GtpyCollectionItemType type = static_cast<GtpyCollectionItemType>(
                                          item->typeId());

        switch (type)
        {
            case MyUpdateAvailableItem:
            {
                if (role == Qt::DisplayRole)
                {
                    if (col == 0)
                    {
                        return item->ident();
                    }
                    else if (col == 2)
                    {
                        return item->version();
                    }
                    else if (col == 3)
                    {
                        return item->installedVersion();
                    }
                }
                else if (role == Qt::DecorationRole)
                {
                    if (col == 0)
                    {
                        return gtApp->icon(
                                   QStringLiteral("pluginIcon_16.png"));
                    }
                    else if (col == 1)
                    {
                        return gtApp->icon(
                                   QStringLiteral("infoBlueIcon_16.png"));
                    }
                }
                else if (role == Qt::CheckStateRole && col == 0)
                {
                    if (item->isSelected())
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

            case MyAvailableItem:
            {
                if (role == Qt::DisplayRole)
                {
                    if (col == 0)
                    {
                        return item->ident();
                    }
                    else if (col == 2)
                    {
                        return item->version();
                    }
                }
                else if (role == Qt::DecorationRole)
                {
                    if (col == 0)
                    {
                        return gtApp->icon(
                                   QStringLiteral("pluginIcon_16.png"));
                    }
                    else if (col == 1)
                    {
                        return gtApp->icon(
                                   QStringLiteral("infoBlueIcon_16.png"));
                    }
                }
                else if (role == Qt::CheckStateRole && col == 0)
                {

                    if (item->isSelected())
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

            case MyInstalledItem:
            {
                if (role == Qt::DisplayRole)
                {
                    if (col == 0)
                    {
                        return item->ident();
                    }
                    else if (col == 2)
                    {
                        return item->version();
                    }
                    else if (col == 3)
                    {
                        return item->installedVersion();
                    }
                }
                else if (role == Qt::DecorationRole)
                {
                    if (col == 0)
                    {
                        return gtApp->icon(
                                   QStringLiteral("pluginIcon_16.png"));
                    }
                    else if (col == 1)
                    {
                        return gtApp->icon(
                                   QStringLiteral("infoBlueIcon_16.png"));
                    }
                }
                else if (role == Qt::ForegroundRole)
                {
                    return QColor(Qt::darkGray);
                }

                break;
            }

            default:
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

    GtpyAbstractCollectionItem* item = static_cast<GtpyAbstractCollectionItem*>
                                       (index.internalPointer());

    if (!item->isCollapsible())
    {
        GtpyCollectionItemType type = static_cast<GtpyCollectionItemType>(
                                          item->typeId());

        switch (type)
        {
            case MyUpdateAvailableItem:
            {
                if (role == Qt::CheckStateRole && col == 0)
                {

                    Qt::CheckState state =
                        static_cast<Qt::CheckState>(value.toInt());

                    if (state == Qt::Checked)
                    {
                        item->setSelected(true);
                    }
                    else
                    {
                        item->setSelected(false);
                    }

                    emit dataChanged(index, index);
                    emit selectionChanged();
                }

                break;
            }

            case MyAvailableItem:
            {
                if (role == Qt::CheckStateRole && col == 0)
                {
                    Qt::CheckState state =
                        static_cast<Qt::CheckState>(value.toInt());

                    if (state == Qt::Checked)
                    {
                        item->setSelected(true);
                    }
                    else
                    {
                        item->setSelected(false);
                    }

                    emit dataChanged(index, index);
                    emit selectionChanged();
                }

                break;
            }

            default:
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

    m_rootItem->clearRoot();

    if (!installedItems.isEmpty())
    {
        m_rootItem->createChild(MyInstalledItem, tr("Installed"));
    }

    if (!availableItems.isEmpty())
    {
        m_rootItem->createChild(MyAvailableItem, tr("Available"));
    }

    if (!updataAvailableItems.isEmpty())
    {
        m_rootItem->createChild(MyUpdateAvailableItem, tr("Update available!"));
    }

    foreach (GtCollectionNetworkItem item, installedItems)
    {
        appendItemTo(item, m_rootItem->child(MyInstalledItem));
    }

    foreach (GtCollectionNetworkItem item, availableItems)
    {
        appendItemTo(item, m_rootItem->child(MyAvailableItem));
    }

    foreach (GtCollectionNetworkItem item, updataAvailableItems)
    {
        appendItemTo(item, m_rootItem->child(MyUpdateAvailableItem));
    }

    endResetModel();

    emit selectionChanged();
}

QModelIndex
GtpyCollectionBrowserModel::index(int row, int column,
                                  const QModelIndex& parent) const
{
    if (m_rootItem == Q_NULLPTR)
    {
        return QModelIndex();
    }

    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    GtpyAbstractCollectionItem* parentItem;

    if (!parent.isValid())
    {
        parentItem = m_rootItem;
    }
    else
    {
        parentItem = static_cast<GtpyAbstractCollectionItem*>(
                         parent.internalPointer());
    }

    GtpyAbstractCollectionItem* childItem = parentItem->child(row);

    if (childItem)
    {
        if (childItem->childCount() > 0 || !childItem->isCollapsible())
        {
            return createIndex(row, column, childItem);
        }
    }

    return QModelIndex();
}

QModelIndex
GtpyCollectionBrowserModel::parent(const QModelIndex& index) const
{
    if (m_rootItem == Q_NULLPTR)
    {
        return QModelIndex();
    }

    if (!index.isValid())
    {
        return QModelIndex();
    }

    GtpyAbstractCollectionItem* childItem =
        static_cast<GtpyAbstractCollectionItem*>(index.internalPointer());

    GtpyAbstractCollectionItem* parentItem = childItem->parentItem();

    if (parentItem == m_rootItem)
    {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

Qt::ItemFlags
GtpyCollectionBrowserModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (index.isValid() && index.parent().isValid() && index.column() == 0)
    {
        GtpyAbstractCollectionItem* item =
            static_cast<GtpyAbstractCollectionItem*>(index.internalPointer());

        bool collapsible = item->isCollapsible();
        GtpyCollectionItemType type = static_cast<GtpyCollectionItemType>(
                                          item->typeId());

        if (!collapsible)
        {
            switch (type)
            {
                case MyUpdateAvailableItem:
                case MyAvailableItem:
                {
                    flags = flags | Qt::ItemIsUserCheckable;
                    break;
                }

                default:
                    break;
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

    GtpyAbstractCollectionItem* item = static_cast<GtpyAbstractCollectionItem*>
                                       (index.internalPointer());

    if (!item)
    {
        return GtCollectionItem();
    }

    if (item->isCollapsible())
    {
        return GtCollectionItem();
    }

    return item->item();
}

QList<GtCollectionNetworkItem>
GtpyCollectionBrowserModel::selectedItems()
{
    QList<GtCollectionNetworkItem> retval;

    GtpyCollectionCollapsibleItem* item = m_rootItem->child(
            MyUpdateAvailableItem);

    if (item)
    {
        retval.append(item->selectedItems());
    }

    item = m_rootItem->child(MyAvailableItem);

    if (item)
    {
        retval.append(item->selectedItems());
    }

    return retval;
}

QList<GtCollectionNetworkItem>
GtpyCollectionBrowserModel::itemsToUpdate()
{
    QList<GtCollectionNetworkItem> retval;

    GtpyCollectionCollapsibleItem* item = m_rootItem->child(
            MyUpdateAvailableItem);

    if (item)
    {
        retval.append(item->uncollapsibleChilren());
    }

    return retval;
}

void
GtpyCollectionBrowserModel::selectAll()
{
    beginResetModel();

    GtpyCollectionCollapsibleItem* item = m_rootItem->child(
            MyUpdateAvailableItem);

    if (item)
    {
        item->selectAllChildren();
    }

    item = m_rootItem->child(MyAvailableItem);

    if (item)
    {
        item->selectAllChildren();
    }

    endResetModel();

    emit selectionChanged();
}

void
GtpyCollectionBrowserModel::unselectAll()
{
    beginResetModel();

    GtpyCollectionCollapsibleItem* item = m_rootItem->child(
            MyUpdateAvailableItem);

    if (item)
    {
        item->unselectAllChildren();
    }

    item = m_rootItem->child(MyAvailableItem);

    if (item)
    {
        item->unselectAllChildren();
    }

    endResetModel();
}

void
GtpyCollectionBrowserModel::appendItemTo(GtCollectionNetworkItem item,
        GtpyCollectionCollapsibleItem* to)
{
    QStringList hierarchy;

    QString cat = item.property(GtpyGlobals::COLLECTION_cat).toString();

    if (!cat.isEmpty())
    {
        hierarchy << cat;

        cat = item.property(GtpyGlobals::COLLECTION_subcat).toString();

        if (!cat.isEmpty())
        {
            hierarchy << cat;
        }
    }

    to->appendChild(new GtpyCollectionItem(item), hierarchy);
}
