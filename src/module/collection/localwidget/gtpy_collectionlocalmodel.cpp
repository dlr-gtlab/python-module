/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionlocalmodel.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 16.09.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QDir>

#include "gtpy_contextmanager.h"
#include "gtpy_constants.h"
#include "gtpy_icons_compat.h"
#include "gtpy_collapsiblelocalitem.h"
#include "gtpy_localitem.h"

#include "gtpy_collectionlocalmodel.h"

GtpyCollectionLocalModel::GtpyCollectionLocalModel(QObject* parent) :
    QAbstractItemModel(parent), m_rootItem(nullptr), m_showInfoColumns(true)
{
    m_rootItem = new GtpyCollapsibleLocalItem("Root");
}

GtpyCollectionLocalModel::~GtpyCollectionLocalModel()
{
    if (m_rootItem)
    {
        delete m_rootItem;
    }
}

int
GtpyCollectionLocalModel::rowCount(const QModelIndex& parent) const
{
    if (!m_rootItem)
    {
        return 0;
    }

    if (parent.column() > 0)
    {
        return 0;
    }

    return !parent.isValid() ? m_rootItem->childCount() :
                               indexToLocalItem(parent)->childCount();
}

int
GtpyCollectionLocalModel::columnCount(const QModelIndex& /*parent*/) const
{
    if (m_showInfoColumns)
    {
        return 3;
    }
    else
    {
        return 1;
    }
}

QVariant
GtpyCollectionLocalModel::data(const QModelIndex& index, int role) const
{
    if (!m_rootItem)
    {
        return {};
    }

    if (!index.isValid())
    {
        return {};
    }

    const int col = index.column();
    auto item = indexToLocalItem(index);

    if (item->isCollapsible())
    {
        switch (role)
        {
            case Qt::DisplayRole:
                if (col == 0)
                {
                    return item->ident();
                }

                break;

#if GT_VERSION < GT_VERSION_CHECK(2, 0, 0)
            case Qt::BackgroundRole:
                if (item->parentItem() == m_rootItem)
                {
                    return QColor(245, 245, 245);
                }

                return QColor(250, 250, 250);
#endif
            default:
                break;
        }
    }
    else
    {
        switch (role)
        {
            case Qt::DisplayRole:
                if (col == 0)
                {
                    return item->ident();
                }
                else if (col == 2)
                {
                    return item->version();
                }

                break;

            case Qt::ToolTipRole:
                return item->description();
                break;

            case Qt::DecorationRole:
                if (col == 0)
                {
                    return GTPY_ICON(property);
                }
                else if (col == 1)
                {
                    return GTPY_ICON(info2);
                }

                break;

            default:
                break;
        }
    }

    return {};
}

QVariant
GtpyCollectionLocalModel::headerData(int section, Qt::Orientation orientation,
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

            break;

        default:
            break;
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

void
GtpyCollectionLocalModel::setItems(const QList<GtCollectionItem>& items)
{
    beginResetModel();

    m_rootItem->deleteChildren();

    foreach (GtCollectionItem item, items)
    {
        QStringList hierarchy;

        auto cat = item.property(gtpy::constants::COLLECTION_CAT).toString();

        if (!cat.isEmpty())
        {
            hierarchy << cat;

            cat = item.property(gtpy::constants::COLLECTION_SUBCAT).toString();

            if (!cat.isEmpty())
            {
                hierarchy << cat;
            }
        }

        m_rootItem->appendChild(new GtpyLocalItem(item), hierarchy);
    }

    endResetModel();
}

QModelIndex
GtpyCollectionLocalModel::index(int row, int column,
                                const QModelIndex& parent) const
{
    if (!m_rootItem)
    {
        return {};
    }

    if (!hasIndex(row, column, parent))
    {
        return {};
    }

    auto childItem = !parent.isValid() ? m_rootItem->child(row) :
                                         indexToLocalItem(parent)->child(row);

    if (childItem)
    {
        if (childItem->childCount() > 0 || !childItem->isCollapsible())
        {
            return createIndex(row, column, childItem);
        }
    }

    return {};
}

QModelIndex
GtpyCollectionLocalModel::parent(const QModelIndex& index) const
{
    if (!m_rootItem)
    {
        return {};
    }

    if (!index.isValid())
    {
        return {};
    }

    auto parent = indexToLocalItem(index)->parentItem();
    return parent != m_rootItem ? createIndex(parent->row(), 0, parent) :
                                  QModelIndex{};
}

GtCollectionItem
GtpyCollectionLocalModel::itemFromIndex(const QModelIndex& index)
{
    if (!index.isValid())
    {
        return {};
    }

    if (index.model() != this)
    {
        return {};
    }

    QModelIndex parent = index.parent();

    if (!parent.isValid())
    {
        return {};
    }

    if (auto item = static_cast<GtpyLocalItem*>(index.internalPointer()))
    {
        if (!item->isCollapsible())
        {
            return item->item();
        }
    }

    return {};
}

bool
GtpyCollectionLocalModel::uninstallItem(const QModelIndex& index)
{
    if (!index.isValid())
    {
        return false;
    }

    if (index.model() != this)
    {
        return false;
    }

    auto item = static_cast<GtpyLocalItem*>(index.internalPointer());

    if (!item)
    {
        return false;
    }

    auto parentItem = static_cast<GtpyCollapsibleLocalItem*>(
                item->parentItem());

    if (!parentItem)
    {
        return false;
    }

    const int row = index.row();

    if (row < 0 || row >= parentItem->childCount())
    {
        return false;
    }

    QModelIndex parentIndex = index.parent();

    beginRemoveRows(parentIndex, row, row);

    QString uuid = item->item().uuid();

    bool success = false;

    if (parentItem->deleteChild(row))
    {
        item = nullptr;
        QString collectionPath = GtpyContextManager::collectionPath();

        if (!collectionPath.isEmpty())
        {
            if (!uuid.isEmpty())
            {
                QDir dir(collectionPath + QDir::separator() + uuid);

                if (dir.exists())
                {
                    success = dir.removeRecursively();
                }
            }
        }
    }

    endRemoveRows();

    if (success)
    {
        int childCount = parentItem->childCount();

        while (childCount == 0)
        {
            int parentRow = parentItem->row();

            parentItem = static_cast<GtpyCollapsibleLocalItem*>(
                             parentItem->parentItem());

            if (parentItem)
            {
                parentIndex = parentIndex.parent();

                beginRemoveRows(parentIndex, parentRow, parentRow);

                parentItem->deleteChild(parentRow);

                if (parentItem == m_rootItem)
                {
                    childCount = -1;
                }
                else
                {
                    childCount = parentItem->childCount();
                }

                endRemoveRows();
            }
            else
            {
                childCount = -1;
            }

        }
    }

    return success;
}

void
GtpyCollectionLocalModel::setShowInfoColumns(bool val)
{
    if (m_showInfoColumns == val)
    {
        return;
    }

    beginResetModel();
    m_showInfoColumns = val;
    endResetModel();
}

GtpyAbstractLocalItem*
GtpyCollectionLocalModel::indexToLocalItem(const QModelIndex& index) const
{
    return static_cast<GtpyAbstractLocalItem*>(index.internalPointer());
}

